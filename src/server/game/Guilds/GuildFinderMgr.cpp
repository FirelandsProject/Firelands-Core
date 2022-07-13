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

#include "GuildFinderMgr.h"
#include "CharacterCache.h"
#include "DatabaseEnv.h"
#include "DBCStores.h"
#include "GameTime.h"
#include "Guild.h"
#include "GuildMgr.h"
#include "Log.h"
#include "ObjectAccessor.h"
#include "Player.h"
#include "World.h"

MembershipRequest::MembershipRequest() : _guildId(0), _availability(0), _classRoles(0), _interests(0), _time(GameTime::GetGameTime())
{
}

MembershipRequest::MembershipRequest(ObjectGuid playerGUID, uint32 guildId, uint32 availability, uint32 classRoles, uint32 interests, std::string comment, time_t submitTime) :
    _comment(std::move(comment)), _guildId(guildId), _playerGUID(playerGUID), _availability(availability), _classRoles(classRoles), _interests(interests), _time(submitTime)
{
}

uint8 MembershipRequest::GetClass() const
{
    return sCharacterCache->GetCharacterCacheByGuid(GetPlayerGUID())->Class;
}

uint8 MembershipRequest::GetLevel() const
{
    return sCharacterCache->GetCharacterCacheByGuid(GetPlayerGUID())->Level;
}

std::string const& MembershipRequest::GetName() const
{
    return sCharacterCache->GetCharacterCacheByGuid(GetPlayerGUID())->Name;
}

GuildFinderMgr::GuildFinderMgr()
{
}

GuildFinderMgr::~GuildFinderMgr()
{
}

void GuildFinderMgr::LoadFromDB()
{
    LoadGuildSettings();
    LoadMembershipRequests();
}

void GuildFinderMgr::LoadGuildSettings()
{
    LOG_INFO("server.loading", "Loading guild finder guild-related settings...");
    //                                                           0                1             2                  3             4           5             6         7
    QueryResult result = CharacterDatabase.Query("SELECT gfgs.guildId, gfgs.availability, gfgs.classRoles, gfgs.interests, gfgs.level, gfgs.listed, gfgs.comment, c.race "
        "FROM guild_finder_guild_settings gfgs "
        "LEFT JOIN guild_member gm ON gm.guildid=gfgs.guildId "
        "LEFT JOIN characters c ON c.guid = gm.guid");

    if (!result)
    {
        LOG_INFO("server.loading", ">> Loaded 0 guild finder guild-related settings. Table `guild_finder_guild_settings` is empty.");
        return;
    }

    uint32 count = 0;
    uint32 oldMSTime = getMSTime();
    do
    {
        Field* fields = result->Fetch();
        uint32 guildId = fields[0].Get<uint32>();
        uint8  availability = fields[1].Get<uint8>();
        uint8  classRoles = fields[2].Get<uint8>();
        uint8  interests = fields[3].Get<uint8>();
        uint8  level = fields[4].Get<uint8>();
        bool   listed = (fields[5].Get<uint8>() != 0);
        std::string comment = fields[6].Get<std::string>();

        TeamId guildTeam = TEAM_ALLIANCE;
        if (ChrRacesEntry const* raceEntry = sChrRacesStore.LookupEntry(fields[7].Get<uint8>()))
            if (raceEntry->BaseLanguage == 1)
                guildTeam = TEAM_HORDE;

        _guildSettings[guildId] = LFGuildSettings(listed, guildTeam, guildId, classRoles, availability, interests, level, comment);

        ++count;
    } while (result->NextRow());

    LOG_INFO("server.loading", ">> Loaded %u guild finder guild-related settings in %u ms.", count, GetMSTimeDiffToNow(oldMSTime));
}

void GuildFinderMgr::LoadMembershipRequests()
{
    LOG_INFO("server.loading", "Loading guild finder membership requests...");
    //                                                      0         1           2            3           4         5         6
    QueryResult result = CharacterDatabase.Query("SELECT guildId, playerGuid, availability, classRole, interests, comment, submitTime "
        "FROM guild_finder_applicant");

    if (!result)
    {
        LOG_INFO("server.loading", ">> Loaded 0 guild finder membership requests. Table `guild_finder_applicant` is empty.");
        return;
    }

    uint32 count = 0;
    uint32 oldMSTime = getMSTime();
    do
    {
        Field* fields = result->Fetch();
        uint32 guildId = fields[0].Get<uint32>();
        uint32 playerId = fields[1].Get<uint32>();
        uint8  availability = fields[2].Get<uint8>();
        uint8  classRoles = fields[3].Get<uint8>();
        uint8  interests = fields[4].Get<uint8>();
        std::string comment = fields[5].Get<std::string>();
        uint32 submitTime = fields[6].Get<uint32>();

        MembershipRequest request(ObjectGuid(HighGuid::Player, playerId), guildId, availability, classRoles, interests, comment, time_t(submitTime));

        _membershipRequests[guildId].push_back(request);

        ++count;
    } while (result->NextRow());

    LOG_INFO("server.loading", ">> Loaded %u guild finder membership requests in %u ms.", count, GetMSTimeDiffToNow(oldMSTime));
}

void GuildFinderMgr::AddMembershipRequest(uint32 guildGuid, MembershipRequest const& request)
{
    _membershipRequests[guildGuid].push_back(request);

    CharacterDatabaseTransaction trans = CharacterDatabase.BeginTransaction();
    CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_REP_GUILD_FINDER_APPLICANT);
    stmt->SetData(0, request.GetGuildId());
    stmt->SetData(1, request.GetPlayerGUID());
    stmt->SetData(2, request.GetAvailability());
    stmt->SetData(3, request.GetClassRoles());
    stmt->SetData(4, request.GetInterests());
    stmt->SetData(5, request.GetComment());
    stmt->SetData(6, request.GetSubmitTime());
    trans->Append(stmt);
    CharacterDatabase.CommitTransaction(trans);

    // Notify the applicant his submittion has been added
    if (Player* player = ObjectAccessor::FindPlayer(request.GetPlayerGUID()))
        SendMembershipRequestListUpdate(*player);

    // Notify the guild master and officers the list changed
    if (Guild* guild = sGuildMgr->GetGuildById(guildGuid))
        SendApplicantListUpdate(*guild);
}

void GuildFinderMgr::RemoveAllMembershipRequestsFromPlayer(uint32 playerId)
{
    for (MembershipRequestStore::iterator itr = _membershipRequests.begin(); itr != _membershipRequests.end(); ++itr)
    {
        std::vector<MembershipRequest>::iterator itr2 = itr->second.begin();
        for (; itr2 != itr->second.end(); ++itr2)
            if (itr2->GetPlayerGUID() == playerId)
                break;

        if (itr2 == itr->second.end())
            continue;

        CharacterDatabaseTransaction trans = CharacterDatabase.BeginTransaction();
        CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GUILD_FINDER_APPLICANT);
        stmt->SetData(0, itr2->GetGuildId());
        stmt->SetData(1, itr2->GetPlayerGUID());
        trans->Append(stmt);

        CharacterDatabase.CommitTransaction(trans);
        itr->second.erase(itr2);

        // Notify the guild master and officers the list changed
        if (Guild* guild = sGuildMgr->GetGuildById(itr->first))
            SendApplicantListUpdate(*guild);
    }
}

void GuildFinderMgr::RemoveMembershipRequest(uint32 playerId, uint32 guildId)
{
    std::vector<MembershipRequest>::iterator itr = _membershipRequests[guildId].begin();
    for (; itr != _membershipRequests[guildId].end(); ++itr)
        if (itr->GetPlayerGUID() == playerId)
            break;

    if (itr == _membershipRequests[guildId].end())
        return;

    CharacterDatabaseTransaction trans = CharacterDatabase.BeginTransaction();

    CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GUILD_FINDER_APPLICANT);
    stmt->SetData(0, itr->GetGuildId());
    stmt->SetData(1, itr->GetPlayerGUID().GetCounter());
    trans->Append(stmt);

    CharacterDatabase.CommitTransaction(trans);

    // Notify the applicant his submittion has been removed
    if (Player* player = ObjectAccessor::FindPlayer(itr->GetPlayerGUID()))
        SendMembershipRequestListUpdate(*player);

    // Notify the guild master and officers the list changed
    if (Guild* guild = sGuildMgr->GetGuildById(guildId))
        SendApplicantListUpdate(*guild);

    _membershipRequests[guildId].erase(itr);
}

std::list<MembershipRequest> GuildFinderMgr::GetAllMembershipRequestsForPlayer(uint32 playerGuid)
{
    std::list<MembershipRequest> resultSet;
    for (MembershipRequestStore::const_iterator itr = _membershipRequests.begin(); itr != _membershipRequests.end(); ++itr)
    {
        std::vector<MembershipRequest> const& guildReqs = itr->second;
        for (std::vector<MembershipRequest>::const_iterator itr2 = guildReqs.begin(); itr2 != guildReqs.end(); ++itr2)
        {
            if (itr2->GetPlayerGUID() == playerGuid)
            {
                resultSet.push_back(*itr2);
                break;
            }
        }
    }
    return resultSet;
}

uint8 GuildFinderMgr::CountRequestsFromPlayer(uint32 playerId)
{
    uint8 result = 0;
    for (MembershipRequestStore::const_iterator itr = _membershipRequests.begin(); itr != _membershipRequests.end(); ++itr)
    {
        for (std::vector<MembershipRequest>::const_iterator itr2 = itr->second.begin(); itr2 != itr->second.end(); ++itr2)
        {
            if (itr2->GetPlayerGUID() != playerId)
                continue;
            ++result;
            break;
        }
    }
    return result;
}

LFGuildStore GuildFinderMgr::GetGuildsMatchingSetting(LFGuildPlayer& settings, TeamId faction)
{
    LFGuildStore resultSet;
    for (LFGuildStore::const_iterator itr = _guildSettings.begin(); itr != _guildSettings.end(); ++itr)
    {
        LFGuildSettings const& guildSettings = itr->second;

        if (guildSettings.GetTeam() != faction)
            continue;

        if (!(guildSettings.GetAvailability() & settings.GetAvailability()))
            continue;

        if (!(guildSettings.GetClassRoles() & settings.GetClassRoles()))
            continue;

        if (!(guildSettings.GetInterests() & settings.GetInterests()))
            continue;

        if (!(guildSettings.GetLevel() & settings.GetLevel()))
            continue;

        resultSet.insert(std::make_pair(itr->first, guildSettings));
    }

    return resultSet;
}

bool GuildFinderMgr::HasRequest(uint32 playerId, uint32 guildId)
{
    for (std::vector<MembershipRequest>::const_iterator itr = _membershipRequests[guildId].begin(); itr != _membershipRequests[guildId].end(); ++itr)
        if (itr->GetPlayerGUID() == playerId)
            return true;
    return false;
}

void GuildFinderMgr::SetGuildSettings(uint32 guildGuid, LFGuildSettings const& settings)
{
    _guildSettings[guildGuid] = settings;

    CharacterDatabaseTransaction trans = CharacterDatabase.BeginTransaction();

    CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_REP_GUILD_FINDER_GUILD_SETTINGS);
    stmt->SetData(0, settings.GetGUID());
    stmt->SetData(1, settings.GetAvailability());
    stmt->SetData(2, settings.GetClassRoles());
    stmt->SetData(3, settings.GetInterests());
    stmt->SetData(4, settings.GetLevel());
    stmt->SetData(5, settings.IsListed());
    stmt->SetData(6, settings.GetComment());
    trans->Append(stmt);

    CharacterDatabase.CommitTransaction(trans);
}

void GuildFinderMgr::DeleteGuild(uint32 guildId)
{
    std::vector<MembershipRequest>::iterator itr = _membershipRequests[guildId].begin();
    while (itr != _membershipRequests[guildId].end())
    {
        CharacterDatabaseTransaction trans = CharacterDatabase.BeginTransaction();

        uint32 applicant = itr->GetPlayerGUID();

        CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GUILD_FINDER_APPLICANT);
        stmt->SetData(0, itr->GetGuildId());
        stmt->SetData(1, applicant);
        trans->Append(stmt);

        stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GUILD_FINDER_GUILD_SETTINGS);
        stmt->SetData(0, itr->GetGuildId());
        trans->Append(stmt);

        CharacterDatabase.CommitTransaction(trans);

        // Notify the applicant his submition has been removed
        if (Player* player = ObjectAccessor::FindPlayer(itr->GetPlayerGUID()))
            SendMembershipRequestListUpdate(*player);

        ++itr;
    }

    _membershipRequests.erase(guildId);
    _guildSettings.erase(guildId);

    // Notify the guild master the list changed (even if he's not a GM any more, not sure if needed)
    if (Guild* guild = sGuildMgr->GetGuildById(guildId))
        SendApplicantListUpdate(*guild);
}

void GuildFinderMgr::SendApplicantListUpdate(Guild& guild)
{
    WorldPacket data(SMSG_LF_GUILD_APPLICANT_LIST_UPDATED, 0);
    if (Player* player = ObjectAccessor::FindPlayer(guild.GetLeaderGUID()))
        player->SendDirectMessage(&data);
    guild.BroadcastPacketToRank(&data, GR_OFFICER);
}

void GuildFinderMgr::SendMembershipRequestListUpdate(Player& player)
{
    WorldPacket data(SMSG_LF_GUILD_APPLICATIONS_LIST_CHANGED, 0);
    player.SendDirectMessage(&data);
}
