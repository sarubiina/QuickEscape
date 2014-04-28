#include <SDLApp.h>
#include <SDL.h>
#include <SDL_image.h>
#include <stdexcept>
#include <Game.h>
using namespace std;

SDLApp::SDLApp()
{
	//Initialize all SDL subsystems
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		throw runtime_error(SDL_GetError());

	//initialize SDL Image 
	int initFlags = IMG_INIT_JPG | IMG_INIT_PNG;
	int retFlags = IMG_Init(initFlags);

	if (retFlags != initFlags)
	{
		throw runtime_error(IMG_GetError());
	}

	//make pointers safe
	window_ = NULL;
	renderer_ = NULL;
}

SDLApp::~SDLApp()
{
	SDL_DestroyWindow(window_);
	SDL_DestroyRenderer(renderer_);
	IMG_Quit();
	SDL_Quit();
}

void 
SDLApp::Init(const std::string & title, int width, int height, int flags)
{
	//open SDL graphics window
	window_ = SDL_CreateWindow(title.c_str(),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width, height,
		flags);
	//sanity check for window
	if (window_ == NULL)
		throw runtime_error(SDL_GetError());
	
	//create renderer
	renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_SOFTWARE);
	//sanity check
	if (renderer_ == NULL)
		throw runtime_error(SDL_GetError());
}

void 
SDLApp::Render(SDL_Texture *tex, SDL_Rect src, SDL_Rect dst)
{
	//Draw the textures
	SDL_RenderCopy(renderer_, tex, &src, &dst);
}

void
SDLApp::HandleInput()
{
	SDL_Event ev;
	while (SDL_PollEvent(&ev)) 
	{
		switch (ev.type)
		{
		case SDL_QUIT:
		case SDL_KEYDOWN: 
				Command *pCmd = CommandUtils::Parse(ev);
				pCmd->Execute(*Game::GetInstance());
				delete pCmd;
				break;
		}
	}
	
}