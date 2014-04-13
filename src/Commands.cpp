// Copyright anssi.grohn@karelia.fi (c) 2014.
// Licensed under GPLv3.
#include <Game.h>
#include <Commands.h>
#include <globals.h>
#include <sstream>
#include <algorithm>
#include <Room.h>
#include <tinyxml2.h>
#include <boost/regex.hpp>
////////////////////////////////////////////////////////////////////////////////
using namespace std;
using namespace tinyxml2;
using namespace boost;
////////////////////////////////////////////////////////////////////////////////
#define g_Game (*Game::GetInstance())
extern map<XMLError,string> g_TinyXmlErrors;
////////////////////////////////////////////////////////////////////////////////
CommandAliases CommandUtils::commandAliases;
////////////////////////////////////////////////////////////////////////////////
void
CommandUtils::Load( const std::string & file )
{
  XMLDocument doc;
  doc.LoadFile(file.c_str());

  if ( doc.ErrorID() != XML_NO_ERROR )
  {
    ostringstream ss;
    ss << "Loadfile:" << g_TinyXmlErrors[doc.ErrorID()] << " " << doc.GetErrorStr1() << " " << doc.GetErrorStr2();
    throw XmlParsingException(ss.str());
  }
  
  XMLElement *elem = doc.FirstChildElement("Commands");
  if ( !elem ) throw ElementMissingException("Commands");
  XMLElement *cmd = elem->FirstChildElement();
  if ( !cmd) throw ElementMissingException("No command aliaes defined");
  while ( cmd )
  {
    if ( !cmd->Attribute("match")) throw AttributeMissingException("match");
    std::string name = cmd->Name();
    for( auto & a : name ) a = tolower(a);
    
    regex r;
    r.assign(cmd->Attribute("match"));
    commandAliases[name] = r;
    cmd = cmd->NextSiblingElement();
  }
}
////////////////////////////////////////////////////////////////////////////////
bool
CommandUtils::Match( const std::string & cmd, const std::string & input)
{
  return regex_match( input, commandAliases[cmd]);
}
////////////////////////////////////////////////////////////////////////////////
Command *
CommandUtils::Parse( const std::string & input )
{
  // Convert line to lowercase
  string tmp = input;
  transform(tmp.begin(),tmp.end(), tmp.begin(), [] (char c){ return tolower(c);});
  stringstream ss;
  ss << tmp;
  ss >> tmp;
  Room *current = g_Game.GetCurrentRoom();
  if ( Match("quit",tmp) ) 
  {
    return new QuitCommand();
  }
  else if ( Match("move",tmp) )
  {
    string dir;
    ss >> dir;

    Direction d = kNumDirs;
    if      ( dir == "south" ) d = South;
    else if ( dir == "north" ) d = North;
    else if ( dir == "east" )  d = East;
    else if ( dir == "west" )  d = West;
    // Takes care of kNumDirs value as well
    MoveCommand *pCmd = new MoveCommand(d, current, current->GetNextRoom(d) );
    return pCmd;
  }
  else if ( tmp == "mn" ){  return new MoveCommand(North, current, current->GetNextRoom(North)); }
  else if ( tmp == "me" ){  return new MoveCommand(East,  current, current->GetNextRoom(East));  }
  else if ( tmp == "ms" ){  return new MoveCommand(South, current, current->GetNextRoom(South)); }
  else if ( tmp == "mw" ){  return new MoveCommand(West,  current, current->GetNextRoom(West));  }
  
  else if ( Match("take", tmp) )
  {
    // take entire description in the end.
    string what = Game::Trim(ss.str().substr(tmp.size()));
    return new TakeCommand(what);
  }
  else if ( Match("drop",tmp ) )
  {
    // take entire description in the end.
    string what = Game::Trim(ss.str().substr(tmp.size()));
    return new DropCommand(what);
  }
  else if ( Match( "inventory",tmp))
  {
    return new InventoryCommand();
  }
  else if ( Match("look",tmp) )
  {
    return new LookCommand();
  } 
  else if ( Match("examine", tmp))
  {
    ExamineCommand * s = new ExamineCommand();
    if ( ss.str().size() > tmp.size())
    { 
      string what = ss.str().substr(tmp.size());
      s->m_strTarget = Game::Trim(what);
    }
    // make everything lower-case
    for( auto & c : s->m_strTarget ) c = tolower(c);
    return s;
  }
  else if ( Match("use",tmp) )
  {
    UseCommand *pUse = NULL;
    string rest = ss.str().substr(tmp.size());
    size_t pos = rest.find(" to ");
    
    if ( pos != string::npos)
    {
      string what = rest.substr(0,pos);
      string to = rest.substr(pos+what.size());
      pUse = new UseCommand(Game::Trim(what), Game::Trim(to));
    }
    else
    {
      pUse = new UseCommand(Game::Trim(rest), "");
    }
    return pUse;
  }
  else if ( !tmp.empty() )
  {
    return new UnknownCommand();
  }
  return new NullCommand();
}
////////////////////////////////////////////////////////////////////////////////
void 
UseCommand::Execute( CommandHandler & handler )
{ 
  handler.Execute(*this);
}
////////////////////////////////////////////////////////////////////////////////
void 
ExamineCommand::Execute( CommandHandler & handler )
{ 
  handler.Execute(*this);
}
////////////////////////////////////////////////////////////////////////////////
void 
QuitCommand::Execute( CommandHandler & handler )
{ 
  handler.Execute(*this);
}
////////////////////////////////////////////////////////////////////////////////
void 
MoveCommand::Execute( CommandHandler & handler )
{ 
  handler.Execute(*this);
}
////////////////////////////////////////////////////////////////////////////////
void 
TakeCommand::Execute( CommandHandler & handler )
{ 
  handler.Execute(*this);
}
////////////////////////////////////////////////////////////////////////////////
void 
DropCommand::Execute( CommandHandler & handler )
{ 
  handler.Execute(*this);
}
////////////////////////////////////////////////////////////////////////////////
void 
InventoryCommand::Execute( CommandHandler & handler )
{ 
  handler.Execute(*this);
}
////////////////////////////////////////////////////////////////////////////////
void 
LookCommand::Execute( CommandHandler & handler )
{ 
  handler.Execute(*this);
}
////////////////////////////////////////////////////////////////////////////////
void 
NullCommand::Execute( CommandHandler & handler )
{ 
  handler.Execute(*this);
}
////////////////////////////////////////////////////////////////////////////////
void 
UnknownCommand::Execute( CommandHandler & handler )
{ 
  handler.Execute(*this);
}
////////////////////////////////////////////////////////////////////////////////
