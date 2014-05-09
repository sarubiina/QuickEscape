#ifndef __Scene_h__
#define __Scene_h__

#include <SDL.h>
#include <string>
#include "Updateable.h"
#include <PropertyHolder.h>
class Scene : public Updateable,
	public PropertyHolder
{
protected:
	std::string name_;
	float alpha;
	float timer;
public:
	void SetName(const std::string & name);
	const std::string GetName();
	virtual void Init(SDL_Renderer * renderer);
	virtual void Render(SDL_Renderer * renderer);
	virtual void Update(float seconds);
	virtual void OnEvent(SDL_Event & ev);


};
#endif 