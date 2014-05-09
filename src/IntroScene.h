#ifndef __intro_scene_h__
#define __intro_scene_h__
#include "Scene.h"
#include <SDL.h>
class IntroScene : public Scene {
protected:
	SDL_Texture * splash_;
public:
	void Init(SDL_Renderer * renderer);
	void Update(float seconds);
	void Render(SDL_Renderer * renerer);
	void OnEvent(SDL_Event & ev);
};
#endif 