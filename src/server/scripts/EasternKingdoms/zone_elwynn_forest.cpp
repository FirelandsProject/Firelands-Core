/*
 * Copyright (C) 2011-2012 Project SkyFire <http://www.projectskyfire.org/>
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2006-2012 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
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

#include "Player.h"
#include "ScriptedCreature.h"
#include "ScriptMgr.h"

enum Northshire
{
    NPC_BLACKROCK_BATTLE_WORG = 49871,
    NPC_STORMWIND_INFANTRY    = 49869,
    NPC_BROTHER_PAXTON        = 951,
    NPC_INJURED_SOLDIER_DUMMY = 50378,

    SPELL_SPYING              = 92857,
    SPELL_SNEAKING            = 93046,
    SPELL_SPYGLASS            = 80676,

    SPELL_RENEW               = 93094,
    SPELL_PRAYER_OF_HEALING   = 93091,
    SPELL_FORTITUDE           = 13864,
    SPELL_PENANCE             = 47750,
    SPELL_FLASH_HEAL          = 17843,
    SPELL_RENEWEDLIFE         = 93097,

    ACTION_HEAL               = 1,
    EVENT_HEALED_1            = 1,
    EVENT_HEALED_2            = 2
};

class npc_stormwind_infantry : public CreatureScript
{
public:
    npc_stormwind_infantry() : CreatureScript("npc_stormwind_infantry") { }

    struct npc_stormwind_infantryAI : public ScriptedAI
    {
        npc_stormwind_infantryAI(Creature* creature) : ScriptedAI(creature)
        {
            me->HandleEmoteCommand(EMOTE_STATE_READY1H);
        }

        EventMap events;

        void EnterCombat(Unit * who)
        {
            me->AddUnitState(UNIT_STATE_ROOT);
            me->HandleEmoteCommand(EMOTE_STATE_READY1H);
        }

        void Reset()
        {
            me->HandleEmoteCommand(EMOTE_STATE_READY1H);
        }

        void DamageTaken(Unit* attacker, uint32& damage)
        {
            if (attacker->GetEntry() == NPC_BLACKROCK_BATTLE_WORG && damage >= me->GetHealth())
                me->SetHealth(me->GetMaxHealth());
        }

        void UpdateAI(const uint32 diff)
        {
            DoMeleeAttackIfReady();
        }
    };
    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_stormwind_infantryAI (creature);
    }
};

/*######
## npc_blackrock_battle_worg
######*/

class npc_blackrock_battle_worg : public CreatureScript
{
public:
    npc_blackrock_battle_worg() : CreatureScript("npc_blackrock_battle_worg") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_blackrock_battle_worgAI (creature);
    }

    struct npc_blackrock_battle_worgAI : public ScriptedAI
    {
        npc_blackrock_battle_worgAI(Creature* creature) : ScriptedAI(creature) { }

        void DamageTaken(Unit* who, uint32& damage)
        {
            if (who->GetEntry() == NPC_STORMWIND_INFANTRY && damage >= me->GetHealth())
                me->SetHealth(me->GetMaxHealth());

            if (who->GetTypeId() == TYPEID_PLAYER || who->isPet())
            {
                if (Creature* guard = me->FindNearestCreature(NPC_STORMWIND_INFANTRY, 6.0f, true))
                {
                    guard->getThreatManager().resetAllAggro();
                    guard->CombatStop(true);
                }

                me->LowerPlayerDamageReq(me->GetMaxHealth());
                me->getThreatManager().resetAllAggro();
                me->GetMotionMaster()->MoveChase(who);
                me->AI()->AttackStart(who);
            }
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
            {
                if (Creature* guard = me->FindNearestCreature(NPC_STORMWIND_INFANTRY, 6.0f, true))
                {
                    me->SetReactState(REACT_AGGRESSIVE);
                    me->AI()->AttackStart(guard);
                }
            }

            DoMeleeAttackIfReady();
        }
    };
};

class npc_injured_soldier : public CreatureScript
{
public:
    npc_injured_soldier() : CreatureScript("npc_injured_soldier") { }

    struct npc_injured_soldierAI : public ScriptedAI
    {
        npc_injured_soldierAI(Creature* creature) : ScriptedAI(creature) { }

        EventMap events;
        Player* owner;

        void Reset()
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_UNK_15);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_UNK_14);
            me->SetFlag(UNIT_FIELD_BYTES_1, 7);
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        }

        void SpellHit(Unit* /*target*/, SpellInfo const* spell)
        {
            if (spell->Id == SPELL_RENEWEDLIFE)
            {
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_UNK_15);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_UNK_14);
                me->RemoveFlag(UNIT_FIELD_BYTES_1, 7);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                DoAction(ACTION_HEAL);
                owner = me->FindNearestPlayer(8.0f);
            }
        }

        void DoAction(int32 const action)
        {
            switch (action)
            {
            case ACTION_HEAL:
                events.ScheduleEvent(EVENT_HEALED_1, 2000);
                break;
            }
        }

        void UpdateAI(uint32 const diff)
        {
            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_HEALED_1:
                    if (owner)
                    {
                        switch(urand(0, 3))
                        {
                        case 0:
                            me->MonsterSay("I will fear no evil!", 0, 0);
                            break;
                        case 1:
                            me->MonsterSay("I... I'm okay! I'm okay!", 0, 0);
                            break;
                        case 2:
                            me->MonsterSay("Bless you, hero!", 0, 0);
                            break;
                        case 3:
                            me->MonsterSay("You are $p! The hero that everyone has been talking about! Thank you!", 0, owner->GetGUID());
                            break;
                        default:
                            break;
                        }
                        me->HandleEmoteCommand(EMOTE_ONESHOT_CHEER);
                        owner->KilledMonsterCredit(me->GetEntry(), 0);
                        events.ScheduleEvent(EVENT_HEALED_2, 2500);
                    }
                    break;
                case EVENT_HEALED_2:
                    me->GetMotionMaster()->MovePoint(0, -8914.525f, -133.963f, 80.534f);
                    me->DespawnOrUnsummon(8000);
                    break;
                default:
                    break;
                }
            }
        }
    };
    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_injured_soldierAI (creature);
    }
};

void AddSC_elwynn_forest()
{
    new npc_injured_soldier();
    new npc_blackrock_battle_worg();
    new npc_stormwind_infantry();
}
