set(CMAKE_INCLUDE_CURRENT_DIR ON)

####################
# Liste des fichiers sources :
##############################
file(
	GLOB
	histogram_files
	../utils.cpp
	histogramme.cpp
	include/utils.hpp
	include/physics/histogramme.hpp
)

#install(TARGETS verif++ DESTINATION bin)

############
# Exécutable à construire :
###########################
add_executable(
	histogram
	${histogram_files}
)

target_link_libraries(
)

install(TARGETS verif++ DESTINATION bin)
