# Copyright (C) 2022 Firelands Project <https://github.com/FirelandsProject>
# Copyright (C) 2008-2013 TrinityCore <http://www.trinitycore.org/>
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

# CMake policies (can not be handled elsewhere)
cmake_minimum_required(VERSION 3.16...3.22)

message(STATUS "CMake version: ${CMAKE_VERSION}")

cmake_policy(SET CMP0005 NEW)

# Set projectname (must be done AFTER setting configurationtypes)
project(FirelandsCata VERSION 0.1.0 LANGUAGES CXX C)

# add this options before PROJECT keyword
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

# Set RPATH-handing (CMake parameters)
set(CMAKE_SKIP_BUILD_RPATH 0)
set(CMAKE_BUILD_WITH_INSTALL_RPATH 0)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH 1)

# set macro-directory
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/src/cmake/macros")

include(CheckCXXSourceRuns)
include(CheckIncludeFiles)
include(ConfigureScripts)
include(ConfigureModules)
include(ConfigureApplications)
include(ConfigureTools)

# build in RelWithDebInfo by default if not explicitly set
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()

# set default buildoptions and print them
include(cmake/options.cmake)

# turn off PCH totally if enabled (hidden setting, mainly for devs)
if(NOPCH)
    set(USE_COREPCH 0)
    set(USE_SCRIPTPCH 0)
endif()

include(ConfigureBaseTargets)
include(CheckPlatform)
include(GroupSources)
include(AutoCollect)
include(ConfigInstall)

# basic packagesearching and setup (further support will be needed, this is a preliminary release!)
find_package(PCHSupport)
find_package(MySQL REQUIRED)

if(NOT WITHOUT_GIT)
    find_package(Git)
endif()

# Find revision ID and hash of the sourcetree
include(cmake/genrev.cmake)

include(src/cmake/fc_macros.cmake)

# print out the results before continuing
include(cmake/showoptions.cmake)

# add dependencies
add_subdirectory(deps)
add_subdirectory(src/common)

# add core sources
add_subdirectory(src)

if(WORLDSERVER)
    # add modules sources
    add_subdirectory(modules)
endif()

#TODO: Implement Unit testing Includes
