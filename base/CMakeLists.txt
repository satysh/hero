# Create a library called "libOLVDetectors" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

set(INCLUDE_DIRECTORIES
${BASE_INCLUDE_DIRECTORIES}
${ROOT_INCLUDE_DIR}
${GEANT4VMC_INCLUDE_DIR}
${GEANT4_INCLUDE_DIR}
${Boost_INCLUDE_DIRS}
${CMAKE_SOURCE_DIR}/data
${CMAKE_SOURCE_DIR}/base
${CMAKE_SOURCE_DIR}/passive
)

include_directories( ${INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${BASE_LINK_DIRECTORIES}
${GEANT4_LIB_DIRS}
${FAIRROOT_LIBRARY_DIR}
${GEANT4VMC_LIBRARY_DIR}
) 

link_directories( ${LINK_DIRECTORIES})

set(SRCS
OLVDetector.cxx
OLVDetectorGeoPar.cxx
)

# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")

Set(LINKDEF OLVBaseLinkDef.h)
Set(LIBRARY_NAME OLVBase)
Set(DEPENDENCIES Base Core)

GENERATE_LIBRARY()

