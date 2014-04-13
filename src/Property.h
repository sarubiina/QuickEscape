// Copyright anssi.grohn@karelia.fi (c) 2014.
// Licensed under GPLv3.
#ifndef __Property_h__
#define __Property_h__
////////////////////////////////////////////////////////////////////////////////
#include <string>
#include <sstream>
#include <stdexcept>
#include <typeinfo>

////////////////////////////////////////////////////////////////////////////////
class Property
{
private:
  std::string	m_strId;
  std::string	m_strValue;

public:

  Property( const std::string & id ) : m_strId (id) {}
  virtual ~Property(){}

  void SetId( const std::string & id ){ m_strId = id; }
  const std::string & GetId() const { return m_strId; }
  /// Type conversion operator. 
  /// Makes is possible to store property value directly into variables.
  template<class TYPE> operator TYPE() {
    return this->As<TYPE>();
  }
  
  template<class TYPE> inline TYPE As() const { 
    TYPE val;							
    std::istringstream ss(m_strValue);				
    if ( ! (ss >> val)) {		
      std::stringstream ss2; ss2 << "Cannot convert " << m_strValue << " to " << typeid(val).name();
      throw std::runtime_error(ss2.str()  );	 
    }
    return val;
  }

  template<class TYPE> void SetValue( TYPE a ) { std::stringstream ss; ss << a; m_strValue = ss.str(); }


};
////////////////////////////////////////////////////////////////////////////////
// we need specialization due to stringstream extracting data only until first whitespace.
template<> inline std::string Property::As<std::string>() const 
{ 
  return m_strValue;                    
}
////////////////////////////////////////////////////////////////////////////////
#endif

