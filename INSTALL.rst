===========================
 Installation instructions
===========================

Dependencies
------------

Build dependencies
******************

- mostly ISO99 compliant C compiler (e.g. GCC or clang)
- C-library compliant with parts of POSIX.1-2008 or SUSv4 (e.g. glibc > 2.10)
- cmake
- cunit

Runtime dependencies
********************

- Linux kernel
- C-library supporting POSIX.1-2008 or SUSv4
- batman_adv kernel module

Building
--------

e.g.::

    mkdir build && cd build
    cmake ..
    make
    make install

