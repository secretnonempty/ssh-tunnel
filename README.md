ssh-tunnel
==========

[![Build Status](https://travis-ci.org/ajbennieston/ssh-tunnel.svg?branch=master)](https://travis-ci.org/ajbennieston/ssh-tunnel)

Copyright (c) 2012 - 2017 Andrew J. Bennieston

https://github.com/ajbennieston/ssh-tunnel

Released under the BSD license. Details below.

"On-demand SSH tunnelling with a client-server architecture."

Overview
--------
ssh-tunneld is a daemon process which listens on tcp/1081 (by default)
for connections from ssh-tunnelc (the client). When a client requests
an SSH tunnel, the ssh-tunneld process will open an SSH connection to
the host you configured, using the "-D" option for SOCKS5 dynamic
forwarding. It then tells the client to go ahead and use the connection.

The client is designed to be used as the "ProxyCommand" for an SSH session
(see "man ssh_config" for details). It requests a tunnel from ssh-tunneld,
then establishes its own connection using the netcat ("nc") program to
connect through the newly created SOCKS5 proxy. When the SSH session ends,
ssh-tunnelc tells the ssh-tunneld that it is done.

ssh-tunneld keeps a count of the connected clients. When this count is > 0,
the same "ssh -D" tunnel is used for all clients. If the count drops to 0,
the tunnel is closed (SIGTERM is sent to the ssh process), and a
subsequent client connect would result in a new tunnel being established.

Requirements
------------
 1. public key (or other passwordless) access to the host being used as the
    remote end of the dynamic forwarding. This can be done with passwordless
    keys (not recommended) or keys which are available in an ssh-agent that
    is in the environment of ssh-tunneld.

 2. netcat-openbsd (the OpenBSD version of netcat, rather than
    netcat-traditional). This is required for the "-x" and "-X"
    (SOCKS proxy) options to the "nc" program to work.

 3. (Dynamic) SSH forwarding enabled on the proxy host (remote host of the
    "ssh -D" command). Note this also requires SSH Protocol version 2.

Usage
-----
 1. Type 'make' to build the client (ssh-tunnelc) and server (ssh-tunneld) in
    their respective directories. This works with either BSD Make or GNU Make.
 
 2. Copy the binaries ssh-tunnelc/ssh-tunnelc and ssh-tunneld/ssh-tunneld to
    somewhere in your $PATH.

 3. Ensure that the ssh-agent has an unlocked copy of the private key for the
    host you want to tunnel through (or use passwordless keys; not recommended).

 4. Start the ssh-tunneld. Typing ssh-tunneld will show usage information.
    At the minimum, you need to provide the hostname to connect to when
    setting up dynamic port forwarding:
    ssh-tunneld somehost.somedomain.tld
 
 5. Add an entry in $HOME/.ssh/config for the host you want to connect to
    through the tunnel. Include the following command:
    ProxyCommand ssh-tunnelc %h %p
    (You can specify additional options; typing ssh-tunnelc will print usage).

 6. Use ssh to connect to the host you created the above entry for. ssh will
    launch ssh-tunnelc, which will communicate with ssh-tunneld. ssh-tunneld
    will create a tunnel, which ssh-tunnelc will use (with the help of nc)
    for the ssh connection you're trying to start. When you're done, logout
    as normal, and ssh-tunnelc will tell ssh-tunneld to tear down the tunnel
    (unless something else is still using it).

Known Issues
------------

1. Active connection count is incorrect if the client terminates due to 
   receiving SIGKILL. There isn't a lot that can be done about this...
   One option might be some kind of keep-alive approach, but I have no
   plans to implement this any time soon.

Supported Platforms
-------------------
ssh-tunnel should work on most POSIX compliant platforms. The list below
indicates platforms on which ssh-tunnel has been tested and is known to work.
If you have tested ssh-tunnel on a platform that is not listed, please submit
a pull request adding that platform to the list.

* Debian 6.x (Squeeze) [x86_64]
* Debian 7.x (Wheezy) [x86_64]
* FreeBSD 9.x [x86_64]
* FreeBSD 10.x [x86_64]
* Mac OS X 10.7 (Lion) [x86_64]
* Mac OS X 10.8 (Mountain Lion) [x86_64]
* Mac OS X 10.9 (Mavericks) [x86_64]
* OpenSuSE 12.x [x86_64]
* Raspbian Wheezy [armv6]

Changelog
---------
2017-05-05 Andrew J. Bennieston
* Fix incorrect call to `memset` in ssh-tunneld.c.

2017-02-02 Andrew J. Bennieston
 * Add continuous integration via Travis-CI.
 * Build using -std=c11 instead of -std=c99.
    
2012-11-28 Andrew J. Bennieston
 * ssh-tunnel compiles on a Raspberry Pi running "Raspbian Wheezy"
   using gcc 4.6 (without any changes!)

2012-11-09 Andrew J. Bennieston
 * Move signal handler registration out of main() in ssh-tunnelc
 * Move "host:port" string building code out of main() in ssh-tunnelc

2012-11-07 Andrew J. Bennieston
 * Move code to fork, set session ID, set umask and close file descriptors
   out of ssh-tunneld main() and into daemonize(); improves potential for
   code reuse as well as tidying up main() somewhat!
 * Use a struct to hold program arguments in ssh-tunnelc (tidies up the
   code a bit)

2012-11-06 Andrew J. Bennieston
 * Use a struct to hold program options in ssh-tunneld (tidies up the
   code a bit)

2012-08-24 Andrew J. Bennieston
 * Ensure that the result of any malloc() or strdup() call is checked
   against NULL
 * Ensure that any strncpy() call results in a correctly null-terminated
   string
 * Change umask of child process to 077 in ssh-tunneld

2012-07-30 Andrew J. Bennieston
 * Use (char *) NULL instead of plain NULL to terminate execlp(...);

2012-06-10 Andrew J. Bennieston
 * Better initialisation of variables in ssh-tunneld.c
 * Rearranged logic on if() .. elseif() .. else when
   forking child process

2012-06-09 Andrew J. Bennieston
 * Improved handling of client quit due to SIGTERM or SIGINT

2012-06-08 Andrew J. Bennieston
 * Updated to compile without warnings on Mac OS X Lion using either
   clang or llvm-gcc
 * Added BSD-style Makefiles to support BSD Make as well as GNU Make
 * Added comments to top of every header / source file to identify
   it as part of ssh-tunnel
 * Reduced the number of times strdup() is used when processing
   command-line options

2012-03-21 Andrew J. Bennieston
 * Split ssh-tunnelc.c across multiple files to improve ease of
   navigation
 * Modified ssh-tunnelc/Makefile to build any .c files in that
   directory then link them together into the ssh-tunnelc binary

2012-03-20 Andrew J. Bennieston
 * Improved security of control socket by binding to localhost only
   (by default)
 * Provided command-line option (-r) to allow binding to * (all
   interfaces) for control socket
 * Changed tabs to spaces in ssh-tunnelc.c and ssh-tunneld.c
 
---------
License:
--------
Copyright (c) 2012, Andrew J. Bennieston
All rights reserved.

Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions
are met:

 * Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in
   the documentation and/or other materials provided with the
   distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
