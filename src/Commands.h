// Copyright anssi.grohn@karelia.fi (c) 2014.
// Licensed under GPLv3.#
#ifndef __Commands_h__
#define __Commands_h__
////////////////////////////////////////////////////////////////////////////////
#include <Command.h>
#include <string>
#include <map>
////////////////////////////////////////////////////////////////////////////////
class Room;
////////////////////////////////////////////////////////////////////////////////
/// All commands that game allows are here.
class UseCommand : public Command { 
public:
  std::string m_strWhat;
  std::string m_strTo;
  UseCommand( const std::string & what, const std::string & to ) : m_strWhat(what),m_strTo(to) {}
  void Execute( CommandHandler & handler );
};
////////////////////////////////////////////////////////////////////////////////
class ExamineCommand : public Command { 
public:
  std::string m_strTarget;
  void Execute( CommandHandler & handler );
  bool HasTarget() const { return !m_strTarget.empty(); }
};
////////////////////////////////////////////////////////////////////////////////
class QuitCommand: public Command { 
public:
  void Execute( CommandHandler & handler );
};
////////////////////////////////////////////////////////////////////////////////
class MoveCommand : public Command 
{
public:
  Direction m_Dir;
  Room *    m_pFrom;
  Room *    m_pTo;

  MoveCommand( Direction d, Room *from, Room *to ) : m_Dir(d), m_pFrom(from), m_pTo(to) {}
  void Execute( CommandHandler & handler );
};
////////////////////////////////////////////////////////////////////////////////
class UnknownCommand : public Command {
public:
  void Execute( CommandHandler & handler);
};
////////////////////////////////////////////////////////////////////////////////
class TakeCommand : public Command 
{
public:
  std::string m_strWhat;
  TakeCommand( const std::string& what ) : m_strWhat(what){}
  void Execute( CommandHandler & handler );
};
////////////////////////////////////////////////////////////////////////////////
class DropCommand : public TakeCommand 
{
public:
  // C++11 feature; inheriting constructors
  //using TakeCommand::TakeCommand;

  /// earlier compilers will require the hard (more compatible) way 
  DropCommand( const std::string & what ) : TakeCommand(what) {}
  void Execute( CommandHandler & handler );
};
////////////////////////////////////////////////////////////////////////////////
class InventoryCommand : public Command{ 
public:
  void Execute( CommandHandler & handler );
};
////////////////////////////////////////////////////////////////////////////////
class LookCommand : public Command{ 
public:
  void Execute( CommandHandler & handler );
};
////////////////////////////////////////////////////////////////////////////////
class NullCommand : public Command { 
public:
  void Execute( CommandHandler & handler );
};
////////////////////////////////////////////////////////////////////////////////
typedef std::map<std::string, boost::regex> CommandAliases;
////////////////////////////////////////////////////////////////////////////////
/// Commandutils parses user input and converts it to command.
class CommandUtils 
{
private:
  static CommandAliases commandAliases;
  CommandUtils(){}
public:
  static void Load( const std::string & file );
  static Command * Parse( const std::string & input );
  static bool Match( const std::string & cmd, const std::string & input);
};
////////////////////////////////////////////////////////////////////////////////
// Handler interface for all commands.
class CommandHandler 
{
protected:

  CommandHandler(){}

public:
  virtual ~CommandHandler() {}
  virtual void Execute( UseCommand & cmd ) = 0;
  virtual void Execute( ExamineCommand & cmd) = 0;
  virtual void Execute( QuitCommand & cmd) = 0;
  virtual void Execute( MoveCommand & cmd) = 0;
  virtual void Execute( UnknownCommand & cmd) = 0;
  virtual void Execute( TakeCommand & cmd) = 0;
  virtual void Execute( DropCommand & cmd) = 0;
  virtual void Execute( InventoryCommand & cmd) = 0;
  virtual void Execute( LookCommand & cmd) = 0;
  virtual void Execute( NullCommand & cmd) = 0;
};
//////////////////////////////////////////////////////////////////////
// No-operation handler for all commands. Helps in writing code if you just 
// overwrite Execute for one or two commands.
class NOPCommandHandler : public CommandHandler
{
public:

  virtual void Execute( UseCommand & cmd ){}
  virtual void Execute( ExamineCommand & cmd){}
  virtual void Execute( QuitCommand & cmd) {}
  virtual void Execute( MoveCommand & cmd) {}
  virtual void Execute( UnknownCommand & cmd) {}
  virtual void Execute( TakeCommand & cmd) {}
  virtual void Execute( DropCommand & cmd) {}
  virtual void Execute( InventoryCommand & cmd) {}
  virtual void Execute( LookCommand & cmd) {}
  virtual void Execute( NullCommand & cmd) {}
};
////////////////////////////////////////////////////////////////////////////////
#endif
