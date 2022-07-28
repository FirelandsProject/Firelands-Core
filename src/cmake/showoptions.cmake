# output generic information about the core and buildtype chosen
message("")
message("* cataclysm hash : ${rev_info}")
message("* cataclysm date : ${rev_date}")

if(UNIX)
    message("* cataclysm buildtype : ${CMAKE_BUILD_TYPE}")
endif()

message("")

# output information about installation-directories and locations
message("* Install core to : ${CMAKE_INSTALL_PREFIX}")

if(UNIX)
    message("* Install libraries to : ${LIBSDIR}")
    message("* Install configs to : ${CONF_DIR}")
endif()

message("")

# Show infomation about the options selected during configuration
if(AUTHSERVER)
    message("* Build authserver       : Yes (default)")
else()
    message("* Build authserver       : No")
endif()

if(WORLDSERVER)
    message("* Build worldserver      : Yes (default)")
else()
    message("* Build worldserver      : No")
    set(SCRIPTS 0)
endif()

if(SCRIPTS)
    message("* Build with scripts     : Yes (default)")
    add_definitions(-DSCRIPTS)
else()
    message("* Build with scripts     : No")
    set(USE_SCRIPTPCH 0)
endif()

if (MODULES  AND (NOT MODULES STREQUAL "none"))
  message("* Build with modules              : Yes (${MODULES})")
  add_definitions(-DMODULES)
else()
  message("* Build with modules              : No")
endif()

if(TOOLS)
    message("* Build map/vmap tools   : Yes")
    add_definitions(-DNO_CORE_FUNCS)
else()
    message("* Build map/vmap tools   : No  (default)")
endif()

if(USE_COREPCH)
    message("* Build core w/PCH       : Yes (default)")
else()
    message("* Build core w/PCH       : No")
endif()

if(USE_SCRIPTPCH)
    message("* Build scripts w/PCH    : Yes (default)")
else()
    message("* Build scripts w/PCH    : No")
endif()

if(WITH_WARNINGS)
    message("* Show all warnings      : Yes")
else()
    message("* Show compile-warnings  : No  (default)")
endif()

if(WITH_COREDEBUG)
    message("* Use coreside debug     : Yes")
    add_definitions(-DFIRELANDS_DEBUG)
else()
    message("* Use coreside debug     : No  (default)")
endif()

if(WIN32)
    if(USE_MYSQL_SOURCES)
        message("* Use MySQL sourcetree   : Yes (default)")
    else()
        message("* Use MySQL sourcetree   : No")
    endif()
endif(WIN32)

if(NOJEM)
    message("")
    message("*** WARNING: jemalloc linking has been disabled!")
    message("*** Please note that this is for DEBUGGING WITH VALGRIND only!")
    message("*** DO NOT DISABLE IT UNLESS YOU KNOW WHAT YOU'RE DOING!")
endif()

if(ENABLE_VMAP_CHECKS)
    message("* Enable vmap DisableMgr checks   : Yes (default)")
    add_definitions(-DENABLE_VMAP_CHECKS)
else()
    message("* Enable vmap DisableMgr checks   : No")
endif()

if(WITH_STRICT_DATABASE_TYPE_CHECKS)
  message("")
  message(" *** WITH_STRICT_DATABASE_TYPE_CHECKS - WARNING!")
  message(" *** Validates uses of database Get***() functions from Field class")
  message(" *** invalid calls will result in returning value 0")
  message(" *** NOT COMPATIBLE WITH MARIADB!")
  add_definitions(-DFIRELANDS_STRICT_DATABASE_TYPE_CHECKS)
endif()


message("")
