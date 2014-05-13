#include "GameScene.h"
#include "Game.h"
#include <SDL_image.h>
#include <stdexcept>
using namespace std;
using namespace tinyxml2;
void
GameScene::Init(SDL_Renderer * renderer) {
	// Create book cover, pages and player textures and rects for them
	player_ = IMG_LoadTexture(renderer, "res/player0.png");
	if (player_ == NULL) {
		throw runtime_error(IMG_GetError());
	}

	covers_ = IMG_LoadTexture(renderer, "res/cover.png");
	if (covers_ == NULL){
		throw runtime_error(SDL_GetError());
	}

	pages_ = IMG_LoadTexture(renderer, "res/pages.png");
	if (pages_ == NULL){
		throw runtime_error(SDL_GetError());
	}

	renderToTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET, 500, 700);
	if (renderToTexture == NULL) {
		throw runtime_error(SDL_GetError());
	}

	

	XMLDocument doc;
	doc.LoadFile("res/font.xml");
	if (doc.ErrorID() != XML_NO_ERROR)
	{
		ostringstream ss;
		ss << "Error relating to XML file font.xml";
		throw XmlParsingException(ss.str());

	}

	if (TTF_Init() < 0) {
		throw runtime_error(TTF_GetError());
	}

	// Read font.xml and assign values from it
	fontName = "OldStyle1";
	XMLElement *elem = doc.FirstChildElement("font");
	string temp;
	while (elem != NULL) {
		if (elem->Attribute("name")) {
			temp = elem->Attribute("name");
			if (temp.compare(fontName) == 0) {
				fontPath = elem->Attribute("path");
				temp = elem->Attribute("size");
				fontSize = atoi(temp.c_str());
				break;
			}
		}
		elem = elem->NextSiblingElement("font");
	}

	font = NULL;
	// Load font
	font = TTF_OpenFont(fontPath, fontSize);
	if (font == NULL) {
		throw runtime_error(TTF_GetError());
	}

	leftPage.Init(renderer, 460, 620);
	leftPage._justification = Justification::Left;

	//Set alpha blending for all textures
	alpha = 255;
	SDL_SetTextureBlendMode(covers_, SDL_BLENDMODE_BLEND);
	SDL_SetTextureBlendMode(pages_, SDL_BLENDMODE_BLEND);
	SDL_SetTextureBlendMode(player_, SDL_BLENDMODE_BLEND);
	SDL_SetTextureBlendMode(leftPage._pageTexture, SDL_BLENDMODE_BLEND);
	SDL_SetTextureBlendMode(renderToTexture, SDL_BLENDMODE_BLEND);

	SDL_SetTextureAlphaMod(pages_, alpha);
	SDL_SetTextureAlphaMod(covers_, alpha);
	SDL_SetTextureAlphaMod(player_, alpha);
	SDL_SetTextureAlphaMod(leftPage._pageTexture, alpha);


	int pageW, pageH;
	SDL_QueryTexture(pages_, NULL, NULL, &pageW, &pageH);
	pageRect.w = pageW;
	pageRect.h = pageH;
	pageRect.x = 0;
	pageRect.y = 0;

	int coverW, coverH;
	SDL_QueryTexture(covers_, NULL, NULL, &coverW, &coverH);
	coverRect.w = coverW;
	coverRect.h = coverH;
	coverRect.x = 0;
	coverRect.y = 0;

	// Create source Rect for player
	int playerW, playerH;
	SDL_QueryTexture(player_, NULL, NULL, &playerW, &playerH);
	srcPlayerRect.w = 16;
	srcPlayerRect.h = 16;
	srcPlayerRect.x = 112;
	srcPlayerRect.y = 16;

	// set players destination vectors size and position
	Game::GetInstance()->playerRect.w = srcPlayerRect.w * 2;
	Game::GetInstance()->playerRect.h = srcPlayerRect.h * 2;
	Game::GetInstance()->playerRect.x = 750;
	Game::GetInstance()->playerRect.y = 350;
	text = "Hi everybody!\n";
	leftPage << text;
	
	timer = 0;
}

void
GameScene::Render(SDL_Renderer *  renderer) {

	leftPage.Compose(font);
	leftPage.RenderContent(renderer);
	
	//Render text to renderToTexture
	SDL_SetRenderTarget(renderer, renderToTexture);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
	SDL_RenderClear(renderer);
	SDL_Rect dst = { pageRect.x + 30, pageRect.y + 40, 0, 0 };
	SDL_QueryTexture(leftPage._pageTexture, NULL, NULL, &dst.w, &dst.h);
	SDL_RenderCopy(renderer, leftPage._pageTexture, NULL, &dst);

	//Render textures
	SDL_SetRenderTarget(renderer, NULL);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	SDL_RenderCopy(renderer, covers_, NULL, &coverRect);
	SDL_RenderCopy(renderer, pages_, NULL, &pageRect);
	dst = pageRect;
	dst.w = pageRect.w / 2;
	SDL_RenderCopy(renderer, renderToTexture, NULL, &dst);
	SDL_RenderCopy(renderer, player_, &srcPlayerRect, &Game::GetInstance()->playerRect);

	// Render white box on top of everything that fades out
	boxRGBA(renderer, 1000, 700, 0, 0, 255, 255, 255, alpha);

}

void
GameScene::Update(float seconds) {

	timer += seconds;

	// Fade in scene's textures
	if (timer > 0.5f) {
		if (alpha > 0) {
			alpha -= seconds * 150;
		}
		else {
			alpha = 0;
		}
	}

	while (timer > 4) {
	
		leftPage << "Oh, hullo!\n";
		timer = 0;
	}
	
}

void
GameScene::OnEvent(SDL_Event & ev) {
	switch (ev.type) {
	case SDL_QUIT:
		Game::GetInstance()->GetProperty("running").SetValue(false);
		break;
	case SDL_KEYDOWN: case SDL_CONTROLLERBUTTONDOWN:
		Command *pCmd = CommandUtils::Parse(ev);
		pCmd->Execute(*Game::GetInstance());
		delete pCmd;
		break;
	}
}