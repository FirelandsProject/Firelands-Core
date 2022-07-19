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

add_subdirectory(Extractor_projects)

# Used for install targets
set(TOOLS_DIR "tools")

#install documentation and generation scripts
install(
    FILES
       "${CMAKE_CURRENT_SOURCE_DIR}/Extractor_Binaries/ExtractResources.sh"
       "${CMAKE_CURRENT_SOURCE_DIR}/Extractor_Binaries/make_vmaps.bat"
      # "${CMAKE_CURRENT_SOURCE_DIR}/Extractor_Binaries/mmap_excluded.txt"
      # "${CMAKE_CURRENT_SOURCE_DIR}/Extractor_Binaries/MoveMapGen.sh"
      # "${CMAKE_CURRENT_SOURCE_DIR}/Extractor_Binaries/offmesh.txt"
        "${CMAKE_CURRENT_SOURCE_DIR}/Extractor_Binaries/README.txt"
    DESTINATION
        "${BIN_DIR}/${TOOLS_DIR}"
)

