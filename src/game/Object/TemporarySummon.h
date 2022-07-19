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

#ifndef _FIRELANDS_TEMPSUMMON_H
#define _FIRELANDS_TEMPSUMMON_H

#include "Creature.h"
#include "ObjectAccessor.h"

class TemporarySummon : public Creature
{
    public:
        explicit TemporarySummon(ObjectGuid summoner = ObjectGuid());
        virtual ~TemporarySummon() {};

        void Update(uint32 update_diff, uint32 time) override;
        void SetSummonProperties(TempSpawnType type, uint32 lifetime);
        void Summon(TempSpawnType type, uint32 lifetime);
        void UnSummon();
        void SaveToDB();
        ObjectGuid const& GetSummonerGuid() const { return m_summoner ; }
        Unit* GetSummoner() const { return sObjectAccessor.GetUnit(*this, m_summoner); }
        void SetLinkedToOwnerAura(uint32 flags) { m_linkedToOwnerAura |= flags; };
    private:
        void RemoveAuraFromOwner();
        void SaveToDB(uint32, uint8, uint32) override       // overwrited of Creature::SaveToDB     - don't must be called
        {
            FIRELANDS_ASSERT(false);
        }
        void DeleteFromDB() override                        // overwrited of Creature::DeleteFromDB - don't must be called
        {
            FIRELANDS_ASSERT(false);
        }

        TempSpawnType m_type;
        uint32 m_timer;
        uint32 m_lifetime;
        ObjectGuid m_summoner;
        uint32 m_linkedToOwnerAura;
};

class TemporarySummonWaypoint : public TemporarySummon
{
    public:
        explicit TemporarySummonWaypoint(ObjectGuid summoner, uint32 waypoint_id, int32 path_id, uint32 pathOrigin);

        uint32 GetWaypointId() const { return m_waypoint_id; }
        int32 GetPathId() const { return m_path_id; }
        uint32 GetPathOrigin() const { return m_pathOrigin; }

    private:
        uint32 m_waypoint_id;
        int32 m_path_id;
        uint32 m_pathOrigin;
};

#endif
