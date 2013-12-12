set(CMAKE_INCLUDE_CURRENT_DIR ON)

file(
	GLOB
	io_files
	*.cpp
)

file(
	GLOB
	io_include
	*.cpp
)

add_library(
	verif_io
	STATIC
	${io_files}
	${io_include}
)

target_link_libraries(
	verif_io
	hdf5_cpp
	hdf5
)

install(TARGETS verif_io DESTINATION lib)
#install(FILES ${meg_include} DESTINATION include)

