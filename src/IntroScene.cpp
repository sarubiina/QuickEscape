#include "IntroScene.h"
#include "Game.h"
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_image.h>
#include <stdexcept>

using namespace std;
///////////////////////////////////////////////
IntroScene::~IntroScene() 
{
	SDL_DestroyTexture(splash_);
}
void
IntroScene::Init(SDL_Renderer * renderer) 
{
	splash_ = IMG_LoadTexture(renderer, "res/splash.bmp");
	if (splash_ == NULL) {
		throw runtime_error(IMG_GetError());
	}
	alpha_ = 0.0f; //how transparent
	fading_ = false;
}

void
IntroScene::Render(SDL_Renderer *  renderer) 
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, splash_, NULL, NULL);
	if (fading_)
	{
		int h, w;
		SDL_GetWindowSize(Game::GetInstance()->GetWindow(), &w, &h);
		boxRGBA(renderer, 0, 0, w, h, 0, 0, 0, (int) alpha_);
	}
}

void
IntroScene::Update(float seconds) 
{
	//fade implemention
	const float ALPHA_VALUE_STEP = 128.0f;
	if (fading_)
	{
		alpha_ += seconds *ALPHA_VALUE_STEP;
		//when alpha value is completely black
		if (alpha_ >= 255.0f)
		{
			Game::GetInstance()->SetCurrentScene("gamescene");
		}
	}
}

void
IntroScene::OnEvent(SDL_Event & ev) 
{
	switch (ev.type) {
	case SDL_QUIT:
		Game::GetInstance()->GetProperty("running").SetValue(false);
		break;
	case SDL_KEYDOWN:
		fading_ = true;
	default:
		break;
	}
}