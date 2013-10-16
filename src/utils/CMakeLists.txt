#
# CMakeLists.txt
# author: Jan Dušek <jan.dusek90@gmail.com>

set(SM_UTILS_HEADERS 
	Exception.h
	File.h
	Logging.h
	Variant.h
	ArrayRef.h
)

set(SM_UTILS_SOURCES
	Exception.cpp
	File.cpp
	Logging.cpp
)

# add win32 specific files
if(WIN32)
	set(SM_UTILS_SOURCES ${SM_UTILS_SOURCES} Exception-win32.cpp)
endif()

add_library(utils STATIC ${SM_UTILS_SOURCES} ${SM_UTILS_HEADERS})
target_link_libraries(utils)
