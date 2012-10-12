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

License
-------

*libmeshutil* is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free
Software Foundation; either version 3 of the License, or (at your option) any
later version.

You can find a copy of the license in the COPYING file or at
http://www.gnu.org/licenses/lgpl-3.0.txt.

