# This file is part of the FirelandsCore Project. See AUTHORS file for Copyright information
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

cmake_minimum_required(VERSION 3.16)

# add this options before PROJECT keyword
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

# CMake policies (can not be handled elsewhere)
cmake_policy(SET CMP0005 NEW)

# Set projectname (must be done AFTER setting configurationtypes)
project(FirelandsCore VERSION 0.1.0 LANGUAGES CXX C)

# Set RPATH-handing (CMake parameters)
set(CMAKE_SKIP_BUILD_RPATH 0)
set(CMAKE_BUILD_WITH_INSTALL_RPATH 0)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH 1)

# set macro-directory
list(APPEND CMAKE_MODULE_PATH
    "${CMAKE_SOURCE_DIR}/cmake/macros")

include(CheckCXXSourceRuns)
include(CheckIncludeFiles)
include(ConfigureScripts)

# include macro for modules
include(ConfigureModules)
include(ConfigureBaseTargets)
include(CheckPlatform)
include(GroupSources)
include(AutoCollect)

# build in Release-mode by default if not explicitly set
if(CMAKE_GENERATOR STREQUAL "Ninja Multi-Config")
    set(CMAKE_DEFAULT_BUILD_TYPE "RelWithDebInfo" CACHE INTERNAL "")
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()

# set default buildoptions and print them
include(cmake/options.cmake)
include(cmake/utils.cmake)
include(cmake/fc_custom_macros.cmake)

#
# Loading dyn modules
#

# add modules and dependencies
GET_SUBDIR_LIST(sub_DIRS "${CMAKE_SOURCE_DIR}/modules" FALSE FALSE)

FOREACH(subdir ${sub_DIRS})
    get_filename_component(MODULENAME ${subdir} NAME)

    if(";${DISABLED_FC_MODULES};" MATCHES ";${MODULENAME};")
        continue()
    endif()

    STRING(REGEX REPLACE "^${CMAKE_SOURCE_DIR}/" "" subdir_rel ${subdir})

    if(EXISTS "${subdir}/CMakeLists.txt")
        add_subdirectory("${subdir_rel}")
    endif()
ENDFOREACH()

# turn off PCH totally if enabled (hidden setting, mainly for devs)
if(NOPCH)
    set(USE_COREPCH 0)
    set(USE_SCRIPTPCH 0)
endif()

find_package(PCHSupport)
find_package(MySQL)

if(NOT WITHOUT_GIT)
    find_package(Git 1.7)
endif()

# Find revision ID and hash of the sourcetree
include(cmake/genrev.cmake)

# print out the results before continuing
include(cmake/showoptions.cmake)

# add dependencies
add_subdirectory(dep)

# add core sources
add_subdirectory(src)

if(SERVERS)
    add_subdirectory(modules)
endif(SERVERS)

include(CTest)

if(BUILD_TESTING)
    list(APPEND CMAKE_MODULE_PATH
        "${Catch2_SOURCE_DIR}/contrib")
    include(Catch)

    add_subdirectory(tests)
endif()
