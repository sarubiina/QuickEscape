// Copyright anssi.grohn@karelia.fi (c) 2014.
// Licensed under GPLv3.
#ifndef __Command_h__
#define __Command_h__
class CommandHandler;
////////////////////////////////////////////////////////////////////////////////
class Command 
{
public:
  virtual ~Command() {}
  // Function for enabling double dispatch (calling CommandHandler::Execute with proper type)
  virtual void Execute( CommandHandler & handler ) = 0;
};
////////////////////////////////////////////////////////////////////////////////
#endif
