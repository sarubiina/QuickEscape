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
	
	pages_ = IMG_LoadTexture(renderer, "res/pages.png");
	if (pages_ == NULL)
		throw runtime_error(SDL_GetError());
	
	bookcover_ = IMG_LoadTexture(renderer, "res/cover.png");
	if (bookcover_ == NULL)
		throw runtime_error(SDL_GetError());
	
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
	pos_.x /= 2;
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
	*page_ << "hello world";
	page_->SetDirty(true);
	alpha_ = 255.0F;
	fading_ = true;
}
////////////////////////////////////////
GameScene::~GameScene()
{
	SDL_DestroyTexture(bookcover_);
	SDL_DestroyTexture(pages_);
}
////////////////////////////////////////
void
GameScene::Render(SDL_Renderer *  renderer)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	SDL_RenderCopy(renderer, bookcover_, NULL, NULL);
	SDL_RenderCopy(renderer, pages_, NULL, NULL);
	SDL_RenderCopy(renderer, character_, &player_, &pos_);

	if (page_->IsDirty())
	{
		page_->Compose(fonts_["text"]);
		page_->RenderContent(renderer);
		page_->SetDirty(false);
	}
	SDL_Rect d = { 50, 50, 390, 570 };
	SDL_RenderCopy(renderer, page_->_pageTexture, NULL, &d);
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
}
////////////////////////////////////////
void
GameScene::OnEvent(SDL_Event & ev) 
{
	switch (ev.type) 
	{
	case SDL_QUIT:
		Game::GetInstance()->GetProperty("running").SetValue(false);
		Mix_FadeOutMusic(3000);
		break;
	case SDL_KEYDOWN:
		Command *pCmd = CommandUtils::Parse(ev);
		pCmd->Execute(*this);
		delete pCmd;
		break;
	}
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
