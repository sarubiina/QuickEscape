// Copyright anssi.grohn@karelia.fi (c) 2014.
// Licensed under GPLv3.
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cctype>
#include <stdexcept>
#include <Game.h>
#include <Room.h>
#include <RoomFactory.h>
#include <Logger.h>
////////////////////////////////////////////////////////////////////////////////
using namespace std;
using namespace tinyxml2;
////////////////////////////////////////////////////////////////////////////////
map<Direction,Direction> g_Opposite;
map<XMLError,string> g_TinyXmlErrors;
map<Direction,string> g_DirStr;
Game * Game::m_pInstance = NULL;
////////////////////////////////////////////////////////////////////////////////
Game::Game() : m_pCurrentRoom(NULL)
{
  Logger::GetInstance().open("log.txt");
  SetProperty("running", true);
  g_Log << "Start";

  g_Opposite[North] = South;
  g_Opposite[East] = West;
  g_Opposite[South] = North;
  g_Opposite[West] = East;
  
  g_TinyXmlErrors[XML_NO_ERROR] = "Success, no errors";
  g_TinyXmlErrors[XML_NO_ATTRIBUTE] = "No Attribute";
  g_TinyXmlErrors[XML_WRONG_ATTRIBUTE_TYPE] = "Wrong attrib type";

  g_TinyXmlErrors[XML_ERROR_FILE_NOT_FOUND] = "file not found";
  g_TinyXmlErrors[XML_ERROR_FILE_COULD_NOT_BE_OPENED] = "cant open file";
  g_TinyXmlErrors[XML_ERROR_FILE_READ_ERROR] = "file read error";
  g_TinyXmlErrors[XML_ERROR_ELEMENT_MISMATCH] = "element mismatch";
  g_TinyXmlErrors[XML_ERROR_PARSING_ELEMENT] = "parsing element";
  g_TinyXmlErrors[XML_ERROR_PARSING_ATTRIBUTE] = "parsing attribute";
  g_TinyXmlErrors[XML_ERROR_IDENTIFYING_TAG] = "identifying tag";
  g_TinyXmlErrors[XML_ERROR_PARSING_TEXT] = "parsing text";
  g_TinyXmlErrors[XML_ERROR_PARSING_CDATA] = "cdata parsing";
  g_TinyXmlErrors[XML_ERROR_PARSING_COMMENT] = "parsing comment";
  g_TinyXmlErrors[XML_ERROR_PARSING_DECLARATION] = "parsing declaration";
  g_TinyXmlErrors[XML_ERROR_PARSING_UNKNOWN] = "parsing unknown";
  g_TinyXmlErrors[XML_ERROR_EMPTY_DOCUMENT] = "empty document";
  g_TinyXmlErrors[XML_ERROR_MISMATCHED_ELEMENT] = "mismatched element";

  g_DirStr[North]  = "north";
  g_DirStr[South] = "south";
  g_DirStr[West] = "west";
  g_DirStr[East] = "east";
}
////////////////////////////////////////////////////////////////////////////////
Game::~Game()
{
  
}
////////////////////////////////////////////////////////////////////////////////
void Game::Play()
{
  
  LoadMap("res/dungeon0.xml");
  CommandUtils::Load("res/commands.xml");
  for( auto a : m_Rooms )
  {
    if ( a.second == NULL )  
    {
      ostringstream ss; 
      ss << "Whoops, " << a.first << " is NULL";
      throw NullKeyException(ss.str());
    }
  }

  cout << m_Story << "\n";

  while ( GetProperty("running") ) 
  {
    Room & room = *GetCurrentRoom();
    bool visited;
    
    if ( (room.HasProperty("visited") == false) || 
	 ((visited = room.GetProperty("visited")) == false) )
    {    
      room.SetProperty("visited", true);
    }  
    
    cout << "> ";

    string tmp;
    getline(cin,tmp);
    
    Command *pCmd = CommandUtils::Parse(tmp);
    pCmd->Execute(*this);
    delete pCmd;
    
  }  
  Save("res/dungeon0.xml");

}
////////////////////////////////////////////////////////////////////////////////
Room * 
Game::GetCurrentRoom()
{
  return m_pCurrentRoom;
}
////////////////////////////////////////////////////////////////////////////////
void   
Game::SetCurrentRoom(Room *pRoom )
{
  m_pCurrentRoom = pRoom;
}
////////////////////////////////////////////////////////////////////////////////
void
Game::LoadMap( const std::string & filename )
{
  XMLDocument doc;
  doc.LoadFile(filename.c_str());


  if ( doc.ErrorID() != XML_NO_ERROR )
  {
    ostringstream ss;
    ss << "Loadfile:" << g_TinyXmlErrors[doc.ErrorID()] << " " << doc.GetErrorStr1() << " " << doc.GetErrorStr2();
    throw XmlParsingException(ss.str());

  }

  // Load rooms
  XMLElement *pElem = doc.FirstChildElement( "Room");
  
  while( pElem != NULL )
  {
    g_Log << "loading..." << pElem->Attribute("id");
    Room *pRoom  = NULL;
    // Custom rooms
    if ( pElem->Attribute("class")) 
    {
      std::string classID = pElem->Attribute("class");
      pRoom = RoomFactory::Create(classID);
    } 
    else 
    {
      pRoom = new Room();	
    }
    pRoom->Load( *pElem );
    if ( m_Rooms.find(pRoom->GetId()) != m_Rooms.end()) 
    {
      throw DuplicateRoomIdException(pRoom->GetId());
    }
    m_Rooms[pRoom->GetId()] = pRoom;
    pElem = pElem->NextSiblingElement("Room");
  }
  // Load transitions

  pElem = doc.FirstChildElement( "Transitions");
  if ( pElem != NULL ) 
  {
    pElem = pElem->FirstChildElement("Transition");
  
    while( pElem != NULL )
    {
      const char * from = pElem->Attribute("from");
      const char * to = pElem->Attribute("to");
      const char * direction = pElem->Attribute("direction");
      const char * oneway = pElem->Attribute("oneway");

      if ( direction == NULL ) throw AttributeMissingException("Direction is missing from transition");
      if ( from == NULL ) throw AttributeMissingException("From attribute is missing from transition!");
      if ( to == NULL )   throw AttributeMissingException("To attribute is missing from transition!");
    
      // check is from a proper id
      if ( m_Rooms.find(from) == m_Rooms.end() ) 
      { 
	ostringstream ss;
	ss << from << " is not a proper room id";
	throw InvalidAttributeException(ss.str());
      }
      // check is to a proper id
      if ( m_Rooms.find(to) == m_Rooms.end() ) 
      { 
	ostringstream ss;
	ss << to << " is not a proper room id";
	throw InvalidAttributeException(ss.str());
      }
      string tmp = direction;
      transform(tmp.begin(),tmp.end(), tmp.begin(), [] (char c){ return tolower(c);});    
      Direction dir = kNumDirs;
      if ( tmp == "east" ) dir = East;
      else if ( tmp == "north" ) dir = North;
      else if ( tmp == "south" ) dir = South;
      else if ( tmp == "west" ) dir = West;
      else throw InvalidAttributeException("Direction is not properly set");
   
      m_Rooms[from]->SetNextRoom(dir, m_Rooms[to]);
      if ( !oneway )
      {
	m_Rooms[to]->SetNextRoom(g_Opposite[dir], m_Rooms[from]);
      }
      pElem = pElem->NextSiblingElement("Transition");
    }
  } 
  else 
  {
    throw ElementMissingException("Transitions is missing!");
  }
  
  // set current room
  pElem = doc.FirstChildElement("OnStart");
  if ( pElem == NULL ) throw ElementMissingException("No OnStart defined");

  pElem = pElem->FirstChildElement("CurrentRoom");
  if ( pElem == NULL ) 
  {
    cout << "No current room set, problems might arise\n";
  } 
  else 
  {
    const char *szRoomId = pElem->Attribute("id");
    if ( szRoomId == NULL) throw InvalidAttributeException("Bad format CurrentRoom");
    string tmp = szRoomId;
    if ( m_Rooms.find(tmp) == m_Rooms.end() ) 
    {
      ostringstream ss;
      ss << "No room " << tmp << " found!";
      throw ElementMissingException(ss.str());
    }
    SetCurrentRoom(m_Rooms[tmp]);
  }
  pElem = doc.FirstChildElement("OnStart");
  pElem = pElem->FirstChildElement("Story");
  if ( pElem == NULL ) throw ElementMissingException("Story");
  m_Story = (pElem->GetText() == NULL) ? "" : pElem->GetText();

  pElem = doc.FirstChildElement("Player");
  if ( !pElem ) throw new ElementMissingException("Player");
  m_Player.Load(*pElem);
}
////////////////////////////////////////////////////////////////////////////////
std::string  
Game::MakeReadable( std::list<std::string> & words ) 
{

    stringstream ssWords;
    size_t numWords = 0;

    for( auto word : words)
    {
      if ( (words.size() == 1) || 
	   (numWords == words.size()-1 && words.size() > 1 ))
      {
	ssWords << word;
      }
      else if ( numWords < words.size()-2 )	ssWords << word << ", ";
      else if ( numWords < words.size()-1 )	ssWords << word << " and ";
      
      numWords++;
    }
    
    return ssWords.str();
}
////////////////////////////////////////////////////////////////////////////////
string 
Game::Trim( const string & str )
{
  string whitespaces (" \t\f\v\n\r");
  size_t first = str.find_first_not_of(whitespaces);
  size_t last =  str.find_last_not_of(whitespaces);
  if ( first != string::npos && last != string::npos) 
    return str.substr(first,last-first+1);
  else
    return string();
}
////////////////////////////////////////////////////////////////////////////////
void
Game::Save( const std::string & filename )
{
  FILE *file = fopen(filename.c_str(), "w+");
  if ( !file ) 
  {
    stringstream ss;
    ss << "Cannot open file '" << filename << "' for writing";
    throw runtime_error( ss.str() );
  }

  XMLPrinter printer(file);
  printer.PushHeader(true,true);

  // store rooms with properties and items
  for(auto elem : m_Rooms )
  {
    elem.second->Save(printer);
  }

  // store transitions
  map<pair<Direction,Room *>, Room * > savedTransitions;
  printer.OpenElement("Transitions");

  for(auto elem : m_Rooms )
  {
    Room * from = elem.second;
    for( int dir = (int)North; dir<kNumDirs; dir++)
    {
      Direction d = static_cast<Direction>(dir);
      Room * to = from->GetNextRoom(d);
      if ( to != NULL )
      {
	// if two-way
	if( from == to->GetNextRoom(g_Opposite[d]))
	{
	  // store our transition
	  savedTransitions[make_pair<>(d,from)] = to;
	  // remove link between rooms
	  from->SetNextRoom(d,NULL);
	  to->SetNextRoom(g_Opposite[d], NULL);
	
	  printer.OpenElement("Transition");
	  printer.PushAttribute("from", from->GetId().c_str());
	  printer.PushAttribute("to", to->GetId().c_str());
	  printer.PushAttribute("direction", g_DirStr[d].c_str());
	  printer.CloseElement();
	}
	else  // one-way
	{
	  printer.OpenElement("Transition");
	  printer.PushAttribute("from", from->GetId().c_str());
	  printer.PushAttribute("to", to->GetId().c_str());
	  printer.PushAttribute("direction", g_DirStr[d].c_str());
	  printer.PushAttribute("oneway", "true");
	  printer.CloseElement();
	}
      }
    }
  }
  printer.CloseElement();
  // restore links between actual rooms
  for( auto e : savedTransitions )
  {
    // for readabilty
    Room * from = e.first.second;
    Room * to = e.second;
    Direction dir = e.first.first;
    g_Log << "adding transition " << from->GetId() << " <-> " << to->GetId() << " via " << g_DirStr[dir] << "\n";
    from->SetNextRoom( dir, to );
    to->SetNextRoom( g_Opposite[dir], from );
  }
  
  // Store player's current location.
  printer.OpenElement("OnStart");
  printer.OpenElement("CurrentRoom");
  printer.PushAttribute("id", GetCurrentRoom()->GetId().c_str());
  printer.CloseElement();
  printer.OpenElement("Story");
  printer.PushText(m_Story.c_str());
  printer.CloseElement();
  printer.CloseElement();
  
  m_Player.Save(printer);
  
  
}
////////////////////////////////////////////////////////////////////////////////
Game * 
Game::GetInstance()
{
  if ( m_pInstance == NULL )
  {
    m_pInstance = new Game();
  }
  return m_pInstance;
}
////////////////////////////////////////////////////////////////////////////////
void
Game::Destroy()
{
  if ( m_pInstance )
  {
    delete m_pInstance;
    m_pInstance = NULL;
  }
}
////////////////////////////////////////////////////////////////////////////////
Player &
Game::GetPlayer()
{
  return m_Player;
}
////////////////////////////////////////////////////////////////////////////////
Rooms &  
Game::GetRooms()
{
  return m_Rooms;
}
////////////////////////////////////////////////////////////////////////////////
void 
Game::Execute(QuitCommand & cmd)
{
  SetProperty("running", false);
}
////////////////////////////////////////////////////////////////////////////////
void
Game::Execute(MoveCommand & cmd)
{
  if ( cmd.m_Dir == kNumDirs )
  {
    cout << "You want to move ... where?\n";
  } 
  else 
  {
    Room *pNext = GetCurrentRoom()->GetNextRoom(cmd.m_Dir);
    if ( pNext ) 
    {
      // Handle move commands in both rooms
      GetCurrentRoom()->Execute(cmd);
      SetCurrentRoom(pNext);
      GetCurrentRoom()->Execute(cmd);
    }
    else
    {
      cout << "You bump your head on the wall. You can't go that way.\n";
    }
  }
}
////////////////////////////////////////////////////////////////////////////////
void
Game::Execute( UnknownCommand & cmd)
{
  cout << "Sorry, I did not quite get that.\n";
}
////////////////////////////////////////////////////////////////////////////////
void
Game::Execute( TakeCommand & cmd )
{
  GetCurrentRoom()->Execute(cmd);
}
////////////////////////////////////////////////////////////////////////////////
void
Game::Execute( DropCommand & cmd)
{
  GetCurrentRoom()->Execute(cmd);
}
////////////////////////////////////////////////////////////////////////////////
void
Game::Execute( InventoryCommand & cmd)
{
  m_Player.Execute(cmd);
}
////////////////////////////////////////////////////////////////////////////////
void
Game::Execute( LookCommand & cmd )
{
  GetCurrentRoom()->Execute(cmd);
}
////////////////////////////////////////////////////////////////////////////////
void
Game::Execute( ExamineCommand & cmd )
{
  try 
  {
    m_Player.Execute(cmd);
  } 
  catch ( ExamineCommandFailOnPlayerException & ex )
  {
    GetCurrentRoom()->Execute(cmd);
  }

}
////////////////////////////////////////////////////////////////////////////////
void
Game::Execute( UseCommand & cmd )
{
  // try player's inventory use first
  try {
    m_Player.Execute(cmd);
  } 
  catch( UseCommandFailOnPlayerException & ex )
  {
    // no go, try from room instead.
    GetCurrentRoom()->Execute(cmd);
  }
}
////////////////////////////////////////////////////////////////////////////////
void
Game::Execute( NullCommand & cmd )
{
  
}
////////////////////////////////////////////////////////////////////////////////

