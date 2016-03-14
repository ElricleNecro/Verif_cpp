set(CMAKE_INCLUDE_CURRENT_DIR ON)

file(
	GLOB
	log_files
	*.cpp
)

file(
	GLOB
	log_include
	*.cpp
)

add_library(
	verif_log
	SHARED
	${log_files}
	${log_include}
)

target_link_libraries(
	verif_log
)

install(TARGETS verif_log DESTINATION lib)
install(FILES ${log_include} DESTINATION include/verif)

