FUNCTION(GET_SUBDIR_LIST result curdir recursive includeRoot)
    # glob recurse seem's doesn't work
    FILE(GLOB children RELATIVE ${curdir} "${curdir}/[^\\.]*")

    if(${includeRoot})
        SET(dirlist "${curdir}")
    else()
        SET(dirlist "")
    endif()

    FOREACH(child ${children})
        IF(IS_DIRECTORY "${curdir}/${child}")
            if(${recursive})
                GET_SUBDIR_LIST(sub_Dirs "${curdir}/${child}" TRUE FALSE)
                SET(dirlist "${curdir}/${child}" ${sub_Dirs} ${dirlist})
            else()
                SET(dirlist "${curdir}/${child}" ${dirlist})
            endif()
        ENDIF()
    ENDFOREACH()

    SET(${result} ${dirlist} PARENT_SCOPE)
ENDFUNCTION(GET_SUBDIR_LIST result curdir recursive)
