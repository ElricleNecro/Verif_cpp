set(CMAKE_INCLUDE_CURRENT_DIR ON)

#############
# Répertoire à inclure :
########################
include_directories(../include/ ) #${IOGadget_INCLUDE_DIRS})

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

############
# Exécutable à construire :
###########################
add_executable(
	verif++
	${verif_files}
)

target_link_libraries(
	verif++
	dl
	yaml-cpp
)

install(TARGETS verif++ DESTINATION bin)
