// Copyright anssi.grohn@karelia.fi (c) 2014.
// Licensed under GPLv3.
#ifndef __Stairway_h__
#define __Stairway_h__
#include <Room.h>
class Stairway : public Room 
{
public:
  void Execute( MoveCommand & cmd );

};

#endif
