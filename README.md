# Firelands Cata

**Firelands-Cata** is an open source project based on __MaNGOS__ written mostly in C++. It's fast, runs on multiple
platforms and stores game data in [MySQL][40] or [MariaDB][41]. It also has
optional support for SOAP.

It aims to be 100% compatible with the final version of CATA [World of Warcraft][2],
namely [patch 4.3.4][4].

Requirements
------------
The server supports a wide range of operating systems, and various compiler platforms.
In order to do that, we use various free cross-platform libraries and use [CMake][19] (v3.12+) to provide
a cross-platform build system which adapts to your chosen operating system and compiler.

Operating systems
-----------------
Currently we support running the server on the following operating systems:

* **[Windows][20]**, 32 bit and 64 bit. Windows Server 2008 (or newer) or Windows 8 (or newer) is recommended.
* **Linux**, 32 bit and 64 bit. [Debian 7][21] and [Ubuntu 12.04 LTS][22] are
  recommended. Other distributions with similar package versions will work, too.
* **BSD**, 32 bit and 64 bit. [FreeBSD][23], [NetBSD][24], [OpenBSD][25] are recommended.
* **MAC OSX**, 64 bit. MacOS BigSur is recommended.

Of course, newer versions should work, too. In the case of Windows, matching
server versions will work, too.

Compilers
---------
Building the server is currently possible with these compilers:

* **[Microsoft Visual Studio][31] 32 bit and 64 bit.** All editions of Visual Studio
from 2015 upwards are officially supported (although support for 2015 and 2017 are end of life.)

* **[Clang][33]**, 32 bit and 64 bit. The Clang compiler can be used on any
  supported operating system.

Dependencies
------------
The server stands on the shoulders of several well-known Open Source libraries plus
a few awesome, but less known libraries to prevent us from inventing the wheel again.

**Please note that Linux and Mac OS X users should install packages using
their systems package management instead of source packages.**

* **[Git][34] / [Github for Windows][35]**: This version control software allows you to get the source files in the first place.
* **[MySQL][40]** / **[MariaDB][41]**: These databases are used to store content and user data.
* **[ACE][43]**: aka Adaptive Communication Environment, provides us with a solid cross-platform framework for abstracting operating system specific details.
* **[Recast][44]**: In order to create navigation data from the client's map files, Recast is used to do the dirty work. It provides functions for rendering, pathing, etc.
* **[G3D][45]**: This engine provides the basic framework for handling 3D data and is used to handle basic map data.
* **[Stormlib][46]**: Provides an abstraction layer for reading from the client's data files.
* **[Zlib][53]/[Zlib for Windows][51]** provides compression algorithms used in both MPQ archive handling and the client/server protocol.
* **[Bzip2][54]/[Bzip2 for Windows][52]** provides compression algorithms used in MPQ archives.
* **[OpenSSL][48]/[OpenSSL for Windows][55]** provides encryption algorithms used when authenticating clients.

**ACE**, **Recast**, **G3D**, **Stormlib**, **Zlib** and **Bzip2** are included in the standard distribution as
we rely on specific versions.

Optional dependencies
---------------------

* **[Doxygen][49]**: If you want to export HTML or PDF formatted documentation for the Mangos API, you should install this.


<br>We have a small, but extremely friendly and helpful community managed by MadMax and Antz.


Our discord/forum motto is:
```js
'Be nice or Be somewhere else'
```
Any trolling or unpleasantness is swiftly dealt with !!

**Official Website**
----

We welcome anyone who is interested in enjoying older versions of wow or contributing and helping out !

* [**Official Firelands Project Website**][3]

**Discord Server**
----

We also have a Discord server where many of us hang out and discuss Mangos related stuff.

* [**Discord Server**][9]

**Main Wiki**
----

The repository of as much information as we can pack in. Details regarding the Database, file type definitions, packet definitons etc.

* [**Wiki Table of Contents**][15]


**Bug / Issue Tracker**
----

Found an issue or something which doesn't seem right, please log it in the relevant section of the Bug Tracker.

* [**Bug Tracker**][16]

**Installation Guides**
----

Installation instructions for various operation systems can be found here.

* [**Installation Guides**][17]

Acknowledgements
--------
World of Warcraft, and all related art, images, and lore are copyright [Blizzard Entertainment, Inc.][1]


[1]: http://blizzard.com/ "Blizzard Entertainment Inc. - We love you!"
[2]: https://worldofwarcraft.com/ "World of Warcraft"
[3]: https://github.com/FirelandsProject "Main Firelands Project Website"
[4]: http://www.wowpedia.org/Patch_4.3.4 "Cataclysm - Patch 4.3.4 release notes"
[7]: http://www.cppreference.com/ "C / C++ reference"
[9]: https://discord.gg/fPxMjHS8xs "Our community hub on Discord"
[15]: https://github.com/FirelandsProject/wiki "Firelands Core Wiki"
[16]:https://github.com/FirelandsProject/Firelands-Core/issues "Firelands Core Cata Issue tracker"
[17]: https://github.com/FirelandsProject/wiki "Installation Guides"
[19]: http://www.cmake.org/ "CMake - Cross Platform Make"
[20]: http://windows.microsoft.com/ "Microsoft Windows"
[21]: http://www.debian.org/ "Debian - The Universal Operating System"
[22]: http://www.ubuntu.com/ "Ubuntu - The world's most popular free OS"
[23]: http://www.freebsd.org/ "FreeBSD - The Power To Serve"
[24]: http://www.netbsd.org/ "NetBSD - The NetBSD Project"
[25]: http://www.openbsd.org/ "OpenBSD - Free, functional and secure"
[31]: https://visualstudio.microsoft.com/vs/older-downloads/ "Visual Studio Downloads"
[33]: http://clang.llvm.org/ "clang - a C language family frontend for LLVM"
[34]: http://git-scm.com/ "Git - Distributed version control system"
[35]: http://windows.github.com/ "github - windows client"
[40]: https://dev.mysql.com/downloads/ "MySQL - The world's most popular open source database"
[41]: https://mariadb.org/download/ "MariaDB - An enhanced, drop-in replacement for MySQL"
[43]: http://www.dre.vanderbilt.edu/~schmidt/ACE.html "ACE - The ADAPTIVE Communication Environment"
[44]: http://github.com/memononen/recastnavigation "Recast - Navigation-mesh Toolset for Games"
[45]: http://sourceforge.net/projects/g3d/ "G3D - G3D Innovation Engine"
[46]: http://zezula.net/en/mpq/stormlib.html "Stormlib - A library for reading data from MPQ archives"
[48]: http://www.openssl.org/ "OpenSSL - The Open Source toolkit for SSL/TLS"
[49]: https://www.doxygen.nl/download.html "Doxygen - API documentation generator"
[51]: http://gnuwin32.sourceforge.net/packages/zlib.htm "Zlib for Windows"
[52]: http://gnuwin32.sourceforge.net/packages/bzip2.htm "Bzip2 for Windows"
[53]: http://www.zlib.net/ "Zlib"
[54]: http://www.bzip.org/ "Bzip2"
[55]: http://slproweb.com/products/Win32OpenSSL.html "OpenSSL for Windows"