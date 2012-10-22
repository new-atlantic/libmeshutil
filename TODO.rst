=======
 Tasks
=======

Before a stable release
-----------------------

* Don't force mu_bat_mesh_node to use 17 char MAC address representation:
    - Wrapper functions to reads and writes:
        + mu_mac_addr_to_str
        + mu_str_to_mac_addr
* Use versioned functions and a symbol map
    - How portable is this?
* Check return values of fclose()?
* Support for multiple versions
* Testing of multiple versions
* Implementation notes for all functions
* BUG: batman_adv.sh test sometimes fails if bat0 exists before the test is run
* Embed version string in meshutil.h
* Make test via CMake should warn about missing privileges for tests
* Unit tests covering all functions
    - more cunit tests?
* Analyze with valgrind
