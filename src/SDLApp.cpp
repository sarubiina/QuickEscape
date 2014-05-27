#include <SDL.h>
#include <SDLApp.h>
#include <Game.h>
#include <Scene.h>
#include <IntroScene.h>
#include <GameScene.h>
#include <stdexcept>
#include <sstream>
#include <Logger.h>
using namespace std;
////////////////////////////////////////////////////////
SDLApp::SDLApp()
{
	// Initialize all SDL subsystems
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) 
		throw runtime_error(SDL_GetError());
	
	// Initialize SDL_image
	int initFlags = IMG_INIT_JPG | IMG_INIT_PNG;
	int retFlags = IMG_Init(initFlags);

	if (retFlags != initFlags)
		throw runtime_error(IMG_GetError());
	
	if (TTF_Init())
	{
		throw runtime_error(TTF_GetError());
	}
	

	//initialize SDL_mixer
	int iDesiredFlags = MIX_INIT_MP3 | MIX_INIT_OGG;
	int iInitedFlags = Mix_Init(iDesiredFlags);

	if ((iDesiredFlags & iInitedFlags) != iDesiredFlags)
	{
		throw runtime_error(Mix_GetError());
	}
	//audio quality flags and values
	int		audio_freq		= 22050;
	Uint16	audio_format	= AUDIO_S16SYS;
	int		audio_channels	= 2;
	int		audio_buffers	= 1024;

	if (Mix_OpenAudio(audio_freq, audio_format, audio_channels, audio_buffers))
	{
		throw runtime_error(Mix_GetError());
	}

	// make pointers safe
	window_ = NULL;
	renderer_ = NULL;
	currentScene_ = NULL;
}
////////////////////////////////////////////////////////
SDLApp::~SDLApp() {

	SDL_DestroyWindow(window_);
	SDL_DestroyRenderer(renderer_);

	for (auto & e : scenes_)
	{
		delete e.second;
	}

	for (auto & e : music_)
	{
		Mix_FreeMusic(e.second);
	}
	music_.clear();

	for (auto & e : sound_)
	{
		Mix_FreeChunk(e.second);
	}
	music_.clear();

	Mix_CloseAudio();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}
////////////////////////////////////////////////////////
void
SDLApp::Init(const std::string & title, int width, int height,
int flags) 
{
	//Create a window, which size is the image's dimensions
	window_ = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		width, height, flags);
	if (window_ == nullptr)
	{
		throw runtime_error(SDL_GetError());
	}

	//Create a renderer
	renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
	if (renderer_ == nullptr)
	{
		throw runtime_error(SDL_GetError());
	}

	//loading sounds
	Mix_Music * music = Mix_LoadMUS("res/background.mp3");
		if (music == NULL)
			throw runtime_error(Mix_GetError());
		music_["background"] = music;

	Mix_Chunk * sound = Mix_LoadWAV("res/step.wav");
		if (sound == NULL)
			throw runtime_error(Mix_GetError());
		sound_["step"] = sound;

	sound = Mix_LoadWAV("res/wall.wav");
		if (sound == NULL)
			throw runtime_error(Mix_GetError());
		sound_["wall"] = sound;

	sound = Mix_LoadWAV("res/yell.wav");
		if (sound == NULL)
			throw runtime_error(Mix_GetError());
		sound_["yell"] = sound;

	sound = Mix_LoadWAV("res/squish.wav");
		if (sound == NULL)
			throw runtime_error(Mix_GetError());
		sound_["squish"] = sound;

	sound = Mix_LoadWAV("res/flippage.wav");
		if (sound == NULL)
			throw runtime_error(Mix_GetError());
		sound_["flippage"] = sound;

	//add intro scene
	IntroScene * intro = new IntroScene();
	intro->Init(renderer_);
	intro->SetName("intro");
	AddScene(intro);
	SetCurrentScene("intro");

	//add game scene
	GameScene * gamescene = new GameScene();
	gamescene->Init(renderer_);
	gamescene->SetName("gamescene");
	AddScene(gamescene);

	page_ = gamescene->page_;
}
////////////////////////////////////////////////////////
void
SDLApp::Render()
{

	if (currentScene_) 
	{
		currentScene_->Render(renderer_);
	}

	SDL_RenderPresent(renderer_);
}
////////////////////////////////////////////////////////
void
SDLApp::HandleInput() 
{
	SDL_Event ev;
	while (SDL_PollEvent(&ev)) 
	{
		switch (ev.type)
		{
		case SDL_QUIT: 
			Game::GetInstance()->GetProperty("running").SetValue(false);
			break;
		case SDL_KEYDOWN:
			currentScene_->OnEvent(ev);
			break;
		}
	}
}
////////////////////////////////////////////////////////
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
////////////////////////////////////////////////////////
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
		ostringstream ss("No scene with name '");
		ss << name << "' exists!";
		throw runtime_error(ss.str());
	}
}
////////////////////////////////////////////////////////
void
SDLApp::SetCurrentScene(const std::string & name) 
{
	auto it = scenes_.find(name);
	if (it != scenes_.end())
	{
		//call exit for previous scene
		if (currentScene_ != NULL)
			currentScene_->OnExit();
		currentScene_ = it->second;
		//call enter for new scene
		currentScene_->OnEnter();
	}
}
////////////////////////////////////////////////////////
Scene *
SDLApp::GetCurrentScene() 
{
	return currentScene_;
}
////////////////////////////////////////////////////////
SDL_Window *
SDLApp::GetWindow() 
{
	return window_;
}
////////////////////////////////////////////////////////
void
SDLApp::Update() 
{
	const int MIN_ALLOWED_TIME_STEP_MS = 5;
	Uint32 slice = SDL_GetTicks() - time_;
	if (slice >= MIN_ALLOWED_TIME_STEP_MS) 
	{
		if (currentScene_) 
		{
			float seconds = float(slice) * 0.001f;
			currentScene_->Update(seconds);
			time_ = SDL_GetTicks();
		}
	}
}