#ifndef __game_scene_h__
#define __game_scene_h__
#include <Scene.h>
#include <Page.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <map>
const int NUM_PAGE_ANIM_FRAMES = 6;

class GameScene : public Scene,
				  public NOPCommandHandler
{
protected:
	SDL_Texture * bookcover_;
	SDL_Texture * pages_[NUM_PAGE_ANIM_FRAMES];
	SDL_Texture * character_;
	SDL_Rect player_;
	SDL_Rect pos_;
	float alpha_{ 0.0f };
	bool fading_{ false };
	std::map<std::string, TTF_Font *> fonts_;
	int pageToShow_{ 0 };
	bool pageTurning_{ false };
	bool pageTurningLeft_{ false };
	float pageFrame_{ 0 };
public:
	Page * page_{ NULL };
	Page * titlePage_;
	Page * endPage_;

	virtual ~GameScene();
	void Init(SDL_Renderer * renderer);
	void Update(float seconds);
	void Render(SDL_Renderer * renderer);
	void OnEvent(SDL_Event & ev);

	void Execute(UseCommand & cmd);
	void Execute(ExamineCommand & cmd);
	void Execute(QuitCommand & cmd);
	void Execute(MoveCommand & cmd);
	void Execute(UnknownCommand & cmd);
	void Execute(TakeCommand & cmd);
	void Execute(DropCommand & cmd);
	void Execute(InventoryCommand & cmd);
	void Execute(LookCommand & cmd);
	void Execute(NullCommand & cmd);
};
#endif // !__intro_scene_h__
