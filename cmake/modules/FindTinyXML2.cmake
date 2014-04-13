# - Find tinyxml2 source/include folder
# This module finds tinyxml2 if it is installed and determines where
# the files are. This code sets the following variables:
#
#  TINYXML2_FOUND             - have tinyxml2 been found
#  TINYXML2_INCLUDE_DIR       - path to where tinyxml2.h is found
#
FIND_PATH(TINYXML2_INCLUDE_DIR tinyxml2.h
  HINTS $ENV{TINYXML2DIR}
  PATH_SUFFIXES include
  PATHS ~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  /usr
  ${CMAKE_SOURCE_DIR}/deps
  )
find_library(TINYXML2_LIBRARY
        tinyxml2
        HINTS $ENV{TINYXML2DIR}
        PATH_SUFFIXES lib64 lib
        PATHS ~/Library/Frameworks
         /Library/Frameworks
         /usr/local
         /usr
	 ${CMAKE_SOURCE_DIR}/deps
)
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(TINYXML2 DEFAULT_MSG TINYXML2_LIBRARY)

mark_as_advanced(TINYXML2_LIBRARY TINYXML2_INCLUDE_DIR)

