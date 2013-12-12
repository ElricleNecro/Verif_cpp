set(CMAKE_INCLUDE_CURRENT_DIR ON)

########
# Dépendances :
###############
#find_package(IOGadget)

#############
# Répertoire à inclure :
########################
include_directories(../include/ ) #${IOGadget_INCLUDE_DIRS})
#link_directories(${IOGadget_LIBRARY_DIRS})

#message(STATUS "${IOGadget_LIBRARIES}")

####################
# Liste des fichiers sources :
##############################
file(
	GLOB
	verif_files
	main.cpp
	utils.cpp
	app/*.cpp
	log/*.cpp
	physics/*.cpp
	tree/*.cpp
	cli/*.cpp
	io/*.cpp
	plugins/*.cpp
	include/utils.hpp
	include/app/*.hpp
	include/log/*.hpp
	include/physics/*.hpp
	include/tree/*.hpp
	include/cli/*.hpp
	include/io/*.hpp
	include/plugins/*.hpp
)

#install(TARGETS verif++ DESTINATION bin)

############
# Exécutable à construire :
###########################
add_executable(
	verif++
	${verif_files}
)

target_link_libraries(
	verif++
	#${IOGadget_LIBRARIES}
	dl
)

install(TARGETS verif++ DESTINATION bin)
