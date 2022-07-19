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

#include "Chat.h"
#include "Language.h"
#include "GuildMgr.h"
#include "Guild.h"

 /**********************************************************************
     CommandTable : guildCommandTable
 /***********************************************************************/

 /** \brief GM command level 3 - Create a guild.
  *
  * This command allows a GM (level 3) to create a guild.
  *
  * The "args" parameter contains the name of the guild leader
  * and then the name of the guild.
  *
  */
bool ChatHandler::HandleGuildCreateCommand(char* args)
{
    // guildmaster name optional
    char* guildMasterStr = ExtractOptNotLastArg(&args);

    Player* target;
    if (!ExtractPlayerTarget(&guildMasterStr, &target))
    {
        return false;
    }

    char* guildStr = ExtractQuotedArg(&args);
    if (!guildStr)
    {
        return false;
    }

    std::string guildname = guildStr;

    if (target->GetGuildId())
    {
        SendSysMessage(LANG_PLAYER_IN_GUILD);
        return true;
    }

    Guild* guild = new Guild;
    if (!guild->Create(target, guildname))
    {
        delete guild;
        SendSysMessage(LANG_GUILD_NOT_CREATED);
        SetSentErrorMessage(true);
        return false;
    }

    sGuildMgr.AddGuild(guild);
    return true;
}

bool ChatHandler::HandleGuildInviteCommand(char* args)
{
    // player name optional
    char* nameStr = ExtractOptNotLastArg(&args);

    // if not guild name only (in "") then player name
    ObjectGuid target_guid;
    if (!ExtractPlayerTarget(&nameStr, NULL, &target_guid))
    {
        return false;
    }

    char* guildStr = ExtractQuotedArg(&args);
    if (!guildStr)
    {
        return false;
    }

    std::string glName = guildStr;
    Guild* targetGuild = sGuildMgr.GetGuildByName(glName);
    if (!targetGuild)
    {
        return false;
    }

    // player's guild membership checked in AddMember before add
    if (!targetGuild->AddMember(target_guid, targetGuild->GetLowestRank()))
    {
        return false;
    }

    return true;
}

bool ChatHandler::HandleGuildUninviteCommand(char* args)
{
    Player* target;
    ObjectGuid target_guid;
    if (!ExtractPlayerTarget(&args, &target, &target_guid))
    {
        return false;
    }

    uint32 glId = target ? target->GetGuildId() : Player::GetGuildIdFromDB(target_guid);
    if (!glId)
    {
        return false;
    }

    Guild* targetGuild = sGuildMgr.GetGuildById(glId);
    if (!targetGuild)
    {
        return false;
    }

    if (targetGuild->DelMember(target_guid))
    {
        targetGuild->Disband();
        delete targetGuild;
    }

    return true;
}

bool ChatHandler::HandleGuildRankCommand(char* args)
{
    char* nameStr = ExtractOptNotLastArg(&args);

    Player* target;
    ObjectGuid target_guid;
    std::string target_name;
    if (!ExtractPlayerTarget(&nameStr, &target, &target_guid, &target_name))
    {
        return false;
    }

    uint32 glId = target ? target->GetGuildId() : Player::GetGuildIdFromDB(target_guid);
    if (!glId)
    {
        return false;
    }

    Guild* targetGuild = sGuildMgr.GetGuildById(glId);
    if (!targetGuild)
    {
        return false;
    }

    uint32 newrank;
    if (!ExtractUInt32(&args, newrank))
    {
        return false;
    }

    if (newrank > targetGuild->GetLowestRank())
    {
        return false;
    }

    MemberSlot* slot = targetGuild->GetMemberSlot(target_guid);
    if (!slot)
    {
        return false;
    }

    slot->ChangeRank(newrank);
    return true;
}

bool ChatHandler::HandleGuildDeleteCommand(char* args)
{
    if (!*args)
    {
        return false;
    }

    char* guildStr = ExtractQuotedArg(&args);
    if (!guildStr)
    {
        return false;
    }

    std::string gld = guildStr;

    Guild* targetGuild = sGuildMgr.GetGuildByName(gld);
    if (!targetGuild)
    {
        return false;
    }

    targetGuild->Disband();
    delete targetGuild;

    return true;
}
