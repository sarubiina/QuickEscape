// Copyright anssi.grohn@karelia.fi (c) 2014.
// Licensed under GPLv3.
#ifndef __Room_h__
#define __Room_h__
////////////////////////////////////////////////////////////////////////////////
#include <string>
#include <list>
#include <Updateable.h>
#include <GameObject.h>
#include <map>
#include <tinyxml2.h>
#include <globals.h>
#include <Commands.h>
////////////////////////////////////////////////////////////////////////////////
class Game;
////////////////////////////////////////////////////////////////////////////////
class Room : public GameObject,
	     public CommandHandler
{
private:

  Room *		m_aRooms[kNumDirs];
  Game *		m_pGame;
  std::string		m_strEnterMessage;
  std::string		m_classHint;
public:

  Room();
  virtual ~Room();
  void SetGame( Game * pGame );
  Game * GetGame() const;


  void   SetNextRoom( Direction d, Room * pRoom );
  Room * GetNextRoom( Direction d );
  void	 SetEnterMessage( const std::string & msg );
  const std::string & GetEnterMessage() const;

  void   SetClassHint( const std::string & hint );
  void Load( tinyxml2::XMLDocument & doc, const std::string & id = "");  
  void Load( tinyxml2::XMLElement & elem );
  void Save(tinyxml2::XMLPrinter & p);
  
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
