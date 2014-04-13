// Copyright anssi.grohn@karelia.fi (c) 2014.
// Licensed under GPLv3.
#ifndef __Game_h__
#define __Game_h__
////////////////////////////////////////////////////////////////////////////////
#include <string>
#include <map>
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <tinyxml2.h>
#include <GameObject.h>
#include <Player.h>
#include <PropertyHolder.h>
#include <Commands.h>
////////////////////////////////////////////////////////////////////////////////
class IRenderer;
class Room;
typedef std::map<std::string, Room *> Rooms;
////////////////////////////////////////////////////////////////////////////////
class Game : public PropertyHolder,
	     public CommandHandler
{
private:
  Rooms       m_Rooms;
  Room *      m_pCurrentRoom;
  Player      m_Player;
  std::string m_Story;
  static Game *m_pInstance;
  Game();
  virtual ~Game();
public:
  
  static Game * GetInstance();
  static void   Destroy();

  void   Play();
  void   LoadActions(const std::string & file );
  Room * GetCurrentRoom();
  void   SetCurrentRoom(Room *pRoom );
  void   LoadMap( const std::string & file ); 
  void   Save( const std::string & file );
  static std::string  MakeReadable( std::list<std::string> & list ) ;
  static std::string Trim( const std::string & str );
  Player & GetPlayer();
  Rooms &  GetRooms();
  
  void Execute( UseCommand & cmd );
  void Execute( ExamineCommand & cmd);
  void Execute( QuitCommand & cmd);
  void Execute( MoveCommand & cmd);
  void Execute( UnknownCommand & cmd);
  void Execute( TakeCommand & cmd);
  void Execute( DropCommand & cmd);
  void Execute( InventoryCommand & cmd);
  void Execute( LookCommand & cmd);
  void Execute( NullCommand & cmd);
};
////////////////////////////////////////////////////////////////////////////////
#endif
