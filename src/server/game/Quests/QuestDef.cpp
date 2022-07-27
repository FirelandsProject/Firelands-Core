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

#include "QuestDef.h"
#include "GameEventMgr.h"
#include "Player.h"
#include "World.h"
#include "ObjectMgr.h"

Quest::Quest(Field* questRecord)
{
    Id = questRecord[0].Get<uint32>();
    Method = questRecord[1].Get<uint8>();
    Level = questRecord[2].Get<int16>();
    MinLevel = uint32(questRecord[3].Get<int16>());
    MaxLevel = uint32(questRecord[4].Get<int16>());
    ZoneOrSort = questRecord[5].Get<int16>();
    Type = questRecord[6].Get<uint16>();
    SuggestedPlayers = questRecord[7].Get<uint8>();
    LimitTime = questRecord[8].Get<uint32>();
    RequiredClasses = questRecord[9].Get<uint16>();
    RequiredRaces = questRecord[10].Get<uint32>();
    RequiredSkillId = questRecord[11].Get<uint16>();
    RequiredSkillPoints = questRecord[12].Get<uint16>();
    RequiredFactionId1 = questRecord[13].Get<uint16>();
    RequiredFactionId2 = questRecord[14].Get<uint16>();
    RequiredFactionValue1 = questRecord[15].Get<int32>();
    RequiredFactionValue2 = questRecord[16].Get<int32>();
    RequiredMinRepFaction = questRecord[17].Get<uint16>();
    RequiredMaxRepFaction = questRecord[18].Get<uint16>();
    RequiredMinRepValue = questRecord[19].Get<int32>();
    RequiredMaxRepValue = questRecord[20].Get<int32>();
    PrevQuestId = questRecord[21].Get<int32>();
    NextQuestId = questRecord[22].Get<int32>();
    ExclusiveGroup = questRecord[23].Get<int32>();
    NextQuestIdChain = questRecord[24].Get<uint32>();
    RewardXPId = questRecord[25].Get<uint8>();
    RewardOrRequiredMoney = questRecord[26].Get<int32>();
    RewardMoneyMaxLevel = questRecord[27].Get<uint32>();
    RewardSpell = questRecord[28].Get<uint32>();
    RewardSpellCast = questRecord[29].Get<int32>();
    RewardHonor = questRecord[30].Get<uint32>();
    RewardHonorMultiplier = questRecord[31].Get<float>();
    RewardMailTemplateId = questRecord[32].Get<uint32>();
    RewardMailDelay = questRecord[33].Get<uint32>();
    SourceItemId = questRecord[34].Get<uint32>();
    SourceItemIdCount = questRecord[35].Get<uint8>();
    SourceSpellid = questRecord[36].Get<uint32>();
    Flags = questRecord[37].Get<uint32>();
    SpecialFlags = questRecord[38].Get<uint8>();
    MinimapTargetMark = questRecord[39].Get<uint8>();
    RewardTitleId = questRecord[40].Get<uint32>();
    RequiredPlayerKills = questRecord[41].Get<uint8>();
    RewardTalents = questRecord[42].Get<uint8>();
    RewardArenaPoints = questRecord[43].Get<uint16>();
    RewardSkillId = questRecord[44].Get<uint16>();
    RewardSkillPoints = questRecord[45].Get<uint8>();
    RewardReputationMask = questRecord[46].Get<uint8>();
    QuestGiverPortrait = questRecord[47].Get<uint32>();
    QuestTurnInPortrait = questRecord[48].Get<uint32>();

    for (int i = 0; i < QUEST_REWARDS_COUNT; ++i)
        RewardItemId[i] = questRecord[49 + i].Get<uint32>();

    for (int i = 0; i < QUEST_REWARDS_COUNT; ++i)
        RewardItemIdCount[i] = questRecord[53 + i].Get<uint16>();

    for (int i = 0; i < QUEST_REWARD_CHOICES_COUNT; ++i)
        RewardChoiceItemId[i] = questRecord[57 + i].Get<uint32>();

    for (int i = 0; i < QUEST_REWARD_CHOICES_COUNT; ++i)
        RewardChoiceItemCount[i] = questRecord[63 + i].Get<uint16>();

    for (int i = 0; i < QUEST_REPUTATIONS_COUNT; ++i)
        RewardFactionId[i] = questRecord[69 + i].Get<uint16>();

    for (int i = 0; i < QUEST_REPUTATIONS_COUNT; ++i)
        RewardFactionValueId[i] = questRecord[74 + i].Get<int32>();

    for (int i = 0; i < QUEST_REPUTATIONS_COUNT; ++i)
        RewardFactionValueIdOverride[i] = questRecord[79 + i].Get<int32>();

    PointMapId = questRecord[84].Get<uint16>();
    PointX = questRecord[85].Get<float>();
    PointY = questRecord[86].Get<float>();
    PointOption = questRecord[87].Get<uint32>();
    Title = questRecord[88].Get<std::string>();
    Objectives = questRecord[89].Get<std::string>();
    Details = questRecord[90].Get<std::string>();
    EndText = questRecord[91].Get<std::string>();
    CompletedText = questRecord[92].Get<std::string>();
    OfferRewardText = questRecord[93].Get<std::string>();
    RequestItemsText = questRecord[94].Get<std::string>();

    for (int i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
        RequiredNpcOrGo[i] = questRecord[95 + i].Get<int32>();

    for (int i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
        RequiredNpcOrGoCount[i] = questRecord[99 + i].Get<uint16>();

    for (int i = 0; i < QUEST_SOURCE_ITEM_IDS_COUNT; ++i)
        RequiredSourceItemId[i] = questRecord[103 + i].Get<uint32>();

    for (int i = 0; i < QUEST_SOURCE_ITEM_IDS_COUNT; ++i)
        RequiredSourceItemCount[i] = questRecord[107 + i].Get<uint16>();

    for (int i = 0; i < QUEST_ITEM_OBJECTIVES_COUNT; ++i)
        RequiredItemId[i] = questRecord[111 + i].Get<uint32>();

    for (int i = 0; i < QUEST_ITEM_OBJECTIVES_COUNT; ++i)
        RequiredItemCount[i] = questRecord[117 + i].Get<uint16>();

    RequiredSpell = questRecord[123].Get<uint32>();

    for (int i = 0; i < QUEST_OBJECTIVES_COUNT; ++i) // To be removed
        RequiredSpellCast[i] = questRecord[124 + i].Get<uint32>();

    for (int i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
        ObjectiveText[i] = questRecord[128 + i].Get<std::string>();

    for (int i = 0; i < QUEST_REWARD_CURRENCY_COUNT; ++i)
        RewardCurrencyId[i] = questRecord[132 + i].Get<uint16>();

    for (int i = 0; i < QUEST_REWARD_CURRENCY_COUNT; ++i)
        RewardCurrencyCount[i] = questRecord[136 + i].Get<uint32>();

    for (int i = 0; i < QUEST_REQUIRED_CURRENCY_COUNT; ++i)
        RequiredCurrencyId[i] = questRecord[140 + i].Get<uint16>();

    for (int i = 0; i < QUEST_REQUIRED_CURRENCY_COUNT; ++i)
        RequiredCurrencyCount[i] = questRecord[144 + i].Get<uint32>();

    QuestGiverTextWindow = questRecord[148].Get<std::string>();
    QuestGiverTargetName = questRecord[149].Get<std::string>();
    QuestTurnTextWindow = questRecord[150].Get<std::string>();
    QuestTurnTargetName = questRecord[151].Get<std::string>();
    SoundAccept = questRecord[152].Get<uint16>();
    SoundTurnIn = questRecord[153].Get<uint16>();

    for (int i = 0; i < QUEST_EMOTE_COUNT; ++i)
        DetailsEmote[i] = questRecord[154 + i].Get<uint16>();

    for (int i = 0; i < QUEST_EMOTE_COUNT; ++i)
        DetailsEmoteDelay[i] = questRecord[158 + i].Get<uint32>();

    EmoteOnIncomplete = questRecord[162].Get<uint16>();
    EmoteOnComplete = questRecord[163].Get<uint16>();

    for (int i = 0; i < QUEST_EMOTE_COUNT; ++i)
        OfferRewardEmote[i] = questRecord[164 + i].Get<int16>();

    for (int i = 0; i < QUEST_EMOTE_COUNT; ++i)
        OfferRewardEmoteDelay[i] = questRecord[168 + i].Get<int32>();

    // int32 WDBVerified = questRecord[172].Get<int32>();

    if (SpecialFlags & QUEST_SPECIAL_FLAGS_AUTO_ACCEPT)
        Flags |= QUEST_FLAGS_AUTO_ACCEPT;

    m_reqItemsCount = 0;
    m_reqNpcOrGoCount = 0;
    m_rewItemsCount = 0;
    m_rewChoiceItemsCount = 0;
    m_rewCurrencyCount = 0;
    m_reqCurrencyCount = 0;

    for (int i = 0; i < QUEST_ITEM_OBJECTIVES_COUNT; ++i)
        if (RequiredItemId[i])
            ++m_reqItemsCount;

    for (int i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
        if (RequiredNpcOrGo[i])
            ++m_reqNpcOrGoCount;

    for (int i = 0; i < QUEST_REWARDS_COUNT; ++i)
        if (RewardItemId[i])
            ++m_rewItemsCount;

    for (int i = 0; i < QUEST_REWARD_CHOICES_COUNT; ++i)
        if (RewardChoiceItemId[i])
            ++m_rewChoiceItemsCount;

    for (int i = 0; i < QUEST_REWARD_CURRENCY_COUNT; ++i)
        if (RewardCurrencyId[i])
            ++m_rewCurrencyCount;

    for (int i = 0; i < QUEST_REQUIRED_CURRENCY_COUNT; ++i)
        if (RequiredCurrencyId[i])
            ++m_reqCurrencyCount;

}

uint32 Quest::XPValue(Player* player) const
{
    if (player)
    {
        int32 quest_level = (Level == -1 ? player->getLevel() : Level);
        const QuestXPEntry* xpentry = sQuestXPStore.LookupEntry(quest_level);
        if (!xpentry)
            return 0;

        int32 diffFactor = 2 * (quest_level - player->getLevel()) + 20;
        if (diffFactor < 1)
            diffFactor = 1;
        else if (diffFactor > 10)
            diffFactor = 10;

        uint32 xp = diffFactor * xpentry->Exp[RewardXPId] / 10;
        if (xp <= 100)
            xp = 5 * ((xp + 2) / 5);
        else if (xp <= 500)
            xp = 10 * ((xp + 5) / 10);
        else if (xp <= 1000)
            xp = 25 * ((xp + 12) / 25);
        else
            xp = 50 * ((xp + 25) / 50);

        return xp;
    }

    return 0;
}

int32 Quest::GetRewOrReqMoney() const
{
    if (RewardOrRequiredMoney <= 0)
        return RewardOrRequiredMoney;

    return int32(RewardOrRequiredMoney * sWorld->getRate(RATE_DROP_MONEY));
}

void Quest::BuildExtraQuestInfo(WorldPacket& data, Player* player) const
{
    data << uint32(GetRewChoiceItemsCount());
    for (uint8 i = 0; i < QUEST_REWARD_CHOICES_COUNT; ++i)
        data << uint32(RewardChoiceItemId[i]);
    for (uint8 i = 0; i < QUEST_REWARD_CHOICES_COUNT; ++i)
        data << uint32(RewardChoiceItemCount[i]);
    for (uint8 i = 0; i < QUEST_REWARD_CHOICES_COUNT; ++i)
    {
        if (ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(RewardChoiceItemId[i]))
            data << uint32(itemTemplate->DisplayInfoID);
        else
            data << uint32(0);
    }

    data << uint32(GetRewItemsCount());
    for (uint8 i = 0; i < QUEST_REWARDS_COUNT; ++i)
        data << uint32(RewardItemId[i]);
    for (uint8 i = 0; i < QUEST_REWARDS_COUNT; ++i)
        data << uint32(RewardItemIdCount[i]);
    for (uint8 i = 0; i < QUEST_REWARDS_COUNT; ++i)
    {
        if (ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(RewardItemId[i]))
            data << uint32(itemTemplate->DisplayInfoID);
        else
            data << uint32(0);
    }

    data << uint32(GetRewOrReqMoney());
    data << uint32(XPValue(player) * sWorld->getRate(RATE_XP_QUEST));

    data << uint32(GetCharTitleId());
    data << uint32(0);                                      // unk
    data << float(0.0f);                                    // unk
    data << uint32(GetBonusTalents());
    data << uint32(0);                                      // unk
    data << uint32(GetRewardReputationMask());

    /* Pre cata struct, some of these unks might be the missing values in cata:
    // rewarded honor points. Multiply with 10 to satisfy client
    data << 10 * Firelands::Honor::hk_honor_at_level(_session->GetPlayer()->getLevel(), quest->GetRewHonorMultiplier());
    data << float(0);                                       // unk, honor multiplier?
    data << uint32(0x08);                                   // unused by client?
    data << uint32(quest->GetRewSpell());                   // reward spell, this spell will display (icon) (casted if RewSpellCast == 0)
    data << int32(quest->GetRewSpellCast());                // casted spell
    data << uint32(0);                                      // unknown
    data << uint32(quest->GetBonusTalents());               // bonus talents
    data << uint32(quest->GetRewArenaPoints());             // arena points
    data << uint32(0);
    */

    for (uint8 i = 0; i < QUEST_REPUTATIONS_COUNT; ++i)    // reward factions ids
        data << uint32(RewardFactionId[i]);

    for (uint8 i = 0; i < QUEST_REPUTATIONS_COUNT; ++i)    // columnid in QuestFactionReward.dbc (zero based)?
        data << int32(RewardFactionValueId[i]);

    for (uint8 i = 0; i < QUEST_REPUTATIONS_COUNT; ++i)    // reward reputation override?
        data << uint32(RewardFactionValueIdOverride[i]);

    data << uint32(GetRewSpell());
    data << uint32(GetRewSpellCast());

    for (uint8 i = 0; i < QUEST_REWARD_CURRENCY_COUNT; ++i)
        data << uint32(RewardCurrencyId[i]);

    for (uint8 i = 0; i < QUEST_REWARD_CURRENCY_COUNT; ++i)
        data << uint32(RewardCurrencyCount[i]);

    data << uint32(GetRewardSkillId());
    data << uint32(GetRewardSkillPoints());
}

bool Quest::IsAutoAccept() const
{
    return sWorld->getBoolConfig(CONFIG_QUEST_IGNORE_AUTO_ACCEPT) ? false : (HasFlag(QUEST_FLAGS_AUTO_ACCEPT) || HasFlag(QUEST_FLAGS_AUTO_TAKE));
}

bool Quest::IsAutoComplete() const
{
    return sWorld->getBoolConfig(CONFIG_QUEST_IGNORE_AUTO_COMPLETE) ? false : (Method == 0 || (HasFlag(QUEST_FLAGS_AUTOCOMPLETE) && !IsAutoAccept()));
}

bool Quest::IsSeasonal() const
{
    /*
    switch (ZoneOrSort)
    {
        case -QUEST_SORT_SEASONAL:
        case -QUEST_SORT_SPECIAL:
        case -QUEST_SORT_DARKMOON_FAIRE:
        case -QUEST_SORT_LUNAR_FESTIVAL:
        case -QUEST_SORT_MIDSUMMER:
        case -QUEST_SORT_BREWFEST:
        case -QUEST_SORT_NOBLEGARDEN:
        case -QUEST_SORT_PILGRIMS_BOUNTY:
        case -QUEST_SORT_LOVE_IS_IN_THE_AIR:
        case -QUEST_SORT_CHILDRENS_WEEK:
            break;
        default:
            return false;
    }

    return !IsRepeatable();
    */

    return sGameEventMgr->GetEventIdForQuest(this);
}

void Quest::SetSeasonal(uint32 /*eventId*/)
{
    SpecialFlags |= QUEST_SPECIAL_FLAGS_REPEATABLE;
}

bool Quest::IsRaidQuest(Difficulty difficulty) const
{
    switch (Type)
    {
    case QUEST_TYPE_RAID:
        return true;
    case QUEST_TYPE_RAID_10:
        return !(difficulty & RAID_DIFFICULTY_MASK_25MAN);
    case QUEST_TYPE_RAID_25:
        return difficulty & RAID_DIFFICULTY_MASK_25MAN;
    default:
        break;
    }

    return false;
}

bool Quest::IsAllowedInRaid(Difficulty difficulty) const
{
    if (IsRaidQuest(difficulty))
        return true;

    return sWorld->getBoolConfig(CONFIG_QUEST_IGNORE_RAID);
}

uint32 Quest::CalculateHonorGain(uint8 level) const
{
    if (level > GT_MAX_LEVEL)
        level = GT_MAX_LEVEL;

    uint32 honor = 0;

    /*if (GetRewHonorAddition() > 0 || GetRewHonorMultiplier() > 0.0f)
    {
        // values stored from 0.. for 1...
        TeamContributionPointsEntry const* tc = sTeamContributionPointsStore.LookupEntry(level);
        if (!tc)
            return 0;

        honor = uint32(tc->value * GetRewHonorMultiplier() * 0.1f);
        honor += GetRewHonorAddition();
    }*/

    return honor;
}

bool Quest::IsReported(Player* player) const
{
    if (!player)
        return false;

    uint32 status = 0;
    uint32 count = 0;
    bool finish = false;

    sObjectMgr->GetReportQuestStatuCount(GetQuestId(), status, count);
    if (!player->HaveReportQuest(GetQuestId()))
    {
        count++;
        sObjectMgr->ModifyReportQuestData(GetQuestId(), status, count);
    }

    if (status == 1)
        finish = true;

    return finish;
}
