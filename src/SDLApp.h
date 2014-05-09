#ifndef __SDLApp_h__
#define __SDLApp_h__
#include <SDL.h>
#include "SDL_image.h"
#include <string>
#include "Command.h"
#include <map>
class Scene;
class SDLApp
{
protected:
	SDL_Window * window_;
	SDL_Renderer * renderer_;
	SDL_GameController * controller;
	std::map<std::string, Scene *> scenes_;
	Scene * currentScene_;
	Uint32 time_;
	float seconds;
	float timer;

public:
	SDLApp();
	virtual ~SDLApp();


	void Init(const std::string & title, int width, int height,
		int flags = SDL_WINDOW_SHOWN);
	void Render();
	void Update();
	void HandleInput();

	void AddScene(Scene * scene);
	void DeleteScene(const std::string & name);
	void SetCurrentScene(const std::string & name);
	Scene * GetCurrentScene();
	SDL_Window * GetWindow();

};

#endif