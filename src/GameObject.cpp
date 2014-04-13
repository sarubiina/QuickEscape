// Copyright anssi.grohn@karelia.fi (c) 2014.
// Licensed under GPLv3.
#include "GameObject.h"
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <Logger.h>
using namespace std;
using namespace tinyxml2;
////////////////////////////////////////////////////////////////////////////////
GameObject::GameObject()
{
  
}
////////////////////////////////////////////////////////////////////////////////
GameObject::~GameObject()
{
  
}
////////////////////////////////////////////////////////////////////////////////
void 
GameObject::SetName( string name) 
{
 m_strName = name;
}
////////////////////////////////////////////////////////////////////////////////
const string &
GameObject::GetName() const
{
  return m_strName;
}

////////////////////////////////////////////////////////////////////////////////
void 
GameObject::SetId(const string & id )
{
  m_id = id;
}
////////////////////////////////////////////////////////////////////////////////
const std::string & 
GameObject::GetId() const
{
  return m_id;
}
////////////////////////////////////////////////////////////////////////////////
Inventory & 
GameObject::GetItems()
{
  return m_inventory;
}
////////////////////////////////////////////////////////////////////////////////
void 
GameObject::LoadProperties( XMLElement & e )
{
  XMLElement * prop = e.FirstChildElement("Property");

  while ( prop ){
    g_Log << "Property id is: " << prop->Attribute("id") << "\n";
    if ( !prop->Attribute("id") ) throw AttributeMissingException("Property - No id attribute");
    if ( !prop->Attribute("type") ) throw AttributeMissingException("Property - No type attribute");
    if ( !prop->Attribute("value") ) throw AttributeMissingException("Property - No value attribute");
    string type = prop->Attribute("type");
    
    if ( type == "bool" )		AddProperty( prop->Attribute("id"), prop->BoolAttribute("value"));
    else if ( type == "int" )	AddProperty( prop->Attribute("id"), prop->IntAttribute("value"));
    else if ( type == "float" )	AddProperty( prop->Attribute("id"), prop->FloatAttribute("value"));
    else if ( type == "string" )	AddProperty( prop->Attribute("id"), prop->Attribute("value"));
    else throw InvalidAttributeException("Invalid attribute type defined!");
    prop = prop->NextSiblingElement("Property");
  }
}
////////////////////////////////////////////////////////////////////////////////
void 
GameObject::LoadItems( tinyxml2::XMLElement & items )
{
  XMLElement * item = items.FirstChildElement("Item");
  while ( item ){


    GameObject *pItem = new GameObject();
    try {
      pItem->Load(*item);
      GetItems().push_back(pItem);
    } 
    catch ( std::exception & ex )
    {
      g_Error << "Could not load item" << ex.what();
      delete pItem;
    }    

    item = item->NextSiblingElement("Item");

  }
}
////////////////////////////////////////////////////////////////////////////////
void 
GameObject::Load( XMLElement & e )
{
  const char * attrib = e.Attribute("id");  
  if ( !attrib ) throw AttributeMissingException("Attribute id is missing!");
  SetId(attrib);

  attrib = e.Attribute("description");
  if ( !attrib ) throw AttributeMissingException("Attribute description is missing!");
  SetDescription(attrib);

  attrib = e.Attribute("name");
  if ( !attrib ) throw AttributeMissingException("Attribute name is missing!");
  SetName(attrib);
  
  attrib = e.Attribute("match");
  if ( attrib) 
  {
    SetNamePattern(attrib);
  }
  
  // Load properties 
  XMLElement *props = e.FirstChildElement("Properties");
  if ( props )  LoadProperties( *props);

  XMLElement *items = e.FirstChildElement("Items");
  if ( items ) LoadItems(*items);
  else g_Log << "There were no items...\n";
}
////////////////////////////////////////////////////////////////////////////////
void 
GameObject::SetDescription( const std::string & desc )
{
  m_strDescription = desc;
}
////////////////////////////////////////////////////////////////////////////////
const std::string & 
GameObject::GetDescription() const
{
  return m_strDescription;
}
////////////////////////////////////////////////////////////////////////////////
void
GameObject::Save( XMLPrinter & p )
{
  p.OpenElement("Item");
  p.PushAttribute("name", GetName().c_str());
  p.PushAttribute("id", GetId().c_str());
  p.PushAttribute("description", GetDescription().c_str());
  p.PushAttribute("match", GetNamePattern().str().c_str());
  SaveProperties(p);

  p.CloseElement();
}
////////////////////////////////////////////////////////////////////////////////
void
GameObject::SaveProperties( XMLPrinter & p )
{
  p.OpenElement("Properties");
  for(auto property : GetProperties())
  {
    p.OpenElement("Property");
    p.PushAttribute("id",property.second->GetId().c_str());
    p.PushAttribute("type", "string");
    string tmp = (*property.second).As<string>();
    p.PushAttribute("value", tmp.c_str());
    p.CloseElement();
  }
  p.CloseElement();
}
////////////////////////////////////////////////////////////////////////////////
void 
GameObject::SetNamePattern( const std::string & pattern )
{
  m_regMatchExpression.assign(pattern);
}
////////////////////////////////////////////////////////////////////////////////
boost::regex & 
GameObject::GetNamePattern()
{
  return m_regMatchExpression;
}
////////////////////////////////////////////////////////////////////////////////
