set(CMAKE_INCLUDE_CURRENT_DIR ON)

file(
	GLOB
	pi_files
	*.cpp
)

file(
	GLOB
	pi_include
	*.cpp
)

add_library(
	verif_plugin
	SHARED
	${pi_files}
	${pi_include}
)

target_link_libraries(
	verif_plugin
	dl
)

install(TARGETS verif_plugin DESTINATION lib)
install(FILES ${plugin_include} DESTINATION include/verif)

