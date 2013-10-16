# FindGLM - attempts to locate the glm matrix/vector library.
#
# This module defines the following variables (on success):
#   GLM_INCLUDE_DIRS  - where to find glm/glm.hpp
#   GLM_FOUND         - if the library was successfully located
#
# It is trying a few standard installation locations, but can be customized
# with the following variables:
#   GLM_ROOT_DIR      - root directory of a glm installation
#                       Headers are expected to be found in either:
#                       <GLM_ROOT_DIR>/glm/glm.hpp           OR
#                       <GLM_ROOT_DIR>/include/glm/glm.hpp
#                       This variable can either be a cmake or environment
#                       variable.

SET(GLM_ENV_ROOT_DIR $ENV{GLM_ROOT_DIR})

IF(NOT GLM_ROOT_DIR AND GLM_ENV_ROOT_DIR)
	SET(GLM_ROOT_DIR ${GLM_ENV_ROOT_DIR})
ENDIF()

SET(GLM_SEARCH_DIRS 
	"/usr/include"
	"/usr/local/include"
)
IF(GLM_ROOT_DIR)
	SET(GLM_SEARCH_DIRS 
		"${GLM_ROOT_DIR}"
		"${GLM_ROOT_DIR}/include"
		${GLM_SEARCH_DIRS}
	)
ENDIF(GLM_ROOT_DIR)

FIND_PATH(GLM_INCLUDE_DIR "glm/glm.hpp" PATHS ${GLM_SEARCH_DIRS})

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GLM DEFAULT_MSG GLM_INCLUDE_DIR)
	
IF(GLM_FOUND)
	SET(GLM_INCLUDE_DIRS ${GLM_INCLUDE_DIR})
ENDIF(GLM_FOUND)
