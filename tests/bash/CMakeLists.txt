IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

  include_directories (${meshutil_SOURCE_DIR}/src)

  set(BASH_TEST_BINARIES kmod_available kmod_loaded kmod_version
                         if_available_default if_available_named
                         if_hwaddr_default if_hwaddr_named
                         if_up_default if_up_named
                         n_nodes_if_default n_nodes_if_named
                         node_addresses_if_default node_addresses_if_named
                         actual_next_hop_addr_default actual_next_hop_addr_named
                         potential_next_hop_addr_default potential_next_hop_addr_named)

  set(EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/bin/")

  foreach(test_bin ${BASH_TEST_BINARIES})
    add_executable (${test_bin} src/${test_bin}.c)
    target_link_libraries (${test_bin} meshutil)
  endforeach(test_bin)

  add_test (bash_batman_adv_test runners/batman_adv.sh)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
