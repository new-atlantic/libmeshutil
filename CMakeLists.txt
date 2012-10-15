cmake_minimum_required (VERSION 2.8)
project (meshutil C)
set (meshutil_VERSION_MAJOR 0)
set (meshutil_VERSION_MINOR 0)
set (meshutil_PATCH_VERSION 1)
set (meshutil_VERSION
   "${meshutil_VERSION_MAJOR}.${meshutil_VERSION_MINOR}.${meshutil_PATCH_VERSION}")

add_library (meshutil SHARED src/batman_adv.c)
add_library (meshutil_static STATIC src/batman_adv.c)

set (HARDENING_FLAGS "-Wformat -Wformat-security -Werror=format-security -D_FORTIFY_SOURCE=2 -fstack-protector --param ssp-buffer-size=4 -Wl,-z,now -Wl,-z,relro")

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -Wall -Wextra -fpic ${HARDENING_FLAGS}")
if (CMAKE_COMPILER_IS_GNUCC)
	set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
endif (CMAKE_COMPILER_IS_GNUCC)

install (TARGETS meshutil
         LIBRARY DESTINATION lib)
install(DIRECTORY src/ DESTINATION include/meshutil
          FILES_MATCHING PATTERN "*.h")

enable_testing ()
add_subdirectory("tests/bash")
find_library (CUNIT_LIBRARY cunit)
add_subdirectory("tests/cunit/")
