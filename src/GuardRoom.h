// Copyright anssi.grohn@karelia.fi (c) 2014.
// Licensed under GPLv3.
#ifndef __GuardRoom_h__
#define __GuardRoom_h__
////////////////////////////////////////////////////////////////////////////////
#include <Room.h>
////////////////////////////////////////////////////////////////////////////////
class GuardRoom : public Room
		  
{
private:
  int	m_numSearches;
  int   m_numTotalSearches;
  bool m_bRandomized;
  void  RandomizeExits();
public:
  GuardRoom();
  void Execute( ExamineCommand & cmd );
  void Execute( UseCommand & cmd );
  void Execute( QuitCommand & cmd) { m_numSearches = 0; Room::Execute(cmd);}
  void Execute( MoveCommand & cmd);
  void Execute( UnknownCommand & cmd) {}
  void Execute( TakeCommand & cmd) {m_numSearches = 0; Room::Execute(cmd);}
  void Execute( DropCommand & cmd) {m_numSearches = 0; Room::Execute(cmd);}
  void Execute( InventoryCommand & cmd) {m_numSearches = 0; Room::Execute(cmd);}
  void Execute( LookCommand & cmd) { m_numSearches = 0; Room::Execute(cmd);}
  void Execute( NullCommand & cmd) {}
};
////////////////////////////////////////////////////////////////////////////////

#endif
