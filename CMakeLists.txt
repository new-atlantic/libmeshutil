cmake_minimum_required (VERSION 2.8)
project (meshutil C)
set (meshutil_VERSION_MAJOR 0)
set (meshutil_VERSION_MINOR 0)
set (meshutil_PATCH_VERSION 0)
set (meshutil_VERSION
   ${meshutil_VERSION_MAJOR}.${meshutil_VERSION_MINOR}.${meshutil_PATCH_VERSION})
add_definitions ('-Dmeshutil_VERSION="${meshutil_VERSION}"')

add_library (meshutil SHARED src/batman_adv.c)

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
find_library (CUNIT_LIBRARY cunit)

set (BASH_TEST_DIR "tests/bash/")
set (CUNIT_TEST_DIR "tests/cunit/")

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	add_executable (${BASH_TEST_DIR}bin/kmod_available
                        ${BASH_TEST_DIR}src/kmod_available.c)
        target_link_libraries (${BASH_TEST_DIR}bin/kmod_available meshutil)
	add_executable (${BASH_TEST_DIR}bin/kmod_loaded
                        ${BASH_TEST_DIR}src/kmod_loaded.c)
        target_link_libraries (${BASH_TEST_DIR}bin/kmod_loaded meshutil)
	add_executable (${BASH_TEST_DIR}bin/kmod_version
                        ${BASH_TEST_DIR}src/kmod_version.c)
        target_link_libraries (${BASH_TEST_DIR}bin/kmod_version meshutil)

	add_executable (${BASH_TEST_DIR}bin/if_available_default
                        ${BASH_TEST_DIR}src/if_available_default.c)
        target_link_libraries (${BASH_TEST_DIR}bin/if_available_default meshutil)

	add_executable (${BASH_TEST_DIR}bin/if_available_named
                        ${BASH_TEST_DIR}src/if_available_named.c)
        target_link_libraries (${BASH_TEST_DIR}bin/if_available_named meshutil)

	add_executable (${BASH_TEST_DIR}bin/if_up_default
                        ${BASH_TEST_DIR}src/if_up_default.c)
        target_link_libraries (${BASH_TEST_DIR}bin/if_up_default meshutil)

	add_executable (${BASH_TEST_DIR}bin/if_up_named
                        ${BASH_TEST_DIR}src/if_up_named.c)
        target_link_libraries (${BASH_TEST_DIR}bin/if_up_named meshutil)

	add_executable (${BASH_TEST_DIR}bin/if_hwaddr_default
                        ${BASH_TEST_DIR}src/if_hwaddr_default.c)
        target_link_libraries (${BASH_TEST_DIR}bin/if_hwaddr_default meshutil)

	add_executable (${BASH_TEST_DIR}bin/if_hwaddr_named
                        ${BASH_TEST_DIR}src/if_hwaddr_named.c)
        target_link_libraries (${BASH_TEST_DIR}bin/if_hwaddr_named meshutil)

	add_executable (${BASH_TEST_DIR}bin/n_nodes_if_default
                        ${BASH_TEST_DIR}src/n_nodes_if_default.c)
        target_link_libraries (${BASH_TEST_DIR}bin/n_nodes_if_default meshutil)

	add_executable (${BASH_TEST_DIR}bin/n_nodes_if_named
                        ${BASH_TEST_DIR}src/n_nodes_if_named.c)
        target_link_libraries (${BASH_TEST_DIR}bin/n_nodes_if_named meshutil)

	add_test (bash_batman_adv_test ${BASH_TEST_DIR}runners/batman_adv.sh)

	add_executable (${CUNIT_TEST_DIR}bin/cunit_batman_adv
                        ${CUNIT_TEST_DIR}src/batman_adv_tests.c)
	target_link_libraries (${CUNIT_TEST_DIR}bin/cunit_batman_adv
                               ${CUNIT_LIBRARY} meshutil)
	add_test (cunit_batman_adv_test ${CUNIT_TEST_DIR}bin/cunit_batman_adv)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
