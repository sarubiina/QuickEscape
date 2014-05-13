#include <SDL.h>
#include "SDL_image.h"
#include "SDLApp.h"
#include <stdexcept>
#include <sstream>
#include "Game.h"
#include "Scene.h"
#include "IntroScene.h"
#include "GameScene.h"
using namespace std;

SDLApp::SDLApp(){
	// Initialize all SDL subsystems
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		throw runtime_error(SDL_GetError());
	}

		// Initialize SDL_image
		int initFlags = IMG_INIT_JPG | IMG_INIT_PNG;
		int retFlags = IMG_Init(initFlags);

		if (retFlags != initFlags){
			throw runtime_error(IMG_GetError());
		}
		// make pointers safe
		window_ = NULL;
		renderer_ = NULL;
		if (SDL_GameControllerOpen(0) != NULL){
			controller = SDL_GameControllerOpen(0);
			SDL_GameControllerEventState(SDL_ENABLE);
		}
		currentScene_ = NULL;
		seconds = 0;
		timer = 0;

	time_ = SDL_GetTicks();

}

SDLApp::~SDLApp() {
	SDL_DestroyWindow(window_);
	SDL_DestroyRenderer(renderer_);
	SDL_GameControllerClose(controller);
	controller = NULL;
	IMG_Quit();
	SDL_Quit();
}

void
SDLApp::Init(const std::string & title, int width, int height,
int flags) {

	//Create a window, which size is the image's dimensions
	window_ = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		width, height, flags);
	if (window_ == nullptr){
		throw runtime_error(SDL_GetError());
	}

	//Create a renderer
	renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_SOFTWARE | SDL_RENDERER_TARGETTEXTURE);
	if (renderer_ == nullptr){
		throw runtime_error(SDL_GetError());
	}

	IntroScene * intro = new IntroScene();
	intro->Init(renderer_);
	intro->SetName("intro");
	AddScene(intro);
	SetCurrentScene("intro");

	GameScene * game = new GameScene();
	game->Init(renderer_);
	game->SetName("game");
	AddScene(game);
	

}

void
SDLApp::Render(){

	if (currentScene_) {
		currentScene_->Render(renderer_);
	}

	SDL_RenderPresent(renderer_);
}

void
SDLApp::HandleInput() {
	SDL_Event ev;
	while (SDL_PollEvent(&ev)) {
		switch (ev.type)
		{
		case SDL_CONTROLLERDEVICEADDED:
			controller = SDL_GameControllerOpen(0);
			SDL_GameControllerEventState(SDL_ENABLE);
			break;
		case SDL_QUIT: 
			Game::GetInstance()->GetProperty("running").SetValue(false);
			break;
		case SDL_KEYDOWN: case SDL_CONTROLLERBUTTONDOWN:
			currentScene_->OnEvent(ev);
			break;
		}
	}
}

void
SDLApp::AddScene(Scene * scene) {
	if (scenes_.find(scene->GetName()) != scenes_.end()) {
		ostringstream ss("Scene with name '");
		ss << scene->GetName() << "' already exists!";
		throw runtime_error(ss.str());
	}
	scenes_[scene->GetName()] = scene;
}

void
SDLApp::DeleteScene(const std::string & name) {
	auto it = scenes_.find(name);
	if (it != scenes_.end()) {
		delete it->second;
		scenes_.erase(it);
	}
	else {
		ostringstream ss("No scene with name '");
		ss << name << "' exists!";
		throw runtime_error(ss.str());
	}
}

void
SDLApp::SetCurrentScene(const std::string & name) {
	auto it = scenes_.find(name);
	if (it != scenes_.end()) {
		currentScene_ = it->second;
	}
}

Scene *
SDLApp::GetCurrentScene() {
	return currentScene_;
}

SDL_Window *
SDLApp::GetWindow() {
	return window_;
}

void
SDLApp::Update() {
	const int MIN_ALLOWED_TIME_STEP_MS = 5;
	Uint32 slice = SDL_GetTicks() - time_;
	if (slice >= MIN_ALLOWED_TIME_STEP_MS) {
		if (currentScene_) {
			float seconds = float(slice) * 0.001f;
			currentScene_->Update(seconds);
			time_ = SDL_GetTicks();
			timer += seconds;
			// Change to GameScene from IntroScene after two seconds
			if (currentScene_->GetName().compare("intro") == 0) {
				if (timer > 3.5f) {
					SetCurrentScene("game");
					timer = 0;
				}
			}

		}
	}


}