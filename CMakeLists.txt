#
# This file is part of the Firelands Core Project. See AUTHORS file for Copyright information
#
# This program is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the
# Free Software Foundation; either version 2 of the License, or (at your
# option) any later version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
# more details.
#
# You should have received a copy of the GNU General Public License along
# with this program. If not, see <http://www.gnu.org/licenses/>.
#

cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

cmake_policy(SET CMP0028 NEW)
cmake_policy(SET CMP0054 NEW)
cmake_policy(SET CMP0042 NEW)
cmake_policy(SET CMP0048 NEW)
cmake_policy(SET CMP0063 NEW)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set_property(GLOBAL
  PROPERTY USE_FOLDERS ON
)

# ==================================================================================
# Define available cmake options below
option(BUILD_SERVER "Build the worldserver" ON)
option(BUILD_AUTH "Build the authserver" ON)
option(BUILD_TOOLS "Build the map/vmap/mmap extractors" ON)
option(USE_STORMLIB "Use StormLib for reading MPQs" ON)
option(SCRIPT_LIB_ELUNA "Compile with support for Eluna scripts" ON)
option(SCRIPT_LIB_SD3 "Compile with support for ScriptDev3 scripts" ON)
option(PLAYERBOTS "Enable Player Bots" OFF)
option(SOAP "Enable remote access via SOAP" OFF)
option(PCH "Enable precompiled headers" ON)
option(DEBUG "Enable debug build (only on non IDEs)" OFF)

# ==================================================================================
message("")
message(
  "This script builds the Firelands Cata server.
  Options that can be used in order to configure the process:
   General:
    CMAKE_INSTALL_PREFIX    Path where the server should be installed to
    BUILD_SERVER            Build the main server
    BUILD_AUTH              Build the authserver
    BUILD_TOOLS             Build the map/vmap/mmap extractors
    USE_STORMLIB            Use StormLib for reading MPQs
    SOAP                    Enable remote access via SOAP
    PCH                     Enable use of precompiled headers
    DEBUG                   Debug build, only for systems without IDE (Linux, *BSD)
   Scripting engines:
    SCRIPT_LIB_ELUNA        Compile with support for Eluna scripts
    SCRIPT_LIB_SD3          Compile with support for ScriptDev3 scripts
   Modules:
    PLAYERBOTS              Enable Player Bots

  To set an option simply type -D<OPTION>=<VALUE> after 'cmake <srcs>'.
  Also, you can specify the generator with -G. See 'cmake --help' for more details
  For example: cmake .. -DCMAKE_INSTALL_PREFIX=/opt/firelands -DSCRIPT_LIB_SD3=0"
)
message("")

# ==================================================================================#
project(firelands-cata VERSION 0.1.0)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install CACHE PATH "Default install directory" FORCE)
endif()

if(DEBUG)
  set(CMAKE_BUILD_TYPE Debug)
else()
  set(CMAKE_BUILD_TYPE Release)
endif()

if(UNIX)
  set(BIN_DIR ${CMAKE_INSTALL_PREFIX}/bin)

  if(NOT CONF_INSTALL_DIR)
    set(CONF_INSTALL_DIR ./etc)
  endif()
else()
  set(BIN_DIR ${CMAKE_INSTALL_PREFIX})

  if(NOT CONF_INSTALL_DIR)
    set(CONF_INSTALL_DIR .)
  endif()
endif()

if(NOT WITHOUT_GIT)
  find_package(Git)
endif()

find_package(Threads REQUIRED)
find_package(MySQL REQUIRED)
find_package(DL REQUIRED)

find_package(ZLIB QUIET)
find_package(BZip2 QUIET)

find_package(OpenSSL REQUIRED)

include(${CMAKE_SOURCE_DIR}/cmake/GenRevision.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/EnsureVersion.cmake)

include(${CMAKE_SOURCE_DIR}/cmake/FirelandsParams.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/SetDefinitions.cmake)

if(PCH)
  include(${CMAKE_SOURCE_DIR}/cmake/PCHSupport.cmake)
endif()

add_subdirectory(dep)
add_subdirectory(src)

include(${CMAKE_SOURCE_DIR}/cmake/StatusInfo.cmake)
