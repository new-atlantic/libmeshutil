set(EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/bin/")

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	add_executable (cunit_batman_adv src/batman_adv_tests.c)
	target_link_libraries (cunit_batman_adv meshutil cunit)
	add_test (cunit_batman_adv_test cunit_batman_adv)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
