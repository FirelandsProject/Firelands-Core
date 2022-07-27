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

#include "Common.h"
#include "Corpse.h"
#include "Player.h"
#include "UpdateMask.h"
#include "ObjectAccessor.h"
#include "DatabaseEnv.h"
#include "Opcodes.h"
#include "GossipDef.h"
#include "World.h"

Corpse::Corpse(CorpseType type) : WorldObject(type != CORPSE_BONES), m_type(type)
{
    m_objectType |= TYPEMASK_CORPSE;
    m_objectTypeId = TYPEID_CORPSE;

    m_updateFlag = UPDATEFLAG_STATIONARY_POSITION;

    m_valuesCount = CORPSE_END;

    m_time = time(NULL);

    lootForBody = false;
    lootRecipient = NULL;
}

Corpse::~Corpse()
{
}

void Corpse::AddToWorld()
{
    ///- Register the corpse for guid lookup
    if (!IsInWorld())
        sObjectAccessor->AddObject(this);

    Object::AddToWorld();
}

void Corpse::RemoveFromWorld()
{
    ///- Remove the corpse from the accessor
    if (IsInWorld())
        sObjectAccessor->RemoveObject(this);

    Object::RemoveFromWorld();
}

bool Corpse::Create(uint32 guidlow, Map* map)
{
    SetMap(map);
    Object::_Create(guidlow, 0, HIGHGUID_CORPSE);
    return true;
}

bool Corpse::Create(uint32 guidlow, Player* owner)
{
    ASSERT(owner);

    Relocate(owner->GetPositionX(), owner->GetPositionY(), owner->GetPositionZ(), owner->GetOrientation());

    if (!IsPositionValid())
    {
        LOG_ERROR("entities.player", "Corpse (guidlow {}, owner {}) not created. Suggested coordinates isn't valid (X: {} Y: {})",
            guidlow, owner->GetName().c_str(), owner->GetPositionX(), owner->GetPositionY());
        return false;
    }

    //we need to assign owner's map for corpse
    //in other way we will get a crash in Corpse::SaveToDB()
    SetMap(owner->GetMap());

    WorldObject::_Create(guidlow, HIGHGUID_CORPSE, owner->GetPhaseMask());

    SetObjectScale(1);
    SetUInt64Value(CORPSE_FIELD_OWNER, owner->GetGUID());

    _gridCoord = Firelands::ComputeGridCoord(GetPositionX(), GetPositionY());

    return true;
}

void Corpse::SaveToDB()
{
    // prevent DB data inconsistence problems and duplicates
    SQLTransaction trans = CharacterDatabase.BeginTransaction();
    DeleteFromDB(trans);

    uint16 index = 0;
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_CORPSE);
    stmt->SetData(index++, GetGUIDLow());                                           // corpseGuid
    stmt->SetData(index++, GUID_LOPART(GetOwnerGUID()));                            // guid
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
    stmt->SetData(index++, GetPhaseMask());                                         // phaseMask
    trans->Append(stmt);

    CharacterDatabase.CommitTransaction(trans);
}

void Corpse::DeleteBonesFromWorld()
{
    ASSERT(GetType() == CORPSE_BONES);
    Corpse* corpse = ObjectAccessor::GetCorpse(*this, GetGUID());

    if (!corpse)
    {
        LOG_ERROR("entities.player", "Bones {} not found in world.", GetGUIDLow());
        return;
    }

    AddObjectToRemoveList();
}

void Corpse::DeleteFromDB(SQLTransaction& trans)
{
    PreparedStatement* stmt = NULL;
    if (GetType() == CORPSE_BONES)
    {
        // Only specific bones
        stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CORPSE);
        stmt->SetData(0, GetGUIDLow());
    }
    else
    {
        // all corpses (not bones)
        stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_PLAYER_CORPSES);
        stmt->SetData(0, GUID_LOPART(GetOwnerGUID()));
    }
    trans->Append(stmt);
}

bool Corpse::LoadCorpseFromDB(uint32 guid, Field* fields)
{
    //        0     1     2     3            4      5          6          7       8       9      10        11    12          13          14          15         16
    // SELECT posX, posY, posZ, orientation, mapId, displayId, itemCache, bytes1, bytes2, flags, dynFlags, time, corpseType, instanceId, phaseMask, corpseGuid, guid FROM corpse WHERE corpseType <> 0

    uint32 ownerGuid = fields[16].Get<uint32>();
    float posX   = fields[0].Get<float>();
    float posY   = fields[1].Get<float>();
    float posZ   = fields[2].Get<float>();
    float o      = fields[3].Get<float>();
    uint32 mapId = fields[4].Get<uint16>();

    Object::_Create(guid, 0, HIGHGUID_CORPSE);

    SetUInt32Value(CORPSE_FIELD_DISPLAY_ID, fields[5].Get<uint32>());
    _LoadIntoDataField(fields[6].Get<std::string>().c_str(), CORPSE_FIELD_ITEM, EQUIPMENT_SLOT_END);
    SetUInt32Value(CORPSE_FIELD_BYTES_1, fields[7].Get<uint32>());
    SetUInt32Value(CORPSE_FIELD_BYTES_2, fields[8].Get<uint32>());
    SetUInt32Value(CORPSE_FIELD_FLAGS, fields[9].Get<uint8>());
    SetUInt32Value(CORPSE_FIELD_DYNAMIC_FLAGS, fields[10].Get<uint8>());
    SetUInt64Value(CORPSE_FIELD_OWNER, MAKE_NEW_GUID(ownerGuid, 0, HIGHGUID_PLAYER));

    m_time = time_t(fields[11].Get<uint32>());

    uint32 instanceId  = fields[13].Get<uint32>();
    uint32 phaseMask   = fields[14].Get<uint16>();

    // place
    SetLocationInstanceId(instanceId);
    SetLocationMapId(mapId);
    SetPhaseMask(phaseMask, false);
    Relocate(posX, posY, posZ, o);

    if (!IsPositionValid())
    {
        LOG_ERROR("entities.player", "Corpse (guid: {}, owner: {}) is not created, given coordinates are not valid (X: {}, Y: {}, Z: {})",
            GetGUIDLow(), GUID_LOPART(GetOwnerGUID()), posX, posY, posZ);
        return false;
    }

    _gridCoord = Firelands::ComputeGridCoord(GetPositionX(), GetPositionY());
    return true;
}

bool Corpse::IsExpired(time_t t) const
{
    if (m_type == CORPSE_BONES)
        return m_time < t - 60 * MINUTE;
    else
        return m_time < t - 3 * DAY;
}
