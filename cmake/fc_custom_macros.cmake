#
# This file is part of the Firelands Core Project. See AUTHORS file for Copyright information
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#
MACRO(ADD_GLOBAL_VAR name val)
    GET_GLOBAL_VAR(${name})

    set_property(GLOBAL PROPERTY ${name}
        ${${name}}
        ${val}
    )

    # after set , create the variable for current scope
    GET_GLOBAL_VAR(${name})
ENDMACRO()

#
# GET_GLOBAL_VAR
#
MACRO(GET_GLOBAL_VAR name)
    get_property(${name} GLOBAL PROPERTY ${name})
ENDMACRO()

#
# ADD_LIST_TO_CACHE
#
MACRO(ADD_LIST_TO_CACHE name val)
    # avoid duplicates
    if(";${${name}};" MATCHES ";${val};")
    # nothing to do for now
    else()
        set(${name} ${val} ${${name}} CACHE INTERNAL "CU Var")
    endif()
ENDMACRO()

#
# FC_ADD_SCRIPT
#
MACRO(FC_ADD_SCRIPT path)
    ADD_GLOBAL_VAR("FC_SCRIPTS_SOURCES" "${path}")
ENDMACRO()

#
# FC_ADD_SCRIPTS
#
# This macro can be used to automatically load scripts for the ScriptMgr
# from a specified folder, instead of manually list them within the cmake
# NOTE: you must still manually specify the script loader header
#
MACRO(FC_ADD_SCRIPTS path)
    GET_SUBDIR_LIST(sub_DIRS ${path} TRUE TRUE)

    FOREACH(subdir ${sub_DIRS})
        file(GLOB sources "${subdir}/*.cpp" "${subdir}/*.h")
        ADD_LIST_TO_CACHE(scripts_STAT_SRCS "${sources}")
    ENDFOREACH()
ENDMACRO()

#
# FC_ADD_SCRIPT_LOADER
#
MACRO(FC_ADD_SCRIPT_LOADER script_dec include)
    set(lower_prio_scripts ${ARGN})
    list(LENGTH lower_prio_scripts num_lower_prio_scripts)

    if(${num_lower_prio_scripts} GREATER 0)
        GET_GLOBAL_VAR("FC_ADD_SCRIPTS_LIST")

        foreach(lower_prio_script ${lower_prio_scripts})
            if("${FC_ADD_SCRIPTS_LIST}" MATCHES "Add${lower_prio_script}Scripts()")
                message("-- ${script_dec} demands lower priority: ${lower_prio_script} --")
                list(REMOVE_ITEM FC_ADD_SCRIPTS_LIST "Add${lower_prio_script}Scripts()")
                CU_SET_GLOBAL("FC_ADD_SCRIPTS_LIST" "${FC_ADD_SCRIPTS_LIST}")
                list(APPEND removed_lower_prio_scripts ${lower_prio_script})
            endif()
        endforeach()

        ADD_GLOBAL_VAR("FC_ADD_SCRIPTS_LIST" "Add${script_dec}Scripts()\;")

        foreach(lower_prio_script ${removed_lower_prio_scripts})
            ADD_GLOBAL_VAR("FC_ADD_SCRIPTS_LIST" "Add${lower_prio_script}Scripts()\;")
        endforeach()
    else()
        ADD_GLOBAL_VAR("FC_ADD_SCRIPTS_LIST" "Add${script_dec}Scripts()\;")
    endif()

    if(NOT ${include} STREQUAL "")
        GET_GLOBAL_VAR("FC_ADD_SCRIPTS_INCLUDE")

        if(NOT ";${FC_ADD_SCRIPTS_INCLUDE};" MATCHES ";${include};")
            ADD_GLOBAL_VAR("FC_ADD_SCRIPTS_INCLUDE" "${include}\;")
        endif()
    endif()
ENDMACRO()

#
# FC_ADD_CONFIG_FILE
#
MACRO(FC_ADD_CONFIG_FILE configFilePath)
    message("> Warning: module using deprecated add config file api")
ENDMACRO()

#
# Use it like:
# CopyModuleConfig("fm-mytesting.conf.dist")
#
function(CopyModuleConfig configDir)
    set(postPath "configs/modules")

    if(WIN32)
        if("${CMAKE_MAKE_PROGRAM}" MATCHES "MSBuild")
            add_custom_command(TARGET modules
                POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/bin/$(ConfigurationName)/${postPath}")
            add_custom_command(TARGET modules
                POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy "${configDir}" "${CMAKE_BINARY_DIR}/bin/$(ConfigurationName)/${postPath}")
        elseif(MINGW)
            add_custom_command(TARGET modules
                POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/bin/${postPath}")
            add_custom_command(TARGET modules
                POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy "${configDir} ${CMAKE_BINARY_DIR}/bin/${postPath}")
        endif()
    endif()

    if(UNIX)
        install(FILES "${configDir}" DESTINATION "${CONF_DIR}/modules")
    elseif(WIN32)
        install(FILES "${configDir}" DESTINATION "${CMAKE_INSTALL_PREFIX}/${postPath}")
    endif()

    unset(postPath)
endfunction()
