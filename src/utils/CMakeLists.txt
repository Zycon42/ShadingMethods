#
# CMakeLists.txt
# author: Jan Dušek <jan.dusek90@gmail.com>

include_directories(
	${GLM_INCLUDE_DIRS}
)

set(SM_UTILS_HEADERS 
	Exception.h
	Logging.h
	ArrayRef.h
	Plane.h
	Frustum.h
	BoundingBox.h
)

set(SM_UTILS_SOURCES
	Exception.cpp
	Logging.cpp
	Frustum.cpp
	BoundingBox.cpp
)

# add win32 specific files
if(WIN32)
	set(SM_UTILS_SOURCES ${SM_UTILS_SOURCES} Exception-win32.cpp)
endif()

add_library(utils STATIC ${SM_UTILS_SOURCES} ${SM_UTILS_HEADERS})
target_link_libraries(utils)
