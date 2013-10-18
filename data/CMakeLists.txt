#
# CMakeLists.txt
# author: Jan Dušek <jan.dusek90@gmail.com>

configure_file(shaders/simple.vert
	${CMAKE_CURRENT_BINARY_DIR}/shaders/simple.vert COPYONLY)

configure_file(shaders/simple.frag
	${CMAKE_CURRENT_BINARY_DIR}/shaders/simple.frag COPYONLY)


configure_file(shaders/phong.vert
	${CMAKE_CURRENT_BINARY_DIR}/shaders/phong.vert COPYONLY)

configure_file(shaders/phong.frag
	${CMAKE_CURRENT_BINARY_DIR}/shaders/phong.frag COPYONLY)

