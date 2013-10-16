#
# Try to find GLEW library and include path.
# Once done this will define
#
# GLEW_FOUND
# GLEW_INCLUDE_DIRS
# GLEW_LIBRARIES
# 

IF (MSVC)
	FIND_PATH( GLEW_INCLUDE_DIRS GL/glew.h
		$ENV{PROGRAMFILES}/GLEW/include
		$ENV{GLEW_ROOT}/include
		DOC "The directory where GL/glew.h resides")

	# prefer static libs
    FIND_LIBRARY( GLEW_RELEASE_LIBRARY
        NAMES glew32s glew32
        PATHS
        $ENV{PROGRAMFILES}/GLEW/lib
        $ENV{GLEW_ROOT}/lib
        DOC "The GLEW optimized library")
		
	FIND_LIBRARY (GLEW_DEBUG_LIBRARY
		NAMES glew32sd glew32d
		PATHS
		$ENV{PROGRAMFILES}/GLEW/lib
		$ENV{GLEW_ROOT}/lib
		DOC "The GLEW debug library")
		
	SET(GLEW_LIBRARIES debug ${GLEW_DEBUG_LIBRARY} optimized ${GLEW_RELEASE_LIBRARY})
ELSE (MSVC)
	FIND_PATH( GLEW_INCLUDE_DIRS GL/glew.h
		/usr/include
		/usr/local/include
		/sw/include
		/opt/local/include
		$ENV{GLEW_ROOT}/include
		DOC "The directory where GL/glew.h resides")

	FIND_LIBRARY( GLEW_LIBRARIES
		NAMES libGLEW GLEW libglew glew libglew32
		PATHS
		/usr/lib64
		/usr/lib
		/usr/local/lib64
		/usr/local/lib
		/sw/lib
		/opt/local/lib
		$ENV{GLEW_ROOT}/lib
		DOC "The GLEW library")
ENDIF (MSVC)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GLEW DEFAULT_MSG GLEW_INCLUDE_DIRS GLEW_LIBRARIES)