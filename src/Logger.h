// Copyright anssi.grohn@karelia.fi (c) 2014.
// Licensed under GPLv3.
#ifndef __Logger_h__
#define __Logger_h__
#include <ios>
#include <fstream>
#include <string>
#include <cstddef>
#include <ctime>
////////////////////////////////////////////////////////////////////////////////
class Logger : public std::ofstream
{
protected:

  static Logger * logger;
  Logger() {}

public:
  
  static Logger & GetInstance()
  {
    if ( logger == NULL ) 
      logger = new Logger();
    return *logger;
  }

  static void Destroy()
  {
    if (logger && logger->is_open()) 
    {
      logger->flush();
      logger->close();
    }
    delete logger;
    logger = NULL;
  }
  
  static std::string Time();

};
////////////////////////////////////////////////////////////////////////////////
#define g_Log   (Logger::GetInstance() << "\n" << std::unitbuf << Logger::Time() << "    <LOG> " )
#define g_Error (Logger::GetInstance() << "\n" << std::unitbuf << Logger::Time() << "  <ERROR> " )
#define g_Warn  (Logger::GetInstance() << "\n" << std::unitbuf << Logger::Time() << "<WARNING> " )
////////////////////////////////////////////////////////////////////////////////
#endif
