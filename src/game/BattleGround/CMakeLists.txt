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

include(MacroFirelandsSourceGroup)

#-----------------------------------------------------------------------------
# Define the battleground library
file(GLOB sources *.cpp)
file(GLOB headers *.h)

set(battleground_LIB_SRCS ${sources} ${headers})

firelands_source_group(${battleground_LIB_SRCS})

include_directories(
    ${ACE_INCLUDE_DIRS}
    ${MYSQL_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/dep/include/g3dlite/
    ${CMAKE_SOURCE_DIR}/src/framework/
    ${CMAKE_SOURCE_DIR}/src/shared/
    ${CMAKE_SOURCE_DIR}/src/game/
    ${CMAKE_SOURCE_DIR}/src/game/Time/
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_BINARY_DIR}
    ${CMAKE_BINARY_DIR}/src/shared/
)

#-----------------------------------------------------------------------------
# Build the battleground library
add_library(firelands-battleground STATIC ${battleground_LIB_SRCS})
target_link_libraries(firelands-battleground firelands-shared firelands-framework g3dlite ${ACE_LIBRARIES} ${MYSQL_LIBRARIES})

if(${CMAKE_BUILD_TYPE} MATCHES Debug)
    set(BUILD_PROPERTIES "-DFIRELANDS_DEBUG")
    set_target_properties(firelands-battleground PROPERTIES COMPILE_FLAGS ${BUILD_PROPERTIES})
endif()