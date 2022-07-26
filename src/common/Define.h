/*
 * Copyright (C) 2022 Firelands Project <https://github.com/FirelandsProject>
 * Copyright (C) 2008-2013 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef FIRELANDS_DEFINE_H
#define FIRELANDS_DEFINE_H

#include "CompilerDefs.h"
#include <cinttypes>
#include <climits>
#include <cstddef>

#define FIRELANDS_LITTLEENDIAN 0
#define FIRELANDS_BIGENDIAN    1

#if !defined(FIRELANDS_ENDIAN)
#    if defined(BOOST_BIG_ENDIAN)
#           define FIRELANDS_ENDIAN FIRELANDS_BIGENDIAN
#    else
#           define FIRELANDS_ENDIAN FIRELANDS_LITTLEENDIAN
#   endif
#endif

#if PLATFORM == PLATFORM_WINDOWS
#  define FIRELANDS_PATH_MAX MAX_PATH
#  define _USE_MATH_DEFINES
#else //PLATFORM != PLATFORM_WINDOWS
#  define FIRELANDS_PATH_MAX PATH_MAX
#endif //PLATFORM

#if !defined(COREDEBUG)
#  define FIRELANDS_INLINE inline
#else //COREDEBUG
#  if !defined(FIRELANDS_DEBUG)
#    define FIRELANDS_DEBUG
#  endif //FIRELANDS_DEBUG
#  define FIRELANDS_INLINE
#endif //!COREDEBUG

#if COMPILER == COMPILER_GNU
#  define ATTR_PRINTF(F, V) __attribute__ ((format (printf, F, V)))
#else //COMPILER != COMPILER_GNU
#  define ATTR_PRINTF(F, V)
#endif //COMPILER == COMPILER_GNU

#ifdef FIRELANDS_API_USE_DYNAMIC_LINKING
#  if COMPILER == COMPILER_MICROSOFT
#    define FC_API_EXPORT __declspec(dllexport)
#    define FC_API_IMPORT __declspec(dllimport)
#  elif COMPILER == COMPILER_GNU
#    define FC_API_EXPORT __attribute__((visibility("default")))
#    define FC_API_IMPORT
#  else
#    error compiler not supported!
#  endif
#else
#  define FC_API_EXPORT
#  define FC_API_IMPORT
#endif

#ifdef FIRELANDS_API_EXPORT_COMMON
#  define FC_COMMON_API FC_API_EXPORT
#else
#  define FC_COMMON_API FC_API_IMPORT
#endif

#ifdef FIRELANDS_API_EXPORT_DATABASE
#  define FC_DATABASE_API FC_API_EXPORT
#else
#  define FC_DATABASE_API FC_API_IMPORT
#endif

#ifdef FIRELANDS_API_EXPORT_SHARED
#  define FC_SHARED_API FC_API_EXPORT
#else
#  define FC_SHARED_API FC_API_IMPORT
#endif

#ifdef FIRELANDS_API_EXPORT_GAME
#  define FC_GAME_API FC_API_EXPORT
#else
#  define FC_GAME_API FC_API_IMPORT
#endif

#define UI64LIT(N) UINT64_C(N)
#define SI64LIT(N) INT64_C(N)

#define STRING_VIEW_FMT_ARG(str) static_cast<int>((str).length()), (str).data()

typedef std::int64_t int64;
typedef std::int32_t int32;
typedef std::int16_t int16;
typedef std::int8_t int8;
typedef std::uint64_t uint64;
typedef std::uint32_t uint32;
typedef std::uint16_t uint16;
typedef std::uint8_t uint8;


enum DBCFormer
{
    FT_NA = 'x',                                              //not used or unknown, 4 byte size
    FT_NA_BYTE = 'X',                                         //not used or unknown, byte
    FT_STRING = 's',                                          //char*
    FT_FLOAT = 'f',                                           //float
    FT_INT = 'i',                                             //uint32
    FT_BYTE = 'b',                                            //uint8
    FT_SORT = 'd',                                            //sorted by this field, field is not included
    FT_IND = 'n',                                             //the same, but parsed to data
    FT_LOGIC = 'l',                                           //Logical (boolean)
    FT_SQL_PRESENT = 'p',                                     //Used in sql format to mark column present in sql dbc
    FT_SQL_ABSENT = 'a'                                       //Used in sql format to mark column absent in sql dbc
};
#endif //FIRELANDS_DEFINE_H
