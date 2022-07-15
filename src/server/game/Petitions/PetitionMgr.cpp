/*
* Copyright (C) 2008-2017 FirelandsCore <http://www.FirelandsCore.org/>
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

#include "PetitionMgr.h"
#include "DatabaseEnv.h"
#include "Log.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "Timer.h"
#include "WorldSession.h"

#include <unordered_map>

namespace
{
    std::unordered_map<ObjectGuid, Petition> _petitionStore;
}

PetitionMgr* PetitionMgr::instance()
{
    static PetitionMgr instance;
    return &instance;
}

void PetitionMgr::LoadPetitions()
{
    uint32 oldMSTime = getMSTime();
    _petitionStore.clear();

    QueryResult result = CharacterDatabase.Query("SELECT petitionguid, ownerguid, name, type FROM petition");
    if (!result)
    {
        LOG_INFO("server.loading", ">> Loaded 0 petitions.");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();
        AddPetition(ObjectGuid::Create<HighGuid::Item>(fields[0].Get<uint32>()), ObjectGuid::Create<HighGuid::Player>(fields[1].Get<uint32>()), fields[2].Get<std::string>(), static_cast<CharterTypes>(fields[3].Get<uint8>()), true);
        ++count;
    } while (result->NextRow());

    LOG_INFO("server.loading", ">> Loaded %u petitions in: %u ms.", count, GetMSTimeDiffToNow(oldMSTime));
}

void PetitionMgr::LoadSignatures()
{
    uint32 oldMSTime = getMSTime();

    QueryResult result = CharacterDatabase.Query("SELECT petitionguid, player_account, playerguid FROM petition_sign");
    if (!result)
    {
        LOG_INFO("server.loading", ">> Loaded 0 Petition signs!");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        Petition* petition = GetPetition(ObjectGuid::Create<HighGuid::Item>(fields[0].Get<uint32>()));
        if (!petition)
            continue;

        petition->AddSignature(petition->petitionGuid, fields[1].Get<uint32>(), ObjectGuid::Create<HighGuid::Player>(fields[2].Get<uint32>()), true);
        ++count;
    } while (result->NextRow());

    LOG_INFO("server.loading", ">> Loaded %u Petition signs in %u ms.", count, GetMSTimeDiffToNow(oldMSTime));
}

void PetitionMgr::AddPetition(ObjectGuid petitionGuid, ObjectGuid ownerGuid, std::string const& name, CharterTypes type, bool isLoading)
{
    Petition& p = _petitionStore[petitionGuid];
    p.petitionGuid = petitionGuid;
    p.ownerGuid = ownerGuid;
    p.petitionName = name;
    p.petitionType = type;
    p.signatures.clear();

    if (isLoading)
        return;

    CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_PETITION);
    stmt->SetData(0, ownerGuid.GetCounter());
    stmt->SetData(1, petitionGuid.GetCounter());
    stmt->SetData(2, name);
    stmt->SetData(3, uint8(type));
    CharacterDatabase.Execute(stmt);
}

void PetitionMgr::RemovePetition(ObjectGuid petitionGuid)
{
    _petitionStore.erase(petitionGuid);

    // Delete From DB
    CharacterDatabaseTransaction trans = CharacterDatabase.BeginTransaction();

    CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_PETITION_BY_GUID);
    stmt->SetData(0, petitionGuid.GetCounter());
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_PETITION_SIGNATURE_BY_GUID);
    stmt->SetData(0, petitionGuid.GetCounter());
    trans->Append(stmt);

    CharacterDatabase.CommitTransaction(trans);
}

Petition* PetitionMgr::GetPetition(ObjectGuid petitionGuid)
{
    auto itr = _petitionStore.find(petitionGuid);
    if (itr != _petitionStore.end())
        return &itr->second;

    return nullptr;
}

Petition* PetitionMgr::GetPetitionByOwnerWithType(ObjectGuid ownerGuid, CharterTypes type)
{
    for (auto& petitionPair : _petitionStore)
        if (petitionPair.second.ownerGuid == ownerGuid && petitionPair.second.petitionType == type)
            return &petitionPair.second;

    return nullptr;
}

void PetitionMgr::RemovePetitionsByOwnerAndType(ObjectGuid ownerGuid, CharterTypes type)
{
    for (auto itr = _petitionStore.begin(); itr != _petitionStore.end();)
    {
        if (itr->second.ownerGuid == ownerGuid)
        {
            if (type == CHARTER_TYPE_ANY)
                itr = _petitionStore.erase(itr);
            else if (type == itr->second.petitionType)
            {
                itr = _petitionStore.erase(itr);
                break;
            }
            else
                ++itr;
        }
        else
            ++itr;
    }

    CharacterDatabasePreparedStatement* stmt;
    CharacterDatabaseTransaction trans = CharacterDatabase.BeginTransaction();
    if (type == CHARTER_TYPE_ANY)
    {
        stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_PETITION_BY_OWNER);
        stmt->SetData(0, ownerGuid.GetCounter());
        trans->Append(stmt);

        stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_PETITION_SIGNATURE_BY_OWNER);
        stmt->SetData(0, ownerGuid.GetCounter());
        trans->Append(stmt);
    }
    else
    {
        stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_PETITION_BY_OWNER_AND_TYPE);
        stmt->SetData(0, ownerGuid.GetCounter());
        stmt->SetData(1, uint8(type));
        trans->Append(stmt);

        stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_PETITION_SIGNATURE_BY_OWNER_AND_TYPE);
        stmt->SetData(0, ownerGuid.GetCounter());
        stmt->SetData(1, uint8(type));
        trans->Append(stmt);
    }
    CharacterDatabase.CommitTransaction(trans);
}

void PetitionMgr::RemoveSignaturesBySignerAndType(ObjectGuid signerGuid, CharterTypes type)
{
    for (auto& petitionPair : _petitionStore)
    {
        if (petitionPair.second.petitionType == CHARTER_TYPE_ANY || petitionPair.second.petitionType == type)
            petitionPair.second.RemoveSignatureBySigner(signerGuid);
    }

    CharacterDatabasePreparedStatement* stmt;
    if (type == CHARTER_TYPE_ANY)
    {
        stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_ALL_PETITION_SIGNATURES);
        stmt->SetData(0, signerGuid.GetCounter());
    }
    else
    {
        stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_PETITION_SIGNATURE);
        stmt->SetData(0, signerGuid.GetCounter());
        stmt->SetData(1, uint8(type));
    }
    CharacterDatabase.Execute(stmt);
}

bool Petition::IsPetitionSignedByAccount(uint32 accountId) const
{
    for (Signature const& signature : signatures)
        if (signature.first == accountId)
            return true;

    return false;
}

void Petition::AddSignature(ObjectGuid petitionGuid, uint32 accountId, ObjectGuid playerGuid, bool isLoading)
{
    signatures.emplace_back(accountId, playerGuid);

    if (isLoading)
        return;

    CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_PETITION_SIGNATURE);

    stmt->SetData(0, ownerGuid.GetCounter());
    stmt->SetData(1, petitionGuid.GetCounter());
    stmt->SetData(2, playerGuid.GetCounter());
    stmt->SetData(3, accountId);

    CharacterDatabase.Execute(stmt);
}

void Petition::UpdateName(std::string const& newName)
{
    petitionName = newName;

    CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_PETITION_NAME);
    stmt->SetData(0, newName);
    stmt->SetData(1, petitionGuid.GetCounter());
    CharacterDatabase.Execute(stmt);
}

void Petition::RemoveSignatureBySigner(ObjectGuid playerGuid)
{
    for (auto itr = signatures.begin(); itr != signatures.end(); ++itr)
    {
        if (itr->second == playerGuid)
        {
            signatures.erase(itr);

            // notify owner
            if (Player* owner = ObjectAccessor::FindConnectedPlayer(ownerGuid))
                owner->GetSession()->SendPetitionQueryOpcode(petitionGuid);

            break;
        }
    }
}
