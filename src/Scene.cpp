#include "Scene.h"
#include <iostream>


void 
Scene::SetName(const std::string & name) 
{
	name_ = name;
}

const 
std::string Scene::GetName()
{
	return name_;
}

void 
Scene::Init(SDL_Renderer * renderer) 
{

}

void 
Scene::Render(SDL_Renderer * renderer)
{
	
}

void
Scene::Update(float seconds)
{
	
}

void 
Scene::OnEvent(SDL_Event & ev) 
{

}

void
Scene::OnEnter()
{

}

void
Scene::OnExit()
{

}