#
# CMakeLists.txt
# author: Jan Dušek <jan.dusek90@gmail.com>

include_directories(
	${SDL2_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS}
	${PROJECT_SOURCE_DIR}/src/utils ${PROJECT_SOURCE_DIR}/src/engine
)

# on MSVC we use static glew and therefore we need to add proper definitions
if (MSVC)
	add_definitions(/D GLEW_STATIC)
endif()

set(SM_APPLICATION_HEADERS
	Application.h
	KeyboardHandler.h
)

set(SM_APPLICATION_SOURCES
	Application.cpp
	main.cpp
)

# on windows start with WinMain()
add_executable(${PROJECT_NAME} WIN32 ${SM_APPLICATION_SOURCES} ${SM_APPLICATION_HEADERS})
target_link_libraries(${PROJECT_NAME} engine utils ${SDL2_LIBRARY} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES})

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
