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
if(WIN32)
    # Crash logs
    set(winDebugging
        ${CMAKE_SOURCE_DIR}/src/common/Debugging/WheatyExceptionReport.cpp
        ${CMAKE_SOURCE_DIR}/src/common/Debugging/WheatyExceptionReport.h)

    # Service
    set(winService
        ${CMAKE_SOURCE_DIR}/src/common/Platform/ServiceWin32.cpp
        ${CMAKE_SOURCE_DIR}/src/common/Platform/ServiceWin32.h)
endif()

if(CMAKE_COMPILER_IS_GNUCXX AND NOT MINGW)
    add_definitions(-fno-delete-null-pointer-checks)
endif()

add_subdirectory(genrev)
add_subdirectory(server)

if(TOOLS)
    add_subdirectory(tools)
endif(TOOLS)
