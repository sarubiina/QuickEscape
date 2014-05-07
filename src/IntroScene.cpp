#include <IntroScene.h>
#include <SDL.h>
#include <SDL_image.h>
#include <stdexcept>
#include <Game.h>
using namespace std;

void 
IntroScene::Init(SDL_Renderer * renderer)
{
	splash_ = IMG_LoadTexture(renderer, "res/splash.bmp");
	if (splash_ == NULL)
	{
		throw runtime_error(IMG_GetError());
	}
	SDL_SetTextureBlendMode(splash_, SDL_BLENDMODE_BLEND);
	alpha = 255;
	timer = 0;
}
void 
IntroScene::Update(float seconds)
{
	timer += seconds;

	//Fade out splash texture
	if (timer > 1) 
	{
		alpha -= seconds * 130;
		if (alpha > 0) 
		{
			SDL_SetTextureAlphaMod(splash_, alpha);
		}
	}
}
void 
IntroScene::Render(SDL_Renderer * renderer)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, splash_, NULL, NULL);
}
void 
IntroScene::OnEvent(SDL_Event & ev)
{
	switch (ev.type)
	{
	case SDL_QUIT:
		Game::GetInstance()->SetProperty("running", false);
		break;
	default:
		break;
	}
}