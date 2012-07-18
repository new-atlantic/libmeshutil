cmake_minimum_required (VERSION 2.8)
project (meshutil C)
set (meshutil_VERSION_MAJOR 0)
set (meshutil_VERSION_MINOR 0)
set (meshutil_PATCH_VERSION 0)
set (meshutil_VERSION
   ${meshutil_VERSION_MAJOR}.${meshutil_VERSION_MINOR}.${meshutil_PATCH_VERSION})
add_definitions ('-Dmeshutil_VERSION="${meshutil_VERSION}"')

add_library (meshutil SHARED src/batman_adv.c)

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -Wall -Wextra -fpic")
if (CMAKE_COMPILER_IS_GNUCC)
	set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
endif (CMAKE_COMPILER_IS_GNUCC)

install (TARGETS meshutil
         LIBRARY DESTINATION lib)
install(DIRECTORY src/ DESTINATION include/meshutil
          FILES_MATCHING PATTERN "*.h")

enable_testing ()
find_library (CUNIT_LIBRARY cunit)

set (test_BIN_DIR "tests/bin/")
#add_executable (${test_BIN_DIR}cunit_meshutil tests/cunit/meshutil_tests.c)
#target_link_libraries (${test_BIN_DIR}cunit_meshutil ${CUNIT_LIBRARY} meshutil)
#add_test (cunit_meshutil_test ${test_BIN_DIR}cunit_meshutil)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	add_executable (${test_BIN_DIR}cunit_batman_adv tests/cunit/batman_adv_tests.c)
	target_link_libraries (${test_BIN_DIR}cunit_batman_adv ${CUNIT_LIBRARY})
	add_test (cunit_batman_adv_test ${test_BIN_DIR}cunit_batman_adv)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
