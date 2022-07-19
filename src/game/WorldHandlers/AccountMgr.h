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

#ifndef FIRELANDS_H_ACCMGR
#define FIRELANDS_H_ACCMGR

#include "Common.h"

/* An enumeration of the possible results of an account operation. */
enum AccountOpResult
{
    AOR_OK,
    AOR_NAME_TOO_LONG,
    AOR_PASS_TOO_LONG,
    AOR_NAME_ALREADY_EXIST,
    AOR_NAME_NOT_EXIST,
    AOR_DB_INTERNAL_ERROR
};

#define MAX_ACCOUNT_STR 32

/* A class that is used to manage accounts. */
class AccountMgr
{
    public:
        AccountMgr();
        ~AccountMgr();

        AccountOpResult CreateAccount(std::string username, std::string password);
        AccountOpResult CreateAccount(std::string username, std::string password, uint32 expansion);
        AccountOpResult DeleteAccount(uint32 accid);
        AccountOpResult ChangeUsername(uint32 accid, std::string new_uname, std::string new_passwd);
        AccountOpResult ChangePassword(uint32 accid, std::string new_passwd);
        bool CheckPassword(uint32 accid, std::string passwd);

        uint32 GetId(std::string username);
        AccountTypes GetSecurity(uint32 acc_id);
        bool GetName(uint32 acc_id, std::string& name);
        uint32 GetCharactersCount(uint32 acc_id);
        std::string CalculateShaPassHash(std::string& name, std::string& password);
};

/* A macro that creates a global variable called `sAccountMgr` that is an instance of the `AccountMgr` class. */
#define sAccountMgr Firelands::Singleton<AccountMgr>::Instance()
#endif
