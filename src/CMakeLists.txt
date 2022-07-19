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

# Build the firelands shared library
add_subdirectory(shared)

add_subdirectory(genrev)

# Needs to link against world.lib
if(BUILD_SERVER OR BUILD_TOOLS)
    # Build the firelands game library
    add_subdirectory(game)
endif()

if(BUILD_SERVER)
    # Build the firelands worldserver
    add_subdirectory(worldserver)

    # Build the selected modules
    add_subdirectory(modules)
endif()

# Build the firelands realm authentication server
if(BUILD_AUTH)
    add_subdirectory(authserver)
endif()

# If we want the tools for map/vmap/mmap extraction
if(BUILD_TOOLS)
    add_subdirectory(tools)
endif()

if (BUILD_SERVER OR BUILD_AUTH)
    if(WIN32)
        get_filename_component(MYSQL_LIB_DIR ${MySQL_LIBRARIES} DIRECTORY)
        get_filename_component(MYSQL_LIB_DLL ${MySQL_LIBRARIES} NAME)
        STRING(REPLACE ".lib" ".dll" MYSQL_LIB_DLL ${MYSQL_LIB_DLL})
        install(FILES ${MYSQL_LIB_DIR}/${MYSQL_LIB_DLL} DESTINATION ${BIN_DIR})
    endif()
endif()
