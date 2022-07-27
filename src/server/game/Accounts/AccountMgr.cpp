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

#include "AccountMgr.h"
#include "DatabaseEnv.h"
#include "ObjectAccessor.h"
#include "Player.h"
#include "SRP6.h"
#include "ScriptMgr.h"
#include "Utilities/Util.h"
#include "WorldSession.h"


namespace AccountMgr {
    AccountOpResult CreateAccount(std::string username, std::string password)
    {
        if (utf8length(username) > MAX_ACCOUNT_STR)
            return AOR_NAME_TOO_LONG;                           // username's too long

        if (utf8length(password) > MAX_PASS_STR)
            return AOR_PASS_TOO_LONG;                           // password's too long

        Utf8ToUpperOnlyLatin(username);
        Utf8ToUpperOnlyLatin(password);

        if (GetId(username))
            return AOR_NAME_ALREDY_EXIST;                       // username does already exist

        PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_INS_ACCOUNT);

        stmt->setString(0, username);
        auto [salt, verifier] = Firelands::Crypto::SRP6::MakeRegistrationData(username, password);
        stmt->SetData(1, salt);
        stmt->SetData(2, verifier);
        stmt->SetData(3, uint8(sWorld->getIntConfig(CONFIG_EXPANSION)));

        LoginDatabase.Execute(stmt);

        stmt = LoginDatabase.GetPreparedStatement(LOGIN_INS_REALM_CHARACTERS_INIT);

        LoginDatabase.Execute(stmt);

        return AOR_OK;                                          // everything's fine
    }

    AccountOpResult DeleteAccount(uint32 accountId)
    {
        // Check if accounts exists
        PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_ACCOUNT_BY_ID);
        stmt->SetData(0, accountId);
        PreparedQueryResult result = LoginDatabase.Query(stmt);

        if (!result)
            return AOR_NAME_NOT_EXIST;

        // Obtain accounts characters
        stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARS_BY_ACCOUNT_ID);

        stmt->SetData(0, accountId);

        result = CharacterDatabase.Query(stmt);

        if (result)
        {
            do
            {
                uint32 guidLow = (*result)[0].GetUInt32();
                uint64 guid = MAKE_NEW_GUID(guidLow, 0, HIGHGUID_PLAYER);

                // Kick if player is online
                if (Player* p = ObjectAccessor::FindPlayer(guid))
                {
                    WorldSession* s = p->GetSession();
                    s->KickPlayer("Relog (kicking old session)");                            // mark session to remove at next session list update
                    s->LogoutPlayer(false);                     // logout player without waiting next session list update
                }

                Player::DeleteFromDB(guid, accountId, false);       // no need to update realm characters
            } while (result->NextRow());
        }

        // table realm specific but common for all characters of account for realm
        stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_TUTORIALS);
        stmt->SetData(0, accountId);
        CharacterDatabase.Execute(stmt);

        stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_ACCOUNT_DATA);
        stmt->SetData(0, accountId);
        CharacterDatabase.Execute(stmt);

        stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHARACTER_BAN);
        stmt->SetData(0, accountId);
        CharacterDatabase.Execute(stmt);

        SQLTransaction trans = LoginDatabase.BeginTransaction();

        stmt = LoginDatabase.GetPreparedStatement(LOGIN_DEL_ACCOUNT);
        stmt->SetData(0, accountId);
        trans->Append(stmt);

        stmt = LoginDatabase.GetPreparedStatement(LOGIN_DEL_ACCOUNT_ACCESS);
        stmt->SetData(0, accountId);
        trans->Append(stmt);

        stmt = LoginDatabase.GetPreparedStatement(LOGIN_DEL_REALM_CHARACTERS);
        stmt->SetData(0, accountId);
        trans->Append(stmt);

        stmt = LoginDatabase.GetPreparedStatement(LOGIN_DEL_ACCOUNT_BANNED);
        stmt->SetData(0, accountId);
        trans->Append(stmt);

        LoginDatabase.CommitTransaction(trans);

        return AOR_OK;
    }

    AccountOpResult ChangeUsername(uint32 accountId, std::string newUsername, std::string newPassword)
    {
        // Check if accounts exists
        PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_ACCOUNT_BY_ID);
        stmt->SetData(0, accountId);
        PreparedQueryResult result = LoginDatabase.Query(stmt);

        if (!result)
            return AOR_NAME_NOT_EXIST;

        if (utf8length(newUsername) > MAX_ACCOUNT_STR)
            return AOR_NAME_TOO_LONG;

        if (utf8length(newPassword) > MAX_PASS_STR)
            return AOR_PASS_TOO_LONG;

        Utf8ToUpperOnlyLatin(newUsername);
        Utf8ToUpperOnlyLatin(newPassword);

        stmt = LoginDatabase.GetPreparedStatement(LOGIN_UPD_USERNAME);

        stmt->SetData(0, newUsername);
        stmt->SetData(2, accountId);

        LoginDatabase.Execute(stmt);

        auto [salt, verifier] = Firelands::Crypto::SRP6::MakeRegistrationData(newUsername, newPassword);
        stmt = LoginDatabase.GetPreparedStatement(LOGIN_UPD_LOGON);
        stmt->SetData(0, salt);
        stmt->SetData(1, verifier);
        stmt->SetData(2, accountId);
        LoginDatabase.Execute(stmt);

        return AOR_OK;
    }

    AccountOpResult ChangePassword(uint32 accountId, std::string newPassword)
    {
        std::string username;

        if (!GetName(accountId, username))
            return AOR_NAME_NOT_EXIST;                          // account doesn't exist

        if (utf8length(newPassword) > MAX_PASS_STR)
            return AOR_PASS_TOO_LONG;

        Utf8ToUpperOnlyLatin(username);
        Utf8ToUpperOnlyLatin(newPassword);

        auto [salt, verifier] = Acore::Crypto::SRP6::MakeRegistrationData(username, newPassword);

        PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_UPD_PASSWORD);

        stmt->SetData(0, salt);
        stmt->SetData(1, verifier);
        stmt->SetData(2, accountId);
        LoginDatabase.Execute(stmt);

        sScriptMgr->OnPasswordChange(accountId);

        return AOR_OK;
    }

    uint32 GetId(std::string const& username)
    {
        PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_GET_ACCOUNT_ID_BY_USERNAME);
        stmt->SetData(0, username);
        PreparedQueryResult result = LoginDatabase.Query(stmt);

        return (result) ? (*result)[0].Get<uint32>() : 0;
    }

    uint32 GetSecurity(uint32 accountId)
    {
        PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_GET_ACCOUNT_ACCESS_GMLEVEL);
        stmt->SetData(0, accountId);
        PreparedQueryResult result = LoginDatabase.Query(stmt);

        return (result) ? (*result)[0].Get<uint8>() : uint32(SEC_PLAYER);
    }

    uint32 GetSecurity(uint32 accountId, int32 realmId)
    {
        PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_GET_GMLEVEL_BY_REALMID);
        stmt->SetData(0, accountId);
        stmt->SetData(1, realmId);
        PreparedQueryResult result = LoginDatabase.Query(stmt);

        return (result) ? (*result)[0].Get<uint8>() : uint32(SEC_PLAYER);
    }

    bool GetName(uint32 accountId, std::string& name)
    {
        PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_GET_USERNAME_BY_ID);
        stmt->SetData(0, accountId);
        PreparedQueryResult result = LoginDatabase.Query(stmt);

        if (result)
        {
            name = (*result)[0].Get<std::string>();
            return true;
        }

        return false;
    }

    bool CheckPassword(uint32 accountId, std::string password)
    {
        std::string username;

        if (!GetName(accountId, username))
            return false;

        Utf8ToUpperOnlyLatin(username);
        Utf8ToUpperOnlyLatin(password);

        PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_CHECK_PASSWORD);
        stmt->SetData(0, accountId);
        if (PreparedQueryResult result = LoginDatabase.Query(stmt))
        {
            Firelands::Crypto::SRP6::Salt salt = (*result)[0].Get<Binary, Firelands::Crypto::SRP6::SALT_LENGTH>();
            Firelands::Crypto::SRP6::Verifier verifier = (*result)[1].Get<Binary, Firelands::Crypto::SRP6::VERIFIER_LENGTH>();
            if (Firelands::Crypto::SRP6::CheckLogin(username, password, salt, verifier))
                return true;
        }

        return false;
    }

    uint32 GetCharactersCount(uint32 accountId)
    {
        // check character count
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_SUM_CHARS);
        stmt->SetData(0, accountId);
        PreparedQueryResult result = CharacterDatabase.Query(stmt);

        return (result) ? (*result)[0].Get<uint64>() : 0;
    }

    bool IsPlayerAccount(uint32 gmlevel)
    {
        return gmlevel == SEC_PLAYER;
    }

    bool IsModeratorAccount(uint32 gmlevel)
    {
        return gmlevel >= SEC_MODERATOR && gmlevel <= SEC_CONSOLE;
    }

    bool IsGMAccount(uint32 gmlevel)
    {
        return gmlevel >= SEC_GAMEMASTER && gmlevel <= SEC_CONSOLE;
    }

    bool IsAdminAccount(uint32 gmlevel)
    {
        return gmlevel >= SEC_ADMINISTRATOR && gmlevel <= SEC_CONSOLE;
    }

    bool IsConsoleAccount(uint32 gmlevel)
    {
        return gmlevel == SEC_CONSOLE;
    }
    }
