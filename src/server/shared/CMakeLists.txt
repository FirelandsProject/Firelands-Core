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
    PRIVATE_SOURCES)

# Group sources
GroupSources(${CMAKE_CURRENT_SOURCE_DIR})

add_library(shared
    ${PRIVATE_SOURCES})

CollectIncludeDirectories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC_INCLUDES)

target_include_directories(shared
    PUBLIC
    ${PUBLIC_INCLUDES}
    PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(shared
    PRIVATE
    fc-core-interface
    PUBLIC
    database)

set_target_properties(shared
    PROPERTIES
    FOLDER
    "server")
