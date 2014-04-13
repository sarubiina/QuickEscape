// Copyright anssi.grohn@karelia.fi (c) 2014.
// Licensed under GPLv3.
#include <Inventory.h>
#include <list>
#include <algorithm>
#include <GameObject.h>
#include <Game.h>
#include <boost/regex.hpp>
////////////////////////////////////////////////////////////////////////////////
using namespace std;
////////////////////////////////////////////////////////////////////////////////
GameObject * 
Inventory::operator[]( const std::string & name) const
{
  // case-insensitive match
  string tmp = name;
  // convert string to lowercase
  transform( tmp.begin(), tmp.end(), tmp.begin(), [](char c) -> char { return tolower(c); } );
  for(auto a : *this )
  {
    string item = a->GetName();
    // convert string to lowercase
    transform( item.begin(), item.end(), item.begin(), [](char c) -> char { return tolower(c); });

      if ( a->GetNamePattern().empty() == false && 
	   boost::regex_match( tmp, a->GetNamePattern())) return a;
      if ( tmp == item ) return a;
  }
  return NULL;
} 
////////////////////////////////////////////////////////////////////////////////
std::ostream & operator<<( std::ostream & stream, const Inventory & inv )
{
  std::list<string> tmp;
  for ( auto i : inv ){
    tmp.push_back(i->GetName());
  }
  stream << Game::MakeReadable(tmp);
  return stream;
}
///////////////////////////////////////////////////////////////////////////////
void 
Inventory::Remove( const std::string & name )
{
  Inventory::iterator it;
  for( it = begin(); it!= end(); it++)
  {
    if ( (*it)->GetName() == name ) break;
  }
  if ( it != end()) erase(it);
}
////////////////////////////////////////////////////////////////////////////////
