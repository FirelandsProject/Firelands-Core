# Copyright (C) 2008-2018 FirelandsCore <http://www.FirelandsCore.org/>
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

# An interface library to make the target com available to other targets
add_library(firelands-compile-option-interface INTERFACE)

# Use -std=c++11 instead of -std=gnu++11
set(CXX_EXTENSIONS OFF)

# An interface library to make the target features available to other targets
add_library(firelands-feature-interface INTERFACE)

target_compile_features(firelands-feature-interface
  INTERFACE
  cxx_std_17
  cxx_alias_templates
  cxx_auto_type
  cxx_constexpr
  cxx_decltype
  cxx_decltype_auto
  cxx_final
  cxx_lambdas
  cxx_generic_lambdas
  cxx_variadic_templates
  cxx_defaulted_functions
  cxx_nullptr
  cxx_trailing_return_types
  cxx_return_type_deduction)

# An interface library to make the warnings level available to other targets
# This interface taget is set-up through the platform specific script
add_library(firelands-warning-interface INTERFACE)

# An interface used for all other interfaces
add_library(firelands-default-interface INTERFACE)
target_link_libraries(firelands-default-interface
  INTERFACE
  firelands-compile-option-interface
  firelands-feature-interface)

# An interface used for silencing all warnings
add_library(firelands-no-warning-interface INTERFACE)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  target_compile_options(firelands-no-warning-interface
    INTERFACE
    /W0)
else()
  target_compile_options(firelands-no-warning-interface
    INTERFACE
    -w)
endif()

# An interface library to change the default behaviour
# to hide symbols automatically.
add_library(firelands-hidden-symbols-interface INTERFACE)

# An interface amalgamation which provides the flags and definitions
# used by the dependency targets.
add_library(firelands-dependency-interface INTERFACE)
target_link_libraries(firelands-dependency-interface
  INTERFACE
  firelands-default-interface
  firelands-no-warning-interface
  firelands-hidden-symbols-interface)

# An interface amalgamation which provides the flags and definitions
# used by the core targets.
add_library(firelands-core-interface INTERFACE)
target_link_libraries(firelands-core-interface
  INTERFACE
  firelands-default-interface
  firelands-warning-interface)
