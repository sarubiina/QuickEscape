// Copyright anssi.grohn@karelia.fi (c) 2014.
// Licensed under GPLv3.
#include <Player.h>
#include <globals.h>
#include <iostream>
#include <Game.h>
////////////////////////////////////////////////////////////////////////////////
using namespace tinyxml2;
using namespace std;
////////////////////////////////////////////////////////////////////////////////
void
Player::Save( XMLPrinter & p )
{
  p.OpenElement("Player");
  p.PushAttribute("name", GetName().c_str());
  p.PushAttribute("id", GetId().c_str());
  p.PushAttribute("description", GetDescription().c_str());
  p.OpenElement("Items");
  for(auto i : GetItems() ) i->Save(p);
  p.CloseElement();
  SaveProperties(p);
  p.CloseElement();
}
////////////////////////////////////////////////////////////////////////////////
void
Player::Execute( UseCommand & cmd )
{
  throw UseCommandFailOnPlayerException("Item not found");
}
////////////////////////////////////////////////////////////////////////////////
void
Player::Execute( InventoryCommand & cmd )
{
  if ( GetItems().empty() ) 
  {
    *Game::GetInstance()->page_ << "You have no items.\n";
  }
  else 
  {
    // List player's items.
	*Game::GetInstance()->page_ << "You have ";
    std::list<string> items;
    for(auto item : GetItems()) items.push_back(item->GetName());
	*Game::GetInstance()->page_ << Game::MakeReadable(items) << "\n";
  }
}
////////////////////////////////////////////////////////////////////////////////
void
Player::Execute( ExamineCommand & cmd )
{
  
  GameObject * item = GetItems()[cmd.m_strTarget];
  if ( !item ) throw ExamineCommandFailOnPlayerException("");
  *Game::GetInstance()->page_ << "After taking a closer look, you see " << item->GetDescription() << ". ";
  
}
////////////////////////////////////////////////////////////////////////////////
