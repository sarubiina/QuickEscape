// Copyright anssi.grohn@karelia.fi (c) 2014.
// Licensed under GPLv3.
#ifndef __GameObject_h__
#define __GameObject_h__
#include <iostream>
#include <string>
#include <Updateable.h>
#include <PropertyHolder.h>
#include <globals.h>
#include <list>
#include <tinyxml2.h>
#include <Inventory.h>
#include <boost/regex.hpp>
////////////////////////////////////////////////////////////////////////////////
class GameObject : public Updateable,
		   public PropertyHolder

{
private:
  std::string	 m_strName;
  std::string	 m_id;
  std::string	 m_strDescription;
  boost::regex   m_regMatchExpression;
  Inventory	 m_inventory;
protected:

  void LoadProperties( tinyxml2::XMLElement & properties );
  void LoadItems( tinyxml2::XMLElement & items );
  void SaveProperties( tinyxml2::XMLPrinter & p );
public:

  GameObject();
  virtual ~GameObject();
  void SetName( std::string name) ;
  const std::string & GetName() const;
  
  void SetNamePattern( const std::string & pattern );
  boost::regex & GetNamePattern();
  void SetId(const std::string & id );
  const std::string & GetId() const;
  void Update(){}

  Inventory & GetItems();
  

  void SetDescription( const std::string & desc );
  const std::string & GetDescription() const;
  void Load( tinyxml2::XMLElement & elem );
  virtual void Save( tinyxml2::XMLPrinter & printer );

};

#endif
