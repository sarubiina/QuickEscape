#include "Game.h"
#include <iostream>
#include <string>
#include <SDL.h>
////////////////////////////////////////////////////////////////////////////////
using namespace std;
////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
/////////////////////////SDL RELATED START/////////////////////////
	int splashWidth;
	int splashHeight;

//Initialize all SDL subsystems
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		cout << "SDL_Init Error: " << SDL_GetError() << endl;
		return 1;
	}


	//Load splash image into surface
	SDL_Surface *splash = SDL_LoadBMP("res/splash.bmp");
	if (splash == nullptr)
	{
		cout << "SDL_LoadBMP Error: " << SDL_GetError() << endl;
		return 1;
	}

//Save splash images width and height into variables so they can be used
	splashWidth = splash->w;
	splashHeight = splash->h;

//Create a window, which size is the image's dimensions
	SDL_Window *win = SDL_CreateWindow("QuickEscape", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		splashWidth, splashHeight, SDL_WINDOW_SHOWN);
	if (win == nullptr)
	{
		cout << "SDL_CreateWindow Error: " << SDL_GetError() << endl;
		return 1;
	}

//Create a renderer
	SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED |
		SDL_RENDERER_PRESENTVSYNC);
	if (ren == nullptr)
	{
		cout << "SDL_CreateRenderer Error: " << SDL_GetError() << endl;
		return 1;
	}

//Create texture from the surface
	SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, splash);
		SDL_FreeSurface(splash);
	if (tex == nullptr)
	{
		cout << "SDL_CreateTextureFromSurface Error" << SDL_GetError() << endl;
		return 1;
	}

//Draw the splash texture
	SDL_RenderClear(ren);
	SDL_RenderCopy(ren, tex, NULL, NULL);
	SDL_RenderPresent(ren);
//Display screen for 3 seconds
	SDL_Delay(3000);

//Purge the used SDL assets
	SDL_DestroyTexture(tex);
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
/////////////////////////SDL RELATED END/////////////////////////

  Game & g = *Game::GetInstance();
  g.Play();

  return 0;
}
////////////////////////////////////////////////////////////////////////////////