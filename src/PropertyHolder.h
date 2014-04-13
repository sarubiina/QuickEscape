// Copyright anssi.grohn@karelia.fi (c) 2014.
// Licensed under GPLv3.
#ifndef __PropertyHolder_h__
#define __PropertyHolder_h__
////////////////////////////////////////////////////////////////////////////////
#include <string>
#include <globals.h>
#include <map>
#include <Property.h>
#include <stdexcept>
typedef std::map<std::string, Property *> Properties;
////////////////////////////////////////////////////////////////////////////////
class PropertyHolder 
{
private:
  Properties  m_Properties;
public:
  Property & operator[](const std::string & id );
  bool   HasProperty( const std::string & id );
  template<class TYPE> void AddProperty( const std::string & id, const TYPE & val );
  Property & GetProperty(const std::string & id ) throw (std::runtime_error);
  Properties & GetProperties();
  template<class TYPE> void SetProperty( const std::string & id,  const TYPE & val );
};
////////////////////////////////////////////////////////////////////////////////
template<class TYPE> 
void 
PropertyHolder::AddProperty( const std::string & id, const TYPE & val ) {

  Property *p = new Property(id);
  p->SetValue( val );
  if ( m_Properties.find(id) == m_Properties.end()){
    m_Properties[id] = p;
  } else {
    throw std::runtime_error(std::string(__FUNCTION__) + std::string("Duplicate property found!"));
  }
}
////////////////////////////////////////////////////////////////////////////////
template<class TYPE> 
void 
PropertyHolder::SetProperty( const std::string & id, const TYPE & val ) 
{
  if ( m_Properties.find(id) != m_Properties.end())
  {
    m_Properties[id]->SetValue(val);
  } 
  else 
  {
    Property *p= new Property(id);
    p->SetValue(val);
    m_Properties[id] = p;
  }
}
////////////////////////////////////////////////////////////////////////////////
#endif
