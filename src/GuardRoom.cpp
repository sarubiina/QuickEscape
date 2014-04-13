// Copyright anssi.grohn@karelia.fi (c) 2014.
// Licensed under GPLv3.
#include <GuardRoom.h>
#include <iostream>
#include <Game.h>
#include <cstdlib>
////////////////////////////////////////////////////////////////////////////////
using namespace std;
#define g_Game (*Game::GetInstance())
////////////////////////////////////////////////////////////////////////////////
GuardRoom::GuardRoom() : m_bRandomized(false)
{

}
////////////////////////////////////////////////////////////////////////////////
void
GuardRoom::Execute( ExamineCommand & cmd )
{
  Room & room  = *this;
  
  if ( cmd.m_strTarget == "guard") 
  {
    m_numSearches++;
    m_numTotalSearches++;
    // Three consequtive searches wake guard
    if ( m_numSearches >= 3 )
    {
      // Report death message attached to room
      cout << GetProperty("deathmessage").As<string>() <<  "\n";
      g_Game.SetProperty("running", false);
    }
    else if ( m_numTotalSearches == 4 && g_Game.GetPlayer().GetItems()["key"] == NULL )
    {
      cout << GetProperty("key_found_message").As<string>() << ". ";
      GameObject *pKey = new GameObject();
      pKey->SetId("ID_KEY");
      pKey->SetName("key");
      pKey->SetDescription("key you pick-pocketed from guard");
      g_Game.GetPlayer().GetItems().push_back( pKey);
    }
    else 
    {
      cout << GetProperty("guard_search_msg").As<string>() << ". ";
    }
  }
  else if ( cmd.m_strTarget == "room" )
  {
    cout << room.GetDescription() << "\n";
    list<string> items;
    for(auto item : room.GetItems()) 
    {
      items.push_back(item->GetName());
    }
    
    if ( items.empty()) 
    {
      cout << "There seems to be nothing in the room. ";
    }
    else 
    {
      cout << "You can see " << Game::MakeReadable(items) << " in here.";    
    }
    cout << "\n";
  } 
  else 
  {
    // examining room items.
    GameObject * item = GetItems()[cmd.m_strTarget];
    if ( item ) cout << item->GetDescription() << ". ";
    else     cout << "I do not know how to examine " << cmd.m_strTarget << ". ";
    cout << "\n";
  }
}
////////////////////////////////////////////////////////////////////////////////
void
GuardRoom::Execute( MoveCommand & cmd )
{
  // if no slippers, guard wakes up

  if ( cmd.m_pTo == this ) // invoke only when entering this room.
  { 
    if (g_Game.GetPlayer().GetItems()["slippers"] == NULL ) 
    {
      // Report death message attached to room
      cout << GetProperty("deathmessage").As<string>() <<  "\n";
      g_Game.SetProperty("running", false);
    }
    else
    {
      m_numSearches = 0; 
      Room::Execute(cmd);
      RandomizeExits();
      
    }
  }
}
////////////////////////////////////////////////////////////////////////////////
void 
GuardRoom::Execute( UseCommand & cmd )
{
  if ( !m_bRandomized ) RandomizeExits();
  m_numSearches = 0; 
  if ( cmd.m_strWhat == "key" )
  {
    if ( g_Game.GetPlayer().GetItems()["key"] == NULL )
    {
      cout << "If you only had a key.\n";
    }
    else if ( cmd.m_strTo  == "door" )
    {
      cout << "Use key to which door?\n";
    }
    else if ( cmd.m_strTo == "door to north")
    {
      // Enable north exit 
      cout << GetProperty("north_door_open_message").As<string>() << "\n";
      if ( GetItems()["door to north"]->GetProperty("deadly").As<bool>() == true )
	SetNextRoom( North, g_Game.GetRooms()["RID_LAVAPIT"]);
      else
	SetNextRoom( North, g_Game.GetRooms()["RID_EXIT"]);
    }
    else if ( cmd.m_strTo == "door to south")
    {
      // Enable south exit 
      cout << GetProperty("south_door_open_message").As<string>() << "\n";
      if ( GetItems()["door to south"]->GetProperty("deadly").As<bool>() == true )
	SetNextRoom( South, g_Game.GetRooms()["RID_LAVAPIT"]);
      else
	SetNextRoom( South, g_Game.GetRooms()["RID_EXIT"]);

    }
    else if ( cmd.m_strTo == "door to east")
    {
      // Enable east exit 
      cout << GetProperty("east_door_open_message").As<string>() << "\n";
      if ( GetItems()["door to east"]->GetProperty("deadly").As<bool>() == true )
	SetNextRoom( East, g_Game.GetRooms()["RID_LAVAPIT"]);
      else
	SetNextRoom( East, g_Game.GetRooms()["RID_EXIT"]);
    }
    else 
    {
      cout << "You can't use key on that.\n";
    }
  }
}
////////////////////////////////////////////////////////////////////////////////
void
GuardRoom::RandomizeExits()
{
  // Upon entering, door exits change place
  GameObject* array[3] = { GetItems()["door to north"],
			   GetItems()["door to south"],
			   GetItems()["door to east"] };
  
  // when key is used, proper exit is selected.
  for( auto a : array ) 
  {
    a->SetProperty("deadly", true);
    a->SetDescription("Door feels warm to touch");
  }
  GameObject *pSafety = array[rand()%3];
  pSafety->SetProperty("deadly", false);
  pSafety->SetDescription("Door feels cold to touch");
  
  m_bRandomized = true;
}
////////////////////////////////////////////////////////////////////////////////

