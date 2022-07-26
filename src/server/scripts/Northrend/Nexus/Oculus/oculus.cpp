/*
 * Copyright (C) 2022 Firelands Project <https://github.com/FirelandsProject>
 * Copyright (C) 2008-2013 TrinityCore <http://www.trinitycore.org/> 
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

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "Vehicle.h"
#include "CombatAI.h"
#include "oculus.h"
#include "Player.h"

#define GOSSIP_ITEM_DRAKES         "So where do we go from here?"
#define GOSSIP_ITEM_BELGARISTRASZ1 "I want to fly on the wings of the Red Flight"
#define GOSSIP_ITEM_BELGARISTRASZ2 "What abilities do Ruby Drakes have?"
#define GOSSIP_ITEM_VERDISA1       "I want to fly on the wings of the Green Flight"
#define GOSSIP_ITEM_VERDISA2       "What abilities do Emerald Drakes have?"
#define GOSSIP_ITEM_ETERNOS1       "I want to fly on the wings of the Bronze Flight"
#define GOSSIP_ITEM_ETERNOS2       "What abilities do Amber Drakes have?"

#define HAS_ESSENCE(a) ((a)->HasItemCount(ITEM_EMERALD_ESSENCE) || (a)->HasItemCount(ITEM_AMBER_ESSENCE) || (a)->HasItemCount(ITEM_RUBY_ESSENCE))

enum GossipNPCs
{
    GOSSIP_TEXTID_DRAKES                          = 13267,
    GOSSIP_TEXTID_BELGARISTRASZ1                  = 12916,
    GOSSIP_TEXTID_BELGARISTRASZ2                  = 13466,
    GOSSIP_TEXTID_BELGARISTRASZ3                  = 13254,
    GOSSIP_TEXTID_VERDISA1                        = 1,
    GOSSIP_TEXTID_VERDISA2                        = 1,
    GOSSIP_TEXTID_VERDISA3                        = 1,
    GOSSIP_TEXTID_ETERNOS1                        = 1,
    GOSSIP_TEXTID_ETERNOS2                        = 1,
    GOSSIP_TEXTID_ETERNOS3                        = 13256,

    ITEM_EMERALD_ESSENCE                          = 37815,
    ITEM_AMBER_ESSENCE                            = 37859,
    ITEM_RUBY_ESSENCE                             = 37860,

    SPELL_SHOCK_CHARGE                            = 49836
};

enum Drakes
{
/*Ruby Drake,
(npc 27756) (item 37860)
(summoned by spell Ruby Essence = 37860 ---> Call Amber Drake == 49462 ---> Summon 27756)
*/
    SPELL_RIDE_RUBY_DRAKE_QUE                     = 49463,          //Apply Aura: Periodic Trigger, Interval: 3 seconds ---> 49464
    SPELL_RUBY_DRAKE_SADDLE                       = 49464,          //Allows you to ride on the back of an Amber Drake. ---> Dummy
    SPELL_RUBY_SEARING_WRATH                      = 50232,          //(60 yds) - Instant - Breathes a stream of fire at an enemy dragon, dealing 6800 to 9200 Fire damage and then jumping to additional dragons within 30 yards. Each jump increases the damage by 50%. Affects up to 5 total targets
    SPELL_RUBY_EVASIVE_AURA                       = 50248,          //Instant - Allows the Ruby Drake to generate Evasive Charges when hit by hostile attacks and spells.
    SPELL_RUBY_EVASIVE_MANEUVERS                  = 50240,          //Instant - 5 sec. cooldown - Allows your drake to dodge all incoming attacks and spells. Requires Evasive Charges to use. Each attack or spell dodged while this ability is active burns one Evasive Charge. Lasts 30 sec. or until all charges are exhausted.
    //you do not have acces to until you kill Mage-Lord Urom
    SPELL_RUBY_MARTYR                             = 50253,          //Instant - 10 sec. cooldown - Redirect all harmful spells cast at friendly drakes to yourself for 10 sec.

/*Amber Drake,
(npc 27755)  (item 37859)
(summoned by spell Amber Essence = 37859 ---> Call Amber Drake == 49461 ---> Summon 27755)
*/

    SPELL_RIDE_AMBER_DRAKE_QUE                    = 49459,          //Apply Aura: Periodic Trigger, Interval: 3 seconds ---> 49460
    SPELL_AMBER_DRAKE_SADDLE                      = 49460,          //Allows you to ride on the back of an Amber Drake. ---> Dummy
    SPELL_AMBER_SHOCK_LANCE                       = 49840,          //(60 yds) - Instant - Deals 4822 to 5602 Arcane damage and detonates all Shock Charges on an enemy dragon. Damage is increased by 6525 for each detonated.
    // SPELL_AMBER_STOP_TIME                                        //Instant - 1 min cooldown - Halts the passage of time, freezing all enemy dragons in place for 10 sec. This attack applies 5 Shock Charges to each affected target.
    //you do not have access to until you kill the  Mage-Lord Urom.
    SPELL_AMBER_TEMPORAL_RIFT                     = 49592,          //(60 yds) - Channeled - Channels a temporal rift on an enemy dragon for 10 sec. While trapped in the rift, all damage done to the target is increased by 100%. In addition, for every 15, 000 damage done to a target affected by Temporal Rift, 1 Shock Charge is generated.

/*Emerald Drake,
(npc 27692)  (item 37815),
 (summoned by spell Emerald Essence = 37815 ---> Call Emerald Drake == 49345 ---> Summon 27692)
*/
    SPELL_RIDE_EMERALD_DRAKE_QUE                  = 49427,         //Apply Aura: Periodic Trigger, Interval: 3 seconds ---> 49346
    SPELL_EMERALD_DRAKE_SADDLE                    = 49346,         //Allows you to ride on the back of an Amber Drake. ---> Dummy
    SPELL_EMERALD_LEECHING_POISON                 = 50328,         //(60 yds) - Instant - Poisons the enemy dragon, leeching 1300 to the caster every 2 sec. for 12 sec. Stacks up to 3 times.
    SPELL_EMERALD_TOUCH_THE_NIGHTMARE             = 50341,         //(60 yds) - Instant - Consumes 30% of the caster's max health to inflict 25, 000 nature damage to an enemy dragon and reduce the damage it deals by 25% for 30 sec.
    // you do not have access to until you kill the Mage-Lord Urom
    SPELL_EMERALD_DREAM_FUNNEL                    = 50344,         //(60 yds) - Channeled - Transfers 5% of the caster's max health to a friendly drake every second for 10 seconds as long as the caster channels.
};

enum Says
{
    SAY_VAROS                         = 0,
    SAY_UROM                          = 1,
    SAY_BELGARISTRASZ                 = 0,
    SAY_DRAKES_TAKEOFF                = 0,
    WHISPER_DRAKES_WELCOME            = 1,
    WHISPER_DRAKES_ABILITIES          = 2,
    WHISPER_DRAKES_SPECIAL            = 3,
    WHISPER_DRAKES_LOWHEALTH          = 4
};

class npc_verdisa_beglaristrasz_eternos : public CreatureScript
{
public:
    npc_verdisa_beglaristrasz_eternos() : CreatureScript("npc_verdisa_beglaristrasz_eternos") { }

        InstanceScript* instance;

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
    {
        player->PlayerTalkClass->ClearMenus();
        switch (creature->GetEntry())
        {
        case NPC_VERDISA: //Verdisa
            switch (action)
            {
            case GOSSIP_ACTION_INFO_DEF + 1:
                if (!HAS_ESSENCE(player))
                {
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_VERDISA1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_VERDISA2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
                    player->SEND_GOSSIP_MENU(GOSSIP_TEXTID_VERDISA1, creature->GetGUID());
                }
                else
                {
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_VERDISA2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
                    player->SEND_GOSSIP_MENU(GOSSIP_TEXTID_VERDISA2, creature->GetGUID());
                }
                break;
            case GOSSIP_ACTION_INFO_DEF + 2:
            {
                ItemPosCountVec dest;
                uint8 msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, ITEM_EMERALD_ESSENCE, 1);
                if (msg == EQUIP_ERR_OK)
                    player->StoreNewItem(dest, ITEM_EMERALD_ESSENCE, true);
                player->CLOSE_GOSSIP_MENU();
                break;
            }
            case GOSSIP_ACTION_INFO_DEF + 3:
                player->SEND_GOSSIP_MENU(GOSSIP_TEXTID_VERDISA3, creature->GetGUID());
                break;
            }
            break;
        case NPC_BELGARISTRASZ: //Belgaristrasz
            switch (action)
            {
            case GOSSIP_ACTION_INFO_DEF + 1:
                if (!HAS_ESSENCE(player))
                {
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_BELGARISTRASZ1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_BELGARISTRASZ2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
                    player->SEND_GOSSIP_MENU(GOSSIP_TEXTID_BELGARISTRASZ1, creature->GetGUID());
                }
                else
                {
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_BELGARISTRASZ2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
                    player->SEND_GOSSIP_MENU(GOSSIP_TEXTID_BELGARISTRASZ2, creature->GetGUID());
                }
                break;
            case GOSSIP_ACTION_INFO_DEF + 2:
            {
                ItemPosCountVec dest;
                uint8 msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, ITEM_RUBY_ESSENCE, 1);
                if (msg == EQUIP_ERR_OK)
                    player->StoreNewItem(dest, ITEM_RUBY_ESSENCE, true);
                player->CLOSE_GOSSIP_MENU();
                break;
            }
            case GOSSIP_ACTION_INFO_DEF + 3:
                player->SEND_GOSSIP_MENU(GOSSIP_TEXTID_BELGARISTRASZ3, creature->GetGUID());
                break;
            }
            break;
        case NPC_ETERNOS: //Eternos
            switch (action)
            {
            case GOSSIP_ACTION_INFO_DEF + 1:
                if (!HAS_ESSENCE(player))
                {
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ETERNOS1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ETERNOS2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
                    player->SEND_GOSSIP_MENU(GOSSIP_TEXTID_ETERNOS1, creature->GetGUID());
                }
                else
                {
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ETERNOS2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
                    player->SEND_GOSSIP_MENU(GOSSIP_TEXTID_ETERNOS2, creature->GetGUID());
                }
                break;
            case GOSSIP_ACTION_INFO_DEF + 2:
            {
                ItemPosCountVec dest;
                uint8 msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, ITEM_AMBER_ESSENCE, 1);
                if (msg == EQUIP_ERR_OK)
                    player->StoreNewItem(dest, ITEM_AMBER_ESSENCE, true);
                player->CLOSE_GOSSIP_MENU();
                break;
            }
            case GOSSIP_ACTION_INFO_DEF + 3:
                player->SEND_GOSSIP_MENU(GOSSIP_TEXTID_ETERNOS3, creature->GetGUID());
                break;
            }
            break;
        }

        return true;
    }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        if (creature->isQuestGiver())
            player->PrepareQuestMenu(creature->GetGUID());

        if (InstanceScript* instance = creature->GetInstanceScript())
        {
            if (instance->IsDone(DATA_DRAKOS_EVENT))
            {
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_DRAKES, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
                player->SEND_GOSSIP_MENU(GOSSIP_TEXTID_DRAKES, creature->GetGUID());
            }
        }

        return true;
    }

    struct npc_verdisa_beglaristrasz_eternosAI : public ScriptedAI
    {
        npc_verdisa_beglaristrasz_eternosAI(Creature* creature) : ScriptedAI(creature) { }

        void MovementInform(uint32 /*type*/, uint32 id)
        {
            // When Belgaristraz finish his moving say grateful text
            if (me->GetEntry() == NPC_BELGARISTRASZ)
                if (id == 0)
                {
                    Talk(SAY_BELGARISTRASZ);
                }
            // The gossip flag should activate when Drakos die and not from DB
            if (id == 0)
                me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_verdisa_beglaristrasz_eternosAI(creature);
    }
};

class npc_image_belgaristrasz : public CreatureScript
{
public:
    npc_image_belgaristrasz() : CreatureScript("npc_image_belgaristrasz") { }

    struct npc_image_belgaristraszAI : public ScriptedAI
    {
        npc_image_belgaristraszAI(Creature* creature) : ScriptedAI(creature) { }

        void IsSummonedBy(Unit* summoner)
        {
            if (summoner->GetEntry() == NPC_VAROS)
            {
               Talk(SAY_VAROS);
               me->DespawnOrUnsummon(60000);
            }
            if (summoner->GetEntry() == NPC_UROM)
            {
               Talk(SAY_UROM);
               me->DespawnOrUnsummon(60000);
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_image_belgaristraszAI(creature);
    }
};

class npc_ruby_emerald_amber_drake : public CreatureScript
{
public:
    npc_ruby_emerald_amber_drake() : CreatureScript("npc_ruby_emerald_amber_drake") { }

    struct npc_ruby_emerald_amber_drakeAI : public VehicleAI
    {
        npc_ruby_emerald_amber_drakeAI(Creature* creature) : VehicleAI(creature)
        {
            instance = creature->GetInstanceScript();
        }

            InstanceScript* instance;

            uint64 summonerGUID;
            uint32 WelcomeTimer;
            uint32 WelcomeSequelTimer;
            uint32 SpecialTimer;
            uint32 WarningTimer;
            uint32 TakeOffTimer;

            bool WelcomeOff;
            bool WelcomeSequelOff;
            bool SpecialOff;
            bool HealthWarningOff;
            bool DisableTakeOff;

        void Reset()
        {
            summonerGUID = 0;
            WelcomeTimer = 4500;
            WelcomeSequelTimer = 4500;
            SpecialTimer = 10000;
            WarningTimer = 25000;
            TakeOffTimer = 3500;

            WelcomeOff = false;
            WelcomeSequelOff = false;
            SpecialOff = false;
            HealthWarningOff = false;
            DisableTakeOff = false;
        }

        void IsSummonedBy(Unit* summoner)
        {
            if (instance && instance->GetBossState(DATA_EREGOS_EVENT) == IN_PROGRESS)
                if (Creature* eregos = me->FindNearestCreature(NPC_EREGOS, 450.0f, true))
                {
                    eregos->DespawnOrUnsummon(); // On retail this kills abusive call of drake during engaged Eregos
                }
            summonerGUID = summoner->GetGUID();
            me->SetFacingToObject(summoner);
            // TO DO: Drake Ques should be casted from vehicle to player, however the way core handle triggered spells from auras break it no matter the conditions. So this change the caster and give the same result until someone fix triggered spells from auras that involve implicit targets or make exception for this case.
            if (me->GetEntry() == NPC_RUBY_DRAKE_VEHICLE)
                summoner->CastSpell(summoner, SPELL_RIDE_RUBY_DRAKE_QUE);
            if (me->GetEntry() == NPC_EMERALD_DRAKE_VEHICLE)
                summoner->CastSpell(summoner, SPELL_RIDE_EMERALD_DRAKE_QUE);
            if (me->GetEntry() == NPC_AMBER_DRAKE_VEHICLE)
                summoner->CastSpell(summoner, SPELL_RIDE_AMBER_DRAKE_QUE);
            Position pos;
            summoner->GetPosition(&pos);
            me->GetMotionMaster()->MovePoint(0, pos);
            me->SetSpeed(MOVE_FLIGHT, 4.0f, true);
        }

        void PassengerBoarded(Unit* unit, int8 /*seat*/, bool apply)
        {
            if (apply)
            {
                unit->SetSpeed(MOVE_FLIGHT, 4.0f, true);
                me->SetSpeed(MOVE_FLIGHT, 4.0f, true);
                me->UpdateSpeed(MOVE_FLIGHT, true);
                unit->UpdateSpeed(MOVE_FLIGHT, true);
            }
            else
            {
                unit->SetSpeed(MOVE_FLIGHT, 1.0f, true);
            }
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type == POINT_MOTION_TYPE && id == 0)
            {
                me->SetDisableGravity(false); // Needed this for proper animation after spawn, the summon in air fall to ground bug leave no other option for now, if this isn't used the drake will only walk on move.
            }
        }

        void UpdateAI(const uint32 diff)
        {
            if (instance && !(instance->IsDone(DATA_VAROS_EVENT)))
            {
                if (me->HasAuraType(SPELL_AURA_CONTROL_VEHICLE))
                {
                    if (!(WelcomeOff))
                    {
                        if (WelcomeTimer <= diff)
                        {
                            Talk(WHISPER_DRAKES_WELCOME, me->GetCreatorGUID());
                            WelcomeOff = true;
                            WelcomeSequelOff = true;
                        }
                        else WelcomeTimer -= diff;
                    }
                }
            }
            if (me->HasAuraType(SPELL_AURA_CONTROL_VEHICLE))
            {
                if (WelcomeSequelOff)
                {
                    if (WelcomeSequelTimer <= diff)
                    {
                        Talk(WHISPER_DRAKES_ABILITIES, me->GetCreatorGUID());
                        WelcomeSequelOff = false;
                    }
                    else WelcomeSequelTimer -= diff;
                }
            }
            if (me->HasAuraType(SPELL_AURA_CONTROL_VEHICLE))
            {
                if (instance && instance->IsDone(DATA_UROM_EVENT))
                {
                    if (!(SpecialOff))
                    {
                        if (SpecialTimer <= diff)
                        {
                            Talk(WHISPER_DRAKES_SPECIAL, me->GetCreatorGUID());
                            SpecialOff = true;
                        }
                        else SpecialTimer -= diff;
                    }
                }
            }
            if (me->HasAuraType(SPELL_AURA_CONTROL_VEHICLE))
            {
                if (!(HealthWarningOff))
                {
                    if (me->GetHealthPct() <= 40.0f)
                    {
                        Talk(WHISPER_DRAKES_LOWHEALTH, me->GetCreatorGUID());
                        HealthWarningOff = true;
                    }
                }
            }
            if (me->HasAuraType(SPELL_AURA_CONTROL_VEHICLE))
            {
                if (HealthWarningOff)
                {
                    if (WarningTimer <= diff)
                    {
                        HealthWarningOff = false;
                        WarningTimer = 25000;
                    }
                    else WarningTimer -= diff;
                }
            }
            if (!(me->HasAuraType(SPELL_AURA_CONTROL_VEHICLE)))
            {
                if (!(DisableTakeOff))
                {
                    if (TakeOffTimer <= diff)
                    {
                        me->DespawnOrUnsummon(2050);
                        me->SetOrientation(2.5f);
                        me->SetSpeed(MOVE_FLIGHT, 1.0f, true);
                        Talk(SAY_DRAKES_TAKEOFF);
                        Position pos;
                        me->GetPosition(&pos);
                        pos.m_positionX += 10.0f;
                        pos.m_positionY += 10.0f;
                        pos.m_positionZ += 12.0f;
                        me->GetMotionMaster()->MovePoint(1, pos);
                        DisableTakeOff = true;
                    }
                    else TakeOffTimer -= diff;
                 }
            }
        };
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_ruby_emerald_amber_drakeAI(creature);
    }
};

class npc_oculus_ringlord_conjurer : public CreatureScript
{
public:
    npc_oculus_ringlord_conjurer() : CreatureScript("npc_oculus_ringlord_conjurer") { }

    struct npc_oculus_ringlord_conjurerAI : public ScriptedAI
    {
        npc_oculus_ringlord_conjurerAI(Creature* c) : ScriptedAI(c) { }

        void EnterCombat(Unit* /*who*/)
        {
            me->AddAura(DUNGEON_MODE(50717, 59276), me);
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_oculus_ringlord_conjurerAI(creature);
    }
};

class npc_oculus_ringlord_sorceress : public CreatureScript
{
public:
    npc_oculus_ringlord_sorceress() : CreatureScript("npc_oculus_ringlord_sorceress") { }

    struct npc_oculus_ringlord_sorceressAI : public ScriptedAI
    {
        npc_oculus_ringlord_sorceressAI(Creature* c) : ScriptedAI(c) { }

        void Reset()
        {
            _blizzardTimer = 6000;
            _flameStrikeTimer = 3000;
        }

        void UpdateAI(uint32 const diff)
        {
            if (!UpdateVictim())
                return;

            if (_blizzardTimer <= diff)
            {
                if (!me->IsNonMeleeSpellCasted(false))
                {
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 30, true))
                        DoCast(target, DUNGEON_MODE(50715, 59278));
                    _blizzardTimer = urand(12000, 17000);
                }
                else
                    _blizzardTimer = urand(5000, 10000);
            }
            else
                _blizzardTimer -= diff;

            if (_flameStrikeTimer <= diff)
            {
                if (!me->IsNonMeleeSpellCasted(false))
                {
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 30, true))
                        DoCast(target, DUNGEON_MODE(16102, 61402));
                    _flameStrikeTimer = urand(6000, 13000);
                }
                else
                    _flameStrikeTimer = urand(4000, 8000);
            }
            else
                _flameStrikeTimer -= diff;

            DoMeleeAttackIfReady();
        }

    private:
        uint32 _blizzardTimer;
        uint32 _flameStrikeTimer;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_oculus_ringlord_sorceressAI(creature);
    }
};

class spell_amber_drake_schock_lance : public SpellScriptLoader
{
public:
    spell_amber_drake_schock_lance() : SpellScriptLoader("spell_amber_drake_schock_lance") { }

    class spell_amber_drake_schock_lance_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_amber_drake_schock_lance_SpellScript);

        void RecalculateDamage()
        {
            if (Aura* charge = GetHitUnit()->GetAura(SPELL_SHOCK_CHARGE))
            {
                SetHitDamage(6525 * charge->GetStackAmount() + GetHitDamage());
                charge->Remove();
            }
        }

        void Register()
        {
            OnHit += SpellHitFn(spell_amber_drake_schock_lance_SpellScript::RecalculateDamage);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_amber_drake_schock_lance_SpellScript();
    }
};

class IsNoValidDrake
{
public:
    bool operator() (WorldObject* unit)
    {
        if (unit->ToCreature())
        {
            switch (unit->ToCreature()->GetEntry())
            {
            case 27638: // Azure Ring Guardian
            case 27656: // Ley-Guardian Eregos
            case 28276: // Greater Ley-Whelp
                return false;
            default:
                break;
            }
        }
        return true;
    }
};

class spell_amber_drake_stop_time : public SpellScriptLoader
{
public:
    spell_amber_drake_stop_time() : SpellScriptLoader("spell_amber_drake_stop_time") { }

    class spell_amber_drake_stop_time_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_amber_drake_stop_time_SpellScript);

        void FilterTargets(std::list<WorldObject*>& unitList)
        {
            unitList.remove_if(IsNoValidDrake());
        }

        void HandleStun(SpellEffIndex /*effIndex*/)
        {
            if (GetHitUnit())
                GetHitUnit()->CastCustomSpell(SPELL_SHOCK_CHARGE, SPELLVALUE_AURA_STACK, 5, GetHitUnit(), true);
        }

        void Register()
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_amber_drake_stop_time_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            OnEffectHitTarget += SpellEffectFn(spell_amber_drake_stop_time_SpellScript::HandleStun, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_amber_drake_stop_time_SpellScript();
    }
};

class spell_amber_drake_temporal_rift : public SpellScriptLoader
{
public:
    spell_amber_drake_temporal_rift() : SpellScriptLoader("spell_amber_drake_temporal_rift") { }

    class spell_amber_drake_temporal_rift_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_amber_drake_temporal_rift_AuraScript);

        void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (GetTarget())
                _targetHealth = GetTarget()->GetHealth();

            _damage = 0;
        }

        void HandlePeriodicTick(AuraEffect const* /*aurEff*/)
        {
            _damage += _targetHealth - GetTarget()->GetHealth();

            for (; _damage >= 15000; _damage -= 15000)
                GetTarget()->CastSpell(GetTarget(), SPELL_SHOCK_CHARGE, true);

            _targetHealth = GetTarget()->GetHealth();
        }

        void Register()
        {
            OnEffectApply += AuraEffectApplyFn(spell_amber_drake_temporal_rift_AuraScript::OnApply, EFFECT_2, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_amber_drake_temporal_rift_AuraScript::HandlePeriodicTick, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
        }

        uint32 _targetHealth;
        uint32 _damage;
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_amber_drake_temporal_rift_AuraScript();
    }
};

class spell_gen_stop_time : public SpellScriptLoader
{
public:
    spell_gen_stop_time() : SpellScriptLoader("spell_gen_stop_time") { }

    class spell_gen_stop_time_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_gen_stop_time_AuraScript);

        void Apply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;
            Unit* target = GetTarget();
            for (uint32 i = 0; i < 5; ++i)
                caster->CastSpell(target, SPELL_SHOCK_CHARGE, false);
        }

        void Register()
        {
            AfterEffectApply += AuraEffectApplyFn(spell_gen_stop_time_AuraScript::Apply, EFFECT_0, SPELL_AURA_MOD_STUN, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_gen_stop_time_AuraScript();
    }
};

class spell_call_ruby_emerald_amber_drake : public SpellScriptLoader
{
public:
    spell_call_ruby_emerald_amber_drake() : SpellScriptLoader("spell_call_ruby_emerald_amber_drake") { }

    class spell_call_ruby_emerald_amber_drake_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_call_ruby_emerald_amber_drake_SpellScript);

        void ChangeSummonPos(SpellEffIndex /*effIndex*/)
        {
            // Adjust effect summon position
            WorldLocation summonPos = *GetExplTargetDest();
            Position offset = {0.0f, 0.0f, 12.0f, 0.0f};
            summonPos.RelocateOffset(offset);
            SetExplTargetDest(summonPos);
            GetHitDest()->RelocateOffset(offset);
        }

        void ModDestHeight(SpellEffIndex /*effIndex*/)
        {
            // Used to cast visual effect at proper position
            Position offset = {0.0f, 0.0f, 12.0f, 0.0f};
            const_cast<WorldLocation*>(GetExplTargetDest())->RelocateOffset(offset);
        }

        void Register()
        {
            OnEffectHit += SpellEffectFn(spell_call_ruby_emerald_amber_drake_SpellScript::ChangeSummonPos, EFFECT_0, SPELL_EFFECT_SUMMON);
            OnEffectLaunch += SpellEffectFn(spell_call_ruby_emerald_amber_drake_SpellScript::ModDestHeight, EFFECT_0, SPELL_EFFECT_SUMMON);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_call_ruby_emerald_amber_drake_SpellScript();
    }
};



void AddSC_oculus()
{
    new npc_verdisa_beglaristrasz_eternos();
    new npc_image_belgaristrasz();
    new npc_ruby_emerald_amber_drake();
    new npc_oculus_ringlord_conjurer();
    new npc_oculus_ringlord_sorceress();
    new spell_amber_drake_schock_lance();
    new spell_amber_drake_stop_time();
    new spell_amber_drake_temporal_rift();
    new spell_gen_stop_time();
    new spell_call_ruby_emerald_amber_drake();
}