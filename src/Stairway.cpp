// Copyright anssi.grohn@karelia.fi (c) 2014.
// Licensed under GPLv3.
#include <Stairway.h>
#include <Game.h>
#include <iostream>
////////////////////////////////////////////////////////////////////////////////
#define g_Game (*Game::GetInstance())
using namespace std;
////////////////////////////////////////////////////////////////////////////////
void
Stairway::Execute( MoveCommand & cmd )
{

  if ( cmd.m_pTo == this && 
       g_Game.GetPlayer().GetItems()["torch"] == NULL )
  {
    // Report death message attached to room
    *g_Game.page_ << GetProperty("deathmessage").As<string>() <<  "\n";
	Mix_PlayChannel(-1, g_Game.sound_["yell"], 0);
	SDL_Delay(1000);
	Mix_PlayChannel(-1, g_Game.sound_["squish"], 0);
	g_Game.SetProperty("running", false);
	Mix_FadeOutMusic(3000);
	g_Game.page_->SetDirty(true);
  }
  else
  {
    Room::Execute(cmd);
  }
}
////////////////////////////////////////////////////////////////////////////////
