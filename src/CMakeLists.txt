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
	cfg/*.cpp
	# io/*.cpp
	io/exception.cpp
	io/reader.cpp
	io/reader_interface.cpp
	io/types.cpp
	io/hdf5.cpp
	io/writer.cpp
	io/writer_interface.cpp
	plugins/*.cpp
	include/utils.hpp
	include/app/*.hpp
	include/log/*.hpp
	include/physics/*.hpp
	include/tree/*.hpp
	include/cli/*.hpp
	include/cfg/*.hpp
	include/io/*.hpp
	include/plugins/*.hpp
	include/io/exception.hpp
	include/io/reader.hpp
	include/io/reader_interface.hpp
	include/io/types.hpp
	include/io/hdf5.hpp
	include/io/writer.hpp
	include/io/writer_interface.hpp
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
	hdf5
	boost_filesystem
	boost_system
)

install(TARGETS verif++ DESTINATION bin)
