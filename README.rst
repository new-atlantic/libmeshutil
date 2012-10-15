=============
 libmeshutil
=============

**Warning:**
    This README is based on wishful thinking.

Overview
--------

*libmeshutil* is a C library that provides an API for querying information from
different mesh routing protocols. More specifically, it provides information
about the state of the routing protocol, the network interfaces it uses as well
as information about the network accessed via the protocol. Information about
the network includes measures such as the number of nodes in the network and
addresses of direct and potential next hops. The exact information accessible
via the APIs depends on the routing protocol (and version) in question.

For the programming of applications that are not tied to a specific routing
protocol it is preferrable to use *libmeshaware* which provides a protocol
independent abstraction layer over *libmeshutil* as well as bindings for
various programming languages.

Supported routing protocols
---------------------------

At the moment *libmeshutil* only supports *B.A.T.M.A.N. advanced*, which
requires the Linux kernel. Support for other protocols such as *BABEL* and
*olsr* is planned.

B.A.T.M.A.N. advanced
*********************

At the moment all the functions for *B.A.T.M.A.N. advanced* have only been
tested with version 2011.4.0 and compatiblity with other versions is not
guaranteed. Specifically versions preceding 2010.0.0 should not work.
Compatibility and testing with multiple versions is the aim, but there is
no intention to support versions older than 2010.0.0.

Some functionality of batman_adv requires the Linux debug filesystem to be
mounted. This requires root (or CAP_SYS_ADMIN?) privileges.

Testing
-------

The bash based batman_adv tests require root privileges to run and will fail
if run without. The bash based test suite is also quite brittle with false
negatives due to race conditions etc.

License
-------

*libmeshutil* is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free
Software Foundation; either version 3 of the License, or (at your option) any
later version.

You can find a copy of the license in the COPYING file or at
http://www.gnu.org/licenses/lgpl-3.0.txt.

