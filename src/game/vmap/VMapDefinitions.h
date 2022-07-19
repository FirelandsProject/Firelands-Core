/*
 * This file is part of the Firelands Core Project. See AUTHORS file for Copyright information
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

#ifndef FIRELANDS_H_VMAPDEFINITIONS
#define FIRELANDS_H_VMAPDEFINITIONS

#include <cstring>

#define LIQUID_TILE_SIZE (533.333f / 128.f)

namespace VMAP
{
    const char VMAP_MAGIC[] = "VMAP_4.0";                       /**< used in final vmap files */
    const char RAW_VMAP_MAGIC[] = "VMAPc06";                // used in extracted vmap files with raw data
    const char GAMEOBJECT_MODELS[] = "temp_gameobject_models";  /**< TODO */

    /**
     * @brief defined in TileAssembler.cpp currently
     *
     * @param rf
     * @param dest
     * @param compare
     * @param len
     * @return bool
     */
    bool readChunk(FILE* rf, char* dest, const char* compare, uint32 len);
}

#ifndef NO_CORE_FUNCS
#include "Errors.h"
#include "Log.h"
#define ERROR_LOG(...) sLog.outError(__VA_ARGS__);
#elif defined MMAP_GENERATOR
#include <assert.h>
#define FIRELANDS_ASSERT(x) assert(x)
#define DEBUG_LOG(...) 0
#define DETAIL_LOG(...) 0
#define LOG_FILTER_MAP_LOADING true
#define DEBUG_FILTER_LOG(F,...) do{ if (F) DEBUG_LOG(__VA_ARGS__); } while(0)
#define ERROR_LOG(...) do{ printf("ERROR:"); printf(__VA_ARGS__); printf("\n"); } while(0)
#else
#include <assert.h>
#define FIRELANDS_ASSERT(x) assert(x)
#define DEBUG_LOG(...) do{ printf(__VA_ARGS__); printf("\n"); } while(0)
#define DETAIL_LOG(...) do{ printf(__VA_ARGS__); printf("\n"); } while(0)
#define LOG_FILTER_MAP_LOADING true
#define DEBUG_FILTER_LOG(F,...) do{ if (F) DEBUG_LOG(__VA_ARGS__); } while(0)
#define ERROR_LOG(...) do{ printf("ERROR:"); printf(__VA_ARGS__); printf("\n"); } while(0)
#endif

#endif // _VMAPDEFINITIONS_H
