#ifndef __SDLApp_h__
#define __SDLApp_h__
#include <SDL.h>
#include <Command.h>
#include <string>
#include <SDL_image.h>

class SDLApp {
protected:
	SDL_Window * window_;
	SDL_Renderer * renderer_;
public:
	SDLApp();
	virtual ~SDLApp();

	void Init(const std::string & title, int width, int height, int flags = SDL_INIT_EVERYTHING);
	void Render(SDL_Texture *tex, SDL_Rect src, SDL_Rect dst);
	void HandleInput();
};
#endif