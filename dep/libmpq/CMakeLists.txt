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

#project(libmpq VERSION 0.4.2)

include(CheckIncludeFiles)
include(CheckFunctionExists)

check_include_files(dlfcn.h HAVE_DLFCN_H)
check_include_files(inttypes.h HAVE_INTTYPES_H)
check_include_files(memory.h HAVE_MEMORY_H)
check_include_files(stdint.h HAVE_STDINT_H)
check_include_files(stdlib.h HAVE_STDLIB_H)
check_include_files(strings.h HAVE_STRINGS_H)
check_include_files(string.h HAVE_STRING_H)
check_include_files(sys/stat.h HAVE_SYS_STAT_H)
check_include_files(sys/types.h HAVE_SYS_TYPES_H)
check_include_files(unistd.h HAVE_UNISTD_H)
check_function_exists(fseeko HAVE_FSEEKO)

set(STDC_HEADERS 1)
set(HAVE_LIBBZ2 1)
set(HAVE_LIBZ 1)

set(LIBMPQ_PACKAGE_VERSION "0.4.2")
set(LIBMPQ_PACKAGE_NAME "libmpq")

configure_file(libmpq/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)

add_library(libmpq STATIC
    libmpq/common.c
    libmpq/common.h
    libmpq/crypt_buf.h
    libmpq/explode.c
    libmpq/explode.h
    libmpq/extract.c
    libmpq/extract.h
    libmpq/huffman.c
    libmpq/huffman.h
    libmpq/mpq-internal.h
    libmpq/mpq.c
    libmpq/mpq.h
    libmpq/platform.h
    libmpq/wave.c
    libmpq/wave.h
    ${CMAKE_CURRENT_BINARY_DIR}/config.h
)

target_include_directories(libmpq
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(libmpq
    PRIVATE
        ZLIB::ZLIB
        BZip2::BZip2
)
