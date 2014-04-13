// Copyright anssi.grohn@karelia.fi (c) 2014.
// Licensed under GPLv3.
#ifndef __Player_h__
#define __Player_h__
////////////////////////////////////////////////////////////////////////////////
#include <GameObject.h>
#include <tinyxml2.h>
#include <Commands.h>
////////////////////////////////////////////////////////////////////////////////
class Player : public GameObject,
	       public NOPCommandHandler
{
public:
  void Save( tinyxml2::XMLPrinter & p);
  void Execute( UseCommand & cmd );
  void Execute( InventoryCommand & cmd );
  void Execute( ExamineCommand & cmd );
};
////////////////////////////////////////////////////////////////////////////////
#endif
