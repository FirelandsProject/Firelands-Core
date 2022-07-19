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


# Since the different tools from mpqlib and StormLib are fairly different, so is the loadlib.
# Therefore, I have temporarily split the lib into two versions depending on which toolset is compiled.
# These can definitely be merged at some point, however we need to document and understand the differences to do so.


add_library(loadlib STATIC
  sl/loadlib.cpp
  sl/adt.cpp
  sl/wdt.cpp
  sl/mpq.cpp)

target_include_directories(loadlib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(loadlib PUBLIC stormlib)