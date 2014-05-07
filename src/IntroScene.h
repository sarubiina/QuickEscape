#ifndef __IntroScene_h__
#define __IntroScene_h__
#include <SDL.h>
#include <Scene.h>
class IntroScene : public Scene
{
protected:
	SDL_Texture * splash_;
public:
	void Init(SDL_Renderer * renderer);
	void Update(float seconds);
	void Render(SDL_Renderer * renderer);
	void OnEvent(SDL_Event & ev);
};
#endif