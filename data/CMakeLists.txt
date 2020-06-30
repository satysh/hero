# Create a library called "libdata" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

set(INCLUDE_DIRECTORIES
${BASE_INCLUDE_DIRECTORIES}
${Boost_INCLUDE_DIRS}
${ROOT_INCLUDE_DIR}
${CMAKE_SOURCE_DIR}/data
${CMAKE_SOURCE_DIR}/base
)

include_directories( ${INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${BASE_LINK_DIRECTORIES}
${FAIRROOT_LIBRARY_DIR}
)

link_directories( ${LINK_DIRECTORIES})

set(SRCS
HEROPoint.cxx
HEROStack.cxx
HEROMCTrack.cxx
HERODigi.cxx
)

# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")
SET(HEADERS ${HEADERS})
set(LINKDEF  HERODataLinkDef.h)
set(LIBRARY_NAME HEROData)
set(DEPENDENCIES Base Core)

GENERATE_LIBRARY()
