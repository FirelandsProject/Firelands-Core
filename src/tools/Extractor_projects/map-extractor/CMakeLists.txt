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

project(FIRELANDS_MAP_EXTRACTOR)

set(EXECUTABLE_NAME "map-extractor")

if(WIN32)
    # add resource file to windows build
    set(EXECUTABLE_SRCS ${EXECUTABLE_SRCS} map-extractor.rc)
endif()

include_directories(
    ${CMAKE_SOURCE_DIR}/dep/StormLib/src
    ${CMAKE_SOURCE_DIR}/dep/loadlib
    ${CMAKE_SOURCE_DIR}/src/tools/Extractor_projects/shared
)

set(SOURCES
    ${CMAKE_SOURCE_DIR}/src/tools/Extractor_projects/shared/ExtractorCommon.cpp
)

add_executable(${EXECUTABLE_NAME} dbcfile.cpp System.cpp ${SOURCES} ${EXECUTABLE_SRCS})

target_link_libraries(${EXECUTABLE_NAME} loadlib storm)

install(TARGETS ${EXECUTABLE_NAME} DESTINATION "${BIN_DIR}/${TOOLS_DIR}")

if(WIN32 AND MSVC)
    install(FILES $<TARGET_PDB_FILE:${EXECUTABLE_NAME}> DESTINATION ${BIN_DIR}/${TOOLS_DIR} OPTIONAL)
endif()