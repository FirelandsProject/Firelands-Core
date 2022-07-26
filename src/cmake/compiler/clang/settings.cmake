# Set build-directive (used in core to tell which buildtype we used)
add_definitions(-D_BUILD_DIRECTIVE="${CMAKE_BUILD_TYPE}")

# This tests for a bug in clang-7 that causes linkage to fail for 64-bit from_chars (in some configurations)
# If the clang requirement is bumped to >= clang-8, you can remove this check, as well as
# the associated ifdef block in src/common/Utilities/StringConvert.h
include(CheckCXXSourceCompiles)

check_cxx_source_compiles("
#include <charconv>
#include <cstdint>
int main()
{
    uint64_t n;
    char const c[] = \"0\";
    std::from_chars(c, c+1, n);
    return static_cast<int>(n);
}
" CLANG_HAVE_PROPER_CHARCONV)

if(NOT CLANG_HAVE_PROPER_CHARCONV)
    message(STATUS "Clang: Detected from_chars bug for 64-bit integers, workaround enabled")
    target_compile_definitions(fc-compile-option-interface
        INTERFACE
        -DFIRELANDS_NEED_CHARCONV_WORKAROUND)
endif()

if(WITH_WARNINGS)
    set(WARNING_FLAGS "-W -Wall -Wextra -Winit-self -Wfatal-errors")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${WARNING_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WARNING_FLAGS} -Woverloaded-virtual")
    message(STATUS "Clang: All warnings enabled")
endif()

if(WITH_COREDEBUG)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g3")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3")
    message(STATUS "Clang: Debug-flags set (-g3)")
endif()
