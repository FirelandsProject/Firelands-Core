# Copyright (C) 2022 Firelands Project <https://github.com/FirelandsProject>
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

CollectSourceFiles(
    ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE_SOURCES

    # Exclude
    ${CMAKE_CURRENT_SOURCE_DIR}/PrecompiledHeaders)

if(USE_COREPCH)
    set(PRIVATE_PCH_HEADER PrecompiledHeaders/databasePCH.h)
endif()

add_library(database
    ${PRIVATE_SOURCES})

CollectIncludeDirectories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC_INCLUDES
    # Exclude
    ${CMAKE_CURRENT_SOURCE_DIR}/PrecompiledHeaders)

target_include_directories(database
    PUBLIC
    ${PUBLIC_INCLUDES}
    PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(database
    PRIVATE
    fc-core-interface
    mysql
    PUBLIC
    common)

set_target_properties(database
    PROPERTIES
    FOLDER
    "server")

# Generate precompiled header
if(USE_COREPCH)
    add_cxx_pch(database ${PRIVATE_PCH_HEADER})
endif()
