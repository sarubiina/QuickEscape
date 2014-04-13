// Copyright anssi.grohn@karelia.fi (c) 2014.
// Licensed under GPLv3.
#include <PropertyHolder.h>
#include <stdexcept>
#include <sstream>
using namespace std;
////////////////////////////////////////////////////////////////////////////////
Property &
PropertyHolder::GetProperty(const std::string & id ) throw (std::runtime_error) {
  Properties::iterator it = m_Properties.find(id);
  
  if ( it == m_Properties.end()) 
  {
    stringstream ss;
    ss << "no such property '" << id << "'";
    throw runtime_error(ss.str());
  }
  return *(*it).second;
}
////////////////////////////////6:3////////////////////////////////////////////////
Properties &
PropertyHolder::GetProperties() 
{
  return m_Properties;
}
////////////////////////////////////////////////////////////////////////////////
Property & 
PropertyHolder::operator[](const std::string & id )
{
  return GetProperty(id);
}
////////////////////////////////////////////////////////////////////////////////
bool   
PropertyHolder::HasProperty( const std::string & id )
{
  return (m_Properties.find(id) != m_Properties.end());
}
////////////////////////////////////////////////////////////////////////////////
