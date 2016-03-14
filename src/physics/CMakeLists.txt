set(CMAKE_INCLUDE_CURRENT_DIR ON)

file(
	GLOB
	physics_files
	*.cpp
)

file(
	GLOB
	physics_include
	*.cpp
)

add_library(
	verif_physics
	SHARED
	${physics_files}
	${physics_include}
)

target_link_libraries(
	verif_physics
)

install(TARGETS verif_physics DESTINATION lib)
install(FILES ${physics_include} DESTINATION include/verif)

