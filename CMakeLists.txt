cmake_minimum_required (VERSION 2.8)
project (meshutil)
set (meshutil_VERSION_MAJOR 0)
set (meshutil_VERSION_MINOR 1)
set (meshutil_PATCH_VERSION 0)
set (meshutil_VERSION
   "${meshutil_VERSION_MAJOR}.${meshutil_VERSION_MINOR}.${meshutil_PATCH_VERSION}")

add_library (meshutil SHARED src/batman_adv/KernelModule.cpp)
add_library (meshutil_static STATIC src/batman_adv/KernelModule.cpp)

set (HARDENING_FLAGS "-Wformat -Wformat-security -Werror=format-security -D_FORTIFY_SOURCE=2 -fstack-protector --param ssp-buffer-size=4 -Wl,-z,now -Wl,-z,relro")

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall -Wextra -fpic -fvisibility=hidden ${HARDENING_FLAGS}")
if (CMAKE_COMPILER_IS_GNUCXX)
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif (CMAKE_COMPILER_IS_GNUCXX)

install (TARGETS meshutil LIBRARY DESTINATION lib)
install (FILES src/batman_adv/KernelModule.hpp DESTINATION include/meshutil/batman_adv/)
