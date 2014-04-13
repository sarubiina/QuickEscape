// Copyright anssi.grohn@karelia.fi (c) 2014.
// Licensed under GPLv3.
#ifndef __globals_h__
#define __globals_h__

#include <stdexcept>
// runtime_error is inherited from std::exception...
#define MAKE_EXCEPTION_TYPE( NAME )					\
struct NAME : std::runtime_error				\
{								\
  NAME(const std::string & msg ) : std::runtime_error(msg){}    \
};
MAKE_EXCEPTION_TYPE(NullKeyException);
MAKE_EXCEPTION_TYPE(XmlParsingException);
MAKE_EXCEPTION_TYPE(InvalidAttributeException);
MAKE_EXCEPTION_TYPE(AttributeMissingException);
MAKE_EXCEPTION_TYPE(ElementMissingException);
MAKE_EXCEPTION_TYPE(DuplicateRoomIdException);
MAKE_EXCEPTION_TYPE(UseCommandFailOnPlayerException);
MAKE_EXCEPTION_TYPE(ExamineCommandFailOnPlayerException);
enum Direction { North, South, East, West, kNumDirs };

#endif
