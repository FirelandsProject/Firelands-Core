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

/** \file
    \ingroup worldserver
*/

#include "Common.h"
#include "WorldSocket.h"
#include "WorldSocketMgr.h"
#include "World.h"
#include "WorldThread.h"
#include "Timer.h"
#include "ObjectAccessor.h"
#include "MapManager.h"
#include "Database/DatabaseEnv.h"

#include <chrono>
#include <thread>

#ifdef ENABLE_ELUNA
#include "LuaEngine.h"
#endif /* ENABLE_ELUNA */

#define WORLD_SLEEP_CONST 50

#ifdef WIN32
  #include "ServiceWin32.h"
  extern int m_ServiceStatus;
#endif

WorldThread::WorldThread(uint16 port, const char* host) : listen_addr(port, host)
{
}

int WorldThread::open(void* unused)
{
    if (sWorldSocketMgr->StartNetwork(listen_addr) == -1)
    {
        sLog.outError("Failed to start network");
        Log::WaitBeforeContinueIfNeed();
        World::StopNow(ERROR_EXIT_CODE);
        return -1;
    }
#ifdef ENABLE_ELUNA
    sEluna->OnStartup();
#endif /* ENABLE_ELUNA */

    activate();
    return 0;
}

/// Heartbeat for the World
int WorldThread::svc()
{
    uint32 realCurrTime = 0;
    uint32 realPrevTime = getMSTime();
    sLog.outString("World Updater Thread started (%dms min update interval)", WORLD_SLEEP_CONST);

    ///- While we have not World::m_stopEvent, update the world
    while (!World::IsStopped())
    {
        ++World::m_worldLoopCounter;
        realCurrTime = getMSTime();

        uint32 diff = getMSTimeDiff(realPrevTime, realCurrTime);

        sWorld.Update(diff);
        realPrevTime = realCurrTime;

        uint32 executionTimeDiff = getMSTimeDiff(realCurrTime, getMSTime());

        // we know exactly how long it took to update the world, if the update took less than WORLD_SLEEP_CONST, sleep for WORLD_SLEEP_CONST - world update time
        if (executionTimeDiff < WORLD_SLEEP_CONST)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(WORLD_SLEEP_CONST - executionTimeDiff));
        }
#ifdef _WIN32
        if (m_ServiceStatus == 0) // service stopped
        {
            World::StopNow(SHUTDOWN_EXIT_CODE);
        }

        while (m_ServiceStatus == 2) // service paused
            Sleep(1000);
#endif
    }
#ifdef ENABLE_ELUNA
    sEluna->OnShutdown();
#endif /* ENABLE_ELUNA */
    sWorld.KickAll();                                       // save and kick all players
    sWorld.UpdateSessions(1);                               // real players unload required UpdateSessions call
    sWorldSocketMgr->StopNetwork();

    sMapMgr.UnloadAll();                                    // unload all grids (including locked in memory)

#ifdef ENABLE_ELUNA
    // Eluna must be unloaded after Maps, since ~Map calls sEluna->OnDestroy,
    //   and must be unloaded before the DB, since it can access the DB.
    Eluna::Uninitialize();
#endif /* ENABLE_ELUNA */

    sLog.outString("World Updater Thread stopped");
    return 0;
}
