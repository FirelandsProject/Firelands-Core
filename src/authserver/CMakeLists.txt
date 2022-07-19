#/*
 #* This file is part of the Firelands Core Project. See AUTHORS file for Copyright information
 #*
 #* This program is free software; you can redistribute it and/or modify it
 #* under the terms of the GNU General Public License as published by the
 #* Free Software Foundation; either version 2 of the License, or (at your
 #* option) any later version.
 #*
 #* This program is distributed in the hope that it will be useful, but WITHOUT
 #* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 #* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 #* more details.
 #*
 #* You should have received a copy of the GNU General Public License along
 #* with this program. If not, see <http://www.gnu.org/licenses/>.
 #*/

#Auth Files
file(GLOB_RECURSE SRC_GRP_AUTH Auth/*.cpp Auth/*.h)
source_group("Authentication" FILES ${SRC_GRP_AUTH})

#Patch Files
file(GLOB_RECURSE SRC_GRP_PATCH Patch/*.cpp Patch/*.h)
source_group("Patch" FILES ${SRC_GRP_PATCH})

#Realm Files
file(GLOB_RECURSE SRC_GRP_REALM Realm/*.cpp Realm/*.h)
source_group("Realmlist" FILES ${SRC_GRP_REALM})

#Socket Buffer Files
file(GLOB_RECURSE SRC_GRP_SOCKB SocketBuffer/*.cpp SocketBuffer/*.h)
source_group("Socket Buffer" FILES ${SRC_GRP_SOCKB})


configure_file(authserver.conf.dist.in ${CMAKE_CURRENT_BINARY_DIR}/authserver.conf.dist)

if(WIN32)
    set(PRODUCT_NAME "authserver.exe")
    set(PRODUCT_ICON ${CMAKE_CURRENT_SOURCE_DIR}/authserver.ico)
    set(PRODUCT_DESCRIPTION "Firelands Authentication Daemon")
    configure_file(${CMAKE_SOURCE_DIR}/cmake/win/VersionInfo.h.in ${CMAKE_CURRENT_BINARY_DIR}/VersionInfo.h)
endif()

add_executable(authserver
    ${SRC_GRP_AUTH}
    ${SRC_GRP_PATCH}
    ${SRC_GRP_REALM}
    ${SRC_GRP_SOCKB}
    Main.cpp
    $<$<BOOL:${WIN32}>:
        ${CMAKE_CURRENT_BINARY_DIR}/VersionInfo.h
        ${CMAKE_SOURCE_DIR}/cmake/win/VersionInfo.rc
    >
)

target_include_directories(authserver
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}/VersionInfo.h
)

target_link_libraries(authserver
    PUBLIC
        shared
        Threads::Threads
        DL::DL
)

install(
    TARGETS authserver
    DESTINATION ${BIN_DIR}
)

install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/authserver.conf.dist
    DESTINATION ${CONF_INSTALL_DIR}
)

if(WIN32 AND MSVC)
    install(
        FILES $<TARGET_PDB_FILE:authserver>
        DESTINATION ${BIN_DIR}
        OPTIONAL
        )
endif()
