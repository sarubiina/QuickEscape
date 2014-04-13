// Copyright anssi.grohn@karelia.fi (c) 2014.
// Licensed under GPLv3.
#ifndef __Inventory_h__
#define __Inventory_h__
#include <iostream>
#include <string>
#include <list>
////////////////////////////////////////////////////////////////////////////////
class GameObject;
typedef std::list<GameObject *>  GameObjectList;
////////////////////////////////////////////////////////////////////////////////
class Inventory : public GameObjectList 
{
public:
  // For easier access from inventory object.
  GameObject * operator[]( const std::string & name ) const;
  void Add( GameObject *pItem );
  void Remove( GameObject *pItem );
  void Remove ( const std::string & name );
};
std::ostream & operator<<( std::ostream & stream, const Inventory & i );
////////////////////////////////////////////////////////////////////////////////
#endif
