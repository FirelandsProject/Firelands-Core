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

#ifndef FIRELANDS_H_MOVE_MAP
#define FIRELANDS_H_MOVE_MAP

#include "Define.h"

#include "../../dep/recastnavigation/Detour/Include/DetourAlloc.h"
#include "../../dep/recastnavigation/Detour/Include/DetourNavMesh.h"
#include "../../dep/recastnavigation/Detour/Include/DetourNavMeshQuery.h"

#include "Utilities/UnorderedMapSet.h"

class Unit;

//  memory management
inline void* dtCustomAlloc(size_t size, dtAllocHint /*hint*/)
{
    return (void*)new unsigned char[size];
}

inline void dtCustomFree(void* ptr)
{
    delete[](unsigned char*)ptr;
}

//  move map related classes
namespace MMAP
{
    typedef UNORDERED_MAP<uint32, dtTileRef> MMapTileSet;
    typedef UNORDERED_MAP<uint32, dtNavMeshQuery*> NavMeshQuerySet;

    // dummy struct to hold map's mmap data
    struct MMapData
    {
        MMapData(dtNavMesh* mesh) : navMesh(mesh) {}
        ~MMapData()
        {
            for (NavMeshQuerySet::iterator i = navMeshQueries.begin(); i != navMeshQueries.end(); ++i)
            {
                dtFreeNavMeshQuery(i->second);
            }

            if (navMesh)
            {
                dtFreeNavMesh(navMesh);
            }
        }

        dtNavMesh* navMesh;

        // we have to use single dtNavMeshQuery for every instance, since those are not thread safe
        NavMeshQuerySet navMeshQueries;     // instanceId to query
        MMapTileSet mmapLoadedTiles;        // maps [map grid coords] to [dtTile]
    };


    typedef UNORDERED_MAP<uint32, MMapData*> MMapDataSet;

    // singelton class
    // holds all all access to mmap loading unloading and meshes
    class MMapManager
    {
        public:
            MMapManager() : loadedTiles(0) {}
            ~MMapManager();

            bool loadMap(uint32 mapId, int32 x, int32 y);
            bool unloadMap(uint32 mapId, int32 x, int32 y);
            bool unloadMap(uint32 mapId);
            bool unloadMapInstance(uint32 mapId, uint32 instanceId);

            // the returned [dtNavMeshQuery const*] is NOT threadsafe
            dtNavMeshQuery const* GetNavMeshQuery(uint32 mapId, uint32 instanceId);
            dtNavMesh const* GetNavMesh(uint32 mapId);

            uint32 getLoadedTilesCount() const { return loadedTiles; }
            uint32 getLoadedMapsCount() const { return loadedMMaps.size(); }
        private:
            bool loadMapData(uint32 mapId);
            uint32 packTileID(int32 x, int32 y);

            MMapDataSet loadedMMaps;
            uint32 loadedTiles;
    };

    // static class
    // holds all mmap global data
    // access point to MMapManager singelton
    class MMapFactory
    {
        public:
            static MMapManager* createOrGetMMapManager();
            static void clear();
            static void preventPathfindingOnMaps(const char* ignoreMapIds);
            static bool IsPathfindingEnabled(uint32 mapId, const Unit* unit);
            static bool IsPathfindingForceEnabled(const Unit* unit);
            static bool IsPathfindingForceDisabled(const Unit* unit);
    };
}

#endif  // _MOVE_MAP_H