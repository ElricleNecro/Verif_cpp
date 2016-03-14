set(CMAKE_INCLUDE_CURRENT_DIR ON)

file(
	GLOB
	tree_files
	*.cpp
)

file(
	GLOB
	tree_include
	*.cpp
)

add_library(
	verif_tree
	SHARED
	${tree_files}
	${tree_include}
)

target_link_libraries(
	verif_tree
)

install(TARGETS verif_tree DESTINATION lib)
install(FILES ${tree_include} DESTINATION include/verif)

