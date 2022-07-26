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

#ifndef FIRELANDS_PASSIVEAI_H
#define FIRELANDS_PASSIVEAI_H

#include "CreatureAI.h"

class PassiveAI : public CreatureAI
{
    public:
        explicit PassiveAI(Creature* c);

        void MoveInLineOfSight(Unit*) {}
        void AttackStart(Unit*) {}
        void UpdateAI(const uint32);

        static int Permissible(const Creature*) { return PERMIT_BASE_IDLE;  }
};

class PossessedAI : public CreatureAI
{
    public:
        explicit PossessedAI(Creature* c);

        void MoveInLineOfSight(Unit*) {}
        void AttackStart(Unit* target);
        void UpdateAI(const uint32);
        void EnterEvadeMode() {}

        void JustDied(Unit*);
        void KilledUnit(Unit* victim);

        static int Permissible(const Creature*) { return PERMIT_BASE_IDLE;  }
};

class NullCreatureAI : public CreatureAI
{
    public:
        explicit NullCreatureAI(Creature* c);

        void MoveInLineOfSight(Unit*) {}
        void AttackStart(Unit*) {}
        void UpdateAI(const uint32) {}
        void EnterEvadeMode() {}
        void OnCharmed(bool /*apply*/) {}

        static int Permissible(const Creature*) { return PERMIT_BASE_IDLE;  }
};

class CritterAI : public PassiveAI
{
    public:
        explicit CritterAI(Creature* c) : PassiveAI(c) {}

        void DamageTaken(Unit* done_by, uint32& /*damage*/);
        void EnterEvadeMode();
};

class TriggerAI : public NullCreatureAI
{
    public:
        explicit TriggerAI(Creature* c) : NullCreatureAI(c) {}
        void IsSummonedBy(Unit* summoner);
};

class TeleporterAI : public CreatureAI
{
    public:
        explicit TeleporterAI(Creature* c);

        void Reset();
        void MoveInLineOfSight(Unit*) {}
        void AttackStart(Unit*) {}
        void SetData(uint32 uiType, uint32 uiData);
        void UpdateAI(const uint32);

        static int Permissible(const Creature*) { return PERMIT_BASE_IDLE;  }

        bool OnGossipHello(Player* player, Creature* creature);
        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action);
        void SetDirectTeleportToLastDest(bool val) { isTeleportedAtLastDest = val; }
        void SetVisibleOnActiveTeleportations(bool val);

     private:
        std::map<uint32, bool > _teleportationStates;
        bool isTeleportedAtLastDest;
        bool isVisibleOnActiveTeleportation;
};

#endif
