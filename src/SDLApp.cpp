#include <SDLApp.h>
#include <SDL.h>
#include <stdexcept>
#include <Game.h>
#include <Logger.h>
#include <sstream>
#include <Scene.h>
#include <IntroScene.h>
#include <GameScene.h>
#include <SDL_image.h>
using namespace std;

SDLApp::SDLApp()
{
	//Initialize all SDL subsystems
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		throw runtime_error(SDL_GetError());
	}
	//initialize SDL Image 
	int initFlags = IMG_INIT_JPG | IMG_INIT_PNG;
	int retFlags = IMG_Init(initFlags);

	if ( retFlags != initFlags)
	{
		throw runtime_error(IMG_GetError());
	}

	//make pointers safe
	window_ = NULL;
	renderer_ = NULL;
	currentScene_ = NULL;

	timer = 0;
	time_ = SDL_GetTicks();
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

	IntroScene * intro = new IntroScene();
	intro->Init(renderer_);
	intro->SetName("Intro");
	AddScene(intro);
	SetCurrentScene("intro");

	GameScene * game = new GameScene();
	game->Init(renderer_);
	game->SetName("game");
	AddScene(game);
}

void 
SDLApp::Render()
{
	if (currentScene_) currentScene_->Render(renderer_);
	SDL_RenderPresent(renderer_);
}

void
SDLApp::Update()
{
	const int MIN_ALLOWED_TIME_STEPS_MS = 5;

	Uint32 slice = SDL_GetTicks() - time_;
	if (slice >= MIN_ALLOWED_TIME_STEPS_MS)
	{
		if (currentScene_)
		{
			float seconds = (float)slice * 0.001f;
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
			currentScene_->OnEvent(ev);
		break;
		}
	}	
}
void 
SDLApp::AddScene(Scene * scene)
{
	if (scenes_.find(scene->GetName()) != scenes_.end())
	{
		ostringstream ss("Scene with name '");
		ss << scene->GetName() << "' already exists!";
		throw runtime_error(ss.str());
	}
	scenes_[scene->GetName()] = scene;
}
void 
SDLApp::DeleteScene(const std::string & name)
{
	auto it = scenes_.find(name);
	if (it != scenes_.end())
	{
		delete it->second;
		scenes_.erase(it);
	}
	else
	{
		ostringstream ss("no scene with name '");
		ss << name << "' exists!";
		throw runtime_error(ss.str());
	}
}
void 
SDLApp::SetCurrentScene(const std::string & name)
{
	auto it = scenes_.find(name);
	if (it != scenes_.end())
	{
		currentScene_ = it->second;
	}
}
Scene *
SDLApp:: GetCurrentScene()
{
	return currentScene_;
}

SDL_Window * 
SDLApp::GetWindow()
{
	return window_;
}