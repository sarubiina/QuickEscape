#ifndef __SDLApp_h__
#define __SDLApp_h__
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <string>
#include <Command.h>
#include <map>
#include <Page.h>
class Scene;
class SDLApp
{
protected:
	SDL_Window * window_;
	SDL_Renderer * renderer_;

	std::map<std::string, Scene *> scenes_;
	Scene * currentScene_;
	Uint32 time_;

public:
	Page * page_;
	SDLApp(); 
	virtual ~SDLApp();
	
	void Init(const std::string & title, int width, int height,
		int flags = SDL_WINDOW_SHOWN);
	void Render();
	void Update();
	void HandleInput();

	std::map<std::string, Mix_Music *> music_;
	std::map<std::string, Mix_Chunk *> sound_;

	void AddScene(Scene * scene);
	void DeleteScene(const std::string & name);
	void SetCurrentScene(const std::string & name);
	Scene * GetCurrentScene();
	SDL_Window * GetWindow();

};

#endif