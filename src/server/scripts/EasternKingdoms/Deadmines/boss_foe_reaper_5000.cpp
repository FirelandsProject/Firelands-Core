/*
 * Copyright (C) 2005-2011 MaNGOS <http://www.getmangos.com/>
 *
 * Copyright (C) 2008-2011 Firelands <http://www.trinitycore.org/>
 *
 * Copyright (C) 2006-2011 ScriptDev2 <http://www.scriptdev2.com/>
 *
 * Copyright (C) 2010-2011 Project Firelands <http://www.projecttrinity.org/>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

/* ToDo
 * Make Overdrive Randomize (Like Marrowgar)
 * Spell Harvest needs Voids + Charge (need Retail Infos)
 * Correct Timers
 * Add the OFF_LINE Spell, but first the Creatures to Activate Foe Reaper 5000
 */

#include "ScriptedCreature.h"
#include "ScriptMgr.h"
#include "deadmines.h"

enum Spells
{
    SPELL_ENERGIZE = 89132,
    SPELL_ACTIVATE = 89198,
    SPELL_ACTIVATE_2 = 89121,
    SPELL_RED_EYES = 24263,
    SPELL_ENERGIZE_2 = 91846, // hit 45979 or boss
    // All Modes
    SPELL_OFF_LINE                  = 88348,
    SPELL_OVERDRIVE                 = 88481,
    SPELL_HARVEST                   = 88495,
    SPELL_HARVEST_READY             = 88508,
    SPELL_HARVEST_2                 = 88497,
    SPELL_HARVEST_SWEEP             = 88521,
    SPELL_REAPER_STRIKE             = 88490,
    SPELL_SAFETY_RESTRICTIONS       = 88522,

    // heroic
    SPELL_SUMMON_SCORIES = 91839,
    SPELL_SCORIES_SHIELD = 91815,

    // scoried
    SPELL_FIXATE = 91830,
};

enum Events
{
    EVENT_OVERDRIVE                 = 1,
    EVENT_OVERDRIVE_MOVEMENTS,
    EVENT_OVERDRIVE_END,
    EVENT_HARVEST,
    EVENT_HARVEST_SWEEP,
    EVENT_HARVEST_MOVEMENTS,
    EVENT_SCORIES,

    EVENT_INTRO,
    EVENT_INTRO_FINAL,

    // scories
    EVENT_FIXATE,
    EVENT_FIXATE_CHECK,
};

enum Texts
{
    SAY_AGGRO                       = 0,
    EMOTE_OVERDRIVE                 = 1,
    SAY_OVERDRIVE                   = 2,
    SAY_HARVEST                     = 3,
    EMOTE_HARVEST                   = 4,
    SAY_SAFETY                      = 5,
    SAY_DEATH                       = 6,
    SAY_KILLER                      = 7,
};

enum npcs
{
    NPC_REAPER_DEFIAS              = 47403,
    NPC_FRIENDLY_PROTOTYPE         = 49208,
    NPC_HARVEST_DUMMY              = 47468,
    NPC_SCORIES                    = 49229,
};

enum points
{
    POINT_HARVEST = 1,
};

enum Phases
{
    PHASE_INTRO = 2,
    PHASE_FIGHT,
};

class boss_foe_reaper_5000 : public CreatureScript
{
    struct boss_foe_reaper_5000AI : public BossAI
    {
        boss_foe_reaper_5000AI(Creature * creature) : BossAI(creature, BOSS_FOE_REAPER_5000_DATA) {}

        void Reset()
        {
            _reaperCount = 4;
            introCount = 0;
            enraged = false;
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            me->SetReactState(REACT_PASSIVE);
            me->CastSpell(me, SPELL_OFF_LINE, true);
            me->GetPosition(&_targetPos);
            _Reset();
            InitPrototypes();
            events.SetPhase(PHASE_INTRO);
        }

        void InitPrototypes()
        {
            me->SummonCreature(NPC_REAPER_DEFIAS, -182.74f, -565.97f, 19.31f, 3.35f);
            me->SummonCreature(NPC_REAPER_DEFIAS, -205.53f, -552.74f, 19.31f, 4.54f);
            me->SummonCreature(NPC_REAPER_DEFIAS, -228.67f, -565.75f, 19.31f, 5.98f);
            me->SummonCreature(NPC_REAPER_DEFIAS, -229.72f, -590.37f, 19.31f, 0.71f);
            me->SummonCreature(NPC_FRIENDLY_PROTOTYPE, -205.83f, -553.79f, 51.23f, 0.60f);
        }

        void SummonedCreatureDies(Creature* summon, Unit* /*killer*/)
        {
            if (summon->GetEntry() == NPC_REAPER_DEFIAS)
            {
                summon->CastSpell(me, SPELL_ENERGIZE, true);
                _reaperCount--;
                me->SummonCreature(NPC_FRIENDLY_PROTOTYPE, summon->GetPositionX(), summon->GetPositionY(), summon->GetPositionZ(), summon->GetOrientation());
                if (_reaperCount <= 0)
                {
                    events.ScheduleEvent(EVENT_INTRO, 1000);
                }
            }
        }

        void JustSummoned(Creature *summon)
        {
            if (summon->GetEntry() == NPC_HARVEST_DUMMY)
            {
                Talk(EMOTE_HARVEST);
                summon->CastSpell(summon, SPELL_HARVEST_READY, true);
                summon->GetPosition(&_targetPos);
                events.ScheduleEvent(EVENT_HARVEST_MOVEMENTS, 0);
            }
            else if (summon->GetEntry() == NPC_SCORIES)
            {
                summon->NearTeleportTo(summon->GetPositionX(), summon->GetPositionY(), 19.31f, summon->GetOrientation());
                summon->SetInCombatWithZone();
            }
            BossAI::JustSummoned(summon);
        }

        void EnterCombat(Unit * who)
        {
            events.SetPhase(PHASE_FIGHT);
            events.ScheduleEvent(EVENT_OVERDRIVE, 20000);
            events.ScheduleEvent(EVENT_HARVEST, 35000);
            if (IsHeroic())
                events.ScheduleEvent(EVENT_SCORIES, 15000);
            _EnterCombat();
        }

        void JustDied(Unit * /*killer*/)
        {
            std::list<Creature* > triggerList;
            me->GetCreatureListWithEntryInGrid(triggerList, NPC_FRIENDLY_PROTOTYPE, 100.0f);
            for (std::list<Creature* >::iterator itr = triggerList.begin(); itr != triggerList.end(); itr++)
                if (Creature *bot = *itr)
                    if (!bot->isAlive() || bot->HealthBelowPct(11))
                    {
                        instance->SetData(DATA_PRODIGY, 0);
                        break;
                    }
            Talk(SAY_DEATH);
            _JustDied();
        }

        void KilledUnit(Unit * victim)
        {
            Talk(SAY_KILLER);
        }

        void DoMeleeAttackIfReady()
        {
            if (me->getVictim())
                if (me->isAttackReady() && me->IsWithinMeleeRange(me->getVictim()))
                {
                    me->CastSpell(me->getVictim(), SPELL_REAPER_STRIKE, true);
                    me->resetAttackTimer();
                }
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type != POINT_MOTION_TYPE)
                return;
            if (id == POINT_HARVEST)
                events.ScheduleEvent(EVENT_HARVEST_SWEEP, 2000);
        }

        void DamageTaken(Unit* /*attacker*/, uint32& /*damage*/)
        {
            if (!enraged && me->HealthBelowPct(30))
            {
                enraged = true;
                Talk(SAY_SAFETY);
                DoCast(SPELL_SAFETY_RESTRICTIONS);
            }
        }

        void UpdateAI(uint32 const diff)
        {
            if (!UpdateVictim() && !events.IsInPhase(PHASE_INTRO))
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
            {
                if (me->HasAura(SPELL_OVERDRIVE))
                    me->ClearUnitState(UNIT_STATE_CASTING);
                if (me->HasAura(SPELL_HARVEST_2))
                    me->ClearUnitState(UNIT_STATE_CASTING);
                return;
            }

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch(eventId)
                {
                    case EVENT_INTRO:
                    {
                        std::list<Creature*> stalker;
                        me->GetCreatureListWithEntryInGrid(stalker, 45979, 10.0f);
                        for (std::list<Creature *>::iterator itr = stalker.begin(); itr != stalker.end(); itr++)
                            (*itr)->CastSpell(me, SPELL_ENERGIZE, true);
                        if (introCount++ < 3)
                            events.ScheduleEvent(EVENT_INTRO, 1000);
                        else
                            events.ScheduleEvent(EVENT_INTRO_FINAL, 1000);
                        break;
                    }
                    case EVENT_INTRO_FINAL:
                    {
                        Talk(SAY_AGGRO);
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        me->RemoveAura(SPELL_OFF_LINE);
                        me->SetReactState(REACT_DEFENSIVE);
                        break;
                    }
                    case EVENT_OVERDRIVE:
                        if (!me->HasAura(SPELL_HARVEST_2))
                        {
                            Talk(EMOTE_OVERDRIVE);
                            Talk(SAY_OVERDRIVE);
                            me->SetReactState(REACT_PASSIVE);
                            me->CastSpell(me, SPELL_OVERDRIVE);
                            events.ScheduleEvent(EVENT_OVERDRIVE_MOVEMENTS, 2000);
                            events.ScheduleEvent(EVENT_OVERDRIVE_END, 11000);
                            events.ScheduleEvent(EVENT_OVERDRIVE, 20000);
                        }
                        else
                            events.ScheduleEvent(EVENT_OVERDRIVE, 20000);
                        break;
                    case EVENT_OVERDRIVE_MOVEMENTS:
                    {
                        Position pos;
                        me->GetFirstCollisionPosition(pos, (float)urand(20, 50) * (float)rand_norm(), (float)rand_norm() * static_cast<float>(2 * M_PI));
                        me->GetMotionMaster()->MovePoint(0, pos);
                        if (me->HasAura(SPELL_OVERDRIVE))
                            events.ScheduleEvent(EVENT_OVERDRIVE_MOVEMENTS, 1000);
                        break;
                    }
                    case EVENT_OVERDRIVE_END:
                        events.CancelEvent(EVENT_OVERDRIVE_MOVEMENTS);
                        me->SetReactState(REACT_AGGRESSIVE);
                        if (Unit *victim = me->getVictim())
                            DoStartMovement(victim);
                        break;
                    case EVENT_HARVEST:
                        if (!me->HasAura(SPELL_OVERDRIVE))
                        {
                            Talk(SAY_HARVEST);
                            me->SetReactState(REACT_PASSIVE);
                            DoCastRandom(SPELL_HARVEST, 0.0f);
                            events.ScheduleEvent(EVENT_HARVEST, 35000);
                        }
                        else
                            events.ScheduleEvent(EVENT_HARVEST, 35000);
                        break;
                    case EVENT_HARVEST_MOVEMENTS:
                    {
                        me->GetMotionMaster()->MovePoint(POINT_HARVEST, _targetPos);
                        break;
                    }
                    case EVENT_HARVEST_SWEEP:
                    {
                        DoCast(SPELL_HARVEST_SWEEP);
                        me->RemoveAura(SPELL_HARVEST);
                        me->RemoveAura(SPELL_HARVEST_2);
                        me->SetReactState(REACT_AGGRESSIVE);
                        if (me->getVictim())
                            DoStartMovement(me->getVictim());
                        if (Creature *target = me->FindNearestCreature(NPC_HARVEST_DUMMY, 200))
                            target->DespawnOrUnsummon(1);
                        break;
                    }
                    case EVENT_SCORIES:
                    {
                        switch (rand() % 4)
                        {
                            case 0:
                                me->CastSpell(-205.56f, -572.14f, 20.98f, SPELL_SUMMON_SCORIES, true);
                                break;
                            case 1:
                                me->CastSpell(-199.27f, -579.92f, 20.98f, SPELL_SUMMON_SCORIES, true);
                                break;
                            case 2:
                                me->CastSpell(-206.17f, -586.03f, 20.98f, SPELL_SUMMON_SCORIES, true);
                                break;
                            case 3:
                                me->CastSpell(-212.79f, -578.70f, 20.98f, SPELL_SUMMON_SCORIES, true);
                                break;
                            default:
                                break;
                        }
                        events.ScheduleEvent(EVENT_SCORIES, 17000);
                    }
                }
            }

            DoMeleeAttackIfReady();
        }
    private:
        bool enraged;
        int8 _reaperCount, introCount;
        Position _targetPos;
    };
public:
    boss_foe_reaper_5000() : CreatureScript("boss_foe_reaper_5000") {}

    CreatureAI * GetAI(Creature * creature) const
    {
        return new boss_foe_reaper_5000AI(creature);
    }
};

class npc_scormies : public CreatureScript
{
public:
    npc_scormies() : CreatureScript("npc_scormies") { }

    struct npc_scormiesAI : public ScriptedAI
    {
        npc_scormiesAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
        }

        void Reset()
        {
            _fixateTarget = 0;
            me->CastSpell(me, SPELL_SCORIES_SHIELD, true);
            _events.Reset();
            _events.ScheduleEvent(EVENT_FIXATE, 30000);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            _events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_FIXATE:
                        if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, NonTankTargetSelector(me)))
                        {
                            me->CastSpell(target, SPELL_FIXATE, true);
                            _fixateTarget = target->GetGUID();
                        }
                        _events.ScheduleEvent(EVENT_FIXATE_CHECK, 1000);
                        _events.ScheduleEvent(EVENT_FIXATE, 30000);
                        break;
                    case EVENT_FIXATE_CHECK:
                        if (_fixateTarget != 0)
                        {
                            if (Unit *target = Unit::GetUnit(*me, _fixateTarget))
                            {
                                if (target->HasAura(SPELL_FIXATE) && me->getVictim() != target)
                                    me->AddThreat(target, 50000000.0f);
                                else if (!target->HasAura(SPELL_FIXATE))
                                {
                                    me->getThreatManager().addThreat(target, -90000000);
                                    _fixateTarget = 0;
                                }
                            }
                            else
                                _fixateTarget = 0;
                            if (_fixateTarget)
                                _events.ScheduleEvent(EVENT_FIXATE_CHECK, 1000);
                        }
                        break;
                    default:
                        break;
                }
            }
        }

    private :
        InstanceScript *instance;
        EventMap _events;
        uint64 _fixateTarget;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_scormiesAI (creature);
    }
};

// 16210
class achievement_prototype_prodigy : public AchievementCriteriaScript
{
public:
    achievement_prototype_prodigy() : AchievementCriteriaScript("achievement_prototype_prodigy") { }

    bool OnCheck(Player* /*source*/, Unit* target)
    {
        if (!target)
            return false;
        if (InstanceScript *instance = target->GetInstanceScript())
            return instance->instance->IsHeroic() && instance->GetData(DATA_PRODIGY);
        return false;
    }
};


void AddSC_boss_foe_reaper_5000()
{
    new boss_foe_reaper_5000();
    new npc_scormies();
    new achievement_prototype_prodigy();
}
