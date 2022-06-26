/*
 * This file is part of the FirelandsCore Project. See AUTHORS file for Copyright information
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

 /* ScriptData
 SDName: FollowerAI
 SD%Complete: 50
 SDComment: This AI is under development
 SDCategory: Npc
 EndScriptData */

#include "ScriptedFollowerAI.h"
#include "Creature.h"
#include "Group.h"
#include "Log.h"
#include "Map.h"
#include "MotionMaster.h"
#include "ObjectAccessor.h"
#include "Player.h"

float const MAX_PLAYER_DISTANCE = 100.0f;

enum Points
{
    POINT_COMBAT_START = 0xFFFFFF
};

FollowerAI::FollowerAI(Creature* creature) : ScriptedAI(creature), _updateFollowTimer(2500), _followState(STATE_FOLLOW_NONE), _questForFollow(0) { }

void FollowerAI::MoveInLineOfSight(Unit* who)
{
    if (HasFollowState(STATE_FOLLOW_INPROGRESS) && !ShouldAssistPlayerInCombatAgainst(who))
        return;

    ScriptedAI::MoveInLineOfSight(who);
}

void FollowerAI::JustDied(Unit* /*killer*/)
{
    if (!HasFollowState(STATE_FOLLOW_INPROGRESS) || !_leaderGUID || !_questForFollow)
        return;

    /// @todo need a better check for quests with time limit.
    if (Player* player = GetLeaderForFollower())
    {
        if (Group* group = player->GetGroup())
        {
            for (GroupReference* groupRef = group->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
                if (Player* member = groupRef->GetSource())
                    if (member->IsInMap(player))
                        member->FailQuest(_questForFollow);
        }
        else
            player->FailQuest(_questForFollow);
    }
}

void FollowerAI::JustReachedHome()
{
    if (!HasFollowState(STATE_FOLLOW_INPROGRESS))
        return;

    if (Player* player = GetLeaderForFollower())
    {
        if (HasFollowState(STATE_FOLLOW_PAUSED))
            return;
        me->FollowTarget(player);
    }
    else
        me->DespawnOrUnsummon();
}

void FollowerAI::OwnerAttackedBy(Unit* other)
{
    if (!me->HasReactState(REACT_PASSIVE) && ShouldAssistPlayerInCombatAgainst(other))
        AttackStart(other);
}

void FollowerAI::UpdateAI(uint32 uiDiff)
{
    if (HasFollowState(STATE_FOLLOW_INPROGRESS) && !me->GetVictim())
    {
        if (_updateFollowTimer <= uiDiff)
        {
            if (HasFollowState(STATE_FOLLOW_COMPLETE) && !HasFollowState(STATE_FOLLOW_POSTEVENT))
            {
                LOG_DEBUG("scripts", "FollowerAI is set completed, despawns.");
                me->DespawnOrUnsummon();
                return;
            }

            bool maxRangeExceeded = true;
            bool questAbandoned = (_questForFollow != 0);
            if (Player* player = GetLeaderForFollower())
            {
                if (Group* group = player->GetGroup())
                {
                    for (GroupReference* groupRef = group->GetFirstMember(); groupRef && (maxRangeExceeded || questAbandoned); groupRef = groupRef->next())
                    {
                        Player* member = groupRef->GetSource();
                        if (!member)
                            continue;
                        if (maxRangeExceeded && me->IsWithinDistInMap(member, MAX_PLAYER_DISTANCE))
                            maxRangeExceeded = false;
                        if (questAbandoned)
                        {
                            QuestStatus status = member->GetQuestStatus(_questForFollow);
                            if ((status == QUEST_STATUS_COMPLETE) || (status == QUEST_STATUS_INCOMPLETE))
                                questAbandoned = false;
                        }
                    }
                }
                else
                {
                    if (me->IsWithinDistInMap(player, MAX_PLAYER_DISTANCE))
                        maxRangeExceeded = false;
                    if (questAbandoned)
                    {
                        QuestStatus status = player->GetQuestStatus(_questForFollow);
                        if ((status == QUEST_STATUS_COMPLETE) || (status == QUEST_STATUS_INCOMPLETE))
                            questAbandoned = false;
                    }
                }
            }

            if (maxRangeExceeded || questAbandoned)
            {
                LOG_DEBUG("scripts.ai.followerai", "FollowerAI::UpdateAI: failed because player/group was to far away or not found (%s)", me->GetGUID().ToString().c_str());
                me->DespawnOrUnsummon();
                return;
            }

            _updateFollowTimer = 1000;
        }
        else
            _updateFollowTimer -= uiDiff;
    }

    UpdateFollowerAI(uiDiff);
}

void FollowerAI::UpdateFollowerAI(uint32 /*uiDiff*/)
{
    if (!UpdateVictim())
        return;

    DoMeleeAttackIfReady();
}

void FollowerAI::StartFollow(Player* player, uint32 factionForFollower, uint32 quest)
{
    if (Map* map = me->GetMap())
    {
        if (CreatureData const* cdata = me->GetCreatureData())
        {
            if (sWorld->getBoolConfig(CONFIG_RESPAWN_DYNAMIC_ESCORTNPC) && (cdata->spawnGroupData->flags & SPAWNGROUP_FLAG_ESCORTQUESTNPC))
                me->SaveRespawnTime(me->GetRespawnDelay());
        }
    }

    if (me->GetVictim())
    {
        LOG_DEBUG("scripts", "FollowerAI attempt to StartFollow while in combat.");
        return;
    }

    if (HasFollowState(STATE_FOLLOW_INPROGRESS))
    {
        LOG_ERROR("scripts", "FollowerAI attempt to StartFollow while already following.");
        return;
    }

    // set variables
    _leaderGUID = player->GetGUID();

    if (factionForFollower)
        me->SetFaction(factionForFollower);

    _questForFollow = quest;

    if (me->GetMotionMaster()->GetCurrentMovementGeneratorType() == WAYPOINT_MOTION_TYPE)
    {
        me->GetMotionMaster()->Clear();
        me->GetMotionMaster()->MoveIdle();
        LOG_DEBUG("scripts", "FollowerAI start with WAYPOINT_MOTION_TYPE, set to MoveIdle.");
    }

    me->SetUInt32Value(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_NONE);

    AddFollowState(STATE_FOLLOW_INPROGRESS);

    me->FollowTarget(player);

    LOG_DEBUG("scripts.ai.followerai", "FollowerAI::StartFollow: start follow %s - %s (%s)", player->GetName().c_str(), _leaderGUID.ToString().c_str(), me->GetGUID().ToString().c_str());
}

Player* FollowerAI::GetLeaderForFollower()
{
    if (Player* player = ObjectAccessor::GetPlayer(*me, _leaderGUID))
    {
        if (player->IsAlive())
            return player;
        else
        {
            if (Group* group = player->GetGroup())
            {
                for (GroupReference* groupRef = group->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
                {
                    Player* member = groupRef->GetSource();
                    if (member && me->IsWithinDistInMap(member, MAX_PLAYER_DISTANCE) && member->IsAlive())
                    {
                        LOG_DEBUG("scripts", "FollowerAI GetLeader changed and returned new leader.");
                        _leaderGUID = member->GetGUID();
                        return member;
                    }
                }
            }
        }
    }

    LOG_DEBUG("scripts", "FollowerAI GetLeader can not find suitable leader.");
    return nullptr;
}

void FollowerAI::SetFollowComplete(bool bWithEndEvent)
{
    if (me->HasUnitState(UNIT_STATE_FOLLOW))
    {
        me->ClearUnitState(UNIT_STATE_FOLLOW);

        me->StopMoving();
        me->GetMotionMaster()->Clear();
        me->GetMotionMaster()->MoveIdle();
    }

    if (bWithEndEvent)
        AddFollowState(STATE_FOLLOW_POSTEVENT);
    else
    {
        if (HasFollowState(STATE_FOLLOW_POSTEVENT))
            RemoveFollowState(STATE_FOLLOW_POSTEVENT);
    }

    AddFollowState(STATE_FOLLOW_COMPLETE);
}

void FollowerAI::SetFollowPaused(bool paused)
{
    if (!HasFollowState(STATE_FOLLOW_INPROGRESS) || HasFollowState(STATE_FOLLOW_COMPLETE))
        return;

    if (paused)
    {
        AddFollowState(STATE_FOLLOW_PAUSED);

        if (me->HasUnitState(UNIT_STATE_FOLLOW))
        {
            me->ClearUnitState(UNIT_STATE_FOLLOW);

            me->StopMoving();
            me->GetMotionMaster()->Clear();
            me->GetMotionMaster()->MoveIdle();
        }
    }
    else
    {
        RemoveFollowState(STATE_FOLLOW_PAUSED);

        if (Player* leader = GetLeaderForFollower())
            me->FollowTarget(leader);
    }
}

bool FollowerAI::ShouldAssistPlayerInCombatAgainst(Unit* who) const
{
    if (!who || !who->GetVictim())
        return false;

    // experimental (unknown) flag not present
    if (!(me->GetCreatureTemplate()->type_flags & CREATURE_TYPE_FLAG_CAN_ASSIST))
        return false;

    if (!who->isInAccessiblePlaceFor(me))
        return false;

    if (!CanAIAttack(who))
        return false;

    // we cannot attack in evade mode
    if (me->IsInEvadeMode())
        return false;

    // or if enemy is in evade mode
    if (who->GetTypeId() == TYPEID_UNIT && who->ToCreature()->IsInEvadeMode())
        return false;

    // never attack friendly
    if (me->IsFriendlyTo(who))
        return false;

    // too far away and no free sight
    if (!me->IsWithinDistInMap(who, MAX_PLAYER_DISTANCE) || !me->IsWithinLOSInMap(who))
        return false;

    return true;
}
