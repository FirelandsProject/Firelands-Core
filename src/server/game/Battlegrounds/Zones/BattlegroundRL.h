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
#ifndef __BATTLEGROUNDRL_H
#define __BATTLEGROUNDRL_H

#include "Arena.h"

enum BattlegroundRLObjectTypes
{
    BG_RL_OBJECT_DOOR_1 = 0,
    BG_RL_OBJECT_DOOR_2 = 1,
    BG_RL_OBJECT_BUFF_1 = 2,
    BG_RL_OBJECT_BUFF_2 = 3,
    BG_RL_OBJECT_READYMARKER_1 = 4,
    BG_RL_OBJECT_READYMARKER_2 = 5,
    BG_RL_OBJECT_MAX = 6
};

enum BattlegroundRLObjects
{
    BG_RL_OBJECT_TYPE_DOOR_1 = 185918,
    BG_RL_OBJECT_TYPE_DOOR_2 = 185917,
    BG_RL_OBJECT_TYPE_BUFF_1 = 184663,
    BG_RL_OBJECT_TYPE_BUFF_2 = 184664,
    BG_RL_OBJECT_READYMARKER = 940000
};

class FC_GAME_API BattlegroundRL : public Arena
{
public:
    BattlegroundRL();

    /* inherited from BattlegroundClass */
    void FillInitialWorldStates(WorldPacket& d) override;
    void StartingEventCloseDoors() override;
    void StartingEventOpenDoors() override;

    void HandleAreaTrigger(Player* player, uint32 trigger) override;
    bool SetupBattleground() override;
    bool HandlePlayerUnderMap(Player* player) override;
};
#endif
