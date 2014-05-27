#include "GameScene.h"
#include "Game.h"
#include <SDL_image.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_ttf.h>
#include <stdexcept>
#include <tinyxml2.h>
#include <map>
#include <globals.h>
#include <Logger.h>
#include <Room.h>

using namespace std;
using namespace tinyxml2;
extern map<XMLError, string> g_TinyXmlErrors;
////////////////////////////////////////
void
GameScene::Init(SDL_Renderer * renderer) 
{
	// Create book cover, pages and player textures and rects for them
	bookcover_ = IMG_LoadTexture(renderer, "res/cover.png");
	if (bookcover_ == NULL)
		throw runtime_error(IMG_GetError());

	for (int i = 0; i < NUM_PAGE_ANIM_FRAMES; i++)
	{
		ostringstream ss;
		ss << "res/book/page0" << i + 1 << ".png";
		pages_[i] = IMG_LoadTexture(renderer, ss.str().c_str());
		if (pages_[i] == NULL)
			throw runtime_error(IMG_GetError());
	}

	character_ = IMG_LoadTexture(renderer, "res/player0.png");
	if (character_ == NULL) 
		throw runtime_error(IMG_GetError());

	// Create source Rect for player
	player_.x = 112;
	player_.y = 16;
	player_.w = 16;
	player_.h = 16;

	// set players destination vectors size and position
	pos_ = player_;

	SDL_GetWindowSize(Game::GetInstance()->GetWindow(), &pos_.x, &pos_.y);
	pos_.x = 800;
	pos_.y /= 2;
	pos_.w *= 2;
	pos_.h *= 2;

	//load fonts
	XMLDocument fontDoc;
	fontDoc.LoadFile("res/fonts.xml");
	if (fontDoc.ErrorID() != XML_NO_ERROR)
	{
		ostringstream ss;
		ss << "Font loadfile:" 
			<< g_TinyXmlErrors[fontDoc.ErrorID()]
			<< " " << fontDoc.GetErrorStr1()
			<< " " << fontDoc.GetErrorStr2();
		throw XmlParsingException(ss.str());
	}
	XMLElement *pElem = fontDoc.FirstChildElement("Fonts");
	if (!pElem) throw XmlParsingException("no fonts declaration found");
	pElem = pElem->FirstChildElement("Font");
	while (pElem != NULL)
	{
		const char * name = pElem->Attribute("name");
		const char * size = pElem->Attribute("size");
		const char * path = pElem->Attribute("path");
		if (!name) throw AttributeMissingException("name missing from font");
		if (!size) throw AttributeMissingException("size missing from font");
		if (!path) throw AttributeMissingException("path missing from font");
		
		g_Log << "loading font" << name << "\n";
		//change size to int
		TTF_Font *tmp = NULL; 
		stringstream ss(size);
		int fontsize;
		if (!(ss >> fontsize)) throw runtime_error("cannot convert from font size to int");
		
		tmp = TTF_OpenFont(path, fontsize);
		if (tmp == NULL) throw runtime_error(TTF_GetError());
		fonts_[name] = tmp;

		pElem = pElem->NextSiblingElement("Font");
	}

	//using page class
	page_ = new Page();
	page_->fillMode_ = FillMode::Tail;
	page_->_justification = Justification::Left;
	page_->Init(renderer, 412, 612); //size

	titlePage_ = new Page();
	titlePage_->fillMode_ = FillMode::Head;
	titlePage_->_justification = Justification::Center;
	titlePage_->Init(renderer, 412, 612); //size
	*titlePage_ << "Quick Escape by Anssi Grohn";
	titlePage_->SetDirty(true);

	endPage_ = new Page();
	endPage_->fillMode_ = FillMode::Head;
	endPage_->_justification = Justification::Center;
	endPage_->Init(renderer, 412, 612); //size
	*endPage_ << "The End - Leave or finnish the story?";
	endPage_->SetDirty(true);

	alpha_ = 255.0F;
	fading_ = true;
}
////////////////////////////////////////
GameScene::~GameScene()
{
	SDL_DestroyTexture(bookcover_);
	for (auto & e : pages_)
	{
		SDL_DestroyTexture(e);
	}
	SDL_DestroyTexture(character_);
	for (auto & e : fonts_)
	{
		TTF_CloseFont(e.second);
	}
}
////////////////////////////////////////
void
GameScene::Render(SDL_Renderer *  renderer)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	SDL_RenderCopy(renderer, bookcover_, NULL, NULL);

	SDL_Rect pageDest{ 30, 45, 940, 630 };
	SDL_RenderCopy(renderer, pages_[(int)pageFrame_], NULL, &pageDest);

	SDL_Rect d = { 50, 50, 390, 570 };
	

	if (page_->IsDirty())
	{
		page_->Compose(fonts_["text"]);
		page_->RenderContent(renderer);
		page_->SetDirty(false);
	}
	if (titlePage_->IsDirty())
	{
		titlePage_->Compose(fonts_["title"]);
		titlePage_->RenderContent(renderer);
		titlePage_->SetDirty(false);
	}
	if (endPage_->IsDirty())
	{
		endPage_->Compose(fonts_["special"]);
		endPage_->RenderContent(renderer);
		endPage_->SetDirty(false);
	}
	if (pageToShow_ == 0)
	{
		SDL_RenderCopy(renderer, titlePage_->_pageTexture, NULL, &d);
	}
	else if (pageToShow_ == 1)
	{
		SDL_RenderCopy(renderer, page_->_pageTexture, NULL, &d);
		SDL_RenderCopy(renderer, character_, &player_, &pos_);
	}
	else if (pageToShow_ == 2)
	{
		SDL_RenderCopy(renderer, endPage_->_pageTexture, NULL, &d);
	}
	if (fading_)
	{
		int h, w;
		SDL_GetWindowSize(Game::GetInstance()->GetWindow(), &w, &h);
		boxRGBA(renderer, 0, 0, w, h, 255, 255, 255, (int) alpha_);
	}
}
////////////////////////////////////////
void
GameScene::Update(float seconds)
{
	//fade implemention
	const float ALPHA_VALUE_STEP = 128.0f;
	if (fading_)
	{
		alpha_ -= seconds *ALPHA_VALUE_STEP;
		//when alpha value is completely white
		if (alpha_ <= 0.001f) fading_ = false;
	}
	if (pageTurning_)
	{
		pageFrame_ += seconds*2.5f;
		Mix_PlayChannel(0, Game::GetInstance()->sound_["flippage"], 0);
		if (pageFrame_ > 5.0f)
		{
			pageTurning_ = false;
			pageFrame_ = 0.0f;
			pageToShow_++;
		}
	}
	if (pageTurningLeft_)
	{
		pageFrame_ -= seconds*2.5f;
		Mix_PlayChannel(0, Game::GetInstance()->sound_["flippage"], 0);
		if (pageFrame_ < 0.0f)
		{
			pageTurningLeft_ = false;
			pageTurning_ = false;
			pageFrame_ = 0.0f;
			pageToShow_--;
		}
	}
}
////////////////////////////////////////
void
GameScene::OnEvent(SDL_Event & ev) 
{
	if (ev.type == SDL_QUIT)
	{
		Game::GetInstance()->GetProperty("running").SetValue(false);
		Mix_FadeOutMusic(3000);
	}
	if(ev.type == SDL_KEYDOWN)
	{
		if (ev.key.keysym.sym == SDLK_SPACE)
		{
			pageTurning_ = true;
		}
		if (ev.key.keysym.sym == SDLK_b)
		{
			pageTurningLeft_ = true;
			pageFrame_ = 5;
		}
	}
	Command *pCmd = CommandUtils::Parse(ev);
	pCmd->Execute(*this);
	delete pCmd;
}
////////////////////////////////////////////////////////////////////////////////
void
GameScene::Execute(QuitCommand & cmd)
{
	Game::GetInstance()->SetProperty("running", false);
}
////////////////////////////////////////////////////////////////////////////////
void
GameScene::Execute(MoveCommand & cmd)
{
	if (cmd.m_Dir == kNumDirs)
	{
		*page_ << "You want to move ... where?\n";
	}
	else
	{
		Room *pNext = Game::GetInstance()->GetCurrentRoom()->GetNextRoom(cmd.m_Dir);
		if (pNext)
		{
			// Handle move commands in both rooms
			Game::GetInstance()->GetCurrentRoom()->Execute(cmd);
			Game::GetInstance()->SetCurrentRoom(pNext);
			Game::GetInstance()->GetCurrentRoom()->Execute(cmd);

			//Player moves on screen
			switch (cmd.m_Dir)
			{
			case North:
				pos_.y -= pos_.h;
				break;
			case South:
				pos_.y += pos_.h;
				break;
			case East:
				pos_.x += pos_.w;
				break;
			case West:
				pos_.x -= pos_.w;
				break;
			default:
				break;
			}
			Mix_PlayChannel(-1, Game::GetInstance()->sound_["step"], 0);
		}
		else
		{
			*page_ << "You bump your head on the wall. You can't go that way.\n";
			Mix_PlayChannel(-1, Game::GetInstance()->sound_["wall"], 0);
		}
	}
	page_->SetDirty(true);
}
////////////////////////////////////////////////////////////////////////////////
void
GameScene::Execute(UnknownCommand & cmd)
{
	*page_ << "Sorry, I did not quite get that.\n";
	page_->SetDirty(true);
}
////////////////////////////////////////////////////////////////////////////////
void
GameScene::Execute(TakeCommand & cmd)
{
	Game::GetInstance()->GetCurrentRoom()->Execute(cmd);
}
////////////////////////////////////////////////////////////////////////////////
void
GameScene::Execute(DropCommand & cmd)
{
	Game::GetInstance()->GetCurrentRoom()->Execute(cmd);
}
////////////////////////////////////////////////////////////////////////////////
void
GameScene::Execute(InventoryCommand & cmd)
{
	Game::GetInstance()->GetPlayer().Execute(cmd);
}
////////////////////////////////////////////////////////////////////////////////
void
GameScene::Execute(LookCommand & cmd)
{
	Game::GetInstance()->GetCurrentRoom()->Execute(cmd);
}
////////////////////////////////////////////////////////////////////////////////
void
GameScene::Execute(ExamineCommand & cmd)
{
	try
	{
		Game::GetInstance()->GetPlayer().Execute(cmd);
	}
	catch (ExamineCommandFailOnPlayerException & ex)
	{
		Game::GetInstance()->GetCurrentRoom()->Execute(cmd);
	}

}
////////////////////////////////////////////////////////////////////////////////
void
GameScene::Execute(UseCommand & cmd)
{
	// try player's inventory use first
	try {
		Game::GetInstance()->GetPlayer().Execute(cmd);
	}
	catch (UseCommandFailOnPlayerException & ex)
	{
		// no go, try from room instead.
		Game::GetInstance()->GetCurrentRoom()->Execute(cmd);
	}
}
////////////////////////////////////////////////////////////////////////////////
void
GameScene::Execute(NullCommand & cmd)
{

}
////////////////////////////////////////////////////////////////////////////////
