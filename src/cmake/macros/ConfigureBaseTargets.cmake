#
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

# An interface library to make the target com available to other targets
add_library(fc-compile-option-interface INTERFACE)

# Use -std=c++11 instead of -std=gnu++11
set(CXX_EXTENSIONS OFF)

# Enable C++20 support
set(CMAKE_CXX_STANDARD 20)
message(STATUS "Enabled С++20 standard")

# An interface library to make the warnings level available to other targets
# This interface taget is set-up through the platform specific script
add_library(fc-warning-interface INTERFACE)

# An interface used for all other interfaces
add_library(fc-default-interface INTERFACE)

target_link_libraries(fc-default-interface
  INTERFACE
    fc-compile-option-interface)

# An interface used for silencing all warnings
add_library(fc-no-warning-interface INTERFACE)

if (MSVC)
  target_compile_options(fc-no-warning-interface
    INTERFACE
      /W0)
else()
  target_compile_options(fc-no-warning-interface
    INTERFACE
      -w)
endif()

# An interface library to change the default behaviour
# to hide symbols automatically.
add_library(fc-hidden-symbols-interface INTERFACE)

# An interface amalgamation which provides the flags and definitions
# used by the dependency targets.
add_library(fc-dependency-interface INTERFACE)
target_link_libraries(fc-dependency-interface
  INTERFACE
    fc-default-interface
    fc-no-warning-interface
    fc-hidden-symbols-interface)

# An interface amalgamation which provides the flags and definitions
# used by the core targets.
add_library(fc-core-interface INTERFACE)
target_link_libraries(fc-core-interface
  INTERFACE
    fc-default-interface
    fc-warning-interface)
