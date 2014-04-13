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
    cout << GetProperty("deathmessage").As<string>() <<  "\n";
    g_Game.SetProperty("running", false);
  }
  else
  {
    Room::Execute(cmd);
  }
}
////////////////////////////////////////////////////////////////////////////////
