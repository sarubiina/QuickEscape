#ifndef __Scene_h__
#define __Scene_h__
#include <Updateable.h>
#include <PropertyHolder.h>
#include <SDL.h>
#include <string>
class Scene : public Updateable,
			  public PropertyHolder
{
protected:
	std::string name_;
	float alpha;
	float timer;
public:
	void SetName(const std::string & name);
	const std::string & GetName() const;
	virtual void Init(SDL_Renderer *renderer);
	virtual void Render(SDL_Renderer * renderer);
	virtual void Update(float second);
	virtual void OnEvent(SDL_Event & ev);
};

#endif