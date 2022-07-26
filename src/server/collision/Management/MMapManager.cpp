/*
 * Copyright (C) 2022 Firelands Project <https://github.com/FirelandsProject>
 * Copyright (C) 2008-2013 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2010 MaNGOS <http://getmangos.com/>
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

#include "MMapManager.h"
#include "Log.h"
#include "World.h"

namespace MMAP
{
    // ######################## MMapManager ########################
    MMapManager::~MMapManager()
    {
        for (MMapDataSet::iterator i = loadedMMaps.begin(); i != loadedMMaps.end(); ++i)
            delete i->second;

        // by now we should not have maps loaded
        // if we had, tiles in MMapData->mmapLoadedTiles, their actual data is lost!
    }

    bool MMapManager::loadMapData(uint32 mapId)
    {
        // we already have this map loaded?
        if (loadedMMaps.find(mapId) != loadedMMaps.end())
            return true;

        // load and init dtNavMesh - read parameters from file
        uint32 pathLen = sWorld->GetDataPath().length() + strlen("mmaps/%03i.mmap")+1;
        char *fileName = new char[pathLen];
        snprintf(fileName, pathLen, (sWorld->GetDataPath()+"mmaps/%03i.mmap").c_str(), mapId);

        FILE* file = fopen(fileName, "rb");
        if (!file)
        {
            LOG_DEBUG("maps", "MMAP:loadMapData: Error: Could not open mmap file '{}'", fileName);
            delete [] fileName;
            return false;
        }

        dtNavMeshParams params;
        int count = fread(&params, sizeof(dtNavMeshParams), 1, file);
        fclose(file);
        if (count != 1)
        {
            LOG_DEBUG("maps", "MMAP:loadMapData: Error: Could not read params from file '{}'", fileName);
            delete [] fileName;
            return false;
        }

        dtNavMesh* mesh = dtAllocNavMesh();
        ASSERT(mesh);
        if (dtStatusFailed(mesh->init(&params)))
        {
            dtFreeNavMesh(mesh);
            LOG_ERROR("maps", "MMAP:loadMapData: Failed to initialize dtNavMesh for mmap {:03} from file {}", mapId, fileName);
            delete [] fileName;
            return false;
        }

        delete [] fileName;

        LOG_INFO("maps", "MMAP:loadMapData: Loaded {:03}.mmap", mapId);

        // store inside our map list
        MMapData* mmap_data = new MMapData(mesh);
        mmap_data->mmapLoadedTiles.clear();

        loadedMMaps.insert(std::pair<uint32, MMapData*>(mapId, mmap_data));
        return true;
    }

    uint32 MMapManager::packTileID(int32 x, int32 y)
    {
        return uint32(x << 16 | y);
    }

    bool MMapManager::loadMap(const std::string& /*basePath*/, uint32 mapId, int32 x, int32 y)
    {
        // make sure the mmap is loaded and ready to load tiles
        if (!loadMapData(mapId))
            return false;

        // get this mmap data
        MMapData* mmap = loadedMMaps[mapId];
        ASSERT(mmap->navMesh);

        // check if we already have this tile loaded
        uint32 packedGridPos = packTileID(x, y);
        if (mmap->mmapLoadedTiles.find(packedGridPos) != mmap->mmapLoadedTiles.end())
            return false;

        // load this tile :: mmaps/MMMXXYY.mmtile
        uint32 pathLen = sWorld->GetDataPath().length() + strlen("mmaps/{:03}{:02}{:02}.mmtile")+1;
        char *fileName = new char[pathLen];

        snprintf(fileName, pathLen, (sWorld->GetDataPath()+"mmaps/{:03}{:02}{:02}.mmtile"), mapId, x, y);

        FILE *file = fopen(fileName, "rb");
        if (!file)
        {
            LOG_DEBUG("maps", "MMAP:loadMap: Could not open mmtile file '{}'", fileName);
            delete [] fileName;
            return false;
        }
        delete [] fileName;

        // read header
        MmapTileHeader fileHeader;
        if (fread(&fileHeader, sizeof(MmapTileHeader), 1, file) != 1 || fileHeader.mmapMagic != MMAP_MAGIC)
        {
            LOG_ERROR("maps", "MMAP:loadMap: Bad header in mmap {:03}{:02}{:02}.mmtile", mapId, x, y);
            return false;
        }

        if (fileHeader.mmapVersion != MMAP_VERSION)
        {
            LOG_ERROR("maps", "MMAP:loadMap: {:03}{:02}{:02}.mmtile was built with generator v{}, expected v{}",
                mapId, x, y, fileHeader.mmapVersion, MMAP_VERSION);
            return false;
        }

        unsigned char* data = (unsigned char*)dtAlloc(fileHeader.size, DT_ALLOC_PERM);
        ASSERT(data);

        size_t result = fread(data, fileHeader.size, 1, file);
        if (!result)
        {
            LOG_ERROR("maps", "MMAP:loadMap: Bad header or data in mmap {:03}{:02}{:02}.mmtile", mapId, x, y);
            fclose(file);
            return false;
        }

        fclose(file);

        dtMeshHeader* header = (dtMeshHeader*)data;
        dtTileRef tileRef = 0;

        // memory allocated for data is now managed by detour, and will be deallocated when the tile is removed
        if (dtStatusSucceed(mmap->navMesh->addTile(data, fileHeader.size, DT_TILE_FREE_DATA, 0, &tileRef)))
        {
            mmap->mmapLoadedTiles.insert(std::pair<uint32, dtTileRef>(packedGridPos, tileRef));
            ++loadedTiles;
            LOG_INFO("maps", "MMAP:loadMap: Loaded mmtile {:03}[{:02},{:02}] into {:03}[{:02},{:02}]", mapId, x, y, mapId, header->x, header->y);
            return true;
        }
        else
        {
            LOG_ERROR("maps", "MMAP:loadMap: Could not load {:03}{:02}{:02}.mmtile into navmesh", mapId, x, y);
            dtFree(data);
            return false;
        }

        return false;
    }

    bool MMapManager::unloadMap(uint32 mapId, int32 x, int32 y)
    {
        // check if we have this map loaded
        if (loadedMMaps.find(mapId) == loadedMMaps.end())
        {
            // file may not exist, therefore not loaded
            LOG_DEBUG("maps", "MMAP:unloadMap: Asked to unload not loaded navmesh map. {:03}{:02}{:02}.mmtile", mapId, x, y);
            return false;
        }

        MMapData* mmap = loadedMMaps[mapId];

        // check if we have this tile loaded
        uint32 packedGridPos = packTileID(x, y);
        if (mmap->mmapLoadedTiles.find(packedGridPos) == mmap->mmapLoadedTiles.end())
        {
            // file may not exist, therefore not loaded
            LOG_DEBUG("maps", "MMAP:unloadMap: Asked to unload not loaded navmesh tile. {:03}{:02}{:02}.mmtile", mapId, x, y);
            return false;
        }

        dtTileRef tileRef = mmap->mmapLoadedTiles[packedGridPos];

        // unload, and mark as non loaded
        if (dtStatusFailed(mmap->navMesh->removeTile(tileRef, NULL, NULL)))
        {
            // this is technically a memory leak
            // if the grid is later reloaded, dtNavMesh::addTile will return error but no extra memory is used
            // we cannot recover from this error - assert out
            LOG_ERROR("maps", "MMAP:unloadMap: Could not unload {:03}{:02}{:02}.mmtile from navmesh", mapId, x, y);
            ASSERT(false);
        }
        else
        {
            mmap->mmapLoadedTiles.erase(packedGridPos);
            --loadedTiles;
            LOG_INFO("maps", "MMAP:unloadMap: Unloaded mmtile {:03}[{:02},{:02}] from {:03}", mapId, x, y, mapId);
            return true;
        }

        return false;
    }

    bool MMapManager::unloadMap(uint32 mapId)
    {
        if (loadedMMaps.find(mapId) == loadedMMaps.end())
        {
            // file may not exist, therefore not loaded
            LOG_DEBUG("maps", "MMAP:unloadMap: Asked to unload not loaded navmesh map {:03}", mapId);
            return false;
        }

        // unload all tiles from given map
        MMapData* mmap = loadedMMaps[mapId];
        for (MMapTileSet::iterator i = mmap->mmapLoadedTiles.begin(); i != mmap->mmapLoadedTiles.end(); ++i)
        {
            uint32 x = (i->first >> 16);
            uint32 y = (i->first & 0x0000FFFF);
            if (dtStatusFailed(mmap->navMesh->removeTile(i->second, NULL, NULL)))
                LOG_ERROR("maps", "MMAP:unloadMap: Could not unload {:03}{:02}{:02}.mmtile from navmesh", mapId, x, y);
            else
            {
                --loadedTiles;
                LOG_INFO("maps", "MMAP:unloadMap: Unloaded mmtile {:03}[{:02},{:02}] from {:03}", mapId, x, y, mapId);
            }
        }

        delete mmap;
        loadedMMaps.erase(mapId);
        LOG_INFO("maps", "MMAP:unloadMap: Unloaded {:03}.mmap", mapId);

        return true;
    }

    bool MMapManager::unloadMapInstance(uint32 mapId, uint32 instanceId)
    {
        // check if we have this map loaded
        if (loadedMMaps.find(mapId) == loadedMMaps.end())
        {
            // file may not exist, therefore not loaded
            LOG_DEBUG("maps", "MMAP:unloadMapInstance: Asked to unload not loaded navmesh map {:03}", mapId);
            return false;
        }

        MMapData* mmap = loadedMMaps[mapId];
        if (mmap->navMeshQueries.find(instanceId) == mmap->navMeshQueries.end())
        {
            LOG_DEBUG("maps", "MMAP:unloadMapInstance: Asked to unload not loaded dtNavMeshQuery mapId {:03} instanceId {}", mapId, instanceId);
            return false;
        }

        dtNavMeshQuery* query = mmap->navMeshQueries[instanceId];

        dtFreeNavMeshQuery(query);
        mmap->navMeshQueries.erase(instanceId);
        LOG_INFO("maps", "MMAP:unloadMapInstance: Unloaded mapId {:03} instanceId {}", mapId, instanceId);

        return true;
    }

    dtNavMesh const* MMapManager::GetNavMesh(uint32 mapId)
    {
        if (loadedMMaps.find(mapId) == loadedMMaps.end())
            return NULL;

        return loadedMMaps[mapId]->navMesh;
    }

    dtNavMeshQuery const* MMapManager::GetNavMeshQuery(uint32 mapId, uint32 instanceId)
    {
        if (loadedMMaps.find(mapId) == loadedMMaps.end())
            return NULL;

        MMapData* mmap = loadedMMaps[mapId];
        if (mmap->navMeshQueries.find(instanceId) == mmap->navMeshQueries.end())
        {
            // allocate mesh query
            dtNavMeshQuery* query = dtAllocNavMeshQuery();
            ASSERT(query);
            if (dtStatusFailed(query->init(mmap->navMesh, 1024)))
            {
                dtFreeNavMeshQuery(query);
                LOG_ERROR("maps", "MMAP:GetNavMeshQuery: Failed to initialize dtNavMeshQuery for mapId {:03} instanceId {}", mapId, instanceId);
                return NULL;
            }

            LOG_INFO("maps", "MMAP:GetNavMeshQuery: created dtNavMeshQuery for mapId {:03} instanceId {}", mapId, instanceId);
            mmap->navMeshQueries.insert(std::pair<uint32, dtNavMeshQuery*>(instanceId, query));
        }

        return mmap->navMeshQueries[instanceId];
    }
}