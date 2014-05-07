#include <Scene.h>

void
Scene::Render(SDL_Renderer * renderer) 
{

}

void
Scene::Init(SDL_Renderer * renderer) 
{

}

void 
Scene::Update(float seconds) 
{

}

void
Scene::SetName(const std::string & name) 
{
	name_ = name;
}

const std::string & Scene::GetName() const
{
	return name_;
}

void 
Scene::OnEvent(SDL_Event & ev) 
{

}