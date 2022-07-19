#
#  This file is part of the Firelands Core Project. See AUTHORS file for Copyright information
# 
#  This program is free software; you can redistribute it and/or modify it
#  under the terms of the GNU General Public License as published by the
#  Free Software Foundation; either version 2 of the License, or (at your
#  option) any later version.
# 
#  This program is distributed in the hope that it will be useful, but WITHOUT
#  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
#  FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
#  more details.
# 
#  You should have received a copy of the GNU General Public License along
#  with this program. If not, see <http://www.gnu.org/licenses/>.
#

#Main Files
set(SRC_GRP_MAIN
  AFThread.cpp
  AFThread.h
  CliThread.cpp
  CliThread.h
  RAThread.cpp
  RAThread.h
  WorldThread.cpp
  WorldThread.h
  worldserver.cpp
)
source_group("Main" FILES ${SRC_GRP_MAIN})

set(SRC_GRP_SOAP
  SOAP/SoapThread.cpp
  SOAP/SoapThread.h
)
source_group("SOAP" FILES ${SRC_GRP_SOAP})

if(WIN32)
    set(PRODUCT_NAME "worldserver.exe")
    set(PRODUCT_ICON ${CMAKE_CURRENT_SOURCE_DIR}/worldserver.ico)
    set(PRODUCT_DESCRIPTION "${FIRELANDS_PKG} World Server")
    configure_file(${CMAKE_SOURCE_DIR}/cmake/win/VersionInfo.h.in ${CMAKE_CURRENT_BINARY_DIR}/VersionInfo.h)
endif()


add_executable(worldserver
    ${SRC_GRP_MAIN}
    $<$<BOOL:${SOAP}>:${SRC_GRP_SOAP}>
    $<$<BOOL:${WIN32}>:
        ${CMAKE_CURRENT_BINARY_DIR}/VersionInfo.h
        ${CMAKE_SOURCE_DIR}/cmake/win/VersionInfo.rc
    >
)

target_include_directories(worldserver
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(worldserver
    PUBLIC
        game
        $<$<BOOL:${SOAP}>:gsoap>
        Threads::Threads
        DL::DL
)

install(
    TARGETS worldserver
    DESTINATION ${BIN_DIR}
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/worldserver.conf.dist.in ${CMAKE_CURRENT_BINARY_DIR}/worldserver.conf.dist)
install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/worldserver.conf.dist
    DESTINATION ${CONF_INSTALL_DIR}
)

if(WIN32 AND MSVC)
    install(
        FILES $<TARGET_PDB_FILE:worldserver>
        DESTINATION ${BIN_DIR}
        OPTIONAL
        )
endif()
