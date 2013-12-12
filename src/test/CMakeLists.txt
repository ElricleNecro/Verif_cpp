set(CMAKE_INCLUDE_CURRENT_DIR ON)

########
# Dépendances :
###############
find_package(IOGadget)

#############
# Répertoire à inclure :
########################
include_directories(../../include/ ${IOGadget_INCLUDE_DIRS})
link_directories(${IOGadget_LIBRARY_DIRS})

set(test_build_files ../tree.cpp test_build.cpp)
set(test_dc_files ../tree.cpp ../tree_physics.cpp test_dc.cpp)
set(test_pot_files ../tree.cpp ../tree_physics.cpp test_pot.cpp)
set(test_iog_files ../io/gadget.cpp ../io/exception.cpp ../io/types.cpp gadget_io.cpp)

##########
# Cible à installer :
#####################
#install(TARGETS verif++ DESTINATION bin)

add_executable(
	test_build
	${test_build_files}
)

target_link_libraries(
	test_build
	${IOGadget_LIBRARIES}
)

add_executable(
	test_dc
	${test_dc_files}
)

target_link_libraries(
	test_dc
	${IOGadget_LIBRARIES}
)

add_executable(
	test_pot
	${test_pot_files}
)

target_link_libraries(
	test_pot
	${IOGadget_LIBRARIES}
)

add_executable(
	test_iog
	${test_iog_files}
)

target_link_libraries(
	test_iog
	${IOGadget_LIBRARIES}
)

