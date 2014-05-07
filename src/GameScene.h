#ifndef __game_scene_h__
#define __game_scene_h__
#include "Scene.h"
#include "Page.h"
#include "Command.h"
#include <SDL2_gfxPrimitives.h>
#include <tinyxml2.h>
#include <SDL_ttf.h>
#include <string>
#include <SDL.h>
class GameScene : public Scene {
protected:
	SDL_Texture * player_;
	SDL_Texture * covers_;
	SDL_Texture * pages_;
	SDL_Texture * renderToTexture;
	TTF_Font * font;
	Page leftPage;
	SDL_Rect srcPlayerRect;
	SDL_Rect pageRect;
	SDL_Rect coverRect;
	std::string fontName;
	std::string text;
	const char* fontPath;
	bool printed = false;
	int fontSize;

public:
	void Init(SDL_Renderer * renderer);
	void Update(float seconds);
	void Render(SDL_Renderer * renerer);
	void OnEvent(SDL_Event & ev);
};
#endif 