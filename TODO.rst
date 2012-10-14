=======
 Tasks
=======

Before first release
--------------------

* Doxygen documentation
    - Missing config file
* Missing INSTALL file
* Missing ChangeLog
* Check that all allocated memory and opened file descriptors are closed in
  functions before returning.

Before a stable release
-----------------------

* BUG: make fails if building under build/ due to a relative path error
* Support for multiple versions
* Testing of multiple versions
* Implementation notes for all functions
* BUG: batman_adv.sh test fails if bat0 exists before the test is run
