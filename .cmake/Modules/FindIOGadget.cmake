# - Try to find ImageIOGadget
# Once done, this will define
#
#  IOGadget_FOUND - system has IOGadget
#  IOGadget_INCLUDE_DIRS - the IOGadget include directories
#  IOGadget_LIBRARIES - link these to use IOGadget

include(LibFindMacros)

# Dependencies
#libfind_package(IOGadget IOGadget)

# Use pkg-config to get hints about paths
libfind_pkg_check_modules(IOGadget_PKGCONF iogadget)

# Include dir
find_path(IOGadget_INCLUDE_DIR
	NAMES IOGadget/gadget.h
  PATHS ${IOGadget_PKGCONF_INCLUDE_DIRS}
)

# Finally the library itself
find_library(IOGadget_LIBRARY
	NAMES IOGadget
	PATHS ${IOGadget_PKGCONF_LIBRARY_DIRS}
)

# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
set(IOGadget_PROCESS_INCLUDES IOGadget_INCLUDE_DIR IOGadget_INCLUDE_DIRS)
set(IOGadget_PROCESS_LIBS IOGadget_LIBRARY IOGadget_LIBRARIES)
libfind_process(IOGadget)

