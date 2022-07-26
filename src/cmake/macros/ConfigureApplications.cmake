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

set(AUTHSERVER 0)
set(WORLDSERVER 0)

# Returns the base path to the apps directory in the source directory
function(GetApplicationsBasePath variable)
    set(${variable} "${CMAKE_SOURCE_DIR}/src/server/apps" PARENT_SCOPE)
endfunction()

# Stores the absolut path of the given app in the variable
function(GetPathToApplication app variable)
    GetApplicationsBasePath(APPS_BASE_PATH)
    set(${variable} "${APPS_BASE_PATH}/${app}" PARENT_SCOPE)
endfunction()

# Stores the project name of the given app in the variable
function(GetProjectNameOfApplicationName app variable)
    string(TOLOWER "${app}" GENERATED_NAME)
    set(${variable} "${GENERATED_NAME}" PARENT_SCOPE)
endfunction()

# Creates a list of all applications and stores it in the given variable.
function(GetApplicationsList variable)
    GetApplicationsBasePath(BASE_PATH)
    file(GLOB LOCALE_SOURCE_APP_LIST RELATIVE
        ${BASE_PATH}
        ${BASE_PATH}/*)

    set(${variable})

    foreach(SOURCE_APP ${LOCALE_SOURCE_APP_LIST})
        GetPathToApplication(${SOURCE_APP} SOURCE_APP_PATH)

        if(IS_DIRECTORY ${SOURCE_APP_PATH})
            list(APPEND ${variable} ${SOURCE_APP})
        endif()
    endforeach()

    set(${variable} ${${variable}} PARENT_SCOPE)
endfunction()

# Converts the given application name into it's
# variable name which holds the build type.
function(ApplicationNameToVariable application variable)
    string(TOUPPER ${application} ${variable})
    set(${variable} "APP_${${variable}}")
    set(${variable} ${${variable}} PARENT_SCOPE)
endfunction()

function(CheckApplicationsBuildList)
    GetApplicationsList(APPLICATIONS_BUILD_LIST)

    foreach(APPLICATION_BUILD_NAME ${APPLICATIONS_BUILD_LIST})
        ApplicationNameToVariable(${APPLICATION_BUILD_NAME} APPLICATION_BUILD_VARIABLE)

        if(${APPLICATION_BUILD_VARIABLE} STREQUAL "default")
            set(${APPLICATION_BUILD_VARIABLE} ${APPS_DEFAULT_BUILD})
        endif()

        # Build the Graph values
        if(${APPLICATION_BUILD_VARIABLE} MATCHES "enabled")
            if(${APPLICATION_BUILD_NAME} MATCHES "authserver")
                set(AUTHSERVER 1 PARENT_SCOPE)
            elseif(${APPLICATION_BUILD_NAME} MATCHES "worldserver")
                set(WORLDSERVER 1 PARENT_SCOPE)
            endif()
        endif()
    endforeach()
endfunction()
