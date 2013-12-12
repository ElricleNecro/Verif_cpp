# vim: spelllang=
# Version minimum de CMake qui est nécessaire pour compiler :
cmake_minimum_required(VERSION 2.6)

#########
# Nom du projet :
#################
project(VerificationProgram)

set(PLUGIN_API_VERSION_MAJOR 1)
set(PLUGIN_API_VERSION_MINOR 0)

set(VERIF_VERSION_MAJOR 0)
set(VERIF_VERSION_MINOR 1)

########################
# On a besoin de quelques fichiers non-standards :
##################################################
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/.cmake/Modules/")

#################
# Quelques flags de compilation :
#################################
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -W -Wextra -Wall -Wc++11-compat -Wpedantic -Wpadded -Woverflow -Wmissing-declarations -Wmaybe-uninitialized -Winline -Wfloat-equal -Wdiv-by-zero -Wdeprecated -Wdeprecated-declarations -Wcast-align -Wcast-qual -Wbuiltin-macro-redefined -ftrapv -ffinite-math-only -fdelete-null-pointer-checks")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -W -Wextra -Wall -Wc++11-compat -Wpedantic -Woverflow -Wmissing-declarations -Wuninitialized -Winline -Wfloat-equal -Wdiv-by-zero -Wdeprecated -Wdeprecated-declarations -Wcast-align -Wcast-qual -Wbuiltin-macro-redefined -ftrapv -ffinite-math-only")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG} -g -pg")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} -O3 -pg")

IF(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
	SET(CMAKE_BUILD_TYPE Release)#Debug) # Release)
ENDIF(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)

# The macro below forces the build directory to be different from source directory:
include(MacroOutOfSourceBuild)
macro_ensure_out_of_source_build("${PROJECT_NAME} requires an out of source build.")

add_subdirectory(src)
#add_subdirectory(src/test)

#########
# Configuration :
#################
configure_file(config.h.cmake ${CMAKE_SOURCE_DIR}/include/define.h)

