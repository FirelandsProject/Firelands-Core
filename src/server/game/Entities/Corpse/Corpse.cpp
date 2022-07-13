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

#include "Corpse.h"
#include "CharacterCache.h"
#include "Common.h"
#include "DatabaseEnv.h"
#include "GameTime.h"
#include "Log.h"
#include "Map.h"
#include "ObjectAccessor.h"
#include "PhasingHandler.h"
#include "Player.h"
#include "UpdateData.h"
#include "UpdateMask.h"
#include "World.h"

Corpse::Corpse(CorpseType type) : WorldObject(type != CORPSE_BONES), m_type(type)
{
    m_objectType |= TYPEMASK_CORPSE;
    m_objectTypeId = TYPEID_CORPSE;

    m_updateFlag = UPDATEFLAG_STATIONARY_POSITION;

    m_valuesCount = CORPSE_END;

    m_time = GameTime::GetGameTime();

    lootRecipient = nullptr;
}

Corpse::~Corpse() { }

void Corpse::AddToWorld()
{
    ///- Register the corpse for guid lookup
    if (!IsInWorld())
        GetMap()->GetObjectsStore().Insert<Corpse>(GetGUID(), this);

    Object::AddToWorld();
}

void Corpse::RemoveFromWorld()
{
    ///- Remove the corpse from the accessor
    if (IsInWorld())
        GetMap()->GetObjectsStore().Remove<Corpse>(GetGUID());

    WorldObject::RemoveFromWorld();
}

bool Corpse::Create(ObjectGuid::LowType guidlow)
{
    Object::_Create(guidlow, 0, HighGuid::Corpse);
    return true;
}

bool Corpse::Create(ObjectGuid::LowType guidlow, Player* owner)
{
    ASSERT(owner);

    Relocate(owner->GetPositionX(), owner->GetPositionY(), owner->GetPositionZ(), owner->GetOrientation());

    if (!IsPositionValid())
    {
        LOG_ERROR("entities.player", "Corpse (guidlow %d, owner %s) not created. Suggested coordinates isn't valid (X: %f Y: %f)",
            guidlow, owner->GetName().c_str(), owner->GetPositionX(), owner->GetPositionY());
        return false;
    }

    WorldObject::_Create(guidlow, HighGuid::Corpse);

    SetObjectScale(1.0f);
    SetGuidValue(CORPSE_FIELD_OWNER, owner->GetGUID());

    _cellCoord = Firelands::ComputeCellCoord(GetPositionX(), GetPositionY());

    PhasingHandler::InheritPhaseShift(this, owner);

    return true;
}

void Corpse::SaveToDB()
{
    // prevent DB data inconsistence problems and duplicates
    CharacterDatabaseTransaction trans = CharacterDatabase.BeginTransaction();
    DeleteFromDB(trans);

    uint16 index = 0;
    CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_CORPSE);
    stmt->SetData(index++, GetOwnerGUID().GetCounter());                            // guid
    stmt->SetData(index++, GetPositionX());                                         // posX
    stmt->SetData(index++, GetPositionY());                                         // posY
    stmt->SetData(index++, GetPositionZ());                                         // posZ
    stmt->SetData(index++, GetOrientation());                                       // orientation
    stmt->SetData(index++, GetMapId());                                             // mapId
    stmt->SetData(index++, GetUInt32Value(CORPSE_FIELD_DISPLAY_ID));                // displayId
    stmt->SetData(index++, _ConcatFields(CORPSE_FIELD_ITEM, EQUIPMENT_SLOT_END));   // itemCache
    stmt->SetData(index++, GetUInt32Value(CORPSE_FIELD_BYTES_1));                   // bytes1
    stmt->SetData(index++, GetUInt32Value(CORPSE_FIELD_BYTES_2));                   // bytes2
    stmt->SetData(index++, GetUInt32Value(CORPSE_FIELD_FLAGS));                     // flags
    stmt->SetData(index++, GetUInt32Value(CORPSE_FIELD_DYNAMIC_FLAGS));             // dynFlags
    stmt->SetData(index++, uint32(m_time));                                         // time
    stmt->SetData(index++, GetType());                                              // corpseType
    stmt->SetData(index++, GetInstanceId());                                        // instanceId
    trans->Append(stmt);

    for (PhaseShift::PhaseRef const& phase : GetPhaseShift().GetPhases())
    {
        index = 0;
        stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_CORPSE_PHASES);
        stmt->SetData(index++, GetOwnerGUID().GetCounter());                        // OwnerGuid
        stmt->SetData(index++, phase.Id);                                           // PhaseId
        trans->Append(stmt);
    }

    CharacterDatabase.CommitTransaction(trans);
}

void Corpse::DeleteFromDB(CharacterDatabaseTransaction& trans)
{
    Corpse::DeleteFromDB(GetOwnerGUID(), trans);
}

void Corpse::DeleteFromDB(ObjectGuid const& ownerGuid, CharacterDatabaseTransaction& trans)
{
    CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CORPSE);
    stmt->SetData(0, ownerGuid.GetCounter());
    CharacterDatabase.ExecuteOrAppend(trans, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CORPSE_PHASES);
    stmt->SetData(0, ownerGuid.GetCounter());
    CharacterDatabase.ExecuteOrAppend(trans, stmt);
}

void Corpse::ResetGhostTime()
{
    m_time = GameTime::GetGameTime();
}

bool Corpse::LoadCorpseFromDB(ObjectGuid::LowType guid, Field* fields)
{
    //        0     1     2     3            4      5          6          7       8       9      10        11    12          13          14
    // SELECT posX, posY, posZ, orientation, mapId, displayId, itemCache, bytes1, bytes2, flags, dynFlags, time, corpseType, instanceId, guid FROM corpse WHERE mapId = ? AND instanceId = ?

    float posX = fields[0].Get<float>();
    float posY = fields[1].Get<float>();
    float posZ = fields[2].Get<float>();
    float o = fields[3].Get<float>();
    uint32 mapId = fields[4].Get<uint16>();

    Object::_Create(guid, 0, HighGuid::Corpse);

    SetObjectScale(1.0f);
    SetUInt32Value(CORPSE_FIELD_DISPLAY_ID, fields[5].Get<uint32>());
    _LoadIntoDataField(fields[6].Get<std::string>(), CORPSE_FIELD_ITEM, EQUIPMENT_SLOT_END);
    SetUInt32Value(CORPSE_FIELD_BYTES_1, fields[7].Get<uint32>());
    SetUInt32Value(CORPSE_FIELD_BYTES_2, fields[8].Get<uint32>());
    SetUInt32Value(CORPSE_FIELD_FLAGS, fields[9].Get<uint8>());
    SetUInt32Value(CORPSE_FIELD_DYNAMIC_FLAGS, fields[10].Get<uint8>());
    SetGuidValue(CORPSE_FIELD_OWNER, ObjectGuid::Create<HighGuid::Player>(fields[14].Get<uint32>()));

    m_time = time_t(fields[11].Get<uint32>());

    uint32 instanceId = fields[13].Get<uint32>();

    // place
    SetLocationInstanceId(instanceId);
    SetLocationMapId(mapId);
    Relocate(posX, posY, posZ, o);

    if (!IsPositionValid())
    {
        LOG_ERROR("entities.player", "Corpse (%s, owner: %s) is not created, given coordinates are not valid (X: %f, Y: %f, Z: %f)",
            GetGUID().ToString().c_str(), GetOwnerGUID().ToString().c_str(), posX, posY, posZ);
        return false;
    }

    _cellCoord = Firelands::ComputeCellCoord(GetPositionX(), GetPositionY());
    return true;
}

bool Corpse::IsExpired(time_t t) const
{
    // Deleted character
    if (!sCharacterCache->HasCharacterCacheEntry(GetOwnerGUID()))
        return true;

    if (m_type == CORPSE_BONES)
        return m_time < t - 60 * MINUTE;
    else
        return m_time < t - 3 * DAY;
}
