#
# CMakeLists.txt
# author: Jan Dušek <jan.dusek90@gmail.com>

include_directories(
	${SDL2_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS}
	${PROJECT_SOURCE_DIR}/src/utils
)

# on MSVC we use static glew and therefore we need to add proper definitions
if (MSVC)
	add_definitions(/D GLEW_STATIC)
endif()

set(SM_ENGINE_HEADERS
	Renderer.h
	Mesh.h
	Shader.h
	Common.h
	ShaderProgram.h
	Buffer.h
	Material.h
	BaseSceneObject.h
	Scene.h
	Camera.h
	FpsCamera.h
	RenderBatch.h
	UniformBuffer.h
	ShaderManager.h
	Light.h
	Interfaces.h
	BVH.h
	VertexArrayObject.h
	BoundingBoxDrawer.h
	Query.h
)

set(SM_ENGINE_SOURCES
	Renderer.cpp
	Mesh.cpp
	Shader.cpp
	ShaderProgram.cpp
	BaseSceneObject.cpp
	Scene.cpp
	Camera.cpp
	FpsCamera.cpp
	RenderBatch.cpp
	ShaderManager.cpp
	Light.cpp
	BoundingBoxDrawer.cpp
)

add_library(engine ${SM_ENGINE_SOURCES} ${SM_ENGINE_HEADERS})
target_link_libraries(engine utils ${SDL2_LIBRARY} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES})