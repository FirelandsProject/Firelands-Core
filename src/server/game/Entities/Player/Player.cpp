/*
 * This file is part of the Firelands Core Project. See AUTHORS file for
 * Copyright information
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

#include "Player.h"
#include "AccountMgr.h"
#include "AchievementMgr.h"
#include "Archaeology.h"
#include "ArenaTeam.h"
#include "ArenaTeamMgr.h"
#include "Bag.h"
#include "Battlefield.h"
#include "BattlefieldMgr.h"
#include "BattlefieldTB.h"
#include "BattlefieldWG.h"
#include "Battleground.h"
#include "BattlegroundMgr.h"
#include "BattlegroundScore.h"
#include "CellImpl.h"
#include "Channel.h"
#include "ChannelMgr.h"
#include "CharacterCache.h"
#include "CharacterDatabaseCleaner.h"
#include "CharacterPackets.h"
#include "Chat.h"
#include "CinematicMgr.h"
#include "CombatLogPackets.h"
#include "CombatPackets.h"
#include "Common.h"
#include "ConditionMgr.h"
#include "CreatureAI.h"
#include "DB2Stores.h"
#include "DatabaseEnv.h"
#include "DisableMgr.h"
#include "Formulas.h"
#include "GameClient.h"
#include "GameEventMgr.h"
#include "GameObjectAI.h"
#include "GameTime.h"
#include "GitRevision.h"
#include "GossipDef.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "Group.h"
#include "GroupMgr.h"
#include "Guild.h"
#include "GuildMgr.h"
#include "InstancePackets.h"
#include "InstanceSaveMgr.h"
#include "InstanceScript.h"
#include "ItemPackets.h"
#include "KillRewarder.h"
#include "LFGMgr.h"
#include "Language.h"
#include "Log.h"
#include "LootItemStorage.h"
#include "LootMgr.h"
#include "LootPackets.h"
#include "Mail.h"
#include "MapInstanced.h"
#include "MapManager.h"
#include "MiscPackets.h"
#include "MotionMaster.h"
#include "MovementPacketSender.h"
#include "MovementPackets.h"
#include "MovementStructures.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "Opcodes.h"
#include "OutdoorPvP.h"
#include "OutdoorPvPMgr.h"
#include "Pet.h"
#include "PetitionMgr.h"
#include "PhasingHandler.h"
#include "PoolMgr.h"
#include "QueryCallback.h"
#include "QueryHolder.h"
#include "QuestDef.h"
#include "QuestPackets.h"
#include "QuestPools.h"
#include "Realm.h"
#include "ReputationMgr.h"
#include "SkillDiscovery.h"
#include "SocialMgr.h"
#include "Spell.h"
#include "SpellAuraEffects.h"
#include "SpellAuras.h"
#include "SpellHistory.h"
#include "SpellMgr.h"
#include "SpellPackets.h"
#include "StringConvert.h"
#include "TicketMgr.h"
#include "Tokenize.h"
#include "TradeData.h"
#include "Trainer.h"
#include "Transport.h"
#include "UpdateData.h"
#include "UpdateFieldFlags.h"
#include "UpdateMask.h"
#include "Util.h"
#include "Vehicle.h"
#include "Weather.h"
#include "WeatherMgr.h"
#include "World.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "WorldStateMgr.h"
#include "WorldStatePackets.h"
#include <G3D/g3dmath.h>

#define ZONE_UPDATE_INTERVAL (1 * IN_MILLISECONDS)

// corpse reclaim times
#define DEATH_EXPIRE_STEP (5 * MINUTE)
#define MAX_DEATH_COUNT 3

static uint32 copseReclaimDelay[MAX_DEATH_COUNT] = {30, 60, 120};

uint32 const MasterySpells[MAX_CLASSES] = {
    0,
    87500, // Warrior
    87494, // Paladin
    87493, // Hunter
    87496, // Rogue
    87495, // Priest
    87492, // Death Knight
    87497, // Shaman
    86467, // Mage
    87498, // Warlock
    0,
    87491, // Druid
};

uint64 const MAX_MONEY_AMOUNT = 9999999999ULL;

Player::Player(WorldSession *session) : Unit(true) {
  m_speakTime = 0;
  m_speakCount = 0;

  m_objectType |= TYPEMASK_PLAYER;
  m_objectTypeId = TYPEID_PLAYER;

  m_valuesCount = PLAYER_END;

  m_session = session;

  m_ingametime = 0;
  m_sharedQuestId = 0;
  m_popupQuestId = 0;

  m_ExtraFlags = 0;

  m_spellModTakingSpell = nullptr;
  // m_pad = 0;

  // players always accept
  if (!GetSession()->HasPermission(rbac::RBAC_PERM_CAN_FILTER_WHISPERS))
    SetAcceptWhispers(true);

  m_comboPoints = 0;

  m_foodEmoteTimerCount = 0;
  m_weaponChangeTimer = 0;

  m_zoneUpdateId = uint32(-1);
  m_zoneUpdateTimer = 0;

  m_areaUpdateId = 0;
  m_team = 0;

  m_needsZoneUpdate = false;

  m_nextSave = sWorld->getIntConfig(CONFIG_INTERVAL_SAVE);

  _resurrectionData = nullptr;

  memset(m_items, 0, sizeof(Item *) * PLAYER_SLOTS_COUNT);

  m_social = nullptr;

  // group is initialized in the reference constructor
  SetGroupInvite(nullptr);
  m_groupUpdateMask = 0;
  m_auraRaidUpdateMask = 0;
  m_bPassOnGroupLoot = false;

  duel = nullptr;

  m_GuildIdInvited = 0;
  m_ArenaTeamIdInvited = 0;

  m_atLoginFlags = AT_LOGIN_NONE;

  mSemaphoreTeleport_Near = false;
  mSemaphoreTeleport_Far = false;

  m_DelayedOperations = 0;
  m_bCanDelayTeleport = false;
  m_bHasDelayedTeleport = false;
  m_teleport_options = 0;

  m_trade = nullptr;

  m_cinematic = 0;
  m_movie = 0;

  PlayerTalkClass = new PlayerMenu(GetSession());
  m_currentBuybackSlot = BUYBACK_SLOT_START;

  m_DailyQuestChanged = false;
  m_lastDailyQuestTime = 0;

  // Init rune flags
  for (uint8 i = 0; i < MAX_RUNES; ++i) {
    SetRuneTimer(i, 0xFFFFFFFF);
    SetLastRuneGraceTimer(i, 0);
  }

  for (uint8 i = 0; i < MAX_TIMERS; i++)
    m_MirrorTimer[i] = DISABLED_MIRROR_TIMER;

  m_MirrorTimerFlags = UNDERWATER_NONE;
  m_MirrorTimerFlagsLast = UNDERWATER_NONE;
  m_hostileReferenceCheckTimer = 0;
  m_drunkTimer = 0;
  m_deathTimer = 0;
  m_deathExpireTime = 0;

  m_swingErrorMsg = 0;

  for (uint8 j = 0; j < PLAYER_MAX_BATTLEGROUND_QUEUES; ++j) {
    m_bgBattlegroundQueueID[j].bgQueueTypeId = BATTLEGROUND_QUEUE_NONE;
    m_bgBattlegroundQueueID[j].invitedToInstance = 0;
  }

  m_logintime = GameTime::GetGameTime();
  m_Last_tick = m_logintime;
  m_Played_time[PLAYED_TIME_TOTAL] = 0;
  m_Played_time[PLAYED_TIME_LEVEL] = 0;
  m_WeaponProficiency = 0;
  m_ArmorProficiency = 0;
  m_canParry = false;
  m_canBlock = false;
  m_canTitanGrip = false;
  m_titanGripPenaltySpellId = 0;

  m_temporaryUnsummonedPetNumber = 0;
  // cache for UNIT_CREATED_BY_SPELL to allow
  // returning reagents for temporarily removed pets
  // when dying/logging out
  m_oldpetspell = 0;
  m_lastpetnumber = 0;

  ////////////////////Rest System/////////////////////
  _restTime = 0;
  inn_triggerId = 0;
  m_rest_bonus = 0;
  _restFlagMask = 0;
  ////////////////////Rest System/////////////////////

  m_mailsUpdated = false;
  unReadMails = 0;
  m_nextMailDelivereTime = 0;

  m_itemUpdateQueueBlocked = false;

  m_stableSlots = 0;

  /////////////////// Instance System /////////////////////

  m_HomebindTimer = 0;
  m_InstanceValid = true;
  m_dungeonDifficulty = DUNGEON_DIFFICULTY_NORMAL;
  m_raidDifficulty = RAID_DIFFICULTY_10MAN_NORMAL;
  m_raidMapDifficulty = RAID_DIFFICULTY_10MAN_NORMAL;

  m_lastPotionId = 0;
  _talentMgr = new PlayerTalentInfo();

  for (uint8 i = 0; i < BASEMOD_END; ++i) {
    m_auraBaseFlatMod[i] = 0.0f;
    m_auraBasePctMod[i] = 1.0f;
  }

  for (uint8 i = 0; i < MAX_COMBAT_RATING; i++)
    m_baseRatingValue[i] = 0;

  m_baseSpellPower = 0;
  m_baseManaRegen = 0;
  m_baseHealthRegen = 0;
  m_spellPenetrationItemMod = 0;

  // Honor System
  m_lastHonorUpdateTime = GameTime::GetGameTime();

  m_IsBGRandomWinner = false;

  // Player summoning
  m_summon_expire = 0;

  m_seer = this;

  m_homebindMapId = 0;
  m_homebindAreaId = 0;
  m_homebindX = 0;
  m_homebindY = 0;
  m_homebindZ = 0;

  m_contestedPvPTimer = 0;

  m_declinedname = nullptr;

  m_isActive = true;

  m_runes = nullptr;

  m_lastFallTime = 0;
  m_lastFallZ = 0;

  m_grantableLevels = 0;
  m_fishingSteps = 0;

  m_ControlledByPlayer = true;

  sWorld->IncreasePlayerCount();

  m_ChampioningFaction = 0;

  isDebugAreaTriggers = false;

  m_WeeklyQuestChanged = false;

  m_MonthlyQuestChanged = false;

  m_SeasonalQuestChanged = false;

  m_LFGRewardStatusChanged = false;

  SetPendingBind(0, 0);

  _activeCheats = CHEAT_NONE;
  healthBeforeDuel = 0;
  manaBeforeDuel = 0;

  _maxPersonalArenaRate = 0;

  memset(_voidStorageItems, 0,
         VOID_STORAGE_MAX_SLOT * sizeof(VoidStorageItem *));

  _cinematicMgr = new CinematicMgr(this);

  m_achievementMgr = new AchievementMgr<Player>(this);
  m_reputationMgr = new ReputationMgr(this);
  _hasValidLFGLeavePoint = false;
  _archaeology = new Archaeology(this);
  m_petScalingSynchTimer.Reset(1000);
  m_groupUpdateTimer.Reset(5000);
}

Player::~Player() {
  // it must be unloaded already in PlayerLogout and accessed only for logged
  // player
  // m_social = nullptr;

  // Note: buy back item already deleted from DB when player was saved
  for (uint8 i = 0; i < PLAYER_SLOTS_COUNT; ++i)
    delete m_items[i];

  delete _talentMgr;

  // all mailed items should be deleted, also all mail should be deallocated
  for (PlayerMails::iterator itr = m_mail.begin(); itr != m_mail.end(); ++itr)
    delete *itr;

  for (ItemMap::iterator iter = mMitems.begin(); iter != mMitems.end(); ++iter)
    delete iter->second; // if item is duplicated... then server may crash ...
                         // but that item should be deallocated

  delete PlayerTalkClass;

  for (size_t x = 0; x < ItemSetEff.size(); x++)
    delete ItemSetEff[x];

  delete m_declinedname;
  delete m_runes;
  delete m_achievementMgr;
  delete m_reputationMgr;
  delete _cinematicMgr;
  delete _archaeology;

  for (uint8 i = 0; i < VOID_STORAGE_MAX_SLOT; ++i)
    delete _voidStorageItems[i];

  for (uint8 i = 0; i < PlayerPetDataStore.size(); i++)
    delete PlayerPetDataStore[i];

  ClearResurrectRequestData();

  sWorld->DecreasePlayerCount();
}

void Player::CleanupsBeforeDelete(bool finalCleanup) {
  TradeCancel(false);
  DuelComplete(DUEL_INTERRUPTED);

  Unit::CleanupsBeforeDelete(finalCleanup);

  // clean up player-instance binds, may unload some instance saves
  for (uint8 i = 0; i < MAX_DIFFICULTY; ++i) {
    for (BoundInstancesMap::iterator itr = m_boundInstances[i].begin();
         itr != m_boundInstances[i].end(); ++itr) {
      itr->second.save->RemovePlayer(this);
    }
  }
}

bool Player::Create(ObjectGuid::LowType guidlow,
                    CharacterCreateInfo *createInfo) {
  // FIXME: outfitId not used in player creating
  /// @todo need more checks against packet modifications

  Object::_Create(guidlow, 0, HighGuid::Player);

  m_name = createInfo->Name;

  PlayerInfo const *info =
      sObjectMgr->GetPlayerInfo(createInfo->Race, createInfo->Class);
  if (!info) {
    LOG_INFO("entities.player.cheat",
             "Player::Create: Possible hacking attempt: Account %u tried to "
             "create a character named '%s' with an invalid race/class pair "
             "(%u/%u) - refusing to do so.",
             GetSession()->GetAccountId(), m_name.c_str(), createInfo->Race,
             createInfo->Class);
    return false;
  }

  for (uint8 i = 0; i < PLAYER_SLOTS_COUNT; i++)
    m_items[i] = nullptr;

  Relocate(info->positionX, info->positionY, info->positionZ,
           info->orientation);

  ChrClassesEntry const *cEntry =
      sChrClassesStore.LookupEntry(createInfo->Class);
  if (!cEntry) {
    LOG_INFO("entities.player.cheat",
             "Player::Create: Possible hacking attempt: Account %u tried to "
             "create a character named '%s' with an invalid character class "
             "(%u) - refusing to do so (wrong DBC-files?)",
             GetSession()->GetAccountId(), m_name.c_str(), createInfo->Class);
    return false;
  }

  SetMap(sMapMgr->CreateMap(info->mapId, this));

  uint8 powertype = cEntry->DisplayPower;

  SetObjectScale(1.0f);

  SetFactionForRace(createInfo->Race);

  if (!IsValidGender(createInfo->Gender)) {
    LOG_INFO("entities.player.cheat",
             "Player::Create: Possible hacking attempt: Account %u tried to "
             "create a character named '%s' with an invalid gender (%u) - "
             "refusing to do so",
             GetSession()->GetAccountId(), m_name.c_str(), createInfo->Gender);
    return false;
  }

  if (!ValidateAppearance(createInfo->Race, createInfo->Class,
                          createInfo->Gender, createInfo->HairStyle,
                          createInfo->HairColor, createInfo->Face,
                          createInfo->FacialHair, createInfo->Skin, true)) {
    LOG_INFO("entities.player.cheat",
             "Player::Create: Possible hacking attempt: Account %u tried to "
             "create a character named '%s' with invalid appearance attributes "
             "- refusing to do so",
             GetSession()->GetAccountId(), m_name.c_str());
    return false;
  }

  SetByteValue(UNIT_FIELD_BYTES_0, UNIT_BYTES_0_OFFSET_RACE, createInfo->Race);
  SetByteValue(UNIT_FIELD_BYTES_0, UNIT_BYTES_0_OFFSET_CLASS,
               createInfo->Class);
  SetByteValue(UNIT_FIELD_BYTES_0, UNIT_BYTES_0_OFFSET_GENDER,
               createInfo->Gender);
  SetByteValue(UNIT_FIELD_BYTES_0, UNIT_BYTES_0_OFFSET_POWER_TYPE, powertype);
  InitDisplayIds();
  if (sWorld->getIntConfig(CONFIG_GAME_TYPE) == REALM_TYPE_PVP ||
      sWorld->getIntConfig(CONFIG_GAME_TYPE) == REALM_TYPE_RPPVP) {
    SetByteFlag(UNIT_FIELD_BYTES_2, UNIT_BYTES_2_OFFSET_PVP_FLAG,
                UNIT_BYTE2_FLAG_PVP);
    SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED);
  }

  SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_REGENERATE_POWER);
  SetFloatValue(UNIT_FIELD_HOVERHEIGHT, 1.0f); // default for players in 3.0.3

  SetInt32Value(PLAYER_FIELD_WATCHED_FACTION_INDEX,
                uint32(-1)); // -1 is default value

  SetByteValue(PLAYER_BYTES, PLAYER_BYTES_OFFSET_SKIN_ID, createInfo->Skin);
  SetByteValue(PLAYER_BYTES, PLAYER_BYTES_OFFSET_FACE_ID, createInfo->Face);
  SetByteValue(PLAYER_BYTES, PLAYER_BYTES_OFFSET_HAIR_STYLE_ID,
               createInfo->HairStyle);
  SetByteValue(PLAYER_BYTES, PLAYER_BYTES_OFFSET_HAIR_COLOR_ID,
               createInfo->HairColor);
  SetByteValue(PLAYER_BYTES_2, PLAYER_BYTES_2_OFFSET_FACIAL_STYLE,
               createInfo->FacialHair);
  SetByteValue(
      PLAYER_BYTES_2, PLAYER_BYTES_2_OFFSET_REST_STATE,
      (GetSession()->IsARecruiter() || GetSession()->GetRecruiterId() != 0)
          ? REST_STATE_RAF_LINKED
          : REST_STATE_NOT_RAF_LINKED);
  SetByteValue(PLAYER_BYTES_3, PLAYER_BYTES_3_OFFSET_GENDER,
               createInfo->Gender);
  SetByteValue(PLAYER_BYTES_3, PLAYER_BYTES_3_OFFSET_ARENA_FACTION, 0);

  SetUInt64Value(OBJECT_FIELD_DATA, 0);
  SetUInt32Value(PLAYER_GUILDRANK, 0);
  SetGuildLevel(0);
  SetUInt32Value(PLAYER_GUILD_TIMESTAMP, 0);

  for (int i = 0; i < KNOWN_TITLES_SIZE; ++i)
    SetUInt64Value(PLAYER__FIELD_KNOWN_TITLES + i, 0); // 0=disabled
  SetUInt32Value(PLAYER_CHOSEN_TITLE, 0);

  SetUInt32Value(PLAYER_FIELD_KILLS, 0);
  SetUInt32Value(PLAYER_FIELD_LIFETIME_HONORABLE_KILLS, 0);

  // set starting level
  uint32 start_level =
      getClass() != CLASS_DEATH_KNIGHT
          ? sWorld->getIntConfig(CONFIG_START_PLAYER_LEVEL)
          : sWorld->getIntConfig(CONFIG_START_DEATH_KNIGHT_PLAYER_LEVEL);

  if (m_session->HasPermission(rbac::RBAC_PERM_USE_START_GM_LEVEL)) {
    uint32 gm_level = sWorld->getIntConfig(CONFIG_START_GM_LEVEL);
    if (gm_level > start_level)
      start_level = gm_level;
  }

  SetUInt32Value(UNIT_FIELD_LEVEL, start_level);

  InitRunes();

  SetUInt64Value(PLAYER_FIELD_COINAGE,
                 sWorld->getIntConfig(CONFIG_START_PLAYER_MONEY));
  SetCurrency(CURRENCY_TYPE_HONOR_POINTS,
              sWorld->getIntConfig(CONFIG_CURRENCY_START_HONOR_POINTS));
  SetCurrency(CURRENCY_TYPE_JUSTICE_POINTS,
              sWorld->getIntConfig(CONFIG_CURRENCY_START_JUSTICE_POINTS));
  SetCurrency(CURRENCY_TYPE_CONQUEST_POINTS,
              sWorld->getIntConfig(CONFIG_CURRENCY_START_CONQUEST_POINTS));

  // Played time
  m_Last_tick = GameTime::GetGameTime();
  m_Played_time[PLAYED_TIME_TOTAL] = 0;
  m_Played_time[PLAYED_TIME_LEVEL] = 0;

  // base stats and related field values
  InitStatsForLevel();
  InitTaxiNodesForLevel();
  InitGlyphsForLevel();
  InitTalentForLevel();
  InitializeSkillFields();
  InitPrimaryProfessions(); // to max set before any spell added

  // apply original stats mods before spell loading or item equipment that call
  // before equip _RemoveStatsMods()
  UpdateMaxHealth(); // Update max Health (for add bonus from stamina)
  SetFullHealth();
  SetFullPower(POWER_MANA);

  // original spells
  LearnDefaultSkills();
  LearnCustomSpells();

  // original action bar
  for (PlayerCreateInfoActions::const_iterator action_itr =
           info->action.begin();
       action_itr != info->action.end(); ++action_itr)
    addActionButton(action_itr->button, action_itr->action, action_itr->type);

  // original items
  if (CharStartOutfitEntry const *oEntry = sDBCManager.GetCharStartOutfitEntry(
          createInfo->Race, createInfo->Class, createInfo->Gender)) {
    for (int j = 0; j < MAX_OUTFIT_ITEMS; ++j) {
      if (oEntry->ItemID[j] <= 0)
        continue;

      uint32 itemId = oEntry->ItemID[j];

      // just skip, reported in ObjectMgr::LoadItemTemplates
      ItemTemplate const *iProto = sObjectMgr->GetItemTemplate(itemId);
      if (!iProto)
        continue;

      // BuyCount by default
      uint32 count = iProto->GetBuyCount();

      // special amount for food/drink
      if (iProto->GetClass() == ITEM_CLASS_CONSUMABLE &&
          iProto->GetSubClass() == ITEM_SUBCLASS_FOOD) {
        if (iProto->Effects.size() >= 1) {
          switch (iProto->Effects[0].Category) {
          case SPELL_CATEGORY_FOOD: // food
            count = getClass() == CLASS_DEATH_KNIGHT ? 10 : 4;
            break;
          case SPELL_CATEGORY_DRINK: // drink
            count = 2;
            break;
          }
        }
        if (iProto->GetMaxStackSize() < count)
          count = iProto->GetMaxStackSize();
      }
      StoreNewItemInBestSlots(itemId, count);
    }
  }

  for (PlayerCreateInfoItems::const_iterator item_id_itr = info->item.begin();
       item_id_itr != info->item.end(); ++item_id_itr)
    StoreNewItemInBestSlots(item_id_itr->item_id, item_id_itr->item_amount);

  // bags and main-hand weapon must equipped at this moment
  // now second pass for not equipped (offhand weapon/shield if it attempt
  // equipped before main-hand weapon) or ammo not equipped in special bag
  for (uint8 i = INVENTORY_SLOT_ITEM_START; i < INVENTORY_SLOT_ITEM_END; i++) {
    if (Item *pItem = GetItemByPos(INVENTORY_SLOT_BAG_0, i)) {
      uint16 eDest;
      // equip offhand weapon/shield if it attempt equipped before main-hand
      // weapon
      InventoryResult msg = CanEquipItem(NULL_SLOT, eDest, pItem, false);
      if (msg == EQUIP_ERR_OK) {
        RemoveItem(INVENTORY_SLOT_BAG_0, i, true);
        EquipItem(eDest, pItem, true);
      }
      // move other items to more appropriate slots
      else {
        ItemPosCountVec sDest;
        msg = CanStoreItem(NULL_BAG, NULL_SLOT, sDest, pItem, false);
        if (msg == EQUIP_ERR_OK) {
          RemoveItem(INVENTORY_SLOT_BAG_0, i, true);
          StoreItem(sDest, pItem, true);
        }
      }
    }
  }
  // all item positions resolved

  GetThreatManager().Initialize();

  return true;
}

bool Player::StoreNewItemInBestSlots(uint32 titem_id, uint32 titem_amount) {
  LOG_DEBUG("entities.player.items",
            "Player::StoreNewItemInBestSlots: Player '%s' (%s) creates initial "
            "item (ItemID: %u, Count: %u)",
            GetName().c_str(), GetGUID().ToString().c_str(), titem_id,
            titem_amount);

  // attempt equip by one
  while (titem_amount > 0) {
    uint16 eDest;
    InventoryResult msg = CanEquipNewItem(NULL_SLOT, eDest, titem_id, false);
    if (msg != EQUIP_ERR_OK)
      break;

    EquipNewItem(eDest, titem_id, true);
    AutoUnequipOffhandIfNeed();
    --titem_amount;
  }

  if (titem_amount == 0)
    return true; // equipped

  // attempt store
  ItemPosCountVec sDest;
  // store in main bag to simplify second pass (special bags can be not equipped
  // yet at this moment)
  InventoryResult msg = CanStoreNewItem(INVENTORY_SLOT_BAG_0, NULL_SLOT, sDest,
                                        titem_id, titem_amount);
  if (msg == EQUIP_ERR_OK) {
    StoreNewItem(sDest, titem_id, true, GenerateItemRandomPropertyId(titem_id));
    return true; // stored
  }

  // item can't be added
  LOG_ERROR(
      "entities.player.items",
      "Player::StoreNewItemInBestSlots: Player '%s' (%s) can't equip or store "
      "initial item (ItemID: %u, Race: %u, Class: %u, InventoryResult: %u)",
      GetName().c_str(), GetGUID().ToString().c_str(), titem_id, getRace(),
      getClass(), msg);
  return false;
}

void Player::SendMirrorTimer(MirrorTimerType Type, uint32 MaxValue,
                             uint32 CurrentValue, int32 Regen) {
  if (int(MaxValue) == DISABLED_MIRROR_TIMER) {
    if (int(CurrentValue) != DISABLED_MIRROR_TIMER)
      StopMirrorTimer(Type);
    return;
  }

  SendDirectMessage(WorldPackets::Misc::StartMirrorTimer(
                        Type, CurrentValue, MaxValue, Regen, 0, false)
                        .Write());
}

void Player::StopMirrorTimer(MirrorTimerType Type) {
  m_MirrorTimer[Type] = DISABLED_MIRROR_TIMER;
  SendDirectMessage(WorldPackets::Misc::StopMirrorTimer(Type).Write());
}

bool Player::IsImmuneToEnvironmentalDamage() const {
  // check for GM and death state included in isAttackableByAOE
  return !isTargetableForAttack(false);
}

uint32 Player::EnvironmentalDamage(EnviromentalDamage type, uint32 damage) {
  if (IsImmuneToEnvironmentalDamage())
    return 0;

  // Absorb, resist some environmental damage type
  uint32 absorb = 0;
  uint32 resist = 0;
  switch (type) {
  case DAMAGE_LAVA:
  case DAMAGE_SLIME: {
    DamageInfo dmgInfo(this, this, damage, nullptr,
                       type == DAMAGE_LAVA ? SPELL_SCHOOL_MASK_FIRE
                                           : SPELL_SCHOOL_MASK_NATURE,
                       DIRECT_DAMAGE, BASE_ATTACK);
    CalcAbsorbResist(dmgInfo);
    absorb = dmgInfo.GetAbsorb();
    resist = dmgInfo.GetResist();
    damage = dmgInfo.GetDamage();
    break;
  }
  default:
    break;
  }

  DealDamageMods(this, damage, &absorb);

  WorldPackets::CombatLog::EnvironmentalDamageLog environmentalDamageLog;
  environmentalDamageLog.Victim = GetGUID();
  environmentalDamageLog.Type =
      type != DAMAGE_FALL_TO_VOID ? type : DAMAGE_FALL;
  environmentalDamageLog.Amount = damage;
  environmentalDamageLog.Absorbed = absorb;
  environmentalDamageLog.Resisted = resist;
  SendMessageToSet(environmentalDamageLog.Write(), true);

  uint32 final_damage = DealDamage(this, damage, nullptr, SELF_DAMAGE,
                                   SPELL_SCHOOL_MASK_NORMAL, nullptr, false);

  if (!IsAlive()) {
    if (type == DAMAGE_FALL) // DealDamage does not apply item durability loss
                             // from self-induced damage.
    {
      LOG_DEBUG("entities.player",
                "Player::EnvironmentalDamage: Player '%s' (%s) fall to death, "
                "losing 10%% durability",
                GetName().c_str(), GetGUID().ToString().c_str());
      DurabilityLossAll(0.10f, false);
      // durability lost message
      SendDurabilityLoss(this, 10);
    }

    UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_DEATHS_FROM, 1, type);
  }

  return final_damage;
}

int32 Player::getMaxTimer(MirrorTimerType timer) const {
  switch (timer) {
  case FATIGUE_TIMER:
    return MINUTE * IN_MILLISECONDS;
  case BREATH_TIMER: {
    if (!IsAlive() || HasAuraType(SPELL_AURA_WATER_BREATHING) ||
        GetSession()->GetSecurity() >=
            AccountTypes(sWorld->getIntConfig(CONFIG_DISABLE_BREATHING)))
      return DISABLED_MIRROR_TIMER;
    int32 UnderWaterTime = 3 * MINUTE * IN_MILLISECONDS;
    AuraEffectList const &mModWaterBreathing =
        GetAuraEffectsByType(SPELL_AURA_MOD_WATER_BREATHING);
    for (AuraEffectList::const_iterator i = mModWaterBreathing.begin();
         i != mModWaterBreathing.end(); ++i)
      AddPct(UnderWaterTime, (*i)->GetAmount());
    return UnderWaterTime;
  }
  case FIRE_TIMER: {
    if (!IsAlive())
      return DISABLED_MIRROR_TIMER;
    return 1 * IN_MILLISECONDS;
  }
  default:
    return 0;
  }
}

void Player::UpdateMirrorTimers() {
  // Desync flags for update on next HandleDrowning
  if (m_MirrorTimerFlags)
    m_MirrorTimerFlagsLast = ~m_MirrorTimerFlags;
}

void Player::StopMirrorTimers() {
  StopMirrorTimer(FATIGUE_TIMER);
  StopMirrorTimer(BREATH_TIMER);
  StopMirrorTimer(FIRE_TIMER);
}

bool Player::IsMirrorTimerActive(MirrorTimerType type) const {
  return m_MirrorTimer[type] == getMaxTimer(type);
}

void Player::HandleDrowning(uint32 time_diff) {
  if (!m_MirrorTimerFlags)
    return;

  // In water
  if (m_MirrorTimerFlags & UNDERWATER_INWATER) {
    // Breath timer not activated - activate it
    if (m_MirrorTimer[BREATH_TIMER] == DISABLED_MIRROR_TIMER) {
      m_MirrorTimer[BREATH_TIMER] = getMaxTimer(BREATH_TIMER);
      SendMirrorTimer(BREATH_TIMER, m_MirrorTimer[BREATH_TIMER],
                      m_MirrorTimer[BREATH_TIMER], -1);
    } else // If activated - do tick
    {
      m_MirrorTimer[BREATH_TIMER] -= time_diff;
      // Timer limit - need deal damage
      if (m_MirrorTimer[BREATH_TIMER] < 0) {
        m_MirrorTimer[BREATH_TIMER] += 1 * IN_MILLISECONDS;
        // Calculate and deal damage
        /// @todo Check this formula
        uint32 damage = GetMaxHealth() / 5 + urand(0, getLevel() - 1);
        EnvironmentalDamage(DAMAGE_DROWNING, damage);
      } else if (!(m_MirrorTimerFlagsLast &
                   UNDERWATER_INWATER)) // Update time in client if need
        SendMirrorTimer(BREATH_TIMER, getMaxTimer(BREATH_TIMER),
                        m_MirrorTimer[BREATH_TIMER], -1);
    }
  } else if (m_MirrorTimer[BREATH_TIMER] !=
             DISABLED_MIRROR_TIMER) // Regen timer
  {
    int32 UnderWaterTime = getMaxTimer(BREATH_TIMER);
    // Need breath regen
    m_MirrorTimer[BREATH_TIMER] += 10 * time_diff;
    if (m_MirrorTimer[BREATH_TIMER] >= UnderWaterTime || !IsAlive())
      StopMirrorTimer(BREATH_TIMER);
    else if (m_MirrorTimerFlagsLast & UNDERWATER_INWATER)
      SendMirrorTimer(BREATH_TIMER, UnderWaterTime, m_MirrorTimer[BREATH_TIMER],
                      10);
  }

  // In dark water
  if (m_MirrorTimerFlags & UNDERWATER_INDARKWATER) {
    // Fatigue timer not activated - activate it
    if (m_MirrorTimer[FATIGUE_TIMER] == DISABLED_MIRROR_TIMER) {
      m_MirrorTimer[FATIGUE_TIMER] = getMaxTimer(FATIGUE_TIMER);
      SendMirrorTimer(FATIGUE_TIMER, m_MirrorTimer[FATIGUE_TIMER],
                      m_MirrorTimer[FATIGUE_TIMER], -1);
    } else {
      m_MirrorTimer[FATIGUE_TIMER] -= time_diff;
      // Timer limit - need deal damage or teleport ghost to graveyard
      if (m_MirrorTimer[FATIGUE_TIMER] < 0) {
        m_MirrorTimer[FATIGUE_TIMER] += 1 * IN_MILLISECONDS;
        if (IsAlive()) // Calculate and deal damage
        {
          uint32 damage = GetMaxHealth() / 5 + urand(0, getLevel() - 1);
          EnvironmentalDamage(DAMAGE_EXHAUSTED, damage);
        } else if (HasFlag(PLAYER_FLAGS,
                           PLAYER_FLAGS_GHOST)) // Teleport ghost to graveyard
          RepopAtGraveyard();
      } else if (!(m_MirrorTimerFlagsLast & UNDERWATER_INDARKWATER))
        SendMirrorTimer(FATIGUE_TIMER, getMaxTimer(FATIGUE_TIMER),
                        m_MirrorTimer[FATIGUE_TIMER], -1);
    }
  } else if (m_MirrorTimer[FATIGUE_TIMER] !=
             DISABLED_MIRROR_TIMER) // Regen timer
  {
    int32 DarkWaterTime = getMaxTimer(FATIGUE_TIMER);
    m_MirrorTimer[FATIGUE_TIMER] += 10 * time_diff;
    if (m_MirrorTimer[FATIGUE_TIMER] >= DarkWaterTime || !IsAlive())
      StopMirrorTimer(FATIGUE_TIMER);
    else if (m_MirrorTimerFlagsLast & UNDERWATER_INDARKWATER)
      SendMirrorTimer(FATIGUE_TIMER, DarkWaterTime,
                      m_MirrorTimer[FATIGUE_TIMER], 10);
  }

  if (m_MirrorTimerFlags & (UNDERWATER_INLAVA /*| UNDERWATER_INSLIME*/) &&
      !(_lastLiquid && _lastLiquid->SpellID)) {
    // Breath timer not activated - activate it
    if (m_MirrorTimer[FIRE_TIMER] == DISABLED_MIRROR_TIMER)
      m_MirrorTimer[FIRE_TIMER] = getMaxTimer(FIRE_TIMER);
    else {
      m_MirrorTimer[FIRE_TIMER] -= time_diff;
      if (m_MirrorTimer[FIRE_TIMER] < 0) {
        m_MirrorTimer[FIRE_TIMER] += 1 * IN_MILLISECONDS;
        // Calculate and deal damage
        /// @todo Check this formula
        uint32 damage = urand(600, 700);
        if (m_MirrorTimerFlags & UNDERWATER_INLAVA)
          EnvironmentalDamage(DAMAGE_LAVA, damage);
        // need to skip Slime damage in Undercity,
        // maybe someone can find better way to handle environmental damage
        // else if (m_zoneUpdateId != 1497)
        //    EnvironmentalDamage(DAMAGE_SLIME, damage);
      }
    }
  } else
    m_MirrorTimer[FIRE_TIMER] = DISABLED_MIRROR_TIMER;

  // Recheck timers flag
  m_MirrorTimerFlags &= ~UNDERWATER_EXIST_TIMERS;
  for (uint8 i = 0; i < MAX_TIMERS; ++i)
    if (m_MirrorTimer[i] != DISABLED_MIRROR_TIMER) {
      m_MirrorTimerFlags |= UNDERWATER_EXIST_TIMERS;
      break;
    }
  m_MirrorTimerFlagsLast = m_MirrorTimerFlags;
}

/// The player sobers by 1% every 9 seconds
void Player::HandleSobering() {
  m_drunkTimer = 0;

  uint8 currentDrunkValue = GetDrunkValue();
  uint8 drunk = currentDrunkValue ? --currentDrunkValue : 0;
  SetDrunkValue(drunk);
}

DrunkenState Player::GetDrunkenstateByValue(uint8 value) {
  if (value >= 90)
    return DRUNKEN_SMASHED;
  if (value >= 50)
    return DRUNKEN_DRUNK;
  if (value)
    return DRUNKEN_TIPSY;
  return DRUNKEN_SOBER;
}

void Player::SetDrunkValue(uint8 newDrunkValue, uint32 itemId /*= 0*/) {
  bool isSobering = newDrunkValue < GetDrunkValue();
  uint32 oldDrunkenState = Player::GetDrunkenstateByValue(GetDrunkValue());
  if (newDrunkValue > 100)
    newDrunkValue = 100;

  // select drunk percent or total SPELL_AURA_MOD_FAKE_INEBRIATE amount,
  // whichever is higher for visibility updates
  int32 drunkPercent = std::max<int32>(
      newDrunkValue, GetTotalAuraModifier(SPELL_AURA_MOD_FAKE_INEBRIATE));
  if (drunkPercent) {
    m_invisibilityDetect.AddFlag(INVISIBILITY_DRUNK);
    m_invisibilityDetect.SetValue(INVISIBILITY_DRUNK, drunkPercent);
  } else if (!HasAuraType(SPELL_AURA_MOD_FAKE_INEBRIATE) && !newDrunkValue)
    m_invisibilityDetect.DelFlag(INVISIBILITY_DRUNK);

  uint32 newDrunkenState = Player::GetDrunkenstateByValue(newDrunkValue);
  SetByteValue(PLAYER_BYTES_3, PLAYER_BYTES_3_OFFSET_INEBRIATION,
               newDrunkValue);
  UpdateObjectVisibility();

  if (!isSobering)
    m_drunkTimer = 0; // reset sobering timer

  if (newDrunkenState == oldDrunkenState)
    return;

  WorldPackets::Misc::CrossedInebriationThreshold data;
  data.Guid = GetGUID();
  data.Threshold = newDrunkenState;
  data.ItemID = itemId;

  SendMessageToSet(data.Write(), true);
}

void Player::Update(uint32 p_time) {
  if (!IsInWorld())
    return;

  // undelivered mail
  if (m_nextMailDelivereTime &&
      m_nextMailDelivereTime <= GameTime::GetGameTime()) {
    SendNewMail();
    ++unReadMails;

    // It will be recalculate at mailbox open (for unReadMails important non-0
    // until mailbox open, it also will be recalculated)
    m_nextMailDelivereTime = 0;
  }

  // Update cinematic location, if 500ms have passed and we're doing a cinematic
  // now.
  _cinematicMgr->m_cinematicDiff += p_time;
  if (_cinematicMgr->m_activeCinematicCameraId != 0 &&
      GetMSTimeDiffToNow(_cinematicMgr->m_lastCinematicCheck) >
          CINEMATIC_UPDATEDIFF) {
    _cinematicMgr->m_lastCinematicCheck = GameTime::GetGameTimeMS();
    _cinematicMgr->UpdateCinematicLocation(p_time);
  }

  // used to implement delayed far teleport
  SetCanDelayTeleport(true);
  Unit::Update(p_time);
  SetCanDelayTeleport(false);

  time_t now = GameTime::GetGameTime();

  UpdatePvPFlag(now);

  UpdateContestedPvP(p_time);

  UpdateDuelFlag(now);

  CheckDuelDistance(now);

  UpdateAfkReport(now);

  Unit::AIUpdateTick(p_time);

  // Update items that have just a limited lifetime
  if (now > m_Last_tick)
    UpdateItemDuration(uint32(now - m_Last_tick));

  // check every second
  if (now > m_Last_tick + 1)
    UpdateSoulboundTradeItems();

  // If mute expired, remove it from the DB
  if (GetSession()->m_muteTime && GetSession()->m_muteTime < now) {
    GetSession()->m_muteTime = 0;
    LoginDatabasePreparedStatement *stmt =
        LoginDatabase.GetPreparedStatement(LOGIN_UPD_MUTE_TIME);
    stmt->SetData(0, 0); // Set the mute time to 0
    stmt->SetData(1, "");
    stmt->SetData(2, "");
    stmt->SetData(3, GetSession()->GetAccountId());
    LoginDatabase.Execute(stmt);
  }

  if (!m_timedquests.empty()) {
    QuestSet::iterator iter = m_timedquests.begin();
    while (iter != m_timedquests.end()) {
      QuestStatusData &q_status = m_QuestStatus[*iter];
      if (q_status.Timer <= p_time) {
        uint32 quest_id = *iter;
        ++iter; // current iter will be removed in FailQuest
        FailQuest(quest_id);
      } else {
        q_status.Timer -= p_time;
        m_QuestStatusSave[*iter] = QUEST_DEFAULT_SAVE_TYPE;
        ++iter;
      }
    }
  }

  m_achievementMgr->UpdateTimedAchievements(p_time);

  if (HasUnitState(UNIT_STATE_MELEE_ATTACKING) &&
      !HasUnitState(UNIT_STATE_CASTING)) {
    if (Unit *victim = GetVictim()) {
      // default combat reach 10
      /// @todo add weapon, skill check

      if (isAttackReady(BASE_ATTACK)) {
        if (!IsWithinMeleeRange(victim)) {
          setAttackTimer(BASE_ATTACK, 100);
          if (m_swingErrorMsg != 1) // send single time (client auto repeat)
          {
            SendAttackSwingNotInRange();
            m_swingErrorMsg = 1;
          }
        }
        // 120 degrees of radiant range
        else if (!IsWithinBoundaryRadius(victim) &&
                 !HasInArc(2 * float(M_PI) / 3, victim)) {
          setAttackTimer(BASE_ATTACK, 100);
          if (m_swingErrorMsg != 2) // send single time (client auto repeat)
          {
            SendAttackSwingBadFacingAttack();
            m_swingErrorMsg = 2;
          }
        } else {
          m_swingErrorMsg = 0; // reset swing error state

          // prevent base and off attack in same time, delay attack at 0.2 sec
          if (haveOffhandWeapon())
            if (getAttackTimer(OFF_ATTACK) < ATTACK_DISPLAY_DELAY)
              setAttackTimer(OFF_ATTACK, ATTACK_DISPLAY_DELAY);

          // do attack
          AttackerStateUpdate(victim, BASE_ATTACK);
          resetAttackTimer(BASE_ATTACK);
        }
      }

      if (haveOffhandWeapon() && isAttackReady(OFF_ATTACK)) {
        if (!IsWithinMeleeRange(victim))
          setAttackTimer(OFF_ATTACK, 100);
        else if (!IsWithinBoundaryRadius(victim) &&
                 !HasInArc(2 * float(M_PI) / 3, victim)) {
          setAttackTimer(OFF_ATTACK, 100);
        } else {
          // prevent base and off attack in same time, delay attack at 0.2 sec
          if (getAttackTimer(BASE_ATTACK) < ATTACK_DISPLAY_DELAY)
            setAttackTimer(BASE_ATTACK, ATTACK_DISPLAY_DELAY);

          // do attack
          AttackerStateUpdate(victim, OFF_ATTACK);
          resetAttackTimer(OFF_ATTACK);
        }
      }

      /*Unit* owner = victim->GetOwner();
      Unit* u = owner ? owner : victim;
      if (u->IsPvP() && (!duel || duel->opponent != u))
      {
          UpdatePvP(true);
          RemoveAurasWithInterruptFlags(SpellAuraInterruptFlags::PvPActive);
      }*/
    }
  }

  if (HasFlag(PLAYER_FLAGS, PLAYER_FLAGS_RESTING)) {
    if (roll_chance_i(3) && _restTime > 0) // freeze update
    {
      time_t currTime = GameTime::GetGameTime();
      time_t timeDiff = currTime - _restTime;
      if (timeDiff >= 10) // freeze update
      {
        _restTime = currTime;

        float bubble = 0.125f * sWorld->getRate(RATE_REST_INGAME);
        float extraPerSec =
            ((float)GetUInt32Value(PLAYER_NEXT_LEVEL_XP) / 72000.0f) * bubble;

        // speed collect rest bonus (section/in hour)
        float currRestBonus = GetRestBonus();
        SetRestBonus(currRestBonus + timeDiff * extraPerSec);
      }
    }
  }

  if (m_weaponChangeTimer > 0) {
    if (p_time >= m_weaponChangeTimer)
      m_weaponChangeTimer = 0;
    else
      m_weaponChangeTimer -= p_time;
  }

  if (m_zoneUpdateTimer > 0) {
    if (p_time >= m_zoneUpdateTimer) {
      // On zone update tick check if we are still in an inn if we are supposed
      // to be in one
      if (HasRestFlag(REST_FLAG_IN_TAVERN)) {
        AreaTriggerEntry const *atEntry =
            sAreaTriggerStore.LookupEntry(GetInnTriggerId());
        if (!atEntry || !IsInAreaTriggerRadius(atEntry))
          RemoveRestFlag(REST_FLAG_IN_TAVERN);
      }

      uint32 newzone, newarea;
      GetZoneAndAreaId(newzone, newarea);

      if (m_zoneUpdateId != newzone)
        UpdateZone(newzone, newarea); // also update area
      else {
        // use area updates as well
        // needed for free far all arenas for example
        if (m_areaUpdateId != newarea)
          UpdateArea(newarea);

        m_zoneUpdateTimer = ZONE_UPDATE_INTERVAL;
      }
    } else
      m_zoneUpdateTimer -= p_time;
  }

  // Power regeneration update
  _powerUpdateTimer -= p_time;
  if (IsAlive())
    RegenerateAll(p_time);

  // Reset the power update timer after regeneration happened
  if (_powerUpdateTimer <= 0)
    _powerUpdateTimer = GetPowerUpdateInterval();

  if (m_deathState == JUST_DIED)
    KillPlayer();

  if (m_nextSave > 0) {
    if (p_time >= m_nextSave) {
      // m_nextSave reset in SaveToDB call
      SaveToDB();
      LOG_DEBUG("entities.player", "Player::Update: Player '%s' (%s) saved",
                GetName().c_str(), GetGUID().ToString().c_str());
      ;
    } else
      m_nextSave -= p_time;
  }

  // Handle Water/drowning
  HandleDrowning(p_time);

  // Played time
  if (now > m_Last_tick) {
    uint32 elapsed = uint32(now - m_Last_tick);
    m_Played_time[PLAYED_TIME_TOTAL] += elapsed; // Total played time
    m_Played_time[PLAYED_TIME_LEVEL] += elapsed; // Level played time
    m_Last_tick = now;
  }

  if (GetDrunkValue()) {
    m_drunkTimer += p_time;
    if (m_drunkTimer > 9 * IN_MILLISECONDS)
      HandleSobering();
  }

  if (HasPendingBind()) {
    if (_pendingBindTimer <= p_time) {
      // Player left the instance
      if (_pendingBindId == GetInstanceId())
        BindToInstance();
      SetPendingBind(0, 0);
    } else
      _pendingBindTimer -= p_time;
  }

  // not auto-free ghost from body in instances
  if (m_deathTimer > 0 && !GetMap()->Instanceable() &&
      !HasAuraType(SPELL_AURA_PREVENT_RESURRECTION)) {
    if (p_time >= m_deathTimer) {
      m_deathTimer = 0;
      BuildPlayerRepop();
      RepopAtGraveyard();
    } else
      m_deathTimer -= p_time;
  }

  UpdateEnchantTime(p_time);
  UpdateHomebindTime(p_time);

  if (!_instanceResetTimes.empty()) {
    for (InstanceTimeMap::iterator itr = _instanceResetTimes.begin();
         itr != _instanceResetTimes.end();) {
      if (itr->second < now)
        _instanceResetTimes.erase(itr++);
      else
        ++itr;
    }
  }

  if (getClass() == CLASS_DEATH_KNIGHT) {
    // Update rune timers
    for (uint8 i = 0; i < MAX_RUNES; ++i) {
      uint32 timer = GetRuneTimer(i);

      // Don't update timer if rune is disabled
      if (GetRuneCooldown(i))
        continue;

      // Timer has began
      if (timer < 0xFFFFFFFF) {
        timer += p_time;
        SetRuneTimer(i, std::min(uint32(2500), timer));
      }
    }
  }

  // group update
  m_groupUpdateTimer.Update(p_time);
  if (m_groupUpdateTimer.Passed()) {
    SendUpdateToOutOfRangeGroupMembers();
    m_groupUpdateTimer.Reset(5000);
  }

  Pet *pet = GetPet();
  if (pet && !pet->IsWithinDistInMap(this, GetMap()->GetVisibilityRange()) &&
      !pet->isPossessed())
    // if (pet && !pet->IsWithinDistInMap(this,
    // GetMap()->GetVisibilityDistance()) && (GetCharmGUID() && (pet->GetGUID()
    // != GetCharmGUID())))
    RemovePet(pet, PET_SAVE_DISMISS, true);

  m_petScalingSynchTimer.Update(p_time);
  if (m_petScalingSynchTimer.Passed()) {
    if (pet) {
      pet->UpdatePetScalingAuras();
      pet->UpdateAllStats();
    }

    m_petScalingSynchTimer.Reset(1000);
  }

  if (IsAlive()) {
    if (m_hostileReferenceCheckTimer <= p_time) {
      m_hostileReferenceCheckTimer = 15 * IN_MILLISECONDS;
      if (!GetMap()->IsDungeon())
        GetCombatManager().EndCombatBeyondRange(GetVisibilityRange(), true);
    } else
      m_hostileReferenceCheckTimer -= p_time;
  }

  // we should execute delayed teleports only for alive(!) players
  // because we don't want player's ghost teleported from graveyard
  if (IsHasDelayedTeleport() && IsAlive())
    TeleportTo(m_teleport_dest, m_teleport_options);
}

void Player::setDeathState(DeathState s) {
  uint32 ressSpellId = 0;

  bool cur = IsAlive();

  if (s == JUST_DIED) {
    if (!cur) {
      LOG_ERROR(
          "entities.player",
          "Player::setDeathState: Attempt to kill a dead player '%s' (%s)",
          GetName().c_str(), GetGUID().ToString().c_str());
      return;
    }

    // drunken state is cleared on death
    SetDrunkValue(0);
    // lost combo points at any target (targeted combo points clear in
    // Unit::setDeathState)
    ClearComboPoints();

    ClearResurrectRequestData();

    // FIXME: is pet dismissed at dying or releasing spirit? if second, add
    // setDeathState(DEAD) to HandleRepopRequestOpcode and define pet unsummon
    // here with (s == DEAD)
    RemovePet(nullptr, PET_SAVE_DISMISS, true);

    // save value before aura remove in Unit::setDeathState
    ressSpellId = GetUInt32Value(PLAYER_SELF_RES_SPELL);

    // passive spell
    if (!ressSpellId)
      ressSpellId = GetResurrectionSpellId();
    UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_DEATH_AT_MAP, 1);
    UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_DEATH, 1);
    UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_DEATH_IN_DUNGEON, 1);

    // reset all death criterias
    ResetAchievementCriteria(ACHIEVEMENT_CRITERIA_CONDITION_NO_DEATH, 0);
  }

  Unit::setDeathState(s);

  // restore resurrection spell id for player after aura remove
  if (s == JUST_DIED && cur && ressSpellId)
    SetUInt32Value(PLAYER_SELF_RES_SPELL, ressSpellId);

  if (IsAlive() && !cur)
    // clear aura case after resurrection by another way (spells will be applied
    // before next death)
    SetUInt32Value(PLAYER_SELF_RES_SPELL, 0);
}

void Player::ToggleAFK() {
  ToggleFlag(PLAYER_FLAGS, PLAYER_FLAGS_AFK);

  // afk player not allowed in battleground
  if (!IsGameMaster() && isAFK() && InBattleground() && !InArena())
    LeaveBattleground();
}

void Player::ToggleDND() { ToggleFlag(PLAYER_FLAGS, PLAYER_FLAGS_DND); }

uint8 Player::GetChatTag() const {
  uint8 tag = CHAT_TAG_NONE;

  if (isGMChat())
    tag |= CHAT_TAG_GM;
  if (isDND())
    tag |= CHAT_TAG_DND;
  if (isAFK())
    tag |= CHAT_TAG_AFK;
  if (HasFlag(PLAYER_FLAGS, PLAYER_FLAGS_DEVELOPER))
    tag |= CHAT_TAG_DEV;

  return tag;
}

bool Player::TeleportTo(uint32 mapid, float x, float y, float z,
                        float orientation, uint32 options) {
  if (!MapManager::IsValidMapCoord(mapid, x, y, z, orientation)) {
    LOG_ERROR("maps",
              "Player::TeleportTo: Invalid map (%d) or invalid coordinates (X: "
              "%f, Y: %f, Z: %f, O: %f) given when teleporting player '%s' "
              "(%s, MapID: %d, X: %f, Y: %f, Z: %f, O: %f).",
              mapid, x, y, z, orientation, GetGUID().ToString().c_str(),
              GetName().c_str(), GetMapId(), GetPositionX(), GetPositionY(),
              GetPositionZ(), GetOrientation());
    return false;
  }

  if (!GetSession()->HasPermission(rbac::RBAC_PERM_SKIP_CHECK_DISABLE_MAP) &&
      DisableMgr::IsDisabledFor(DISABLE_TYPE_MAP, mapid, this)) {
    LOG_INFO("entities.player.cheat",
             "Player::TeleportTo: Player '%s' (%s) tried to enter a forbidden "
             "map (MapID: %u)",
             GetGUID().ToString().c_str(), GetName().c_str(), mapid);
    SendTransferAborted(mapid, TRANSFER_ABORT_MAP_NOT_ALLOWED);
    return false;
  }

  // preparing unsummon pet if lost (we must get pet before teleportation or
  // will not find it later)
  Pet *pet = GetPet();

  MapEntry const *mEntry = sMapStore.LookupEntry(mapid);

  // don't let enter battlegrounds without assigned battleground id (for example
  // through areatrigger)... don't let gm level > 1 either
  if (!InBattleground() && mEntry->IsBattlegroundOrArena())
    return false;

  // client without expansion support
  if (GetSession()->GetExpansion() < mEntry->Expansion()) {
    LOG_DEBUG("maps",
              "Player '%s' (%s) using client without required expansion tried "
              "teleport to non accessible map (MapID: %u)",
              GetName().c_str(), GetGUID().ToString().c_str(), mapid);

    if (TransportBase *transport = GetTransport()) {
      transport->RemovePassenger(this);
      RepopAtGraveyard(); // teleport to near graveyard if on transport, looks
                          // blizz like :)
    }

    SendTransferAborted(mapid, TRANSFER_ABORT_INSUF_EXPAN_LVL,
                        mEntry->Expansion());

    return false; // normal client can't teleport to this map...
  } else
    LOG_DEBUG("maps", "Player %s (%s) is being teleported to map (MapID: %u)",
              GetName().c_str(), GetGUID().ToString().c_str(), mapid);

  if (m_vehicle)
    ExitVehicle();

  // reset movement flags at teleport, because player will continue move with
  // these flags after teleport
  SetUnitMovementFlags(GetUnitMovementFlags() &
                       MOVEMENTFLAG_MASK_HAS_PLAYER_STATUS_OPCODE);
  m_movementInfo.ResetJump();
  DisableSpline();

  if (TransportBase *transport = GetTransport()) {
    if (!(options & TELE_TO_NOT_LEAVE_TRANSPORT))
      transport->RemovePassenger(this);
  }

  // The player was ported to another map and loses the duel immediately.
  // We have to perform this check before the teleport, otherwise the
  // ObjectAccessor won't find the flag.
  if (duel && GetMapId() != mapid &&
      GetMap()->GetGameObject(GetGuidValue(PLAYER_DUEL_ARBITER)))
    DuelComplete(DUEL_FLED);

  if (GetMapId() == mapid) {
    // lets reset far teleport flag if it wasn't reset during chained teleport
    SetSemaphoreTeleportFar(false);
    // setup delayed teleport flag
    SetDelayedTeleportFlag(IsCanDelayTeleport());
    // if teleport spell is cast in Unit::Update() func
    // then we need to delay it until update process will be finished
    if (IsHasDelayedTeleport()) {
      SetSemaphoreTeleportNear(true);
      // lets save teleport destination for player
      m_teleport_dest = WorldLocation(mapid, x, y, z, orientation);
      m_teleport_options = options;
      return true;
    }

    if (!(options & TELE_TO_NOT_UNSUMMON_PET)) {
      // same map, only remove pet if out of range for new position
      if (pet && !pet->IsWithinDist3d(x, y, z, GetMap()->GetVisibilityRange()))
        UnsummonPetTemporaryIfAny();
    }

    if (!(options & TELE_TO_NOT_LEAVE_COMBAT))
      CombatStop();

    // this will be used instead of the current location in SaveToDB
    m_teleport_dest = WorldLocation(mapid, x, y, z, orientation);
    m_teleport_options = options;
    SetFallInformation(0, GetPositionZ());

    // code for finish transfer called in WorldSession::HandleMovementOpcodes()
    // at client packet CMSG_MOVE_TELEPORT_ACK
    SetSemaphoreTeleportNear(true);
    // near teleport, triggering send CMSG_MOVE_TELEPORT_ACK from client at
    // landing
    if (!GetSession()->PlayerLogout()) {
      m_teleport_dest.m_positionZ += GetHoverOffset();
      SendTeleportPacket(m_teleport_dest);
    }
  } else {
    if (getClass() == CLASS_DEATH_KNIGHT && GetMapId() == 609 &&
        !IsGameMaster() && !HasSpell(50977))
      return false;

    // far teleport to another map
    Map *oldmap = IsInWorld() ? GetMap() : nullptr;

    // check if we can enter before stopping combat / removing pet / totems /
    // interrupting spells

    // Check enter rights before map getting to avoid creating instance copy for
    // player this check not dependent from map instance copy and same for all
    // instance copies of selected map
    if (sMapMgr->PlayerCannotEnter(mapid, this, false))
      return false;

    // I think this always returns true. Correct me if I am wrong.
    //  If the map is not created, assume it is possible to enter it.
    //  It will be created in the WorldPortAck.
    // Map* map = sMapMgr->FindBaseNonInstanceMap(mapid);
    // if (!map || map->CanEnter(this))
    {
      // lets reset near teleport flag if it wasn't reset during chained
      // teleports
      SetSemaphoreTeleportNear(false);
      // setup delayed teleport flag
      SetDelayedTeleportFlag(IsCanDelayTeleport());
      // if teleport spell is cast in Unit::Update() func
      // then we need to delay it until update process will be finished
      if (IsHasDelayedTeleport()) {
        SetSemaphoreTeleportFar(true);
        // lets save teleport destination for player
        m_teleport_dest = WorldLocation(mapid, x, y, z, orientation);
        m_teleport_options = options;
        return true;
      }

      SetSelection(ObjectGuid::Empty);

      CombatStop();

      ResetContestedPvP();

      // remove player from battleground on far teleport (when changing maps)
      if (Battleground const *bg = GetBattleground()) {
        // Note: at battleground join battleground id set before teleport
        // and we already will found "current" battleground
        // just need check that this is targeted map or leave
        if (bg->GetMapId() != mapid)
          LeaveBattleground(false); // don't teleport to entry point
      }

      // remove arena spell coldowns/buffs now to also remove pet's cooldowns
      // before it's temporarily unsummoned
      if (mEntry->IsBattleArena() && !IsGameMaster()) {
        RemoveArenaSpellCooldowns(true);
        RemoveArenaAuras();
        if (pet)
          pet->RemoveArenaAuras();
      }

      // remove pet on map change
      if (pet)
        UnsummonPetTemporaryIfAny();

      // remove all dyn objects
      RemoveAllDynObjects();

      // stop spellcasting
      // not attempt interrupt teleportation spell at caster teleport
      if (!(options & TELE_TO_SPELL))
        if (IsNonMeleeSpellCast(true))
          InterruptNonMeleeSpells(true);

      // remove auras before removing from map...
      RemoveAurasWithInterruptFlags(SpellAuraInterruptFlags::Moving |
                                    SpellAuraInterruptFlags::Turning);

      if (!GetSession()->PlayerLogout()) {
        // send transfer packets
        WorldPackets::Movement::TransferPending transferPending;
        transferPending.MapID = mapid;
        if (Transport *transport = dynamic_cast<Transport *>(GetTransport())) {
          transferPending.Ship.emplace();
          transferPending.Ship->ID = transport->GetEntry();
          transferPending.Ship->OriginMapID = GetMapId();
        }

        SendDirectMessage(transferPending.Write());
      }

      // remove from old map now
      if (oldmap)
        oldmap->RemovePlayerFromMap(this, false);

      // players on mount will be dismounted. the speed and height change should
      // not require an ACK and should be applied directly
      PurgeAndApplyPendingMovementChanges(false);

      m_teleport_dest = WorldLocation(mapid, x, y, z, orientation);
      m_teleport_options = options;
      SetFallInformation(0, GetPositionZ());
      // if the player is saved before worldportack (at logout for example)
      // this will be used instead of the current location in SaveToDB

      if (!GetSession()->PlayerLogout()) {
        WorldPackets::Movement::SuspendToken suspendToken;
        suspendToken.SequenceIndex = 1; // to-do implement incremental value
        suspendToken.Reason = 1;
        SendDirectMessage(suspendToken.Write());
      }

      // move packet sent by client always after far teleport
      // code for finish transfer to new map called in
      // WorldSession::HandleMoveWorldportAckOpcode at client packet
      SetSemaphoreTeleportFar(true);
    }
    // else
    //     return false;
  }
  return true;
}

bool Player::TeleportTo(WorldLocation const &loc, uint32 options /*= 0*/) {
  return TeleportTo(loc.GetMapId(), loc.GetPositionX(), loc.GetPositionY(),
                    loc.GetPositionZ(), loc.GetOrientation(), options);
}

bool Player::TeleportToBGEntryPoint() {
  if (m_bgData.joinPos.m_mapId == MAPID_INVALID)
    return false;

  ScheduleDelayedOperation(DELAYED_BG_MOUNT_RESTORE);
  ScheduleDelayedOperation(DELAYED_BG_TAXI_RESTORE);
  ScheduleDelayedOperation(DELAYED_BG_GROUP_RESTORE);
  return TeleportTo(m_bgData.joinPos);
}

void Player::ProcessDelayedOperations() {
  if (m_DelayedOperations == 0)
    return;

  if (m_DelayedOperations & DELAYED_RESURRECT_PLAYER) {
    ResurrectPlayer(0.0f, false);

    if (GetMaxHealth() > _resurrectionData->Health)
      SetHealth(_resurrectionData->Health);
    else
      SetFullHealth();

    if (uint32(GetMaxPower(POWER_MANA)) > _resurrectionData->Mana)
      SetPower(POWER_MANA, _resurrectionData->Mana);
    else
      SetPower(POWER_MANA, GetMaxPower(POWER_MANA));

    SetPower(POWER_RAGE, 0);
    SetPower(POWER_ENERGY, GetMaxPower(POWER_ENERGY));
    SetPower(POWER_ECLIPSE, 0);

    if (uint32 aura = _resurrectionData->Aura) {
      CastSpellExtraArgs args(TRIGGERED_FULL_MASK);
      args.SetOriginalCaster(_resurrectionData->GUID);
      CastSpell(this, aura, args);
    }

    SpawnCorpseBones();
  }

  if (m_DelayedOperations & DELAYED_SAVE_PLAYER)
    SaveToDB();

  if (m_DelayedOperations & DELAYED_SPELL_CAST_DESERTER)
    CastSpell(this, 26013, true); // Deserter

  if (m_DelayedOperations & DELAYED_BG_MOUNT_RESTORE) {
    if (m_bgData.mountSpell) {
      CastSpell(this, m_bgData.mountSpell, true);
      m_bgData.mountSpell = 0;
    }
  }

  if (m_DelayedOperations & DELAYED_BG_TAXI_RESTORE) {
    if (m_bgData.HasTaxiPath()) {
      m_taxi.AddTaxiDestination(m_bgData.taxiPath[0]);
      m_taxi.AddTaxiDestination(m_bgData.taxiPath[1]);
      m_bgData.ClearTaxiPath();

      ContinueTaxiFlight();
    }
  }

  if (m_DelayedOperations & DELAYED_BG_GROUP_RESTORE) {
    if (Group *g = GetGroup())
      g->SendUpdateToPlayer(GetGUID());
  }

  // we have executed ALL delayed ops, so clear the flag
  m_DelayedOperations = 0;
}

void Player::AddToWorld() {
  ///- Do not add/remove the player from the object storage
  ///- It will crash when updating the ObjectAccessor
  ///- The player should only be added when logging in
  Unit::AddToWorld();

  for (uint8 i = PLAYER_SLOT_START; i < PLAYER_SLOT_END; ++i)
    if (m_items[i])
      m_items[i]->AddToWorld();
}

void Player::RemoveFromWorld() {
  // cleanup
  if (IsInWorld()) {
    ///- Release charmed creatures, unsummon totems and remove pets/guardians
    StopCastingCharm();
    StopCastingBindSight();
    UnsummonPetTemporaryIfAny();
    ClearComboPoints();
    ClearComboPointHolders();
    ObjectGuid lootGuid = GetLootGUID();
    if (!lootGuid.IsEmpty())
      m_session->DoLootRelease(lootGuid);
    sOutdoorPvPMgr->HandlePlayerLeaveZone(this, m_zoneUpdateId);
    sBattlefieldMgr->HandlePlayerLeaveZone(this, m_zoneUpdateId);
  }

  // Remove items from world before self - player must be found in
  // Item::RemoveFromObjectUpdate
  for (uint8 i = PLAYER_SLOT_START; i < PLAYER_SLOT_END; ++i) {
    if (m_items[i])
      m_items[i]->RemoveFromWorld();
  }

  ///- Do not add/remove the player from the object storage
  ///- It will crash when updating the ObjectAccessor
  ///- The player should only be removed when logging out
  Unit::RemoveFromWorld();

  for (ItemMap::iterator iter = mMitems.begin(); iter != mMitems.end(); ++iter)
    iter->second->RemoveFromWorld();

  if (m_uint32Values) {
    if (WorldObject *viewpoint = GetViewpoint()) {
      LOG_ERROR("entities.player",
                "Player::RemoveFromWorld: Player '%s' (%s) has viewpoint "
                "(Entry:%u, Type: %u) when removed from world",
                GetName().c_str(), GetGUID().ToString().c_str(),
                viewpoint->GetEntry(), viewpoint->GetTypeId());
      SetViewpoint(viewpoint, false);
    }
  }
}

void Player::SetObjectScale(float scale) {
  Unit::SetObjectScale(scale);
  SetFloatValue(UNIT_FIELD_BOUNDINGRADIUS,
                scale * DEFAULT_PLAYER_BOUNDING_RADIUS);
  SetFloatValue(UNIT_FIELD_COMBATREACH, scale * DEFAULT_PLAYER_COMBAT_REACH);
  if (IsInWorld())
    SendMovementSetCollisionHeight(GetCollisionHeight(),
                                   UPDATE_COLLISION_HEIGHT_SCALE);
}

bool Player::IsImmunedToSpellEffect(SpellInfo const *spellInfo, uint32 index,
                                    Unit *caster) const {
  // players are immune to taunt (the aura and the spell effect)
  if (spellInfo->Effects[index].IsAura(SPELL_AURA_MOD_TAUNT))
    return true;
  if (spellInfo->Effects[index].IsEffect(SPELL_EFFECT_ATTACK_ME))
    return true;

  return Unit::IsImmunedToSpellEffect(spellInfo, index, caster);
}

void Player::RegenerateAll(uint32 diff) {
  m_foodEmoteTimerCount += diff;

  Regenerate(POWER_ENERGY, diff);
  Regenerate(POWER_MANA, diff);
  Regenerate(POWER_FOCUS, diff);
  Regenerate(POWER_HOLY_POWER, diff);
  Regenerate(POWER_RUNIC_POWER, diff);
  Regenerate(POWER_RAGE, diff);
  Regenerate(POWER_ALTERNATE_POWER, diff);

  // Runes act as cooldowns, and they don't need to send any data
  if (getClass() == CLASS_DEATH_KNIGHT) {
    for (uint8 i = 0; i < MAX_RUNES; i += 2) {
      uint8 runeToRegen = i;
      uint32 cd = GetRuneCooldown(i);
      uint32 secondRuneCd = GetRuneCooldown(i + 1);
      float cdmod = GetFloatValue(PLAYER_RUNE_REGEN_1) * 10.0f;
      // Regenerate second rune of the same type only after first rune is off
      // the cooldown
      if (secondRuneCd && (cd > secondRuneCd || !cd)) {
        runeToRegen = i + 1;
        cd = secondRuneCd;
      }

      if (cd)
        SetRuneCooldown(runeToRegen,
                        (cd > (diff * cdmod)) ? cd - (diff * cdmod) : 0);
    }
  }

  // Handles the emotes for drinking and eating.
  // According to sniffs there is a background timer going on that repeats
  // independed from the time window where the aura applies. That's why we dont
  // need to reset the timer on apply. In sniffs I have seen that the first call
  // for the spell visual is totally random, then after 5 seconds over and over
  // again which confirms my theory that we have a independed timer.
  if (m_foodEmoteTimerCount >= 5000) {
    std::vector<AuraEffect *> auraList;
    AuraEffectList const &ModRegenAuras =
        GetAuraEffectsByType(SPELL_AURA_MOD_REGEN);
    AuraEffectList const &ModPowerRegenAuras =
        GetAuraEffectsByType(SPELL_AURA_MOD_POWER_REGEN);

    auraList.reserve(ModRegenAuras.size() + ModPowerRegenAuras.size());
    auraList.insert(auraList.end(), ModRegenAuras.begin(), ModRegenAuras.end());
    auraList.insert(auraList.end(), ModPowerRegenAuras.begin(),
                    ModPowerRegenAuras.end());

    for (auto itr = auraList.begin(); itr != auraList.end(); ++itr) {
      // Food emote comes above drinking emote if we have to decide (mage regen
      // food for example)
      if ((*itr)->GetBase()->HasEffectType(SPELL_AURA_MOD_REGEN) &&
          (*itr)->GetSpellInfo()->HasAuraInterruptFlag(
              SpellAuraInterruptFlags::Standing)) {
        SendPlaySpellVisualKit(SPELL_VISUAL_KIT_FOOD, 0, 0);
        break;
      } else if ((*itr)->GetBase()->HasEffectType(SPELL_AURA_MOD_POWER_REGEN) &&
                 (*itr)->GetSpellInfo()->HasAuraInterruptFlag(
                     SpellAuraInterruptFlags::Standing)) {
        SendPlaySpellVisualKit(SPELL_VISUAL_KIT_DRINK, 0, 0);
        break;
      }
    }
    m_foodEmoteTimerCount -= 5000;
  }
}

void Player::RegenerateHealth() {
  // Do not regenerate health when in combat, when not polymorphed or when any
  // regeneration aura allows it
  if (IsInCombat() && !IsPolymorphed() && !m_baseHealthRegen &&
      !HasAuraType(SPELL_AURA_MOD_REGEN_DURING_COMBAT) &&
      !HasAuraType(SPELL_AURA_MOD_HEALTH_REGEN_IN_COMBAT))
    return;

  uint32 curValue = GetHealth();
  uint32 maxValue = GetMaxHealth();

  if (curValue >= maxValue)
    return;

  float HealthIncreaseRate = sWorld->getRate(RATE_HEALTH);
  float addValue = 0.0f;

  // polymorphed case
  if (IsPolymorphed())
    addValue = float(GetMaxHealth()) / 3.0f;
  // normal regen case (maybe partly in combat case)
  else if (!IsInCombat() || HasAuraType(SPELL_AURA_MOD_REGEN_DURING_COMBAT)) {
    addValue = HealthIncreaseRate;

    if (!IsInCombat()) {
      if (getLevel() < 15)
        addValue =
            (0.20f * ((float)GetMaxHealth()) / getLevel() * HealthIncreaseRate);
      else
        addValue = 0.015f * ((float)GetMaxHealth()) * HealthIncreaseRate;

      AuraEffectList const &mModHealthRegenPct =
          GetAuraEffectsByType(SPELL_AURA_MOD_HEALTH_REGEN_PERCENT);
      for (AuraEffectList::const_iterator i = mModHealthRegenPct.begin();
           i != mModHealthRegenPct.end(); ++i)
        AddPct(addValue, (*i)->GetAmount());

      addValue += GetTotalAuraModifier(SPELL_AURA_MOD_REGEN) * 0.4f;
    } else if (HasAuraType(SPELL_AURA_MOD_REGEN_DURING_COMBAT))
      ApplyPct(addValue,
               GetTotalAuraModifier(SPELL_AURA_MOD_REGEN_DURING_COMBAT));

    if (!IsStandState())
      addValue *= 1.5f;
  }

  // always regeneration bonus (including combat)
  addValue += GetTotalAuraModifier(SPELL_AURA_MOD_HEALTH_REGEN_IN_COMBAT);
  addValue += m_baseHealthRegen / 2.5f;

  if (addValue < 0.0f)
    addValue = 0.0f;

  ModifyHealth(int32(addValue));
}

void Player::ResetAllPowers() {
  SetFullHealth();
  switch (GetPowerType()) {
  case POWER_MANA:
    SetFullPower(POWER_MANA);
    break;
  case POWER_RAGE:
    SetPower(POWER_RAGE, 0);
    break;
  case POWER_ENERGY:
    SetFullPower(POWER_ENERGY);
    break;
  case POWER_RUNIC_POWER:
    SetPower(POWER_RUNIC_POWER, 0);
    break;
  case POWER_ECLIPSE:
    SetPower(POWER_ECLIPSE, 0);
    break;
  default:
    break;
  }
}

bool Player::CanInteractWithQuestGiver(Object *questGiver) const {
  switch (questGiver->GetTypeId()) {
  case TYPEID_UNIT:
    return GetNPCIfCanInteractWith(questGiver->GetGUID(),
                                   UNIT_NPC_FLAG_QUESTGIVER) != nullptr;
  case TYPEID_GAMEOBJECT:
    return GetGameObjectIfCanInteractWith(
               questGiver->GetGUID(), GAMEOBJECT_TYPE_QUESTGIVER) != nullptr;
  case TYPEID_PLAYER:
    return IsAlive() && questGiver->ToPlayer()->IsAlive();
  case TYPEID_ITEM:
    return IsAlive();
  default:
    break;
  }
  return false;
}

Creature *Player::GetNPCIfCanInteractWith(ObjectGuid const &guid,
                                          uint32 npcflagmask) const {
  // unit checks
  if (!guid)
    return nullptr;

  if (!IsInWorld())
    return nullptr;

  if (IsInFlight())
    return nullptr;

  // exist (we need look pets also for some interaction (quest/etc)
  Creature *creature = ObjectAccessor::GetCreatureOrPetOrVehicle(*this, guid);
  if (!creature)
    return nullptr;

  // Deathstate checks
  if (!IsAlive() && !(creature->GetCreatureTemplate()->type_flags &
                      CREATURE_TYPE_FLAG_GHOST_VISIBLE))
    return nullptr;

  // alive or spirit healer
  if (!creature->IsAlive() && !(creature->GetCreatureTemplate()->type_flags &
                                CREATURE_TYPE_FLAG_CAN_INTERACT_WHILE_DEAD))
    return nullptr;

  // appropriate npc type
  if (npcflagmask && !creature->HasFlag(UNIT_NPC_FLAGS, npcflagmask))
    return nullptr;

  // not allow interaction under control, but allow with own pets
  if (creature->GetCharmerGUID())
    return nullptr;

  // not unfriendly/hostile
  if (creature->GetReactionTo(this) <= REP_UNFRIENDLY)
    return nullptr;

  // not too far, taken from CGGameUI::SetInteractTarget
  if (!creature->IsWithinDistInMap(this, creature->GetCombatReach() + 4.0f))
    return nullptr;

  return creature;
}

GameObject *
Player::GetGameObjectIfCanInteractWith(ObjectGuid const &guid) const {
  if (!guid)
    return nullptr;

  if (!IsInWorld())
    return nullptr;

  if (IsInFlight())
    return nullptr;

  // exist
  GameObject *go = ObjectAccessor::GetGameObject(*this, guid);
  if (!go)
    return nullptr;

  if (!go->IsWithinDistInMap(this))
    return nullptr;

  return go;
}

GameObject *Player::GetGameObjectIfCanInteractWith(ObjectGuid const &guid,
                                                   GameobjectTypes type) const {
  GameObject *go = GetGameObjectIfCanInteractWith(guid);
  if (!go)
    return nullptr;

  if (go->GetGoType() != type)
    return nullptr;

  return go;
}

bool Player::IsInAreaTriggerRadius(AreaTriggerEntry const *trigger) const {
  if (!trigger)
    return false;

  if (GetMapId() != trigger->ContinentID &&
      !GetPhaseShift().HasVisibleMapId(trigger->ContinentID))
    return false;

  if (trigger->PhaseID || trigger->PhaseGroupID || trigger->PhaseUseFlags)
    if (!PhasingHandler::InDbPhaseShift(this, trigger->PhaseUseFlags,
                                        trigger->PhaseID,
                                        trigger->PhaseGroupID))
      return false;

  if (trigger->Radius > 0.f) {
    // if we have radius check it
    float dist = GetDistance(trigger->Pos.X, trigger->Pos.Y, trigger->Pos.Z);
    if (dist > trigger->Radius)
      return false;
  } else {
    Position center(trigger->Pos.X, trigger->Pos.Y, trigger->Pos.Z,
                    trigger->Box_yaw);
    if (!IsWithinBox(center, trigger->Box_length / 2.f,
                     trigger->Box_width / 2.f, trigger->Box_height / 2.f))
      return false;
  }

  return true;
}

void Player::SetGameMaster(bool on) {
  if (on) {
    m_ExtraFlags |= PLAYER_EXTRA_GM_ON;
    SetFaction(FACTION_FRIENDLY);
    SetFlag(PLAYER_FLAGS, PLAYER_FLAGS_GM);
    SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_ALLOW_CHEAT_SPELLS);

    if (Pet *pet = GetPet())
      pet->SetFaction(FACTION_FRIENDLY);

    RemoveByteFlag(UNIT_FIELD_BYTES_2, UNIT_BYTES_2_OFFSET_PVP_FLAG,
                   UNIT_BYTE2_FLAG_FFA_PVP);
    ResetContestedPvP();

    CombatStopWithPets();

    PhasingHandler::SetAlwaysVisible(GetPhaseShift(), true);
    m_serverSideVisibilityDetect.SetValue(SERVERSIDE_VISIBILITY_GM,
                                          GetSession()->GetSecurity());
  } else {
    PhasingHandler::SetAlwaysVisible(GetPhaseShift(), false);

    m_ExtraFlags &= ~PLAYER_EXTRA_GM_ON;
    SetFactionForRace(getRace());
    RemoveFlag(PLAYER_FLAGS, PLAYER_FLAGS_GM);
    RemoveFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_ALLOW_CHEAT_SPELLS);

    if (Pet *pet = GetPet())
      pet->SetFaction(GetFaction());

    // restore FFA PvP Server state
    if (sWorld->IsFFAPvPRealm())
      SetByteFlag(UNIT_FIELD_BYTES_2, UNIT_BYTES_2_OFFSET_PVP_FLAG,
                  UNIT_BYTE2_FLAG_FFA_PVP);

    // restore FFA PvP area state, remove not allowed for GM mounts
    UpdateArea(m_areaUpdateId);

    m_serverSideVisibilityDetect.SetValue(SERVERSIDE_VISIBILITY_GM, SEC_PLAYER);
  }

  UpdateObjectVisibility();
}

bool Player::CanBeGameMaster() const {
  return GetSession()->HasPermission(rbac::RBAC_PERM_COMMAND_GM);
}

void Player::SetGMVisible(bool on) {
  if (on) {
    m_ExtraFlags &= ~PLAYER_EXTRA_GM_INVISIBLE; // remove flag
    m_serverSideVisibility.SetValue(SERVERSIDE_VISIBILITY_GM, SEC_PLAYER);
  } else {
    m_ExtraFlags |= PLAYER_EXTRA_GM_INVISIBLE; // add flag

    SetAcceptWhispers(false);
    SetGameMaster(true);

    m_serverSideVisibility.SetValue(SERVERSIDE_VISIBILITY_GM,
                                    GetSession()->GetSecurity());
  }

  for (Channel *channel : m_channels)
    channel->SetInvisible(this, !on);
}

bool Player::IsGroupVisibleFor(Player const *p) const {
  switch (sWorld->getIntConfig(CONFIG_GROUP_VISIBILITY)) {
  default:
    return IsInSameGroupWith(p);
  case 1:
    return IsInSameRaidWith(p);
  case 2:
    return GetTeam() == p->GetTeam();
  case 3:
    return false;
  }
}

bool Player::IsInSameGroupWith(Player const *p) const {
  return p == this || (GetGroup() != nullptr && GetGroup() == p->GetGroup() &&
                       GetGroup()->SameSubGroup(this, p));
}

bool Player::IsInSameRaidWith(Player const *p) const {
  return p == this || (GetGroup() != nullptr && GetGroup() == p->GetGroup());
}

///- If the player is invited, remove him. If the group if then only 1 person,
/// disband the group.
void Player::UninviteFromGroup() {
  Group *group = GetGroupInvite();
  if (!group)
    return;

  group->RemoveInvite(this);

  if (group->IsCreated()) {
    if (group->GetMembersCount() <= 1) // group has just 1 member => disband
      group->Disband(true);
  } else {
    if (group->GetInviteeCount() <= 1) {
      group->RemoveAllInvites();
      delete group;
    }
  }
}

void Player::RemoveFromGroup(
    Group *group, ObjectGuid guid,
    RemoveMethod method /*= GROUP_REMOVEMETHOD_DEFAULT*/,
    ObjectGuid kicker /*= ObjectGuid::Empty*/,
    char const *reason /*= nullptr*/) {
  if (!group)
    return;

  group->RemoveMember(guid, method, kicker, reason);
}

void Player::GiveXP(uint32 xp, Unit *victim, float group_rate) {
  if (xp < 1)
    return;

  if (!IsAlive() && !GetBattlegroundId())
    return;

  if (HasFlag(PLAYER_FLAGS, PLAYER_FLAGS_NO_XP_GAIN))
    return;

  if (victim && victim->GetTypeId() == TYPEID_UNIT &&
      !victim->ToCreature()->hasLootRecipient())
    return;

  uint8 level = getLevel();

  sScriptMgr->OnGivePlayerXP(this, xp, victim);

  // XP to money conversion processed in Player::RewardQuest
  if (IsMaxLevel())
    return;

  uint32 bonus_xp;
  bool recruitAFriend = GetsRecruitAFriendBonus(true);

  // RaF does NOT stack with rested experience
  if (recruitAFriend)
    bonus_xp = 2 * xp; // xp + bonus_xp must add up to 3 * xp for RaF;
                       // calculation for quests done client-side
  else
    bonus_xp = victim ? GetXPRestBonus(xp) : 0; // XP resting bonus

  WorldPackets::Character::LogXPGain packet;
  packet.Victim = victim ? victim->GetGUID() : ObjectGuid::Empty;
  packet.Original = xp + bonus_xp;
  packet.Reason = victim ? LOG_XP_REASON_KILL : LOG_XP_REASON_NO_KILL;
  packet.Amount = xp;
  packet.GroupBonus = group_rate;
  packet.ReferAFriendBonusType = recruitAFriend ? 1 : 0;
  SendDirectMessage(packet.Write());

  uint32 curXP = GetUInt32Value(PLAYER_XP);
  uint32 nextLvlXP = GetUInt32Value(PLAYER_NEXT_LEVEL_XP);
  uint32 newXP = curXP + xp + bonus_xp;

  while (newXP >= nextLvlXP && !IsMaxLevel()) {
    newXP -= nextLvlXP;

    if (!IsMaxLevel())
      GiveLevel(level + 1);

    level = getLevel();
    nextLvlXP = GetUInt32Value(PLAYER_NEXT_LEVEL_XP);
  }

  SetUInt32Value(PLAYER_XP, newXP);
}

// Update player to next level
// Current player experience not update (must be update by caller)
void Player::GiveLevel(uint8 level) {
  uint8 oldLevel = getLevel();
  if (level == oldLevel)
    return;

  if (Guild *guild = GetGuild())
    guild->UpdateMemberData(this, GUILD_MEMBER_DATA_LEVEL, level);

  PlayerLevelInfo info;
  sObjectMgr->GetPlayerLevelInfo(getRace(), getClass(), level, &info);

  uint32 basehp = 0, basemana = 0;
  sObjectMgr->GetPlayerClassLevelInfo(getClass(), level, basehp, basemana);

  WorldPackets::Misc::LevelUpInfo packet;
  packet.Level = level;
  packet.HealthDelta = 0;

  /// @todo find some better solution
  // for (int i = 0; i < MAX_STORED_POWERS; ++i)
  packet.PowerDelta[0] = int32(basemana) - int32(GetCreateMana());
  packet.PowerDelta[1] = 0;
  packet.PowerDelta[2] = 0;
  packet.PowerDelta[3] = 0;
  packet.PowerDelta[4] = 0;
  packet.PowerDelta[5] = 0;

  for (uint8 i = STAT_STRENGTH; i < MAX_STATS; ++i)
    packet.StatDelta[i] = int32(info.stats[i]) - GetCreateStat(Stats(i));

  SendDirectMessage(packet.Write());

  SetUInt32Value(PLAYER_NEXT_LEVEL_XP, sObjectMgr->GetXPForLevel(level));

  // update level, max level of skills
  m_Played_time[PLAYED_TIME_LEVEL] = 0; // Level Played Time reset

  _ApplyAllLevelScaleItemMods(false);

  SetLevel(level);

  UpdateSkillsForLevel();

  // save base values (bonuses already included in stored stats
  for (uint8 i = STAT_STRENGTH; i < MAX_STATS; ++i)
    SetCreateStat(Stats(i), info.stats[i]);

  SetCreateHealth(basehp);
  SetCreateMana(basemana);

  InitTalentForLevel();
  InitTaxiNodesForLevel();
  InitGlyphsForLevel();

  UpdateAllStats();

  _ApplyAllLevelScaleItemMods(true); // Moved to above SetFullHealth so player
                                     // will have full health from Heirlooms

  // set current level health and mana/energy to maximum after applying all
  // mods.
  SetFullHealth();
  SetFullPower(POWER_MANA);

  // update level to hunter/summon pet
  if (Pet *pet = GetPet())
    pet->SynchronizeLevelWithOwner();

  // Update QuestGivers
  SendQuestGiverStatusMultiple();

  if (MailLevelReward const *mailReward =
          sObjectMgr->GetMailLevelReward(level, getRaceMask())) {
    /// @todo Poor design of mail system
    CharacterDatabaseTransaction trans = CharacterDatabase.BeginTransaction();
    MailDraft(mailReward->mailTemplateId)
        .SendMailTo(trans, this,
                    MailSender(MAIL_CREATURE, mailReward->senderEntry));
    CharacterDatabase.CommitTransaction(trans);
  }

  UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_REACH_LEVEL);

  // Refer-A-Friend
  if (GetSession()->GetRecruiterId())
    if (level <
        sWorld->getIntConfig(CONFIG_MAX_RECRUIT_A_FRIEND_BONUS_PLAYER_LEVEL))
      if (level % 2 == 0) {
        ++m_grantableLevels;

        if (!HasByteFlag(PLAYER_FIELD_BYTES,
                         PLAYER_FIELD_BYTES_OFFSET_RAF_GRANTABLE_LEVEL, 0x01))
          SetByteFlag(PLAYER_FIELD_BYTES,
                      PLAYER_FIELD_BYTES_OFFSET_RAF_GRANTABLE_LEVEL, 0x01);
      }

  sScriptMgr->OnPlayerLevelChanged(this, oldLevel);
}

bool Player::IsMaxLevel() const {
  return getLevel() >= GetUInt32Value(PLAYER_FIELD_MAX_LEVEL);
}

void Player::InitTalentForLevel() {
  uint8 level = getLevel();
  // talents base at level diff (talents = level - 9 but some can be used
  // already)
  if (level < 10) {
    // Remove all talent points
    if (GetUsedTalentCount() > 0) // Free any used talents
    {
      ResetTalents(true);
      SetFreeTalentPoints(0);
    }
  } else {
    if (level < sWorld->getIntConfig(CONFIG_MIN_DUALSPEC_LEVEL) ||
        GetSpecsCount() == 0) {
      SetSpecsCount(1);
      SetActiveSpec(0);
    }

    uint32 talentPointsForLevel = CalculateTalentsPoints();

    // if used more that have then reset
    if (GetUsedTalentCount() > talentPointsForLevel) {
      if (!GetSession()->HasPermission(
              rbac::RBAC_PERM_SKIP_CHECK_MORE_TALENTS_THAN_ALLOWED))
        ResetTalents(true);
      else
        SetFreeTalentPoints(0);
    }
    // else update amount of free points
    else
      SetFreeTalentPoints(talentPointsForLevel - GetUsedTalentCount());
  }

  if (!GetSession()->PlayerLoading())
    SendTalentsInfoData(false); // update at client
}

void Player::InitStatsForLevel(bool reapplyMods) {
  if (reapplyMods) // reapply stats values only on .reset stats (level) command
    _RemoveAllStatBonuses();

  uint32 basehp = 0, basemana = 0;
  sObjectMgr->GetPlayerClassLevelInfo(getClass(), getLevel(), basehp, basemana);

  PlayerLevelInfo info;
  sObjectMgr->GetPlayerLevelInfo(getRace(), getClass(), getLevel(), &info);

  uint8 exp_max_lvl =
      DBCManager::GetMaxLevelForExpansion(GetSession()->GetExpansion());
  uint8 conf_max_lvl = sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL);
  if (exp_max_lvl == DEFAULT_MAX_LEVEL || exp_max_lvl >= conf_max_lvl)
    SetUInt32Value(PLAYER_FIELD_MAX_LEVEL, conf_max_lvl);
  else
    SetUInt32Value(PLAYER_FIELD_MAX_LEVEL, exp_max_lvl);
  SetUInt32Value(PLAYER_NEXT_LEVEL_XP, sObjectMgr->GetXPForLevel(getLevel()));

  // reset before any aura state sources (health set/aura apply)
  SetUInt32Value(UNIT_FIELD_AURASTATE, 0);

  UpdateSkillsForLevel();

  // set default cast time multiplier
  SetFloatValue(UNIT_MOD_CAST_SPEED, 1.0f);
  SetFloatValue(UNIT_MOD_CAST_HASTE, 1.0f);
  SetFloatValue(PLAYER_FIELD_MOD_HASTE, 1.0f);
  SetFloatValue(PLAYER_FIELD_MOD_RANGED_HASTE, 1.0f);

  // reset size before reapply auras
  SetObjectScale(1.0f);

  // save base values (bonuses already included in stored stats
  for (uint8 i = STAT_STRENGTH; i < MAX_STATS; ++i)
    SetCreateStat(Stats(i), info.stats[i]);

  for (uint8 i = STAT_STRENGTH; i < MAX_STATS; ++i)
    SetStat(Stats(i), info.stats[i]);

  SetCreateHealth(basehp);

  // set create powers
  SetCreateMana(basemana);

  SetArmor(int32(m_createStats[STAT_AGILITY] * 2));

  InitStatBuffMods();

  // reset rating fields values
  for (uint16 index = PLAYER_FIELD_COMBAT_RATING_1;
       index < PLAYER_FIELD_COMBAT_RATING_1 + MAX_COMBAT_RATING; ++index)
    SetUInt32Value(index, 0);

  SetUInt32Value(PLAYER_FIELD_MOD_HEALING_DONE_POS, 0);
  SetFloatValue(PLAYER_FIELD_MOD_HEALING_PCT, 1.0f);
  SetFloatValue(PLAYER_FIELD_MOD_HEALING_DONE_PCT, 1.0f);
  for (uint8 i = 0; i < 7; ++i) {
    SetUInt32Value(PLAYER_FIELD_MOD_DAMAGE_DONE_NEG + i, 0);
    SetUInt32Value(PLAYER_FIELD_MOD_DAMAGE_DONE_POS + i, 0);
    SetFloatValue(PLAYER_FIELD_MOD_DAMAGE_DONE_PCT + i, 1.0f);
  }

  SetFloatValue(PLAYER_FIELD_MOD_SPELL_POWER_PCT, 1.0f);
  SetFloatValue(PLAYER_FIELD_MOD_HASTE_REGEN, 1.0f);
  SetFloatValue(PLAYER_FIELD_OVERRIDE_SPELL_POWER_BY_AP_PCT, 0.0f);

  // reset attack power, damage and attack speed fields
  SetFloatValue(UNIT_FIELD_BASEATTACKTIME, 2000.0f);
  SetFloatValue(UNIT_FIELD_BASEATTACKTIME + 1, 2000.0f); // offhand attack time
  SetFloatValue(UNIT_FIELD_RANGEDATTACKTIME, 2000.0f);

  SetFloatValue(UNIT_FIELD_MINDAMAGE, 0.0f);
  SetFloatValue(UNIT_FIELD_MAXDAMAGE, 0.0f);
  SetFloatValue(UNIT_FIELD_MINOFFHANDDAMAGE, 0.0f);
  SetFloatValue(UNIT_FIELD_MAXOFFHANDDAMAGE, 0.0f);
  SetFloatValue(UNIT_FIELD_MINRANGEDDAMAGE, 0.0f);
  SetFloatValue(UNIT_FIELD_MAXRANGEDDAMAGE, 0.0f);
  SetFloatValue(PLAYER_FIELD_WEAPON_DMG_MULTIPLIERS, 1.0f);

  SetInt32Value(UNIT_FIELD_ATTACK_POWER, 0);
  SetInt32Value(UNIT_FIELD_ATTACK_POWER_MOD_POS, 0);
  SetInt32Value(UNIT_FIELD_ATTACK_POWER_MOD_NEG, 0);
  SetFloatValue(UNIT_FIELD_ATTACK_POWER_MULTIPLIER, 0.0f);
  SetInt32Value(UNIT_FIELD_RANGED_ATTACK_POWER, 0);
  SetInt32Value(UNIT_FIELD_ATTACK_POWER_MOD_POS, 0);
  SetInt32Value(UNIT_FIELD_ATTACK_POWER_MOD_NEG, 0);
  SetFloatValue(UNIT_FIELD_RANGED_ATTACK_POWER_MULTIPLIER, 0.0f);

  // Base crit values (will be recalculated in UpdateAllStats() at loading and
  // in _ApplyAllStatBonuses() at reset
  SetFloatValue(PLAYER_CRIT_PERCENTAGE, 0.0f);
  SetFloatValue(PLAYER_OFFHAND_CRIT_PERCENTAGE, 0.0f);
  SetFloatValue(PLAYER_RANGED_CRIT_PERCENTAGE, 0.0f);

  // Init spell schools (will be recalculated in UpdateAllStats() at loading and
  // in _ApplyAllStatBonuses() at reset
  for (uint8 i = 0; i < 7; ++i)
    SetFloatValue(PLAYER_SPELL_CRIT_PERCENTAGE1 + i, 0.0f);

  SetFloatValue(PLAYER_PARRY_PERCENTAGE, 0.0f);
  SetFloatValue(PLAYER_BLOCK_PERCENTAGE, 0.0f);

  // Static 30% damage blocked
  SetUInt32Value(PLAYER_SHIELD_BLOCK, 30);

  // Dodge percentage
  SetFloatValue(PLAYER_DODGE_PERCENTAGE, 0.0f);

  // set armor (resistance 0) to original value (create_agility*2)
  SetArmor(int32(m_createStats[STAT_AGILITY] * 2));
  SetFloatValue(UNIT_FIELD_RESISTANCEBUFFMODSPOSITIVE + SPELL_SCHOOL_NORMAL,
                0.0f);
  SetFloatValue(UNIT_FIELD_RESISTANCEBUFFMODSNEGATIVE + SPELL_SCHOOL_NORMAL,
                0.0f);
  // set other resistance to original value (0)
  for (uint8 i = 1; i < MAX_SPELL_SCHOOL; ++i) {
    SetResistance(SpellSchools(i), 0);
    SetFloatValue(UNIT_FIELD_RESISTANCEBUFFMODSPOSITIVE + i, 0.0f);
    SetFloatValue(UNIT_FIELD_RESISTANCEBUFFMODSNEGATIVE + i, 0.0f);
  }

  SetUInt32Value(PLAYER_FIELD_MOD_TARGET_RESISTANCE, 0);
  SetUInt32Value(PLAYER_FIELD_MOD_TARGET_PHYSICAL_RESISTANCE, 0);
  for (uint8 i = 0; i < MAX_SPELL_SCHOOL; ++i) {
    SetUInt32Value(UNIT_FIELD_POWER_COST_MODIFIER + i, 0);
    SetFloatValue(UNIT_FIELD_POWER_COST_MULTIPLIER + i, 0.0f);
  }
  // Reset no reagent cost field
  for (uint8 i = 0; i < 3; ++i)
    SetUInt32Value(PLAYER_NO_REAGENT_COST_1 + i, 0);
  // Init data for form but skip reapply item mods for form
  InitDataForForm(reapplyMods);

  // save new stats
  for (uint8 i = POWER_MANA; i < MAX_POWERS; ++i)
    SetMaxPower(Powers(i), GetCreatePowers(Powers(i)));

  SetMaxHealth(basehp); // stamina bonus will applied later

  // cleanup mounted state (it will set correctly at aura loading if player
  // saved at mount.
  SetUInt32Value(UNIT_FIELD_MOUNTDISPLAYID, 0);

  // cleanup unit flags (will be re-applied if need at aura load).
  RemoveFlag(UNIT_FIELD_FLAGS,
             UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_REMOVE_CLIENT_CONTROL |
                 UNIT_FLAG_NOT_ATTACKABLE_1 | UNIT_FLAG_IMMUNE_TO_PC |
                 UNIT_FLAG_IMMUNE_TO_NPC | UNIT_FLAG_LOOTING |
                 UNIT_FLAG_PET_IN_COMBAT | UNIT_FLAG_SILENCED |
                 UNIT_FLAG_PACIFIED | UNIT_FLAG_STUNNED | UNIT_FLAG_IN_COMBAT |
                 UNIT_FLAG_DISARMED | UNIT_FLAG_CONFUSED | UNIT_FLAG_FLEEING |
                 UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_SKINNABLE |
                 UNIT_FLAG_MOUNT | UNIT_FLAG_TAXI_FLIGHT);
  SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED); // must be set

  SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_REGENERATE_POWER); // must be set

  // cleanup player flags (will be re-applied if need at aura load), to avoid
  // have ghost flag without ghost aura, for example.
  RemoveFlag(PLAYER_FLAGS, PLAYER_FLAGS_AFK | PLAYER_FLAGS_DND |
                               PLAYER_FLAGS_GM | PLAYER_FLAGS_GHOST |
                               PLAYER_FLAGS_DISABLE_CASTING_EXCEPT_ABILITIES |
                               PLAYER_FLAGS_DISABLE_ATTACKING_EXCEPT_ABILITIES);

  RemoveStandFlags(UNIT_STAND_FLAGS_ALL); // one form stealth modified bytes
  RemoveByteFlag(UNIT_FIELD_BYTES_2, UNIT_BYTES_2_OFFSET_PVP_FLAG,
                 UNIT_BYTE2_FLAG_FFA_PVP | UNIT_BYTE2_FLAG_SANCTUARY);

  // restore if need some important flags
  SetUInt32Value(PLAYER_FIELD_BYTES2, 0); // flags empty by default

  if (reapplyMods) // reapply stats values only on .reset stats (level) command
    _ApplyAllStatBonuses();

  // set current level health and mana/energy to maximum after applying all
  // mods.
  SetFullHealth();
  SetFullPower(POWER_MANA);
  SetFullPower(POWER_ENERGY);
  if (GetPower(POWER_RAGE) > GetMaxPower(POWER_RAGE))
    SetFullPower(POWER_RAGE);
  SetFullPower(POWER_FOCUS);
  SetPower(POWER_RUNIC_POWER, 0);

  // update level to hunter/summon pet
  if (Pet *pet = GetPet())
    pet->SynchronizeLevelWithOwner();
}

void Player::SendKnownSpells(bool firstLogin /*= false*/) {
  WorldPackets::Spells::SendKnownSpells knownSpells;
  knownSpells.InitialLogin = firstLogin;

  knownSpells.KnownSpells.reserve(m_spells.size());
  for (PlayerSpellMap::value_type const &spell : m_spells) {
    if (spell.second.state == PLAYERSPELL_REMOVED)
      continue;

    if (!spell.second.active || spell.second.disabled)
      continue;

    knownSpells.KnownSpells.push_back(spell.first);
  }

  GetSpellHistory()->WriteSpellHistoryEntries<Player>(
      knownSpells.SpellHistoryEntries);

  SendDirectMessage(knownSpells.Write());
}

void Player::RemoveMail(uint32 id) {
  for (PlayerMails::iterator itr = m_mail.begin(); itr != m_mail.end(); ++itr) {
    if ((*itr)->messageID == id) {
      // do not delete item, because Player::removeMail() is called when
      // returning mail to sender.
      m_mail.erase(itr);
      return;
    }
  }
}

void Player::SendMailResult(uint32 mailId, MailResponseType mailAction,
                            MailResponseResult mailError, uint32 equipError,
                            ObjectGuid::LowType item_guid,
                            uint32 item_count) const {
  WorldPacket data(SMSG_SEND_MAIL_RESULT,
                   (4 + 4 + 4 +
                    (mailError == MAIL_ERR_EQUIP_ERROR
                         ? 4
                         : (mailAction == MAIL_ITEM_TAKEN ? 4 + 4 : 0))));
  data << uint32(mailId);
  data << uint32(mailAction);
  data << uint32(mailError);
  if (mailError == MAIL_ERR_EQUIP_ERROR)
    data << uint32(equipError);
  else if (mailAction == MAIL_ITEM_TAKEN) {
    data << uint32(item_guid);  // item guid low?
    data << uint32(item_count); // item count?
  }
  SendDirectMessage(&data);
}

void Player::SendNewMail() const {
  // deliver undelivered mail
  WorldPacket data(SMSG_RECEIVED_MAIL, 4);
  data << (uint32)0;
  SendDirectMessage(&data);
}

void Player::UpdateNextMailTimeAndUnreads() {
  // calculate next delivery time (min. from non-delivered mails
  // and recalculate unReadMail
  time_t cTime = GameTime::GetGameTime();
  m_nextMailDelivereTime = 0;
  unReadMails = 0;
  for (PlayerMails::iterator itr = m_mail.begin(); itr != m_mail.end(); ++itr) {
    if ((*itr)->deliver_time > cTime) {
      if (!m_nextMailDelivereTime ||
          m_nextMailDelivereTime > (*itr)->deliver_time)
        m_nextMailDelivereTime = (*itr)->deliver_time;
    } else if (((*itr)->checked & MAIL_CHECK_MASK_READ) == 0)
      ++unReadMails;
  }
}

void Player::AddNewMailDeliverTime(time_t deliver_time) {
  if (deliver_time <= GameTime::GetGameTime()) // ready now
  {
    ++unReadMails;
    SendNewMail();
  } else // not ready and no have ready mails
  {
    if (!m_nextMailDelivereTime || m_nextMailDelivereTime > deliver_time)
      m_nextMailDelivereTime = deliver_time;
  }
}

void DeleteSpellFromAllPlayers(uint32 spellId) {
  CharacterDatabaseStatements stmts[2] = {CHAR_DEL_INVALID_SPELL_SPELLS,
                                          CHAR_DEL_INVALID_SPELL_TALENTS};
  for (uint8 i = 0; i < 2; i++) {
    CharacterDatabasePreparedStatement *stmt =
        CharacterDatabase.GetPreparedStatement(stmts[i]);

    stmt->SetData(0, spellId);

    CharacterDatabase.Execute(stmt);
  }
}

bool Player::AddTalent(uint32 spellId, uint8 spec, bool learning) {
  SpellInfo const *spellInfo = sSpellMgr->GetSpellInfo(spellId);
  if (!spellInfo) {
    // do character spell book cleanup (all characters)
    if (!IsInWorld() && !learning) // spell load case
    {
      LOG_ERROR(
          "spells",
          "Player::AddTalent: Spell (ID: %u) does not exist. Deleting for all "
          "characters in `character_spell` and `character_talent`.",
          spellId);

      DeleteSpellFromAllPlayers(spellId);
    } else
      LOG_ERROR("spells", "Player::AddTalent: Spell (ID: %u) does not exist",
                spellId);

    return false;
  }

  if (!SpellMgr::IsSpellValid(spellInfo, this, false)) {
    // do character spell book cleanup (all characters)
    if (!IsInWorld() && !learning) // spell load case
    {
      LOG_ERROR("spells",
                "Player::AddTalent: Spell (ID: %u) is invalid. Deleting for "
                "all characters in `character_spell` and `character_talent`.",
                spellId);

      DeleteSpellFromAllPlayers(spellId);
    } else
      LOG_ERROR("spells", "Player::AddTalent: Spell (ID: %u) is invalid",
                spellId);

    return false;
  }

  PlayerTalentMap::iterator itr = GetTalentMap(spec)->find(spellId);
  if (itr != GetTalentMap(spec)->end())
    itr->second->state = PLAYERSPELL_UNCHANGED;
  else if (TalentSpellPos const *talentPos =
               sDBCManager.GetTalentSpellPos(spellId)) {
    if (TalentEntry const *talentInfo =
            sTalentStore.LookupEntry(talentPos->talent_id)) {
      for (uint8 rank = 0; rank < MAX_TALENT_RANK; ++rank) {
        // skip learning spell and no rank spell case
        uint32 rankSpellId = talentInfo->SpellRank[rank];
        if (!rankSpellId || rankSpellId == spellId)
          continue;

        itr = GetTalentMap(spec)->find(rankSpellId);
        if (itr != GetTalentMap(spec)->end())
          itr->second->state = PLAYERSPELL_REMOVED;
      }
    }

    PlayerTalent *newtalent = new PlayerTalent();

    newtalent->state = learning ? PLAYERSPELL_NEW : PLAYERSPELL_UNCHANGED;
    newtalent->spec = spec;

    (*GetTalentMap(spec))[spellId] = newtalent;
    return true;
  }
  return false;
}

bool Player::AddSpell(uint32 spellId, bool active, bool learning,
                      bool dependent, bool disabled, bool loading /*= false*/,
                      uint32 fromSkill /*= 0*/) {
  SpellInfo const *spellInfo = sSpellMgr->GetSpellInfo(spellId);
  if (!spellInfo) {
    // do character spell book cleanup (all characters)
    if (!IsInWorld() && !learning) // spell load case
    {
      LOG_ERROR("spells",
                "Player::AddSpell: Spell (ID: %u) does not exist. deleting for "
                "all characters in `character_spell` and `character_talent`.",
                spellId);

      DeleteSpellFromAllPlayers(spellId);
    } else
      LOG_ERROR("spells", "Player::AddSpell: Spell (ID: %u) does not exist",
                spellId);

    return false;
  }

  if (!SpellMgr::IsSpellValid(spellInfo, this, false)) {
    // do character spell book cleanup (all characters)
    if (!IsInWorld() && !learning) // spell load case
    {
      LOG_ERROR("spells",
                "Player::AddSpell: Spell (ID: %u) is invalid. deleting for all "
                "characters in `character_spell` and `character_talent`.",
                spellId);

      DeleteSpellFromAllPlayers(spellId);
    } else
      LOG_ERROR("spells", "Player::AddSpell: Spell (ID: %u) is invalid",
                spellId);

    return false;
  }

  PlayerSpellState state = learning ? PLAYERSPELL_NEW : PLAYERSPELL_UNCHANGED;

  bool dependent_set = false;
  bool disabled_case = false;
  bool superceded_old = false;

  PlayerSpellMap::iterator itr = m_spells.find(spellId);

  // Remove temporary spell if found to prevent conflicts
  if (itr != m_spells.end() && itr->second.state == PLAYERSPELL_TEMPORARY)
    RemoveTemporarySpell(spellId);
  else if (itr != m_spells.end()) {
    uint32 next_active_spell_id = 0;
    // fix activate state for non-stackable low rank (and find next spell for
    // !active case)
    if (!spellInfo->IsStackableWithRanks() && spellInfo->IsRanked()) {
      if (uint32 next = sSpellMgr->GetNextSpellInChain(spellId)) {
        if (HasSpell(next)) {
          // high rank already known so this must !active
          active = false;
          next_active_spell_id = next;
        }
      }
    }

    // not do anything if already known in expected state
    if (itr->second.state != PLAYERSPELL_REMOVED &&
        itr->second.active == active && itr->second.dependent == dependent &&
        itr->second.disabled == disabled) {
      if (!IsInWorld() && !learning) // explicitly load from DB and then exist
                                     // in it already and set correctly
        itr->second.state = PLAYERSPELL_UNCHANGED;

      return false;
    }

    // dependent spell known as not dependent, overwrite state
    if (itr->second.state != PLAYERSPELL_REMOVED && !itr->second.dependent &&
        dependent) {
      itr->second.dependent = dependent;
      if (itr->second.state != PLAYERSPELL_NEW)
        itr->second.state = PLAYERSPELL_CHANGED;
      dependent_set = true;
    }

    // update active state for known spell
    if (itr->second.active != active &&
        itr->second.state != PLAYERSPELL_REMOVED && !itr->second.disabled) {
      itr->second.active = active;

      if (!IsInWorld() && !learning &&
          !dependent_set) // explicitly load from DB and then exist in it
                          // already and set correctly
        itr->second.state = PLAYERSPELL_UNCHANGED;
      else if (itr->second.state != PLAYERSPELL_NEW)
        itr->second.state = PLAYERSPELL_CHANGED;

      if (active) {
        if (spellInfo->IsPassive() && HandlePassiveSpellLearn(spellInfo))
          CastSpell(this, spellId, true);
      } else if (IsInWorld()) {
        if (next_active_spell_id)
          SendSupercededSpell(spellId, next_active_spell_id);
        else {
          WorldPackets::Spells::UnlearnedSpells unlearnedSpells;
          unlearnedSpells.SpellID = spellId;
          SendDirectMessage(unlearnedSpells.Write());
        }
      }

      return active; // learn (show in spell book if active now)
    }

    if (itr->second.disabled != disabled &&
        itr->second.state != PLAYERSPELL_REMOVED) {
      if (itr->second.state != PLAYERSPELL_NEW)
        itr->second.state = PLAYERSPELL_CHANGED;
      itr->second.disabled = disabled;

      if (disabled)
        return false;

      disabled_case = true;
    } else
      switch (itr->second.state) {
      case PLAYERSPELL_UNCHANGED: // known saved spell
        return false;
      case PLAYERSPELL_REMOVED: // re-learning removed not saved spell
      {
        m_spells.erase(itr);
        state = PLAYERSPELL_CHANGED;
        break; // need re-add
      }
      default: // known not saved yet spell (new or modified)
      {
        // can be in case spell loading but learned at some previous spell
        // loading
        if (!IsInWorld() && !learning && !dependent_set)
          itr->second.state = PLAYERSPELL_UNCHANGED;

        return false;
      }
      }
  }

  if (!disabled_case) // skip new spell adding if spell already known (disabled
                      // spells case)
  {
    // talent: unlearn all other talent ranks (high and low)
    if (TalentSpellPos const *talentPos =
            sDBCManager.GetTalentSpellPos(spellId)) {
      if (TalentEntry const *talentInfo =
              sTalentStore.LookupEntry(talentPos->talent_id)) {
        for (uint8 rank = 0; rank < MAX_TALENT_RANK; ++rank) {
          // skip learning spell and no rank spell case
          uint32 rankSpellId = talentInfo->SpellRank[rank];
          if (!rankSpellId || rankSpellId == spellId)
            continue;

          RemoveSpell(rankSpellId, false, false);
        }
      }
    }
    // non talent spell: learn low ranks (recursive call)
    else if (uint32 prev_spell = sSpellMgr->GetPrevSpellInChain(spellId)) {
      if (!IsInWorld() ||
          disabled) // at spells loading, no output, but allow save
        AddSpell(prev_spell, active, true, true, disabled, false, fromSkill);
      else // at normal learning
        LearnSpell(prev_spell, true, fromSkill);
    }

    std::pair<PlayerSpellMap::iterator, bool> inserted = m_spells.emplace(
        std::piecewise_construct, std::forward_as_tuple(spellId),
        std::forward_as_tuple());
    PlayerSpell &newspell = inserted.first->second;
    // learning a previous rank might have given us this spell already from a
    // skill autolearn, most likely with PLAYERSPELL_NEW state we dont want to
    // do double insert if this happened during load from db so we force state
    // to CHANGED, just in case
    newspell.state = inserted.second ? state : PLAYERSPELL_CHANGED;
    newspell.active = active;
    newspell.dependent = dependent;
    newspell.disabled = disabled;

    // replace spells in action bars and spellbook to bigger rank if only one
    // spell rank must be accessible
    if (newspell.active && !newspell.disabled &&
        !spellInfo->IsStackableWithRanks() && spellInfo->IsRanked()) {
      for (PlayerSpellMap::iterator itr2 = m_spells.begin();
           itr2 != m_spells.end(); ++itr2) {
        if (itr2->second.state == PLAYERSPELL_REMOVED)
          continue;

        SpellInfo const *i_spellInfo = sSpellMgr->GetSpellInfo(itr2->first);
        if (!i_spellInfo)
          continue;

        if (spellInfo->IsDifferentRankOf(i_spellInfo)) {
          if (itr2->second.active) {
            if (spellInfo->IsHighRankOf(i_spellInfo)) {
              if (IsInWorld()) // not send spell (re-/over-)learn packets at
                               // loading
                SendSupercededSpell(itr2->first, spellId);

              // mark old spell as disable (SMSG_SUPERCEDED_SPELL replace it in
              // client by new)
              itr2->second.active = false;
              if (itr2->second.state != PLAYERSPELL_NEW)
                itr2->second.state = PLAYERSPELL_CHANGED;
              superceded_old =
                  true; // new spell replace old in action bars and spell book.
            } else {
              if (IsInWorld()) // not send spell (re-/over-)learn packets at
                               // loading
                SendSupercededSpell(spellId, itr2->first);

              // mark new spell as disable (not learned yet for client and will
              // not learned)
              newspell.active = false;
              if (newspell.state != PLAYERSPELL_NEW)
                newspell.state = PLAYERSPELL_CHANGED;
            }
          }
        }
      }
    }

    // return false if spell disabled
    if (newspell.disabled)
      return false;
  }

  uint32 talentCost = sDBCManager.GetTalentSpellCost(spellId);

  // cast talents with SPELL_EFFECT_LEARN_SPELL (other dependent spells will
  // learned later as not auto-learned) note: all spells with
  // SPELL_EFFECT_LEARN_SPELL isn't passive
  if (!loading && talentCost > 0 &&
      spellInfo->HasEffect(SPELL_EFFECT_LEARN_SPELL)) {
    // ignore stance requirement for talent learn spell (stance set for spell
    // only for client spell description show)
    CastSpell(this, spellId, true);
  }
  // also cast passive spells (including all talents without
  // SPELL_EFFECT_LEARN_SPELL) with additional checks
  else if (spellInfo->IsPassive()) {
    if (HandlePassiveSpellLearn(spellInfo))
      CastSpell(this, spellId, true);
  } else if (spellInfo->HasEffect(SPELL_EFFECT_SKILL_STEP)) {
    CastSpell(this, spellId, true);
    return false;
  } else if (spellInfo->HasAttribute(SPELL_ATTR1_CAST_WHEN_LEARNED))
    CastSpell(this, spellId, true);

  // update used talent points count
  SetUsedTalentCount(GetUsedTalentCount() + talentCost);

  // update free primary prof.points (if any, can be none in case GM .learn
  // prof. learning)
  if (uint32 freeProfs = GetFreePrimaryProfessionPoints()) {
    if (spellInfo->IsPrimaryProfessionFirstRank())
      SetFreePrimaryProfessions(freeProfs - 1);
  }

  SkillLineAbilityMapBounds skill_bounds =
      sSpellMgr->GetSkillLineAbilityMapBounds(spellId);

  if (SpellLearnSkillNode const *spellLearnSkill =
          sSpellMgr->GetSpellLearnSkill(spellId)) {
    // add dependent skills if this spell is not learned from adding skill
    // already
    if (spellLearnSkill->skill != fromSkill) {
      uint32 skill_value = GetPureSkillValue(spellLearnSkill->skill);
      uint32 skill_max_value = GetPureMaxSkillValue(spellLearnSkill->skill);

      if (skill_value < spellLearnSkill->value)
        skill_value = spellLearnSkill->value;

      uint32 new_skill_max_value = spellLearnSkill->maxvalue == 0
                                       ? GetMaxSkillValueForLevel()
                                       : spellLearnSkill->maxvalue;

      if (skill_max_value < new_skill_max_value)
        skill_max_value = new_skill_max_value;

      SetSkill(spellLearnSkill->skill, spellLearnSkill->step, skill_value,
               skill_max_value);
    }
  } else {
    // not ranked skills
    for (SkillLineAbilityMap::const_iterator _spell_idx = skill_bounds.first;
         _spell_idx != skill_bounds.second; ++_spell_idx) {
      SkillLineEntry const *pSkill =
          sSkillLineStore.LookupEntry(_spell_idx->second->SkillLine);
      if (!pSkill)
        continue;

      if (pSkill->ID == fromSkill)
        continue;

      ///@todo: confirm if rogues start with lockpicking skill at level 1 but
      /// only receive the spell to use it at level 16
      // Also added for runeforging. It's already confirmed this happens upon
      // learning for Death Knights, not from character creation.
      if ((_spell_idx->second->AcquireMethod ==
               SKILL_LINE_ABILITY_LEARNED_ON_SKILL_LEARN &&
           !HasSkill(pSkill->ID)) ||
          ((pSkill->ID == SKILL_LOCKPICKING ||
            pSkill->ID == SKILL_RUNEFORGING) &&
           _spell_idx->second->TrivialSkillLineRankHigh == 0))
        LearnDefaultSkill(pSkill->ID, 0);
    }
  }

  // learn dependent spells
  SpellLearnSpellMapBounds spell_bounds =
      sSpellMgr->GetSpellLearnSpellMapBounds(spellId);

  for (SpellLearnSpellMap::const_iterator itr2 = spell_bounds.first;
       itr2 != spell_bounds.second; ++itr2) {
    if (!itr2->second.autoLearned) {
      if (!IsInWorld() ||
          !itr2->second.active) // at spells loading, no output, but allow save
        AddSpell(itr2->second.spell, itr2->second.active, true, true, false);
      else // at normal learning
        LearnSpell(itr2->second.spell, true);
    }
  }

  if (!GetSession()->PlayerLoading()) {
    // not ranked skills
    for (SkillLineAbilityMap::const_iterator _spell_idx = skill_bounds.first;
         _spell_idx != skill_bounds.second; ++_spell_idx) {
      UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LINE,
                                _spell_idx->second->SkillLine);
      UpdateAchievementCriteria(
          ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILLLINE_SPELLS,
          _spell_idx->second->SkillLine);
    }

    UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_LEARN_SPELL, spellId);
  }

  // return true (for send learn packet) only if spell active (in case ranked
  // spells) and not replace old spell
  return active && !disabled && !superceded_old;
}

void Player::AddTemporarySpell(uint32 spellId) {
  PlayerSpellMap::iterator itr = m_spells.find(spellId);
  // spell already added - do not do anything
  if (itr != m_spells.end())
    return;
  PlayerSpell *newspell = &m_spells[spellId];
  newspell->state = PLAYERSPELL_TEMPORARY;
  newspell->active = true;
  newspell->dependent = false;
  newspell->disabled = false;
}

void Player::RemoveTemporarySpell(uint32 spellId) {
  PlayerSpellMap::iterator itr = m_spells.find(spellId);
  // spell already not in list - do not do anything
  if (itr == m_spells.end())
    return;
  // spell has other state than temporary - do not change it
  if (itr->second.state != PLAYERSPELL_TEMPORARY)
    return;
  m_spells.erase(itr);
}

bool Player::HandlePassiveSpellLearn(SpellInfo const *spellInfo) {
  // note: form passives activated with shapeshift spells be implemented by
  // HandleShapeshiftBoosts instead of spell_learn_spell talent dependent
  // passives activated at form apply have proper stance data
  ShapeshiftForm form = GetShapeshiftForm();
  bool need_cast =
      (!spellInfo->Stances ||
       (form && (spellInfo->Stances & (UI64LIT(1) << (form - 1)))) ||
       (!form && spellInfo->HasAttribute(SPELL_ATTR2_NOT_NEED_SHAPESHIFT)));

  // Check EquippedItemClass
  // passive spells which apply aura and have an item requirement are to be
  // added manually, instead of casted
  if (spellInfo->EquippedItemClass >= 0) {
    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i) {
      if (spellInfo->Effects[i].IsAura()) {
        if (!HasAura(spellInfo->Id) && HasItemFitToSpellRequirements(spellInfo))
          AddAura(spellInfo->Id, this);
        return false;
      }
    }
  }

  if (spellInfo->HasAttribute(SPELL_ATTR8_MASTERY_SPECIALIZATION))
    need_cast &= IsCurrentSpecMasterySpell(spellInfo);

  // Check CasterAuraStates
  return need_cast && (!spellInfo->CasterAuraState ||
                       HasAuraState(AuraStateType(spellInfo->CasterAuraState)));
}

bool Player::IsCurrentSpecMasterySpell(SpellInfo const *spellInfo) const {
  if (TalentTabEntry const *talentTab =
          sTalentTabStore.LookupEntry(GetPrimaryTalentTree(GetActiveSpec())))
    return spellInfo->Id == talentTab->MasterySpellID[0] ||
           spellInfo->Id == talentTab->MasterySpellID[1];

  return false;
}

void Player::LearnSpell(uint32 spell_id, bool dependent,
                        uint32 fromSkill /*= 0*/) {
  PlayerSpellMap::iterator itr = m_spells.find(spell_id);

  bool disabled = (itr != m_spells.end()) ? itr->second.disabled : false;
  bool active = disabled ? itr->second.active : true;

  bool learning =
      AddSpell(spell_id, active, true, dependent, false, false, fromSkill);

  // prevent duplicated entires in spell book, also not send if not in world
  // (loading)
  if (learning && IsInWorld()) {
    WorldPacket data(SMSG_LEARNED_SPELL, 8);
    data << uint32(spell_id);
    data << uint32(0);
    SendDirectMessage(&data);
  }

  // learn all disabled higher ranks and required spells (recursive)
  if (disabled) {
    if (uint32 nextSpell = sSpellMgr->GetNextSpellInChain(spell_id)) {
      PlayerSpellMap::iterator iter = m_spells.find(nextSpell);
      if (iter != m_spells.end() && iter->second.disabled)
        LearnSpell(nextSpell, false, fromSkill);
    }

    SpellsRequiringSpellMapBounds spellsRequiringSpell =
        sSpellMgr->GetSpellsRequiringSpellBounds(spell_id);
    for (SpellsRequiringSpellMap::const_iterator itr2 =
             spellsRequiringSpell.first;
         itr2 != spellsRequiringSpell.second; ++itr2) {
      PlayerSpellMap::iterator iter2 = m_spells.find(itr2->second);
      if (iter2 != m_spells.end() && iter2->second.disabled)
        LearnSpell(itr2->second, false, fromSkill);
    }
  }

  if (CanControlPet(spell_id) && GetPet())
    PetSpellInitialize();

  if (Guild *guild = GetGuild())
    guild->UpdateMemberData(this, GUILD_MEMBER_DATA_PROFESSIONS, 0);
}

void Player::RemoveSpell(uint32 spell_id, bool disabled, bool learn_low_rank) {
  PlayerSpellMap::iterator itr = m_spells.find(spell_id);
  if (itr == m_spells.end())
    return;

  if (itr->second.state == PLAYERSPELL_REMOVED ||
      (disabled && itr->second.disabled) ||
      itr->second.state == PLAYERSPELL_TEMPORARY)
    return;

  // unlearn non talent higher ranks (recursive)
  if (uint32 nextSpell = sSpellMgr->GetNextSpellInChain(spell_id)) {
    if (HasSpell(nextSpell) && !sDBCManager.GetTalentSpellPos(nextSpell))
      RemoveSpell(nextSpell, disabled, false);
  }
  // unlearn spells dependent from recently removed spells
  SpellsRequiringSpellMapBounds spellsRequiringSpell =
      sSpellMgr->GetSpellsRequiringSpellBounds(spell_id);
  for (SpellsRequiringSpellMap::const_iterator itr2 =
           spellsRequiringSpell.first;
       itr2 != spellsRequiringSpell.second; ++itr2)
    RemoveSpell(itr2->second, disabled);

  // re-search, it can be corrupted in prev loop
  itr = m_spells.find(spell_id);
  if (itr == m_spells.end())
    return; // already unleared

  bool giveTalentPoints = disabled || !itr->second.disabled;

  bool cur_active = itr->second.active;
  bool cur_dependent = itr->second.dependent;

  if (disabled) {
    itr->second.disabled = disabled;
    if (itr->second.state != PLAYERSPELL_NEW)
      itr->second.state = PLAYERSPELL_CHANGED;
  } else {
    if (itr->second.state == PLAYERSPELL_NEW)
      m_spells.erase(itr);
    else
      itr->second.state = PLAYERSPELL_REMOVED;
  }

  RemoveOwnedAura(spell_id, GetGUID());

  // remove pet auras
  for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
    if (PetAura const *petSpell = sSpellMgr->GetPetAura(spell_id, i))
      RemovePetAura(petSpell);

  // free talent points
  uint32 talentCosts = sDBCManager.GetTalentSpellCost(spell_id);
  if (talentCosts > 0 && giveTalentPoints) {
    if (talentCosts < GetUsedTalentCount())
      SetUsedTalentCount(GetUsedTalentCount() - talentCosts);
    else
      SetUsedTalentCount(0);
  }

  // update free primary prof.points (if not overflow setting, can be in case GM
  // use before .learn prof. learning)
  SpellInfo const *spellInfo = sSpellMgr->GetSpellInfo(spell_id);
  if (spellInfo && spellInfo->IsPrimaryProfessionFirstRank()) {
    uint32 freeProfs = GetFreePrimaryProfessionPoints() + 1;
    if (freeProfs <= sWorld->getIntConfig(CONFIG_MAX_PRIMARY_TRADE_SKILL))
      SetFreePrimaryProfessions(freeProfs);
  }

  // remove dependent skill
  SpellLearnSkillNode const *spellLearnSkill =
      sSpellMgr->GetSpellLearnSkill(spell_id);
  if (spellLearnSkill) {
    uint32 prev_spell = sSpellMgr->GetPrevSpellInChain(spell_id);
    if (!prev_spell) // first rank, remove skill
      SetSkill(spellLearnSkill->skill, 0, 0, 0);
    else {
      // search prev. skill setting by spell ranks chain
      SpellLearnSkillNode const *prevSkill =
          sSpellMgr->GetSpellLearnSkill(prev_spell);
      while (!prevSkill && prev_spell) {
        prev_spell = sSpellMgr->GetPrevSpellInChain(prev_spell);
        prevSkill = sSpellMgr->GetSpellLearnSkill(
            sSpellMgr->GetFirstSpellInChain(prev_spell));
      }

      if (!prevSkill) // not found prev skill setting, remove skill
        SetSkill(spellLearnSkill->skill, 0, 0, 0);
      else // set to prev. skill setting values
      {
        uint32 skill_value = GetPureSkillValue(prevSkill->skill);
        uint32 skill_max_value = GetPureMaxSkillValue(prevSkill->skill);

        if (skill_value > prevSkill->value)
          skill_value = prevSkill->value;

        uint32 new_skill_max_value = prevSkill->maxvalue == 0
                                         ? GetMaxSkillValueForLevel()
                                         : prevSkill->maxvalue;

        if (skill_max_value > new_skill_max_value)
          skill_max_value = new_skill_max_value;

        SetSkill(prevSkill->skill, prevSkill->step, skill_value,
                 skill_max_value);
      }
    }
  }

  // remove dependent spells
  SpellLearnSpellMapBounds spell_bounds =
      sSpellMgr->GetSpellLearnSpellMapBounds(spell_id);

  for (SpellLearnSpellMap::const_iterator itr2 = spell_bounds.first;
       itr2 != spell_bounds.second; ++itr2)
    RemoveSpell(itr2->second.spell, disabled);

  // activate lesser rank in spellbook/action bar, and cast it if need
  bool prev_activate = false;

  if (uint32 prev_id = sSpellMgr->GetPrevSpellInChain(spell_id)) {
    // if talent then lesser rank also talent and need learn
    if (talentCosts) {
      // I cannot see why mangos has these lines.
      // if (learn_low_rank)
      //    learnSpell(prev_id, false);
    }
    // if ranked non-stackable spell: need activate lesser rank and update
    // dendence state
    /// No need to check for spellInfo != nullptr here because if cur_active is
    /// true, then that means that the spell was already in m_spells, and only
    /// valid spells can be pushed there.
    else if (cur_active && !spellInfo->IsStackableWithRanks() &&
             spellInfo->IsRanked()) {
      // need manually update dependence state (learn spell ignore like
      // attempts)
      PlayerSpellMap::iterator prev_itr = m_spells.find(prev_id);
      if (prev_itr != m_spells.end()) {
        if (prev_itr->second.dependent != cur_dependent) {
          prev_itr->second.dependent = cur_dependent;
          if (prev_itr->second.state != PLAYERSPELL_NEW)
            prev_itr->second.state = PLAYERSPELL_CHANGED;
        }

        // now re-learn if need re-activate
        if (!prev_itr->second.active && learn_low_rank) {
          if (AddSpell(prev_id, true, false, prev_itr->second.dependent,
                       prev_itr->second.disabled)) {
            // downgrade spell ranks in spellbook and action bar
            SendSupercededSpell(spell_id, prev_id);
            prev_activate = true;
          }
        }
      }
    }
  }

  if (spell_id == 46917 && m_canTitanGrip) {
    RemoveAurasDueToSpell(m_titanGripPenaltySpellId);
    SetCanTitanGrip(false);
  }

  if (m_canDualWield) {
    SpellInfo const *spellInfo = sSpellMgr->GetSpellInfo(spell_id);
    if (spellInfo->IsPassive()) {
      for (int i = 0; i < MAX_SPELL_EFFECTS; i++)
        if (spellInfo->Effects[i].Effect == SPELL_EFFECT_DUAL_WIELD) {
          SetCanDualWield(false);
          break;
        }
    }
  }

  if (sWorld->getBoolConfig(CONFIG_OFFHAND_CHECK_AT_SPELL_UNLEARN))
    AutoUnequipOffhandIfNeed();

  // remove from spell book if not replaced by lesser rank
  if (!prev_activate) {
    WorldPackets::Spells::UnlearnedSpells unlearnedSpells;
    unlearnedSpells.SpellID = spell_id;
    SendDirectMessage(unlearnedSpells.Write());
  }

  if (Guild *guild = GetGuild())
    guild->UpdateMemberData(this, GUILD_MEMBER_DATA_PROFESSIONS, 0);
}

void Player::RemoveArenaSpellCooldowns(bool removeActivePetCooldowns) {
  // remove cooldowns on spells that have < 10 min CD
  GetSpellHistory()->ResetCooldowns(
      [](SpellHistory::CooldownStorageType::iterator itr) -> bool {
        SpellInfo const *spellInfo = sSpellMgr->AssertSpellInfo(itr->first);
        return spellInfo->RecoveryTime < 10 * MINUTE * IN_MILLISECONDS &&
               spellInfo->CategoryRecoveryTime < 10 * MINUTE * IN_MILLISECONDS;
      },
      true);

  // pet cooldowns
  if (removeActivePetCooldowns)
    if (Pet *pet = GetPet())
      pet->GetSpellHistory()->ResetAllCooldowns();
}

uint32 Player::GetNextResetTalentsCost() const {
  // The first time reset costs 1 gold
  if (GetTalentResetCost() < 1 * GOLD)
    return 1 * GOLD;
  // then 5 gold
  else if (GetTalentResetCost() < 5 * GOLD)
    return 5 * GOLD;
  // After that it increases in increments of 5 gold
  else if (GetTalentResetCost() < 10 * GOLD)
    return 10 * GOLD;
  else {
    uint64 months = (GameTime::GetGameTime() - GetTalentResetTime()) / MONTH;
    if (months > 0) {
      // This cost will be reduced by a rate of 5 gold per month
      int32 new_cost = int32(GetTalentResetCost() - 5 * GOLD * months);
      // to a minimum of 10 gold.
      return (new_cost < 10 * GOLD ? 10 * GOLD : new_cost);
    } else {
      // After that it increases in increments of 5 gold
      int32 new_cost = GetTalentResetCost() + 5 * GOLD;
      // until it hits a cap of 50 gold.
      if (new_cost > 50 * GOLD)
        new_cost = 50 * GOLD;
      return new_cost;
    }
  }
}

bool Player::ResetTalents(bool no_cost) {
  sScriptMgr->OnPlayerTalentsReset(this, no_cost);

  // not need after this call
  if (HasAtLoginFlag(AT_LOGIN_RESET_TALENTS))
    RemoveAtLoginFlag(AT_LOGIN_RESET_TALENTS, true);

  uint32 talentPointsForLevel = CalculateTalentsPoints();

  if (!GetUsedTalentCount()) {
    SetFreeTalentPoints(talentPointsForLevel);
    return false;
  }

  uint32 cost = 0;

  if (!no_cost && !sWorld->getBoolConfig(CONFIG_NO_RESET_TALENT_COST)) {
    cost = GetNextResetTalentsCost();

    if (!HasEnoughMoney(uint64(cost))) {
      SendBuyError(BUY_ERR_NOT_ENOUGHT_MONEY, nullptr, 0, 0);
      return false;
    }
  }

  RemovePet(nullptr, PET_SAVE_DISMISS, true);

  for (uint32 talentId = 0; talentId < sTalentStore.GetNumRows(); ++talentId) {
    TalentEntry const *talentInfo = sTalentStore.LookupEntry(talentId);

    if (!talentInfo)
      continue;

    TalentTabEntry const *talentTabInfo =
        sTalentTabStore.LookupEntry(talentInfo->TabID);

    if (!talentTabInfo)
      continue;

    // unlearn only talents for character class
    // some spell learned by one class as normal spells or know at creation but
    // another class learn it as talent, to prevent unexpected lost normal
    // learned spell skip another class talents
    if ((getClassMask() & talentTabInfo->ClassMask) == 0)
      continue;

    for (int8 rank = MAX_TALENT_RANK - 1; rank >= 0; --rank) {
      // skip non-existing talent ranks
      if (talentInfo->SpellRank[rank] == 0)
        continue;
      SpellInfo const *_spellEntry =
          sSpellMgr->GetSpellInfo(talentInfo->SpellRank[rank]);
      if (!_spellEntry)
        continue;
      RemoveSpell(talentInfo->SpellRank[rank], true);
      // search for spells that the talent teaches and unlearn them
      for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
        if (_spellEntry->Effects[i].TriggerSpell > 0 &&
            _spellEntry->Effects[i].Effect == SPELL_EFFECT_LEARN_SPELL)
          RemoveSpell(_spellEntry->Effects[i].TriggerSpell, true);
      // if this talent rank can be found in the PlayerTalentMap, mark the
      // talent as removed so it gets deleted
      PlayerTalentMap::iterator plrTalent =
          GetTalentMap(GetActiveSpec())->find(talentInfo->SpellRank[rank]);
      if (plrTalent != GetTalentMap(GetActiveSpec())->end())
        plrTalent->second->state = PLAYERSPELL_REMOVED;
    }
  }

  // Remove spec specific spells
  uint32 const *talentTabs = sDBCManager.GetTalentTabPages(getClass());
  for (uint32 i = 0; i < MAX_TALENT_TABS; ++i) {
    if (std::vector<uint32> const *specSpells =
            sDBCManager.GetTalentTreePrimarySpells(talentTabs[i]))
      for (size_t j = 0; j < specSpells->size(); ++j)
        RemoveSpell(specSpells->at(j), true);

    TalentTabEntry const *talentTabInfo =
        sTalentTabStore.LookupEntry(talentTabs[i]);
    for (uint32 j = 0; j < MAX_MASTERY_SPELLS; ++j)
      if (uint32 mastery = talentTabInfo->MasterySpellID[j])
        RemoveAurasDueToSpell(mastery);
  }

  SetPrimaryTalentTree(GetActiveSpec(), 0);
  SetFreeTalentPoints(talentPointsForLevel);

  // We consider resetting talents as changing a specialization as well as we
  // now can pick an entirely new primary talent tree which equals changing
  // specs
  RemoveAurasWithInterruptFlags(SpellAuraInterruptFlags2::ChangeSpec);

  CharacterDatabaseTransaction trans = CharacterDatabase.BeginTransaction();
  _SaveTalents(trans);
  _SaveSpells(trans);
  CharacterDatabase.CommitTransaction(trans);

  if (!no_cost) {
    ModifyMoney(-(int64)cost);
    UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_GOLD_SPENT_FOR_TALENTS,
                              cost);
    UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_NUMBER_OF_TALENT_RESETS,
                              1);

    SetTalentResetCost(cost);
    SetTalentResetTime(GameTime::GetGameTime());
  }

  /* when prev line will dropped use next line
  if (Pet* pet = GetPet())
  {
      if (pet->getPetType() == HUNTER_PET &&
  !pet->GetCreatureTemplate()->IsTameable(CanTameExoticPets()))
          RemovePet(nullptr, PET_SAVE_NOT_IN_SLOT, true);
  }
  */

  return true;
}

Mail *Player::GetMail(uint32 id) {
  for (PlayerMails::iterator itr = m_mail.begin(); itr != m_mail.end(); ++itr)
    if ((*itr)->messageID == id)
      return (*itr);

  return nullptr;
}

void Player::BuildCreateUpdateBlockForPlayer(UpdateData *data,
                                             Player *target) const {
  if (target == this) {
    for (uint8 i = 0; i < EQUIPMENT_SLOT_END; ++i) {
      if (m_items[i] == nullptr)
        continue;

      m_items[i]->BuildCreateUpdateBlockForPlayer(data, target);
    }

    for (uint8 i = INVENTORY_SLOT_BAG_START; i < BANK_SLOT_BAG_END; ++i) {
      if (m_items[i] == nullptr)
        continue;

      m_items[i]->BuildCreateUpdateBlockForPlayer(data, target);
    }
  }

  Unit::BuildCreateUpdateBlockForPlayer(data, target);
}

void Player::DestroyForPlayer(Player *target, bool onDeath /*= false*/) const {
  Unit::DestroyForPlayer(target, onDeath);

  for (uint8 i = 0; i < EQUIPMENT_SLOT_END; ++i) {
    if (m_items[i] == nullptr)
      continue;

    m_items[i]->DestroyForPlayer(target);
  }

  if (target == this) {
    for (uint8 i = INVENTORY_SLOT_BAG_START; i < BANK_SLOT_BAG_END; ++i) {
      if (m_items[i] == nullptr)
        continue;

      m_items[i]->DestroyForPlayer(target);
    }
  }
}

bool Player::HasSpell(uint32 spell) const {
  PlayerSpellMap::const_iterator itr = m_spells.find(spell);
  return (itr != m_spells.end() && itr->second.state != PLAYERSPELL_REMOVED &&
          !itr->second.disabled);
}

bool Player::HasTalent(uint32 spell, uint8 spec) const {
  PlayerTalentMap::const_iterator itr = GetTalentMap(spec)->find(spell);
  return (itr != GetTalentMap(spec)->end() &&
          itr->second->state != PLAYERSPELL_REMOVED);
}

bool Player::HasActiveSpell(uint32 spell) const {
  PlayerSpellMap::const_iterator itr = m_spells.find(spell);
  return (itr != m_spells.end() && itr->second.state != PLAYERSPELL_REMOVED &&
          itr->second.active && !itr->second.disabled);
}

/**
 * Deletes a character from the database
 *
 * The way characters will be deleted is decided based on the config option.
 *
 * @see Player::DeleteOldCharacters
 *
 * @param playerguid       the low-GUID from the player which should be deleted
 * @param accountId        the account id from the player
 * @param updateRealmChars when this flag is set, the amount of characters on
 * that realm will be updated in the realmlist
 * @param deleteFinally    if this flag is set, the config option will be
 * ignored and the character will be permanently removed from the database
 */
void Player::DeleteFromDB(ObjectGuid playerguid, uint32 accountId,
                          bool updateRealmChars, bool deleteFinally) {
  // Avoid realm-update for non-existing account
  if (accountId == 0)
    updateRealmChars = false;

  // Convert guid to low GUID for CharacterNameData, but also other methods on
  // success
  ObjectGuid::LowType guid = playerguid.GetCounter();
  uint32 charDelete_method = sWorld->getIntConfig(CONFIG_CHARDELETE_METHOD);
  CharacterCacheEntry const *characterInfo =
      sCharacterCache->GetCharacterCacheByGuid(playerguid);
  std::string name;
  if (characterInfo)
    name = characterInfo->Name;

  if (deleteFinally)
    charDelete_method = CHAR_DELETE_REMOVE;
  else if (characterInfo) // To avoid a query, we select loaded data. If it
                          // doesn't exist, return.
  {
    // Define the required variables
    uint32 charDelete_minLvl =
        sWorld->getIntConfig(characterInfo->Class != CLASS_DEATH_KNIGHT
                                 ? CONFIG_CHARDELETE_MIN_LEVEL
                                 : CONFIG_CHARDELETE_DEATH_KNIGHT_MIN_LEVEL);

    // if we want to finalize the character removal or the character does not
    // meet the level requirement of either heroic or non-heroic settings, we
    // set it to mode CHAR_DELETE_REMOVE
    if (characterInfo->Level < charDelete_minLvl)
      charDelete_method = CHAR_DELETE_REMOVE;
  }

  CharacterDatabaseTransaction trans = CharacterDatabase.BeginTransaction();
  if (ObjectGuid::LowType guildId =
          sCharacterCache->GetCharacterGuildIdByGuid(playerguid))
    if (Guild *guild = sGuildMgr->GetGuildById(guildId))
      guild->DeleteMember(trans, playerguid, false, false, true);

  // close player ticket if any
  GmTicket *ticket = sTicketMgr->GetTicketByPlayer(playerguid);
  if (ticket)
    sTicketMgr->CloseTicket(ticket->GetId(), playerguid);

  // remove from arena teams
  LeaveAllArenaTeams(playerguid);

  // the player was uninvited already on logout so just remove from group
  CharacterDatabasePreparedStatement *stmt =
      CharacterDatabase.GetPreparedStatement(CHAR_SEL_GROUP_MEMBER);

  stmt->SetData(0, guid);

  PreparedQueryResult resultGroup = CharacterDatabase.Query(stmt);

  if (resultGroup)
    if (Group *group =
            sGroupMgr->GetGroupByDbStoreId((*resultGroup)[0].Get<uint32>()))
      RemoveFromGroup(group, playerguid);

  // Remove signs from petitions (also remove petitions if owner);
  RemovePetitionsAndSigns(playerguid, CHARTER_TYPE_ANY);

  switch (charDelete_method) {
    // Completely remove from the database
  case CHAR_DELETE_REMOVE: {
    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHAR_COD_ITEM_MAIL);
    stmt->SetData(0, guid);
    PreparedQueryResult resultMail = CharacterDatabase.Query(stmt);

    if (resultMail) {
      std::unordered_map<uint32, std::vector<Item *>> itemsByMail;

      stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_MAILITEMS);
      stmt->SetData(0, guid);
      PreparedQueryResult resultItems = CharacterDatabase.Query(stmt);

      if (resultItems) {
        do {
          Field *fields = resultItems->Fetch();
          uint32 mailId = fields[15].Get<uint32>();
          if (Item *mailItem =
                  _LoadMailedItem(playerguid, nullptr, mailId, nullptr, fields))
            itemsByMail[mailId].push_back(mailItem);

        } while (resultItems->NextRow());
      }

      do {
        Field *mailFields = resultMail->Fetch();

        uint32 mail_id = mailFields[0].Get<uint32>();
        uint8 mailType = mailFields[1].Get<uint8>();
        uint16 mailTemplateId = mailFields[2].Get<uint16>();
        uint32 sender = mailFields[3].Get<uint32>();
        std::string subject = mailFields[4].Get<std::string>();
        std::string body = mailFields[5].Get<std::string>();
        uint64 money = mailFields[6].Get<uint64>();
        bool has_items = mailFields[7].Get<bool>();

        // We can return mail now
        // So firstly delete the old one
        stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_MAIL_BY_ID);
        stmt->SetData(0, mail_id);
        trans->Append(stmt);

        // Mail is not from player
        if (mailType != MAIL_NORMAL) {
          if (has_items) {
            stmt = CharacterDatabase.GetPreparedStatement(
                CHAR_DEL_MAIL_ITEM_BY_ID);
            stmt->SetData(0, mail_id);
            trans->Append(stmt);
          }
          continue;
        }

        MailDraft draft(subject, body);
        if (mailTemplateId)
          draft =
              MailDraft(mailTemplateId, false); // items are already included

        auto itemsItr = itemsByMail.find(mail_id);
        if (itemsItr != itemsByMail.end()) {
          for (Item *item : itemsItr->second)
            draft.AddItem(item);

          // MailDraft will take care of freeing memory
          itemsByMail.erase(itemsItr);
        }

        stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_MAIL_ITEM_BY_ID);
        stmt->SetData(0, mail_id);
        trans->Append(stmt);

        uint32 pl_account =
            sCharacterCache->GetCharacterAccountIdByGuid(playerguid);

        draft.AddMoney(money).SendReturnToSender(pl_account, guid, sender,
                                                 trans);
      } while (resultMail->NextRow());
    }

    // Unsummon and delete for pets in world is not required: player deleted
    // from CLI or character list with not loaded pet. NOW we can finally clear
    // other DB data related to character
    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHAR_PETS);
    stmt->SetData(0, guid);
    PreparedQueryResult resultPets = CharacterDatabase.Query(stmt);

    if (resultPets) {
      do {
        ObjectGuid::LowType petguidlow = (*resultPets)[0].Get<uint32>();
        Pet::DeleteFromDB(petguidlow);
      } while (resultPets->NextRow());
    }

    // Delete char from social list of online chars
    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHAR_SOCIAL);
    stmt->SetData(0, guid);

    if (PreparedQueryResult resultFriends = CharacterDatabase.Query(stmt)) {
      do {
        if (Player *playerFriend = ObjectAccessor::FindPlayer(ObjectGuid(
                HighGuid::Player, 0, (*resultFriends)[0].Get<uint32>()))) {
          playerFriend->GetSocial()->RemoveFromSocialList(playerguid,
                                                          SOCIAL_FLAG_ALL);
          sSocialMgr->SendFriendStatus(playerFriend, FRIEND_REMOVED,
                                       playerguid);
        }
      } while (resultFriends->NextRow());
    }

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHARACTER);
    stmt->SetData(0, guid);
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_PLAYER_ACCOUNT_DATA);
    stmt->SetData(0, guid);
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_DECLINED_NAME);
    stmt->SetData(0, guid);
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_ACTION);
    stmt->SetData(0, guid);
    trans->Append(stmt);

    stmt =
        CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHARACTER_ARENA_STATS);
    stmt->SetData(0, guid);
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_AURA);
    stmt->SetData(0, guid);
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_PLAYER_BGDATA);
    stmt->SetData(0, guid);
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_BATTLEGROUND_RANDOM);
    stmt->SetData(0, guid);
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_PLAYER_CURRENCY);
    stmt->SetData(0, guid);
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_GIFT);
    stmt->SetData(0, guid);
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_PLAYER_HOMEBIND);
    stmt->SetData(0, guid);
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_INSTANCE);
    stmt->SetData(0, guid);
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_INVENTORY);
    stmt->SetData(0, guid);
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_QUESTSTATUS);
    stmt->SetData(0, guid);
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(
        CHAR_DEL_CHAR_QUESTSTATUS_REWARDED);
    stmt->SetData(0, guid);
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_REPUTATION);
    stmt->SetData(0, guid);
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_SPELL);
    stmt->SetData(0, guid);
    trans->Append(stmt);

    stmt =
        CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_SPELL_COOLDOWNS);
    stmt->SetData(0, guid);
    trans->Append(stmt);

    if (sWorld->getBoolConfig(CONFIG_DELETE_CHARACTER_TICKET_TRACE)) {
      stmt = CharacterDatabase.GetPreparedStatement(
          CHAR_UPD_PLAYER_GM_TICKETS_ON_CHAR_DELETION);
      stmt->SetData(0, guid);
      trans->Append(stmt);
    } else {
      stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_PLAYER_GM_TICKETS);
      stmt->SetData(0, guid);
      trans->Append(stmt);
    }

    stmt =
        CharacterDatabase.GetPreparedStatement(CHAR_DEL_ITEM_INSTANCE_BY_OWNER);
    stmt->SetData(0, guid);
    trans->Append(stmt);

    stmt =
        CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_SOCIAL_BY_FRIEND);
    stmt->SetData(0, guid);
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_SOCIAL_BY_GUID);
    stmt->SetData(0, guid);
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_MAIL);
    stmt->SetData(0, guid);
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_MAIL_ITEMS);
    stmt->SetData(0, guid);
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_PET_BY_OWNER);
    stmt->SetData(0, guid);
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(
        CHAR_DEL_CHAR_PET_DECLINEDNAME_BY_OWNER);
    stmt->SetData(0, guid);
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_ACHIEVEMENTS);
    stmt->SetData(0, guid);
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(
        CHAR_DEL_CHAR_ACHIEVEMENT_PROGRESS);
    stmt->SetData(0, guid);
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_EQUIPMENTSETS);
    stmt->SetData(0, guid);
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(
        CHAR_DEL_GUILD_EVENTLOG_BY_PLAYER);
    stmt->SetData(0, guid);
    stmt->SetData(1, guid);
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(
        CHAR_DEL_GUILD_BANK_EVENTLOG_BY_PLAYER);
    stmt->SetData(0, guid);
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_GLYPHS);
    stmt->SetData(0, guid);
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(
        CHAR_DEL_CHARACTER_QUESTSTATUS_DAILY);
    stmt->SetData(0, guid);
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(
        CHAR_DEL_CHARACTER_QUESTSTATUS_WEEKLY);
    stmt->SetData(0, guid);
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(
        CHAR_DEL_CHARACTER_QUESTSTATUS_MONTHLY);
    stmt->SetData(0, guid);
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(
        CHAR_DEL_CHARACTER_QUESTSTATUS_SEASONAL);
    stmt->SetData(0, guid);
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_TALENT);
    stmt->SetData(0, guid);
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_SKILLS);
    stmt->SetData(0, guid);
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_STATS);
    stmt->SetData(0, guid);
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_FISHINGSTEPS);
    stmt->SetData(0, guid);
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(
        CHAR_DEL_CHARACTER_REWARDSTATUS_LFG);
    stmt->SetData(0, guid);
    trans->Append(stmt);

    Corpse::DeleteFromDB(playerguid, trans);
    break;
  }
  // The character gets unlinked from the account, the name gets freed up and
  // appears as deleted ingame
  case CHAR_DELETE_UNLINK: {
    stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_DELETE_INFO);
    stmt->SetData(0, guid);

    trans->Append(stmt);
    break;
  }
  default:
    LOG_INFO("entities.player.cheat",
             "Player::DeleteFromDB: Tried to delete player (%s) with "
             "unsupported delete method (%u).",
             playerguid.ToString().c_str(), charDelete_method);

    if (trans->GetSize() > 0)
      CharacterDatabase.CommitTransaction(trans);
    return;
  }

  CharacterDatabase.CommitTransaction(trans);

  if (updateRealmChars)
    sWorld->UpdateRealmCharCount(accountId);

  sCharacterCache->DeleteCharacterCacheEntry(playerguid, name);
}

/**
 * Characters which were kept back in the database after being deleted and are
 * now too old (see config option "CharDelete.KeepDays"), will be completely
 * deleted.
 *
 * @see Player::DeleteFromDB
 */
void Player::DeleteOldCharacters() {
  uint32 keepDays = sWorld->getIntConfig(CONFIG_CHARDELETE_KEEP_DAYS);
  if (!keepDays)
    return;

  Player::DeleteOldCharacters(keepDays);
}

/**
 * Characters which were kept back in the database after being deleted and are
 * older than the specified amount of days, will be completely deleted.
 *
 * @see Player::DeleteFromDB
 *
 * @param keepDays overwrite the config option by another amount of days
 */
void Player::DeleteOldCharacters(uint32 keepDays) {
  LOG_INFO("entities.player",
           "Player::DeleteOldCharacters: Deleting all characters which have "
           "been deleted %u days before...",
           keepDays);

  CharacterDatabasePreparedStatement *stmt =
      CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHAR_OLD_CHARS);
  stmt->SetData(0, uint32(GameTime::GetGameTime() - time_t(keepDays * DAY)));
  PreparedQueryResult result = CharacterDatabase.Query(stmt);

  if (result) {
    LOG_DEBUG("entities.player",
              "Player::DeleteOldCharacters: Found " UI64FMTD
              " character(s) to delete",
              result->GetRowCount());
    do {
      Field *fields = result->Fetch();
      Player::DeleteFromDB(
          ObjectGuid(HighGuid::Player, fields[0].Get<uint32>()),
          fields[1].Get<uint32>(), true, true);
    } while (result->NextRow());
  }
}

/* Preconditions:
  - a resurrectable corpse must not be loaded for the player (only bones)
  - the player must be in world
*/
void Player::BuildPlayerRepop() {
  WorldPacket data(SMSG_PRE_RESURRECT, GetPackGUID().size());
  data << GetPackGUID();
  SendDirectMessage(&data);

  if (getRace() == RACE_NIGHTELF)
    CastSpell(this, 20584, true);
  CastSpell(this, 8326, true);

  RemoveAurasWithInterruptFlags(SpellAuraInterruptFlags::Release);

  // there must be SMSG.FORCE_RUN_SPEED_CHANGE, SMSG.FORCE_SWIM_SPEED_CHANGE,
  // SMSG.MOVE_WATER_WALK there must be SMSG.STOP_MIRROR_TIMER

  // the player cannot have a corpse already on current map, only bones which
  // are not returned by GetCorpse
  WorldLocation corpseLocation = GetCorpseLocation();
  if (corpseLocation.GetMapId() == GetMapId()) {
    LOG_ERROR("entities.player",
              "Player::BuildPlayerRepop: Player '%s' (%s) already has a corpse",
              GetName().c_str(), GetGUID().ToString().c_str());
    return;
  }

  // create a corpse and place it at the player's location
  Corpse *corpse = CreateCorpse();
  if (!corpse) {
    LOG_ERROR(
        "entities.player",
        "Player::BuildPlayerRepop: Error creating corpse for player '%s' (%s)",
        GetName().c_str(), GetGUID().ToString().c_str());
    return;
  }
  GetMap()->AddToMap(corpse);

  // convert player body to ghost
  setDeathState(DEAD);
  SetHealth(1);

  SetWaterWalking(true);
  if (!GetSession()->isLogingOut() && !HasUnitState(UNIT_STATE_STUNNED))
    SetRooted(false);

  // BG - remove insignia related
  RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_SKINNABLE);

  int32 corpseReclaimDelay = CalculateCorpseReclaimDelay();

  if (corpseReclaimDelay >= 0)
    SendCorpseReclaimDelay(corpseReclaimDelay);

  // to prevent cheating
  corpse->ResetGhostTime();

  StopMirrorTimers(); // disable timers(bars)

  // OnPlayerRepop hook
  sScriptMgr->OnPlayerRepop(this);
}

void Player::ResurrectPlayer(float restore_percent, bool applySickness) {
  WorldPackets::Misc::DeathReleaseLoc packet;
  packet.MapID = -1;
  SendDirectMessage(packet.Write());

  // speed change, land walk

  // remove death flag + set aura
  RemoveFlag(PLAYER_FLAGS, PLAYER_FLAGS_IS_OUT_OF_BOUNDS);

  // This must be called always even on Players with race != RACE_NIGHTELF in
  // case of faction change
  RemoveAurasDueToSpell(20584); // RACE_NIGHTELF speed bonuses
  RemoveAurasDueToSpell(8326);  // SPELL_AURA_GHOST

  if (GetSession()->IsARecruiter() || (GetSession()->GetRecruiterId() != 0))
    SetFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_REFER_A_FRIEND);

  setDeathState(ALIVE);

  SetWaterWalking(false, true);
  if (!HasUnitState(UNIT_STATE_STUNNED))
    SetRooted(false);

  m_deathTimer = 0;

  // set health/powers (0- will be set in caller)
  if (restore_percent > 0.0f) {
    SetHealth(uint32(GetMaxHealth() * restore_percent));
    SetPower(POWER_MANA, uint32(GetMaxPower(POWER_MANA) * restore_percent));
    SetPower(POWER_RAGE, 0);
    SetPower(POWER_ENERGY, uint32(GetMaxPower(POWER_ENERGY) * restore_percent));
    SetPower(POWER_FOCUS, uint32(GetMaxPower(POWER_FOCUS) * restore_percent));
    SetPower(POWER_ECLIPSE, 0);
  }

  // trigger update zone for alive state zone updates
  uint32 newzone, newarea;
  GetZoneAndAreaId(newzone, newarea);
  UpdateZone(newzone, newarea);
  sOutdoorPvPMgr->HandlePlayerResurrects(this, newzone);

  if (InBattleground()) {
    if (Battleground *bg = GetBattleground())
      bg->HandlePlayerResurrect(this);
  }

  // update visibility
  UpdateObjectVisibility();

  // recast lost by death auras of any items held in the inventory
  CastAllObtainSpells();

  if (!applySickness)
    return;

  // Characters from level 1-10 are not affected by resurrection sickness.
  // Characters from level 11-19 will suffer from one minute of sickness
  // for each level they are above 10.
  // Characters level 20 and up suffer from ten minutes of sickness.
  int32 startLevel = sWorld->getIntConfig(CONFIG_DEATH_SICKNESS_LEVEL);

  if (int32(getLevel()) >= startLevel) {
    // set resurrection sickness
    CastSpell(this, 15007, true);

    // not full duration
    if (int32(getLevel()) < startLevel + 9) {
      int32 delta = (int32(getLevel()) - startLevel + 1) * MINUTE;

      if (Aura *aur = GetAura(15007, GetGUID())) {
        aur->SetDuration(delta * IN_MILLISECONDS);
      }
    }
  }
}

void Player::KillPlayer() {
  if (IsFlying() && !GetTransport())
    GetMotionMaster()->MoveFall();

  SetRooted(true);

  StopMirrorTimers(); // disable timers(bars)

  setDeathState(CORPSE);
  // SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_IN_PVP);

  SetUInt32Value(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_NONE);
  ApplyModFlag(PLAYER_FIELD_BYTES, PLAYER_FIELD_BYTE_RELEASE_TIMER,
               !sMapStore.LookupEntry(GetMapId())->Instanceable() &&
                   !HasAuraType(SPELL_AURA_PREVENT_RESURRECTION));

  // 6 minutes until repop at graveyard
  m_deathTimer = 6 * MINUTE * IN_MILLISECONDS;

  UpdateCorpseReclaimDelay(); // dependent at use SetDeathPvP() call before kill

  int32 corpseReclaimDelay = CalculateCorpseReclaimDelay();

  if (corpseReclaimDelay >= 0)
    SendCorpseReclaimDelay(corpseReclaimDelay);

  // don't create corpse at this moment, player might be falling

  // update visibility
  UpdateObjectVisibility();
}

void Player::OfflineResurrect(ObjectGuid const &guid,
                              CharacterDatabaseTransaction &trans) {
  Corpse::DeleteFromDB(guid, trans);
  CharacterDatabasePreparedStatement *stmt =
      CharacterDatabase.GetPreparedStatement(CHAR_UPD_ADD_AT_LOGIN_FLAG);
  stmt->SetData(0, AT_LOGIN_RESURRECT);
  stmt->SetData(1, guid.GetCounter());
  CharacterDatabase.ExecuteOrAppend(trans, stmt);
}

Corpse *Player::CreateCorpse() {
  // prevent the existence of 2 corpses for one player
  SpawnCorpseBones();

  uint32 _cfb1, _cfb2;

  Corpse *corpse = new Corpse((m_ExtraFlags & PLAYER_EXTRA_PVP_DEATH)
                                  ? CORPSE_RESURRECTABLE_PVP
                                  : CORPSE_RESURRECTABLE_PVE);
  SetPvPDeath(false);

  if (!corpse->Create(GetMap()->GenerateLowGuid<HighGuid::Corpse>(), this)) {
    delete corpse;
    return nullptr;
  }

  _corpseLocation.WorldRelocate(*this);

  uint8 skin = GetByteValue(PLAYER_BYTES, PLAYER_BYTES_OFFSET_SKIN_ID);
  uint8 face = GetByteValue(PLAYER_BYTES, PLAYER_BYTES_OFFSET_FACE_ID);
  uint8 hairstyle =
      GetByteValue(PLAYER_BYTES, PLAYER_BYTES_OFFSET_HAIR_STYLE_ID);
  uint8 haircolor =
      GetByteValue(PLAYER_BYTES, PLAYER_BYTES_OFFSET_HAIR_COLOR_ID);
  uint8 facialStyle =
      GetByteValue(PLAYER_BYTES_2, PLAYER_BYTES_2_OFFSET_FACIAL_STYLE);

  _cfb1 = ((0x00) | (getRace() << 8) |
           (GetByteValue(PLAYER_BYTES_3, PLAYER_BYTES_3_OFFSET_GENDER) << 16) |
           (skin << 24));
  _cfb2 = ((face) | (hairstyle << 8) | (haircolor << 16) | (facialStyle << 24));

  corpse->SetUInt32Value(CORPSE_FIELD_BYTES_1, _cfb1);
  corpse->SetUInt32Value(CORPSE_FIELD_BYTES_2, _cfb2);

  uint32 flags = CORPSE_FLAG_UNK2;

  if (HasFlag(PLAYER_FLAGS, PLAYER_FLAGS_HIDE_HELM))
    flags |= CORPSE_FLAG_HIDE_HELM;

  if (HasFlag(PLAYER_FLAGS, PLAYER_FLAGS_HIDE_CLOAK))
    flags |= CORPSE_FLAG_HIDE_CLOAK;

  if (InBattleground() && !InArena())
    flags |= CORPSE_FLAG_LOOTABLE; // to be able to remove insignia

  corpse->SetUInt32Value(CORPSE_FIELD_FLAGS, flags);
  corpse->SetUInt32Value(CORPSE_FIELD_DISPLAY_ID, GetNativeDisplayId());

  uint32 iDisplayID;
  uint32 iIventoryType;
  uint32 _cfi;

  for (uint8 i = 0; i < EQUIPMENT_SLOT_END; i++) {
    if (m_items[i]) {
      iDisplayID = m_items[i]->GetDisplayId();
      iIventoryType = m_items[i]->GetTemplate()->GetInventoryType();

      _cfi = iDisplayID | (iIventoryType << 24);
      corpse->SetUInt32Value(CORPSE_FIELD_ITEM + i, _cfi);
    }
  }

  // register for player, but not show
  GetMap()->AddCorpse(corpse);

  // we do not need to save corpses for BG/arenas
  if (!GetMap()->IsBattlegroundOrArena())
    corpse->SaveToDB();

  return corpse;
}

void Player::SpawnCorpseBones(bool triggerSave /*= true*/) {
  _corpseLocation.WorldRelocate();
  if (GetMap()->ConvertCorpseToBones(GetGUID()))
    if (triggerSave &&
        !GetSession()->PlayerLogoutWithSave()) // at logout we will already
                                               // store the player
      SaveToDB(); // prevent loading as ghost without corpse
}

Corpse *Player::GetCorpse() const {
  return GetMap()->GetCorpseByPlayer(GetGUID());
}

void Player::DurabilityLossAll(double percent, bool inventory) {
  for (uint8 i = EQUIPMENT_SLOT_START; i < EQUIPMENT_SLOT_END; i++)
    if (Item *pItem = GetItemByPos(INVENTORY_SLOT_BAG_0, i))
      DurabilityLoss(pItem, percent);

  if (inventory) {
    // bags not have durability
    // for (int i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; i++)

    for (uint8 i = INVENTORY_SLOT_ITEM_START; i < INVENTORY_SLOT_ITEM_END; i++)
      if (Item *pItem = GetItemByPos(INVENTORY_SLOT_BAG_0, i))
        DurabilityLoss(pItem, percent);

    for (uint8 i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; i++)
      if (Bag *pBag = GetBagByPos(i))
        for (uint32 j = 0; j < pBag->GetBagSize(); j++)
          if (Item *pItem = GetItemByPos(i, j))
            DurabilityLoss(pItem, percent);
  }
}

void Player::DurabilityLoss(Item *item, double percent) {
  if (!item)
    return;

  uint32 pMaxDurability = item->GetUInt32Value(ITEM_FIELD_MAXDURABILITY);

  if (!pMaxDurability)
    return;

  percent /= GetTotalAuraMultiplier(SPELL_AURA_MOD_DURABILITY_LOSS);

  uint32 pDurabilityLoss = uint32(pMaxDurability * percent);

  if (pDurabilityLoss < 1)
    pDurabilityLoss = 1;

  DurabilityPointsLoss(item, pDurabilityLoss);
}

void Player::DurabilityPointsLossAll(int32 points, bool inventory) {
  for (uint8 i = EQUIPMENT_SLOT_START; i < EQUIPMENT_SLOT_END; i++)
    if (Item *pItem = GetItemByPos(INVENTORY_SLOT_BAG_0, i))
      DurabilityPointsLoss(pItem, points);

  if (inventory) {
    // bags not have durability
    // for (int i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; i++)

    for (uint8 i = INVENTORY_SLOT_ITEM_START; i < INVENTORY_SLOT_ITEM_END; i++)
      if (Item *pItem = GetItemByPos(INVENTORY_SLOT_BAG_0, i))
        DurabilityPointsLoss(pItem, points);

    for (uint8 i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; i++)
      if (Bag *pBag = (Bag *)GetItemByPos(INVENTORY_SLOT_BAG_0, i))
        for (uint32 j = 0; j < pBag->GetBagSize(); j++)
          if (Item *pItem = GetItemByPos(i, j))
            DurabilityPointsLoss(pItem, points);
  }
}

void Player::DurabilityPointsLoss(Item *item, int32 points) {
  if (HasAuraType(SPELL_AURA_PREVENT_DURABILITY_LOSS))
    return;

  int32 pMaxDurability = item->GetUInt32Value(ITEM_FIELD_MAXDURABILITY);
  int32 pOldDurability = item->GetUInt32Value(ITEM_FIELD_DURABILITY);
  int32 pNewDurability = pOldDurability - points;

  if (pNewDurability < 0)
    pNewDurability = 0;
  else if (pNewDurability > pMaxDurability)
    pNewDurability = pMaxDurability;

  if (pOldDurability != pNewDurability) {
    // modify item stats _before_ Durability set to 0 to pass _ApplyItemMods
    // internal check
    if (pNewDurability == 0 && pOldDurability > 0 && item->IsEquipped())
      _ApplyItemMods(item, item->GetSlot(), false);

    item->SetUInt32Value(ITEM_FIELD_DURABILITY, pNewDurability);

    // modify item stats _after_ restore durability to pass _ApplyItemMods
    // internal check
    if (pNewDurability > 0 && pOldDurability == 0 && item->IsEquipped())
      _ApplyItemMods(item, item->GetSlot(), true);

    item->SetState(ITEM_CHANGED, this);
  }
}

void Player::DurabilityPointLossForEquipSlot(EquipmentSlots slot) {
  if (Item *pItem = GetItemByPos(INVENTORY_SLOT_BAG_0, slot))
    DurabilityPointsLoss(pItem, 1);
}

void Player::DurabilityRepairAll(bool takeCost, float discountMod,
                                 bool guildBank) {
  // Collecting all items that can be repaired and repair costs
  std::list<std::pair<Item *, uint32>> itemRepairCostStore;

  // equipped, backpack, bags itself
  for (uint8 i = EQUIPMENT_SLOT_START; i < INVENTORY_SLOT_ITEM_END; i++)
    if (Item *item = GetItemByPos(((INVENTORY_SLOT_BAG_0 << 8) | i)))
      if (uint32 cost = item->CalculateDurabilityRepairCost(discountMod))
        itemRepairCostStore.push_back(std::make_pair(item, cost));

  // bank, buyback and keys not repaired

  // items in inventory bags
  for (uint8 j = INVENTORY_SLOT_BAG_START; j < INVENTORY_SLOT_BAG_END; j++)
    for (uint8 i = 0; i < MAX_BAG_SIZE; i++)
      if (Item *item = GetItemByPos(((j << 8) | i)))
        if (uint32 cost = item->CalculateDurabilityRepairCost(discountMod))
          itemRepairCostStore.push_back(std::make_pair(item, cost));

  // Handling a free repair case - just repair every item without taking cost.
  if (!takeCost) {
    for (auto const &[item, cost] : itemRepairCostStore)
      DurabilityRepair(item->GetPos(), false, 0.f);

    return;
  }

  // Handling a free repair case - just repair every item without taking cost.
  if (!takeCost) {
    for (auto const &[item, cost] : itemRepairCostStore)
      DurabilityRepair(item->GetPos(), false, 0.f);

    return;
  }

  if (guildBank) {
    // Handling a repair for guild money case.
    // We have to repair items one by one until the guild bank has enough money
    // available for withdrawal or until all items are repaired.

    Guild *guild = GetGuild();
    if (!guild)
      return; // silent return, client shouldn't display this button for players
              // without guild.

    uint64 const availableGuildMoney =
        guild->GetMemberAvailableMoneyForRepairItems(GetGUID());
    if (availableGuildMoney == 0)
      return;

    // Sort the items by repair cost from lowest to highest
    itemRepairCostStore.sort([](auto const &a, auto const &b) -> bool {
      return a.second < b.second;
    });

    // We must calculate total repair cost and take money once to avoid spam in
    // the guild bank log and reduce number of transactions in the database
    uint32 totalCost = 0;

    for (auto const &[item, cost] : itemRepairCostStore) {
      uint64 newTotalCost = totalCost + cost;
      if (newTotalCost > availableGuildMoney || newTotalCost > MAX_MONEY_AMOUNT)
        break;

      totalCost = static_cast<uint32>(newTotalCost);

      // Repair item without taking cost. We'll do it later.
      DurabilityRepair(item->GetPos(), false, 0.f);
    }

    // Take money for repairs from the guild bank
    guild->HandleMemberWithdrawMoney(GetSession(), totalCost, true);
  } else {
    // Handling a repair for player's money case.
    // Unlike repairing for guild money, in this case we must first check if
    // player has enough money to repair all the items at once.

    uint32 totalCost = 0;
    for (auto const &[item, cost] : itemRepairCostStore)
      totalCost += cost;

    if (!HasEnoughMoney(uint64(totalCost)))
      return; // silent return, client should display error by itself and not
              // send opcode.

    ModifyMoney(-int32(totalCost));

    // Payment for repair has already been taken, so just repair every item
    // without taking cost.
    for (auto const &[item, cost] : itemRepairCostStore)
      DurabilityRepair(item->GetPos(), false, 0.f);
  }
}

void Player::DurabilityRepair(uint16 pos, bool takeCost, float discountMod) {
  Item *item = GetItemByPos(pos);
  if (!item)
    return;

  if (takeCost) {
    uint32 cost = item->CalculateDurabilityRepairCost(discountMod);

    if (!HasEnoughMoney(uint64(cost))) {
      LOG_DEBUG("entities.player.items",
                "Player::DurabilityRepair: Player '%s' (%s) has not enough "
                "money to repair item",
                GetName().c_str(), GetGUID().ToString().c_str());
      return;
    }

    ModifyMoney(-int32(cost));
  }

  bool isBroken = item->IsBroken();

  item->SetUInt32Value(ITEM_FIELD_DURABILITY,
                       item->GetUInt32Value(ITEM_FIELD_MAXDURABILITY));
  item->SetState(ITEM_CHANGED, this);

  // reapply mods for total broken and repaired item if equipped
  if (IsEquipmentPos(pos) && isBroken)
    _ApplyItemMods(item, pos & 255, true);
}

void Player::RepopAtGraveyard() {
  // note: this can be called also when the player is alive
  // for example from WorldSession::HandleMovementOpcodes

  AreaTableEntry const *zone = sAreaTableStore.LookupEntry(GetAreaId());

  // Such zones are considered unreachable as a ghost and the player must be
  // automatically revived
  if ((!IsAlive() && zone && zone->Flags & AREA_FLAG_NEED_FLY) ||
      GetTransport() ||
      GetPositionZ() < GetMap()->GetMinHeight(GetPhaseShift(), GetPositionX(),
                                              GetPositionY())) {
    ResurrectPlayer(0.5f);
    SpawnCorpseBones();
  }

  WorldSafeLocsEntry const *ClosestGrave;

  // Special handle for battleground maps
  if (Battleground *bg = GetBattleground())
    ClosestGrave = bg->GetClosestGraveyard(this);
  else {
    if (Battlefield *bf = sBattlefieldMgr->GetBattlefieldToZoneId(GetZoneId()))
      ClosestGrave = bf->GetClosestGraveyard(this);
    else
      ClosestGrave = sObjectMgr->GetClosestGraveyard(*this, GetTeam(), this);
  }

  // stop countdown until repop
  m_deathTimer = 0;

  // if no grave found, stay at the current location
  // and don't show spirit healer location
  if (ClosestGrave) {
    float const *orientation =
        sObjectMgr->GetGraveyardOrientation(ClosestGrave->ID);
    TeleportTo(ClosestGrave->Continent, ClosestGrave->Loc.X,
               ClosestGrave->Loc.Y, ClosestGrave->Loc.Z,
               orientation ? *orientation : GetOrientation());
    if (isDead()) // not send if alive, because it used in TeleportTo()
    {
      WorldPackets::Misc::DeathReleaseLoc packet;
      packet.MapID = ClosestGrave->Continent;
      packet.Loc.Pos = {ClosestGrave->Loc.X, ClosestGrave->Loc.Y,
                        ClosestGrave->Loc.Z};
      SendDirectMessage(packet.Write());
    }
  } else if (GetPositionZ() < GetMap()->GetMinHeight(GetPhaseShift(),
                                                     GetPositionX(),
                                                     GetPositionY()))
    TeleportTo(m_homebindMapId, m_homebindX, m_homebindY, m_homebindZ,
               GetOrientation());

  RemoveFlag(PLAYER_FLAGS, PLAYER_FLAGS_IS_OUT_OF_BOUNDS);
}

bool Player::CanJoinConstantChannelInZone(ChatChannelsEntry const *channel,
                                          AreaTableEntry const *zone) const {
  if (channel->Flags & CHANNEL_DBC_FLAG_ZONE_DEP &&
      zone->Flags & AREA_FLAG_ARENA_INSTANCE)
    return false;

  if ((channel->Flags & CHANNEL_DBC_FLAG_CITY_ONLY) &&
      (!(zone->Flags & AREA_FLAG_SLAVE_CAPITAL)))
    return false;

  if ((channel->Flags & CHANNEL_DBC_FLAG_GUILD_REQ) && GetGuildId())
    return false;

  return true;
}

void Player::JoinedChannel(Channel *c) { m_channels.push_back(c); }

void Player::LeftChannel(Channel *c) { m_channels.remove(c); }

void Player::CleanupChannels() {
  while (!m_channels.empty()) {
    Channel *ch = *m_channels.begin();
    m_channels.erase(m_channels.begin()); // remove from player's channel list
    ch->LeaveChannel(
        this,
        false); // not send to client, not remove from player's channel list

    // delete channel if empty
    if (ChannelMgr *cMgr = ChannelMgr::forTeam(GetTeam())) {
      if (ch->IsConstant())
        cMgr->LeftChannel(ch->GetChannelId(), ch->GetZoneEntry());
      else
        cMgr->LeftChannel(ch->GetName());
    }
  }
  LOG_DEBUG("chat.system",
            "Player::CleanupChannels: Channels of player '%s' (%s) cleaned up.",
            GetName().c_str(), GetGUID().ToString().c_str());
}

void Player::UpdateLocalChannels(uint32 newZone) {
  if (GetSession()->PlayerLoading() && !IsBeingTeleportedFar())
    return; // The client handles it automatically after loading, but not after
            // teleporting

  AreaTableEntry const *current_zone = sAreaTableStore.LookupEntry(newZone);
  if (!current_zone)
    return;

  ChannelMgr *cMgr = ChannelMgr::forTeam(GetTeam());
  if (!cMgr)
    return;

  for (uint32 i = 0; i < sChatChannelsStore.GetNumRows(); ++i) {
    ChatChannelsEntry const *channelEntry = sChatChannelsStore.LookupEntry(i);
    if (!channelEntry)
      continue;

    if (!(channelEntry->Flags & CHANNEL_DBC_FLAG_INITIAL))
      continue;

    Channel *usedChannel = nullptr;
    for (Channel *channel : m_channels) {
      if (channel->GetChannelId() == i) {
        usedChannel = channel;
        break;
      }
    }

    Channel *removeChannel = nullptr;
    Channel *joinChannel = nullptr;
    bool sendRemove = true;

    if (CanJoinConstantChannelInZone(channelEntry, current_zone)) {
      if (!(channelEntry->Flags & CHANNEL_DBC_FLAG_GLOBAL)) {
        if (channelEntry->Flags & CHANNEL_DBC_FLAG_CITY_ONLY && usedChannel)
          continue; // Already on the channel, as city channel names are not
                    // changing

        joinChannel =
            cMgr->GetJoinChannel(channelEntry->ID, std::string(), current_zone);
        if (usedChannel) {
          if (joinChannel != usedChannel) {
            removeChannel = usedChannel;
            sendRemove = false; // Do not send leave channel, it already
                                // replaced at client
          } else
            joinChannel = nullptr;
        }
      } else
        joinChannel = cMgr->GetJoinChannel(channelEntry->ID, std::string());
    } else
      removeChannel = usedChannel;

    if (joinChannel)
      joinChannel->JoinChannel(
          this, ""); // Changed Channel: ... or Joined Channel: ...

    if (removeChannel) {
      removeChannel->LeaveChannel(this, sendRemove); // Leave old channel

      LeftChannel(removeChannel); // Remove from player's channel list
      cMgr->LeftChannel(removeChannel->GetChannelId(),
                        removeChannel->GetZoneEntry()); // Delete if empty
    }
  }
}

void Player::LeaveLFGChannel() {
  for (JoinedChannelsList::iterator i = m_channels.begin();
       i != m_channels.end(); ++i) {
    if ((*i)->IsLFG()) {
      (*i)->LeaveChannel(this);
      break;
    }
  }
}

void Player::HandleBaseModFlatValue(BaseModGroup modGroup, float amount,
                                    bool apply) {
  if (modGroup >= BASEMOD_END) {
    LOG_ERROR("spells",
              "Player::HandleBaseModValue: Invalid BaseModGroup/BaseModType "
              "(%u/%u) for player '%s' (%s)",
              modGroup, FLAT_MOD, GetName().c_str(),
              GetGUID().ToString().c_str());
    return;
  }

  m_auraBaseFlatMod[modGroup] += apply ? amount : -amount;
  UpdateBaseModGroup(modGroup);
}

void Player::ApplyBaseModPctValue(BaseModGroup modGroup, float pct) {
  if (modGroup >= BASEMOD_END) {
    LOG_ERROR("spells",
              "Player::HandleBaseModValue: Invalid BaseModGroup/BaseModType "
              "(%u/%u) for player '%s' (%s)",
              modGroup, FLAT_MOD, GetName().c_str(),
              GetGUID().ToString().c_str());
    return;
  }

  AddPct(m_auraBasePctMod[modGroup], pct);
  UpdateBaseModGroup(modGroup);
}

void Player::SetBaseModFlatValue(BaseModGroup modGroup, float val) {
  if (m_auraBaseFlatMod[modGroup] == val)
    return;

  m_auraBaseFlatMod[modGroup] = val;
  UpdateBaseModGroup(modGroup);
}

void Player::SetBaseModPctValue(BaseModGroup modGroup, float val) {
  if (m_auraBasePctMod[modGroup] == val)
    return;

  m_auraBasePctMod[modGroup] = val;
  UpdateBaseModGroup(modGroup);
}

void Player::UpdateDamageDoneMods(WeaponAttackType attackType) {
  Unit::UpdateDamageDoneMods(attackType);

  UnitMods unitMod;
  switch (attackType) {
  case BASE_ATTACK:
    unitMod = UNIT_MOD_DAMAGE_MAINHAND;
    break;
  case OFF_ATTACK:
    unitMod = UNIT_MOD_DAMAGE_OFFHAND;
    break;
  case RANGED_ATTACK:
    unitMod = UNIT_MOD_DAMAGE_RANGED;
    break;
  default:
    ABORT();
    break;
  }

  float amount = 0.0f;
  Item *item = GetWeaponForAttack(attackType, true);
  if (!item)
    return;

  for (uint8 slot = 0; slot < MAX_ENCHANTMENT_SLOT; ++slot) {
    SpellItemEnchantmentEntry const *enchantmentEntry =
        sSpellItemEnchantmentStore.LookupEntry(
            item->GetEnchantmentId(EnchantmentSlot(slot)));
    if (!enchantmentEntry)
      continue;

    for (uint8 i = 0; i < MAX_ITEM_ENCHANTMENT_EFFECTS; ++i) {
      switch (enchantmentEntry->Effect[i]) {
      case ITEM_ENCHANTMENT_TYPE_DAMAGE:
        amount += enchantmentEntry->EffectPointsMin[i];
        break;
      case ITEM_ENCHANTMENT_TYPE_TOTEM:
        if (getClass() == CLASS_SHAMAN)
          amount += enchantmentEntry->EffectPointsMin[i] *
                    item->GetTemplate()->GetDelay() / 1000.0f;
        break;
      default:
        break;
      }
    }
  }

  HandleStatFlatModifier(unitMod, TOTAL_VALUE, amount, true);
}

void Player::UpdateBaseModGroup(BaseModGroup modGroup) {
  if (!CanModifyStats())
    return;

  switch (modGroup) {
  case CRIT_PERCENTAGE:
    UpdateCritPercentage(BASE_ATTACK);
    break;
  case RANGED_CRIT_PERCENTAGE:
    UpdateCritPercentage(RANGED_ATTACK);
    break;
  case OFFHAND_CRIT_PERCENTAGE:
    UpdateCritPercentage(OFF_ATTACK);
    break;
  default:
    break;
  }
}

float Player::GetBaseModValue(BaseModGroup modGroup,
                              BaseModType modType) const {
  if (modGroup >= BASEMOD_END || modType >= MOD_END) {
    LOG_ERROR("spells",
              "Player::GetBaseModValue: Invalid BaseModGroup/BaseModType "
              "(%u/%u) for player '%s' (%s)",
              modGroup, modType, GetName().c_str(),
              GetGUID().ToString().c_str());
    return 0.0f;
  }

  return (modType == FLAT_MOD ? m_auraBaseFlatMod[modGroup]
                              : m_auraBasePctMod[modGroup]);
}

float Player::GetTotalBaseModValue(BaseModGroup modGroup) const {
  if (modGroup >= BASEMOD_END) {
    LOG_ERROR("spells",
              "Player::GetTotalBaseModValue: Invalid BaseModGroup (%u) for "
              "player '%s' (%s)",
              modGroup, GetName().c_str(), GetGUID().ToString().c_str());
    return 0.0f;
  }

  return m_auraBaseFlatMod[modGroup] * m_auraBasePctMod[modGroup];
}

float Player::GetMeleeCritFromAgility() const {
  uint8 level = getLevel();
  uint32 pclass = getClass();

  if (level > GT_MAX_LEVEL)
    level = GT_MAX_LEVEL;

  GtChanceToMeleeCritBaseEntry const *critBase =
      sGtChanceToMeleeCritBaseStore.LookupEntry(pclass - 1);
  GtChanceToMeleeCritEntry const *critRatio =
      sGtChanceToMeleeCritStore.LookupEntry((pclass - 1) * GT_MAX_LEVEL +
                                            level - 1);
  if (critBase == nullptr || critRatio == nullptr)
    return 0.0f;

  float crit = critBase->base + GetStat(STAT_AGILITY) * critRatio->ratio;
  return crit * 100.0f;
}

void Player::GetDodgeFromAgility(float &diminishing,
                                 float &nondiminishing) const {
  /*
    "Patch 4.2.0 (2011-06-28): Death Knights, Paladins, and Warriors no
    longer receive any bonus to their chance to dodge from Agility. Their base
    chance to dodge is now a fixed 5%."
  */
  if (getClass() == CLASS_WARRIOR || getClass() == CLASS_PALADIN ||
      getClass() == CLASS_DEATH_KNIGHT) {
    nondiminishing += 5.f;
    return;
  }

  // Table for base dodge values
  std::array<float, MAX_CLASSES> const baseDodge = {
      0.037580f,  // Warrior  (obsolete since 4.2)
      0.036520f,  // Paladin  (obsolete since 4.2)
      -0.054500f, // Hunter
      -0.005900f, // Rogue
      0.031830f,  // Priest
      0.036640f,  // Death Knight  (obsolete since 4.2)
      0.016750f,  // Shaman
      0.034575f,  // Mage
      0.020350f,  // Warlock
      0.0f,       // ??
      0.049510f   // Druid
  };

  // Crit to dodge coefficient (Todo: these coefficients are based on previous
  // author's code so they still need to be validated)
  std::array<float, MAX_CLASSES> const critToDodge = {
      0.f,   // Warrior (obsolete since 4.2)
      0.f,   // Paladin (obsolete since 4.2)
      0.74f, // Hunter
      1.33f, // Rogue
      0.67f, // Priest
      0.f,   // Death Knight (absolete since 4.2)
      1.07f, // Shaman
      0.66f, // Mage
      0.65f, // Warlock
      0.f,   // ??
      1.32f  // Druid
  };

  uint8 level = getLevel();
  uint32 playerClass = getClass();

  if (level > GT_MAX_LEVEL)
    level = GT_MAX_LEVEL;

  // Dodge per agility is proportional to crit per agility, which is available
  // from DBC files
  GtChanceToMeleeCritEntry const *dodgeRatio =
      sGtChanceToMeleeCritStore.LookupEntry((playerClass - 1) * GT_MAX_LEVEL +
                                            level - 1);
  if (!dodgeRatio || playerClass > MAX_CLASSES)
    return;

  float baseAgility =
      GetCreateStat(STAT_AGILITY) *
      GetPctModifierValue(UnitMods(UNIT_MOD_STAT_START + STAT_AGILITY),
                          BASE_PCT);
  float bonusAgility = GetStat(STAT_AGILITY) - baseAgility;

  // calculate diminishing (green in char screen) and non-diminishing (white)
  // contribution
  diminishing =
      100.0f * bonusAgility * dodgeRatio->ratio * critToDodge[playerClass - 1];
  nondiminishing =
      100.0f * (baseDodge[playerClass - 1] +
                baseAgility * dodgeRatio->ratio * critToDodge[playerClass - 1]);
}

float Player::GetSpellCritFromIntellect() const {
  uint8 level = getLevel();
  uint32 pclass = getClass();

  if (level > GT_MAX_LEVEL)
    level = GT_MAX_LEVEL;

  GtChanceToSpellCritBaseEntry const *critBase =
      sGtChanceToSpellCritBaseStore.LookupEntry(pclass - 1);
  GtChanceToSpellCritEntry const *critRatio =
      sGtChanceToSpellCritStore.LookupEntry((pclass - 1) * GT_MAX_LEVEL +
                                            level - 1);
  if (critBase == nullptr || critRatio == nullptr)
    return 0.0f;

  float crit = critBase->base + GetStat(STAT_INTELLECT) * critRatio->ratio;
  return crit * 100.0f;
}

float Player::GetRatingMultiplier(CombatRating cr) const {
  uint8 level = getLevel();

  if (level > GT_MAX_LEVEL)
    level = GT_MAX_LEVEL;

  GtCombatRatingsEntry const *Rating =
      sGtCombatRatingsStore.LookupEntry(cr * GT_MAX_LEVEL + level - 1);
  // gtOCTClassCombatRatingScalarStore.dbc starts with 1, CombatRating with
  // zero, so cr+1
  GtOCTClassCombatRatingScalarEntry const *classRating =
      sGtOCTClassCombatRatingScalarStore.LookupEntry(
          (getClass() - 1) * GT_MAX_RATING + cr + 1);
  if (!Rating || !classRating)
    return 1.0f; // By default use minimum coefficient (not must be called)

  return classRating->ratio / Rating->ratio;
}

float Player::GetRatingBonusValue(CombatRating cr) const {
  float baseResult = float(GetUInt32Value(PLAYER_FIELD_COMBAT_RATING_1 + cr)) *
                     GetRatingMultiplier(cr);
  if (cr != CR_RESILIENCE_PLAYER_DAMAGE_TAKEN)
    return baseResult;
  return float(1.0f - pow(0.99f, baseResult)) * 100.0f;
}

float Player::GetExpertiseDodgeOrParryReduction(
    WeaponAttackType attType) const {
  switch (attType) {
  case BASE_ATTACK:
    return GetUInt32Value(PLAYER_EXPERTISE) / 4.0f;
  case OFF_ATTACK:
    return GetUInt32Value(PLAYER_OFFHAND_EXPERTISE) / 4.0f;
  default:
    break;
  }
  return 0.0f;
}

float Player::OCTRegenMPPerSpirit() const {
  uint8 level = getLevel();
  uint32 pclass = getClass();

  if (level > GT_MAX_LEVEL)
    level = GT_MAX_LEVEL;

  //    GtOCTRegenMPEntry     const* baseRatio =
  //    sGtOCTRegenMPStore.LookupEntry((pclass-1)*GT_MAX_LEVEL + level-1);
  GtRegenMPPerSptEntry const *moreRatio = sGtRegenMPPerSptStore.LookupEntry(
      (pclass - 1) * GT_MAX_LEVEL + level - 1);
  if (moreRatio == nullptr)
    return 0.0f;

  // Formula get from PaperDollFrame script
  float spirit = GetStat(STAT_SPIRIT);
  float regen = spirit * moreRatio->ratio;
  return regen;
}

void Player::ApplyRatingMod(CombatRating combatRating, int32 value,
                            bool apply) {
  // explicit affected values
  switch (combatRating) {
  case CR_HASTE_MELEE: {
    ApplyAttackTimePercentMod(BASE_ATTACK,
                              m_baseRatingValue[combatRating] *
                                  GetRatingMultiplier(combatRating),
                              false);
    ApplyAttackTimePercentMod(
        BASE_ATTACK,
        (m_baseRatingValue[combatRating] + (apply ? value : -value)) *
            GetRatingMultiplier(combatRating),
        true);
    ApplyAttackTimePercentMod(OFF_ATTACK,
                              m_baseRatingValue[combatRating] *
                                  GetRatingMultiplier(combatRating),
                              false);
    ApplyAttackTimePercentMod(
        OFF_ATTACK,
        (m_baseRatingValue[combatRating] + (apply ? value : -value)) *
            GetRatingMultiplier(combatRating),
        true);
    ApplyHasteRegenMod(BASE_ATTACK,
                       m_baseRatingValue[combatRating] *
                           GetRatingMultiplier(combatRating),
                       false);
    ApplyHasteRegenMod(
        BASE_ATTACK,
        (m_baseRatingValue[combatRating] + (apply ? value : -value)) *
            GetRatingMultiplier(combatRating),
        true);
    break;
  }
  case CR_HASTE_RANGED: {
    ApplyAttackTimePercentMod(RANGED_ATTACK,
                              m_baseRatingValue[combatRating] *
                                  GetRatingMultiplier(combatRating),
                              false);
    ApplyAttackTimePercentMod(
        RANGED_ATTACK,
        (m_baseRatingValue[combatRating] + (apply ? value : -value)) *
            GetRatingMultiplier(combatRating),
        true);
    ApplyHasteRegenMod(RANGED_ATTACK,
                       m_baseRatingValue[combatRating] *
                           GetRatingMultiplier(combatRating),
                       false);
    ApplyHasteRegenMod(
        RANGED_ATTACK,
        (m_baseRatingValue[combatRating] + (apply ? value : -value)) *
            GetRatingMultiplier(combatRating),
        true);
    break;
  }
  case CR_HASTE_SPELL: {
    ApplyCastTimePercentMod(m_baseRatingValue[combatRating] *
                                GetRatingMultiplier(combatRating),
                            false);
    ApplyCastTimePercentMod(
        (m_baseRatingValue[combatRating] + (apply ? value : -value)) *
            GetRatingMultiplier(combatRating),
        true);
    break;
  }
  default:
    break;
  }

  m_baseRatingValue[combatRating] += (apply ? value : -value);

  UpdateRating(combatRating);
}

void Player::UpdateRating(CombatRating cr) {
  int32 amount = m_baseRatingValue[cr];
  // Apply bonus from SPELL_AURA_MOD_RATING_FROM_STAT
  // stat used stored in miscValueB for this aura
  AuraEffectList const &modRatingFromStat =
      GetAuraEffectsByType(SPELL_AURA_MOD_RATING_FROM_STAT);
  for (AuraEffectList::const_iterator i = modRatingFromStat.begin();
       i != modRatingFromStat.end(); ++i)
    if ((*i)->GetMiscValue() & (1 << cr))
      amount += int32(CalculatePct(GetStat(Stats((*i)->GetMiscValueB())),
                                   (*i)->GetAmount()));
  if (amount < 0)
    amount = 0;
  SetUInt32Value(PLAYER_FIELD_COMBAT_RATING_1 + cr, uint32(amount));

  bool affectStats = CanModifyStats();

  switch (cr) {
  case CR_WEAPON_SKILL:
  case CR_DEFENSE_SKILL:
    break;
  case CR_DODGE:
    UpdateDodgePercentage();
    break;
  case CR_PARRY:
    UpdateParryPercentage();
    break;
  case CR_BLOCK:
    UpdateBlockPercentage();
    break;
  case CR_HIT_MELEE:
    UpdateMeleeHitChances();
    break;
  case CR_HIT_RANGED:
    UpdateRangedHitChances();
    break;
  case CR_HIT_SPELL:
    UpdateSpellHitChances();
    break;
  case CR_CRIT_MELEE:
    if (affectStats) {
      UpdateCritPercentage(BASE_ATTACK);
      UpdateCritPercentage(OFF_ATTACK);
    }
    break;
  case CR_CRIT_RANGED:
    if (affectStats)
      UpdateCritPercentage(RANGED_ATTACK);
    break;
  case CR_CRIT_SPELL:
    if (affectStats)
      UpdateAllSpellCritChances();
    break;
  case CR_HIT_TAKEN_MELEE:  // Deprecated since Cataclysm
  case CR_HIT_TAKEN_RANGED: // Deprecated since Cataclysm
  case CR_HIT_TAKEN_SPELL:  // Deprecated since Cataclysm
  case CR_RESILIENCE_PLAYER_DAMAGE_TAKEN:
  case CR_RESILIENCE_CRIT_TAKEN:
  case CR_CRIT_TAKEN_SPELL:  // Deprecated since Cataclysm
  case CR_ARMOR_PENETRATION: // Deprecated since Cataclysm
    break;
  case CR_HASTE_MELEE: // Implemented in Player::ApplyRatingMod
  case CR_HASTE_RANGED:
  case CR_HASTE_SPELL:
    break;
  case CR_WEAPON_SKILL_MAINHAND: // Implemented in Unit::RollMeleeOutcomeAgainst
  case CR_WEAPON_SKILL_OFFHAND:
  case CR_WEAPON_SKILL_RANGED:
    break;
  case CR_EXPERTISE:
    if (affectStats) {
      UpdateExpertise(BASE_ATTACK);
      UpdateExpertise(OFF_ATTACK);
    }
    break;
  case CR_MASTERY:
    UpdateMastery();
    break;
  }
}

void Player::UpdateAllRatings() {
  for (uint8 cr = 0; cr < MAX_COMBAT_RATING; ++cr)
    UpdateRating(CombatRating(cr));
}

void Player::SetRegularAttackTime() {
  for (uint8 i = 0; i < MAX_ATTACK; ++i) {
    Item *tmpitem = GetWeaponForAttack(WeaponAttackType(i), true);
    if (tmpitem && !tmpitem->IsBroken()) {
      ItemTemplate const *proto = tmpitem->GetTemplate();
      if (proto->GetDelay())
        SetAttackTime(WeaponAttackType(i), proto->GetDelay());
    } else
      SetAttackTime(
          WeaponAttackType(i),
          BASE_ATTACK_TIME); // If there is no weapon reset attack time to base
                             // (might have been changed from forms)
  }
}

void Player::StoreRaidMapDifficulty() {
  m_raidMapDifficulty = GetMap()->GetDifficulty();
}

// skill+step, checking for max value
bool Player::UpdateSkill(uint32 skill_id, uint32 step) {
  if (!skill_id)
    return false;

  SkillStatusMap::iterator itr = mSkillStatus.find(skill_id);
  if (itr == mSkillStatus.end() || itr->second.uState == SKILL_DELETED)
    return false;

  uint16 field = itr->second.pos / 2;
  uint8 offset = itr->second.pos & 1; // itr->second.pos % 2

  uint16 value = GetUInt16Value(PLAYER_SKILL_RANK_0 + field, offset);
  uint16 max = GetUInt16Value(PLAYER_SKILL_MAX_RANK_0 + field, offset);

  if (!max || !value || value >= max)
    return false;

  uint32 new_value = value + step;
  if (new_value > max)
    new_value = max;

  SetSkillRank(itr->second.pos, new_value);
  if (itr->second.uState != SKILL_NEW)
    itr->second.uState = SKILL_CHANGED;

  UpdateSkillEnchantments(skill_id, value, new_value);
  UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_REACH_SKILL_LEVEL,
                            skill_id);

  if (skill_id == SKILL_ARCHAEOLOGY)
    _archaeology->Update();

  return true;
}

inline int32 SkillGainChance(Player const *player, uint32 SkillValue,
                             uint32 GrayLevel, uint32 GreenLevel,
                             uint32 YellowLevel) {
  int32 chance = 0;
  if (SkillValue >= GrayLevel)
    chance = sWorld->getIntConfig(CONFIG_SKILL_CHANCE_GREY) * 10;
  else if (SkillValue >= GreenLevel)
    chance = sWorld->getIntConfig(CONFIG_SKILL_CHANCE_GREEN) * 10;
  else if (SkillValue >= YellowLevel)
    chance = sWorld->getIntConfig(CONFIG_SKILL_CHANCE_YELLOW) * 10;
  else
    chance = sWorld->getIntConfig(CONFIG_SKILL_CHANCE_ORANGE) * 10;

  // SPELL_AURA_INCREASE_SKILL_GAIN_CHANCE bonus
  chance *=
      player->GetTotalAuraMultiplier(SPELL_AURA_INCREASE_SKILL_GAIN_CHANCE);

  return chance;
}

bool Player::UpdateCraftSkill(uint32 spellid) {
  LOG_DEBUG("entities.player.skills",
            "Player::UpdateCraftSkill: Player '%s' (%s), SpellID: %d",
            GetName().c_str(), GetGUID().ToString().c_str(), spellid);

  SkillLineAbilityMapBounds bounds =
      sSpellMgr->GetSkillLineAbilityMapBounds(spellid);

  for (SkillLineAbilityMap::const_iterator _spell_idx = bounds.first;
       _spell_idx != bounds.second; ++_spell_idx) {
    if (_spell_idx->second->SkillLine) {
      uint32 SkillValue = GetPureSkillValue(_spell_idx->second->SkillLine);

      // Alchemy Discoveries here
      SpellInfo const *spellEntry = sSpellMgr->GetSpellInfo(spellid);
      if (spellEntry && spellEntry->Mechanic == MECHANIC_DISCOVERY) {
        if (uint32 discoveredSpell = GetSkillDiscoverySpell(
                _spell_idx->second->SkillLine, spellid, this))
          LearnSpell(discoveredSpell, false);
      }

      uint32 craft_skill_gain =
          sWorld->getIntConfig(CONFIG_SKILL_GAIN_CRAFTING);
      uint32 craft_skill_points = _spell_idx->second->NumSkillUps;

      if (craft_skill_points) {
        craft_skill_points *= craft_skill_gain;
        LOG_INFO("entities.player",
                 "craft_skill_points is %u, spellID: %u, skillID: %u",
                 craft_skill_points, spellid, _spell_idx->second->SkillLine);
        return UpdateSkillPro(
            _spell_idx->second->SkillLine,
            SkillGainChance(this, SkillValue,
                            _spell_idx->second->TrivialSkillLineRankHigh,
                            (_spell_idx->second->TrivialSkillLineRankHigh +
                             _spell_idx->second->TrivialSkillLineRankLow) /
                                2,
                            _spell_idx->second->TrivialSkillLineRankLow),
            craft_skill_points);
      }

      return UpdateSkillPro(
          _spell_idx->second->SkillLine,
          SkillGainChance(this, SkillValue,
                          _spell_idx->second->TrivialSkillLineRankHigh,
                          (_spell_idx->second->TrivialSkillLineRankHigh +
                           _spell_idx->second->TrivialSkillLineRankLow) /
                              2,
                          _spell_idx->second->TrivialSkillLineRankLow),
          craft_skill_gain);
    }
  }
  return false;
}

bool Player::UpdateGatherSkill(uint32 SkillId, uint32 SkillValue,
                               uint32 RedLevel, uint32 Multiplicator) {
  LOG_DEBUG("entities.player.skills",
            "UpdateGatherSkill(SkillId %d SkillLevel %d RedLevel %d)", SkillId,
            SkillValue, RedLevel);

  uint32 gathering_skill_gain =
      sWorld->getIntConfig(CONFIG_SKILL_GAIN_GATHERING);

  // For skinning and Mining chance decrease with level. 1-74 - no decrease,
  // 75-149 - 2 times, 225-299 - 8 times
  switch (SkillId) {
  case SKILL_ARCHAEOLOGY:
    return UpdateSkillPro(SkillId,
                          ((SkillValue < 50) ? 1000 : 0) * Multiplicator,
                          gathering_skill_gain);
  case SKILL_HERBALISM:
  case SKILL_LOCKPICKING:
  case SKILL_JEWELCRAFTING:
  case SKILL_INSCRIPTION:
    return UpdateSkillPro(SkillId,
                          SkillGainChance(this, SkillValue, RedLevel + 100,
                                          RedLevel + 50, RedLevel + 25) *
                              Multiplicator,
                          gathering_skill_gain);
  case SKILL_SKINNING:
    if (sWorld->getIntConfig(CONFIG_SKILL_CHANCE_SKINNING_STEPS) == 0)
      return UpdateSkillPro(SkillId,
                            SkillGainChance(this, SkillValue, RedLevel + 100,
                                            RedLevel + 50, RedLevel + 25) *
                                Multiplicator,
                            gathering_skill_gain);
    else
      return UpdateSkillPro(
          SkillId,
          (SkillGainChance(this, SkillValue, RedLevel + 100, RedLevel + 50,
                           RedLevel + 25) *
           Multiplicator) >>
              (SkillValue /
               sWorld->getIntConfig(CONFIG_SKILL_CHANCE_SKINNING_STEPS)),
          gathering_skill_gain);
  case SKILL_MINING:
    if (sWorld->getIntConfig(CONFIG_SKILL_CHANCE_MINING_STEPS) == 0)
      return UpdateSkillPro(SkillId,
                            SkillGainChance(this, SkillValue, RedLevel + 100,
                                            RedLevel + 50, RedLevel + 25) *
                                Multiplicator,
                            gathering_skill_gain);
    else
      return UpdateSkillPro(
          SkillId,
          (SkillGainChance(this, SkillValue, RedLevel + 100, RedLevel + 50,
                           RedLevel + 25) *
           Multiplicator) >>
              (SkillValue /
               sWorld->getIntConfig(CONFIG_SKILL_CHANCE_MINING_STEPS)),
          gathering_skill_gain);
  }
  return false;
}

uint8 GetFishingStepsNeededToLevelUp(uint32 SkillValue) {
  // These formulas are guessed to be as close as possible to how the skill
  // difficulty curve for fishing was on Retail.
  if (SkillValue < 75)
    return 1;

  if (SkillValue <= 300)
    return SkillValue / 44;

  return SkillValue / 31;
}

bool Player::UpdateFishingSkill() {
  LOG_DEBUG("entities.player.skills",
            "Player::UpdateFishingSkill: Player '%s' (%s)", GetName().c_str(),
            GetGUID().ToString().c_str());

  uint32 SkillValue = GetPureSkillValue(SKILL_FISHING);

  if (SkillValue >= GetMaxSkillValue(SKILL_FISHING))
    return false;

  uint8 stepsNeededToLevelUp = GetFishingStepsNeededToLevelUp(SkillValue);
  ++m_fishingSteps;

  if (m_fishingSteps >= stepsNeededToLevelUp) {
    m_fishingSteps = 0;

    uint32 gathering_skill_gain =
        sWorld->getIntConfig(CONFIG_SKILL_GAIN_GATHERING);
    return UpdateSkillPro(SKILL_FISHING, 100 * 10, gathering_skill_gain);
  }

  return false;
}

void Player::GiveXpForGather(uint32 const &skillId) {
  // Skip if the profession is no gather profession
  if (skillId != SKILL_HERBALISM && skillId != SKILL_MINING &&
      skillId != SKILL_ARCHAEOLOGY)
    return;

  uint32 zoneId = GetZoneId();
  WorldMapAreaEntry const *areaEntry = sWorldMapAreaStore.LookupEntry(zoneId);
  if (!areaEntry)
    return;

  uint8 levelDiff = std::abs(int32(areaEntry->LevelRangeMax - getLevel()));
  uint8 level = 0;

  if (levelDiff >= 10 && levelDiff < 20)
    level = Firelands::XP::GetGrayLevel(getLevel()) + 1;
  else if (levelDiff >= 20)
    level = Firelands::XP::GetGrayLevel(getLevel());
  else
    level = getLevel() + 3;

  uint32 xp = Firelands::XP::BaseGain(level, areaEntry->LevelRangeMax,
                                      sDBCManager.GetContentLevelsForMapAndZone(
                                          GetMapId(), zoneId)) *
              2;

  if (!xp || levelDiff >= 20)
    return;
  else if (levelDiff >= 15)
    xp = uint32(xp * (1 - (levelDiff / 24)));

  xp *= sWorld->getRate(RATE_XP_KILL);

  GiveXP(xp, nullptr);
}

void Player::SurveyDigSite() { _archaeology->UseSite(); }

void Player::NotifyRequestResearchHistory() {
  _archaeology->SendResearchHistory();
}

bool Player::ArchProjectCompleteable(uint16 projectId) {
  return _archaeology->ProjectCompleteable(projectId);
}

bool Player::HasArchProject(uint16 projectId) {
  return _archaeology->ProjectExists(projectId);
}

void Player::CompleteArchProject(uint16 projectId) {
  _archaeology->CompleteProject(projectId);
}

void Player::SetArchData(ArchData const &data) {
  _archaeology->SetArchData(data);
}

bool Player::UpdateSkillPro(uint16 skillId, int32 chance, uint32 step) {
  // levels sync. with spell requirement for skill levels to learn
  // bonus abilities in sSkillLineAbilityStore
  // Used only to avoid scan DBC at each skill grow
  static uint32 bonusSkillLevels[] = {75, 150, 225, 300, 375, 450, 525};
  static const size_t bonusSkillLevelsSize =
      sizeof(bonusSkillLevels) / sizeof(uint32);

  LOG_DEBUG("entities.player.skills",
            "UpdateSkillPro(SkillId %d, Chance %3.1f%%)", skillId,
            chance / 10.0f);
  if (!skillId)
    return false;

  if (chance <= 0) // speedup in 0 chance case
  {
    LOG_DEBUG("entities.player.skills",
              "Player::UpdateSkillPro Chance=%3.1f%% missed", chance / 10.0f);
    return false;
  }

  SkillStatusMap::iterator itr = mSkillStatus.find(skillId);
  if (itr == mSkillStatus.end() || itr->second.uState == SKILL_DELETED)
    return false;

  uint16 field = itr->second.pos / 2;
  uint8 offset = itr->second.pos & 1; // itr->second.pos % 2

  uint16 value = GetUInt16Value(PLAYER_SKILL_RANK_0 + field, offset);
  uint16 max = GetUInt16Value(PLAYER_SKILL_MAX_RANK_0 + field, offset);

  if (!max || !value || value >= max)
    return false;

  if (irand(1, 1000) > chance) {
    LOG_DEBUG("entities.player.skills",
              "Player::UpdateSkillPro Chance=%3.1f%% missed", chance / 10.0f);
    return false;
  }

  uint16 new_value = value + step;
  if (new_value > max)
    new_value = max;

  SetUInt16Value(PLAYER_SKILL_RANK_0 + field, offset, new_value);
  if (itr->second.uState != SKILL_NEW)
    itr->second.uState = SKILL_CHANGED;

  for (size_t i = 0; i < bonusSkillLevelsSize; ++i) {
    uint32 bsl = bonusSkillLevels[i];
    if (value < bsl && new_value >= bsl) {
      LearnSkillRewardedSpells(skillId, new_value);
      break;
    }
  }

  UpdateSkillEnchantments(skillId, value, new_value);
  UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_REACH_SKILL_LEVEL,
                            skillId);
  if (Guild *guild = GetGuild())
    guild->UpdateMemberData(this, GUILD_MEMBER_DATA_PROFESSIONS, 0);

  LOG_DEBUG("entities.player.skills",
            "Player::UpdateSkillPro Chance=%3.1f%% taken", chance / 10.0f);

  return true;
}

void Player::ModifySkillBonus(uint32 skillid, int32 val, bool talent) {
  SkillStatusMap::const_iterator itr = mSkillStatus.find(skillid);
  if (itr == mSkillStatus.end() || itr->second.uState == SKILL_DELETED)
    return;

  uint16 field = itr->second.pos / 2 +
                 (talent ? PLAYER_SKILL_TALENT_0 : PLAYER_SKILL_MODIFIER_0);
  uint8 offset = itr->second.pos & 1; // itr->second.pos % 2

  uint16 bonus = GetUInt16Value(field, offset);

  SetUInt16Value(field, offset, bonus + val);
}

void Player::UpdateSkillsForLevel() {
  uint32 maxSkill = GetMaxSkillValueForLevel();

  for (SkillStatusMap::iterator itr = mSkillStatus.begin();
       itr != mSkillStatus.end(); ++itr) {
    if (itr->second.uState == SKILL_DELETED || !GetSkillRank(itr->second.pos))
      continue;

    uint32 pskill = itr->first;
    SkillRaceClassInfoEntry const *rcEntry =
        sDBCManager.GetSkillRaceClassInfo(pskill, getRace(), getClass());
    if (!rcEntry)
      continue;

    if (GetSkillRangeType(rcEntry) != SKILL_RANGE_LEVEL)
      continue;

    if (GetSkillRangeType(rcEntry) == SKILL_RANGE_LEVEL) {
      if (rcEntry->Flags & SKILL_FLAG_ALWAYS_MAX_VALUE)
        SetSkillRank(itr->second.pos, maxSkill);

      SetSkillMaxRank(itr->second.pos, maxSkill);
      if (itr->second.uState != SKILL_NEW)
        itr->second.uState = SKILL_CHANGED;
    }

    // Update level dependent skillline spells
    LearnSkillRewardedSpells(rcEntry->SkillID, GetSkillRank(itr->second.pos));
  }
}

void Player::InitializeSkillFields() {
  uint32 i = 0;
  for (SkillLineEntry const *skillLine : sSkillLineStore) {
    if (sDBCManager.GetSkillRaceClassInfo(skillLine->ID, getRace(),
                                          getClass())) {
      SetSkillLineId(i, skillLine->ID);
      mSkillStatus.insert(SkillStatusMap::value_type(
          skillLine->ID, SkillStatusData(i, SKILL_UNCHANGED)));
      if (++i >= PLAYER_MAX_SKILLS)
        break;
    }
  }
}

// This functions sets a skill line value (and adds if doesn't exist yet)
// To "remove" a skill line, set it's values to zero
void Player::SetSkill(uint16 id, uint16 step, uint16 newVal, uint16 maxVal) {
  if (!id)
    return;

  uint16 currVal;
  SkillStatusMap::iterator itr = mSkillStatus.find(id);

  // Handle already stored skills
  if (itr != mSkillStatus.end()) {
    currVal = GetSkillRank(itr->second.pos);

    // Activate and update skill line
    if (newVal) {
      // if skill value is going down, update enchantments before setting the
      // new value
      if (newVal < currVal)
        UpdateSkillEnchantments(id, currVal, newVal);

      // update step
      SetSkillStep(itr->second.pos, step);
      // update value
      SetSkillRank(itr->second.pos, newVal);
      SetSkillMaxRank(itr->second.pos, maxVal);

      LearnSkillRewardedSpells(id, newVal);
      // if skill value is going up, update enchantments after setting the new
      // value
      if (newVal > currVal) {
        UpdateSkillEnchantments(id, currVal, newVal);
        if (id == SKILL_RIDING)
          UpdateMountCapability();
      }

      UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_REACH_SKILL_LEVEL,
                                id);
      UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LEVEL,
                                id);

      // update skill state
      if (itr->second.uState == SKILL_UNCHANGED) {
        if (currVal == 0) // activated skill, mark as new to save into database
          itr->second.uState = SKILL_NEW;
        else // updated skill, mark as changed to save into database
          itr->second.uState = SKILL_CHANGED;
      }

      // archaeology handling
      if (id == SKILL_ARCHAEOLOGY) {
        if (itr->second.uState == SKILL_NEW)
          _archaeology->Learn();

        _archaeology->Update();
      }
    } else if (currVal) // Deactivate skill line
    {
      // remove enchantments needing this skill
      UpdateSkillEnchantments(id, currVal, 0);
      // clear skill fields
      SetSkillStep(itr->second.pos, 0);
      SetSkillRank(itr->second.pos, 0);
      SetSkillMaxRank(itr->second.pos, 0);
      SetSkillTempBonus(itr->second.pos, 0);
      SetSkillPermBonus(itr->second.pos, 0);

      // mark as deleted so the next save will delete the data from the database
      if (itr->second.uState != SKILL_NEW)
        itr->second.uState = SKILL_DELETED;
      else
        itr->second.uState = SKILL_UNCHANGED;

      // remove all spells that related to this skill
      if (std::vector<SkillLineAbilityEntry const *> const *skillLineAbilities =
              sDBCManager.GetSkillLineAbilitiesBySkill(id))
        for (SkillLineAbilityEntry const *skillLineAbility :
             *skillLineAbilities)
          RemoveSpell(sSpellMgr->GetFirstSpellInChain(skillLineAbility->Spell));

      // Clear profession lines
      if (GetUInt32Value(PLAYER_PROFESSION_SKILL_LINE_1) == id)
        SetUInt32Value(PLAYER_PROFESSION_SKILL_LINE_1, 0);
      else if (GetUInt32Value(PLAYER_PROFESSION_SKILL_LINE_1 + 1) == id)
        SetUInt32Value(PLAYER_PROFESSION_SKILL_LINE_1 + 1, 0);

      // archaeology skill unlearned
      if (id == SKILL_ARCHAEOLOGY)
        _archaeology->UnLearn();
    }
  } else {
    // We are about to learn a skill that has been added outside of normal
    // circumstances (Game Master command, scripts etc.)
    uint8 skillSlot = 0;

    // Find a free skill slot
    for (uint32 i = 0; i < PLAYER_MAX_SKILLS; ++i) {
      if (!GetUInt16Value(PLAYER_SKILL_LINEID_0 + i / 2, i & 1)) {
        skillSlot = i;
        break;
      }
    }

    if (!skillSlot) {
      LOG_ERROR("misc",
                "Tried to add skill %u but player %s (%s) cannot have "
                "additional skills",
                id, GetName().c_str(), GetGUID().ToString().c_str());
      return;
    }

    SkillLineEntry const *skillEntry = sSkillLineStore.LookupEntry(id);
    if (!skillEntry) {
      LOG_ERROR("misc", "Skill not found in SkillLineStore: skill #%u", id);
      return;
    }

    if (skillEntry->CategoryID == SKILL_CATEGORY_PROFESSION) {
      if (!GetUInt32Value(PLAYER_PROFESSION_SKILL_LINE_1))
        SetUInt32Value(PLAYER_PROFESSION_SKILL_LINE_1, id);
      else if (!GetUInt32Value(PLAYER_PROFESSION_SKILL_LINE_1 + 1))
        SetUInt32Value(PLAYER_PROFESSION_SKILL_LINE_1 + 1, id);
    }

    if (itr == mSkillStatus.end())
      SetSkillLineId(skillSlot, id);

    SetSkillStep(skillSlot, step);
    SetSkillRank(skillSlot, newVal);
    SetSkillMaxRank(skillSlot, maxVal);

    // apply skill bonuses
    SetSkillTempBonus(skillSlot, 0);
    SetSkillPermBonus(skillSlot, 0);

    UpdateSkillEnchantments(id, 0, newVal);

    mSkillStatus.insert(
        SkillStatusMap::value_type(id, SkillStatusData(skillSlot, SKILL_NEW)));

    if (newVal) {
      UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_REACH_SKILL_LEVEL,
                                id);
      UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LEVEL,
                                id);

      // temporary bonuses
      for (AuraEffect *effect : GetAuraEffectsByType(SPELL_AURA_MOD_SKILL))
        if (effect->GetMiscValue() == int32(id))
          effect->HandleEffect(this, AURA_EFFECT_HANDLE_SKILL, true);

      // permanent bonuses
      for (AuraEffect *effect :
           GetAuraEffectsByType(SPELL_AURA_MOD_SKILL_TALENT))
        if (effect->GetMiscValue() == int32(id))
          effect->HandleEffect(this, AURA_EFFECT_HANDLE_SKILL, true);

      // archaeology skill learned
      if (id == SKILL_ARCHAEOLOGY)
        _archaeology->Learn();

      // Learn all spells for skill
      LearnSkillRewardedSpells(id, newVal);
    }
  }

  if (Guild *guild = GetGuild())
    guild->UpdateMemberData(this, GUILD_MEMBER_DATA_PROFESSIONS, 0);
}

bool Player::HasSkill(uint32 skill) const {
  if (!skill)
    return false;

  SkillStatusMap::const_iterator itr = mSkillStatus.find(skill);
  return (itr != mSkillStatus.end() && itr->second.uState != SKILL_DELETED &&
          GetSkillRank(itr->second.pos));
}

uint16 Player::GetSkillStep(uint16 skill) const {
  if (!skill)
    return 0;

  SkillStatusMap::const_iterator itr = mSkillStatus.find(skill);
  if (itr == mSkillStatus.end() || itr->second.uState == SKILL_DELETED ||
      !GetSkillRank(itr->second.pos))
    return 0;

  return GetUInt16Value(PLAYER_SKILL_STEP_0 + itr->second.pos / 2,
                        itr->second.pos & 1);
}

uint16 Player::GetSkillValue(uint32 skill) const {
  if (!skill)
    return 0;

  SkillStatusMap::const_iterator itr = mSkillStatus.find(skill);
  if (itr == mSkillStatus.end() || itr->second.uState == SKILL_DELETED ||
      !GetSkillRank(itr->second.pos))
    return 0;

  uint16 field = itr->second.pos / 2;
  uint8 offset = itr->second.pos & 1;

  int32 result = int32(GetUInt16Value(PLAYER_SKILL_RANK_0 + field, offset));
  result += int32(GetUInt16Value(PLAYER_SKILL_MODIFIER_0 + field, offset));
  result += int32(GetUInt16Value(PLAYER_SKILL_TALENT_0 + field, offset));
  return (result < 0 ? 0 : result);
}

uint16 Player::GetMaxSkillValue(uint32 skill) const {
  if (!skill)
    return 0;

  SkillStatusMap::const_iterator itr = mSkillStatus.find(skill);
  if (itr == mSkillStatus.end() || itr->second.uState == SKILL_DELETED ||
      !GetSkillRank(itr->second.pos))
    return 0;

  uint16 field = itr->second.pos / 2;
  uint8 offset = itr->second.pos & 1;

  int32 result = int32(GetUInt16Value(PLAYER_SKILL_MAX_RANK_0 + field, offset));
  result += int32(GetUInt16Value(PLAYER_SKILL_MODIFIER_0 + field, offset));
  result += int32(GetUInt16Value(PLAYER_SKILL_TALENT_0 + field, offset));
  return (result < 0 ? 0 : result);
}

uint16 Player::GetPureMaxSkillValue(uint32 skill) const {
  if (!skill)
    return 0;

  SkillStatusMap::const_iterator itr = mSkillStatus.find(skill);
  if (itr == mSkillStatus.end() || itr->second.uState == SKILL_DELETED ||
      GetSkillRank(itr->second.pos))
    return 0;

  uint16 field = itr->second.pos / 2;
  uint8 offset = itr->second.pos & 1;

  return GetUInt16Value(PLAYER_SKILL_MAX_RANK_0 + field, offset);
}

uint16 Player::GetBaseSkillValue(uint32 skill) const {
  if (!skill)
    return 0;

  SkillStatusMap::const_iterator itr = mSkillStatus.find(skill);
  if (itr == mSkillStatus.end() || itr->second.uState == SKILL_DELETED ||
      !GetSkillRank(itr->second.pos))
    return 0;

  uint16 field = itr->second.pos / 2;
  uint8 offset = itr->second.pos & 1;

  int32 result = int32(GetUInt16Value(PLAYER_SKILL_RANK_0 + field, offset));
  result += int32(GetUInt16Value(PLAYER_SKILL_TALENT_0 + field, offset));
  return (result < 0 ? 0 : result);
}

uint16 Player::GetPureSkillValue(uint32 skill) const {
  if (!skill)
    return 0;

  SkillStatusMap::const_iterator itr = mSkillStatus.find(skill);
  if (itr == mSkillStatus.end() || itr->second.uState == SKILL_DELETED ||
      !GetSkillRank(itr->second.pos))
    return 0;

  uint16 field = itr->second.pos / 2;
  uint8 offset = itr->second.pos & 1;

  return GetUInt16Value(PLAYER_SKILL_RANK_0 + field, offset);
}

int16 Player::GetSkillPermBonusValue(uint32 skill) const {
  if (!skill)
    return 0;

  SkillStatusMap::const_iterator itr = mSkillStatus.find(skill);
  if (itr == mSkillStatus.end() || itr->second.uState == SKILL_DELETED ||
      !GetSkillRank(itr->second.pos))
    return 0;

  uint16 field = itr->second.pos / 2;
  uint8 offset = itr->second.pos & 1;

  return GetUInt16Value(PLAYER_SKILL_TALENT_0 + field, offset);
}

int16 Player::GetSkillTempBonusValue(uint32 skill) const {
  if (!skill)
    return 0;

  SkillStatusMap::const_iterator itr = mSkillStatus.find(skill);
  if (itr == mSkillStatus.end() || itr->second.uState == SKILL_DELETED ||
      !GetSkillRank(itr->second.pos))
    return 0;

  uint16 field = itr->second.pos / 2;
  uint8 offset = itr->second.pos & 1;

  return GetUInt16Value(PLAYER_SKILL_MODIFIER_0 + field, offset);
}

void Player::SendActionButtons(uint32 state) const {
  WorldPackets::Spells::UpdateActionButtons packet;

  if (state != 2) {
    for (auto itr = m_actionButtons.begin(); itr != m_actionButtons.end();
         ++itr)
      if (itr->second.uState != ACTIONBUTTON_DELETED &&
          itr->first < packet.ActionButtons.size())
        packet.ActionButtons[itr->first] = itr->second.packedData;
  }
  packet.Reason = state;

  SendDirectMessage(packet.Write());
}

bool Player::IsActionButtonDataValid(uint8 button, uint32 action,
                                     uint8 type) const {
  if (button >= MAX_ACTION_BUTTONS) {
    LOG_ERROR("entities.player",
              "Player::IsActionButtonDataValid: Action %u not added into "
              "button %u for player %s (%s): button must be < %u",
              action, button, GetName().c_str(), GetGUID().ToString().c_str(),
              MAX_ACTION_BUTTONS);
    return false;
  }

  if (action >= MAX_ACTION_BUTTON_ACTION_VALUE) {
    LOG_ERROR("entities.player",
              "Player::IsActionButtonDataValid: Action %u not added into "
              "button %u for player %s (%s): action must be < %u",
              action, button, GetName().c_str(), GetGUID().ToString().c_str(),
              MAX_ACTION_BUTTON_ACTION_VALUE);
    return false;
  }

  switch (type) {
  case ACTION_BUTTON_SPELL:
    if (!sSpellMgr->GetSpellInfo(action)) {
      LOG_DEBUG("entities.player",
                "Player::IsActionButtonDataValid: Spell action %u not added "
                "into button %u for player %s (%s): spell does not exist. This "
                "can be due to a character imported from a different expansion",
                action, button, GetName().c_str(),
                GetGUID().ToString().c_str());
      return false;
    }

    if (!HasSpell(action)) {
      LOG_DEBUG("entities.player",
                "Player::IsActionButtonDataValid: Spell action %u not added "
                "into button %u for player %s (%s): player does not known this "
                "spell, this can be due to a player changing their talents",
                action, button, GetName().c_str(),
                GetGUID().ToString().c_str());
      return false;
    }
    break;
  case ACTION_BUTTON_ITEM:
    if (!sObjectMgr->GetItemTemplate(action)) {
      LOG_ERROR("entities.player",
                "Player::IsActionButtonDataValid: Item action %u not added "
                "into button %u for player %s (%s): item not exist",
                action, button, GetName().c_str(),
                GetGUID().ToString().c_str());
      return false;
    }
    break;
  case ACTION_BUTTON_C:
  case ACTION_BUTTON_CMACRO:
  case ACTION_BUTTON_MACRO:
  case ACTION_BUTTON_EQSET:
  case ACTION_BUTTON_DROPDOWN:
    break;
  default:
    LOG_ERROR("entities.player",
              "Player::IsActionButtonDataValid: Unknown action type %u", type);
    return false; // other cases not checked at this moment
  }

  return true;
}

ActionButton *Player::addActionButton(uint8 button, uint32 action, uint8 type) {
  if (!IsActionButtonDataValid(button, action, type))
    return nullptr;

  // it create new button (NEW state) if need or return existing
  ActionButton &ab = m_actionButtons[button];

  // set data and update to CHANGED if not NEW
  ab.SetActionAndType(action, ActionButtonType(type));

  LOG_DEBUG("entities.player",
            "Player::AddActionButton: Player '%s' (%s) added action '%u' (type "
            "%u) to button '%u'",
            GetName().c_str(), GetGUID().ToString().c_str(), action, type,
            button);
  return &ab;
}

void Player::removeActionButton(uint8 button) {
  ActionButtonList::iterator buttonItr = m_actionButtons.find(button);
  if (buttonItr == m_actionButtons.end() ||
      buttonItr->second.uState == ACTIONBUTTON_DELETED)
    return;

  if (buttonItr->second.uState == ACTIONBUTTON_NEW)
    m_actionButtons.erase(buttonItr); // new and not saved
  else
    buttonItr->second.uState =
        ACTIONBUTTON_DELETED; // saved, will deleted at next save

  LOG_DEBUG(
      "entities.player",
      "Player::RemoveActionButton: Player '%s' (%s) removed action button '%u'",
      GetName().c_str(), GetGUID().ToString().c_str(), button);
}

ActionButton const *Player::GetActionButton(uint8 button) {
  ActionButtonList::iterator buttonItr = m_actionButtons.find(button);
  if (buttonItr == m_actionButtons.end() ||
      buttonItr->second.uState == ACTIONBUTTON_DELETED)
    return nullptr;

  return &buttonItr->second;
}

bool Player::UpdatePosition(float x, float y, float z, float orientation,
                            bool teleport) {
  uint32 oldWmoGroupId = GetWMOGroupId();

  if (!Unit::UpdatePosition(x, y, z, orientation, teleport))
    return false;
  // if (movementInfo.flags & MOVEMENTFLAG_MOVING)
  //     mover->RemoveAurasWithInterruptFlags(SpellAuraInterruptFlags::Moving);
  // if (movementInfo.flags & MOVEMENTFLAG_TURNING)
  //     mover->RemoveAurasWithInterruptFlags(SpellAuraInterruptFlags::Turning);

  // Update player zone if needed
  if (m_needsZoneUpdate) {
    uint32 newZone, newArea;
    GetZoneAndAreaId(newZone, newArea);
    UpdateZone(newZone, newArea);
    m_needsZoneUpdate = false;
  }

  // Add group update flags
  if (GetGroup()) {
    // Position has changed
    SetGroupUpdateFlag(GROUP_UPDATE_FLAG_POSITION);

    // Player is no longer on a WMO that belongs to the same area
    if (oldWmoGroupId != GetWMOGroupId())
      SetGroupUpdateFlag(GROUP_UPDATE_FLAG_WMO_GROUP_ID);
  }

  CheckAreaExploreAndOutdoor();
  return true;
}

void Player::SendMessageToSetInRange(WorldPacket const *data, float dist,
                                     bool self) const {
  if (self)
    SendDirectMessage(data);

  Firelands::MessageDistDeliverer notifier(this, data, dist);
  Cell::VisitWorldObjects(this, notifier, dist);
}

void Player::SendMessageToSetInRange(WorldPacket const *data, float dist,
                                     bool self, bool own_team_only) const {
  if (self)
    SendDirectMessage(data);

  Firelands::MessageDistDeliverer notifier(this, data, dist, own_team_only);
  Cell::VisitWorldObjects(this, notifier, dist);
}

void Player::SendMessageToSet(WorldPacket const *data,
                              Player const *skipped_rcvr) const {
  if (skipped_rcvr != this)
    SendDirectMessage(data);

  // we use World::GetMaxVisibleDistance() because i cannot see why not use a
  // distance update: replaced by GetMap()->GetVisibilityDistance()
  Firelands::MessageDistDeliverer notifier(this, data, GetVisibilityRange(),
                                           false, skipped_rcvr);
  Cell::VisitWorldObjects(this, notifier, GetVisibilityRange());
}

void Player::SendDirectMessage(WorldPacket const *data) const {
  m_session->SendPacket(data);
}

void Player::SendCinematicStart(uint32 cinematicId) {
  WorldPackets::Misc::TriggerCinematic packet;
  packet.CinematicID = cinematicId;
  SendDirectMessage(packet.Write());

  if (CinematicSequencesEntry const *sequence =
          sCinematicSequencesStore.LookupEntry(cinematicId))
    _cinematicMgr->SetActiveCinematicCamera(sequence->Camera[0]);
}

void Player::SendMovieStart(uint32 movieId) {
  SetMovie(movieId);
  WorldPackets::Misc::TriggerMovie packet;
  packet.MovieID = movieId;
  SendDirectMessage(packet.Write());
}

void Player::CheckAreaExploreAndOutdoor() {
  if (!IsAlive())
    return;

  if (IsInFlight())
    return;

  if (sWorld->getBoolConfig(CONFIG_VMAP_INDOOR_CHECK) && !IsOutdoors())
    RemoveAurasWithAttribute(SPELL_ATTR0_OUTDOORS_ONLY);

  uint32 const areaId = GetAreaId();
  AreaTableEntry const *areaEntry = sAreaTableStore.LookupEntry(areaId);

  if (!areaEntry) {
    LOG_ERROR(
        "entities.player",
        "Player '%s' (%s) discovered unknown area (x: %f y: %f z: %f map: %u)",
        GetName().c_str(), GetGUID().ToString().c_str(), GetPositionX(),
        GetPositionY(), GetPositionZ(), GetMapId());
    return;
  }

  uint32 offset = areaEntry->AreaBit / 32;

  if (offset >= PLAYER_EXPLORED_ZONES_SIZE) {
    LOG_ERROR("entities.player",
              "Player::CheckAreaExploreAndOutdoor: Wrong area flag %u in map "
              "data for (X: %f Y: %f) point to field PLAYER_EXPLORED_ZONES_1 + "
              "%u ( %u must be < %u ).",
              areaEntry->AreaBit, GetPositionX(), GetPositionY(), offset,
              offset, PLAYER_EXPLORED_ZONES_SIZE);
    return;
  }

  uint32 val = (uint32)(1 << (areaEntry->AreaBit % 32));
  uint32 currFields = GetUInt32Value(PLAYER_EXPLORED_ZONES_1 + offset);

  if (!(currFields & val)) {
    SetUInt32Value(PLAYER_EXPLORED_ZONES_1 + offset,
                   (uint32)(currFields | val));

    UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_EXPLORE_AREA,
                              GetAreaId());

    if (areaEntry->ExplorationLevel > 0) {
      if (IsMaxLevel())
        SendExplorationExperience(areaId, 0);
      else {
        int32 diff = int32(getLevel()) - areaEntry->ExplorationLevel;
        uint32 XP;
        if (diff < -5) {
          XP = uint32(sObjectMgr->GetBaseXP(getLevel() + 5) *
                      sWorld->getRate(RATE_XP_EXPLORE));
        } else if (diff > 5) {
          int32 exploration_percent = 100 - ((diff - 5) * 5);
          if (exploration_percent < 0)
            exploration_percent = 0;

          XP = uint32(sObjectMgr->GetBaseXP(areaEntry->ExplorationLevel) *
                      exploration_percent / 100 *
                      sWorld->getRate(RATE_XP_EXPLORE));
        } else {
          XP = uint32(sObjectMgr->GetBaseXP(areaEntry->ExplorationLevel) *
                      sWorld->getRate(RATE_XP_EXPLORE));
        }

        if (sWorld->getIntConfig(CONFIG_MIN_DISCOVERED_SCALED_XP_RATIO)) {
          uint32 minScaledXP =
              uint32(sObjectMgr->GetBaseXP(areaEntry->ExplorationLevel) *
                     sWorld->getRate(RATE_XP_EXPLORE)) *
              sWorld->getIntConfig(CONFIG_MIN_DISCOVERED_SCALED_XP_RATIO) / 100;
          XP = std::max(minScaledXP, XP);
        }

        GiveXP(XP, nullptr);
        SendExplorationExperience(areaId, XP);
      }
      LOG_DEBUG("entities.player", "Player '%s' (%s) discovered a new area: %u",
                GetName().c_str(), GetGUID().ToString().c_str(), areaId);
    }
  }
}

uint32 Player::TeamForRace(uint8 race) {
  if (ChrRacesEntry const *rEntry = sChrRacesStore.LookupEntry(race)) {
    switch (rEntry->BaseLanguage) {
    case 1:
      return HORDE;
    case 7:
      return ALLIANCE;
    }
    LOG_ERROR("entities.player",
              "Race (%u) has wrong teamid (%u) in DBC: wrong DBC files?",
              uint32(race), rEntry->BaseLanguage);
  } else
    LOG_ERROR("entities.player", "Race (%u) not found in DBC: wrong DBC files?",
              uint32(race));

  return ALLIANCE;
}

void Player::SetFactionForRace(uint8 race) {
  m_team = TeamForRace(race);

  ChrRacesEntry const *rEntry = sChrRacesStore.LookupEntry(race);
  SetFaction(rEntry ? rEntry->FactionID : 0);
}

ReputationRank Player::GetReputationRank(uint32 faction) const {
  FactionEntry const *factionEntry = sFactionStore.LookupEntry(faction);
  return GetReputationMgr().GetRank(factionEntry);
}

// Calculate total reputation percent player gain with quest/creature level
int32 Player::CalculateReputationGain(ReputationSource source,
                                      uint32 creatureOrQuestLevel, int32 rep,
                                      int32 faction, bool noQuestBonus) {
  float percent = 100.0f;

  float repMod =
      noQuestBonus
          ? 0.0f
          : float(GetTotalAuraModifier(SPELL_AURA_MOD_REPUTATION_GAIN));

  // faction specific auras only seem to apply to kills
  if (source == REPUTATION_SOURCE_KILL)
    repMod += GetTotalAuraModifierByMiscValue(
        SPELL_AURA_MOD_FACTION_REPUTATION_GAIN, faction);

  percent += rep > 0 ? repMod : -repMod;

  float rate;
  switch (source) {
  case REPUTATION_SOURCE_KILL:
    rate = sWorld->getRate(RATE_REPUTATION_LOWLEVEL_KILL);
    break;
  case REPUTATION_SOURCE_QUEST:
  case REPUTATION_SOURCE_DAILY_QUEST:
  case REPUTATION_SOURCE_WEEKLY_QUEST:
  case REPUTATION_SOURCE_MONTHLY_QUEST:
  case REPUTATION_SOURCE_REPEATABLE_QUEST:
    rate = sWorld->getRate(RATE_REPUTATION_LOWLEVEL_QUEST);
    break;
  case REPUTATION_SOURCE_SPELL:
  default:
    rate = 1.0f;
    break;
  }

  if (rate != 1.0f &&
      creatureOrQuestLevel <= Firelands::XP::GetGrayLevel(getLevel()))
    percent *= rate;

  if (percent <= 0.0f)
    return 0;

  // Multiply result with the faction specific rate
  if (RepRewardRate const *repData = sObjectMgr->GetRepRewardRate(faction)) {
    float repRate = 0.0f;
    switch (source) {
    case REPUTATION_SOURCE_KILL:
      repRate = repData->creatureRate;
      break;
    case REPUTATION_SOURCE_QUEST:
      repRate = repData->questRate;
      break;
    case REPUTATION_SOURCE_DAILY_QUEST:
      repRate = repData->questDailyRate;
      break;
    case REPUTATION_SOURCE_WEEKLY_QUEST:
      repRate = repData->questWeeklyRate;
      break;
    case REPUTATION_SOURCE_MONTHLY_QUEST:
      repRate = repData->questMonthlyRate;
      break;
    case REPUTATION_SOURCE_REPEATABLE_QUEST:
      repRate = repData->questRepeatableRate;
      break;
    case REPUTATION_SOURCE_SPELL:
      repRate = repData->spellRate;
      break;
    }

    // for custom, a rate of 0.0 will totally disable reputation gain for this
    // faction/type
    if (repRate <= 0.0f)
      return 0;

    percent *= repRate;
  }

  if (source != REPUTATION_SOURCE_SPELL && GetsRecruitAFriendBonus(false))
    percent *= 1.0f + sWorld->getRate(RATE_REPUTATION_RECRUIT_A_FRIEND_BONUS);

  return CalculatePct(rep, percent);
}

// Calculates how many reputation and currency the player gets uppon killing an
// creature
void Player::RewardOnKill(Unit *victim, float rate) {
  if (!victim || victim->GetTypeId() == TYPEID_PLAYER)
    return;

  if (victim->ToCreature()->IsReputationGainDisabled())
    return;

  RewardOnKillEntry const *Rew = sObjectMgr->GetRewardOnKillEntry(
      victim->ToCreature()->GetCreatureTemplate()->Entry);

  if (!Rew)
    return;

  uint32 ChampioningFaction = 0;

  if (GetChampioningFaction()) {
    // support for: Championing - http://www.wowwiki.com/Championing
    Map const *map = GetMap();
    if (map->IsNonRaidDungeon()) {
      if (LFGDungeonEntry const *dungeon =
              DBCManager::GetLFGDungeon(map->GetId(), map->GetDifficulty())) {
        // WotLK and Cataclysm dungeons only grant championing reputation when
        // being in max level dungeons and fighting for a expansion related
        // faction
        uint8 expansion = GetExpansionForFaction(GetChampioningFaction());
        if (dungeon->Target_level ==
                DBCManager::GetMaxLevelForExpansion(dungeon->ExpansionLevel) &&
            expansion) {
          if (expansion == dungeon->ExpansionLevel)
            ChampioningFaction = GetChampioningFaction();
        } else {
          // Classic Tabards (Alliance and Horde capital reputation) always
          // convert the gained reputation
          if (!expansion)
            ChampioningFaction = GetChampioningFaction();
        }
      }
    }
  }

  uint32 team = GetTeam();

  if (Rew->RepFaction1 && (!Rew->TeamDependent || team == ALLIANCE)) {
    if (FactionEntry const *rewFactionEntry1 =
            sFactionStore.LookupEntry(Rew->RepFaction1)) {
      // Patch 4.3: Wearing Horde or Alliance racial faction tabards now grants
      // reputation in Burning Crusade dungeons as well.
      /*
          Note: retail tests have shown that burning crusade dungeons are the
         only kind of dungeons that grant championing and usual reputation at
         the same time which is why we duplicate the reward and modify the 2nd
      */
      bool secondaryReputationReward1 =
          (GetExpansionForFaction(rewFactionEntry1->ParentFactionID) ==
               EXPANSION_THE_BURNING_CRUSADE ||
           GetExpansionForFaction(Rew->RepFaction1) ==
               EXPANSION_THE_BURNING_CRUSADE);

      uint32 factionId1 = (ChampioningFaction && !secondaryReputationReward1)
                              ? ChampioningFaction
                              : Rew->RepFaction1;
      if (rewFactionEntry1->Expansion && !ChampioningFaction)
        factionId1 = 0;

      int32 donerep1 =
          CalculateReputationGain(REPUTATION_SOURCE_KILL, victim->getLevel(),
                                  Rew->RepValue1, factionId1);
      donerep1 = int32(donerep1 * rate);

      FactionEntry const *factionEntry1 = sFactionStore.LookupEntry(factionId1);
      uint32 current_reputation_rank1 =
          GetReputationMgr().GetRank(factionEntry1);
      if (factionEntry1)
        GetReputationMgr().ModifyReputation(factionEntry1, donerep1,
                                            current_reputation_rank1 >
                                                Rew->ReputationMaxCap1);

      if (factionEntry1 && secondaryReputationReward1 && ChampioningFaction) {
        FactionEntry const *bonusFactionEntry1 =
            sFactionStore.LookupEntry(ChampioningFaction);
        uint32 current_bonus_reputation_rank1 =
            GetReputationMgr().GetRank(bonusFactionEntry1);

        if (bonusFactionEntry1)
          GetReputationMgr().ModifyReputation(bonusFactionEntry1, donerep1,
                                              current_bonus_reputation_rank1 >
                                                  Rew->ReputationMaxCap1);
      }
    }
  }

  if (Rew->RepFaction2 && (!Rew->TeamDependent || team == HORDE)) {
    if (FactionEntry const *rewFactionEntry2 =
            sFactionStore.LookupEntry(Rew->RepFaction2)) {
      bool secondaryReputationReward2 =
          (GetExpansionForFaction(rewFactionEntry2->ParentFactionID) ==
               EXPANSION_THE_BURNING_CRUSADE ||
           GetExpansionForFaction(Rew->RepFaction1) ==
               EXPANSION_THE_BURNING_CRUSADE);

      uint32 factionId2 = (ChampioningFaction && secondaryReputationReward2)
                              ? ChampioningFaction
                              : Rew->RepFaction2;
      if (rewFactionEntry2->Expansion && !ChampioningFaction)
        factionId2 = 0;

      int32 donerep2 =
          CalculateReputationGain(REPUTATION_SOURCE_KILL, victim->getLevel(),
                                  Rew->RepValue2, factionId2);
      donerep2 = int32(donerep2 * rate);

      FactionEntry const *factionEntry2 = sFactionStore.LookupEntry(factionId2);
      uint32 current_reputation_rank2 =
          GetReputationMgr().GetRank(factionEntry2);
      if (factionEntry2)
        GetReputationMgr().ModifyReputation(factionEntry2, donerep2,
                                            current_reputation_rank2 >
                                                Rew->ReputationMaxCap2);

      if (factionEntry2 && secondaryReputationReward2 && ChampioningFaction) {
        FactionEntry const *bonusFactionEntry2 =
            sFactionStore.LookupEntry(ChampioningFaction);
        uint32 current_bonus_reputation_rank2 =
            GetReputationMgr().GetRank(bonusFactionEntry2);

        if (bonusFactionEntry2)
          GetReputationMgr().ModifyReputation(bonusFactionEntry2, donerep2,
                                              current_bonus_reputation_rank2 >
                                                  Rew->ReputationMaxCap1);
      }
    }
  }

  if (Rew->CurrencyId1 && Rew->CurrencyCount1) {
    CurrencyTypesEntry const *currencyId1 =
        sCurrencyTypesStore.LookupEntry(Rew->CurrencyId1);
    if (currencyId1)
      ModifyCurrency(Rew->CurrencyId1, Rew->CurrencyCount1);
  }

  if (Rew->CurrencyId2 && Rew->CurrencyCount2) {
    CurrencyTypesEntry const *currencyId2 =
        sCurrencyTypesStore.LookupEntry(Rew->CurrencyId2);
    if (currencyId2)
      ModifyCurrency(Rew->CurrencyId2, Rew->CurrencyCount2);
  }

  if (Rew->CurrencyId3 && Rew->CurrencyCount3) {
    CurrencyTypesEntry const *currencyId3 =
        sCurrencyTypesStore.LookupEntry(Rew->CurrencyId3);
    if (currencyId3)
      ModifyCurrency(Rew->CurrencyId3, Rew->CurrencyCount3);
  }
}

// Calculate how many reputation points player gain with the quest
void Player::RewardReputation(Quest const *quest) {
  for (uint8 i = 0; i < QUEST_REPUTATIONS_COUNT; ++i) {
    if (!quest->RewardFactionId[i])
      continue;

    int32 rep = 0;
    bool noQuestBonus = false;

    if (quest->RewardFactionValueIdOverride[i]) {
      rep = quest->RewardFactionValueIdOverride[i] / 100;
      noQuestBonus = true;
    } else {
      uint32 row = ((quest->RewardFactionValueId[i] < 0) ? 1 : 0) + 1;
      if (QuestFactionRewEntry const *questFactionRewEntry =
              sQuestFactionRewardStore.LookupEntry(row)) {
        uint32 field = abs(quest->RewardFactionValueId[i]);
        rep = questFactionRewEntry->Difficulty[field];
      }
    }

    if (!rep)
      continue;

    if (quest->IsDaily())
      rep = CalculateReputationGain(REPUTATION_SOURCE_DAILY_QUEST,
                                    GetQuestLevel(quest), rep,
                                    quest->RewardFactionId[i], noQuestBonus);
    else if (quest->IsWeekly())
      rep = CalculateReputationGain(REPUTATION_SOURCE_WEEKLY_QUEST,
                                    GetQuestLevel(quest), rep,
                                    quest->RewardFactionId[i], noQuestBonus);
    else if (quest->IsMonthly())
      rep = CalculateReputationGain(REPUTATION_SOURCE_MONTHLY_QUEST,
                                    GetQuestLevel(quest), rep,
                                    quest->RewardFactionId[i], noQuestBonus);
    else if (quest->IsRepeatable())
      rep = CalculateReputationGain(REPUTATION_SOURCE_REPEATABLE_QUEST,
                                    GetQuestLevel(quest), rep,
                                    quest->RewardFactionId[i], noQuestBonus);
    else
      rep =
          CalculateReputationGain(REPUTATION_SOURCE_QUEST, GetQuestLevel(quest),
                                  rep, quest->RewardFactionId[i], noQuestBonus);

    if (FactionEntry const *factionEntry =
            sFactionStore.LookupEntry(quest->RewardFactionId[i]))
      GetReputationMgr().ModifyReputation(factionEntry, rep);
  }
}

void Player::UpdateHonorFields() {
  /// called when rewarding honor and at each save
  time_t now = time_t(GameTime::GetGameTime());
  time_t today = time_t(GameTime::GetGameTime() / DAY) * DAY;

  if (m_lastHonorUpdateTime < today) {
    time_t yesterday = today - DAY;

    uint16 kills_today = PAIR32_LOPART(GetUInt32Value(PLAYER_FIELD_KILLS));

    // update yesterday's contribution
    if (m_lastHonorUpdateTime >= yesterday) {
      // this is the first update today, reset today's contribution
      SetUInt32Value(PLAYER_FIELD_KILLS, MAKE_PAIR32(0, kills_today));
    } else {
      // no honor/kills yesterday or today, reset
      SetUInt32Value(PLAYER_FIELD_KILLS, 0);
    }
  }

  m_lastHonorUpdateTime = now;
}

/// Calculate the amount of honor gained based on the victim
/// and the size of the group for which the honor is divided
/// An exact honor value can also be given (overriding the calcs)
bool Player::RewardHonor(Unit *victim, uint32 groupsize, int32 honor,
                         bool pvptoken) {
  // do not reward honor in arenas, but enable onkill spellproc
  if (InArena()) {
    if (!victim || victim == this || victim->GetTypeId() != TYPEID_PLAYER)
      return false;

    if (GetBGTeam() == victim->ToPlayer()->GetBGTeam())
      return false;

    return true;
  }

  // 'Inactive' this aura prevents the player from gaining honor points and
  // battleground Tokenize
  if (HasAura(SPELL_AURA_PLAYER_INACTIVE))
    return false;

  ObjectGuid victim_guid;
  uint32 victim_rank = 0;

  // need call before fields update to have chance move yesterday data to
  // appropriate fields before today data change.
  UpdateHonorFields();

  // do not reward honor in arenas, but return true to enable onkill spellproc
  if (InBattleground() && GetBattleground() && GetBattleground()->isArena())
    return true;

  // Promote to float for calculations
  float honor_f = (float)honor;

  if (honor_f <= 0) {
    if (!victim || victim == this ||
        victim->HasAuraType(SPELL_AURA_NO_PVP_CREDIT))
      return false;

    victim_guid = victim->GetGUID();

    if (Player *plrVictim = victim->ToPlayer()) {
      if (GetTeam() == plrVictim->GetTeam() && !sWorld->IsFFAPvPRealm())
        return false;

      uint8 k_level = getLevel();
      uint8 k_grey = Firelands::XP::GetGrayLevel(k_level);
      uint8 v_level = victim->getLevel();

      if (v_level <= k_grey &&
          !sWorld->getIntConfig(CONFIG_MIN_CREATURE_SCALED_XP_RATIO))
        return false;

      // PLAYER_CHOSEN_TITLE VALUES DESCRIPTION
      //  [0]      Just name
      //  [1..14]  Alliance honor titles and player name
      //  [15..28] Horde honor titles and player name
      //  [29..38] Other title and player name
      //  [39+]    Nothing
      uint32 victim_title = victim->GetUInt32Value(PLAYER_CHOSEN_TITLE);
      // Get Killer titles, CharTitlesEntry::bit_index
      // Ranks:
      //  title[1..14]  -> rank[5..18]
      //  title[15..28] -> rank[5..18]
      //  title[other]  -> 0
      if (victim_title == 0)
        victim_guid.Clear(); // Don't show HK: <rank> message, only log.
      else if (victim_title < 15)
        victim_rank = victim_title + 4;
      else if (victim_title < 29)
        victim_rank = victim_title - 14 + 4;
      else
        victim_guid.Clear(); // Don't show HK: <rank> message, only log.

      honor_f = std::ceil(Firelands::Honor::hk_honor_at_level_f(k_level) *
                          (v_level - k_grey) / (k_level - k_grey));

      // count the number of playerkills in one day
      ApplyModUInt32Value(PLAYER_FIELD_KILLS, 1, true);
      // and those in a lifetime
      ApplyModUInt32Value(PLAYER_FIELD_LIFETIME_HONORABLE_KILLS, 1, true);
      UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_EARN_HONORABLE_KILL);
      UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_HK_CLASS,
                                victim->getClass());
      UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_HK_RACE,
                                victim->getRace());
      UpdateAchievementCriteria(
          ACHIEVEMENT_CRITERIA_TYPE_HONORABLE_KILL_AT_AREA, GetAreaId());
      UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_HONORABLE_KILL, 1, 0,
                                0, victim);
      if (Guild *guild = GetGuild())
        guild->UpdateAchievementCriteria(
            ACHIEVEMENT_CRITERIA_TYPE_HONORABLE_KILLS_GUILD, 0, 0, 0, victim,
            this);
    } else {
      if (!victim->ToCreature()->IsRacialLeader())
        return false;

      honor_f = 100.0f; // ??? need more info
      victim_rank = 19; // HK: Leader
    }
  }

  if (victim != nullptr) {
    if (groupsize > 1)
      honor_f /= groupsize;

    // apply honor multiplier from aura (not stacking-get highest)
    AddPct(honor_f, GetMaxPositiveAuraModifier(SPELL_AURA_MOD_HONOR_GAIN_PCT));
  }

  honor_f *= sWorld->getRate(RATE_HONOR);
  // Back to int now
  honor = int32(honor_f);
  // honor - for show honor points in log
  // victim_guid - for show victim name in log
  // victim_rank [1..4]  HK: <dishonored rank>
  // victim_rank [5..19] HK: <alliance\horde rank>
  // victim_rank [0, 20+] HK: <>
  WorldPackets::Combat::PvPCredit data;
  data.Honor = honor;
  data.Target = victim_guid;
  data.Rank = victim_rank;

  SendDirectMessage(data.Write());

  // add honor points
  ModifyCurrency(CURRENCY_TYPE_HONOR_POINTS, int32(honor));

  // Add guild XP
  if (Guild *guild = GetGuild())
    guild->GiveXP(uint32(float(honor) *
                         sWorld->getRate(RATE_XP_HONOR_EARNED_GUILD_MODIFIER)),
                  this);

  if (InBattleground() && honor > 0) {
    if (Battleground *bg = GetBattleground()) {
      bg->UpdatePlayerScore(this, SCORE_BONUS_HONOR, honor,
                            false); // false: prevent looping
    }
  }

  if (sWorld->getBoolConfig(CONFIG_PVP_TOKEN_ENABLE) && pvptoken) {
    if (!victim || victim == this ||
        victim->HasAuraType(SPELL_AURA_NO_PVP_CREDIT))
      return true;

    if (victim->GetTypeId() == TYPEID_PLAYER) {
      // Check if allowed to receive it in current map
      uint8 MapType = sWorld->getIntConfig(CONFIG_PVP_TOKEN_MAP_TYPE);
      if ((MapType == 1 && !InBattleground() && !IsFFAPvP()) ||
          (MapType == 2 && !IsFFAPvP()) || (MapType == 3 && !InBattleground()))
        return true;

      uint32 itemId = sWorld->getIntConfig(CONFIG_PVP_TOKEN_ID);
      int32 count = sWorld->getIntConfig(CONFIG_PVP_TOKEN_COUNT);

      if (AddItem(itemId, count))
        ChatHandler(GetSession())
            .PSendSysMessage(
                "You have been awarded a token for slaying another player.");
    }
  }

  return true;
}

void Player::_LoadCurrency(PreparedQueryResult result) {
  if (!result)
    return;

  do {
    Field *fields = result->Fetch();

    uint16 currencyID = fields[0].Get<uint16>();

    CurrencyTypesEntry const *currency =
        sCurrencyTypesStore.LookupEntry(currencyID);
    if (!currency)
      continue;

    PlayerCurrency cur;
    cur.state = PLAYERCURRENCY_UNCHANGED;
    cur.Quantity = fields[1].Get<uint32>();
    cur.WeeklyQuantity = fields[2].Get<uint32>();
    cur.TrackedQuantity = fields[3].Get<uint32>();
    cur.Flags = fields[4].Get<uint8>();

    _currencyStorage.insert(PlayerCurrenciesMap::value_type(currencyID, cur));

  } while (result->NextRow());
}

void Player::_SaveCurrency(CharacterDatabaseTransaction &trans) {
  CharacterDatabasePreparedStatement *stmt = nullptr;
  for (PlayerCurrenciesMap::iterator itr = _currencyStorage.begin();
       itr != _currencyStorage.end(); ++itr) {
    CurrencyTypesEntry const *entry =
        sCurrencyTypesStore.LookupEntry(itr->first);
    if (!entry) // should never happen
      continue;

    switch (itr->second.state) {
    case PLAYERCURRENCY_NEW:
      stmt = CharacterDatabase.GetPreparedStatement(CHAR_REP_PLAYER_CURRENCY);
      stmt->SetData(0, GetGUID().GetCounter());
      stmt->SetData(1, itr->first);
      stmt->SetData(2, itr->second.Quantity);
      stmt->SetData(3, itr->second.WeeklyQuantity);
      stmt->SetData(4, itr->second.TrackedQuantity);
      stmt->SetData(5, itr->second.Flags);
      trans->Append(stmt);
      break;
    case PLAYERCURRENCY_CHANGED:
      stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_PLAYER_CURRENCY);
      stmt->SetData(0, itr->second.Quantity);
      stmt->SetData(1, itr->second.WeeklyQuantity);
      stmt->SetData(2, itr->second.TrackedQuantity);
      stmt->SetData(3, itr->second.Flags);
      stmt->SetData(4, GetGUID().GetCounter());
      stmt->SetData(5, itr->first);
      trans->Append(stmt);
      break;
    default:
      break;
    }

    itr->second.state = PLAYERCURRENCY_UNCHANGED;
  }
}

void Player::SendNewCurrency(uint32 id) const {
  PlayerCurrenciesMap::const_iterator itr = _currencyStorage.find(id);
  if (itr == _currencyStorage.end())
    return;

  CurrencyTypesEntry const *entry = sCurrencyTypesStore.LookupEntry(id);
  if (!entry) // should never happen
    return;

  WorldPackets::Misc::SetupCurrency packet;
  WorldPackets::Misc::SetupCurrency::Record record;
  record.Type = entry->ID;
  record.Quantity = itr->second.Quantity;
  record.WeeklyQuantity = itr->second.WeeklyQuantity;
  record.MaxWeeklyQuantity = GetCurrencyWeekCap(entry);
  record.TrackedQuantity = itr->second.TrackedQuantity;
  record.Flags = itr->second.Flags;

  packet.Data.push_back(record);

  GetSession()->SendPacket(packet.Write());
}

void Player::SendCurrencies() const {
  WorldPackets::Misc::SetupCurrency packet;

  for (PlayerCurrenciesMap::const_iterator itr = _currencyStorage.begin();
       itr != _currencyStorage.end(); ++itr) {
    CurrencyTypesEntry const *entry =
        sCurrencyTypesStore.LookupEntry(itr->first);

    // not send init meta currencies.
    if (!entry || entry->CategoryID == CURRENCY_CATEGORY_META_CONQUEST)
      continue;

    uint32 precision =
        (entry->Flags & CURRENCY_FLAG_HIGH_PRECISION) ? CURRENCY_PRECISION : 1;

    WorldPackets::Misc::SetupCurrency::Record record;
    record.Type = entry->ID;
    record.Quantity = itr->second.Quantity / precision;
    record.WeeklyQuantity = itr->second.WeeklyQuantity / precision;
    record.MaxWeeklyQuantity = GetCurrencyWeekCap(entry) / precision;

    if (entry->Flags & CURRENCY_FLAG_COUNT_SEASON_TOTAL)
      record.TrackedQuantity = itr->second.TrackedQuantity / precision;

    record.Flags = itr->second.state;
    packet.Data.push_back(record);
  }

  SendDirectMessage(packet.Write());
}

void Player::SendPvpRewards() const {
  WorldPackets::Misc::RequestPVPRewardsResponse packet;
  packet.ArenaMaxRewardPointsThisWeek =
      GetCurrencyWeekCap(CURRENCY_TYPE_CONQUEST_META_ARENA, true);
  packet.ArenaRewardPointsThisWeek =
      GetCurrencyOnWeek(CURRENCY_TYPE_CONQUEST_META_ARENA, true);
  packet.MaxRewardPointsThisWeek =
      GetCurrencyWeekCap(CURRENCY_TYPE_CONQUEST_POINTS, true);
  packet.RewardPointsThisWeek =
      GetCurrencyOnWeek(CURRENCY_TYPE_CONQUEST_POINTS, true);
  packet.RatedMaxRewardPointsThisWeek =
      GetCurrencyWeekCap(CURRENCY_TYPE_CONQUEST_META_RBG, true);
  packet.RatedRewardPointsThisWeek =
      GetCurrencyOnWeek(CURRENCY_TYPE_CONQUEST_META_RBG, true);

  SendDirectMessage(packet.Write());
}

uint32 Player::GetCurrency(uint32 id, bool usePrecision) const {
  PlayerCurrenciesMap::const_iterator itr = _currencyStorage.find(id);
  if (itr == _currencyStorage.end())
    return 0;

  CurrencyTypesEntry const *currency = sCurrencyTypesStore.LookupEntry(id);
  uint32 precision =
      (usePrecision && currency->Flags & CURRENCY_FLAG_HIGH_PRECISION)
          ? CURRENCY_PRECISION
          : 1;

  return itr->second.Quantity / precision;
}

uint32 Player::GetCurrencyOnWeek(uint32 id, bool usePrecision) const {
  PlayerCurrenciesMap::const_iterator itr = _currencyStorage.find(id);
  if (itr == _currencyStorage.end())
    return 0;

  CurrencyTypesEntry const *currency = sCurrencyTypesStore.LookupEntry(id);
  uint32 precision =
      (usePrecision && currency->Flags & CURRENCY_FLAG_HIGH_PRECISION)
          ? CURRENCY_PRECISION
          : 1;

  return itr->second.WeeklyQuantity / precision;
}

uint32 Player::GetCurrencyOnSeason(uint32 id) const {
  PlayerCurrenciesMap::const_iterator itr = _currencyStorage.find(id);
  if (itr == _currencyStorage.end())
    return 0;

  return itr->second.TrackedQuantity;
}

bool Player::HasCurrency(uint32 id, uint32 count) const {
  PlayerCurrenciesMap::const_iterator itr = _currencyStorage.find(id);
  return itr != _currencyStorage.end() && itr->second.Quantity >= count;
}

void Player::ModifyCurrency(uint32 id, int32 count, bool printLog /* = true*/,
                            bool ignoreMultipliers /* = false*/,
                            bool isRefund /* = false*/) {
  if (!count)
    return;

  CurrencyTypesEntry const *currency = sCurrencyTypesStore.LookupEntry(id);
  ASSERT(currency);

  if (!ignoreMultipliers && !isRefund)
    count *=
        GetTotalAuraMultiplierByMiscValue(SPELL_AURA_MOD_CURRENCY_GAIN, id);

  int32 precision =
      currency->Flags & CURRENCY_FLAG_HIGH_PRECISION ? CURRENCY_PRECISION : 1;
  uint32 oldTotalCount = 0;
  uint32 oldWeekCount = 0;
  uint32 oldTrackedCount = 0;
  bool hasSeasonCount = false;
  PlayerCurrenciesMap::iterator itr = _currencyStorage.find(id);
  if (itr == _currencyStorage.end()) {
    PlayerCurrency cur;
    cur.state = PLAYERCURRENCY_NEW;
    cur.Quantity = 0;
    cur.WeeklyQuantity = 0;
    cur.TrackedQuantity = 0;
    cur.Flags = 0;
    _currencyStorage[id] = cur;
    itr = _currencyStorage.find(id);
  } else {
    oldTotalCount = itr->second.Quantity;
    oldWeekCount = itr->second.WeeklyQuantity;
    oldTrackedCount = itr->second.TrackedQuantity;
  }

  // count can't be more then weekCap if used (weekCap > 0)
  uint32 weekCap = GetCurrencyWeekCap(currency);
  if (!isRefund && weekCap && weekCap == oldWeekCount && count > 0)
    return;

  if (!isRefund && weekCap && count > int32(weekCap))
    count = weekCap;

  // count can't be more then totalCap if used (totalCap > 0)
  uint32 totalCap = GetCurrencyTotalCap(currency);

  // Patch 4.0.3a - Justice Points over the hard cap of 4000 will be converted
  // to 47 silver and 50 copper per point.
  uint32 surplousJusticePoints = 0;
  if (totalCap && count > int32(totalCap)) {
    if (id == CURRENCY_TYPE_JUSTICE_POINTS)
      surplousJusticePoints = (oldTotalCount + count - totalCap) / precision;
    count = totalCap;
  }

  if (surplousJusticePoints)
    ModifyMoney(surplousJusticePoints * JUSTICE_POINTS_CONVERSION_MONEY, false);

  int32 newTotalCount = int32(oldTotalCount) + count;
  if (newTotalCount < 0)
    newTotalCount = 0;

  int32 newWeekCount = int32(oldWeekCount) + (count > 0 ? count : 0);
  if (newWeekCount < 0)
    newWeekCount = 0;

  // if we get more than weekCap just set to limit
  if (!isRefund && weekCap && int32(weekCap) < newWeekCount) {
    newWeekCount = int32(weekCap);
    // weekCap - oldWeekCount always >= 0 as we set limit before!
    newTotalCount = oldTotalCount + (weekCap - oldWeekCount);
  }

  // if we get more then totalCap set to maximum;
  if (totalCap && int32(totalCap) < newTotalCount) {
    newTotalCount = int32(totalCap);
    newWeekCount = weekCap;
  }

  if (uint32(newTotalCount) != oldTotalCount) {
    if (currency->Flags & CURRENCY_FLAG_COUNT_SEASON_TOTAL && !isRefund)
      hasSeasonCount = true;

    if (itr->second.state != PLAYERCURRENCY_NEW)
      itr->second.state = PLAYERCURRENCY_CHANGED;

    itr->second.Quantity = newTotalCount;
    itr->second.WeeklyQuantity = isRefund ? oldWeekCount : newWeekCount;

    if (!isRefund && count > 0) {
      UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_CURRENCY, id, count);
      if (hasSeasonCount)
        oldTrackedCount += count;
      itr->second.TrackedQuantity = oldTrackedCount;
    }

    if (currency->CategoryID == CURRENCY_CATEGORY_META_CONQUEST) {
      // count was changed to week limit, now we can modify original points.
      ModifyCurrency(CURRENCY_TYPE_CONQUEST_POINTS, count, printLog);
      return;
    }

    if (itr->second.state == PLAYERCURRENCY_NEW &&
        HasSkill(SKILL_ARCHAEOLOGY) &&
        currency->CategoryID == CURRENCY_CATEGORY_ARCHAEOLOGY)
      _archaeology->ActivateBranch(
          sArchaeologyMgr->Currency2BranchId(currency->ID));

    WorldPacket packet(SMSG_UPDATE_CURRENCY, 12);

    packet.WriteBit(weekCap != 0);
    packet.WriteBit(hasSeasonCount); // hasSeasonCount
    packet.WriteBit(!printLog);      // print in log

    if (hasSeasonCount)
      packet << uint32(oldTrackedCount / precision);

    packet << uint32(newTotalCount / precision);
    packet << uint32(id);
    if (weekCap)
      packet << uint32((!isRefund ? newWeekCount : oldWeekCount) / precision);

    SendDirectMessage(&packet);
  }
}

void Player::SetCurrency(uint32 id, uint32 count,
                         bool /*printLog*/ /*= true*/) {
  PlayerCurrenciesMap::iterator itr = _currencyStorage.find(id);
  if (itr == _currencyStorage.end()) {
    PlayerCurrency cur;
    cur.state = PLAYERCURRENCY_NEW;
    cur.Quantity = count;
    cur.WeeklyQuantity = 0;
    cur.TrackedQuantity = 0;
    cur.Flags = 0;
    _currencyStorage[id] = cur;
  }
}

uint32 Player::GetCurrencyWeekCap(uint32 id, bool usePrecision) const {
  CurrencyTypesEntry const *entry = sCurrencyTypesStore.LookupEntry(id);
  if (!entry)
    return 0;

  uint32 precision =
      (usePrecision && entry->Flags & CURRENCY_FLAG_HIGH_PRECISION)
          ? CURRENCY_PRECISION
          : 1;

  return GetCurrencyWeekCap(entry) / precision;
}

void Player::ResetCurrencyWeekCap() {
  for (uint32 arenaSlot = 0; arenaSlot < MAX_ARENA_SLOT; arenaSlot++) {
    if (uint32 arenaTeamId = GetArenaTeamId(arenaSlot)) {
      ArenaTeam *arenaTeam = sArenaTeamMgr->GetArenaTeamById(arenaTeamId);
      arenaTeam
          ->FinishWeek(); // set played this week etc values to 0 in memory, too
      arenaTeam->SaveToDB();           // save changes
      arenaTeam->NotifyStatsChanged(); // notify the players of the changes
    }
  }

  for (PlayerCurrenciesMap::iterator itr = _currencyStorage.begin();
       itr != _currencyStorage.end(); ++itr) {
    itr->second.WeeklyQuantity = 0;
    itr->second.state = PLAYERCURRENCY_CHANGED;
  }

  WorldPacket data(SMSG_WEEKLY_RESET_CURRENCY, 0);
  SendDirectMessage(&data);
}

uint32 Player::GetCurrencyWeekCap(CurrencyTypesEntry const *currency) const {
  switch (currency->ID) {
    // original conquest not have week cap
  case CURRENCY_TYPE_CONQUEST_POINTS:
    return std::max(
        GetCurrencyWeekCap(CURRENCY_TYPE_CONQUEST_META_ARENA, false),
        GetCurrencyWeekCap(CURRENCY_TYPE_CONQUEST_META_RBG, false));
  case CURRENCY_TYPE_CONQUEST_META_ARENA:
    // should add precision mod = 100
    return Firelands::Currency::ConquestRatingCalculator(
               _maxPersonalArenaRate) *
           CURRENCY_PRECISION;
  case CURRENCY_TYPE_CONQUEST_META_RBG:
    // should add precision mod = 100
    return Firelands::Currency::BgConquestRatingCalculator(
               GetRBGPersonalRating()) *
           CURRENCY_PRECISION;
  }

  return currency->MaxEarnablePerWeek;
}

uint32 Player::GetCurrencyTotalCap(CurrencyTypesEntry const *currency) const {
  uint32 cap = currency->MaxQty;

  switch (currency->ID) {
  case CURRENCY_TYPE_HONOR_POINTS: {
    uint32 honorcap = sWorld->getIntConfig(CONFIG_CURRENCY_MAX_HONOR_POINTS);
    if (honorcap > 0)
      cap = honorcap;
    break;
  }
  case CURRENCY_TYPE_JUSTICE_POINTS: {
    uint32 justicecap =
        sWorld->getIntConfig(CONFIG_CURRENCY_MAX_JUSTICE_POINTS);
    if (justicecap > 0)
      cap = justicecap;
    break;
  }
  }

  return cap;
}

void Player::UpdateConquestCurrencyCap(uint32 currency) {
  uint32 currenciesToUpdate[2] = {currency, CURRENCY_TYPE_CONQUEST_POINTS};

  for (uint32 i = 0; i < 2; ++i) {
    CurrencyTypesEntry const *currencyEntry =
        sCurrencyTypesStore.LookupEntry(currenciesToUpdate[i]);
    if (!currencyEntry)
      continue;

    uint32 precision =
        (currencyEntry->Flags & CURRENCY_FLAG_HIGH_PRECISION) ? 100 : 1;
    uint32 cap = GetCurrencyWeekCap(currencyEntry);

    WorldPacket packet(SMSG_UPDATE_CURRENCY_WEEK_LIMIT, 8);
    packet << uint32(cap / precision);
    packet << uint32(currenciesToUpdate[i]);
    SendDirectMessage(&packet);
  }
}

void Player::SetInGuild(uint32 guildId) {
  if (guildId)
    SetGuidValue(OBJECT_FIELD_DATA, ObjectGuid(HighGuid::Guild, guildId));
  else
    SetGuidValue(OBJECT_FIELD_DATA, ObjectGuid::Empty);

  ApplyModFlag(PLAYER_FLAGS, PLAYER_FLAGS_GUILD_LEVEL_ENABLED,
               guildId != 0 &&
                   sWorld->getBoolConfig(CONFIG_GUILD_LEVELING_ENABLED));
  SetUInt16Value(OBJECT_FIELD_TYPE, 1, guildId != 0);
  sCharacterCache->UpdateCharacterGuildId(GetGUID(), guildId);
}

void Player::SetArenaTeamInfoField(uint8 slot, ArenaTeamInfoType type,
                                   uint32 value) {
  SetUInt32Value(
      PLAYER_FIELD_ARENA_TEAM_INFO_1_1 + (slot * ARENA_TEAM_END) + type, value);
  if (type == ARENA_TEAM_PERSONAL_RATING && value > _maxPersonalArenaRate) {
    _maxPersonalArenaRate = value;
    UpdateConquestCurrencyCap(CURRENCY_TYPE_CONQUEST_META_ARENA);
  }
}

void Player::SetInArenaTeam(uint32 ArenaTeamId, uint8 slot, uint8 type) {
  SetArenaTeamInfoField(slot, ARENA_TEAM_ID, ArenaTeamId);
  SetArenaTeamInfoField(slot, ARENA_TEAM_TYPE, type);
}

uint32 Player::GetZoneIdFromDB(ObjectGuid guid) {
  ObjectGuid::LowType guidLow = guid.GetCounter();
  CharacterDatabasePreparedStatement *stmt =
      CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHAR_ZONE);
  stmt->SetData(0, guidLow);
  PreparedQueryResult result = CharacterDatabase.Query(stmt);

  if (!result)
    return 0;
  Field *fields = result->Fetch();
  uint32 zone = fields[0].Get<uint16>();

  if (!zone) {
    // stored zone is zero, use generic and slow zone detection
    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHAR_POSITION_XYZ);
    stmt->SetData(0, guidLow);
    result = CharacterDatabase.Query(stmt);

    if (!result)
      return 0;
    fields = result->Fetch();
    uint32 map = fields[0].Get<uint16>();
    float posx = fields[1].Get<float>();
    float posy = fields[2].Get<float>();
    float posz = fields[3].Get<float>();

    if (!sMapStore.LookupEntry(map))
      return 0;

    zone = sMapMgr->GetZoneId(PhasingHandler::GetEmptyPhaseShift(), map, posx,
                              posy, posz);

    if (zone > 0) {
      stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_ZONE);

      stmt->SetData(0, uint16(zone));
      stmt->SetData(1, guidLow);

      CharacterDatabase.Execute(stmt);
    }
  }

  return zone;
}

void Player::UpdateArea(uint32 newArea) {
  // FFA_PVP flags are area and not zone id dependent
  // so apply them accordingly
  m_areaUpdateId = newArea;

  AreaTableEntry const *area = sAreaTableStore.LookupEntry(newArea);
  bool oldFFAPvPArea = pvpInfo.IsInFFAPvPArea;
  pvpInfo.IsInFFAPvPArea = area && (area->Flags & AREA_FLAG_ARENA);
  UpdatePvPState(true);

  // check if we were in ffa arena and we left
  if (oldFFAPvPArea && !pvpInfo.IsInFFAPvPArea)
    ValidateAttackersAndOwnTarget();

  PhasingHandler::OnAreaChange(this);
  UpdateAreaDependentAuras(newArea);

  // previously this was in UpdateZone (but after UpdateArea) so nothing will
  // break
  pvpInfo.IsInNoPvPArea = false;
  if (area && area->IsSanctuary()) // in sanctuary
  {
    SetByteFlag(UNIT_FIELD_BYTES_2, UNIT_BYTES_2_OFFSET_PVP_FLAG,
                UNIT_BYTE2_FLAG_SANCTUARY);
    pvpInfo.IsInNoPvPArea = true;
    if (!duel)
      CombatStopWithPets();
  } else
    RemoveByteFlag(UNIT_FIELD_BYTES_2, UNIT_BYTES_2_OFFSET_PVP_FLAG,
                   UNIT_BYTE2_FLAG_SANCTUARY);

  uint32 const areaRestFlag = (GetTeam() == ALLIANCE)
                                  ? AREA_FLAG_REST_ZONE_ALLIANCE
                                  : AREA_FLAG_REST_ZONE_HORDE;
  if (area && area->Flags & areaRestFlag)
    SetRestFlag(REST_FLAG_IN_FACTION_AREA);
  else
    RemoveRestFlag(REST_FLAG_IN_FACTION_AREA);

  UpdateMountCapability();
}

void Player::UpdateZone(uint32 newZone, uint32 newArea) {
  if (!IsInWorld())
    return;

  uint32 const oldZone = m_zoneUpdateId;
  m_zoneUpdateId = newZone;
  m_zoneUpdateTimer = ZONE_UPDATE_INTERVAL;

  GetMap()->UpdatePlayerZoneStats(oldZone, newZone);

  // call leave script hooks immedately (before updating flags)
  if (oldZone != newZone) {
    sOutdoorPvPMgr->HandlePlayerLeaveZone(this, m_zoneUpdateId);
    sBattlefieldMgr->HandlePlayerLeaveZone(this, m_zoneUpdateId);
  }

  // group update
  if (GetGroup()) {
    SetGroupUpdateFlag(GROUP_UPDATE_FULL);
    if (GetPet())
      SetGroupUpdateFlag(GROUP_UPDATE_PET);
  }

  // zone changed, so area changed as well, update it
  UpdateArea(newArea);

  AreaTableEntry const *zone = sAreaTableStore.LookupEntry(newZone);
  if (!zone)
    return;

  if (sWorld->getBoolConfig(CONFIG_WEATHER))
    GetMap()->GetOrGenerateZoneDefaultWeather(newZone);
  GetMap()->SendZoneDynamicInfo(newZone, this);

  UpdateHostileAreaState(zone);

  if (zone->Flags & AREA_FLAG_CAPITAL) // Is in a capital city
  {
    if (!pvpInfo.IsHostile || zone->IsSanctuary())
      SetRestFlag(REST_FLAG_IN_CITY);

    pvpInfo.IsInNoPvPArea = true;
  } else
    RemoveRestFlag(REST_FLAG_IN_CITY); // Recently left a capital city

  UpdatePvPState();

  // remove items with area/map limitations (delete only for alive player to
  // allow back in ghost mode) if player resurrected at teleport this will be
  // applied in resurrect code
  if (IsAlive())
    DestroyZoneLimitedItem(true, newZone);

  // check some item equip limitations (in result lost CanTitanGrip at talent
  // reset, for example)
  AutoUnequipOffhandIfNeed();

  // recent client version not send leave/join channel packets for built-in
  // local channels
  UpdateLocalChannels(newZone);

  if (oldZone != newZone)
    UpdateZoneDependentAuras(newZone);

  // call enter script hooks after everyting else has processed
  sScriptMgr->OnPlayerUpdateZone(this, newZone, newArea);
  if (oldZone != newZone) {
    sOutdoorPvPMgr->HandlePlayerEnterZone(this, newZone);
    sBattlefieldMgr->HandlePlayerEnterZone(this, newZone);
    SendInitWorldStates(newZone,
                        newArea); // only if really enters to new zone, not just
                                  // area change, works strange...
    if (Guild *guild = GetGuild())
      guild->UpdateMemberData(this, GUILD_MEMBER_DATA_ZONEID, newZone);
  }
}

void Player::UpdateHostileAreaState(AreaTableEntry const *area) {
  pvpInfo.IsInHostileArea = false;

  if (area->IsSanctuary()) // sanctuary and arena cannot be overriden
    pvpInfo.IsInHostileArea = false;
  else if (area->Flags & AREA_FLAG_ARENA)
    pvpInfo.IsInHostileArea = true;
  else {
    if (area) {
      if (InBattleground() || area->Flags & AREA_FLAG_COMBAT ||
          (area->PvpCombatWorldStateID != -1 &&
           sWorld->getWorldState(area->PvpCombatWorldStateID) != 0))
        pvpInfo.IsInHostileArea = true;
      else if (sWorld->IsPvPRealm() || (area->Flags & AREA_FLAG_UNK3)) {
        if (area->Flags & AREA_FLAG_CONTESTED_AREA)
          pvpInfo.IsInHostileArea = sWorld->IsPvPRealm();
        else {
          FactionTemplateEntry const *factionTemplate =
              GetFactionTemplateEntry();
          if (!factionTemplate ||
              factionTemplate->FriendGroup & area->FactionGroupMask)
            pvpInfo.IsInHostileArea = false;
          else if (factionTemplate->EnemyGroup & area->FactionGroupMask)
            pvpInfo.IsInHostileArea = true;
          else
            pvpInfo.IsInHostileArea = sWorld->IsPvPRealm();
        }
      }
    }
  }

  // Treat players having a quest flagging for PvP as always in hostile area
  pvpInfo.IsHostile = pvpInfo.IsInHostileArea || HasPvPForcingQuest();
}

// If players are too far away from the duel flag... they lose the duel
void Player::CheckDuelDistance(time_t currTime) {
  if (!duel)
    return;

  ObjectGuid duelFlagGUID = GetGuidValue(PLAYER_DUEL_ARBITER);
  GameObject *obj = GetMap()->GetGameObject(duelFlagGUID);
  if (!obj)
    return;

  if (duel->outOfBound == 0) {
    if (!IsWithinDistInMap(obj, 50)) {
      duel->outOfBound = currTime;

      WorldPacket data(SMSG_DUEL_OUTOFBOUNDS, 0);
      SendDirectMessage(&data);
    }
  } else {
    if (IsWithinDistInMap(obj, 40)) {
      duel->outOfBound = 0;

      WorldPacket data(SMSG_DUEL_INBOUNDS, 0);
      SendDirectMessage(&data);
    } else if (currTime >= (duel->outOfBound + 10))
      DuelComplete(DUEL_FLED);
  }
}

bool Player::IsOutdoorPvPActive() const {
  return IsAlive() && !HasInvisibilityAura() && !HasStealthAura() && IsPvP() &&
         !HasUnitMovementFlag(MOVEMENTFLAG_FLYING) && !IsInFlight();
}

void Player::DuelComplete(DuelCompleteType type) {
  // duel not requested
  if (!duel)
    return;

  // Check if DuelComplete() has been called already up in the stack and in that
  // case don't do anything else here
  if (duel->isCompleted || ASSERT_NOTNULL(duel->opponent->duel)->isCompleted)
    return;

  duel->isCompleted = true;
  duel->opponent->duel->isCompleted = true;

  LOG_DEBUG("entities.unit",
            "Player::DuelComplete: Player '%s' (%s), Opponent: '%s' (%s)",
            GetName().c_str(), GetGUID().ToString().c_str(),
            duel->opponent->GetName().c_str(),
            duel->opponent->GetGUID().ToString().c_str());

  WorldPacket data(SMSG_DUEL_COMPLETE, (1));
  data << (uint8)((type != DUEL_INTERRUPTED) ? 1 : 0);
  SendDirectMessage(&data);

  if (duel->opponent->GetSession())
    duel->opponent->SendDirectMessage(&data);

  if (type != DUEL_INTERRUPTED) {
    data.Initialize(SMSG_DUEL_WINNER, (1 + 20)); // we guess size
    data << uint8(type == DUEL_WON ? 0 : 1);     // 0 = just won; 1 = fled
    data << duel->opponent->GetName();
    data << GetName();
    SendMessageToSet(&data, true);
  }

  sScriptMgr->OnPlayerDuelEnd(duel->opponent, this, type);

  switch (type) {
  case DUEL_FLED:
    // if initiator and opponent are on the same team
    // or initiator and opponent are not PvP enabled, forcibly stop attacking
    if (duel->initiator->GetTeam() == duel->opponent->GetTeam()) {
      duel->initiator->AttackStop();
      duel->opponent->AttackStop();
    } else {
      if (!duel->initiator->IsPvP())
        duel->initiator->AttackStop();
      if (!duel->opponent->IsPvP())
        duel->opponent->AttackStop();
    }
    break;
  case DUEL_WON:
    UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_LOSE_DUEL, 1);
    duel->opponent->UpdateAchievementCriteria(
        ACHIEVEMENT_CRITERIA_TYPE_WIN_DUEL, 1);

    // Credit for quest Death's Challenge
    if (getClass() == CLASS_DEATH_KNIGHT &&
        duel->opponent->GetQuestStatus(12733) == QUEST_STATUS_INCOMPLETE)
      duel->opponent->CastSpell(duel->opponent, 52994, true);

    // Honor points after duel (the winner) - ImpConfig
    if (uint32 amount = sWorld->getIntConfig(CONFIG_HONOR_AFTER_DUEL))
      duel->opponent->RewardHonor(nullptr, 1, amount);

    break;
  default:
    break;
  }

  // Victory emote spell
  if (type != DUEL_INTERRUPTED)
    duel->opponent->CastSpell(duel->opponent, 52852, true);

  // Remove Duel Flag object
  GameObject *obj = GetMap()->GetGameObject(GetGuidValue(PLAYER_DUEL_ARBITER));
  if (obj)
    duel->initiator->RemoveGameObject(obj, true);

  /* remove auras */
  AuraApplicationMap &itsAuras = duel->opponent->GetAppliedAuras();
  for (AuraApplicationMap::iterator i = itsAuras.begin();
       i != itsAuras.end();) {
    Aura const *aura = i->second->GetBase();
    if (!i->second->IsPositive() && aura->GetCasterGUID() == GetGUID() &&
        aura->GetApplyTime() >= duel->startTime)
      duel->opponent->RemoveAura(i);
    else
      ++i;
  }

  AuraApplicationMap &myAuras = GetAppliedAuras();
  for (AuraApplicationMap::iterator i = myAuras.begin(); i != myAuras.end();) {
    Aura const *aura = i->second->GetBase();
    if (!i->second->IsPositive() &&
        aura->GetCasterGUID() == duel->opponent->GetGUID() &&
        aura->GetApplyTime() >= duel->startTime)
      RemoveAura(i);
    else
      ++i;
  }

  // cleanup combo points
  if (GetComboTarget() == duel->opponent->GetGUID())
    ClearComboPoints();
  else if (GetComboTarget() == duel->opponent->GetPetGUID())
    ClearComboPoints();

  if (duel->opponent->GetComboTarget() == GetGUID())
    duel->opponent->ClearComboPoints();
  else if (duel->opponent->GetComboTarget() == GetPetGUID())
    duel->opponent->ClearComboPoints();

  // cleanups
  SetGuidValue(PLAYER_DUEL_ARBITER, ObjectGuid::Empty);
  SetUInt32Value(PLAYER_DUEL_TEAM, 0);
  duel->opponent->SetGuidValue(PLAYER_DUEL_ARBITER, ObjectGuid::Empty);
  duel->opponent->SetUInt32Value(PLAYER_DUEL_TEAM, 0);

  delete duel->opponent->duel;
  duel->opponent->duel = nullptr;
  delete duel;
  duel = nullptr;
}

//---------------------------------------------------------//

void Player::_ApplyItemMods(Item *item, uint8 slot, bool apply,
                            bool updateItemAuras /*= true*/) {
  if (slot >= INVENTORY_SLOT_BAG_END || !item)
    return;

  ItemTemplate const *proto = item->GetTemplate();

  if (!proto)
    return;

  // not apply/remove mods for broken item
  if (item->IsBroken())
    return;

  LOG_DEBUG("entities.player.items",
            "Player::_ApplyItemMods: Applying mods for item %s",
            item->GetGUID().ToString().c_str());

  if (proto->GetSocketColor(
          0)) // only (un)equipping of items with sockets can influence
              // metagems, so no need to waste time with normal items
    CorrectMetaGemEnchants(slot, apply);

  _ApplyItemBonuses(proto, slot, apply);
  ApplyItemEquipSpell(item, apply);
  if (updateItemAuras) {
    ApplyItemDependentAuras(item, apply);

    WeaponAttackType const attackType = Player::GetAttackBySlot(slot);
    if (attackType != MAX_ATTACK)
      UpdateWeaponDependentAuras(attackType);
  }

  ApplyEnchantment(item, apply);

  LOG_DEBUG("entities.player.items", "Player::_ApplyItemMods: completed");
}

void Player::_ApplyItemBonuses(ItemTemplate const *proto, uint8 slot,
                               bool apply, bool only_level_scale /*= false*/) {
  if (slot >= INVENTORY_SLOT_BAG_END || !proto)
    return;

  ScalingStatDistributionEntry const *ssd =
      proto->GetScalingStatDistribution()
          ? sScalingStatDistributionStore.LookupEntry(
                proto->GetScalingStatDistribution())
          : nullptr;
  if (only_level_scale && !ssd)
    return;

  // req. check at equip, but allow use for extended range if range limit max
  // level, set proper level
  uint32 ssd_level = getLevel();
  if (ssd && ssd_level > ssd->Maxlevel)
    ssd_level = ssd->Maxlevel;

  ScalingStatValuesEntry const *ssv =
      ssd ? sScalingStatValuesStore.LookupEntry(ssd_level) : nullptr;
  if (only_level_scale && !ssv)
    return;

  for (uint8 i = 0; i < MAX_ITEM_PROTO_STATS; ++i) {
    int32 statID = proto->GetItemStatType(i);
    if ((statID < 0 && !ssd) || (statID < 0 && ssd && ssd->StatID[i] < 0))
      continue;

    if (statID < 0 && ssd)
      statID = ssd->StatID[i];

    int32 val = proto->GetStatValue(i, this);
    if (!val)
      continue;

    switch (statID) {
    case ITEM_MOD_MANA:
      HandleStatFlatModifier(UNIT_MOD_MANA, BASE_VALUE, float(val), apply);
      break;
    case ITEM_MOD_HEALTH: // modify HP
      HandleStatFlatModifier(UNIT_MOD_HEALTH, BASE_VALUE, float(val), apply);
      break;
    case ITEM_MOD_AGILITY: // modify agility
      HandleStatFlatModifier(UNIT_MOD_STAT_AGILITY, BASE_VALUE, float(val),
                             apply);
      UpdateStatBuffMod(STAT_AGILITY);
      break;
    case ITEM_MOD_STRENGTH: // modify strength
      HandleStatFlatModifier(UNIT_MOD_STAT_STRENGTH, BASE_VALUE, float(val),
                             apply);
      UpdateStatBuffMod(STAT_STRENGTH);
      break;
    case ITEM_MOD_INTELLECT: // modify intellect
      HandleStatFlatModifier(UNIT_MOD_STAT_INTELLECT, BASE_VALUE, float(val),
                             apply);
      UpdateStatBuffMod(STAT_INTELLECT);
      break;
    case ITEM_MOD_SPIRIT: // modify spirit
      HandleStatFlatModifier(UNIT_MOD_STAT_SPIRIT, BASE_VALUE, float(val),
                             apply);
      UpdateStatBuffMod(STAT_SPIRIT);
      break;
    case ITEM_MOD_STAMINA: // modify stamina
      HandleStatFlatModifier(UNIT_MOD_STAT_STAMINA, BASE_VALUE, float(val),
                             apply);
      UpdateStatBuffMod(STAT_STAMINA);
      break;
    case ITEM_MOD_DEFENSE_SKILL_RATING:
      ApplyRatingMod(CR_DEFENSE_SKILL, int32(val), apply);
      break;
    case ITEM_MOD_DODGE_RATING:
      ApplyRatingMod(CR_DODGE, int32(val), apply);
      break;
    case ITEM_MOD_PARRY_RATING:
      ApplyRatingMod(CR_PARRY, int32(val), apply);
      break;
    case ITEM_MOD_BLOCK_RATING:
      ApplyRatingMod(CR_BLOCK, int32(val), apply);
      break;
    case ITEM_MOD_HIT_MELEE_RATING:
      ApplyRatingMod(CR_HIT_MELEE, int32(val), apply);
      break;
    case ITEM_MOD_HIT_RANGED_RATING:
      ApplyRatingMod(CR_HIT_RANGED, int32(val), apply);
      break;
    case ITEM_MOD_HIT_SPELL_RATING:
      ApplyRatingMod(CR_HIT_SPELL, int32(val), apply);
      break;
    case ITEM_MOD_CRIT_MELEE_RATING:
      ApplyRatingMod(CR_CRIT_MELEE, int32(val), apply);
      break;
    case ITEM_MOD_CRIT_RANGED_RATING:
      ApplyRatingMod(CR_CRIT_RANGED, int32(val), apply);
      break;
    case ITEM_MOD_CRIT_SPELL_RATING:
      ApplyRatingMod(CR_CRIT_SPELL, int32(val), apply);
      break;
      // case ITEM_MOD_HIT_TAKEN_MELEE_RATING:
      //     ApplyRatingMod(CR_HIT_TAKEN_MELEE, int32(val), apply);
      //     break;
      // case ITEM_MOD_HIT_TAKEN_RANGED_RATING:
      //     ApplyRatingMod(CR_HIT_TAKEN_RANGED, int32(val), apply);
      //     break;
      // case ITEM_MOD_HIT_TAKEN_SPELL_RATING:
      //     ApplyRatingMod(CR_HIT_TAKEN_SPELL, int32(val), apply);
      //     break;
      // case ITEM_MOD_CRIT_TAKEN_MELEE_RATING:
      //     ApplyRatingMod(CR_CRIT_TAKEN_MELEE, int32(val), apply);
      //     break;
    case ITEM_MOD_CRIT_TAKEN_RANGED_RATING:
      ApplyRatingMod(CR_RESILIENCE_PLAYER_DAMAGE_TAKEN, int32(val), apply);
      break;
      // case ITEM_MOD_CRIT_TAKEN_SPELL_RATING:
      //     ApplyRatingMod(CR_CRIT_TAKEN_SPELL, int32(val), apply);
      //     break;
    case ITEM_MOD_HASTE_MELEE_RATING:
      ApplyRatingMod(CR_HASTE_MELEE, int32(val), apply);
      break;
    case ITEM_MOD_HASTE_RANGED_RATING:
      ApplyRatingMod(CR_HASTE_RANGED, int32(val), apply);
      break;
    case ITEM_MOD_HASTE_SPELL_RATING:
      ApplyRatingMod(CR_HASTE_SPELL, int32(val), apply);
      break;
    case ITEM_MOD_HIT_RATING:
      ApplyRatingMod(CR_HIT_MELEE, int32(val), apply);
      ApplyRatingMod(CR_HIT_RANGED, int32(val), apply);
      ApplyRatingMod(CR_HIT_SPELL, int32(val), apply);
      break;
    case ITEM_MOD_CRIT_RATING:
      ApplyRatingMod(CR_CRIT_MELEE, int32(val), apply);
      ApplyRatingMod(CR_CRIT_RANGED, int32(val), apply);
      ApplyRatingMod(CR_CRIT_SPELL, int32(val), apply);
      break;
      // case ITEM_MOD_HIT_TAKEN_RATING: // Unused since 3.3.5
      //     ApplyRatingMod(CR_HIT_TAKEN_MELEE, int32(val), apply);
      //     ApplyRatingMod(CR_HIT_TAKEN_RANGED, int32(val), apply);
      //     ApplyRatingMod(CR_HIT_TAKEN_SPELL, int32(val), apply);
      //     break;
      // case ITEM_MOD_CRIT_TAKEN_RATING: // Unused since 3.3.5
      //     ApplyRatingMod(CR_CRIT_TAKEN_MELEE, int32(val), apply);
      //     ApplyRatingMod(CR_CRIT_TAKEN_RANGED, int32(val), apply);
      //     ApplyRatingMod(CR_CRIT_TAKEN_SPELL, int32(val), apply);
      //     break;
    case ITEM_MOD_RESILIENCE_RATING:
      ApplyRatingMod(CR_RESILIENCE_PLAYER_DAMAGE_TAKEN, int32(val), apply);
      break;
    case ITEM_MOD_HASTE_RATING:
      ApplyRatingMod(CR_HASTE_MELEE, int32(val), apply);
      ApplyRatingMod(CR_HASTE_RANGED, int32(val), apply);
      ApplyRatingMod(CR_HASTE_SPELL, int32(val), apply);
      break;
    case ITEM_MOD_EXPERTISE_RATING:
      ApplyRatingMod(CR_EXPERTISE, int32(val), apply);
      break;
    case ITEM_MOD_ATTACK_POWER:
      HandleStatFlatModifier(UNIT_MOD_ATTACK_POWER, TOTAL_VALUE, float(val),
                             apply);
      HandleStatFlatModifier(UNIT_MOD_ATTACK_POWER_RANGED, TOTAL_VALUE,
                             float(val), apply);
      break;
    case ITEM_MOD_RANGED_ATTACK_POWER:
      HandleStatFlatModifier(UNIT_MOD_ATTACK_POWER_RANGED, TOTAL_VALUE,
                             float(val), apply);
      break;
    case ITEM_MOD_MANA_REGENERATION:
      ApplyManaRegenBonus(int32(val), apply);
      break;
      // case ITEM_MOD_ARMOR_PENETRATION_RATING:
      //     ApplyRatingMod(CR_ARMOR_PENETRATION, int32(val), apply);
      //     break;
    case ITEM_MOD_SPELL_POWER:
      ApplySpellPowerBonus(int32(val), apply);
      break;
    case ITEM_MOD_HEALTH_REGEN:
      ApplyHealthRegenBonus(int32(val), apply);
      break;
    case ITEM_MOD_SPELL_PENETRATION:
      ApplySpellPenetrationBonus(val, apply);
      break;
    case ITEM_MOD_MASTERY_RATING:
      ApplyRatingMod(CR_MASTERY, int32(val), apply);
      break;
    case ITEM_MOD_FIRE_RESISTANCE:
      HandleStatFlatModifier(UNIT_MOD_RESISTANCE_FIRE, BASE_VALUE, float(val),
                             apply);
      break;
    case ITEM_MOD_FROST_RESISTANCE:
      HandleStatFlatModifier(UNIT_MOD_RESISTANCE_FROST, BASE_VALUE, float(val),
                             apply);
      break;
    case ITEM_MOD_HOLY_RESISTANCE:
      HandleStatFlatModifier(UNIT_MOD_RESISTANCE_HOLY, BASE_VALUE, float(val),
                             apply);
      break;
    case ITEM_MOD_SHADOW_RESISTANCE:
      HandleStatFlatModifier(UNIT_MOD_RESISTANCE_SHADOW, BASE_VALUE, float(val),
                             apply);
      break;
    case ITEM_MOD_NATURE_RESISTANCE:
      HandleStatFlatModifier(UNIT_MOD_RESISTANCE_NATURE, BASE_VALUE, float(val),
                             apply);
      break;
    case ITEM_MOD_ARCANE_RESISTANCE:
      HandleStatFlatModifier(UNIT_MOD_RESISTANCE_ARCANE, BASE_VALUE, float(val),
                             apply);
      break;
    case ITEM_MOD_EXTRA_ARMOR:
      HandleStatFlatModifier(UNIT_MOD_ARMOR, BASE_VALUE, float(val), apply);
      break;
    }
  }

  // Apply Spell Power from ScalingStatValue if set
  if (ssv && proto->GetFlags2() & ITEM_FLAG2_CASTER_WEAPON)
    if (int32 spellbonus = int32(ssv->Spellpower))
      ApplySpellPowerBonus(spellbonus, apply);

  // If set ScalingStatValue armor get it or use item armor
  uint32 armor = proto->GetEffectiveArmor(this);
  if (ssv && proto->GetClass() == ITEM_CLASS_ARMOR)
    armor = ssv->GetArmor(proto->GetInventoryType(), proto->GetSubClass() - 1);

  if (armor) {
    UnitModifierFlatType modType = TOTAL_VALUE;
    if (proto->GetClass() == ITEM_CLASS_ARMOR) {
      switch (proto->GetSubClass()) {
      case ITEM_SUBCLASS_ARMOR_CLOTH:
      case ITEM_SUBCLASS_ARMOR_LEATHER:
      case ITEM_SUBCLASS_ARMOR_MAIL:
      case ITEM_SUBCLASS_ARMOR_PLATE:
      case ITEM_SUBCLASS_ARMOR_SHIELD:
        modType = BASE_VALUE;
        break;
      }
    }
    HandleStatFlatModifier(UNIT_MOD_ARMOR, modType, float(armor), apply);
  }

  // Add armor bonus from ArmorDamageModifier if > 0
  if (proto->GetArmorDamageModifier() > 0)
    HandleStatFlatModifier(UNIT_MOD_ARMOR, TOTAL_VALUE,
                           float(proto->GetArmorDamageModifier()), apply);

  WeaponAttackType attType = BASE_ATTACK;

  if (slot == EQUIPMENT_SLOT_RANGED &&
      (proto->GetInventoryType() == INVTYPE_RANGED ||
       proto->GetInventoryType() == INVTYPE_THROWN ||
       proto->GetInventoryType() == INVTYPE_RANGEDRIGHT)) {
    attType = RANGED_ATTACK;
  } else if (slot == EQUIPMENT_SLOT_OFFHAND) {
    attType = OFF_ATTACK;
  }

  if (CanUseAttackType(attType))
    _ApplyWeaponDamage(slot, proto, ssv, apply);
}

void Player::_ApplyWeaponDamage(uint8 slot, ItemTemplate const *proto,
                                ScalingStatValuesEntry const *ssv, bool apply) {
  WeaponAttackType attType = BASE_ATTACK;
  float damage = 0.0f;

  if (slot == EQUIPMENT_SLOT_RANGED &&
      (proto->GetInventoryType() == INVTYPE_RANGED ||
       proto->GetInventoryType() == INVTYPE_THROWN ||
       proto->GetInventoryType() == INVTYPE_RANGEDRIGHT)) {
    attType = RANGED_ATTACK;
  } else if (slot == EQUIPMENT_SLOT_OFFHAND) {
    attType = OFF_ATTACK;
  }

  float minDamage, maxDamage, dps;
  proto->GetWeaponDamage(this, minDamage, maxDamage, dps);

  // If set dpsMod in ScalingStatValue use it for min (70% from average), max
  // (130% from average) damage
  int32 extraDPS = 0;
  if (ssv) {
    float damageMultiplier = 0.0f;
    extraDPS = ssv->GetDPSAndDamageMultiplier(
        proto->GetSubClass(),
        (proto->GetFlags2() & ITEM_FLAG2_CASTER_WEAPON) != 0,
        &damageMultiplier);
    if (extraDPS) {
      float average = extraDPS * proto->GetDelay() / 1000.0f;
      minDamage = (1.0f - damageMultiplier) * average;
      maxDamage = (1.0f + damageMultiplier) * average;
    }
  }

  if (minDamage > 0) {
    damage = apply ? minDamage : BASE_MINDAMAGE;
    SetBaseWeaponDamage(attType, MINDAMAGE, damage);
  }

  if (maxDamage > 0) {
    damage = apply ? maxDamage : BASE_MAXDAMAGE;
    SetBaseWeaponDamage(attType, MAXDAMAGE, damage);
  }

  if (proto->GetDelay() && !IsInFeralForm()) {
    if (slot == EQUIPMENT_SLOT_RANGED)
      SetAttackTime(RANGED_ATTACK,
                    apply ? proto->GetDelay() : BASE_ATTACK_TIME);
    else if (slot == EQUIPMENT_SLOT_MAINHAND)
      SetAttackTime(BASE_ATTACK, apply ? proto->GetDelay() : BASE_ATTACK_TIME);
    else if (slot == EQUIPMENT_SLOT_OFFHAND)
      SetAttackTime(OFF_ATTACK, apply ? proto->GetDelay() : BASE_ATTACK_TIME);
  }

  if (CanModifyStats() && (damage || proto->GetDelay()))
    UpdateDamagePhysical(attType);
}

void Player::CastAllObtainSpells() {
  for (uint8 slot = INVENTORY_SLOT_ITEM_START; slot < INVENTORY_SLOT_ITEM_END;
       ++slot)
    if (Item *item = GetItemByPos(INVENTORY_SLOT_BAG_0, slot))
      ApplyItemObtainSpells(item, true);

  for (uint8 i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; ++i) {
    Bag *bag = GetBagByPos(i);
    if (!bag)
      continue;

    for (uint32 slot = 0; slot < bag->GetBagSize(); ++slot)
      if (Item *item = bag->GetItemByPos(slot))
        ApplyItemObtainSpells(item, true);
  }
}

void Player::ApplyItemObtainSpells(Item *item, bool apply) {
  ItemTemplate const *itemTemplate = item->GetTemplate();

  for (ItemEffect const &effect : itemTemplate->Effects) {
    if (effect.Trigger !=
        ITEM_SPELLTRIGGER_ON_NO_DELAY_USE) // On obtain trigger
      continue;

    int32 spellId = effect.SpellID;
    if (!spellId)
      continue;

    if (apply) {
      if (!HasAura(spellId))
        CastSpell(this, spellId, item);
    } else
      RemoveAurasDueToSpell(spellId);
  }
}

// this one rechecks weapon auras and stores them in BaseModGroup container
// needed for things like axe specialization applying only to axe weapons in
// case of dual-wield
void Player::UpdateWeaponDependentCritAuras(WeaponAttackType attackType) {
  BaseModGroup modGroup;
  switch (attackType) {
  case BASE_ATTACK:
    modGroup = CRIT_PERCENTAGE;
    break;
  case OFF_ATTACK:
    modGroup = OFFHAND_CRIT_PERCENTAGE;
    break;
  case RANGED_ATTACK:
    modGroup = RANGED_CRIT_PERCENTAGE;
    break;
  default:
    ABORT();
    break;
  }

  float amount = 0.0f;
  amount +=
      GetTotalAuraModifier(SPELL_AURA_MOD_WEAPON_CRIT_PERCENT,
                           std::bind(&Unit::CheckAttackFitToAuraRequirement,
                                     this, attackType, std::placeholders::_1));

  // these auras don't have item requirement (only Combat Expertise in 3.3.5a)
  amount += GetTotalAuraModifier(SPELL_AURA_MOD_CRIT_PCT);

  SetBaseModFlatValue(modGroup, amount);
}

void Player::UpdateAllWeaponDependentCritAuras() {
  for (uint8 i = BASE_ATTACK; i < MAX_ATTACK; ++i)
    UpdateWeaponDependentCritAuras(WeaponAttackType(i));
}

void Player::UpdateWeaponDependentAuras(WeaponAttackType attackType) {
  UpdateWeaponDependentCritAuras(attackType);
  UpdateDamageDoneMods(attackType);
  UpdateDamagePctDoneMods(attackType);
}

void Player::ApplyItemDependentAuras(Item *item, bool apply) {
  if (apply) {
    PlayerSpellMap const &spells = GetSpellMap();
    for (auto itr = spells.begin(); itr != spells.end(); ++itr) {
      if (itr->second.state == PLAYERSPELL_REMOVED || itr->second.disabled)
        continue;

      SpellInfo const *spellInfo = sSpellMgr->GetSpellInfo(itr->first);
      if (!spellInfo || !spellInfo->IsPassive() ||
          spellInfo->EquippedItemClass < 0)
        continue;

      if (!HasAura(itr->first) && HasItemFitToSpellRequirements(spellInfo))
        AddAura(itr->first, this); // no SMSG_SPELL_GO in sniff found
    }
  } else
    RemoveItemDependentAurasAndCasts(item);
}

bool Player::CheckAttackFitToAuraRequirement(WeaponAttackType attackType,
                                             AuraEffect const *aurEff) const {
  SpellInfo const *spellInfo = aurEff->GetSpellInfo();
  if (spellInfo->EquippedItemClass == -1)
    return true;

  Item *item = GetWeaponForAttack(attackType, true);
  if (!item || !item->IsFitToSpellRequirements(spellInfo))
    return false;

  return true;
}

void Player::ApplyItemEquipSpell(Item *item, bool apply, bool form_change) {
  if (!item)
    return;

  ItemTemplate const *proto = item->GetTemplate();
  if (!proto)
    return;

  for (ItemEffect const &effect : proto->Effects) {
    // no spell
    if (effect.SpellID <= 0)
      continue;

    // wrong triggering type
    if (apply && effect.Trigger != ITEM_SPELLTRIGGER_ON_EQUIP)
      continue;

    // check if it is valid spell
    SpellInfo const *spellproto = sSpellMgr->GetSpellInfo(effect.SpellID);
    if (!spellproto)
      continue;

    ApplyEquipSpell(spellproto, item, apply, form_change);
  }
}

void Player::ApplyEquipSpell(SpellInfo const *spellInfo, Item *item, bool apply,
                             bool form_change) {
  if (apply) {
    // Cannot be used in this stance/form
    if (spellInfo->CheckShapeshift(GetShapeshiftForm()) != SPELL_CAST_OK)
      return;

    if (form_change) // check aura active state from other form
    {
      AuraApplicationMapBounds range =
          GetAppliedAuras().equal_range(spellInfo->Id);
      for (AuraApplicationMap::const_iterator itr = range.first;
           itr != range.second; ++itr)
        if (!item ||
            itr->second->GetBase()->GetCastItemGUID() == item->GetGUID())
          return;
    }

    LOG_DEBUG("entities.player",
              "Player::ApplyEquipSpell: Player '%s' (%s) cast %s equip spell "
              "(ID: %i)",
              GetName().c_str(), GetGUID().ToString().c_str(),
              (item ? "item" : "itemset"), spellInfo->Id);

    CastSpell(this, spellInfo->Id, item);
  } else {
    if (form_change) // check aura compatibility
    {
      // Cannot be used in this stance/form
      if (spellInfo->CheckShapeshift(GetShapeshiftForm()) == SPELL_CAST_OK)
        return; // and remove only not compatible at form change
    }

    if (item)
      RemoveAurasDueToItemSpell(
          spellInfo->Id,
          item->GetGUID()); // un-apply all spells, not only at-equipped
    else
      RemoveAurasDueToSpell(spellInfo->Id); // un-apply spell (item set case)
  }
}

void Player::UpdateEquipSpellsAtFormChange() {
  for (uint8 i = 0; i < INVENTORY_SLOT_BAG_END; ++i) {
    if (m_items[i] && !m_items[i]->IsBroken() &&
        CanUseAttackType(GetAttackBySlot(i))) {
      ApplyItemEquipSpell(m_items[i], false,
                          true); // remove spells that not fit to form
      ApplyItemEquipSpell(m_items[i], true,
                          true); // add spells that fit form but not active
    }
  }

  // item set bonuses not dependent from item broken state
  for (size_t setindex = 0; setindex < ItemSetEff.size(); ++setindex) {
    ItemSetEffect *eff = ItemSetEff[setindex];
    if (!eff)
      continue;

    for (uint32 y = 0; y < MAX_ITEM_SET_SPELLS; ++y) {
      SpellInfo const *spellInfo = eff->spells[y];
      if (!spellInfo)
        continue;

      ApplyEquipSpell(spellInfo, nullptr, false,
                      true); // remove spells that not fit to form
      ApplyEquipSpell(spellInfo, nullptr, true,
                      true); // add spells that fit form but not active
    }
  }
}

void Player::CastItemCombatSpell(DamageInfo const &damageInfo) {
  Unit *target = damageInfo.GetVictim();
  if (!target || !target->IsAlive() || target == this)
    return;

  for (uint8 i = EQUIPMENT_SLOT_START; i < EQUIPMENT_SLOT_END; ++i) {
    // If usable, try to cast item spell
    if (Item *item = GetItemByPos(INVENTORY_SLOT_BAG_0, i)) {
      if (!item->IsBroken() && CanUseAttackType(damageInfo.GetAttackType())) {
        if (ItemTemplate const *proto = item->GetTemplate()) {
          // Additional check for weapons
          if (proto->GetClass() == ITEM_CLASS_WEAPON) {
            // offhand item cannot proc from main hand hit etc
            EquipmentSlots slot;
            switch (damageInfo.GetAttackType()) {
            case BASE_ATTACK:
              slot = EQUIPMENT_SLOT_MAINHAND;
              break;
            case OFF_ATTACK:
              slot = EQUIPMENT_SLOT_OFFHAND;
              break;
            case RANGED_ATTACK:
              slot = EQUIPMENT_SLOT_RANGED;
              break;
            default:
              slot = EQUIPMENT_SLOT_END;
              break;
            }
            if (slot != i)
              continue;
            // Check if item is useable (forms or disarm)
            if (damageInfo.GetAttackType() == BASE_ATTACK)
              if (!IsUseEquipedWeapon(true) && !IsInFeralForm())
                continue;
          }

          CastItemCombatSpell(damageInfo, item, proto);
        }
      }
    }
  }
}

void Player::CastItemCombatSpell(DamageInfo const &damageInfo, Item *item,
                                 ItemTemplate const *proto) {
  // Can do effect if any damage done to target
  // for done procs allow normal + critical + absorbs by default
  bool canTrigger = [&]() {
    if (!(damageInfo.GetHitMask() &
          (PROC_HIT_NORMAL | PROC_HIT_CRITICAL | PROC_HIT_ABSORB)))
      return false;

    if (damageInfo.GetSpellInfo() && damageInfo.GetSpellInfo()->HasAttribute(
                                         SPELL_ATTR3_CANT_TRIGGER_CASTER_PROCS))
      return false;

    return true;
  }();

  if (canTrigger) {
    for (ItemEffect const &effect : proto->Effects) {
      // no spell
      if (effect.SpellID <= 0)
        continue;

      // wrong triggering type
      if (effect.Trigger != ITEM_SPELLTRIGGER_CHANCE_ON_HIT)
        continue;

      SpellInfo const *spellInfo = sSpellMgr->GetSpellInfo(effect.SpellID);
      if (!spellInfo) {
        LOG_ERROR("entities.player.items",
                  "Player::CastItemCombatSpell: Player '%s' (%s) cast unknown "
                  "item spell (ID: %i)",
                  GetName().c_str(), GetGUID().ToString().c_str(),
                  effect.SpellID);
        continue;
      }

      // not allow proc extra attack spell at extra attack
      if (m_extraAttacks &&
          spellInfo->HasEffect(SPELL_EFFECT_ADD_EXTRA_ATTACKS))
        return;

      float chance = static_cast<float>(spellInfo->ProcChance);

      if (proto->SpellPPMRate) {
        uint32 WeaponSpeed = GetAttackTime(damageInfo.GetAttackType());
        chance = GetPPMProcChance(WeaponSpeed, proto->SpellPPMRate, spellInfo);
      } else if (chance > 100.0f)
        chance = GetWeaponProcChance();

      if (roll_chance_f(chance) &&
          sScriptMgr->OnCastItemCombatSpell(this, damageInfo.GetVictim(),
                                            spellInfo, item))
        CastSpell(damageInfo.GetVictim(), spellInfo->Id, item);
    }
  }

  // item combat enchantments
  for (uint8 e_slot = 0; e_slot < MAX_ENCHANTMENT_SLOT; ++e_slot) {
    if (e_slot > PRISMATIC_ENCHANTMENT_SLOT &&
        e_slot < PROP_ENCHANTMENT_SLOT_0) // not holding enchantment id
      continue;

    uint32 enchant_id = item->GetEnchantmentId(EnchantmentSlot(e_slot));
    SpellItemEnchantmentEntry const *pEnchant =
        sSpellItemEnchantmentStore.LookupEntry(enchant_id);
    if (!pEnchant)
      continue;

    for (uint8 s = 0; s < MAX_ITEM_ENCHANTMENT_EFFECTS; ++s) {
      if (pEnchant->Effect[s] != ITEM_ENCHANTMENT_TYPE_COMBAT_SPELL)
        continue;

      SpellEnchantProcEntry const *entry =
          sSpellMgr->GetSpellEnchantProcEvent(enchant_id);
      if (entry && entry->HitMask) {
        // Check hit/crit/dodge/parry requirement
        if ((entry->HitMask & damageInfo.GetHitMask()) == 0)
          continue;
      } else {
        // Can do effect if any damage done to target
        // for done procs allow normal + critical + absorbs by default
        if (!canTrigger)
          continue;
      }

      // check if enchant procs only on white hits
      if (entry && (entry->AttributesMask & ENCHANT_PROC_ATTR_WHITE_HIT) &&
          damageInfo.GetSpellInfo())
        continue;

      SpellInfo const *spellInfo =
          sSpellMgr->GetSpellInfo(pEnchant->EffectArg[s]);
      if (!spellInfo) {
        LOG_ERROR("entities.player.items",
                  "Player::CastItemCombatSpell: Player '%s' (%s) cast unknown "
                  "spell (EnchantID: %u, SpellID: %i), ignoring",
                  GetName().c_str(), GetGUID().ToString().c_str(), pEnchant->ID,
                  pEnchant->EffectArg[s]);
        continue;
      }

      float chance = pEnchant->EffectPointsMin[s] != 0
                         ? float(pEnchant->EffectPointsMin[s])
                         : GetWeaponProcChance();
      if (entry) {
        if (entry->ProcsPerMinute != 0.f)
          chance = GetPPMProcChance(proto->GetDelay(), entry->ProcsPerMinute,
                                    spellInfo);
        else if (entry->Chance != 0.f)
          chance = entry->Chance;
      }

      // Apply spell mods
      ApplySpellMod(pEnchant->EffectArg[s], SPELLMOD_CHANCE_OF_SUCCESS, chance);

      // Shiv has 100% chance to apply the poison
      if (FindCurrentSpellBySpellId(5938) && e_slot == TEMP_ENCHANTMENT_SLOT)
        chance = 100.0f;

      if (roll_chance_f(chance)) {
        Unit *target = spellInfo->IsPositive() ? this : damageInfo.GetVictim();

        // reduce effect values if enchant is limited
        CastSpellExtraArgs args(item);
        if (entry && (entry->AttributesMask & ENCHANT_PROC_ATTR_LIMIT_60) &&
            target->getLevel() > 60) {
          int32 const lvlDifference = target->getLevel() - 60;
          int32 const lvlPenaltyFactor = 4; // 4% lost effectiveness per level

          int32 const effectPct =
              std::max(0, 100 - (lvlDifference * lvlPenaltyFactor));

          for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i) {
            if (spellInfo->Effects[i].IsEffect())
              args.AddSpellMod(
                  static_cast<SpellValueMod>(SPELLVALUE_BASE_POINT0 + i),
                  CalculatePct(spellInfo->Effects[i].CalcValue(this),
                               effectPct));
          }
        }

        CastSpell(target, spellInfo->Id, args);
      }
    }
  }
}

void Player::CastItemUseSpell(Item *item, SpellCastTargets const &targets,
                              uint8 castId, uint32 glyphIndex) {
  ItemTemplate const *proto = item->GetTemplate();
  // special learning case
  if (proto->Effects[0].SpellID == 483 || proto->Effects[0].SpellID == 55884) {
    uint32 learn_spell_id = proto->Effects[0].SpellID;
    uint32 learning_spell_id = proto->Effects[1].SpellID;

    SpellInfo const *spellInfo = sSpellMgr->GetSpellInfo(learn_spell_id);
    if (!spellInfo) {
      LOG_ERROR("entities.player",
                "Player::CastItemUseSpell: Item (Entry: %u) has wrong spell id "
                "%u, ignoring.",
                proto->GetId(), learn_spell_id);
      SendEquipError(EQUIP_ERR_INTERNAL_BAG_ERROR, item, nullptr);
      return;
    }

    Spell *spell = new Spell(this, spellInfo, TRIGGERED_NONE);
    spell->m_CastItem = item;
    spell->m_cast_count = castId;
    spell->SetSpellValue(SPELLVALUE_BASE_POINT0, learning_spell_id);
    spell->prepare(targets);
    return;
  }

  for (ItemEffect const &effect : proto->Effects) {
    // no spell
    if (effect.SpellID <= 0)
      continue;

    // wrong triggering type
    if (effect.Trigger != ITEM_SPELLTRIGGER_ON_USE)
      continue;

    SpellInfo const *spellInfo = sSpellMgr->GetSpellInfo(effect.SpellID);
    if (!spellInfo) {
      LOG_ERROR("entities.player",
                "Player::CastItemUseSpell: Item (Entry: %u) has wrong spell id "
                "%u, ignoring",
                proto->GetId(), effect.SpellID);
      continue;
    }

    Spell *spell = new Spell(this, spellInfo, TRIGGERED_NONE);
    spell->m_CastItem = item;
    spell->m_cast_count = castId;
    spell->m_glyphIndex = glyphIndex;
    spell->prepare(targets);
    return;
  }

  // Item enchantments spells cast at use
  for (uint8 e_slot = 0; e_slot < MAX_ENCHANTMENT_SLOT; ++e_slot) {
    if (e_slot > PRISMATIC_ENCHANTMENT_SLOT &&
        e_slot < PROP_ENCHANTMENT_SLOT_0) // not holding enchantment id
      continue;

    uint32 enchant_id = item->GetEnchantmentId(EnchantmentSlot(e_slot));
    SpellItemEnchantmentEntry const *pEnchant =
        sSpellItemEnchantmentStore.LookupEntry(enchant_id);
    if (!pEnchant)
      continue;
    for (uint8 s = 0; s < MAX_ITEM_ENCHANTMENT_EFFECTS; ++s) {
      if (pEnchant->Effect[s] != ITEM_ENCHANTMENT_TYPE_USE_SPELL)
        continue;

      SpellInfo const *spellInfo =
          sSpellMgr->GetSpellInfo(pEnchant->EffectArg[s]);
      if (!spellInfo) {
        LOG_ERROR("entities.player",
                  "Player::CastItemUseSpell: Enchant %i, cast unknown spell %i",
                  pEnchant->ID, pEnchant->EffectArg[s]);
        continue;
      }

      Spell *spell = new Spell(this, spellInfo, TRIGGERED_NONE);
      spell->m_CastItem = item;
      spell->m_cast_count = castId;     // set count of casts
      spell->m_glyphIndex = glyphIndex; // glyph index
      spell->prepare(targets);
      return;
    }
  }
}

void Player::_RemoveAllItemMods() {
  LOG_DEBUG("entities.player.items", "_RemoveAllItemMods start.");

  for (uint8 i = 0; i < INVENTORY_SLOT_BAG_END; ++i) {
    if (m_items[i]) {
      ItemTemplate const *proto = m_items[i]->GetTemplate();
      if (!proto)
        continue;

      // item set bonuses not dependent from item broken state
      if (proto->GetItemSet())
        RemoveItemsSetItem(this, proto);

      if (m_items[i]->IsBroken() || !CanUseAttackType(GetAttackBySlot(i)))
        continue;

      ApplyItemEquipSpell(m_items[i], false);
      ApplyEnchantment(m_items[i], false);
    }
  }

  for (uint8 i = 0; i < INVENTORY_SLOT_BAG_END; ++i) {
    if (m_items[i]) {
      if (m_items[i]->IsBroken() || !CanUseAttackType(GetAttackBySlot(i)))
        continue;
      ItemTemplate const *proto = m_items[i]->GetTemplate();
      if (!proto)
        continue;

      ApplyItemDependentAuras(m_items[i], false);
      _ApplyItemBonuses(proto, i, false);
    }
  }

  LOG_DEBUG("entities.player.items", "_RemoveAllItemMods complete.");
}

void Player::_ApplyAllItemMods() {
  LOG_DEBUG("entities.player.items", "_ApplyAllItemMods start.");

  for (uint8 i = 0; i < INVENTORY_SLOT_BAG_END; ++i) {
    if (m_items[i]) {
      if (m_items[i]->IsBroken() || !CanUseAttackType(GetAttackBySlot(i)))
        continue;

      ItemTemplate const *proto = m_items[i]->GetTemplate();
      if (!proto)
        continue;

      ApplyItemDependentAuras(m_items[i], true);
      _ApplyItemBonuses(proto, i, true);

      WeaponAttackType const attackType = Player::GetAttackBySlot(i);
      if (attackType != MAX_ATTACK)
        UpdateWeaponDependentAuras(attackType);
    }
  }

  for (uint8 i = 0; i < INVENTORY_SLOT_BAG_END; ++i) {
    if (m_items[i]) {
      ItemTemplate const *proto = m_items[i]->GetTemplate();
      if (!proto)
        continue;

      // item set bonuses not dependent from item broken state
      if (proto->GetItemSet())
        AddItemsSetItem(this, m_items[i]);

      if (m_items[i]->IsBroken() || !CanUseAttackType(GetAttackBySlot(i)))
        continue;

      ApplyItemEquipSpell(m_items[i], true);
      ApplyEnchantment(m_items[i], true);
    }
  }

  LOG_DEBUG("entities.player.items", "_ApplyAllItemMods complete.");
}

void Player::_ApplyAllLevelScaleItemMods(bool apply) {
  for (uint8 i = 0; i < INVENTORY_SLOT_BAG_END; ++i) {
    if (m_items[i]) {
      if (m_items[i]->IsBroken() || !CanUseAttackType(GetAttackBySlot(i)))
        continue;

      ItemTemplate const *proto = m_items[i]->GetTemplate();
      if (!proto)
        continue;

      _ApplyItemBonuses(proto, i, apply, true);
    }
  }
}

/*  If in a battleground a player dies, and an enemy removes the insignia, the
   player's bones is lootable Called by remove insignia spell effect    */
void Player::RemovedInsignia(Player *looterPlr) {
  // If player is not in battleground and not in wintergrasp
  if (!GetBattlegroundId() && GetZoneId() != AREA_WINTERGRASP &&
      GetZoneId() != BATTLEFIELD_TB_ZONEID)
    return;

  // If not released spirit, do it !
  if (m_deathTimer > 0) {
    m_deathTimer = 0;
    BuildPlayerRepop();
    RepopAtGraveyard();
  }

  _corpseLocation.WorldRelocate();

  // We have to convert player corpse to bones, not to be able to resurrect
  // there SpawnCorpseBones isn't handy, 'cos it saves player while he in BG
  Corpse *bones = GetMap()->ConvertCorpseToBones(GetGUID(), true);
  if (!bones)
    return;

  // Now we must make bones lootable, and send player loot
  bones->SetFlag(CORPSE_FIELD_DYNAMIC_FLAGS, CORPSE_DYNFLAG_LOOTABLE);

  // We store the level of our player in the gold field
  // We retrieve this information at Player::SendLoot()
  bones->loot.gold = getLevel();
  bones->lootRecipient = looterPlr;
  looterPlr->SendLoot(bones->GetGUID(), LOOT_INSIGNIA);
}

void Player::SendLootRelease(ObjectGuid guid) const {
  WorldPackets::Loot::LootReleaseResponse packet;
  packet.LootObj = guid;
  SendDirectMessage(packet.Write());
}

void Player::SendLoot(ObjectGuid guid, LootType loot_type) {
  if (ObjectGuid lguid = GetLootGUID())
    m_session->DoLootRelease(lguid);

  Loot *loot;
  PermissionTypes permission = ALL_PERMISSION;

  LOG_DEBUG("loot", "Player::SendLoot: Player: '%s' (%s), Loot: %s",
            GetName().c_str(), GetGUID().ToString().c_str(),
            guid.ToString().c_str());
  if (guid.IsGameObject()) {
    GameObject *go = GetMap()->GetGameObject(guid);
    auto shouldLootRelease = [this](GameObject *go, LootType lootType) -> bool {
      // not check distance for GO in case owned GO (fishing bobber case, for
      // example) And permit out of range GO with no owner in case fishing hole
      if (!go)
        return true;

      if (lootType == LOOT_SKINNING) {
        // Disarm Trap
        if (!go->IsWithinDistInMap(this, 20.f))
          return true;
      } else {
        if (lootType != LOOT_FISHINGHOLE &&
            ((lootType != LOOT_FISHING && lootType != LOOT_FISHING_JUNK) ||
             go->GetOwnerGUID() != GetGUID()) &&
            !go->IsWithinDistInMap(this))
          return true;

        if (lootType == LOOT_CORPSE && go->GetRespawnTime() &&
            go->isSpawnedByDefault())
          return true;
      }

      return false;
    };

    if (shouldLootRelease(go, loot_type)) {
      SendLootRelease(guid);
      return;
    }

    loot = &go->loot;

    // loot was generated and respawntime has passed since then, allow to
    // recreate loot to avoid bugs, this rule covers spawned gameobjects only
    // Don't allow to regenerate chest loot inside instances and raids, to avoid
    // exploits with duplicate boss loot being given for some encounters
    if (go->isSpawnedByDefault() && go->getLootState() == GO_ACTIVATED &&
        !go->loot.isLooted() && !go->GetMap()->Instanceable() &&
        go->GetLootGenerationTime() + go->GetRespawnDelay() <
            GameTime::GetGameTime())
      go->SetLootState(GO_READY);

    if (go->getLootState() == GO_READY) {
      uint32 lootid = go->GetGOInfo()->GetLootId();
      if (Battleground *bg = GetBattleground())
        if (!bg->CanActivateGO(go->GetEntry(), GetTeam())) {
          SendLootRelease(guid);
          return;
        }

      if (lootid) {
        loot->clear();

        Group *group = GetGroup();
        bool groupRules =
            (group && go->GetGOInfo()->type == GAMEOBJECT_TYPE_CHEST &&
             go->GetGOInfo()->chest.groupLootRules);

        // check current RR player and get next if necessary
        if (groupRules)
          group->UpdateLooterGuid(go, true);

        loot->FillLoot(lootid, LootTemplates_Gameobject, this, !groupRules,
                       false, go->GetLootMode());
        go->SetLootGenerationTime();

        // get next RR player (for next loot)
        if (groupRules && !go->loot.empty())
          group->UpdateLooterGuid(go);
      }

      if (go->GetLootMode() > 0)
        if (GameObjectTemplateAddon const *addon = go->GetTemplateAddon())
          loot->generateMoneyLoot(addon->mingold, addon->maxgold);

      if (loot_type == LOOT_FISHING)
        go->getFishLoot(loot, this);
      else if (loot_type == LOOT_FISHING_JUNK)
        go->getFishLootJunk(loot, this);

      if (go->GetGOInfo()->type == GAMEOBJECT_TYPE_CHEST &&
          go->GetGOInfo()->chest.groupLootRules) {
        if (Group *group = GetGroup()) {
          switch (group->GetLootMethod()) {
          case GROUP_LOOT:
            // GroupLoot: rolls items over threshold. Items with quality <
            // threshold, round robin
            group->GroupLoot(loot, go);
            break;
          case NEED_BEFORE_GREED:
            group->NeedBeforeGreed(loot, go);
            break;
          case MASTER_LOOT:
            group->MasterLoot(loot, go);
            break;
          default:
            break;
          }
        }
      }

      go->SetLootState(GO_ACTIVATED, this);
    }

    if (go->getLootState() == GO_ACTIVATED) {
      if (Group *group = GetGroup()) {
        switch (group->GetLootMethod()) {
        case MASTER_LOOT:
          permission = group->GetMasterLooterGuid() == GetGUID()
                           ? MASTER_PERMISSION
                           : RESTRICTED_PERMISSION;
          break;
        case FREE_FOR_ALL:
          permission = ALL_PERMISSION;
          break;
        case ROUND_ROBIN:
          permission = ROUND_ROBIN_PERMISSION;
          break;
        default:
          permission = GROUP_PERMISSION;
          break;
        }
      } else
        permission = ALL_PERMISSION;
    }
  } else if (guid.IsItem()) {
    Item *item = GetItemByGuid(guid);

    if (!item) {
      SendLootRelease(guid);
      return;
    }

    permission = OWNER_PERMISSION;

    loot = &item->loot;

    // Store container id
    loot->containerID = item->GetGUID().GetCounter();

    // If item doesn't already have loot, attempt to load it. If that
    // fails then this is first time opening, generate loot
    if (!item->m_lootGenerated &&
        !sLootItemStorage->LoadStoredLoot(item, this)) {
      item->m_lootGenerated = true;
      loot->clear();

      switch (loot_type) {
      case LOOT_DISENCHANTING:
        loot->FillLoot(item->GetTemplate()->DisenchantID,
                       LootTemplates_Disenchant, this, true);
        break;
      case LOOT_PROSPECTING:
        loot->FillLoot(item->GetEntry(), LootTemplates_Prospecting, this, true);
        break;
      case LOOT_MILLING:
        loot->FillLoot(item->GetEntry(), LootTemplates_Milling, this, true);
        break;
      default:
        loot->generateMoneyLoot(item->GetTemplate()->MinMoneyLoot,
                                item->GetTemplate()->MaxMoneyLoot);
        loot->FillLoot(item->GetEntry(), LootTemplates_Item, this, true,
                       loot->gold != 0);

        // Force save the loot and money items that were just rolled
        //  Also saves the container item ID in Loot struct (not to DB)
        if (loot->gold > 0 || loot->unlootedCount > 0)
          sLootItemStorage->AddNewStoredLoot(loot, this);

        break;
      }
    }
  } else if (guid.IsCorpse()) // remove insignia
  {
    Corpse *bones = ObjectAccessor::GetCorpse(*this, guid);

    if (!bones || !(loot_type == LOOT_CORPSE || loot_type == LOOT_INSIGNIA) ||
        bones->GetType() != CORPSE_BONES) {
      SendLootRelease(guid);
      return;
    }

    loot = &bones->loot;

    if (loot->loot_type == LOOT_NONE) {
      uint32 pLevel = bones->loot.gold;
      bones->loot.clear();

      // For AV Achievement
      if (Battleground *bg = GetBattleground()) {
        if (bg->GetTypeID(true) == BATTLEGROUND_AV)
          loot->FillLoot(PLAYER_CORPSE_LOOT_ENTRY, LootTemplates_Creature, this,
                         true);
      }
      // For wintergrasp Quests
      else if (GetZoneId() == AREA_WINTERGRASP)
        loot->FillLoot(PLAYER_CORPSE_LOOT_ENTRY, LootTemplates_Creature, this,
                       true);
      // For Tol Barad Quests
      else if (GetZoneId() == BATTLEFIELD_TB_ZONEID)
        loot->FillLoot(PLAYER_CORPSE_LOOT_ENTRY, LootTemplates_Creature, this,
                       true);

      // It may need a better formula
      // Now it works like this: lvl10: ~6copper, lvl70: ~9silver
      bones->loot.gold = uint32(urand(50, 150) * 0.016f *
                                std::pow(float(pLevel) / 5.76f, 2.5f) *
                                sWorld->getRate(RATE_DROP_MONEY));
    }

    if (bones->lootRecipient != this)
      permission = NONE_PERMISSION;
    else
      permission = OWNER_PERMISSION;
  } else {
    Creature *creature = GetMap()->GetCreature(guid);

    // must be in range and creature must be alive for pickpocket and must be
    // dead for another loot
    if (!creature || creature->IsAlive() != (loot_type == LOOT_PICKPOCKETING) ||
        !creature->IsWithinDistInMap(this, INTERACTION_DISTANCE)) {
      SendLootRelease(guid);
      return;
    }

    if (loot_type == LOOT_PICKPOCKETING && IsFriendlyTo(creature)) {
      SendLootRelease(guid);
      return;
    }

    loot = &creature->loot;

    if (loot_type == LOOT_PICKPOCKETING) {
      if (loot->loot_type != LOOT_PICKPOCKETING) {
        if (creature->CanGeneratePickPocketLoot()) {
          creature->StartPickPocketRefillTimer();
          loot->clear();

          if (uint32 lootid = creature->GetCreatureTemplate()->pickpocketLootId)
            loot->FillLoot(lootid, LootTemplates_Pickpocketing, this, true);

          // Generate extra money for pick pocket loot
          const uint32 a = urand(0, creature->getLevel() / 2);
          const uint32 b = urand(0, getLevel() / 2);
          loot->gold = uint32(10 * (a + b) * sWorld->getRate(RATE_DROP_MONEY));
          permission = OWNER_PERMISSION;
        } else {
          permission = NONE_PERMISSION;
          SendLootError(guid, LOOT_ERROR_ALREADY_PICKPOCKETED);
          return;
        }
      } // else - still has pickpocket loot generated & not fully taken
    } else {
      // exploit fix
      if (!creature->HasFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE)) {
        SendLootError(guid, LOOT_ERROR_DIDNT_KILL);
        return;
      }

      // the player whose group may loot the corpse
      Player *recipient = creature->GetLootRecipient();
      Group *recipientGroup = creature->GetLootRecipientGroup();
      if (!recipient && !recipientGroup) {
        SendLootError(guid, LOOT_ERROR_DIDNT_KILL);
        return;
      }

      if (loot->loot_type == LOOT_NONE) {
        // for creature, loot is filled when creature is killed.
        if (Group *group = creature->GetLootRecipientGroup()) {
          switch (group->GetLootMethod()) {
          case GROUP_LOOT:
            // GroupLoot: rolls items over threshold. Items with quality <
            // threshold, round robin
            group->GroupLoot(loot, creature);
            break;
          case NEED_BEFORE_GREED:
            group->NeedBeforeGreed(loot, creature);
            break;
          case MASTER_LOOT:
            group->MasterLoot(loot, creature);
            break;
          default:
            break;
          }
        }
      }

      // if loot is already skinning loot then don't do anything else
      if (loot->loot_type == LOOT_SKINNING) {
        loot_type = LOOT_SKINNING;
        permission = creature->GetLootRecipientGUID() == GetGUID()
                         ? OWNER_PERMISSION
                         : NONE_PERMISSION;
      } else if (loot_type == LOOT_SKINNING) {
        loot->clear();
        loot->FillLoot(creature->GetCreatureTemplate()->SkinLootId,
                       LootTemplates_Skinning, this, true);
        permission = OWNER_PERMISSION;

        // Set new loot recipient
        creature->SetLootRecipient(this, false);
      }
      // set group rights only for loot_type != LOOT_SKINNING
      else {
        if (creature->GetLootRecipientGroup()) {
          Group *group = GetGroup();
          if (group == creature->GetLootRecipientGroup()) {
            switch (group->GetLootMethod()) {
            case MASTER_LOOT:
              permission = group->GetMasterLooterGuid() == GetGUID()
                               ? MASTER_PERMISSION
                               : RESTRICTED_PERMISSION;
              break;
            case FREE_FOR_ALL:
              permission = ALL_PERMISSION;
              break;
            case ROUND_ROBIN:
              permission = ROUND_ROBIN_PERMISSION;
              break;
            default:
              permission = GROUP_PERMISSION;
              break;
            }
          } else
            permission = NONE_PERMISSION;
        } else if (creature->GetLootRecipient() == this)
          permission = OWNER_PERMISSION;
        else
          permission = NONE_PERMISSION;
      }
    }
  }

  // LOOT_INSIGNIA and LOOT_FISHINGHOLE unsupported by client
  switch (loot_type) {
  case LOOT_INSIGNIA:
    loot_type = LOOT_SKINNING;
    break;
  case LOOT_FISHINGHOLE:
    loot_type = LOOT_FISHING;
    break;
  case LOOT_FISHING_JUNK:
    loot_type = LOOT_FISHING;
    break;
  default:
    break;
  }

  // need know merged fishing/corpse loot type for achievements
  loot->loot_type = loot_type;

  if (permission != NONE_PERMISSION) {
    SetLootGUID(guid);

    WorldPackets::Loot::LootResponse packet;
    packet.Owner = guid;
    packet.AcquireReason = loot_type;
    loot->BuildLootResponse(packet, this, permission);
    SendDirectMessage(packet.Write());

    // add 'this' player as one of the players that are looting 'loot'
    loot->AddLooter(GetGUID());

    if (loot_type == LOOT_CORPSE && !guid.IsItem())
      SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_LOOTING);
  } else
    SendLootError(GetLootGUID(), LOOT_ERROR_DIDNT_KILL);
}

void Player::SendLootError(ObjectGuid guid, LootError error) const {
  WorldPackets::Loot::LootResponse lootResponse;
  lootResponse.Owner = guid;
  lootResponse.AcquireReason = LOOT_NONE;
  lootResponse.FailureReason = error;
  SendDirectMessage(lootResponse.Write());
}

void Player::SendNotifyLootMoneyRemoved() const {
  SendDirectMessage(WorldPackets::Loot::CoinRemoved().Write());
}

void Player::SendNotifyLootItemRemoved(uint8 lootSlot) const {
  WorldPackets::Loot::LootRemoved packet;
  packet.LootListID = lootSlot;
  SendDirectMessage(packet.Write());
}

void Player::SendNotifyCurrencyLootRemoved(uint8 lootSlot) {
  WorldPacket data(SMSG_CURRENCY_LOOT_REMOVED, 1);
  data << uint8(lootSlot);
  SendDirectMessage(&data);
}

void Player::SendUpdateWorldState(uint32 variable, uint32 value,
                                  bool hidden /*= false*/) const {
  WorldPackets::WorldState::UpdateWorldState worldstate;
  worldstate.VariableID = variable;
  worldstate.Value = value;
  worldstate.Hidden = hidden;
  SendDirectMessage(worldstate.Write());
}

void Player::SendInitWorldStates(uint32 zoneId, uint32 areaId) {
  // data depends on zoneid/mapid...
  Battleground *bg = GetBattleground();
  uint32 mapid = GetMapId();
  OutdoorPvP *pvp = sOutdoorPvPMgr->GetOutdoorPvPToZoneId(zoneId);
  InstanceScript *instance = GetInstanceScript();
  Battlefield *bf = sBattlefieldMgr->GetBattlefieldToZoneId(zoneId);

  LOG_DEBUG("network", "Sending SMSG_INIT_WORLD_STATES to Map: %u, Zone: %u",
            mapid, zoneId);

  WorldPackets::WorldState::InitWorldStates packet;
  packet.MapID = mapid;
  packet.AreaID = zoneId;
  packet.SubareaID = areaId;

  packet.Worldstates.emplace_back(2264, 0); // 1
  packet.Worldstates.emplace_back(2263, 0); // 2
  packet.Worldstates.emplace_back(2262, 0); // 3
  packet.Worldstates.emplace_back(2261, 0); // 4
  packet.Worldstates.emplace_back(2260, 0); // 5
  packet.Worldstates.emplace_back(2259, 0); // 6

  packet.Worldstates.emplace_back(
      3191, int32(sWorld->getBoolConfig(CONFIG_ARENA_SEASON_IN_PROGRESS)
                      ? sWorld->getIntConfig(CONFIG_ARENA_SEASON_ID)
                      : 0)); // 7 Current Season - Arena season in progress
  // 0 - End of season
  packet.Worldstates.emplace_back(
      3901, int32(sWorld->getIntConfig(CONFIG_ARENA_SEASON_ID) -
                  sWorld->getBoolConfig(
                      CONFIG_ARENA_SEASON_IN_PROGRESS))); // 8 PreviousSeason

  if (mapid == 530) // Outland
  {
    packet.Worldstates.emplace_back(2495, 0);   // 7
    packet.Worldstates.emplace_back(2493, 0xF); // 8
    packet.Worldstates.emplace_back(2491, 0xF); // 9
  }

  // insert <field> <value>
  switch (zoneId) {
  case 1:    // Dun Morogh
  case 11:   // Wetlands
  case 12:   // Elwynn Forest
  case 38:   // Loch Modan
  case 40:   // Westfall
  case 51:   // Searing Gorge
  case 1519: // Stormwind City
  case 1537: // Ironforge
  case 2257: // Deeprun Tram
  case 3703: // Shattrath City});
    break;
  case 1377: // Silithus
    if (pvp && pvp->GetTypeId() == OUTDOOR_PVP_SI)
      pvp->FillInitialWorldStates(packet);
    else {
      // states are always shown
      packet.Worldstates.emplace_back(2313, 0x0); // 7 ally silityst gathered
      packet.Worldstates.emplace_back(2314, 0x0); // 8 horde silityst gathered
      packet.Worldstates.emplace_back(2317, 0x0); // 9 max silithyst
    }
    // dunno about these... aq opening event maybe?
    packet.Worldstates.emplace_back(2322, 0x0); // 10 sandworm N
    packet.Worldstates.emplace_back(2323, 0x0); // 11 sandworm S
    packet.Worldstates.emplace_back(2324, 0x0); // 12 sandworm SW
    packet.Worldstates.emplace_back(2325, 0x0); // 13 sandworm E
    break;
  case 2597: // Alterac Valley
    if (bg && bg->GetTypeID(true) == BATTLEGROUND_AV)
      bg->FillInitialWorldStates(packet);
    else {
      packet.Worldstates.emplace_back(0x7ae, 0x1); // 7 snowfall n
      packet.Worldstates.emplace_back(0x532, 0x1); // 8 frostwolfhut hc
      packet.Worldstates.emplace_back(0x531, 0x0); // 9 frostwolfhut ac
      packet.Worldstates.emplace_back(0x52e, 0x0); // 10 stormpike firstaid a_a
      packet.Worldstates.emplace_back(
          0x571, 0x0); // 11 east frostwolf tower horde assaulted -unused
      packet.Worldstates.emplace_back(
          0x570, 0x0); // 12 west frostwolf tower horde assaulted - unused
      packet.Worldstates.emplace_back(0x567, 0x1); // 13 frostwolfe c
      packet.Worldstates.emplace_back(0x566, 0x1); // 14 frostwolfw c
      packet.Worldstates.emplace_back(0x550, 0x1); // 15 irondeep (N) ally
      packet.Worldstates.emplace_back(0x544, 0x0); // 16 ice grave a_a
      packet.Worldstates.emplace_back(0x536, 0x0); // 17 stormpike grave h_c
      packet.Worldstates.emplace_back(0x535, 0x1); // 18 stormpike grave a_c
      packet.Worldstates.emplace_back(0x518, 0x0); // 19 stoneheart grave a_a
      packet.Worldstates.emplace_back(0x517, 0x0); // 20 stoneheart grave h_a
      packet.Worldstates.emplace_back(0x574, 0x0); // 21 1396 unk
      packet.Worldstates.emplace_back(
          0x573, 0x0); // 22 iceblood tower horde assaulted -unused
      packet.Worldstates.emplace_back(
          0x572, 0x0); // 23 towerpoint horde assaulted - unused
      packet.Worldstates.emplace_back(0x56f, 0x0); // 24 1391 unk
      packet.Worldstates.emplace_back(0x56e, 0x0); // 25 iceblood a
      packet.Worldstates.emplace_back(0x56d, 0x0); // 26 towerp a
      packet.Worldstates.emplace_back(0x56c, 0x0); // 27 frostwolfe a
      packet.Worldstates.emplace_back(0x56b, 0x0); // 28 froswolfw a
      packet.Worldstates.emplace_back(0x56a, 0x1); // 29 1386 unk
      packet.Worldstates.emplace_back(0x569, 0x1); // 30 iceblood c
      packet.Worldstates.emplace_back(0x568, 0x1); // 31 towerp c
      packet.Worldstates.emplace_back(0x565, 0x0); // 32 stoneh tower a
      packet.Worldstates.emplace_back(0x564, 0x0); // 33 icewing tower a
      packet.Worldstates.emplace_back(0x563, 0x0); // 34 dunn a
      packet.Worldstates.emplace_back(0x562, 0x0); // 35 duns a
      packet.Worldstates.emplace_back(
          0x561, 0x0); // 36 stoneheart bunker alliance assaulted - unused
      packet.Worldstates.emplace_back(
          0x560, 0x0); // 37 icewing bunker alliance assaulted - unused
      packet.Worldstates.emplace_back(
          0x55f, 0x0); // 38 dunbaldar south alliance assaulted - unused
      packet.Worldstates.emplace_back(
          0x55e, 0x0); // 39 dunbaldar north alliance assaulted - unused
      packet.Worldstates.emplace_back(0x55d, 0x0); // 40 stone tower d
      packet.Worldstates.emplace_back(0x3c6, 0x0); // 41 966 unk
      packet.Worldstates.emplace_back(0x3c4, 0x0); // 42 964 unk
      packet.Worldstates.emplace_back(0x3c2, 0x0); // 43 962 unk
      packet.Worldstates.emplace_back(0x516, 0x1); // 44 stoneheart grave a_c
      packet.Worldstates.emplace_back(0x515, 0x0); // 45 stonheart grave h_c
      packet.Worldstates.emplace_back(0x3b6, 0x0); // 46 950 unk
      packet.Worldstates.emplace_back(0x55c, 0x0); // 47 icewing tower d
      packet.Worldstates.emplace_back(0x55b, 0x0); // 48 dunn d
      packet.Worldstates.emplace_back(0x55a, 0x0); // 49 duns d
      packet.Worldstates.emplace_back(0x559, 0x0); // 50 1369 unk
      packet.Worldstates.emplace_back(0x558, 0x0); // 51 iceblood d
      packet.Worldstates.emplace_back(0x557, 0x0); // 52 towerp d
      packet.Worldstates.emplace_back(0x556, 0x0); // 53 frostwolfe d
      packet.Worldstates.emplace_back(0x555, 0x0); // 54 frostwolfw d
      packet.Worldstates.emplace_back(0x554, 0x1); // 55 stoneh tower c
      packet.Worldstates.emplace_back(0x553, 0x1); // 56 icewing tower c
      packet.Worldstates.emplace_back(0x552, 0x1); // 57 dunn c
      packet.Worldstates.emplace_back(0x551, 0x1); // 58 duns c
      packet.Worldstates.emplace_back(0x54f, 0x0); // 59 irondeep (N) horde
      packet.Worldstates.emplace_back(0x54e, 0x0); // 60 irondeep (N) ally
      packet.Worldstates.emplace_back(0x54d, 0x1); // 61 mine (S) neutral
      packet.Worldstates.emplace_back(0x54c, 0x0); // 62 mine (S) horde
      packet.Worldstates.emplace_back(0x54b, 0x0); // 63 mine (S) ally
      packet.Worldstates.emplace_back(0x545, 0x0); // 64 iceblood h_a
      packet.Worldstates.emplace_back(0x543, 0x1); // 65 iceblod h_c
      packet.Worldstates.emplace_back(0x542, 0x0); // 66 iceblood a_c
      packet.Worldstates.emplace_back(0x540, 0x0); // 67 snowfall h_a
      packet.Worldstates.emplace_back(0x53f, 0x0); // 68 snowfall a_a
      packet.Worldstates.emplace_back(0x53e, 0x0); // 69 snowfall h_c
      packet.Worldstates.emplace_back(0x53d, 0x0); // 70 snowfall a_c
      packet.Worldstates.emplace_back(0x53c, 0x0); // 71 frostwolf g h_a
      packet.Worldstates.emplace_back(0x53b, 0x0); // 72 frostwolf g a_a
      packet.Worldstates.emplace_back(0x53a, 0x1); // 73 frostwolf g h_c
      packet.Worldstates.emplace_back(0x539, 0x0); // 74 frostwolf g a_c
      packet.Worldstates.emplace_back(0x538, 0x0); // 75 stormpike grave h_a
      packet.Worldstates.emplace_back(0x537, 0x0); // 76 stormpike grave a_a
      packet.Worldstates.emplace_back(0x534, 0x0); // 77 frostwolf hut h_a
      packet.Worldstates.emplace_back(0x533, 0x0); // 78 frostwolf hut a_a
      packet.Worldstates.emplace_back(0x530, 0x0); // 79 stormpike first aid h_a
      packet.Worldstates.emplace_back(0x52f, 0x0); // 80 stormpike first aid h_c
      packet.Worldstates.emplace_back(0x52d, 0x1); // 81 stormpike first aid a_c
    }
    break;
  case 3277: // Warsong Gulch
    if (bg && bg->GetTypeID(true) == BATTLEGROUND_WS)
      bg->FillInitialWorldStates(packet);
    else {
      packet.Worldstates.emplace_back(0x62d,
                                      0x0); // 7 1581 alliance flag captures
      packet.Worldstates.emplace_back(0x62e, 0x0); // 8 1582 horde flag captures
      packet.Worldstates.emplace_back(
          0x609, 0x0); // 9 1545 unk, set to 1 on alliance flag pickup...
      packet.Worldstates.emplace_back(0x60a,
                                      0x0); // 10 1546 unk, set to 1 on horde
                                            // flag pickup, after drop it's -1
      packet.Worldstates.emplace_back(0x60b, 0x2); // 11 1547 unk
      packet.Worldstates.emplace_back(0x641,
                                      0x3); // 12 1601 unk (max flag captures?)
      packet.Worldstates.emplace_back(
          0x922, 0x1); // 13 2338 horde (0 - hide, 1 - flag ok, 2 - flag picked
                       // up (flashing), 3 - flag picked up (not flashing)
      packet.Worldstates.emplace_back(
          0x923,
          0x1); // 14 2339 alliance (0 - hide, 1 - flag ok, 2 - flag picked up
                // (flashing), 3 - flag picked up (not flashing)
    }
    break;
  case 3358: // Arathi Basin
    if (bg && bg->GetTypeID(true) == BATTLEGROUND_AB)
      bg->FillInitialWorldStates(packet);
    else {
      packet.Worldstates.emplace_back(0x6e7, 0x0); // 7 1767 stables alliance
      packet.Worldstates.emplace_back(0x6e8, 0x0); // 8 1768 stables horde
      packet.Worldstates.emplace_back(0x6e9, 0x0); // 9 1769 unk, ST?
      packet.Worldstates.emplace_back(0x6ea,
                                      0x0); // 10 1770 stables (show/hide)
      packet.Worldstates.emplace_back(
          0x6ec,
          0x0); // 11 1772 farm (0 - horde controlled, 1 - alliance controlled)
      packet.Worldstates.emplace_back(0x6ed, 0x0); // 12 1773 farm (show/hide)
      packet.Worldstates.emplace_back(0x6ee, 0x0); // 13 1774 farm color
      packet.Worldstates.emplace_back(
          0x6ef, 0x0); // 14 1775 gold mine color, may be FM?
      packet.Worldstates.emplace_back(0x6f0, 0x0); // 15 1776 alliance resources
      packet.Worldstates.emplace_back(0x6f1, 0x0); // 16 1777 horde resources
      packet.Worldstates.emplace_back(0x6f2, 0x0); // 17 1778 horde bases
      packet.Worldstates.emplace_back(0x6f3, 0x0); // 18 1779 alliance bases
      packet.Worldstates.emplace_back(0x6f4,
                                      0x7d0); // 19 1780 max resources (2000)
      packet.Worldstates.emplace_back(0x6f6, 0x0); // 20 1782 blacksmith color
      packet.Worldstates.emplace_back(0x6f7,
                                      0x0); // 21 1783 blacksmith (show/hide)
      packet.Worldstates.emplace_back(0x6f8, 0x0); // 22 1784 unk, bs?
      packet.Worldstates.emplace_back(0x6f9, 0x0); // 23 1785 unk, bs?
      packet.Worldstates.emplace_back(
          0x6fb,
          0x0); // 24 1787 gold mine (0 - horde contr, 1 - alliance contr)
      packet.Worldstates.emplace_back(
          0x6fc, 0x0); // 25 1788 gold mine (0 - conflict, 1 - horde)
      packet.Worldstates.emplace_back(
          0x6fd, 0x0); // 26 1789 gold mine (1 - show/0 - hide)
      packet.Worldstates.emplace_back(0x6fe, 0x0); // 27 1790 gold mine color
      packet.Worldstates.emplace_back(
          0x700, 0x0); // 28 1792 gold mine color, wtf?, may be LM?
      packet.Worldstates.emplace_back(
          0x701,
          0x0); // 29 1793 lumber mill color (0 - conflict, 1 - horde contr)
      packet.Worldstates.emplace_back(0x702,
                                      0x0); // 30 1794 lumber mill (show/hide)
      packet.Worldstates.emplace_back(0x703,
                                      0x0); // 31 1795 lumber mill color color
      packet.Worldstates.emplace_back(
          0x732, 0x1); // 32 1842 stables (1 - uncontrolled)
      packet.Worldstates.emplace_back(
          0x733, 0x1); // 33 1843 gold mine (1 - uncontrolled)
      packet.Worldstates.emplace_back(
          0x734, 0x1); // 34 1844 lumber mill (1 - uncontrolled)
      packet.Worldstates.emplace_back(0x735,
                                      0x1); // 35 1845 farm (1 - uncontrolled)
      packet.Worldstates.emplace_back(
          0x736, 0x1); // 36 1846 blacksmith (1 - uncontrolled)
      packet.Worldstates.emplace_back(0x745, 0x2); // 37 1861 unk
      packet.Worldstates.emplace_back(0x7a3,
                                      0x708); // 38 1955 warning limit (1800)
    }
    break;
  case 3820: // Eye of the Storm
    if (bg && bg->GetTypeID(true) == BATTLEGROUND_EY)
      bg->FillInitialWorldStates(packet);
    else {
      packet.Worldstates.emplace_back(0xac1, 0x0); // 7  2753 Horde Bases
      packet.Worldstates.emplace_back(0xac0, 0x0); // 8  2752 Alliance Bases
      packet.Worldstates.emplace_back(
          0xab6, 0x0); // 9  2742 Mage Tower - Horde conflict
      packet.Worldstates.emplace_back(
          0xab5, 0x0); // 10 2741 Mage Tower - Alliance conflict
      packet.Worldstates.emplace_back(
          0xab4, 0x0); // 11 2740 Fel Reaver - Horde conflict
      packet.Worldstates.emplace_back(
          0xab3, 0x0); // 12 2739 Fel Reaver - Alliance conflict
      packet.Worldstates.emplace_back(
          0xab2, 0x0); // 13 2738 Draenei - Alliance conflict
      packet.Worldstates.emplace_back(0xab1,
                                      0x0); // 14 2737 Draenei - Horde conflict
      packet.Worldstates.emplace_back(0xab0, 0x0); // 15 2736 unk // 0 at start
      packet.Worldstates.emplace_back(0xaaf, 0x0); // 16 2735 unk // 0 at start
      packet.Worldstates.emplace_back(0xaad,
                                      0x0); // 17 2733 Draenei - Horde control
      packet.Worldstates.emplace_back(
          0xaac, 0x0); // 18 2732 Draenei - Alliance control
      packet.Worldstates.emplace_back(
          0xaab, 0x1); // 19 2731 Draenei uncontrolled (1 - yes, 0 - no)
      packet.Worldstates.emplace_back(
          0xaaa, 0x0); // 20 2730 Mage Tower - Alliance control
      packet.Worldstates.emplace_back(
          0xaa9, 0x0); // 21 2729 Mage Tower - Horde control
      packet.Worldstates.emplace_back(
          0xaa8, 0x1); // 22 2728 Mage Tower uncontrolled (1 - yes, 0 - no)
      packet.Worldstates.emplace_back(
          0xaa7, 0x0); // 23 2727 Fel Reaver - Horde control
      packet.Worldstates.emplace_back(
          0xaa6, 0x0); // 24 2726 Fel Reaver - Alliance control
      packet.Worldstates.emplace_back(
          0xaa5, 0x1); // 25 2725 Fel Reaver uncontrolled (1 - yes, 0 - no)
      packet.Worldstates.emplace_back(0xaa4,
                                      0x0); // 26 2724 Boold Elf - Horde control
      packet.Worldstates.emplace_back(
          0xaa3, 0x0); // 27 2723 Boold Elf - Alliance control
      packet.Worldstates.emplace_back(
          0xaa2, 0x1); // 28 2722 Boold Elf uncontrolled (1 - yes, 0 - no)
      packet.Worldstates.emplace_back(0xac5,
                                      0x1); // 29 2757 Flag (1 - show, 0 - hide)
                                            // - doesn't work exactly this way!
      packet.Worldstates.emplace_back(
          0xad2, 0x1); // 30 2770 Horde top-stats (1 - show, 0 - hide) // 02 ->
                       // horde picked up the flag
      packet.Worldstates.emplace_back(
          0xad1, 0x1); // 31 2769 Alliance top-stats (1 - show, 0 - hide) // 02
                       // -> alliance picked up the flag
      packet.Worldstates.emplace_back(0xabe, 0x0); // 32 2750 Horde resources
      packet.Worldstates.emplace_back(0xabd, 0x0); // 33 2749 Alliance resources
      packet.Worldstates.emplace_back(0xa05, 0x8e); // 34 2565 unk, constant?
      packet.Worldstates.emplace_back(
          0xaa0, 0x0); // 35 2720 Capturing progress-bar (100 -> empty (only
                       // grey), 0 -> blue|red (no grey), default 0)
      packet.Worldstates.emplace_back(
          0xa9f, 0x0); // 36 2719 Capturing progress-bar (0 - left, 100 - right)
      packet.Worldstates.emplace_back(
          0xa9e, 0x0); // 37 2718 Capturing progress-bar (1 - show, 0 - hide)
      packet.Worldstates.emplace_back(0xc0d, 0x17b); // 38 3085 unk
      // and some more ... unknown
    }
    break;
    // any of these needs change! the client remembers the prev setting!
    // ON EVERY ZONE LEAVE, RESET THE OLD ZONE'S WORLD STATE, BUT AT LEAST THE
    // UI STUFF!
  case 3483: // Hellfire Peninsula
    if (pvp && pvp->GetTypeId() == OUTDOOR_PVP_HP)
      pvp->FillInitialWorldStates(packet);
    else {
      packet.Worldstates.emplace_back(
          0x9ba, 0x1); // 10 // add ally tower main gui icon       // maybe
                       // should be sent only on login?
      packet.Worldstates.emplace_back(
          0x9b9, 0x1); // 11 // add horde tower main gui icon      // maybe
                       // should be sent only on login?
      packet.Worldstates.emplace_back(
          0x9b5, 0x0); // 12 // show neutral broken hill icon      // 2485
      packet.Worldstates.emplace_back(
          0x9b4, 0x1); // 13 // show icon above broken hill        // 2484
      packet.Worldstates.emplace_back(
          0x9b3, 0x0); // 14 // show ally broken hill icon         // 2483
      packet.Worldstates.emplace_back(
          0x9b2, 0x0); // 15 // show neutral overlook icon         // 2482
      packet.Worldstates.emplace_back(
          0x9b1, 0x1); // 16 // show the overlook arrow            // 2481
      packet.Worldstates.emplace_back(
          0x9b0, 0x0); // 17 // show ally overlook icon            // 2480
      packet.Worldstates.emplace_back(
          0x9ae, 0x0); // 18 // horde pvp objectives captured      // 2478
      packet.Worldstates.emplace_back(
          0x9ac, 0x0); // 19 // ally pvp objectives captured       // 2476
      packet.Worldstates.emplace_back(2475,
                                      100); //: ally / horde slider grey area //
                                            //: show only in direct vicinity!
      packet.Worldstates.emplace_back(
          2474, 50); //: ally / horde slider percentage, 100 for ally, 0 for
                     //: horde  // show only in direct vicinity!
      packet.Worldstates.emplace_back(2473,
                                      0); //: ally / horde slider display //
                                          //: show only in direct vicinity!
      packet.Worldstates.emplace_back(
          0x9a8, 0x0); // 20 // show the neutral stadium icon      // 2472
      packet.Worldstates.emplace_back(
          0x9a7, 0x0); // 21 // show the ally stadium icon         // 2471
      packet.Worldstates.emplace_back(
          0x9a6, 0x1); // 22 // show the horde stadium icon        // 2470
    }
    break;
  case 3518: // Nagrand
    if (pvp && pvp->GetTypeId() == OUTDOOR_PVP_NA)
      pvp->FillInitialWorldStates(packet);
    else {
      packet.Worldstates.emplace_back(2503, 0x0); // 10
      packet.Worldstates.emplace_back(2502, 0x0); // 11
      packet.Worldstates.emplace_back(2493, 0x0); // 12
      packet.Worldstates.emplace_back(2491, 0x0); // 13

      packet.Worldstates.emplace_back(2495, 0x0); // 14
      packet.Worldstates.emplace_back(2494, 0x0); // 15
      packet.Worldstates.emplace_back(2497, 0x0); // 16

      packet.Worldstates.emplace_back(2762, 0x0); // 17
      packet.Worldstates.emplace_back(2662, 0x0); // 18
      packet.Worldstates.emplace_back(2663, 0x0); // 19
      packet.Worldstates.emplace_back(2664, 0x0); // 20

      packet.Worldstates.emplace_back(2760, 0x0); // 21
      packet.Worldstates.emplace_back(2670, 0x0); // 22
      packet.Worldstates.emplace_back(2668, 0x0); // 23
      packet.Worldstates.emplace_back(2669, 0x0); // 24

      packet.Worldstates.emplace_back(2761, 0x0); // 25
      packet.Worldstates.emplace_back(2667, 0x0); // 26
      packet.Worldstates.emplace_back(2665, 0x0); // 27
      packet.Worldstates.emplace_back(2666, 0x0); // 28

      packet.Worldstates.emplace_back(2763, 0x0); // 29
      packet.Worldstates.emplace_back(2659, 0x0); // 30
      packet.Worldstates.emplace_back(2660, 0x0); // 31
      packet.Worldstates.emplace_back(2661, 0x0); // 32

      packet.Worldstates.emplace_back(2671, 0x0); // 33
      packet.Worldstates.emplace_back(2676, 0x0); // 34
      packet.Worldstates.emplace_back(2677, 0x0); // 35
      packet.Worldstates.emplace_back(2672, 0x0); // 36
      packet.Worldstates.emplace_back(2673, 0x0); // 37
    }
    break;
  case 3519: // Terokkar Forest
    if (pvp && pvp->GetTypeId() == OUTDOOR_PVP_TF)
      pvp->FillInitialWorldStates(packet);
    else {
      packet.Worldstates.emplace_back(0xa41, 0x0); // 10 // 2625 capture bar pos
      packet.Worldstates.emplace_back(0xa40,
                                      0x14); // 11 // 2624 capture bar neutral
      packet.Worldstates.emplace_back(0xa3f,
                                      0x0); // 12 // 2623 show capture bar
      packet.Worldstates.emplace_back(
          0xa3e, 0x0); // 13 // 2622 horde towers controlled
      packet.Worldstates.emplace_back(0xa3d,
                                      0x5); // 14 // 2621 ally towers controlled
      packet.Worldstates.emplace_back(0xa3c,
                                      0x0); // 15 // 2620 show towers controlled
      packet.Worldstates.emplace_back(0xa88, 0x0); // 16 // 2696 SE Neu
      packet.Worldstates.emplace_back(0xa87, 0x0); // 17 // SE Horde
      packet.Worldstates.emplace_back(0xa86, 0x0); // 18 // SE Ally
      packet.Worldstates.emplace_back(0xa85, 0x0); // 19 //S Neu
      packet.Worldstates.emplace_back(0xa84, 0x0); // 20 S Horde
      packet.Worldstates.emplace_back(0xa83, 0x0); // 21 S Ally
      packet.Worldstates.emplace_back(0xa82, 0x0); // 22 NE Neu
      packet.Worldstates.emplace_back(0xa81, 0x0); // 23 NE Horde
      packet.Worldstates.emplace_back(0xa80, 0x0); // 24 NE Ally
      packet.Worldstates.emplace_back(0xa7e, 0x0); // 25 // 2686 N Neu
      packet.Worldstates.emplace_back(0xa7d, 0x0); // 26 N Horde
      packet.Worldstates.emplace_back(0xa7c, 0x0); // 27 N Ally
      packet.Worldstates.emplace_back(0xa7b, 0x0); // 28 NW Ally
      packet.Worldstates.emplace_back(0xa7a, 0x0); // 29 NW Horde
      packet.Worldstates.emplace_back(0xa79, 0x0); // 30 NW Neutral
      packet.Worldstates.emplace_back(
          0x9d0, 0x5); // 31 // 2512 locked time remaining seconds first digit
      packet.Worldstates.emplace_back(
          0x9ce, 0x0); // 32 // 2510 locked time remaining seconds second digit
      packet.Worldstates.emplace_back(
          0x9cd, 0x0); // 33 // 2509 locked time remaining minutes
      packet.Worldstates.emplace_back(
          0x9cc, 0x0); // 34 // 2508 neutral locked time show
      packet.Worldstates.emplace_back(0xad0,
                                      0x0); // 35 // 2768 horde locked time show
      packet.Worldstates.emplace_back(0xacf,
                                      0x1); // 36 // 2767 ally locked time show
    }
    break;
  case 3521: // Zangarmarsh
    if (pvp && pvp->GetTypeId() == OUTDOOR_PVP_ZM)
      pvp->FillInitialWorldStates(packet);
    else {
      packet.Worldstates.emplace_back(0x9e1, 0x0); // 10 //2529
      packet.Worldstates.emplace_back(0x9e0, 0x0); // 11
      packet.Worldstates.emplace_back(0x9df, 0x0); // 12
      packet.Worldstates.emplace_back(0xa5d, 0x1); // 13 //2653
      packet.Worldstates.emplace_back(0xa5c,
                                      0x0); // 14 //2652 east beacon neutral
      packet.Worldstates.emplace_back(0xa5b, 0x1); // 15 horde
      packet.Worldstates.emplace_back(0xa5a, 0x0); // 16 ally
      packet.Worldstates.emplace_back(
          0xa59, 0x1); // 17 // 2649 Twin spire graveyard horde  12???
      packet.Worldstates.emplace_back(0xa58, 0x0); // 18 ally     14 ???
      packet.Worldstates.emplace_back(0xa57, 0x0); // 19 neutral  7???
      packet.Worldstates.emplace_back(0xa56,
                                      0x0); // 20 // 2646 west beacon neutral
      packet.Worldstates.emplace_back(0xa55, 0x1); // 21 horde
      packet.Worldstates.emplace_back(0xa54, 0x0); // 22 ally
      packet.Worldstates.emplace_back(0x9e7, 0x0); // 23 // 2535
      packet.Worldstates.emplace_back(0x9e6, 0x0); // 24
      packet.Worldstates.emplace_back(0x9e5, 0x0); // 25
      packet.Worldstates.emplace_back(0xa00, 0x0); // 26 // 2560
      packet.Worldstates.emplace_back(0x9ff, 0x1); // 27
      packet.Worldstates.emplace_back(0x9fe, 0x0); // 28
      packet.Worldstates.emplace_back(0x9fd, 0x0); // 29
      packet.Worldstates.emplace_back(0x9fc, 0x1); // 30
      packet.Worldstates.emplace_back(0x9fb, 0x0); // 31
      packet.Worldstates.emplace_back(0xa62, 0x0); // 32 // 2658
      packet.Worldstates.emplace_back(0xa61, 0x1); // 33
      packet.Worldstates.emplace_back(0xa60, 0x1); // 34
      packet.Worldstates.emplace_back(0xa5f, 0x0); // 35
    }
    break;
  case 3698: // Nagrand Arena
    if (bg && bg->GetTypeID(true) == BATTLEGROUND_NA)
      bg->FillInitialWorldStates(packet);
    else {
      packet.Worldstates.emplace_back(0xa0f, 0x0); // 7
      packet.Worldstates.emplace_back(0xa10, 0x0); // 8
      packet.Worldstates.emplace_back(0xa11, 0x0); // 9 show
    }
    break;
  case 3702: // Blade's Edge Arena
    if (bg && bg->GetTypeID(true) == BATTLEGROUND_BE)
      bg->FillInitialWorldStates(packet);
    else {
      packet.Worldstates.emplace_back(0x9f0, 0x0); // 7 gold
      packet.Worldstates.emplace_back(0x9f1, 0x0); // 8 green
      packet.Worldstates.emplace_back(0x9f3, 0x0); // 9 show
    }
    break;
  case 3968: // Ruins of Lordaeron
    if (bg && bg->GetTypeID(true) == BATTLEGROUND_RL)
      bg->FillInitialWorldStates(packet);
    else {
      packet.Worldstates.emplace_back(0xbb8, 0x0); // 7 gold
      packet.Worldstates.emplace_back(0xbb9, 0x0); // 8 green
      packet.Worldstates.emplace_back(0xbba, 0x0); // 9 show
    }
    break;
  case 4378: // Dalaran Sewers
    if (bg && bg->GetTypeID(true) == BATTLEGROUND_DS)
      bg->FillInitialWorldStates(packet);
    else {
      packet.Worldstates.emplace_back(3601, 0x0); // 7 gold
      packet.Worldstates.emplace_back(3600, 0x0); // 8 green
      packet.Worldstates.emplace_back(3610, 0x0); // 9 show
    }
    break;
  case 4384: // Strand of the Ancients
    if (bg && bg->GetTypeID(true) == BATTLEGROUND_SA)
      bg->FillInitialWorldStates(packet);
    else {
      // 1-3 A defend, 4-6 H defend, 7-9 unk defend, 1 - ok, 2 - half destroyed,
      // 3 - destroyed
      packet.Worldstates.emplace_back(0xf09, 0x0); // 7  3849 Gate of Temple
      packet.Worldstates.emplace_back(0xe36,
                                      0x0); // 8  3638 Gate of Yellow Moon
      packet.Worldstates.emplace_back(0xe27,
                                      0x0); // 9  3623 Gate of Green Emerald
      packet.Worldstates.emplace_back(0xe24,
                                      0x0); // 10 3620 Gate of Blue Sapphire
      packet.Worldstates.emplace_back(0xe21, 0x0); // 11 3617 Gate of Red Sun
      packet.Worldstates.emplace_back(0xe1e,
                                      0x0); // 12 3614 Gate of Purple Ametyst

      packet.Worldstates.emplace_back(
          0xdf3, 0x0); // 13 3571 bonus timer (1 - on, 0 - off)
      packet.Worldstates.emplace_back(0xded, 0x0); // 14 3565 Horde Attacker
      packet.Worldstates.emplace_back(0xdec, 0x0); // 15 3564 Alliance Attacker
      // End Round (timer), better explain this by example, eg. ends in 19:59 ->
      // A:BC
      packet.Worldstates.emplace_back(0xde9, 0x0); // 16 3561 C
      packet.Worldstates.emplace_back(0xde8, 0x0); // 17 3560 B
      packet.Worldstates.emplace_back(0xde7, 0x0); // 18 3559 A
      packet.Worldstates.emplace_back(0xe35,
                                      0x0); // 19 3637 East g - Horde control
      packet.Worldstates.emplace_back(0xe34,
                                      0x0); // 20 3636 West g - Horde control
      packet.Worldstates.emplace_back(0xe33,
                                      0x0); // 21 3635 South g - Horde control
      packet.Worldstates.emplace_back(0xe32,
                                      0x0); // 22 3634 East g - Alliance control
      packet.Worldstates.emplace_back(0xe31,
                                      0x0); // 23 3633 West g - Alliance control
      packet.Worldstates.emplace_back(
          0xe30, 0x0); // 24 3632 South g - Alliance control
      packet.Worldstates.emplace_back(
          0xe2f, 0x0); // 25 3631 Chamber of Ancients - Horde control
      packet.Worldstates.emplace_back(
          0xe2e, 0x0); // 26 3630 Chamber of Ancients - Alliance control
      packet.Worldstates.emplace_back(0xe2d,
                                      0x0); // 27 3629 Beach1 - Horde control
      packet.Worldstates.emplace_back(0xe2c,
                                      0x0); // 28 3628 Beach2 - Horde control
      packet.Worldstates.emplace_back(0xe2b,
                                      0x0); // 29 3627 Beach1 - Alliance control
      packet.Worldstates.emplace_back(0xe2a,
                                      0x0); // 30 3626 Beach2 - Alliance control
                                            // and many unks...
    }
    break;
  case 4406: // Ring of Valor
    if (bg && bg->GetTypeID(true) == BATTLEGROUND_RV)
      bg->FillInitialWorldStates(packet);
    else {
      packet.Worldstates.emplace_back(0xe10, 0x0); // 7 gold
      packet.Worldstates.emplace_back(0xe11, 0x0); // 8 green
      packet.Worldstates.emplace_back(0xe1a, 0x0); // 9 show
    }
    break;
  case 4710:
    if (bg && bg->GetTypeID(true) == BATTLEGROUND_IC)
      bg->FillInitialWorldStates(packet);
    else {
      packet.Worldstates.emplace_back(4221, 1); // 7 BG_IC_ALLIANCE_RENFORT_SET
      packet.Worldstates.emplace_back(4222, 1); // 8 BG_IC_HORDE_RENFORT_SET
      packet.Worldstates.emplace_back(4226, 300); // 9 BG_IC_ALLIANCE_RENFORT
      packet.Worldstates.emplace_back(4227, 300); // 10 BG_IC_HORDE_RENFORT
      packet.Worldstates.emplace_back(4322, 1); // 11 BG_IC_GATE_FRONT_H_WS_OPEN
      packet.Worldstates.emplace_back(4321, 1); // 12 BG_IC_GATE_WEST_H_WS_OPEN
      packet.Worldstates.emplace_back(4320, 1); // 13 BG_IC_GATE_EAST_H_WS_OPEN
      packet.Worldstates.emplace_back(4323, 1); // 14 BG_IC_GATE_FRONT_A_WS_OPEN
      packet.Worldstates.emplace_back(4324, 1); // 15 BG_IC_GATE_WEST_A_WS_OPEN
      packet.Worldstates.emplace_back(4325, 1); // 16 BG_IC_GATE_EAST_A_WS_OPEN
      packet.Worldstates.emplace_back(4317, 1); // 17 unknown

      packet.Worldstates.emplace_back(4301, 1); // 18 BG_IC_DOCKS_UNCONTROLLED
      packet.Worldstates.emplace_back(4296, 1); // 19 BG_IC_HANGAR_UNCONTROLLED
      packet.Worldstates.emplace_back(4306, 1); // 20 BG_IC_QUARRY_UNCONTROLLED
      packet.Worldstates.emplace_back(4311,
                                      1); // 21 BG_IC_REFINERY_UNCONTROLLED
      packet.Worldstates.emplace_back(4294,
                                      1); // 22 BG_IC_WORKSHOP_UNCONTROLLED
      packet.Worldstates.emplace_back(4243, 1); // 23 unknown
      packet.Worldstates.emplace_back(4345, 1); // 24 unknown
    }
    break;
    // The Ruby Sanctum
  case 4987:
    if (instance && mapid == 724)
      instance->FillInitialWorldStates(packet);
    else {
      packet.Worldstates.emplace_back(5049,
                                      50); // 9 WORLDSTATE_CORPOREALITY_MATERIAL
      packet.Worldstates.emplace_back(
          5050, 50); // 10 WORLDSTATE_CORPOREALITY_TWILIGHT
      packet.Worldstates.emplace_back(5051,
                                      0); // 11 WORLDSTATE_CORPOREALITY_TOGGLE
    }
    break;
    // Icecrown Citadel
  case 4812:
    if (instance && mapid == 631)
      instance->FillInitialWorldStates(packet);
    else {
      packet.Worldstates.emplace_back(
          4903, 0); // 9  WORLDSTATE_SHOW_TIMER (Blood Quickening weekly)
      packet.Worldstates.emplace_back(4904, 30); // 10 WORLDSTATE_EXECUTION_TIME
      packet.Worldstates.emplace_back(4940, 0);  // 11 WORLDSTATE_SHOW_ATTEMPTS
      packet.Worldstates.emplace_back(4941,
                                      50); // 12 WORLDSTATE_ATTEMPTS_REMAINING
      packet.Worldstates.emplace_back(4942, 50); // 13 WORLDSTATE_ATTEMPTS_MAX
    }
    break;
    // The Culling of Stratholme
  case 4100:
    if (instance && mapid == 595)
      instance->FillInitialWorldStates(packet);
    else {
      packet.Worldstates.emplace_back(3479, 0); // 9  WORLDSTATE_SHOW_CRATES
      packet.Worldstates.emplace_back(3480, 0); // 10 WORLDSTATE_CRATES_REVEALED
      packet.Worldstates.emplace_back(3504, 0); // 11 WORLDSTATE_WAVE_COUNT
      packet.Worldstates.emplace_back(3931, 25); // 12 WORLDSTATE_TIME_GUARDIAN
      packet.Worldstates.emplace_back(3932,
                                      0); // 13 WORLDSTATE_TIME_GUARDIAN_SHOW
    }
    break;
    // The Oculus
  case 4228:
    if (instance && mapid == 578)
      instance->FillInitialWorldStates(packet);
    else {
      packet.Worldstates.emplace_back(
          3524, 0); // 9  WORLD_STATE_CENTRIFUGE_CONSTRUCT_SHOW
      packet.Worldstates.emplace_back(
          3486, 0); // 10 WORLD_STATE_CENTRIFUGE_CONSTRUCT_AMOUNT
    }
    break;
    // Ulduar
  case 4273:
    if (instance && mapid == 603)
      instance->FillInitialWorldStates(packet);
    else {
      packet.Worldstates.emplace_back(4132,
                                      0); // 9  WORLDSTATE_ALGALON_TIMER_ENABLED
      packet.Worldstates.emplace_back(4131,
                                      0); // 10 WORLDSTATE_ALGALON_DESPAWN_TIMER
    }
    break;
    // Violet Hold
  case 4415:
    if (instance && mapid == 608)
      instance->FillInitialWorldStates(packet);
    else {
      packet.Worldstates.emplace_back(3816, 0); // 9  WORLD_STATE_VH_SHOW
      packet.Worldstates.emplace_back(3815,
                                      100); // 10 WORLD_STATE_VH_PRISON_STATE
      packet.Worldstates.emplace_back(3810, 0); // 11 WORLD_STATE_VH_WAVE_COUNT
    }
    break;
    // Halls of Refection
  case 4820:
    if (instance && mapid == 668)
      instance->FillInitialWorldStates(packet);
    else {
      packet.Worldstates.emplace_back(4884,
                                      0); // 9  WORLD_STATE_HOR_WAVES_ENABLED
      packet.Worldstates.emplace_back(4882, 0); // 10 WORLD_STATE_HOR_WAVE_COUNT
    }
    break;
    // Zul Aman
  case 3805:
    if (instance && mapid == 568)
      instance->FillInitialWorldStates(packet);
    else {
      packet.Worldstates.emplace_back(3104,
                                      0); // 9 WORLD_STATE_ZULAMAN_TIMER_ENABLED
      packet.Worldstates.emplace_back(3106, 0); // 10 WORLD_STATE_ZULAMAN_TIMER
    }
    break;
    // The Vortex Pinnacle
  case 5035:
    if (instance && mapid == 657)
      instance->FillInitialWorldStates(packet);
    else
      packet.Worldstates.emplace_back(5649, 0);
    break;
    // Blackwing Descent
  case 5094:
    if (instance && mapid == 669)
      instance->FillInitialWorldStates(packet);
    else
      packet.Worldstates.emplace_back(5652, 1);
    break;
    // Twin Peaks
  case 5031:
    if (bg && bg->GetTypeID(true) == BATTLEGROUND_TP)
      bg->FillInitialWorldStates(packet);
    else {
      packet.Worldstates.emplace_back(0x62d,
                                      0x0); //  7 1581 alliance flag captures
      packet.Worldstates.emplace_back(0x62e,
                                      0x0); //  8 1582 horde flag captures
      packet.Worldstates.emplace_back(0x609, 0x0); //  9 1545 unk
      packet.Worldstates.emplace_back(0x60a, 0x0); // 10 1546 unk
      packet.Worldstates.emplace_back(0x60b, 0x2); // 11 1547 unk
      packet.Worldstates.emplace_back(0x641, 0x3); // 12 1601 unk
      packet.Worldstates.emplace_back(
          0x922, 0x1); // 13 2338 horde (0 - hide, 1 - flag ok, 2 - flag picked
                       // up (flashing), 3 - flag picked up (not flashing)
      packet.Worldstates.emplace_back(
          0x923,
          0x1); // 14 2339 alliance (0 - hide, 1 - flag ok, 2 - flag picked up
                // (flashing), 3 - flag picked up (not flashing)
    }
    break;
    // Battle for Gilneas
  case 5449:
    if (bg && bg->GetTypeID(true) == BATTLEGROUND_BFG)
      bg->FillInitialWorldStates(packet);
    break;
    // Tol Barad Peninsula
  case 5389:
    if (sWorld->getBoolConfig(CONFIG_TOLBARAD_ENABLE)) {
      packet.Worldstates.emplace_back(
          5385u, sWorld->getWorldState(5385)); // TB_WS_ALLIANCE_CONTROLS_SHOW
      packet.Worldstates.emplace_back(
          5384u, sWorld->getWorldState(5384)); // TB_WS_HORDE_CONTROLS_SHOW
      packet.Worldstates.emplace_back(
          5387u, sWorld->getWorldState(5387)); // TB_WS_TIME_NEXT_BATTLE_SHOW
      packet.Worldstates.emplace_back(
          5546u, sWorld->getWorldState(5546)); // TB_WS_ALLIANCE_ATTACKING_SHOW
      packet.Worldstates.emplace_back(
          5547u, sWorld->getWorldState(5547)); // TB_WS_HORDE_ATTACKING_SHOW
    }
    break;
    // Tol Barad
  case 5095:
    if (bf && bf->GetTypeId() == BATTLEFIELD_TB)
      bf->FillInitialWorldStates(packet);
    break;
    // Wintergrasp
  case 4197:
    if (bf && bf->GetTypeId() == BATTLEFIELD_WG)
      bf->FillInitialWorldStates(packet);
    [[fallthrough]];
  default:
    packet.Worldstates.emplace_back(0x914, 0x0); // 7
    packet.Worldstates.emplace_back(0x913, 0x0); // 8
    packet.Worldstates.emplace_back(0x912, 0x0); // 9
    packet.Worldstates.emplace_back(0x915, 0x0); // 10
    break;
  }

  // insert realm wide world states
  sWorldStateMgr->AppendRealmWorldStates(packet.Worldstates);

  // insert map world states
  if (Map *map = GetMap())
    map->AppendWorldStates(packet.Worldstates);

  SendDirectMessage(packet.Write());
  SendBGWeekendWorldStates();
  SendBattlefieldWorldStates();
}

void Player::SendBGWeekendWorldStates() const {
  for (uint32 i = 1; i < sBattlemasterListStore.GetNumRows(); ++i) {
    BattlemasterListEntry const *bl = sBattlemasterListStore.LookupEntry(i);
    if (bl && bl->HolidayWorldState) {
      if (BattlegroundMgr::IsBGWeekend((BattlegroundTypeId)bl->ID))
        SendUpdateWorldState(bl->HolidayWorldState, 1);
      else
        SendUpdateWorldState(bl->HolidayWorldState, 0);
    }
  }
}

void Player::SendBattlefieldWorldStates() const {
  /// Send misc stuff that needs to be sent on every login, like the battle
  /// timers.
  if (sWorld->getBoolConfig(CONFIG_WINTERGRASP_ENABLE)) {
    if (BattlefieldWG *wg = static_cast<BattlefieldWG *>(
            sBattlefieldMgr->GetBattlefieldByBattleId(
                BATTLEFIELD_BATTLEID_WG))) {
      SendUpdateWorldState(BATTLEFIELD_WG_WORLD_STATE_ACTIVE,
                           wg->IsWarTime() ? 0 : 1);
      uint32 timer = wg->IsWarTime()
                         ? 0
                         : (wg->GetTimer() / 1000); // 0 - Time to next battle
      SendUpdateWorldState(ClockWorldState[1],
                           uint32(GameTime::GetGameTime() + timer));
    }
  }

  if (sWorld->getBoolConfig(CONFIG_TOLBARAD_ENABLE)) {
    if (Battlefield *tb = sBattlefieldMgr->GetBattlefieldByBattleId(
            BATTLEFIELD_BATTLEID_TB)) {
      SendUpdateWorldState(TB_WS_FACTION_CONTROLLING,
                           uint32(tb->GetDefenderTeam() + 1));
      uint32 timer = tb->GetTimer() / 1000;
      SendUpdateWorldState(TB_WS_TIME_BATTLE_END,
                           uint32(tb->IsWarTime()
                                      ? uint32(GameTime::GetGameTime() + timer)
                                      : 0));
      SendUpdateWorldState(TB_WS_TIME_NEXT_BATTLE,
                           uint32(!tb->IsWarTime()
                                      ? uint32(GameTime::GetGameTime() + timer)
                                      : 0));
    }
  }
}

uint32 Player::GetXPRestBonus(uint32 xp) {
  uint32 rested_bonus = (uint32)GetRestBonus(); // xp for each rested bonus

  if (rested_bonus > xp) // max rested_bonus == xp or (r+x) = 200% xp
    rested_bonus = xp;

  SetRestBonus(GetRestBonus() - rested_bonus);

  LOG_DEBUG("entities.player",
            "Player::GetXPRestBonus: Player '%s' (%s) gain %u xp (+%u Rested "
            "Bonus). Rested points=%f",
            GetGUID().ToString().c_str(), GetName().c_str(), xp + rested_bonus,
            rested_bonus, GetRestBonus());
  return rested_bonus;
}

void Player::SetBindPoint(ObjectGuid guid) const {
  WorldPackets::Misc::BinderConfirm packet(guid);
  SendDirectMessage(packet.Write());
}

void Player::SendTalentWipeConfirm(ObjectGuid guid) const {
  WorldPacket data(MSG_TALENT_WIPE_CONFIRM, 8 + 4);
  data << uint64(guid);
  uint32 cost = sWorld->getBoolConfig(CONFIG_NO_RESET_TALENT_COST)
                    ? 0
                    : GetNextResetTalentsCost();
  data << cost;
  SendDirectMessage(&data);
}

void Player::ResetPetTalents() {
  // This needs another gossip option + NPC text as a confirmation.
  // The confirmation gossip listid has the text: "Yes, please do."
  Pet *pet = GetPet();

  if (!pet || pet->getPetType() != HUNTER_PET || pet->m_usedTalentCount == 0)
    return;

  CharmInfo *charmInfo = pet->GetCharmInfo();
  if (!charmInfo) {
    LOG_ERROR("entities.player",
              "Object (GUID: %u TypeId: %u) is considered pet-like, but "
              "doesn't have charm info!",
              pet->GetGUID().GetCounter(), pet->GetTypeId());
    return;
  }
  pet->resetTalents();
  SendTalentsInfoData(true);
}

/*********************************************************/
/***                    STORAGE SYSTEM                 ***/
/*********************************************************/

void Player::SetVirtualItemSlot(uint8 i, Item *item) {
  ASSERT(i < 3);
  if (i < 2 && item) {
    if (!item->GetEnchantmentId(TEMP_ENCHANTMENT_SLOT))
      return;
    uint32 charges = item->GetEnchantmentCharges(TEMP_ENCHANTMENT_SLOT);
    if (charges == 0)
      return;
    if (charges > 1)
      item->SetEnchantmentCharges(TEMP_ENCHANTMENT_SLOT, charges - 1);
    else {
      ApplyEnchantment(item, TEMP_ENCHANTMENT_SLOT, false);
      item->ClearEnchantment(TEMP_ENCHANTMENT_SLOT);
    }
  }
}

void Player::SetSheath(SheathState sheathed) {
  switch (sheathed) {
  case SHEATH_STATE_UNARMED: // no prepared weapon
    SetVirtualItemSlot(0, nullptr);
    SetVirtualItemSlot(1, nullptr);
    SetVirtualItemSlot(2, nullptr);
    break;
  case SHEATH_STATE_MELEE: // prepared melee weapon
    SetVirtualItemSlot(0, GetWeaponForAttack(BASE_ATTACK, true));
    SetVirtualItemSlot(1, GetWeaponForAttack(OFF_ATTACK, true));
    SetVirtualItemSlot(2, nullptr);
    break;
  case SHEATH_STATE_RANGED: // prepared ranged weapon
    SetVirtualItemSlot(0, nullptr);
    SetVirtualItemSlot(1, nullptr);
    SetVirtualItemSlot(2, GetWeaponForAttack(RANGED_ATTACK, true));
    break;
  default:
    SetVirtualItemSlot(0, nullptr);
    SetVirtualItemSlot(1, nullptr);
    SetVirtualItemSlot(2, nullptr);
    break;
  }
  Unit::SetSheath(
      sheathed); // this must visualize Sheath changing for other players...
}

uint8 Player::FindEquipSlot(ItemTemplate const *proto, uint32 slot,
                            bool swap) const {
  uint8 playerClass = getClass();

  uint8 slots[4];
  slots[0] = NULL_SLOT;
  slots[1] = NULL_SLOT;
  slots[2] = NULL_SLOT;
  slots[3] = NULL_SLOT;
  switch (proto->GetInventoryType()) {
  case INVTYPE_HEAD:
    slots[0] = EQUIPMENT_SLOT_HEAD;
    break;
  case INVTYPE_NECK:
    slots[0] = EQUIPMENT_SLOT_NECK;
    break;
  case INVTYPE_SHOULDERS:
    slots[0] = EQUIPMENT_SLOT_SHOULDERS;
    break;
  case INVTYPE_BODY:
    slots[0] = EQUIPMENT_SLOT_BODY;
    break;
  case INVTYPE_CHEST:
    slots[0] = EQUIPMENT_SLOT_CHEST;
    break;
  case INVTYPE_ROBE:
    slots[0] = EQUIPMENT_SLOT_CHEST;
    break;
  case INVTYPE_WAIST:
    slots[0] = EQUIPMENT_SLOT_WAIST;
    break;
  case INVTYPE_LEGS:
    slots[0] = EQUIPMENT_SLOT_LEGS;
    break;
  case INVTYPE_FEET:
    slots[0] = EQUIPMENT_SLOT_FEET;
    break;
  case INVTYPE_WRISTS:
    slots[0] = EQUIPMENT_SLOT_WRISTS;
    break;
  case INVTYPE_HANDS:
    slots[0] = EQUIPMENT_SLOT_HANDS;
    break;
  case INVTYPE_FINGER:
    slots[0] = EQUIPMENT_SLOT_FINGER1;
    slots[1] = EQUIPMENT_SLOT_FINGER2;
    break;
  case INVTYPE_TRINKET:
    slots[0] = EQUIPMENT_SLOT_TRINKET1;
    slots[1] = EQUIPMENT_SLOT_TRINKET2;
    break;
  case INVTYPE_CLOAK:
    slots[0] = EQUIPMENT_SLOT_BACK;
    break;
  case INVTYPE_WEAPON: {
    slots[0] = EQUIPMENT_SLOT_MAINHAND;

    // suggest offhand slot only if know dual wielding
    // (this will be replace mainhand weapon at auto equip instead unwonted "you
    // don't known dual wielding" ...
    if (CanDualWield())
      slots[1] = EQUIPMENT_SLOT_OFFHAND;
    break;
  }
  case INVTYPE_SHIELD:
    slots[0] = EQUIPMENT_SLOT_OFFHAND;
    break;
  case INVTYPE_RANGED:
    slots[0] = EQUIPMENT_SLOT_RANGED;
    break;
  case INVTYPE_2HWEAPON:
    slots[0] = EQUIPMENT_SLOT_MAINHAND;
    if (Item *mhWeapon =
            GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND)) {
      if (ItemTemplate const *mhWeaponProto = mhWeapon->GetTemplate()) {
        if (mhWeaponProto->GetSubClass() == ITEM_SUBCLASS_WEAPON_POLEARM ||
            mhWeaponProto->GetSubClass() == ITEM_SUBCLASS_WEAPON_STAFF) {
          const_cast<Player *>(this)->AutoUnequipOffhandIfNeed(true);
          break;
        }
      }
    }

    if (GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND)) {
      if (proto->GetSubClass() == ITEM_SUBCLASS_WEAPON_POLEARM ||
          proto->GetSubClass() == ITEM_SUBCLASS_WEAPON_STAFF) {
        const_cast<Player *>(this)->AutoUnequipOffhandIfNeed(true);
        break;
      }
    }
    if (CanDualWield() && CanTitanGrip() &&
        proto->GetSubClass() != ITEM_SUBCLASS_WEAPON_POLEARM &&
        proto->GetSubClass() != ITEM_SUBCLASS_WEAPON_STAFF)
      slots[1] = EQUIPMENT_SLOT_OFFHAND;
    break;
  case INVTYPE_TABARD:
    slots[0] = EQUIPMENT_SLOT_TABARD;
    break;
  case INVTYPE_WEAPONMAINHAND:
    slots[0] = EQUIPMENT_SLOT_MAINHAND;
    break;
  case INVTYPE_WEAPONOFFHAND:
    slots[0] = EQUIPMENT_SLOT_OFFHAND;
    break;
  case INVTYPE_HOLDABLE:
    slots[0] = EQUIPMENT_SLOT_OFFHAND;
    break;
  case INVTYPE_THROWN:
    slots[0] = EQUIPMENT_SLOT_RANGED;
    break;
  case INVTYPE_RANGEDRIGHT:
    slots[0] = EQUIPMENT_SLOT_RANGED;
    break;
  case INVTYPE_BAG:
    slots[0] = INVENTORY_SLOT_BAG_START + 0;
    slots[1] = INVENTORY_SLOT_BAG_START + 1;
    slots[2] = INVENTORY_SLOT_BAG_START + 2;
    slots[3] = INVENTORY_SLOT_BAG_START + 3;
    break;
  case INVTYPE_RELIC: {
    if (playerClass == CLASS_PALADIN || playerClass == CLASS_DRUID ||
        playerClass == CLASS_SHAMAN || playerClass == CLASS_DEATH_KNIGHT)
      slots[0] = EQUIPMENT_SLOT_RANGED;
    break;
  }
  default:
    return NULL_SLOT;
  }

  if (slot != NULL_SLOT) {
    if (swap || !GetItemByPos(INVENTORY_SLOT_BAG_0, slot))
      for (uint8 i = 0; i < 4; ++i)
        if (slots[i] == slot)
          return slot;
  } else {
    // search free slot at first
    for (uint8 i = 0; i < 4; ++i)
      if (slots[i] != NULL_SLOT &&
          !GetItemByPos(INVENTORY_SLOT_BAG_0, slots[i]))
        // in case 2hand equipped weapon (without titan grip) offhand slot empty
        // but not free
        if (slots[i] != EQUIPMENT_SLOT_OFFHAND || !IsTwoHandUsed())
          return slots[i];

    // if not found free and can swap return first appropriate from used
    for (uint8 i = 0; i < 4; ++i)
      if (slots[i] != NULL_SLOT && swap)
        return slots[i];
  }

  // no free position
  return NULL_SLOT;
}

InventoryResult Player::CanUnequipItems(uint32 item, uint32 count) const {
  uint32 tempcount = 0;

  InventoryResult res = EQUIP_ERR_OK;

  for (uint8 i = EQUIPMENT_SLOT_START; i < INVENTORY_SLOT_BAG_END; ++i)
    if (Item *pItem = GetItemByPos(INVENTORY_SLOT_BAG_0, i))
      if (pItem->GetEntry() == item) {
        InventoryResult ires =
            CanUnequipItem(INVENTORY_SLOT_BAG_0 << 8 | i, false);
        if (ires == EQUIP_ERR_OK) {
          tempcount += pItem->GetCount();
          if (tempcount >= count)
            return EQUIP_ERR_OK;
        } else
          res = ires;
      }

  for (uint8 i = INVENTORY_SLOT_ITEM_START; i < INVENTORY_SLOT_ITEM_END; ++i)
    if (Item *pItem = GetItemByPos(INVENTORY_SLOT_BAG_0, i))
      if (pItem->GetEntry() == item) {
        tempcount += pItem->GetCount();
        if (tempcount >= count)
          return EQUIP_ERR_OK;
      }

  for (uint8 i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; ++i)
    if (Bag *pBag = GetBagByPos(i))
      for (uint32 j = 0; j < pBag->GetBagSize(); ++j)
        if (Item *pItem = GetItemByPos(i, j))
          if (pItem->GetEntry() == item) {
            tempcount += pItem->GetCount();
            if (tempcount >= count)
              return EQUIP_ERR_OK;
          }

  // not found req. item count and have unequippable items
  return res;
}

uint32 Player::GetItemCount(uint32 item, bool inBankAlso,
                            Item *skipItem) const {
  uint32 count = 0;
  for (uint8 i = EQUIPMENT_SLOT_START; i < INVENTORY_SLOT_ITEM_END; i++)
    if (Item *pItem = GetItemByPos(INVENTORY_SLOT_BAG_0, i))
      if (pItem != skipItem && pItem->GetEntry() == item)
        count += pItem->GetCount();

  for (uint8 i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; ++i)
    if (Bag *pBag = GetBagByPos(i))
      count += pBag->GetItemCount(item, skipItem);

  if (skipItem && skipItem->GetTemplate()->GetGemProperties())
    for (uint8 i = EQUIPMENT_SLOT_START; i < INVENTORY_SLOT_ITEM_END; ++i)
      if (Item *pItem = GetItemByPos(INVENTORY_SLOT_BAG_0, i))
        if (pItem != skipItem && pItem->GetTemplate()->GetSocketColor(0))
          count += pItem->GetGemCountWithID(item);

  if (inBankAlso) {
    // checking every item from 39 to 74 (including bank bags)
    for (uint8 i = BANK_SLOT_ITEM_START; i < BANK_SLOT_BAG_END; ++i)
      if (Item *pItem = GetItemByPos(INVENTORY_SLOT_BAG_0, i))
        if (pItem != skipItem && pItem->GetEntry() == item)
          count += pItem->GetCount();

    for (uint8 i = BANK_SLOT_BAG_START; i < BANK_SLOT_BAG_END; ++i)
      if (Bag *pBag = GetBagByPos(i))
        count += pBag->GetItemCount(item, skipItem);

    if (skipItem && skipItem->GetTemplate()->GetGemProperties())
      for (uint8 i = BANK_SLOT_ITEM_START; i < BANK_SLOT_ITEM_END; ++i)
        if (Item *pItem = GetItemByPos(INVENTORY_SLOT_BAG_0, i))
          if (pItem != skipItem && pItem->GetTemplate()->GetSocketColor(0))
            count += pItem->GetGemCountWithID(item);
  }

  return count;
}

uint32 Player::GetItemCountWithLimitCategory(uint32 limitCategory,
                                             Item *skipItem) const {
  uint32 count = 0;
  for (int i = EQUIPMENT_SLOT_START; i < INVENTORY_SLOT_ITEM_END; ++i)
    if (Item *pItem = GetItemByPos(INVENTORY_SLOT_BAG_0, i))
      if (pItem != skipItem)
        if (ItemTemplate const *pProto = pItem->GetTemplate())
          if (pProto->GetItemLimitCategory() == limitCategory)
            count += pItem->GetCount();

  for (int i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; ++i)
    if (Bag *pBag = GetBagByPos(i))
      count += pBag->GetItemCountWithLimitCategory(limitCategory, skipItem);

  for (int i = BANK_SLOT_ITEM_START; i < BANK_SLOT_BAG_END; ++i)
    if (Item *pItem = GetItemByPos(INVENTORY_SLOT_BAG_0, i))
      if (pItem != skipItem)
        if (ItemTemplate const *pProto = pItem->GetTemplate())
          if (pProto->GetItemLimitCategory() == limitCategory)
            count += pItem->GetCount();

  for (int i = BANK_SLOT_BAG_START; i < BANK_SLOT_BAG_END; ++i)
    if (Bag *pBag = GetBagByPos(i))
      count += pBag->GetItemCountWithLimitCategory(limitCategory, skipItem);

  return count;
}

Item *Player::GetItemByGuid(ObjectGuid guid) const {
  for (uint8 i = EQUIPMENT_SLOT_START; i < INVENTORY_SLOT_ITEM_END; ++i)
    if (Item *pItem = GetItemByPos(INVENTORY_SLOT_BAG_0, i))
      if (pItem->GetGUID() == guid)
        return pItem;

  for (int i = BANK_SLOT_ITEM_START; i < BANK_SLOT_BAG_END; ++i)
    if (Item *pItem = GetItemByPos(INVENTORY_SLOT_BAG_0, i))
      if (pItem->GetGUID() == guid)
        return pItem;

  for (uint8 i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; ++i)
    if (Bag *pBag = GetBagByPos(i))
      for (uint32 j = 0; j < pBag->GetBagSize(); ++j)
        if (Item *pItem = pBag->GetItemByPos(j))
          if (pItem->GetGUID() == guid)
            return pItem;

  for (uint8 i = BANK_SLOT_BAG_START; i < BANK_SLOT_BAG_END; ++i)
    if (Bag *pBag = GetBagByPos(i))
      for (uint32 j = 0; j < pBag->GetBagSize(); ++j)
        if (Item *pItem = pBag->GetItemByPos(j))
          if (pItem->GetGUID() == guid)
            return pItem;

  return nullptr;
}

Item *Player::GetItemByPos(uint16 pos) const {
  uint8 bag = pos >> 8;
  uint8 slot = pos & 255;
  return GetItemByPos(bag, slot);
}

Item *Player::GetItemByPos(uint8 bag, uint8 slot) const {
  if (bag == INVENTORY_SLOT_BAG_0 && slot < BANK_SLOT_BAG_END)
    return m_items[slot];
  if (Bag *pBag = GetBagByPos(bag))
    return pBag->GetItemByPos(slot);
  return nullptr;
}

// Does additional check for disarmed weapons
Item *Player::GetUseableItemByPos(uint8 bag, uint8 slot) const {
  if (!CanUseAttackType(GetAttackBySlot(slot)))
    return nullptr;
  return GetItemByPos(bag, slot);
}

Bag *Player::GetBagByPos(uint8 bag) const {
  if ((bag >= INVENTORY_SLOT_BAG_START && bag < INVENTORY_SLOT_BAG_END) ||
      (bag >= BANK_SLOT_BAG_START && bag < BANK_SLOT_BAG_END))
    if (Item *item = GetItemByPos(INVENTORY_SLOT_BAG_0, bag))
      return item->ToBag();
  return nullptr;
}

Item *Player::GetWeaponForAttack(WeaponAttackType attackType,
                                 bool useable /*= false*/) const {
  uint8 slot;
  switch (attackType) {
  case BASE_ATTACK:
    slot = EQUIPMENT_SLOT_MAINHAND;
    break;
  case OFF_ATTACK:
    slot = EQUIPMENT_SLOT_OFFHAND;
    break;
  case RANGED_ATTACK:
    slot = EQUIPMENT_SLOT_RANGED;
    break;
  default:
    return nullptr;
  }

  Item *item;
  if (useable)
    item = GetUseableItemByPos(INVENTORY_SLOT_BAG_0, slot);
  else
    item = GetItemByPos(INVENTORY_SLOT_BAG_0, slot);
  if (!item || item->GetTemplate()->GetClass() != ITEM_CLASS_WEAPON)
    return nullptr;

  if (!useable)
    return item;

  if (item->IsBroken() || IsInFeralForm())
    return nullptr;

  return item;
}

Item *Player::GetShield(bool useable) const {
  Item *item = nullptr;
  if (useable)
    item = GetUseableItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
  else
    item = GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
  if (!item || item->GetTemplate()->GetClass() != ITEM_CLASS_ARMOR)
    return nullptr;

  if (!useable)
    return item;

  if (item->IsBroken())
    return nullptr;

  return item;
}

WeaponAttackType Player::GetAttackBySlot(uint8 slot) {
  switch (slot) {
  case EQUIPMENT_SLOT_MAINHAND:
    return BASE_ATTACK;
  case EQUIPMENT_SLOT_OFFHAND:
    return OFF_ATTACK;
  case EQUIPMENT_SLOT_RANGED:
    return RANGED_ATTACK;
  default:
    return MAX_ATTACK;
  }
}

bool Player::IsInventoryPos(uint8 bag, uint8 slot) {
  if (bag == INVENTORY_SLOT_BAG_0 && slot == NULL_SLOT)
    return true;
  if (bag == INVENTORY_SLOT_BAG_0 &&
      (slot >= INVENTORY_SLOT_ITEM_START && slot < INVENTORY_SLOT_ITEM_END))
    return true;
  if (bag >= INVENTORY_SLOT_BAG_START && bag < INVENTORY_SLOT_BAG_END)
    return true;
  return false;
}

bool Player::IsEquipmentPos(uint8 bag, uint8 slot) {
  if (bag == INVENTORY_SLOT_BAG_0 && (slot < EQUIPMENT_SLOT_END))
    return true;
  if (bag == INVENTORY_SLOT_BAG_0 &&
      (slot >= INVENTORY_SLOT_BAG_START && slot < INVENTORY_SLOT_BAG_END))
    return true;
  return false;
}

bool Player::IsBankPos(uint8 bag, uint8 slot) {
  if (bag == INVENTORY_SLOT_BAG_0 &&
      (slot >= BANK_SLOT_ITEM_START && slot < BANK_SLOT_ITEM_END))
    return true;
  if (bag == INVENTORY_SLOT_BAG_0 &&
      (slot >= BANK_SLOT_BAG_START && slot < BANK_SLOT_BAG_END))
    return true;
  if (bag >= BANK_SLOT_BAG_START && bag < BANK_SLOT_BAG_END)
    return true;
  return false;
}

bool Player::IsBagPos(uint16 pos) {
  uint8 bag = pos >> 8;
  uint8 slot = pos & 255;
  if (bag == INVENTORY_SLOT_BAG_0 &&
      (slot >= INVENTORY_SLOT_BAG_START && slot < INVENTORY_SLOT_BAG_END))
    return true;
  if (bag == INVENTORY_SLOT_BAG_0 &&
      (slot >= BANK_SLOT_BAG_START && slot < BANK_SLOT_BAG_END))
    return true;
  return false;
}

bool Player::IsValidPos(uint8 bag, uint8 slot, bool explicit_pos) const {
  // post selected
  if (bag == NULL_BAG && !explicit_pos)
    return true;

  if (bag == INVENTORY_SLOT_BAG_0) {
    // any post selected
    if (slot == NULL_SLOT && !explicit_pos)
      return true;

    // equipment
    if (slot < EQUIPMENT_SLOT_END)
      return true;

    // bag equip slots
    if (slot >= INVENTORY_SLOT_BAG_START && slot < INVENTORY_SLOT_BAG_END)
      return true;

    // backpack slots
    if (slot >= INVENTORY_SLOT_ITEM_START && slot < INVENTORY_SLOT_ITEM_END)
      return true;

    // bank main slots
    if (slot >= BANK_SLOT_ITEM_START && slot < BANK_SLOT_ITEM_END)
      return true;

    // bank bag slots
    if (slot >= BANK_SLOT_BAG_START && slot < BANK_SLOT_BAG_END)
      return true;

    return false;
  }

  // bag content slots
  // bank bag content slots
  if (Bag *pBag = GetBagByPos(bag)) {
    // any post selected
    if (slot == NULL_SLOT && !explicit_pos)
      return true;

    return slot < pBag->GetBagSize();
  }

  // where this?
  return false;
}

bool Player::HasItemCount(uint32 item, uint32 count, bool inBankAlso) const {
  uint32 tempcount = 0;
  for (uint8 i = EQUIPMENT_SLOT_START; i < INVENTORY_SLOT_ITEM_END; i++) {
    Item *pItem = GetItemByPos(INVENTORY_SLOT_BAG_0, i);
    if (pItem && pItem->GetEntry() == item && !pItem->IsInTrade()) {
      tempcount += pItem->GetCount();
      if (tempcount >= count)
        return true;
    }
  }

  for (uint8 i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; i++) {
    if (Bag *pBag = GetBagByPos(i)) {
      for (uint32 j = 0; j < pBag->GetBagSize(); j++) {
        Item *pItem = GetItemByPos(i, j);
        if (pItem && pItem->GetEntry() == item && !pItem->IsInTrade()) {
          tempcount += pItem->GetCount();
          if (tempcount >= count)
            return true;
        }
      }
    }
  }

  if (inBankAlso) {
    for (uint8 i = BANK_SLOT_ITEM_START; i < BANK_SLOT_BAG_END; i++) {
      Item *pItem = GetItemByPos(INVENTORY_SLOT_BAG_0, i);
      if (pItem && pItem->GetEntry() == item && !pItem->IsInTrade()) {
        tempcount += pItem->GetCount();
        if (tempcount >= count)
          return true;
      }
    }
    for (uint8 i = BANK_SLOT_BAG_START; i < BANK_SLOT_BAG_END; i++) {
      if (Bag *pBag = GetBagByPos(i)) {
        for (uint32 j = 0; j < pBag->GetBagSize(); j++) {
          Item *pItem = GetItemByPos(i, j);
          if (pItem && pItem->GetEntry() == item && !pItem->IsInTrade()) {
            tempcount += pItem->GetCount();
            if (tempcount >= count)
              return true;
          }
        }
      }
    }
  }

  return false;
}

bool Player::HasItemOrGemWithIdEquipped(uint32 item, uint32 count,
                                        uint8 except_slot) const {
  uint32 tempcount = 0;
  for (uint8 i = EQUIPMENT_SLOT_START; i < EQUIPMENT_SLOT_END; ++i) {
    if (i == except_slot)
      continue;

    Item *pItem = GetItemByPos(INVENTORY_SLOT_BAG_0, i);
    if (pItem && pItem->GetEntry() == item) {
      tempcount += pItem->GetCount();
      if (tempcount >= count)
        return true;
    }
  }

  ItemTemplate const *pProto = sObjectMgr->GetItemTemplate(item);
  if (pProto && pProto->GetGemProperties()) {
    for (uint8 i = EQUIPMENT_SLOT_START; i < EQUIPMENT_SLOT_END; ++i) {
      if (i == except_slot)
        continue;

      Item *pItem = GetItemByPos(INVENTORY_SLOT_BAG_0, i);
      if (pItem && pItem->GetTemplate()->GetSocketColor(0)) {
        tempcount += pItem->GetGemCountWithID(item);
        if (tempcount >= count)
          return true;
      }
    }
  }

  return false;
}

bool Player::HasItemWithLimitCategoryEquipped(uint32 limitCategory,
                                              uint32 count,
                                              uint8 except_slot) const {
  uint32 tempcount = 0;
  for (uint8 i = EQUIPMENT_SLOT_START; i < EQUIPMENT_SLOT_END; ++i) {
    if (i == except_slot)
      continue;

    Item *pItem = GetItemByPos(INVENTORY_SLOT_BAG_0, i);
    if (!pItem)
      continue;

    ItemTemplate const *pProto = pItem->GetTemplate();
    if (!pProto)
      continue;

    if (pProto->GetItemLimitCategory() == limitCategory) {
      tempcount += pItem->GetCount();
      if (tempcount >= count)
        return true;
    }
  }

  return false;
}

bool Player::HasGemWithLimitCategoryEquipped(uint32 limitCategory, uint32 count,
                                             uint8 except_slot) const {
  uint32 tempcount = 0;
  for (uint8 i = EQUIPMENT_SLOT_START; i < EQUIPMENT_SLOT_END; ++i) {
    if (i == except_slot)
      continue;

    Item *pItem = GetItemByPos(INVENTORY_SLOT_BAG_0, i);
    if (!pItem)
      continue;

    ItemTemplate const *pProto = pItem->GetTemplate();
    if (!pProto)
      continue;

    if (pProto->GetSocketColor(0) ||
        pItem->GetEnchantmentId(PRISMATIC_ENCHANTMENT_SLOT)) {
      tempcount += pItem->GetGemCountWithLimitCategory(limitCategory);
      if (tempcount >= count)
        return true;
    }
  }

  return false;
}

InventoryResult
Player::CanTakeMoreSimilarItems(uint32 entry, uint32 count, Item *pItem,
                                uint32 *no_space_count /*= nullptr*/,
                                uint32 *offendingItemId /*= nullptr*/) const {
  ItemTemplate const *pProto = sObjectMgr->GetItemTemplate(entry);
  if (!pProto) {
    if (no_space_count)
      *no_space_count = count;
    return EQUIP_ERR_ITEM_MAX_COUNT;
  }

  if (pItem && pItem->m_lootGenerated)
    return EQUIP_ERR_LOOT_GONE;

  // no maximum
  if ((pProto->GetMaxCount() <= 0 && pProto->GetItemLimitCategory() == 0) ||
      pProto->GetMaxCount() == 2147483647)
    return EQUIP_ERR_OK;

  if (pProto->GetMaxCount() > 0) {
    uint32 curcount = GetItemCount(pProto->GetId(), true, pItem);
    if (curcount + count > uint32(pProto->GetMaxCount())) {
      if (no_space_count)
        *no_space_count = count + curcount - pProto->GetMaxCount();
      return EQUIP_ERR_ITEM_MAX_COUNT;
    }
  }

  // check unique-equipped limit
  if (pProto->GetItemLimitCategory()) {
    ItemLimitCategoryEntry const *limitEntry =
        sItemLimitCategoryStore.LookupEntry(pProto->GetItemLimitCategory());
    if (!limitEntry) {
      if (no_space_count)
        *no_space_count = count;
      return EQUIP_ERR_NOT_EQUIPPABLE;
    }

    if (limitEntry->Flags == ITEM_LIMIT_CATEGORY_MODE_HAVE) {
      uint32 curcount =
          GetItemCountWithLimitCategory(pProto->GetItemLimitCategory(), pItem);
      if (curcount + count > uint32(limitEntry->Quantity)) {
        if (no_space_count)
          *no_space_count = count + curcount - limitEntry->Quantity;
        if (offendingItemId)
          *offendingItemId = pProto->GetId();
        return EQUIP_ERR_ITEM_MAX_LIMIT_CATEGORY_COUNT_EXCEEDED_IS;
      }
    }
  }

  return EQUIP_ERR_OK;
}

InventoryResult
Player::CanTakeMoreSimilarItems(Item *pItem,
                                uint32 *itemLimitCategory /*= nullptr*/) const {
  return CanTakeMoreSimilarItems(pItem->GetEntry(), pItem->GetCount(), pItem,
                                 nullptr, itemLimitCategory);
}

InventoryResult
Player::CanStoreNewItem(uint8 bag, uint8 slot, ItemPosCountVec &dest,
                        uint32 item, uint32 count,
                        uint32 *no_space_count /*= nullptr*/) const {
  return CanStoreItem(bag, slot, dest, item, count, nullptr, false,
                      no_space_count);
}

InventoryResult Player::CanStoreItem(uint8 bag, uint8 slot,
                                     ItemPosCountVec &dest, Item *pItem,
                                     bool swap /*= false*/) const {
  if (!pItem)
    return EQUIP_ERR_ITEM_NOT_FOUND;
  uint32 count = pItem->GetCount();
  return CanStoreItem(bag, slot, dest, pItem->GetEntry(), count, pItem, swap,
                      nullptr);
}

bool Player::HasItemTotemCategory(uint32 TotemCategory) const {
  Item *item;
  for (uint8 i = EQUIPMENT_SLOT_START; i < INVENTORY_SLOT_ITEM_END; ++i) {
    item = GetUseableItemByPos(INVENTORY_SLOT_BAG_0, i);
    if (item && DBCManager::IsTotemCategoryCompatibleWith(
                    item->GetTemplate()->GetTotemCategory(), TotemCategory))
      return true;
  }

  Bag *bag;
  for (uint8 i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; ++i) {
    bag = GetBagByPos(i);
    if (bag) {
      for (uint32 j = 0; j < bag->GetBagSize(); ++j) {
        item = GetUseableItemByPos(i, j);
        if (item && sDBCManager.IsTotemCategoryCompatibleWith(
                        item->GetTemplate()->GetTotemCategory(), TotemCategory))
          return true;
      }
    }
  }

  return false;
}

InventoryResult Player::CanStoreItem_InSpecificSlot(uint8 bag, uint8 slot,
                                                    ItemPosCountVec &dest,
                                                    ItemTemplate const *pProto,
                                                    uint32 &count, bool swap,
                                                    Item *pSrcItem) const {
  Item *pItem2 = GetItemByPos(bag, slot);

  // ignore move item (this slot will be empty at move)
  if (pItem2 == pSrcItem)
    pItem2 = nullptr;

  uint32 need_space;

  if (pSrcItem && pSrcItem->IsNotEmptyBag() &&
      !IsBagPos(uint16(bag) << 8 | slot))
    return EQUIP_ERR_DESTROY_NONEMPTY_BAG;

  // empty specific slot - check item fit to slot
  if (!pItem2 || swap) {
    if (bag == INVENTORY_SLOT_BAG_0) {
      // prevent cheating
      if ((slot >= BUYBACK_SLOT_START && slot < BUYBACK_SLOT_END) ||
          slot >= PLAYER_SLOT_END)
        return EQUIP_ERR_WRONG_BAG_TYPE;
    } else {
      Bag *pBag = GetBagByPos(bag);
      if (!pBag)
        return EQUIP_ERR_WRONG_BAG_TYPE;

      ItemTemplate const *pBagProto = pBag->GetTemplate();
      if (!pBagProto)
        return EQUIP_ERR_WRONG_BAG_TYPE;

      if (slot >= pBagProto->GetContainerSlots())
        return EQUIP_ERR_WRONG_BAG_TYPE;

      if (!ItemCanGoIntoBag(pProto, pBagProto))
        return EQUIP_ERR_WRONG_BAG_TYPE;
    }

    // non empty stack with space
    need_space = pProto->GetMaxStackSize();
  }
  // non empty slot, check item type
  else {
    // can be merged at least partly
    InventoryResult res = pItem2->CanBeMergedPartlyWith(pProto);
    if (res != EQUIP_ERR_OK)
      return res;

    // free stack space or infinity
    need_space = pProto->GetMaxStackSize() - pItem2->GetCount();
  }

  if (need_space > count)
    need_space = count;

  ItemPosCount newPosition = ItemPosCount((bag << 8) | slot, need_space);
  if (!newPosition.isContainedIn(dest)) {
    dest.push_back(newPosition);
    count -= need_space;
  }
  return EQUIP_ERR_OK;
}

InventoryResult Player::CanStoreItem_InBag(uint8 bag, ItemPosCountVec &dest,
                                           ItemTemplate const *pProto,
                                           uint32 &count, bool merge,
                                           bool non_specialized, Item *pSrcItem,
                                           uint8 skip_bag,
                                           uint8 skip_slot) const {
  // skip specific bag already processed in first called CanStoreItem_InBag
  if (bag == skip_bag)
    return EQUIP_ERR_WRONG_BAG_TYPE;

  // skip non-existing bag or self targeted bag
  Bag *pBag = GetBagByPos(bag);
  if (!pBag || pBag == pSrcItem)
    return EQUIP_ERR_WRONG_BAG_TYPE;

  if (pSrcItem && pSrcItem->IsNotEmptyBag())
    return EQUIP_ERR_DESTROY_NONEMPTY_BAG;

  ItemTemplate const *pBagProto = pBag->GetTemplate();
  if (!pBagProto)
    return EQUIP_ERR_WRONG_BAG_TYPE;

  // specialized bag mode or non-specilized
  if (non_specialized != (pBagProto->GetClass() == ITEM_CLASS_CONTAINER &&
                          pBagProto->GetSubClass() == ITEM_SUBCLASS_CONTAINER))
    return EQUIP_ERR_WRONG_BAG_TYPE;

  if (!ItemCanGoIntoBag(pProto, pBagProto))
    return EQUIP_ERR_WRONG_BAG_TYPE;

  for (uint32 j = 0; j < pBag->GetBagSize(); j++) {
    // skip specific slot already processed in first called
    // CanStoreItem_InSpecificSlot
    if (j == skip_slot)
      continue;

    Item *pItem2 = GetItemByPos(bag, j);

    // ignore move item (this slot will be empty at move)
    if (pItem2 == pSrcItem)
      pItem2 = nullptr;

    // if merge skip empty, if !merge skip non-empty
    if ((pItem2 != nullptr) != merge)
      continue;

    uint32 need_space = pProto->GetMaxStackSize();

    if (pItem2) {
      // can be merged at least partly
      if (pItem2->CanBeMergedPartlyWith(pProto) != EQUIP_ERR_OK)
        continue;

      // descrease at current stacksize
      need_space -= pItem2->GetCount();
    }

    if (need_space > count)
      need_space = count;

    ItemPosCount newPosition = ItemPosCount((bag << 8) | j, need_space);
    if (!newPosition.isContainedIn(dest)) {
      dest.push_back(newPosition);
      count -= need_space;

      if (count == 0)
        return EQUIP_ERR_OK;
    }
  }
  return EQUIP_ERR_OK;
}

InventoryResult Player::CanStoreItem_InInventorySlots(
    uint8 slot_begin, uint8 slot_end, ItemPosCountVec &dest,
    ItemTemplate const *pProto, uint32 &count, bool merge, Item *pSrcItem,
    uint8 skip_bag, uint8 skip_slot) const {
  // this is never called for non-bag slots so we can do this
  if (pSrcItem && pSrcItem->IsNotEmptyBag())
    return EQUIP_ERR_DESTROY_NONEMPTY_BAG;

  for (uint32 j = slot_begin; j < slot_end; j++) {
    // skip specific slot already processed in first called
    // CanStoreItem_InSpecificSlot
    if (INVENTORY_SLOT_BAG_0 == skip_bag && j == skip_slot)
      continue;

    Item *pItem2 = GetItemByPos(INVENTORY_SLOT_BAG_0, j);

    // ignore move item (this slot will be empty at move)
    if (pItem2 == pSrcItem)
      pItem2 = nullptr;

    // if merge skip empty, if !merge skip non-empty
    if ((pItem2 != nullptr) != merge)
      continue;

    uint32 need_space = pProto->GetMaxStackSize();

    if (pItem2) {
      // can be merged at least partly
      if (pItem2->CanBeMergedPartlyWith(pProto) != EQUIP_ERR_OK)
        continue;

      // descrease at current stacksize
      need_space -= pItem2->GetCount();
    }

    if (need_space > count)
      need_space = count;

    ItemPosCount newPosition =
        ItemPosCount((INVENTORY_SLOT_BAG_0 << 8) | j, need_space);
    if (!newPosition.isContainedIn(dest)) {
      dest.push_back(newPosition);
      count -= need_space;

      if (count == 0)
        return EQUIP_ERR_OK;
    }
  }
  return EQUIP_ERR_OK;
}

InventoryResult Player::CanStoreItem(uint8 bag, uint8 slot,
                                     ItemPosCountVec &dest, uint32 entry,
                                     uint32 count, Item *pItem, bool swap,
                                     uint32 *no_space_count) const {
  LOG_DEBUG("entities.player.items",
            "Player::CanStoreItem: Bag: %u, Slot: %u, Item: %u, Count: %u", bag,
            slot, entry, count);

  ItemTemplate const *pProto = sObjectMgr->GetItemTemplate(entry);
  if (!pProto) {
    if (no_space_count)
      *no_space_count = count;
    return swap ? EQUIP_ERR_CANT_SWAP : EQUIP_ERR_ITEM_NOT_FOUND;
  }

  if (pItem) {
    // item used
    if (pItem->m_lootGenerated) {
      if (no_space_count)
        *no_space_count = count;
      return EQUIP_ERR_LOOT_GONE;
    }

    if (pItem->IsBindedNotWith(this)) {
      if (no_space_count)
        *no_space_count = count;
      return EQUIP_ERR_NOT_OWNER;
    }
  }

  // check count of items (skip for auto move for same player from bank)
  uint32 no_similar_count = 0; // can't store this amount similar items
  InventoryResult res =
      CanTakeMoreSimilarItems(entry, count, pItem, &no_similar_count);
  if (res != EQUIP_ERR_OK) {
    if (count == no_similar_count) {
      if (no_space_count)
        *no_space_count = no_similar_count;
      return res;
    }
    count -= no_similar_count;
  }

  // in specific slot
  if (bag != NULL_BAG && slot != NULL_SLOT) {
    res = CanStoreItem_InSpecificSlot(bag, slot, dest, pProto, count, swap,
                                      pItem);
    if (res != EQUIP_ERR_OK) {
      if (no_space_count)
        *no_space_count = count + no_similar_count;
      return res;
    }

    if (count == 0) {
      if (no_similar_count == 0)
        return EQUIP_ERR_OK;

      if (no_space_count)
        *no_space_count = count + no_similar_count;
      return EQUIP_ERR_ITEM_MAX_COUNT;
    }
  }

  // not specific slot or have space for partly store only in specific slot

  // in specific bag
  if (bag != NULL_BAG) {
    // search stack in bag for merge to
    if (pProto->GetMaxStackSize() != 1) {
      if (bag == INVENTORY_SLOT_BAG_0) // inventory
      {
        res = CanStoreItem_InInventorySlots(
            INVENTORY_SLOT_ITEM_START, INVENTORY_SLOT_ITEM_END, dest, pProto,
            count, true, pItem, bag, slot);
        if (res != EQUIP_ERR_OK) {
          if (no_space_count)
            *no_space_count = count + no_similar_count;
          return res;
        }

        if (count == 0) {
          if (no_similar_count == 0)
            return EQUIP_ERR_OK;

          if (no_space_count)
            *no_space_count = count + no_similar_count;
          return EQUIP_ERR_ITEM_MAX_COUNT;
        }
      } else // equipped bag
      {
        // we need check 2 time (specialized/non_specialized), use NULL_BAG to
        // prevent skipping bag
        res = CanStoreItem_InBag(bag, dest, pProto, count, true, false, pItem,
                                 NULL_BAG, slot);
        if (res != EQUIP_ERR_OK)
          res = CanStoreItem_InBag(bag, dest, pProto, count, true, true, pItem,
                                   NULL_BAG, slot);

        if (res != EQUIP_ERR_OK) {
          if (no_space_count)
            *no_space_count = count + no_similar_count;
          return res;
        }

        if (count == 0) {
          if (no_similar_count == 0)
            return EQUIP_ERR_OK;

          if (no_space_count)
            *no_space_count = count + no_similar_count;
          return EQUIP_ERR_ITEM_MAX_COUNT;
        }
      }
    }

    // search free slot in bag for place to
    if (bag == INVENTORY_SLOT_BAG_0) // inventory
    {
      res = CanStoreItem_InInventorySlots(INVENTORY_SLOT_ITEM_START,
                                          INVENTORY_SLOT_ITEM_END, dest, pProto,
                                          count, false, pItem, bag, slot);
      if (res != EQUIP_ERR_OK) {
        if (no_space_count)
          *no_space_count = count + no_similar_count;
        return res;
      }

      if (count == 0) {
        if (no_similar_count == 0)
          return EQUIP_ERR_OK;

        if (no_space_count)
          *no_space_count = count + no_similar_count;
        return EQUIP_ERR_ITEM_MAX_COUNT;
      }
    } else // equipped bag
    {
      res = CanStoreItem_InBag(bag, dest, pProto, count, false, false, pItem,
                               NULL_BAG, slot);
      if (res != EQUIP_ERR_OK)
        res = CanStoreItem_InBag(bag, dest, pProto, count, false, true, pItem,
                                 NULL_BAG, slot);

      if (res != EQUIP_ERR_OK) {
        if (no_space_count)
          *no_space_count = count + no_similar_count;
        return res;
      }

      if (count == 0) {
        if (no_similar_count == 0)
          return EQUIP_ERR_OK;

        if (no_space_count)
          *no_space_count = count + no_similar_count;
        return EQUIP_ERR_ITEM_MAX_COUNT;
      }
    }
  }

  // not specific bag or have space for partly store only in specific bag

  // search stack for merge to
  if (pProto->GetMaxStackSize() != 1) {
    res = CanStoreItem_InInventorySlots(INVENTORY_SLOT_ITEM_START,
                                        INVENTORY_SLOT_ITEM_END, dest, pProto,
                                        count, true, pItem, bag, slot);
    if (res != EQUIP_ERR_OK) {
      if (no_space_count)
        *no_space_count = count + no_similar_count;
      return res;
    }

    if (count == 0) {
      if (no_similar_count == 0)
        return EQUIP_ERR_OK;

      if (no_space_count)
        *no_space_count = count + no_similar_count;
      return EQUIP_ERR_ITEM_MAX_COUNT;
    }

    if (pProto->GetBagFamily()) {
      for (uint32 i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END;
           i++) {
        res = CanStoreItem_InBag(i, dest, pProto, count, true, false, pItem,
                                 bag, slot);
        if (res != EQUIP_ERR_OK)
          continue;

        if (count == 0) {
          if (no_similar_count == 0)
            return EQUIP_ERR_OK;

          if (no_space_count)
            *no_space_count = count + no_similar_count;
          return EQUIP_ERR_ITEM_MAX_COUNT;
        }
      }
    }

    for (uint32 i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; i++) {
      res = CanStoreItem_InBag(i, dest, pProto, count, true, true, pItem, bag,
                               slot);
      if (res != EQUIP_ERR_OK)
        continue;

      if (count == 0) {
        if (no_similar_count == 0)
          return EQUIP_ERR_OK;

        if (no_space_count)
          *no_space_count = count + no_similar_count;
        return EQUIP_ERR_ITEM_MAX_COUNT;
      }
    }
  }

  // search free slot - special bag case
  if (pProto->GetBagFamily()) {
    for (uint32 i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; i++) {
      res = CanStoreItem_InBag(i, dest, pProto, count, false, false, pItem, bag,
                               slot);
      if (res != EQUIP_ERR_OK)
        continue;

      if (count == 0) {
        if (no_similar_count == 0)
          return EQUIP_ERR_OK;

        if (no_space_count)
          *no_space_count = count + no_similar_count;
        return EQUIP_ERR_ITEM_MAX_COUNT;
      }
    }
  }

  if (pItem && pItem->IsNotEmptyBag())
    return EQUIP_ERR_BAG_IN_BAG;

  // search free slot
  uint8 searchSlotStart = INVENTORY_SLOT_ITEM_START;
  // new bags can be directly equipped
  if (!pItem && pProto->GetClass() == ITEM_CLASS_CONTAINER &&
      pProto->GetSubClass() == ITEM_SUBCLASS_CONTAINER &&
      (pProto->GetBonding() == BIND_NONE ||
       pProto->GetBonding() == BIND_ON_ACQUIRE))
    searchSlotStart = INVENTORY_SLOT_BAG_START;

  res = CanStoreItem_InInventorySlots(searchSlotStart, INVENTORY_SLOT_ITEM_END,
                                      dest, pProto, count, false, pItem, bag,
                                      slot);
  if (res != EQUIP_ERR_OK) {
    if (no_space_count)
      *no_space_count = count + no_similar_count;
    return res;
  }
  if (count == 0) {
    if (no_similar_count == 0)
      return EQUIP_ERR_OK;

    if (no_space_count)
      *no_space_count = count + no_similar_count;
    return EQUIP_ERR_ITEM_MAX_COUNT;
  }

  for (uint8 i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; i++) {
    res = CanStoreItem_InBag(i, dest, pProto, count, false, true, pItem, bag,
                             slot);
    if (res != EQUIP_ERR_OK)
      continue;

    if (count == 0) {
      if (no_similar_count == 0)
        return EQUIP_ERR_OK;

      if (no_space_count)
        *no_space_count = count + no_similar_count;
      return EQUIP_ERR_ITEM_MAX_COUNT;
    }
  }

  if (no_space_count)
    *no_space_count = count + no_similar_count;

  return EQUIP_ERR_INV_FULL;
}

//////////////////////////////////////////////////////////////////////////
InventoryResult Player::CanStoreItems(Item **items, int count,
                                      uint32 *offendingItemId) const {
  Item *item2;

  // fill space table
  uint32 inventoryCounts[INVENTORY_SLOT_ITEM_END - INVENTORY_SLOT_ITEM_START];
  uint32 bagCounts[INVENTORY_SLOT_BAG_END - INVENTORY_SLOT_BAG_START]
                  [MAX_BAG_SIZE];

  memset(inventoryCounts, 0,
         sizeof(uint32) *
             (INVENTORY_SLOT_ITEM_END - INVENTORY_SLOT_ITEM_START));
  memset(bagCounts, 0,
         sizeof(uint32) * (INVENTORY_SLOT_BAG_END - INVENTORY_SLOT_BAG_START) *
             MAX_BAG_SIZE);

  for (uint8 i = INVENTORY_SLOT_ITEM_START; i < INVENTORY_SLOT_ITEM_END; i++) {
    item2 = GetItemByPos(INVENTORY_SLOT_BAG_0, i);
    if (item2 && !item2->IsInTrade())
      inventoryCounts[i - INVENTORY_SLOT_ITEM_START] = item2->GetCount();
  }

  for (uint8 i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; i++)
    if (Bag *pBag = GetBagByPos(i))
      for (uint32 j = 0; j < pBag->GetBagSize(); j++) {
        item2 = GetItemByPos(i, j);
        if (item2 && !item2->IsInTrade())
          bagCounts[i - INVENTORY_SLOT_BAG_START][j] = item2->GetCount();
      }

  // check free space for all items
  for (int k = 0; k < count; ++k) {
    Item *item = items[k];

    // no item
    if (!item)
      continue;

    LOG_DEBUG("entities.player.items",
              "Player::CanStoreItems: Player '%s' (%s), Index: %i ItemID: %u, "
              "Count: %u",
              GetName().c_str(), GetGUID().ToString().c_str(), k + 1,
              item->GetEntry(), item->GetCount());
    ItemTemplate const *pProto = item->GetTemplate();

    // strange item
    if (!pProto)
      return EQUIP_ERR_ITEM_NOT_FOUND;

    // item used
    if (item->m_lootGenerated)
      return EQUIP_ERR_LOOT_GONE;

    // item it 'bind'
    if (item->IsBindedNotWith(this))
      return EQUIP_ERR_NOT_OWNER;

    ItemTemplate const *pBagProto;

    // item is 'one item only'
    InventoryResult res = CanTakeMoreSimilarItems(item, offendingItemId);
    if (res != EQUIP_ERR_OK)
      return res;

    // search stack for merge to
    if (pProto->GetMaxStackSize() != 1) {
      bool b_found = false;

      for (int t = INVENTORY_SLOT_ITEM_START; t < INVENTORY_SLOT_ITEM_END;
           ++t) {
        item2 = GetItemByPos(INVENTORY_SLOT_BAG_0, t);
        if (item2 && item2->CanBeMergedPartlyWith(pProto) == EQUIP_ERR_OK &&
            inventoryCounts[t - INVENTORY_SLOT_ITEM_START] + item->GetCount() <=
                pProto->GetMaxStackSize()) {
          inventoryCounts[t - INVENTORY_SLOT_ITEM_START] += item->GetCount();
          b_found = true;
          break;
        }
      }
      if (b_found)
        continue;

      for (int t = INVENTORY_SLOT_BAG_START;
           !b_found && t < INVENTORY_SLOT_BAG_END; ++t) {
        if (Bag *bag = GetBagByPos(t)) {
          if (ItemCanGoIntoBag(item->GetTemplate(), bag->GetTemplate())) {
            for (uint32 j = 0; j < bag->GetBagSize(); j++) {
              item2 = GetItemByPos(t, j);
              if (item2 &&
                  item2->CanBeMergedPartlyWith(pProto) == EQUIP_ERR_OK &&
                  bagCounts[t - INVENTORY_SLOT_BAG_START][j] +
                          item->GetCount() <=
                      pProto->GetMaxStackSize()) {
                bagCounts[t - INVENTORY_SLOT_BAG_START][j] += item->GetCount();
                b_found = true;
                break;
              }
            }
          }
        }
      }
      if (b_found)
        continue;
    }

    // special bag case
    if (pProto->GetBagFamily()) {
      bool b_found = false;

      for (int t = INVENTORY_SLOT_BAG_START;
           !b_found && t < INVENTORY_SLOT_BAG_END; ++t) {
        if (Bag *bag = GetBagByPos(t)) {
          pBagProto = bag->GetTemplate();

          // not plain container check
          if (pBagProto &&
              (pBagProto->GetClass() != ITEM_CLASS_CONTAINER ||
               pBagProto->GetSubClass() != ITEM_SUBCLASS_CONTAINER) &&
              ItemCanGoIntoBag(pProto, pBagProto)) {
            for (uint32 j = 0; j < bag->GetBagSize(); j++) {
              if (bagCounts[t - INVENTORY_SLOT_BAG_START][j] == 0) {
                bagCounts[t - INVENTORY_SLOT_BAG_START][j] = 1;
                b_found = true;
                break;
              }
            }
          }
        }
      }
      if (b_found)
        continue;
    }

    // search free slot
    bool b_found = false;
    for (int t = INVENTORY_SLOT_ITEM_START; t < INVENTORY_SLOT_ITEM_END; ++t) {
      if (inventoryCounts[t - INVENTORY_SLOT_ITEM_START] == 0) {
        inventoryCounts[t - INVENTORY_SLOT_ITEM_START] = 1;
        b_found = true;
        break;
      }
    }
    if (b_found)
      continue;

    // search free slot in bags
    for (uint8 t = INVENTORY_SLOT_BAG_START;
         !b_found && t < INVENTORY_SLOT_BAG_END; ++t) {
      if (Bag *bag = GetBagByPos(t)) {
        pBagProto = bag->GetTemplate();

        // special bag already checked
        if (pBagProto && (pBagProto->GetClass() != ITEM_CLASS_CONTAINER ||
                          pBagProto->GetSubClass() != ITEM_SUBCLASS_CONTAINER))
          continue;

        for (uint32 j = 0; j < bag->GetBagSize(); j++) {
          if (bagCounts[t - INVENTORY_SLOT_BAG_START][j] == 0) {
            bagCounts[t - INVENTORY_SLOT_BAG_START][j] = 1;
            b_found = true;
            break;
          }
        }
      }
    }

    // no free slot found?
    if (!b_found)
      return EQUIP_ERR_BAG_FULL;
  }

  return EQUIP_ERR_OK;
}

//////////////////////////////////////////////////////////////////////////
InventoryResult Player::CanEquipNewItem(uint8 slot, uint16 &dest, uint32 item,
                                        bool swap) const {
  dest = 0;
  Item *pItem = Item::CreateItem(item, 1, this);
  if (pItem) {
    InventoryResult result = CanEquipItem(slot, dest, pItem, swap);
    delete pItem;
    return result;
  }

  return EQUIP_ERR_ITEM_NOT_FOUND;
}

InventoryResult Player::CanEquipItem(uint8 slot, uint16 &dest, Item *pItem,
                                     bool swap, bool not_loading) const {
  dest = 0;
  if (pItem) {
    LOG_DEBUG(
        "entities.player.items",
        "Player::CanEquipItem: Player '%s' (%s), Slot: %u, Item: %u, Count: %u",
        GetName().c_str(), GetGUID().ToString().c_str(), slot,
        pItem->GetEntry(), pItem->GetCount());
    ItemTemplate const *pProto = pItem->GetTemplate();
    if (pProto) {
      // item used
      if (pItem->m_lootGenerated)
        return EQUIP_ERR_LOOT_GONE;

      if (pItem->IsBindedNotWith(this))
        return EQUIP_ERR_NOT_OWNER;

      // check count of items (skip for auto move for same player from bank)
      InventoryResult res = CanTakeMoreSimilarItems(pItem);
      if (res != EQUIP_ERR_OK)
        return res;

      // check this only in game
      if (not_loading) {
        // May be here should be more stronger checks; STUNNED checked
        // ROOT, CONFUSED, DISTRACTED, FLEEING this needs to be checked.
        if (HasUnitState(UNIT_STATE_STUNNED))
          return EQUIP_ERR_GENERIC_STUNNED;

        if (IsCharmed())
          return EQUIP_ERR_CLIENT_LOCKED_OUT; // @todo is this the correct
                                              // error?

        // do not allow equipping gear except weapons, offhands, projectiles,
        // relics in
        // - combat
        // - in-progress arenas
        if (!pProto->CanChangeEquipStateInCombat()) {
          if (IsInCombat())
            return EQUIP_ERR_NOT_IN_COMBAT;

          if (Battleground *bg = GetBattleground())
            if (bg->isArena() && bg->GetStatus() == STATUS_IN_PROGRESS)
              return EQUIP_ERR_NOT_DURING_ARENA_MATCH;
        }

        if (IsInCombat() &&
            (pProto->GetClass() == ITEM_CLASS_WEAPON ||
             pProto->GetInventoryType() == INVTYPE_RELIC) &&
            m_weaponChangeTimer != 0)
          return EQUIP_ERR_CLIENT_LOCKED_OUT; // maybe exist better err

        if (IsNonMeleeSpellCast(false))
          return EQUIP_ERR_CLIENT_LOCKED_OUT;
      }

      ScalingStatDistributionEntry const *ssd =
          pProto->GetScalingStatDistribution()
              ? sScalingStatDistributionStore.LookupEntry(
                    pProto->GetScalingStatDistribution())
              : 0;
      // check allowed level (extend range to upper values if MaxLevel more or
      // equal max player level, this let GM set high level with 1...max range
      // items)
      if (ssd && ssd->Maxlevel < DEFAULT_MAX_LEVEL &&
          ssd->Maxlevel < getLevel())
        return EQUIP_ERR_NOT_EQUIPPABLE;

      uint8 eslot = FindEquipSlot(pProto, slot, swap);
      if (eslot == NULL_SLOT)
        return EQUIP_ERR_NOT_EQUIPPABLE;

      res = CanUseItem(pItem, not_loading);
      if (res != EQUIP_ERR_OK)
        return res;

      if (!swap && GetItemByPos(INVENTORY_SLOT_BAG_0, eslot))
        return EQUIP_ERR_NO_SLOT_AVAILABLE;

      // if we are swapping 2 equiped items, CanEquipUniqueItem check
      // should ignore the item we are trying to swap, and not the
      // destination item. CanEquipUniqueItem should ignore destination
      // item only when we are swapping weapon from bag
      uint8 ignore = uint8(NULL_SLOT);
      switch (eslot) {
      case EQUIPMENT_SLOT_MAINHAND:
        ignore = EQUIPMENT_SLOT_OFFHAND;
        break;
      case EQUIPMENT_SLOT_OFFHAND:
        ignore = EQUIPMENT_SLOT_MAINHAND;
        break;
      case EQUIPMENT_SLOT_FINGER1:
        ignore = EQUIPMENT_SLOT_FINGER2;
        break;
      case EQUIPMENT_SLOT_FINGER2:
        ignore = EQUIPMENT_SLOT_FINGER1;
        break;
      case EQUIPMENT_SLOT_TRINKET1:
        ignore = EQUIPMENT_SLOT_TRINKET2;
        break;
      case EQUIPMENT_SLOT_TRINKET2:
        ignore = EQUIPMENT_SLOT_TRINKET1;
        break;
      }

      if (ignore == uint8(NULL_SLOT) ||
          pItem != GetItemByPos(INVENTORY_SLOT_BAG_0, ignore))
        ignore = eslot;

      InventoryResult res2 =
          CanEquipUniqueItem(pItem, swap ? ignore : uint8(NULL_SLOT));
      if (res2 != EQUIP_ERR_OK)
        return res2;

      // check unique-equipped special item classes
      if (pProto->GetClass() == ITEM_CLASS_QUIVER)
        for (uint8 i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END;
             ++i)
          if (Item *pBag = GetItemByPos(INVENTORY_SLOT_BAG_0, i))
            if (pBag != pItem)
              if (ItemTemplate const *pBagProto = pBag->GetTemplate())
                if (pBagProto->GetClass() == pProto->GetClass() &&
                    (!swap || pBag->GetSlot() != eslot))
                  return (pBagProto->GetSubClass() == ITEM_SUBCLASS_AMMO_POUCH)
                             ? EQUIP_ERR_ONLY_ONE_AMMO
                             : EQUIP_ERR_ONLY_ONE_QUIVER;

      uint32 type = pProto->GetInventoryType();

      if (eslot == EQUIPMENT_SLOT_OFFHAND) {
        // Do not allow polearm to be equipped in the offhand (rare case for the
        // only 1h polearm 41750)
        if (type == INVTYPE_WEAPON &&
            pProto->GetSubClass() == ITEM_SUBCLASS_WEAPON_POLEARM)
          return EQUIP_ERR_2HSKILLNOTFOUND;
        else if (type == INVTYPE_WEAPON || type == INVTYPE_WEAPONOFFHAND) {
          if (!CanDualWield())
            return EQUIP_ERR_2HSKILLNOTFOUND;
        } else if (type == INVTYPE_2HWEAPON) {
          if (!CanDualWield() || !CanTitanGrip())
            return EQUIP_ERR_2HSKILLNOTFOUND;
        }

        if (IsTwoHandUsed())
          return EQUIP_ERR_2HANDED_EQUIPPED;
      }

      // equip two-hand weapon case (with possible unequip 2 items)
      if (type == INVTYPE_2HWEAPON) {
        if (eslot == EQUIPMENT_SLOT_OFFHAND) {
          if (!CanTitanGrip())
            return EQUIP_ERR_NOT_EQUIPPABLE;
        } else if (eslot != EQUIPMENT_SLOT_MAINHAND)
          return EQUIP_ERR_NOT_EQUIPPABLE;

        if (!CanTitanGrip()) {
          // offhand item must can be stored in inventory for offhand item and
          // it also must be unequipped
          Item *offItem =
              GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
          ItemPosCountVec off_dest;
          if (offItem && (!not_loading ||
                          CanUnequipItem(uint16(INVENTORY_SLOT_BAG_0) << 8 |
                                             EQUIPMENT_SLOT_OFFHAND,
                                         false) != EQUIP_ERR_OK ||
                          CanStoreItem(NULL_BAG, NULL_SLOT, off_dest, offItem,
                                       false) != EQUIP_ERR_OK))
            return swap ? EQUIP_ERR_CANT_SWAP : EQUIP_ERR_INV_FULL;
        }
      }
      dest = ((INVENTORY_SLOT_BAG_0 << 8) | eslot);
      return EQUIP_ERR_OK;
    }
  }

  return !swap ? EQUIP_ERR_ITEM_NOT_FOUND : EQUIP_ERR_CANT_SWAP;
}

InventoryResult Player::CanUnequipItem(uint16 pos, bool swap) const {
  // Applied only to equipped items and bank bags
  if (!IsEquipmentPos(pos) && !IsBagPos(pos))
    return EQUIP_ERR_OK;

  Item *pItem = GetItemByPos(pos);

  // Applied only to existing equipped item
  if (!pItem)
    return EQUIP_ERR_OK;

  LOG_DEBUG(
      "entities.player.items",
      "Player::CanUnequipItem: Player '%s' (%s), Slot: %u, Item: %u, Count: %u",
      GetName().c_str(), GetGUID().ToString().c_str(), pos, pItem->GetEntry(),
      pItem->GetCount());

  ItemTemplate const *pProto = pItem->GetTemplate();
  if (!pProto)
    return EQUIP_ERR_ITEM_NOT_FOUND;

  // item used
  if (pItem->m_lootGenerated)
    return EQUIP_ERR_LOOT_GONE;

  if (IsCharmed())
    return EQUIP_ERR_CLIENT_LOCKED_OUT; // @todo is this the correct error?

  // do not allow unequipping gear except weapons, offhands, projectiles, relics
  // in
  // - combat
  // - in-progress arenas
  if (!pProto->CanChangeEquipStateInCombat()) {
    if (IsInCombat())
      return EQUIP_ERR_NOT_IN_COMBAT;

    if (Battleground *bg = GetBattleground())
      if (bg->isArena() && bg->GetStatus() == STATUS_IN_PROGRESS)
        return EQUIP_ERR_NOT_DURING_ARENA_MATCH;
  }

  if (!swap && pItem->IsNotEmptyBag())
    return EQUIP_ERR_DESTROY_NONEMPTY_BAG;

  return EQUIP_ERR_OK;
}

InventoryResult Player::CanBankItem(uint8 bag, uint8 slot,
                                    ItemPosCountVec &dest, Item *pItem,
                                    bool swap, bool not_loading) const {
  if (!pItem)
    return swap ? EQUIP_ERR_CANT_SWAP : EQUIP_ERR_ITEM_NOT_FOUND;

  uint32 count = pItem->GetCount();

  LOG_DEBUG("entities.player.items",
            "Player::CanBankItem: Player '%s' (%s), Bag: %u, Slot: %u, Item: "
            "%u, Count: %u",
            GetName().c_str(), GetGUID().ToString().c_str(), bag, slot,
            pItem->GetEntry(), pItem->GetCount());
  ItemTemplate const *pProto = pItem->GetTemplate();
  if (!pProto)
    return swap ? EQUIP_ERR_CANT_SWAP : EQUIP_ERR_ITEM_NOT_FOUND;

  // item used
  if (pItem->m_lootGenerated)
    return EQUIP_ERR_LOOT_GONE;

  if (pItem->IsBindedNotWith(this))
    return EQUIP_ERR_NOT_OWNER;

  // Currency Tokenize are not supposed to be swapped out of their hidden bag
  if (pItem->IsCurrencyToken()) {
    LOG_INFO("entities.player.cheat",
             "Possible hacking attempt: Player %s (%s) tried to move token [%s "
             "entry: %u] out of the currency bag!",
             GetName().c_str(), GetGUID().ToString().c_str(),
             pItem->GetGUID().ToString().c_str(), pProto->GetId());
    return EQUIP_ERR_CANT_SWAP;
  }

  // check count of items (skip for auto move for same player from bank)
  InventoryResult res = CanTakeMoreSimilarItems(pItem);
  if (res != EQUIP_ERR_OK)
    return res;

  // in specific slot
  if (bag != NULL_BAG && slot != NULL_SLOT) {
    if (slot >= BANK_SLOT_BAG_START && slot < BANK_SLOT_BAG_END) {
      if (!pItem->IsBag())
        return EQUIP_ERR_WRONG_SLOT;

      if (slot - BANK_SLOT_BAG_START >= GetBankBagSlotCount())
        return EQUIP_ERR_NO_BANK_SLOT;

      res = CanUseItem(pItem, not_loading);
      if (res != EQUIP_ERR_OK)
        return res;
    }

    res = CanStoreItem_InSpecificSlot(bag, slot, dest, pProto, count, swap,
                                      pItem);
    if (res != EQUIP_ERR_OK)
      return res;

    if (count == 0)
      return EQUIP_ERR_OK;
  }

  // not specific slot or have space for partly store only in specific slot

  // in specific bag
  if (bag != NULL_BAG) {
    if (pItem->IsNotEmptyBag())
      return EQUIP_ERR_BAG_IN_BAG;

    // search stack in bag for merge to
    if (pProto->GetMaxStackSize() != 1) {
      if (bag == INVENTORY_SLOT_BAG_0) {
        res = CanStoreItem_InInventorySlots(BANK_SLOT_ITEM_START,
                                            BANK_SLOT_ITEM_END, dest, pProto,
                                            count, true, pItem, bag, slot);
        if (res != EQUIP_ERR_OK)
          return res;

        if (count == 0)
          return EQUIP_ERR_OK;
      } else {
        res = CanStoreItem_InBag(bag, dest, pProto, count, true, false, pItem,
                                 NULL_BAG, slot);
        if (res != EQUIP_ERR_OK)
          res = CanStoreItem_InBag(bag, dest, pProto, count, true, true, pItem,
                                   NULL_BAG, slot);

        if (res != EQUIP_ERR_OK)
          return res;

        if (count == 0)
          return EQUIP_ERR_OK;
      }
    }

    // search free slot in bag
    if (bag == INVENTORY_SLOT_BAG_0) {
      res = CanStoreItem_InInventorySlots(BANK_SLOT_ITEM_START,
                                          BANK_SLOT_ITEM_END, dest, pProto,
                                          count, false, pItem, bag, slot);
      if (res != EQUIP_ERR_OK)
        return res;

      if (count == 0)
        return EQUIP_ERR_OK;
    } else {
      res = CanStoreItem_InBag(bag, dest, pProto, count, false, false, pItem,
                               NULL_BAG, slot);
      if (res != EQUIP_ERR_OK)
        res = CanStoreItem_InBag(bag, dest, pProto, count, false, true, pItem,
                                 NULL_BAG, slot);

      if (res != EQUIP_ERR_OK)
        return res;

      if (count == 0)
        return EQUIP_ERR_OK;
    }
  }

  // not specific bag or have space for partly store only in specific bag

  // search stack for merge to
  if (pProto->GetMaxStackSize() != 1) {
    // in slots
    res = CanStoreItem_InInventorySlots(BANK_SLOT_ITEM_START,
                                        BANK_SLOT_ITEM_END, dest, pProto, count,
                                        true, pItem, bag, slot);
    if (res != EQUIP_ERR_OK)
      return res;

    if (count == 0)
      return EQUIP_ERR_OK;

    // in special bags
    if (pProto->GetBagFamily()) {
      for (uint8 i = BANK_SLOT_BAG_START; i < BANK_SLOT_BAG_END; i++) {
        res = CanStoreItem_InBag(i, dest, pProto, count, true, false, pItem,
                                 bag, slot);
        if (res != EQUIP_ERR_OK)
          continue;

        if (count == 0)
          return EQUIP_ERR_OK;
      }
    }

    for (uint8 i = BANK_SLOT_BAG_START; i < BANK_SLOT_BAG_END; i++) {
      res = CanStoreItem_InBag(i, dest, pProto, count, true, true, pItem, bag,
                               slot);
      if (res != EQUIP_ERR_OK)
        continue;

      if (count == 0)
        return EQUIP_ERR_OK;
    }
  }

  // search free place in special bag
  if (pProto->GetBagFamily()) {
    for (uint8 i = BANK_SLOT_BAG_START; i < BANK_SLOT_BAG_END; i++) {
      res = CanStoreItem_InBag(i, dest, pProto, count, false, false, pItem, bag,
                               slot);
      if (res != EQUIP_ERR_OK)
        continue;

      if (count == 0)
        return EQUIP_ERR_OK;
    }
  }

  // search free space
  res = CanStoreItem_InInventorySlots(BANK_SLOT_ITEM_START, BANK_SLOT_ITEM_END,
                                      dest, pProto, count, false, pItem, bag,
                                      slot);
  if (res != EQUIP_ERR_OK)
    return res;

  if (count == 0)
    return EQUIP_ERR_OK;

  for (uint8 i = BANK_SLOT_BAG_START; i < BANK_SLOT_BAG_END; i++) {
    res = CanStoreItem_InBag(i, dest, pProto, count, false, true, pItem, bag,
                             slot);
    if (res != EQUIP_ERR_OK)
      continue;

    if (count == 0)
      return EQUIP_ERR_OK;
  }
  return EQUIP_ERR_BANK_FULL;
}

InventoryResult Player::CanUseItem(Item *pItem, bool not_loading) const {
  if (pItem) {
    LOG_DEBUG("entities.player.items",
              "Player::CanUseItem: Player '%s' (%s),  Item: %u",
              GetName().c_str(), GetGUID().ToString().c_str(),
              pItem->GetEntry());

    if (!IsAlive() && not_loading)
      return EQUIP_ERR_PLAYER_DEAD;

    // if (isStunned())
    //     return EQUIP_ERR_GENERIC_STUNNED;

    ItemTemplate const *pProto = pItem->GetTemplate();
    if (pProto) {
      if (pItem->IsBindedNotWith(this))
        return EQUIP_ERR_NOT_OWNER;

      InventoryResult res = CanUseItem(pProto);
      if (res != EQUIP_ERR_OK)
        return res;

      if (pItem->GetSkill() != 0) {
        bool allowEquip = false;
        uint32 itemSkill = pItem->GetSkill();
        // Armor that is binded to account can "morph" from plate to mail, etc.
        // if skill is not learned yet.
        if (pProto->GetQuality() == ITEM_QUALITY_HEIRLOOM &&
            pProto->GetClass() == ITEM_CLASS_ARMOR && !HasSkill(itemSkill)) {
          /// @todo when you right-click already equipped item it throws
          /// EQUIP_ERR_PROFICIENCY_NEEDED.
          // In fact it's a visual bug, everything works properly... I need
          // sniffs of operations with binded to account items from off server.

          switch (getClass()) {
          case CLASS_HUNTER:
          case CLASS_SHAMAN:
            allowEquip = (itemSkill == SKILL_MAIL);
            break;
          case CLASS_PALADIN:
          case CLASS_WARRIOR:
            allowEquip = (itemSkill == SKILL_PLATE_MAIL);
            break;
          }
        }
        if (!allowEquip && GetSkillValue(itemSkill) == 0)
          return EQUIP_ERR_PROFICIENCY_NEEDED;
      }

      if (pProto->GetRequiredReputationFaction() &&
          uint32(GetReputationRank(pProto->GetRequiredReputationFaction())) <
              pProto->GetRequiredReputationRank())
        return EQUIP_ERR_CANT_EQUIP_REPUTATION;

      return EQUIP_ERR_OK;
    }
  }
  return EQUIP_ERR_ITEM_NOT_FOUND;
}

InventoryResult Player::CanUseItem(ItemTemplate const *proto) const {
  // Used by group, function NeedBeforeGreed, to know if a prototype can be used
  // by a player

  if (!proto)
    return EQUIP_ERR_ITEM_NOT_FOUND;

  if (((proto->GetFlags2() & ITEM_FLAG2_FACTION_HORDE) && GetTeam() != HORDE) ||
      (((proto->GetFlags2() & ITEM_FLAG2_FACTION_ALLIANCE) &&
        GetTeam() != ALLIANCE)))
    return EQUIP_ERR_CANT_EQUIP_EVER;

  if ((proto->GetAllowableClass() & getClassMask()) == 0 ||
      (proto->GetAllowableRace() & getRaceMask()) == 0)
    return EQUIP_ERR_CANT_EQUIP_EVER;

  if (proto->GetRequiredSkill() != 0) {
    if (GetSkillValue(proto->GetRequiredSkill()) == 0)
      return EQUIP_ERR_PROFICIENCY_NEEDED;
    else if (GetSkillValue(proto->GetRequiredSkill()) <
             proto->GetRequiredSkillRank())
      return EQUIP_ERR_CANT_EQUIP_SKILL;
  }

  if (proto->GetRequiredSpell() != 0 && !HasSpell(proto->GetRequiredSpell()))
    return EQUIP_ERR_PROFICIENCY_NEEDED;

  if (getLevel() < proto->GetRequiredLevel())
    return EQUIP_ERR_CANT_EQUIP_LEVEL_I;

  // If World Event is not active, prevent using event dependant items
  if (proto->GetHolidayID() &&
      !IsHolidayActive((HolidayIds)proto->GetHolidayID()))
    return EQUIP_ERR_CLIENT_LOCKED_OUT;

  // learning (recipes, mounts, pets, etc.)
  if (proto->Effects[0].SpellID == 483 || proto->Effects[0].SpellID == 55884)
    if (HasSpell(proto->Effects[1].SpellID))
      return EQUIP_ERR_INTERNAL_BAG_ERROR;

  return EQUIP_ERR_OK;
}

InventoryResult
Player::CanRollForItemInLFG(ItemTemplate const *proto,
                            WorldObject const *lootedObject) const {
  if (!GetGroup() || !GetGroup()->isLFGGroup())
    return EQUIP_ERR_OK; // not in LFG group

  // check if looted object is inside the lfg dungeon
  Map const *map = lootedObject->GetMap();
  if (!sLFGMgr->inLfgDungeonMap(GetGroup()->GetGUID(), map->GetId(),
                                map->GetDifficulty()))
    return EQUIP_ERR_OK;

  if (!proto)
    return EQUIP_ERR_ITEM_NOT_FOUND;
  // Used by group, function NeedBeforeGreed, to know if a prototype can be used
  // by a player

  const static uint32 item_weapon_skills[MAX_ITEM_SUBCLASS_WEAPON] = {
      SKILL_AXES,
      SKILL_2H_AXES,
      SKILL_BOWS,
      SKILL_GUNS,
      SKILL_MACES,
      SKILL_2H_MACES,
      SKILL_POLEARMS,
      SKILL_SWORDS,
      SKILL_2H_SWORDS,
      0,
      SKILL_STAVES,
      0,
      0,
      SKILL_FIST_WEAPONS,
      0,
      SKILL_DAGGERS,
      SKILL_THROWN,
      SKILL_ASSASSINATION,
      SKILL_CROSSBOWS,
      SKILL_WANDS,
      SKILL_FISHING}; // Copy from function Item::GetSkill()

  if ((proto->GetAllowableClass() & getClassMask()) == 0 ||
      (proto->GetAllowableRace() & getRaceMask()) == 0)
    return EQUIP_ERR_CANT_EQUIP_EVER;

  if (proto->GetRequiredSpell() != 0 && !HasSpell(proto->GetRequiredSpell()))
    return EQUIP_ERR_PROFICIENCY_NEEDED;

  if (proto->GetRequiredSkill() != 0) {
    if (!GetSkillValue(proto->GetRequiredSkill()))
      return EQUIP_ERR_PROFICIENCY_NEEDED;
    else if (GetSkillValue(proto->GetRequiredSkill()) <
             proto->GetRequiredSkillRank())
      return EQUIP_ERR_CANT_EQUIP_SKILL;
  }

  uint8 _class = getClass();

  if (proto->GetClass() == ITEM_CLASS_WEAPON &&
      GetSkillValue(item_weapon_skills[proto->GetSubClass()]) == 0)
    return EQUIP_ERR_PROFICIENCY_NEEDED;

  if (proto->GetClass() == ITEM_CLASS_ARMOR &&
      proto->GetSubClass() > ITEM_SUBCLASS_ARMOR_MISCELLANEOUS &&
      proto->GetSubClass() < ITEM_SUBCLASS_ARMOR_BUCKLER &&
      proto->GetInventoryType() != INVTYPE_CLOAK) {
    if (_class == CLASS_WARRIOR || _class == CLASS_PALADIN ||
        _class == CLASS_DEATH_KNIGHT) {
      if (getLevel() < 40) {
        if (proto->GetSubClass() != ITEM_SUBCLASS_ARMOR_MAIL)
          return EQUIP_ERR_CLIENT_LOCKED_OUT;
      } else if (proto->GetSubClass() != ITEM_SUBCLASS_ARMOR_PLATE)
        return EQUIP_ERR_CLIENT_LOCKED_OUT;
    } else if (_class == CLASS_HUNTER || _class == CLASS_SHAMAN) {
      if (getLevel() < 40) {
        if (proto->GetSubClass() != ITEM_SUBCLASS_ARMOR_LEATHER)
          return EQUIP_ERR_CLIENT_LOCKED_OUT;
      } else if (proto->GetSubClass() != ITEM_SUBCLASS_ARMOR_MAIL)
        return EQUIP_ERR_CLIENT_LOCKED_OUT;
    }

    if (_class == CLASS_ROGUE || _class == CLASS_DRUID)
      if (proto->GetSubClass() != ITEM_SUBCLASS_ARMOR_LEATHER)
        return EQUIP_ERR_CLIENT_LOCKED_OUT;

    if (_class == CLASS_MAGE || _class == CLASS_PRIEST ||
        _class == CLASS_WARLOCK)
      if (proto->GetSubClass() != ITEM_SUBCLASS_ARMOR_CLOTH)
        return EQUIP_ERR_CLIENT_LOCKED_OUT;
  }

  return EQUIP_ERR_OK;
}

// Return stored item (if stored to stack, it can diff. from pItem). And pItem
// ca be deleted in this case.
Item *
Player::StoreNewItem(ItemPosCountVec const &dest, uint32 item, bool update,
                     ItemRandomEnchantmentId const &randomPropertyId /*= {}*/,
                     GuidSet const &allowedLooters) {
  uint32 count = 0;
  for (ItemPosCountVec::const_iterator itr = dest.begin(); itr != dest.end();
       ++itr)
    count += itr->count;

  Item *pItem = Item::CreateItem(item, count, this);
  if (pItem) {
    ItemAddedQuestCheck(item, count);
    UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_RECEIVE_EPIC_ITEM, item,
                              count);
    UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_OWN_ITEM, item, count);
    pItem->SetItemRandomProperties(randomPropertyId);
    pItem = StoreItem(dest, pItem, update);

    if (allowedLooters.size() > 1 &&
        pItem->GetTemplate()->GetMaxStackSize() == 1 && pItem->IsSoulBound()) {
      pItem->SetSoulboundTradeable(allowedLooters);
      pItem->SetUInt32Value(ITEM_FIELD_CREATE_PLAYED_TIME,
                            GetTotalPlayedTime());
      AddTradeableItem(pItem);

      // save data
      std::ostringstream ss;
      GuidSet::const_iterator itr = allowedLooters.begin();
      ss << *itr;
      for (++itr; itr != allowedLooters.end(); ++itr)
        ss << ' ' << itr->GetCounter();

      CharacterDatabasePreparedStatement *stmt =
          CharacterDatabase.GetPreparedStatement(CHAR_INS_ITEM_BOP_TRADE);
      stmt->SetData(0, pItem->GetGUID().GetCounter());
      stmt->SetData(1, ss.str());
      CharacterDatabase.Execute(stmt);
    }
  }
  return pItem;
}

Item *Player::StoreItem(ItemPosCountVec const &dest, Item *pItem, bool update) {
  if (!pItem)
    return nullptr;

  Item *lastItem = pItem;
  for (ItemPosCountVec::const_iterator itr = dest.begin(); itr != dest.end();) {
    uint16 pos = itr->pos;
    uint32 count = itr->count;

    ++itr;

    if (itr == dest.end()) {
      lastItem = _StoreItem(pos, pItem, count, false, update);
      break;
    }

    lastItem = _StoreItem(pos, pItem, count, true, update);
  }
  return lastItem;
}

// Return stored item (if stored to stack, it can diff. from pItem). And pItem
// ca be deleted in this case.
Item *Player::_StoreItem(uint16 pos, Item *pItem, uint32 count, bool clone,
                         bool update) {
  if (!pItem)
    return nullptr;

  uint8 bag = pos >> 8;
  uint8 slot = pos & 255;

  LOG_DEBUG("entities.player.items",
            "Player::_StoreItem: Player '%s' (%s), Bag: %u, Slot: %u, Item: %u "
            "(%s), Count: %u",
            GetName().c_str(), GetGUID().ToString().c_str(), bag, slot,
            pItem->GetEntry(), pItem->GetGUID().ToString().c_str(), count);

  Item *pItem2 = GetItemByPos(bag, slot);

  if (!pItem2) {
    if (clone)
      pItem = pItem->CloneItem(count, this);
    else
      pItem->SetCount(count);

    if (!pItem)
      return nullptr;

    if (pItem->GetTemplate()->GetBonding() == BIND_ON_ACQUIRE ||
        pItem->GetTemplate()->GetBonding() == BIND_QUEST ||
        (pItem->GetTemplate()->GetBonding() == BIND_ON_EQUIP && IsBagPos(pos)))
      pItem->SetBinding(true);

    Bag *pBag = (bag == INVENTORY_SLOT_BAG_0) ? nullptr : GetBagByPos(bag);
    if (!pBag) {
      m_items[slot] = pItem;
      SetGuidValue(PLAYER_FIELD_INV_SLOT_HEAD + (slot * 2), pItem->GetGUID());
      pItem->SetGuidValue(ITEM_FIELD_CONTAINED, GetGUID());
      pItem->SetGuidValue(ITEM_FIELD_OWNER, GetGUID());

      pItem->SetSlot(slot);
      pItem->SetContainer(nullptr);
    } else
      pBag->StoreItem(slot, pItem, update);

    if (IsInWorld() && update) {
      pItem->AddToWorld();
      pItem->SendUpdateToPlayer(this);
    }

    pItem->SetState(ITEM_CHANGED, this);
    if (pBag)
      pBag->SetState(ITEM_CHANGED, this);

    AddEnchantmentDurations(pItem);
    AddItemDurations(pItem);

    if (bag == INVENTORY_SLOT_BAG_0 ||
        (bag >= INVENTORY_SLOT_BAG_START && bag < INVENTORY_SLOT_BAG_END))
      ApplyItemObtainSpells(pItem, true);

    return pItem;
  } else {
    if (pItem2->GetTemplate()->GetBonding() == BIND_ON_ACQUIRE ||
        pItem2->GetTemplate()->GetBonding() == BIND_QUEST ||
        (pItem2->GetTemplate()->GetBonding() == BIND_ON_EQUIP && IsBagPos(pos)))
      pItem2->SetBinding(true);

    pItem2->SetCount(pItem2->GetCount() + count);
    if (IsInWorld() && update)
      pItem2->SendUpdateToPlayer(this);

    if (!clone) {
      // delete item (it not in any slot currently)
      if (IsInWorld() && update) {
        pItem->RemoveFromWorld();
        pItem->DestroyForPlayer(this);
      }

      RemoveEnchantmentDurations(pItem);
      RemoveItemDurations(pItem);

      pItem->SetOwnerGUID(GetGUID()); // prevent error at next SetState in case
                                      // trade/mail/buy from vendor
      pItem->SetNotRefundable(this);
      pItem->ClearSoulboundTradeable(this);
      RemoveTradeableItem(pItem);
      pItem->SetState(ITEM_REMOVED, this);
    }

    AddEnchantmentDurations(pItem2);

    pItem2->SetState(ITEM_CHANGED, this);

    if (bag == INVENTORY_SLOT_BAG_0 ||
        (bag >= INVENTORY_SLOT_BAG_START && bag < INVENTORY_SLOT_BAG_END))
      ApplyItemObtainSpells(pItem2, true);

    return pItem2;
  }
}

Item *Player::EquipNewItem(uint16 pos, uint32 item, bool update) {
  if (Item *pItem = Item::CreateItem(item, 1, this)) {
    ItemAddedQuestCheck(item, 1);
    UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_RECEIVE_EPIC_ITEM, item,
                              1);
    UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_OWN_ITEM, item, 1);
    return EquipItem(pos, pItem, update);
  }

  return nullptr;
}

Item *Player::EquipItem(uint16 pos, Item *pItem, bool update) {
  AddEnchantmentDurations(pItem);
  AddItemDurations(pItem);

  uint8 bag = pos >> 8;
  uint8 slot = pos & 255;

  Item *pItem2 = GetItemByPos(bag, slot);

  if (!pItem2) {
    VisualizeItem(slot, pItem);

    if (IsAlive()) {
      ItemTemplate const *pProto = pItem->GetTemplate();

      // item set bonuses applied only at equip and removed at unequip, and
      // still active for broken items
      if (pProto && pProto->GetItemSet())
        AddItemsSetItem(this, pItem);

      _ApplyItemMods(pItem, slot, true);

      if (pProto && IsInCombat() &&
          (pProto->GetClass() == ITEM_CLASS_WEAPON ||
           pProto->GetInventoryType() == INVTYPE_RELIC) &&
          m_weaponChangeTimer == 0) {
        uint32 cooldownSpell = getClass() == CLASS_ROGUE ? 6123 : 6119;
        SpellInfo const *spellProto = sSpellMgr->GetSpellInfo(cooldownSpell);

        if (!spellProto)
          LOG_ERROR("entities.player",
                    "Player::EquipItem: Weapon switch cooldown spell %u for "
                    "player '%s' (%s) couldn't be found in Spell.dbc",
                    cooldownSpell, GetName().c_str(),
                    GetGUID().ToString().c_str());
        else {
          m_weaponChangeTimer = spellProto->StartRecoveryTime;

          GetSpellHistory()->AddGlobalCooldown(spellProto, m_weaponChangeTimer);
          WorldPacket data;
          GetSpellHistory()->BuildCooldownPacket(
              data, SPELL_COOLDOWN_FLAG_INCLUDE_GCD, cooldownSpell, 0);
          SendDirectMessage(&data);
        }
      }
    }

    if (IsInWorld() && update) {
      pItem->AddToWorld();
      pItem->SendUpdateToPlayer(this);
    }

    ApplyEquipCooldown(pItem);

    // update expertise and armor penetration - passive auras may need it

    if (slot == EQUIPMENT_SLOT_MAINHAND)
      UpdateExpertise(BASE_ATTACK);

    else if (slot == EQUIPMENT_SLOT_OFFHAND)
      UpdateExpertise(OFF_ATTACK);

    UpdateArmorSpecialization();
  } else {
    pItem2->SetCount(pItem2->GetCount() + pItem->GetCount());
    if (IsInWorld() && update)
      pItem2->SendUpdateToPlayer(this);

    // delete item (it not in any slot currently)
    // pItem->DeleteFromDB();
    if (IsInWorld() && update) {
      pItem->RemoveFromWorld();
      pItem->DestroyForPlayer(this);
    }

    RemoveEnchantmentDurations(pItem);
    RemoveItemDurations(pItem);

    pItem->SetOwnerGUID(GetGUID()); // prevent error at next SetState in case
                                    // trade/mail/buy from vendor
    pItem->SetNotRefundable(this);
    pItem->ClearSoulboundTradeable(this);
    RemoveTradeableItem(pItem);
    pItem->SetState(ITEM_REMOVED, this);
    pItem2->SetState(ITEM_CHANGED, this);

    ApplyEquipCooldown(pItem2);

    UpdateArmorSpecialization();

    return pItem2;
  }

  if (slot == EQUIPMENT_SLOT_MAINHAND || slot == EQUIPMENT_SLOT_OFFHAND)
    CheckTitanGripPenalty();

  // only for full equip instead adding to stack
  UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_EQUIP_ITEM,
                            pItem->GetEntry());
  UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_EQUIP_EPIC_ITEM, slot,
                            pItem->GetEntry());
  if (Guild *guild = GetGuild())
    guild->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_OWN_ITEM,
                                     pItem->GetEntry(), 1, 0, nullptr, this);

  return pItem;
}

void Player::QuickEquipItem(uint16 pos, Item *pItem) {
  if (pItem) {
    AddEnchantmentDurations(pItem);
    AddItemDurations(pItem);

    uint8 slot = pos & 255;
    VisualizeItem(slot, pItem);

    if (IsInWorld()) {
      pItem->AddToWorld();
      pItem->SendUpdateToPlayer(this);
    }

    if (slot == EQUIPMENT_SLOT_MAINHAND || slot == EQUIPMENT_SLOT_OFFHAND)
      CheckTitanGripPenalty();

    UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_EQUIP_ITEM,
                              pItem->GetEntry());
    UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_EQUIP_EPIC_ITEM, slot,
                              pItem->GetEntry());
  }
}

void Player::SetVisibleItemSlot(uint8 slot, Item *pItem) {
  if (pItem) {
    SetUInt32Value(PLAYER_VISIBLE_ITEM_1_ENTRYID + (slot * 2),
                   pItem->GetVisibleEntry());
    SetUInt16Value(PLAYER_VISIBLE_ITEM_1_ENCHANTMENT + (slot * 2), 0,
                   pItem->GetEnchantmentId(PERM_ENCHANTMENT_SLOT));
    SetUInt16Value(PLAYER_VISIBLE_ITEM_1_ENCHANTMENT + (slot * 2), 1,
                   pItem->GetEnchantmentId(TEMP_ENCHANTMENT_SLOT));
  } else {
    SetUInt32Value(PLAYER_VISIBLE_ITEM_1_ENTRYID + (slot * 2), 0);
    SetUInt32Value(PLAYER_VISIBLE_ITEM_1_ENCHANTMENT + (slot * 2), 0);
  }
}

void Player::VisualizeItem(uint8 slot, Item *pItem) {
  if (!pItem)
    return;

  // check also BIND_ON_ACQUIRE and BIND_QUEST for .additem or .additemset case
  // by GM (not binded at adding to inventory)
  if (pItem->GetTemplate()->GetBonding() == BIND_ON_EQUIP ||
      pItem->GetTemplate()->GetBonding() == BIND_ON_ACQUIRE ||
      pItem->GetTemplate()->GetBonding() == BIND_QUEST)
    pItem->SetBinding(true);

  LOG_DEBUG("entities.player.items",
            "Player::SetVisibleItemSlot: Player '%s' (%s), Slot: %u, Item: %u",
            GetName().c_str(), GetGUID().ToString().c_str(), slot,
            pItem->GetEntry());

  m_items[slot] = pItem;
  SetGuidValue(PLAYER_FIELD_INV_SLOT_HEAD + (slot * 2), pItem->GetGUID());
  pItem->SetGuidValue(ITEM_FIELD_CONTAINED, GetGUID());
  pItem->SetGuidValue(ITEM_FIELD_OWNER, GetGUID());
  pItem->SetSlot(slot);
  pItem->SetContainer(nullptr);

  if (slot < EQUIPMENT_SLOT_END)
    SetVisibleItemSlot(slot, pItem);

  pItem->SetState(ITEM_CHANGED, this);
}

Item *Player::BankItem(ItemPosCountVec const &dest, Item *pItem, bool update) {
  return StoreItem(dest, pItem, update);
}

void Player::RemoveItem(uint8 bag, uint8 slot, bool update) {
  // note: removeitem does not actually change the item
  // it only takes the item out of storage temporarily
  // note2: if removeitem is to be used for delinking
  // the item must be removed from the player's updatequeue

  Item *pItem = GetItemByPos(bag, slot);
  if (pItem) {
    LOG_DEBUG(
        "entities.player.items",
        "Player::RemoveItem: Player '%s' (%s), Bag: %u, Slot: %u, Item: %u",
        GetName().c_str(), GetGUID().ToString().c_str(), bag, slot,
        pItem->GetEntry());

    RemoveEnchantmentDurations(pItem);
    RemoveItemDurations(pItem);
    RemoveTradeableItem(pItem);

    if (bag == INVENTORY_SLOT_BAG_0) {
      if (slot < INVENTORY_SLOT_BAG_END) {
        ItemTemplate const *pProto = pItem->GetTemplate();
        // item set bonuses applied only at equip and removed at unequip, and
        // still active for broken items

        if (pProto && pProto->GetItemSet())
          RemoveItemsSetItem(this, pProto);

        _ApplyItemMods(pItem, slot, false, update);

        if (slot == EQUIPMENT_SLOT_MAINHAND || slot == EQUIPMENT_SLOT_OFFHAND)
          UpdateExpertise(OFF_ATTACK);
      }

      m_items[slot] = nullptr;
      SetGuidValue(PLAYER_FIELD_INV_SLOT_HEAD + (slot * 2), ObjectGuid::Empty);

      if (slot < EQUIPMENT_SLOT_END) {
        SetVisibleItemSlot(slot, nullptr);
        if (slot == EQUIPMENT_SLOT_MAINHAND || slot == EQUIPMENT_SLOT_OFFHAND)
          CheckTitanGripPenalty();

        if (slot == EQUIPMENT_SLOT_MAINHAND) {
          // clear main hand only enchantments
          for (uint32 enchantSlot = 0; enchantSlot < MAX_ENCHANTMENT_SLOT;
               ++enchantSlot)
            if (SpellItemEnchantmentEntry const *enchantment =
                    sSpellItemEnchantmentStore.LookupEntry(
                        pItem->GetEnchantmentId(EnchantmentSlot(enchantSlot))))
              if (enchantment->GetFlags().HasFlag(
                      SpellItemEnchantmentFlags::MainhandOnly))
                pItem->ClearEnchantment(EnchantmentSlot(enchantSlot));
        }
      }
    } else if (Bag *pBag = GetBagByPos(bag))
      pBag->RemoveItem(slot, update);

    pItem->SetGuidValue(ITEM_FIELD_CONTAINED, ObjectGuid::Empty);
    // pItem->SetUInt64Value(ITEM_FIELD_OWNER, 0); not clear owner at remove (it
    // will be set at store). This used in mail and auction code
    pItem->SetSlot(NULL_SLOT);
    UpdateArmorSpecialization();
    if (IsInWorld() && update)
      pItem->SendUpdateToPlayer(this);
  }
}

// Common operation need to remove item from inventory without delete in trade,
// auction, guild bank, mail....
void Player::MoveItemFromInventory(uint8 bag, uint8 slot, bool update) {
  if (Item *it = GetItemByPos(bag, slot)) {
    ItemRemovedQuestCheck(it->GetEntry(), it->GetCount());
    RemoveItem(bag, slot, update);
    it->SetNotRefundable(this, false);
    RemoveItemFromUpdateQueueOf(it, this);
    if (it->IsInWorld()) {
      it->RemoveFromWorld();
      it->DestroyForPlayer(this);
    }
  }
}

// Common operation need to add item from inventory without delete in trade,
// guild bank, mail....
void Player::MoveItemToInventory(ItemPosCountVec const &dest, Item *pItem,
                                 bool update, bool in_characterInventoryDB) {
  // update quest counters
  ItemAddedQuestCheck(pItem->GetEntry(), pItem->GetCount());
  UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_RECEIVE_EPIC_ITEM,
                            pItem->GetEntry(), pItem->GetCount());

  // store item
  Item *pLastItem = StoreItem(dest, pItem, update);

  // only set if not merged to existing stack (pItem can be deleted already but
  // we can compare pointers any way)
  if (pLastItem == pItem) {
    // update owner for last item (this can be original item with wrong owner
    if (pLastItem->GetOwnerGUID() != GetGUID())
      pLastItem->SetOwnerGUID(GetGUID());

    // if this original item then it need create record in inventory
    // in case trade we already have item in other player inventory
    pLastItem->SetState(in_characterInventoryDB ? ITEM_CHANGED : ITEM_NEW,
                        this);

    if (pLastItem->HasFlag(ITEM_FIELD_FLAGS, ITEM_FIELD_FLAG_BOP_TRADEABLE))
      AddTradeableItem(pLastItem);
  }
}

void Player::DestroyItem(uint8 bag, uint8 slot, bool update) {
  Item *pItem = GetItemByPos(bag, slot);
  if (pItem) {
    LOG_DEBUG(
        "entities.player.items",
        "Player::DestroyItem: Player '%s' (%s), Bag: %u, Slot: %u, Item: %u",
        GetName().c_str(), GetGUID().ToString().c_str(), bag, slot,
        pItem->GetEntry());
    // Also remove all contained items if the item is a bag.
    // This if () prevents item saving crashes if the condition for a bag to be
    // empty before being destroyed was bypassed somehow.
    if (pItem->IsNotEmptyBag())
      for (uint8 i = 0; i < MAX_BAG_SIZE; ++i)
        DestroyItem(slot, i, update);

    if (pItem->HasFlag(ITEM_FIELD_FLAGS, ITEM_FIELD_FLAG_WRAPPED)) {
      CharacterDatabasePreparedStatement *stmt =
          CharacterDatabase.GetPreparedStatement(CHAR_DEL_GIFT);

      stmt->SetData(0, pItem->GetGUID().GetCounter());

      CharacterDatabase.Execute(stmt);
    }

    RemoveEnchantmentDurations(pItem);
    RemoveItemDurations(pItem);

    pItem->SetNotRefundable(this);
    pItem->ClearSoulboundTradeable(this);
    RemoveTradeableItem(pItem);

    ApplyItemObtainSpells(pItem, false);

    ItemRemovedQuestCheck(pItem->GetEntry(), pItem->GetCount());
    sScriptMgr->OnItemRemove(this, pItem);

    ItemTemplate const *pProto = pItem->GetTemplate();
    if (bag == INVENTORY_SLOT_BAG_0) {
      SetGuidValue(PLAYER_FIELD_INV_SLOT_HEAD + (slot * 2), ObjectGuid::Empty);

      // equipment and equipped bags can have applied bonuses
      if (slot < INVENTORY_SLOT_BAG_END) {
        // item set bonuses applied only at equip and removed at unequip, and
        // still active for broken items
        if (pProto->GetItemSet())
          RemoveItemsSetItem(this, pProto);

        _ApplyItemMods(pItem, slot, false);
      }

      if (slot < EQUIPMENT_SLOT_END) {
        if (slot == EQUIPMENT_SLOT_MAINHAND)
          UpdateExpertise(BASE_ATTACK);
        else if (slot == EQUIPMENT_SLOT_OFFHAND)
          UpdateExpertise(OFF_ATTACK);

        // equipment visual show
        SetVisibleItemSlot(slot, nullptr);
      }

      m_items[slot] = nullptr;
    } else if (Bag *pBag = GetBagByPos(bag))
      pBag->RemoveItem(slot, update);

    // Delete rolled money / loot from db.
    // MUST be done before RemoveFromWorld() or GetTemplate() fails
    if (pProto->GetFlags() & ITEM_FLAG_HAS_LOOT)
      sLootItemStorage->RemoveStoredLootForContainer(
          pItem->GetGUID().GetCounter());

    if (IsInWorld() && update) {
      pItem->RemoveFromWorld();
      pItem->DestroyForPlayer(this);
    }

    // pItem->SetOwnerGUID(0);
    pItem->SetGuidValue(ITEM_FIELD_CONTAINED, ObjectGuid::Empty);
    pItem->SetSlot(NULL_SLOT);
    pItem->SetState(ITEM_REMOVED, this);
  }
}

void Player::DestroyItemCount(uint32 itemEntry, uint32 count, bool update,
                              bool unequip_check) {
  LOG_DEBUG("entities.player.items",
            "Player::DestroyItemCount: Player '%s' (%s), Item: %u, Count: %u",
            GetName().c_str(), GetGUID().ToString().c_str(), itemEntry, count);
  uint32 remcount = 0;

  // in inventory
  for (uint8 i = INVENTORY_SLOT_ITEM_START; i < INVENTORY_SLOT_ITEM_END; ++i) {
    if (Item *item = GetItemByPos(INVENTORY_SLOT_BAG_0, i)) {
      if (item->GetEntry() == itemEntry && !item->IsInTrade()) {
        if (item->GetCount() + remcount <= count) {
          // all items in inventory can unequipped
          remcount += item->GetCount();
          DestroyItem(INVENTORY_SLOT_BAG_0, i, update);

          if (remcount >= count)
            return;
        } else {
          ItemRemovedQuestCheck(item->GetEntry(), count - remcount);
          item->SetCount(item->GetCount() - count + remcount);
          if (IsInWorld() && update)
            item->SendUpdateToPlayer(this);
          item->SetState(ITEM_CHANGED, this);
          return;
        }
      }
    }
  }

  // in inventory bags
  for (uint8 i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; i++) {
    if (Bag *bag = GetBagByPos(i)) {
      for (uint32 j = 0; j < bag->GetBagSize(); j++) {
        if (Item *item = bag->GetItemByPos(j)) {
          if (item->GetEntry() == itemEntry && !item->IsInTrade()) {
            // all items in bags can be unequipped
            if (item->GetCount() + remcount <= count) {
              remcount += item->GetCount();
              DestroyItem(i, j, update);

              if (remcount >= count)
                return;
            } else {
              ItemRemovedQuestCheck(item->GetEntry(), count - remcount);
              item->SetCount(item->GetCount() - count + remcount);
              if (IsInWorld() && update)
                item->SendUpdateToPlayer(this);
              item->SetState(ITEM_CHANGED, this);
              return;
            }
          }
        }
      }
    }
  }

  // in equipment and bag list
  for (uint8 i = EQUIPMENT_SLOT_START; i < INVENTORY_SLOT_BAG_END; i++) {
    if (Item *item = GetItemByPos(INVENTORY_SLOT_BAG_0, i)) {
      if (item->GetEntry() == itemEntry && !item->IsInTrade()) {
        if (item->GetCount() + remcount <= count) {
          if (!unequip_check || CanUnequipItem(INVENTORY_SLOT_BAG_0 << 8 | i,
                                               false) == EQUIP_ERR_OK) {
            remcount += item->GetCount();
            DestroyItem(INVENTORY_SLOT_BAG_0, i, update);

            if (remcount >= count)
              return;
          }
        } else {
          ItemRemovedQuestCheck(item->GetEntry(), count - remcount);
          item->SetCount(item->GetCount() - count + remcount);
          if (IsInWorld() && update)
            item->SendUpdateToPlayer(this);
          item->SetState(ITEM_CHANGED, this);
          return;
        }
      }
    }
  }

  // in bank
  for (uint8 i = BANK_SLOT_ITEM_START; i < BANK_SLOT_ITEM_END; i++) {
    if (Item *item = GetItemByPos(INVENTORY_SLOT_BAG_0, i)) {
      if (item->GetEntry() == itemEntry && !item->IsInTrade()) {
        if (item->GetCount() + remcount <= count) {
          remcount += item->GetCount();
          DestroyItem(INVENTORY_SLOT_BAG_0, i, update);
          if (remcount >= count)
            return;
        } else {
          ItemRemovedQuestCheck(item->GetEntry(), count - remcount);
          item->SetCount(item->GetCount() - count + remcount);
          if (IsInWorld() && update)
            item->SendUpdateToPlayer(this);
          item->SetState(ITEM_CHANGED, this);
          return;
        }
      }
    }
  }

  // in bank bags
  for (uint8 i = BANK_SLOT_BAG_START; i < BANK_SLOT_BAG_END; i++) {
    if (Bag *bag = GetBagByPos(i)) {
      for (uint32 j = 0; j < bag->GetBagSize(); j++) {
        if (Item *item = bag->GetItemByPos(j)) {
          if (item->GetEntry() == itemEntry && !item->IsInTrade()) {
            // all items in bags can be unequipped
            if (item->GetCount() + remcount <= count) {
              remcount += item->GetCount();
              DestroyItem(i, j, update);

              if (remcount >= count)
                return;
            } else {
              ItemRemovedQuestCheck(item->GetEntry(), count - remcount);
              item->SetCount(item->GetCount() - count + remcount);
              if (IsInWorld() && update)
                item->SendUpdateToPlayer(this);
              item->SetState(ITEM_CHANGED, this);
              return;
            }
          }
        }
      }
    }
  }

  // in bank bag list
  for (uint8 i = BANK_SLOT_BAG_START; i < BANK_SLOT_BAG_END; i++) {
    if (Item *item = GetItemByPos(INVENTORY_SLOT_BAG_0, i)) {
      if (item->GetEntry() == itemEntry && !item->IsInTrade()) {
        if (item->GetCount() + remcount <= count) {
          if (!unequip_check || CanUnequipItem(INVENTORY_SLOT_BAG_0 << 8 | i,
                                               false) == EQUIP_ERR_OK) {
            remcount += item->GetCount();
            DestroyItem(INVENTORY_SLOT_BAG_0, i, update);
            if (remcount >= count)
              return;
          }
        } else {
          ItemRemovedQuestCheck(item->GetEntry(), count - remcount);
          item->SetCount(item->GetCount() - count + remcount);
          if (IsInWorld() && update)
            item->SendUpdateToPlayer(this);
          item->SetState(ITEM_CHANGED, this);
          return;
        }
      }
    }
  }
}

void Player::DestroyZoneLimitedItem(bool update, uint32 new_zone) {
  LOG_DEBUG("entities.player.items",
            "Player::DestroyZoneLimitedItem: In map %u and area %u for player "
            "'%s' (%s)",
            GetMapId(), new_zone, GetName().c_str(),
            GetGUID().ToString().c_str());

  // in inventory
  for (uint8 i = INVENTORY_SLOT_ITEM_START; i < INVENTORY_SLOT_ITEM_END; i++)
    if (Item *pItem = GetItemByPos(INVENTORY_SLOT_BAG_0, i))
      if (pItem->IsLimitedToAnotherMapOrZone(GetMapId(), new_zone))
        DestroyItem(INVENTORY_SLOT_BAG_0, i, update);

  // in inventory bags
  for (uint8 i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; i++)
    if (Bag *pBag = GetBagByPos(i))
      for (uint32 j = 0; j < pBag->GetBagSize(); j++)
        if (Item *pItem = pBag->GetItemByPos(j))
          if (pItem->IsLimitedToAnotherMapOrZone(GetMapId(), new_zone))
            DestroyItem(i, j, update);

  // in equipment and bag list
  for (uint8 i = EQUIPMENT_SLOT_START; i < INVENTORY_SLOT_BAG_END; i++)
    if (Item *pItem = GetItemByPos(INVENTORY_SLOT_BAG_0, i))
      if (pItem->IsLimitedToAnotherMapOrZone(GetMapId(), new_zone))
        DestroyItem(INVENTORY_SLOT_BAG_0, i, update);
}

void Player::DestroyConjuredItems(bool update) {
  // used when entering arena
  // destroys all conjured items
  LOG_DEBUG("entities.player.items",
            "Player::DestroyConjuredItems: Player '%s' (%s)", GetName().c_str(),
            GetGUID().ToString().c_str());

  // in inventory
  for (uint8 i = INVENTORY_SLOT_ITEM_START; i < INVENTORY_SLOT_ITEM_END; i++)
    if (Item *pItem = GetItemByPos(INVENTORY_SLOT_BAG_0, i))
      if (pItem->IsConjuredConsumable())
        DestroyItem(INVENTORY_SLOT_BAG_0, i, update);

  // in inventory bags
  for (uint8 i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; i++)
    if (Bag *pBag = GetBagByPos(i))
      for (uint32 j = 0; j < pBag->GetBagSize(); j++)
        if (Item *pItem = pBag->GetItemByPos(j))
          if (pItem->IsConjuredConsumable())
            DestroyItem(i, j, update);

  // in equipment and bag list
  for (uint8 i = EQUIPMENT_SLOT_START; i < INVENTORY_SLOT_BAG_END; i++)
    if (Item *pItem = GetItemByPos(INVENTORY_SLOT_BAG_0, i))
      if (pItem->IsConjuredConsumable())
        DestroyItem(INVENTORY_SLOT_BAG_0, i, update);
}

Item *Player::GetItemByEntry(uint32 entry) const {
  // in inventory
  for (uint8 i = INVENTORY_SLOT_ITEM_START; i < INVENTORY_SLOT_ITEM_END; ++i)
    if (Item *pItem = GetItemByPos(INVENTORY_SLOT_BAG_0, i))
      if (pItem->GetEntry() == entry)
        return pItem;

  for (uint8 i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; ++i)
    if (Bag *pBag = GetBagByPos(i))
      for (uint32 j = 0; j < pBag->GetBagSize(); ++j)
        if (Item *pItem = pBag->GetItemByPos(j))
          if (pItem->GetEntry() == entry)
            return pItem;

  for (uint8 i = EQUIPMENT_SLOT_START; i < INVENTORY_SLOT_BAG_END; ++i)
    if (Item *pItem = GetItemByPos(INVENTORY_SLOT_BAG_0, i))
      if (pItem->GetEntry() == entry)
        return pItem;

  return nullptr;
}

void Player::DestroyItemCount(Item *pItem, uint32 &count, bool update) {
  if (!pItem)
    return;

  LOG_DEBUG("entities.player.items",
            "Player::DestroyItemCount: Player '%s' (%s), Item (%s, Entry: %u), "
            "Count: %u",
            GetName().c_str(), GetGUID().ToString().c_str(),
            pItem->GetGUID().ToString().c_str(), pItem->GetEntry(), count);

  if (pItem->GetCount() <= count) {
    count -= pItem->GetCount();

    DestroyItem(pItem->GetBagSlot(), pItem->GetSlot(), update);
  } else {
    ItemRemovedQuestCheck(pItem->GetEntry(), count);
    pItem->SetCount(pItem->GetCount() - count);
    count = 0;
    if (IsInWorld() && update)
      pItem->SendUpdateToPlayer(this);
    pItem->SetState(ITEM_CHANGED, this);
  }
}

void Player::SplitItem(uint16 src, uint16 dst, uint32 count) {
  uint8 srcbag = src >> 8;
  uint8 srcslot = src & 255;

  uint8 dstbag = dst >> 8;
  uint8 dstslot = dst & 255;

  Item *pSrcItem = GetItemByPos(srcbag, srcslot);
  if (!pSrcItem) {
    SendEquipError(EQUIP_ERR_ITEM_NOT_FOUND, pSrcItem, nullptr);
    return;
  }

  if (pSrcItem->m_lootGenerated) // prevent split looting item (item
  {
    // best error message found for attempting to split while looting
    SendEquipError(EQUIP_ERR_SPLIT_FAILED, pSrcItem, nullptr);
    return;
  }

  // not let split all items (can be only at cheating)
  if (pSrcItem->GetCount() == count) {
    SendEquipError(EQUIP_ERR_SPLIT_FAILED, pSrcItem, nullptr);
    return;
  }

  // not let split more existing items (can be only at cheating)
  if (pSrcItem->GetCount() < count) {
    SendEquipError(EQUIP_ERR_TOO_FEW_TO_SPLIT, pSrcItem, nullptr);
    return;
  }

  //! If trading
  if (TradeData *tradeData = GetTradeData()) {
    //! If current item is in trade window (only possible with packet spoofing -
    //! silent return)
    if (tradeData->GetTradeSlotForItem(pSrcItem->GetGUID()) !=
        TRADE_SLOT_INVALID)
      return;
  }

  LOG_DEBUG("entities.player.items",
            "Player::SplitItem: Player '%s' (%s), Bag: %u, Slot: %u, Item: %u, "
            "Count: %u",
            GetName().c_str(), GetGUID().ToString().c_str(), dstbag, dstslot,
            pSrcItem->GetEntry(), count);
  Item *pNewItem = pSrcItem->CloneItem(count, this);
  if (!pNewItem) {
    SendEquipError(EQUIP_ERR_ITEM_NOT_FOUND, pSrcItem, nullptr);
    return;
  }

  if (IsInventoryPos(dst)) {
    // change item amount before check (for unique max count check)
    pSrcItem->SetCount(pSrcItem->GetCount() - count);

    ItemPosCountVec dest;
    InventoryResult msg = CanStoreItem(dstbag, dstslot, dest, pNewItem, false);
    if (msg != EQUIP_ERR_OK) {
      delete pNewItem;
      pSrcItem->SetCount(pSrcItem->GetCount() + count);
      SendEquipError(msg, pSrcItem, nullptr);
      return;
    }

    if (IsInWorld())
      pSrcItem->SendUpdateToPlayer(this);
    pSrcItem->SetState(ITEM_CHANGED, this);
    StoreItem(dest, pNewItem, true);
  } else if (IsBankPos(dst)) {
    // change item amount before check (for unique max count check)
    pSrcItem->SetCount(pSrcItem->GetCount() - count);

    ItemPosCountVec dest;
    InventoryResult msg = CanBankItem(dstbag, dstslot, dest, pNewItem, false);
    if (msg != EQUIP_ERR_OK) {
      delete pNewItem;
      pSrcItem->SetCount(pSrcItem->GetCount() + count);
      SendEquipError(msg, pSrcItem, nullptr);
      return;
    }

    if (IsInWorld())
      pSrcItem->SendUpdateToPlayer(this);
    pSrcItem->SetState(ITEM_CHANGED, this);
    BankItem(dest, pNewItem, true);
  } else if (IsEquipmentPos(dst)) {
    // change item amount before check (for unique max count check), provide
    // space for splitted items
    pSrcItem->SetCount(pSrcItem->GetCount() - count);

    uint16 dest;
    InventoryResult msg = CanEquipItem(dstslot, dest, pNewItem, false);
    if (msg != EQUIP_ERR_OK) {
      delete pNewItem;
      pSrcItem->SetCount(pSrcItem->GetCount() + count);
      SendEquipError(msg, pSrcItem, nullptr);
      return;
    }

    if (IsInWorld())
      pSrcItem->SendUpdateToPlayer(this);
    pSrcItem->SetState(ITEM_CHANGED, this);
    EquipItem(dest, pNewItem, true);
    AutoUnequipOffhandIfNeed();
  }
}

void Player::SwapItem(uint16 src, uint16 dst) {
  uint8 srcbag = src >> 8;
  uint8 srcslot = src & 255;

  uint8 dstbag = dst >> 8;
  uint8 dstslot = dst & 255;

  Item *pSrcItem = GetItemByPos(srcbag, srcslot);
  Item *pDstItem = GetItemByPos(dstbag, dstslot);

  if (!pSrcItem)
    return;

  LOG_DEBUG("entities.player.items",
            "Player::SwapItem: Player '%s' (%s), Bag: %u, Slot: %u, Item: %u",
            GetName().c_str(), GetGUID().ToString().c_str(), dstbag, dstslot,
            pSrcItem->GetEntry());

  if (!IsAlive()) {
    SendEquipError(EQUIP_ERR_PLAYER_DEAD, pSrcItem, pDstItem);
    return;
  }

  // SRC checks

  // check unequip potability for equipped items and bank bags
  if (IsEquipmentPos(src) || IsBagPos(src)) {
    // bags can be swapped with empty bag slots, or with empty bag (items move
    // possibility checked later)
    InventoryResult msg =
        CanUnequipItem(src, !IsBagPos(src) || IsBagPos(dst) ||
                                (pDstItem && pDstItem->ToBag() &&
                                 pDstItem->ToBag()->IsEmpty()));
    if (msg != EQUIP_ERR_OK) {
      SendEquipError(msg, pSrcItem, pDstItem);
      return;
    }
  }

  // prevent put equipped/bank bag in self
  if (IsBagPos(src) && srcslot == dstbag) {
    SendEquipError(EQUIP_ERR_BAG_IN_BAG, pSrcItem, pDstItem);
    return;
  }

  // prevent equipping bag in the same slot from its inside
  if (IsBagPos(dst) && srcbag == dstslot) {
    SendEquipError(EQUIP_ERR_CANT_SWAP, pSrcItem, pDstItem);
    return;
  }

  // DST checks

  if (pDstItem) {
    // check unequip potability for equipped items and bank bags
    if (IsEquipmentPos(dst) || IsBagPos(dst)) {
      // bags can be swapped with empty bag slots, or with empty bag (items move
      // possibility checked later)
      InventoryResult msg = CanUnequipItem(
          dst, !IsBagPos(dst) || IsBagPos(src) ||
                   (pSrcItem->ToBag() && pSrcItem->ToBag()->IsEmpty()));
      if (msg != EQUIP_ERR_OK) {
        SendEquipError(msg, pSrcItem, pDstItem);
        return;
      }
    }
  }

  // NOW this is or item move (swap with empty), or swap with another item
  // (including bags in bag possitions) or swap empty bag with another empty or
  // not empty bag (with items exchange)

  // Move case
  if (!pDstItem) {
    if (IsInventoryPos(dst)) {
      ItemPosCountVec dest;
      InventoryResult msg =
          CanStoreItem(dstbag, dstslot, dest, pSrcItem, false);
      if (msg != EQUIP_ERR_OK) {
        SendEquipError(msg, pSrcItem, nullptr);
        return;
      }

      RemoveItem(srcbag, srcslot, true);
      StoreItem(dest, pSrcItem, true);
      if (IsBankPos(src))
        ItemAddedQuestCheck(pSrcItem->GetEntry(), pSrcItem->GetCount());
    } else if (IsBankPos(dst)) {
      ItemPosCountVec dest;
      InventoryResult msg = CanBankItem(dstbag, dstslot, dest, pSrcItem, false);
      if (msg != EQUIP_ERR_OK) {
        SendEquipError(msg, pSrcItem, nullptr);
        return;
      }

      RemoveItem(srcbag, srcslot, true);
      BankItem(dest, pSrcItem, true);
      ItemRemovedQuestCheck(pSrcItem->GetEntry(), pSrcItem->GetCount());
    } else if (IsEquipmentPos(dst)) {
      uint16 dest;
      InventoryResult msg = CanEquipItem(dstslot, dest, pSrcItem, false);
      if (msg != EQUIP_ERR_OK) {
        SendEquipError(msg, pSrcItem, nullptr);
        return;
      }

      RemoveItem(srcbag, srcslot, true);
      EquipItem(dest, pSrcItem, true);
      AutoUnequipOffhandIfNeed();
    }

    return;
  }

  // attempt merge to / fill target item
  if (!pSrcItem->IsBag() && !pDstItem->IsBag()) {
    InventoryResult msg;
    ItemPosCountVec sDest;
    uint16 eDest = 0;
    if (IsInventoryPos(dst))
      msg = CanStoreItem(dstbag, dstslot, sDest, pSrcItem, false);
    else if (IsBankPos(dst))
      msg = CanBankItem(dstbag, dstslot, sDest, pSrcItem, false);
    else if (IsEquipmentPos(dst))
      msg = CanEquipItem(dstslot, eDest, pSrcItem, false);
    else
      return;

    // can be merge/fill
    if (msg == EQUIP_ERR_OK) {
      if (pSrcItem->GetCount() + pDstItem->GetCount() <=
          pSrcItem->GetTemplate()->GetMaxStackSize()) {
        RemoveItem(srcbag, srcslot, true);

        if (IsInventoryPos(dst))
          StoreItem(sDest, pSrcItem, true);
        else if (IsBankPos(dst))
          BankItem(sDest, pSrcItem, true);
        else if (IsEquipmentPos(dst)) {
          EquipItem(eDest, pSrcItem, true);
          AutoUnequipOffhandIfNeed();
        }
      } else {
        pSrcItem->SetCount(pSrcItem->GetCount() + pDstItem->GetCount() -
                           pSrcItem->GetTemplate()->GetMaxStackSize());
        pDstItem->SetCount(pSrcItem->GetTemplate()->GetMaxStackSize());
        pSrcItem->SetState(ITEM_CHANGED, this);
        pDstItem->SetState(ITEM_CHANGED, this);
        if (IsInWorld()) {
          pSrcItem->SendUpdateToPlayer(this);
          pDstItem->SendUpdateToPlayer(this);
        }
      }
      SendRefundInfo(pDstItem);
      return;
    }
  }

  // impossible merge/fill, do real swap
  InventoryResult msg = EQUIP_ERR_OK;

  // check src->dest move possibility
  ItemPosCountVec sDest;
  uint16 eDest = 0;
  if (IsInventoryPos(dst))
    msg = CanStoreItem(dstbag, dstslot, sDest, pSrcItem, true);
  else if (IsBankPos(dst))
    msg = CanBankItem(dstbag, dstslot, sDest, pSrcItem, true);
  else if (IsEquipmentPos(dst)) {
    msg = CanEquipItem(dstslot, eDest, pSrcItem, true);
    if (msg == EQUIP_ERR_OK)
      msg = CanUnequipItem(eDest, true);
  }

  if (msg != EQUIP_ERR_OK) {
    SendEquipError(msg, pSrcItem, pDstItem);
    return;
  }

  // check dest->src move possibility
  ItemPosCountVec sDest2;
  uint16 eDest2 = 0;
  if (IsInventoryPos(src))
    msg = CanStoreItem(srcbag, srcslot, sDest2, pDstItem, true);
  else if (IsBankPos(src))
    msg = CanBankItem(srcbag, srcslot, sDest2, pDstItem, true);
  else if (IsEquipmentPos(src)) {
    msg = CanEquipItem(srcslot, eDest2, pDstItem, true);
    if (msg == EQUIP_ERR_OK)
      msg = CanUnequipItem(eDest2, true);
  }

  if (msg != EQUIP_ERR_OK) {
    SendEquipError(msg, pDstItem, pSrcItem);
    return;
  }

  // Check bag swap with item exchange (one from empty in not bag possition
  // (equipped (not possible in fact) or store)
  if (Bag *srcBag = pSrcItem->ToBag()) {
    if (Bag *dstBag = pDstItem->ToBag()) {
      Bag *emptyBag = nullptr;
      Bag *fullBag = nullptr;
      if (srcBag->IsEmpty() && !IsBagPos(src)) {
        emptyBag = srcBag;
        fullBag = dstBag;
      } else if (dstBag->IsEmpty() && !IsBagPos(dst)) {
        emptyBag = dstBag;
        fullBag = srcBag;
      }

      // bag swap (with items exchange) case
      if (emptyBag && fullBag) {
        ItemTemplate const *emptyProto = emptyBag->GetTemplate();

        uint32 count = 0;

        for (uint32 i = 0; i < fullBag->GetBagSize(); ++i) {
          Item *bagItem = fullBag->GetItemByPos(i);
          if (!bagItem)
            continue;

          ItemTemplate const *bagItemProto = bagItem->GetTemplate();
          if (!bagItemProto || !ItemCanGoIntoBag(bagItemProto, emptyProto)) {
            // one from items not go to empty target bag
            SendEquipError(EQUIP_ERR_BAG_IN_BAG, pSrcItem, pDstItem);
            return;
          }

          ++count;
        }

        if (count > emptyBag->GetBagSize()) {
          // too small targeted bag
          SendEquipError(EQUIP_ERR_CANT_SWAP, pSrcItem, pDstItem);
          return;
        }

        // Items swap
        count = 0; // will pos in new bag
        for (uint32 i = 0; i < fullBag->GetBagSize(); ++i) {
          Item *bagItem = fullBag->GetItemByPos(i);
          if (!bagItem)
            continue;

          fullBag->RemoveItem(i, true);
          emptyBag->StoreItem(count, bagItem, true);
          bagItem->SetState(ITEM_CHANGED, this);

          ++count;
        }
      }
    }
  }

  // now do moves, remove...
  RemoveItem(dstbag, dstslot, false);
  RemoveItem(srcbag, srcslot, false);

  // add to dest
  if (IsInventoryPos(dst))
    StoreItem(sDest, pSrcItem, true);
  else if (IsBankPos(dst))
    BankItem(sDest, pSrcItem, true);
  else if (IsEquipmentPos(dst))
    EquipItem(eDest, pSrcItem, true);

  // add to src
  if (IsInventoryPos(src))
    StoreItem(sDest2, pDstItem, true);
  else if (IsBankPos(src))
    BankItem(sDest2, pDstItem, true);
  else if (IsEquipmentPos(src))
    EquipItem(eDest2, pDstItem, true);

  // if inventory item was moved, check if we can remove dependent auras,
  // because they were not removed in Player::RemoveItem (update was set to
  // false) do this after swaps are done, we pass nullptr because both weapons
  // could be swapped and none of them should be ignored
  if ((srcbag == INVENTORY_SLOT_BAG_0 && srcslot < INVENTORY_SLOT_BAG_END) ||
      (dstbag == INVENTORY_SLOT_BAG_0 && dstslot < INVENTORY_SLOT_BAG_END))
    ApplyItemDependentAuras((Item *)nullptr, false);

  // if player is moving bags and is looting an item inside this bag
  // release the loot
  if (GetLootGUID()) {
    bool released = false;
    if (IsBagPos(src)) {
      Bag *bag = pSrcItem->ToBag();
      for (uint32 i = 0; i < bag->GetBagSize(); ++i) {
        if (Item *bagItem = bag->GetItemByPos(i)) {
          if (bagItem->GetGUID() == GetLootGUID()) {
            m_session->DoLootRelease(GetLootGUID());
            released = true; // so we don't need to look at dstBag
            break;
          }
        }
      }
    }

    if (!released && IsBagPos(dst)) {
      Bag *bag = pDstItem->ToBag();
      for (uint32 i = 0; i < bag->GetBagSize(); ++i) {
        if (Item *bagItem = bag->GetItemByPos(i)) {
          if (bagItem->GetGUID() == GetLootGUID()) {
            m_session->DoLootRelease(GetLootGUID());
            released = true; // not realy needed here
            break;
          }
        }
      }
    }
  }

  AutoUnequipOffhandIfNeed();
}

void Player::AddItemToBuyBackSlot(Item *pItem) {
  if (pItem) {
    uint32 slot = m_currentBuybackSlot;
    // if current back slot non-empty search oldest or free
    if (m_items[slot]) {
      uint32 oldest_time = GetUInt32Value(PLAYER_FIELD_BUYBACK_TIMESTAMP_1);
      uint32 oldest_slot = BUYBACK_SLOT_START;

      for (uint32 i = BUYBACK_SLOT_START + 1; i < BUYBACK_SLOT_END; ++i) {
        // found empty
        if (!m_items[i]) {
          oldest_slot = i;
          break;
        }

        uint32 i_time = GetUInt32Value(PLAYER_FIELD_BUYBACK_TIMESTAMP_1 + i -
                                       BUYBACK_SLOT_START);

        if (oldest_time > i_time) {
          oldest_time = i_time;
          oldest_slot = i;
        }
      }

      // find oldest
      slot = oldest_slot;
    }

    RemoveItemFromBuyBackSlot(slot, true);
    LOG_DEBUG(
        "entities.player.items",
        "Player::AddItemToBuyBackSlot: Player '%s' (%s), Item: %u, Slot: %u",
        GetName().c_str(), GetGUID().ToString().c_str(), pItem->GetEntry(),
        slot);

    m_items[slot] = pItem;
    time_t base = GameTime::GetGameTime();
    uint32 etime = uint32(base - m_logintime + (30 * 3600));
    uint32 eslot = slot - BUYBACK_SLOT_START;

    SetGuidValue(PLAYER_FIELD_VENDORBUYBACK_SLOT_1 + (eslot * 2),
                 pItem->GetGUID());
    if (ItemTemplate const *proto = pItem->GetTemplate())
      SetUInt32Value(PLAYER_FIELD_BUYBACK_PRICE_1 + eslot,
                     proto->GetSellPrice() * pItem->GetCount());
    else
      SetUInt32Value(PLAYER_FIELD_BUYBACK_PRICE_1 + eslot, 0);
    SetUInt32Value(PLAYER_FIELD_BUYBACK_TIMESTAMP_1 + eslot, (uint32)etime);

    // move to next (for non filled list is move most optimized choice)
    if (m_currentBuybackSlot < BUYBACK_SLOT_END - 1)
      ++m_currentBuybackSlot;
  }
}

Item *Player::GetItemFromBuyBackSlot(uint32 slot) {
  LOG_DEBUG("entities.player.items",
            "Player::GetItemFromBuyBackSlot: Player '%s' (%s), Slot: %u",
            GetName().c_str(), GetGUID().ToString().c_str(), slot);
  if (slot >= BUYBACK_SLOT_START && slot < BUYBACK_SLOT_END)
    return m_items[slot];
  return nullptr;
}

void Player::RemoveItemFromBuyBackSlot(uint32 slot, bool del) {
  LOG_DEBUG("entities.player.items",
            "Player::RemoveItemFromBuyBackSlot: Player '%s' (%s), Slot: %u",
            GetName().c_str(), GetGUID().ToString().c_str(), slot);
  if (slot >= BUYBACK_SLOT_START && slot < BUYBACK_SLOT_END) {
    Item *pItem = m_items[slot];
    if (pItem) {
      pItem->RemoveFromWorld();
      if (del) {
        if (ItemTemplate const *itemTemplate = pItem->GetTemplate())
          if (itemTemplate->GetFlags() & ITEM_FLAG_HAS_LOOT)
            sLootItemStorage->RemoveStoredLootForContainer(
                pItem->GetGUID().GetCounter());

        pItem->SetState(ITEM_REMOVED, this);
      }
    }

    m_items[slot] = nullptr;

    uint32 eslot = slot - BUYBACK_SLOT_START;
    SetGuidValue(PLAYER_FIELD_VENDORBUYBACK_SLOT_1 + (eslot * 2),
                 ObjectGuid::Empty);
    SetUInt32Value(PLAYER_FIELD_BUYBACK_PRICE_1 + eslot, 0);
    SetUInt32Value(PLAYER_FIELD_BUYBACK_TIMESTAMP_1 + eslot, 0);

    // if current backslot is filled set to now free slot
    if (m_items[m_currentBuybackSlot])
      m_currentBuybackSlot = slot;
  }
}

void Player::SendEquipError(InventoryResult msg, Item *pItem, Item *pItem2,
                            uint32 itemid) const {
  WorldPacket data(SMSG_INVENTORY_CHANGE_FAILURE,
                   (msg == EQUIP_ERR_CANT_EQUIP_LEVEL_I ? 22 : 18));
  data << uint8(msg);

  if (msg != EQUIP_ERR_OK) {
    data << uint64(pItem ? pItem->GetGUID() : ObjectGuid::Empty);
    data << uint64(pItem2 ? pItem2->GetGUID() : ObjectGuid::Empty);
    data << uint8(0); // bag type subclass, used with
                      // EQUIP_ERR_EVENT_AUTOEQUIP_BIND_CONFIRM and
                      // EQUIP_ERR_ITEM_DOESNT_GO_INTO_BAG2

    switch (msg) {
    case EQUIP_ERR_CANT_EQUIP_LEVEL_I:
    case EQUIP_ERR_PURCHASE_LEVEL_TOO_LOW: {
      ItemTemplate const *proto =
          pItem ? pItem->GetTemplate() : sObjectMgr->GetItemTemplate(itemid);
      data << uint32(proto ? proto->GetRequiredLevel() : 0);
      break;
    }
    case EQUIP_ERR_NO_OUTPUT: // no idea about this one...
    {
      data << uint64(0); // item guid
      data << uint32(0); // slot
      data << uint64(0); // container
      break;
    }
    case EQUIP_ERR_ITEM_MAX_LIMIT_CATEGORY_COUNT_EXCEEDED_IS:
    case EQUIP_ERR_ITEM_MAX_LIMIT_CATEGORY_SOCKETED_EXCEEDED_IS:
    case EQUIP_ERR_ITEM_MAX_LIMIT_CATEGORY_EQUIPPED_EXCEEDED_IS: {
      ItemTemplate const *proto =
          pItem ? pItem->GetTemplate() : sObjectMgr->GetItemTemplate(itemid);
      data << uint32(proto ? proto->GetItemLimitCategory() : 0);
      break;
    }
    default:
      break;
    }
  }
  SendDirectMessage(&data);
}

void Player::SendBuyError(BuyResult msg, Creature *creature, uint32 item,
                          uint32 /*param*/) const {
  WorldPacket data(SMSG_BUY_FAILED, (8 + 4 + 4 + 1));
  data << uint64(creature ? creature->GetGUID() : ObjectGuid::Empty);
  data << uint32(item);
  data << uint8(msg);
  SendDirectMessage(&data);
}

void Player::SendSellError(SellResult msg, Creature *creature,
                           ObjectGuid guid) const {
  WorldPacket data(SMSG_SELL_ITEM, (8 + 8 + 1)); // last check 4.3.4
  data << uint64(creature ? creature->GetGUID() : ObjectGuid::Empty);
  data << uint64(guid);
  data << uint8(msg);
  SendDirectMessage(&data);
}

bool Player::IsUseEquipedWeapon(bool mainhand) const {
  // disarm applied only to mainhand weapon
  return !IsInFeralForm() &&
         (!mainhand || !HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISARMED));
}

void Player::SetCanTitanGrip(bool value, uint32 penaltySpellId /*= 0*/) {
  if (value == m_canTitanGrip)
    return;

  m_canTitanGrip = value;
  m_titanGripPenaltySpellId = penaltySpellId;
}

void Player::CheckTitanGripPenalty() {
  if (!CanTitanGrip())
    return;

  bool apply = IsUsingTwoHandedWeaponInOneHand();
  if (apply) {
    if (!HasAura(m_titanGripPenaltySpellId))
      CastSpell((Unit *)nullptr, m_titanGripPenaltySpellId, true);
  } else
    RemoveAurasDueToSpell(m_titanGripPenaltySpellId);
}

bool Player::IsTwoHandUsed() const {
  Item *mainItem = GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);
  return mainItem &&
         mainItem->GetTemplate()->GetInventoryType() == INVTYPE_2HWEAPON &&
         !CanTitanGrip();
}

bool Player::IsUsingTwoHandedWeaponInOneHand() const {
  Item *offItem = GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
  if (offItem && offItem->GetTemplate()->GetInventoryType() == INVTYPE_2HWEAPON)
    return true;

  Item *mainItem = GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);
  if (!mainItem ||
      mainItem->GetTemplate()->GetInventoryType() != INVTYPE_2HWEAPON)
    return false;

  if (!offItem)
    return false;

  return true;
}

void Player::TradeCancel(bool sendback) {
  if (m_trade) {
    Player *trader = m_trade->GetTrader();

    // send yellow "Trade canceled" message to both traders
    if (sendback)
      GetSession()->SendCancelTrade();

    trader->GetSession()->SendCancelTrade();

    // cleanup
    delete m_trade;
    m_trade = nullptr;
    delete trader->m_trade;
    trader->m_trade = nullptr;
  }
}

void Player::UpdateSoulboundTradeItems() {
  // also checks for garbage data
  for (GuidUnorderedSet::iterator itr = m_itemSoulboundTradeable.begin();
       itr != m_itemSoulboundTradeable.end();) {
    Item *item = GetItemByGuid(*itr);
    if (!item || item->GetOwnerGUID() != GetGUID() ||
        item->CheckSoulboundTradeExpire())
      itr = m_itemSoulboundTradeable.erase(itr);
    else
      ++itr;
  }
}

void Player::AddTradeableItem(Item *item) {
  m_itemSoulboundTradeable.insert(item->GetGUID());
}

void Player::RemoveTradeableItem(Item *item) {
  m_itemSoulboundTradeable.erase(item->GetGUID());
}

void Player::UpdateItemDuration(uint32 time, bool realtimeonly) {
  if (m_itemDuration.empty())
    return;

  LOG_DEBUG("entities.player.items",
            "Player::UpdateItemDuration: Player '%s' (%s), Time: %u, "
            "RealTimeOnly: %u",
            GetName().c_str(), GetGUID().ToString().c_str(), time,
            realtimeonly);

  for (ItemDurationList::const_iterator itr = m_itemDuration.begin();
       itr != m_itemDuration.end();) {
    Item *item = *itr;
    ++itr; // current element can be erased in UpdateDuration

    if (!realtimeonly ||
        item->GetTemplate()->GetFlags() & ITEM_FLAG_REAL_DURATION)
      item->UpdateDuration(this, time);
  }
}

void Player::UpdateEnchantTime(uint32 time) {
  for (EnchantDurationList::iterator itr = m_enchantDuration.begin(), next;
       itr != m_enchantDuration.end(); itr = next) {
    ASSERT(itr->item);
    next = itr;
    if (!itr->item->GetEnchantmentId(itr->slot)) {
      next = m_enchantDuration.erase(itr);
    } else if (itr->leftduration <= time) {
      ApplyEnchantment(itr->item, itr->slot, false, false);
      itr->item->ClearEnchantment(itr->slot);
      next = m_enchantDuration.erase(itr);
    } else if (itr->leftduration > time) {
      itr->leftduration -= time;
      ++next;
    }
  }
}

void Player::AddEnchantmentDurations(Item *item) {
  for (int x = 0; x < MAX_ENCHANTMENT_SLOT; ++x) {
    if (x > PRISMATIC_ENCHANTMENT_SLOT &&
        x < PROP_ENCHANTMENT_SLOT_0) // not holding enchantment id
      continue;

    if (!item->GetEnchantmentId(EnchantmentSlot(x)))
      continue;

    uint32 duration = item->GetEnchantmentDuration(EnchantmentSlot(x));
    if (duration > 0)
      AddEnchantmentDuration(item, EnchantmentSlot(x), duration);
  }
}

void Player::RemoveEnchantmentDurations(Item *item) {
  for (EnchantDurationList::iterator itr = m_enchantDuration.begin();
       itr != m_enchantDuration.end();) {
    if (itr->item == item) {
      // save duration in item
      item->SetEnchantmentDuration(EnchantmentSlot(itr->slot),
                                   itr->leftduration, this);
      itr = m_enchantDuration.erase(itr);
    } else
      ++itr;
  }
}

void Player::RemoveEnchantmentDurationsReferences(Item *item) {
  for (EnchantDurationList::iterator itr = m_enchantDuration.begin();
       itr != m_enchantDuration.end();) {
    if (itr->item == item)
      itr = m_enchantDuration.erase(itr);
    else
      ++itr;
  }
}

void Player::RemoveArenaEnchantments(EnchantmentSlot slot) {
  // remove enchantments from equipped items first to clean up the
  // m_enchantDuration list
  for (EnchantDurationList::iterator itr = m_enchantDuration.begin(), next;
       itr != m_enchantDuration.end(); itr = next) {
    next = itr;
    if (itr->slot == slot) {
      if (itr->item && itr->item->GetEnchantmentId(slot)) {
        // Poisons and DK runes are enchants which are allowed on arenas
        if (sSpellMgr->IsArenaAllowedEnchancment(
                itr->item->GetEnchantmentId(slot))) {
          ++next;
          continue;
        }
        // remove from stats
        ApplyEnchantment(itr->item, slot, false, false);
        // remove visual
        itr->item->ClearEnchantment(slot);
      }
      // remove from update list
      next = m_enchantDuration.erase(itr);
    } else
      ++next;
  }

  // remove enchants from inventory items
  // NOTE: no need to remove these from stats, since these aren't equipped
  // in inventory
  for (uint8 i = INVENTORY_SLOT_ITEM_START; i < INVENTORY_SLOT_ITEM_END; ++i)
    if (Item *pItem = GetItemByPos(INVENTORY_SLOT_BAG_0, i))
      if (!sSpellMgr->IsArenaAllowedEnchancment(pItem->GetEnchantmentId(slot)))
        pItem->ClearEnchantment(slot);

  // in inventory bags
  for (uint8 i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; ++i)
    if (Bag *pBag = GetBagByPos(i))
      for (uint32 j = 0; j < pBag->GetBagSize(); j++)
        if (Item *pItem = pBag->GetItemByPos(j))
          if (!sSpellMgr->IsArenaAllowedEnchancment(
                  pItem->GetEnchantmentId(slot)))
            pItem->ClearEnchantment(slot);
}

// duration == 0 will remove item enchant
void Player::AddEnchantmentDuration(Item *item, EnchantmentSlot slot,
                                    uint32 duration) {
  if (!item)
    return;

  if (slot >= MAX_ENCHANTMENT_SLOT)
    return;

  for (EnchantDurationList::iterator itr = m_enchantDuration.begin();
       itr != m_enchantDuration.end(); ++itr) {
    if (itr->item == item && itr->slot == slot) {
      itr->item->SetEnchantmentDuration(itr->slot, itr->leftduration, this);
      m_enchantDuration.erase(itr);
      break;
    }
  }
  if (duration > 0) {
    GetSession()->SendItemEnchantTimeUpdate(GetGUID(), item->GetGUID(), slot,
                                            uint32(duration / 1000));
    m_enchantDuration.push_back(EnchantDuration(item, slot, duration));
  }
}

void Player::ApplyReforgeEnchantment(Item *item, bool apply) {
  if (!item || item->IsBroken())
    return;

  ItemReforgeEntry const *reforge = sItemReforgeStore.LookupEntry(
      item->GetEnchantmentId(REFORGE_ENCHANTMENT_SLOT));
  if (!reforge)
    return;

  float removeValue =
      item->GetReforgableStat(ItemModType(reforge->Source_stat)) *
      reforge->Source_multiplier;
  float addValue = removeValue * reforge->Target_multiplier;

  switch (reforge->Source_stat) {
  case ITEM_MOD_MANA:
    HandleStatFlatModifier(UNIT_MOD_MANA, BASE_VALUE, -removeValue, apply);
    break;
  case ITEM_MOD_HEALTH:
    HandleStatFlatModifier(UNIT_MOD_HEALTH, BASE_VALUE, -removeValue, apply);
    break;
  case ITEM_MOD_AGILITY:
    HandleStatFlatModifier(UNIT_MOD_STAT_AGILITY, TOTAL_VALUE, -removeValue,
                           apply);
    UpdateStatBuffMod(STAT_AGILITY);
    break;
  case ITEM_MOD_STRENGTH:
    HandleStatFlatModifier(UNIT_MOD_STAT_STRENGTH, TOTAL_VALUE, -removeValue,
                           apply);
    UpdateStatBuffMod(STAT_STRENGTH);
    break;
  case ITEM_MOD_INTELLECT:
    HandleStatFlatModifier(UNIT_MOD_STAT_INTELLECT, TOTAL_VALUE, -removeValue,
                           apply);
    UpdateStatBuffMod(STAT_INTELLECT);
    break;
  case ITEM_MOD_SPIRIT:
    HandleStatFlatModifier(UNIT_MOD_STAT_SPIRIT, TOTAL_VALUE, -removeValue,
                           apply);
    UpdateStatBuffMod(STAT_SPIRIT);
    break;
  case ITEM_MOD_STAMINA:
    HandleStatFlatModifier(UNIT_MOD_STAT_STAMINA, TOTAL_VALUE, -removeValue,
                           apply);
    UpdateStatBuffMod(STAT_STAMINA);
    break;
  case ITEM_MOD_DEFENSE_SKILL_RATING:
    ApplyRatingMod(CR_DEFENSE_SKILL, -int32(removeValue), apply);
    break;
  case ITEM_MOD_DODGE_RATING:
    ApplyRatingMod(CR_DODGE, -int32(removeValue), apply);
    break;
  case ITEM_MOD_PARRY_RATING:
    ApplyRatingMod(CR_PARRY, -int32(removeValue), apply);
    break;
  case ITEM_MOD_BLOCK_RATING:
    ApplyRatingMod(CR_BLOCK, -int32(removeValue), apply);
    break;
  case ITEM_MOD_HIT_MELEE_RATING:
    ApplyRatingMod(CR_HIT_MELEE, -int32(removeValue), apply);
    break;
  case ITEM_MOD_HIT_RANGED_RATING:
    ApplyRatingMod(CR_HIT_RANGED, -int32(removeValue), apply);
    break;
  case ITEM_MOD_HIT_SPELL_RATING:
    ApplyRatingMod(CR_HIT_SPELL, -int32(removeValue), apply);
    break;
  case ITEM_MOD_CRIT_MELEE_RATING:
    ApplyRatingMod(CR_CRIT_MELEE, -int32(removeValue), apply);
    break;
  case ITEM_MOD_CRIT_RANGED_RATING:
    ApplyRatingMod(CR_CRIT_RANGED, -int32(removeValue), apply);
    break;
  case ITEM_MOD_CRIT_SPELL_RATING:
    ApplyRatingMod(CR_CRIT_SPELL, -int32(removeValue), apply);
    break;
  case ITEM_MOD_HASTE_SPELL_RATING:
    ApplyRatingMod(CR_HASTE_SPELL, -int32(removeValue), apply);
    break;
  case ITEM_MOD_HIT_RATING:
    ApplyRatingMod(CR_HIT_MELEE, -int32(removeValue), apply);
    ApplyRatingMod(CR_HIT_RANGED, -int32(removeValue), apply);
    ApplyRatingMod(CR_HIT_SPELL, -int32(removeValue), apply);
    break;
  case ITEM_MOD_CRIT_RATING:
    ApplyRatingMod(CR_CRIT_MELEE, -int32(removeValue), apply);
    ApplyRatingMod(CR_CRIT_RANGED, -int32(removeValue), apply);
    ApplyRatingMod(CR_CRIT_SPELL, -int32(removeValue), apply);
    break;
  case ITEM_MOD_RESILIENCE_RATING:
    ApplyRatingMod(CR_RESILIENCE_PLAYER_DAMAGE_TAKEN, -int32(removeValue),
                   apply);
    break;
  case ITEM_MOD_HASTE_RATING:
    ApplyRatingMod(CR_HASTE_MELEE, -int32(removeValue), apply);
    ApplyRatingMod(CR_HASTE_RANGED, -int32(removeValue), apply);
    ApplyRatingMod(CR_HASTE_SPELL, -int32(removeValue), apply);
    break;
  case ITEM_MOD_EXPERTISE_RATING:
    ApplyRatingMod(CR_EXPERTISE, -int32(removeValue), apply);
    break;
  case ITEM_MOD_ATTACK_POWER:
    HandleStatFlatModifier(UNIT_MOD_ATTACK_POWER, TOTAL_VALUE, -removeValue,
                           apply);
    HandleStatFlatModifier(UNIT_MOD_ATTACK_POWER_RANGED, TOTAL_VALUE,
                           -removeValue, apply);
    break;
  case ITEM_MOD_RANGED_ATTACK_POWER:
    HandleStatFlatModifier(UNIT_MOD_ATTACK_POWER_RANGED, TOTAL_VALUE,
                           -removeValue, apply);
    break;
  case ITEM_MOD_MANA_REGENERATION:
    ApplyManaRegenBonus(-int32(removeValue), apply);
    break;
    // case ITEM_MOD_ARMOR_PENETRATION_RATING:
    //    ApplyRatingMod(CR_ARMOR_PENETRATION, -int32(removeValue), apply);
    //    break;
  case ITEM_MOD_SPELL_POWER:
    ApplySpellPowerBonus(-int32(removeValue), apply);
    break;
  case ITEM_MOD_HEALTH_REGEN:
    ApplyHealthRegenBonus(-int32(removeValue), apply);
    break;
  case ITEM_MOD_SPELL_PENETRATION:
    ApplyModInt32Value(PLAYER_FIELD_MOD_TARGET_RESISTANCE, -int32(removeValue),
                       apply);
    m_spellPenetrationItemMod +=
        apply ? -int32(removeValue) : int32(removeValue);
    break;
  case ITEM_MOD_BLOCK_VALUE:
    HandleBaseModFlatValue(SHIELD_BLOCK_VALUE, -removeValue, apply);
    break;
  case ITEM_MOD_MASTERY_RATING:
    ApplyRatingMod(CR_MASTERY, -int32(removeValue), apply);
    break;
  case ITEM_MOD_EXTRA_ARMOR:
    HandleStatFlatModifier(UNIT_MOD_ARMOR, BASE_VALUE, -removeValue, apply);
    break;
  }

  switch (reforge->Target_stat) {
  case ITEM_MOD_MANA:
    HandleStatFlatModifier(UNIT_MOD_MANA, BASE_VALUE, addValue, apply);
    break;
  case ITEM_MOD_HEALTH:
    HandleStatFlatModifier(UNIT_MOD_HEALTH, BASE_VALUE, addValue, apply);
    break;
  case ITEM_MOD_AGILITY:
    HandleStatFlatModifier(UNIT_MOD_STAT_AGILITY, TOTAL_VALUE, addValue, apply);
    UpdateStatBuffMod(STAT_AGILITY);
    break;
  case ITEM_MOD_STRENGTH:
    HandleStatFlatModifier(UNIT_MOD_STAT_STRENGTH, TOTAL_VALUE, addValue,
                           apply);
    UpdateStatBuffMod(STAT_STRENGTH);
    break;
  case ITEM_MOD_INTELLECT:
    HandleStatFlatModifier(UNIT_MOD_STAT_INTELLECT, TOTAL_VALUE, addValue,
                           apply);
    UpdateStatBuffMod(STAT_INTELLECT);
    break;
  case ITEM_MOD_SPIRIT:
    HandleStatFlatModifier(UNIT_MOD_STAT_SPIRIT, TOTAL_VALUE, addValue, apply);
    UpdateStatBuffMod(STAT_SPIRIT);
    break;
  case ITEM_MOD_STAMINA:
    HandleStatFlatModifier(UNIT_MOD_STAT_STAMINA, TOTAL_VALUE, addValue, apply);
    UpdateStatBuffMod(STAT_STAMINA);
    break;
  case ITEM_MOD_DEFENSE_SKILL_RATING:
    ApplyRatingMod(CR_DEFENSE_SKILL, int32(addValue), apply);
    break;
  case ITEM_MOD_DODGE_RATING:
    ApplyRatingMod(CR_DODGE, int32(addValue), apply);
    break;
  case ITEM_MOD_PARRY_RATING:
    ApplyRatingMod(CR_PARRY, int32(addValue), apply);
    break;
  case ITEM_MOD_BLOCK_RATING:
    ApplyRatingMod(CR_BLOCK, int32(addValue), apply);
    break;
  case ITEM_MOD_HIT_MELEE_RATING:
    ApplyRatingMod(CR_HIT_MELEE, int32(addValue), apply);
    break;
  case ITEM_MOD_HIT_RANGED_RATING:
    ApplyRatingMod(CR_HIT_RANGED, int32(addValue), apply);
    break;
  case ITEM_MOD_HIT_SPELL_RATING:
    ApplyRatingMod(CR_HIT_SPELL, int32(addValue), apply);
    break;
  case ITEM_MOD_CRIT_MELEE_RATING:
    ApplyRatingMod(CR_CRIT_MELEE, int32(addValue), apply);
    break;
  case ITEM_MOD_CRIT_RANGED_RATING:
    ApplyRatingMod(CR_CRIT_RANGED, int32(addValue), apply);
    break;
  case ITEM_MOD_CRIT_SPELL_RATING:
    ApplyRatingMod(CR_CRIT_SPELL, int32(addValue), apply);
    break;
  case ITEM_MOD_HASTE_SPELL_RATING:
    ApplyRatingMod(CR_HASTE_SPELL, int32(addValue), apply);
    break;
  case ITEM_MOD_HIT_RATING:
    ApplyRatingMod(CR_HIT_MELEE, int32(addValue), apply);
    ApplyRatingMod(CR_HIT_RANGED, int32(addValue), apply);
    ApplyRatingMod(CR_HIT_SPELL, int32(addValue), apply);
    break;
  case ITEM_MOD_CRIT_RATING:
    ApplyRatingMod(CR_CRIT_MELEE, int32(addValue), apply);
    ApplyRatingMod(CR_CRIT_RANGED, int32(addValue), apply);
    ApplyRatingMod(CR_CRIT_SPELL, int32(addValue), apply);
    break;
  case ITEM_MOD_RESILIENCE_RATING:
    ApplyRatingMod(CR_RESILIENCE_PLAYER_DAMAGE_TAKEN, int32(addValue), apply);
    break;
  case ITEM_MOD_HASTE_RATING:
    ApplyRatingMod(CR_HASTE_MELEE, int32(addValue), apply);
    ApplyRatingMod(CR_HASTE_RANGED, int32(addValue), apply);
    ApplyRatingMod(CR_HASTE_SPELL, int32(addValue), apply);
    break;
  case ITEM_MOD_EXPERTISE_RATING:
    ApplyRatingMod(CR_EXPERTISE, int32(addValue), apply);
    break;
  case ITEM_MOD_ATTACK_POWER:
    HandleStatFlatModifier(UNIT_MOD_ATTACK_POWER, TOTAL_VALUE, addValue, apply);
    HandleStatFlatModifier(UNIT_MOD_ATTACK_POWER_RANGED, TOTAL_VALUE, addValue,
                           apply);
    break;
  case ITEM_MOD_RANGED_ATTACK_POWER:
    HandleStatFlatModifier(UNIT_MOD_ATTACK_POWER_RANGED, TOTAL_VALUE, addValue,
                           apply);
    break;
  case ITEM_MOD_MANA_REGENERATION:
    ApplyManaRegenBonus(int32(addValue), apply);
    break;
    // case ITEM_MOD_ARMOR_PENETRATION_RATING:
    //    ApplyRatingMod(CR_ARMOR_PENETRATION, int32(addValue), apply);
    //    break;
  case ITEM_MOD_SPELL_POWER:
    ApplySpellPowerBonus(int32(addValue), apply);
    break;
  case ITEM_MOD_HEALTH_REGEN:
    ApplyHealthRegenBonus(int32(addValue), apply);
    break;
  case ITEM_MOD_SPELL_PENETRATION:
    ApplyModInt32Value(PLAYER_FIELD_MOD_TARGET_RESISTANCE, int32(addValue),
                       apply);
    m_spellPenetrationItemMod += apply ? int32(addValue) : -int32(addValue);
    break;
  case ITEM_MOD_BLOCK_VALUE:
    HandleBaseModFlatValue(SHIELD_BLOCK_VALUE, addValue, apply);
    break;
  case ITEM_MOD_MASTERY_RATING:
    ApplyRatingMod(CR_MASTERY, int32(addValue), apply);
    break;
  case ITEM_MOD_EXTRA_ARMOR:
    HandleStatFlatModifier(UNIT_MOD_ARMOR, BASE_VALUE, addValue, apply);
    break;
  }
}

void Player::ApplyEnchantment(Item *item, bool apply) {
  for (uint32 slot = 0; slot < MAX_ENCHANTMENT_SLOT; ++slot) {
    // Apply reforge as last enchant
    if (slot == REFORGE_ENCHANTMENT_SLOT)
      continue;

    ApplyEnchantment(item, EnchantmentSlot(slot), apply);
  }

  ApplyEnchantment(item, REFORGE_ENCHANTMENT_SLOT, apply);
}

void Player::ApplyEnchantment(Item *item, EnchantmentSlot slot, bool apply,
                              bool apply_dur, bool ignore_condition) {
  if (!item || !item->IsEquipped())
    return;

  if (slot >= MAX_ENCHANTMENT_SLOT)
    return;

  if (slot == TRANSMOGRIFY_ENCHANTMENT_SLOT)
    return;

  if (slot == REFORGE_ENCHANTMENT_SLOT) {
    ApplyReforgeEnchantment(item, apply);
    return;
  }

  uint32 enchant_id = item->GetEnchantmentId(slot);
  if (!enchant_id)
    return;

  SpellItemEnchantmentEntry const *pEnchant =
      sSpellItemEnchantmentStore.LookupEntry(enchant_id);
  if (!pEnchant)
    return;

  if (!ignore_condition && pEnchant->Condition_ID &&
      !EnchantmentFitsRequirements(pEnchant->Condition_ID, -1))
    return;

  if (pEnchant->MinLevel > getLevel())
    return;

  if (pEnchant->RequiredSkillID > 0 &&
      pEnchant->RequiredSkillRank > GetSkillValue(pEnchant->RequiredSkillID))
    return;

  // Cogwheel gems dont have requirement data set in SpellItemEnchantment.dbc,
  // but they do have it in Item-sparse.db2
  if (ItemTemplate const *gem =
          sObjectMgr->GetItemTemplate(pEnchant->Src_itemID))
    if (gem->GetRequiredSkill() &&
        GetSkillValue(gem->GetRequiredSkill()) < gem->GetRequiredSkillRank())
      return;

  // If we're dealing with a gem inside a prismatic socket we need to check the
  // prismatic socket requirements rather than the gem requirements itself. If
  // the socket has no color it is a prismatic socket.
  if ((slot == SOCK_ENCHANTMENT_SLOT || slot == SOCK_ENCHANTMENT_SLOT_2 ||
       slot == SOCK_ENCHANTMENT_SLOT_3) &&
      !item->GetTemplate()->GetSocketColor(slot - SOCK_ENCHANTMENT_SLOT)) {
    // Check if the requirements for the prismatic socket are met before
    // applying the gem stats
    SpellItemEnchantmentEntry const *pPrismaticEnchant =
        sSpellItemEnchantmentStore.LookupEntry(
            item->GetEnchantmentId(PRISMATIC_ENCHANTMENT_SLOT));
    if (!pPrismaticEnchant ||
        (pPrismaticEnchant->RequiredSkillID > 0 &&
         pPrismaticEnchant->RequiredSkillRank >
             GetSkillValue(pPrismaticEnchant->RequiredSkillID)))
      return;
  }

  if (!item->IsBroken()) {
    for (uint8 s = 0; s < MAX_ITEM_ENCHANTMENT_EFFECTS; ++s) {
      uint32 enchant_display_type = pEnchant->Effect[s];
      uint32 enchant_amount = pEnchant->EffectPointsMin[s];
      uint32 enchant_spell_id = pEnchant->EffectArg[s];

      switch (enchant_display_type) {
      case ITEM_ENCHANTMENT_TYPE_NONE:
        break;
      case ITEM_ENCHANTMENT_TYPE_COMBAT_SPELL:
        // processed in Player::CastItemCombatSpell
        break;
      case ITEM_ENCHANTMENT_TYPE_DAMAGE: {
        WeaponAttackType const attackType =
            Player::GetAttackBySlot(item->GetSlot());
        if (attackType != MAX_ATTACK)
          UpdateDamageDoneMods(attackType);
        break;
      }
      case ITEM_ENCHANTMENT_TYPE_EQUIP_SPELL:
        if (enchant_spell_id) {
          if (apply) {
            int32 basepoints = 0;
            // Random Property Exist - try found basepoints for spell
            // (basepoints depends from item suffix factor)
            if (item->GetItemRandomPropertyId() < 0) {
              ItemRandomSuffixEntry const *item_rand =
                  sItemRandomSuffixStore.LookupEntry(
                      abs(item->GetItemRandomPropertyId()));
              if (item_rand) {
                // Search enchant_amount
                for (int k = 0; k < 5; ++k) {
                  if (item_rand->Enchantment[k] == enchant_id) {
                    basepoints = int32((item_rand->AllocationPct[k] *
                                        item->GetItemSuffixFactor()) /
                                       10000);
                    break;
                  }
                }
              }
            }

            CastSpellExtraArgs args(item);
            // Cast custom spell vs all equal basepoints got from enchant_amount
            if (basepoints)
              for (uint32 i = 0; i < MAX_SPELL_EFFECTS; ++i)
                args.SpellValueOverrides.AddMod(
                    SpellValueMod(SPELLVALUE_BASE_POINT0 + i), basepoints);

            CastSpell(this, enchant_spell_id, args);
          } else
            RemoveAurasDueToItemSpell(enchant_spell_id, item->GetGUID());
        }
        break;
      case ITEM_ENCHANTMENT_TYPE_RESISTANCE:
        if (!enchant_amount) {
          ItemRandomSuffixEntry const *item_rand =
              sItemRandomSuffixStore.LookupEntry(
                  abs(item->GetItemRandomPropertyId()));
          if (item_rand) {
            for (int k = 0; k < MAX_ITEM_ENCHANTMENT_EFFECTS; ++k) {
              if (item_rand->Enchantment[k] == enchant_id) {
                enchant_amount = uint32((item_rand->AllocationPct[k] *
                                         item->GetItemSuffixFactor()) /
                                        10000);
                break;
              }
            }
          }
        }

        HandleStatFlatModifier(
            UnitMods(UNIT_MOD_RESISTANCE_START + enchant_spell_id), TOTAL_VALUE,
            float(enchant_amount), apply);
        break;
      case ITEM_ENCHANTMENT_TYPE_STAT: {
        if (!enchant_amount) {
          ItemRandomSuffixEntry const *item_rand_suffix =
              sItemRandomSuffixStore.LookupEntry(
                  abs(item->GetItemRandomPropertyId()));
          if (item_rand_suffix) {
            for (int k = 0; k < 5; ++k) {
              if (item_rand_suffix->Enchantment[k] == enchant_id) {
                enchant_amount = uint32((item_rand_suffix->AllocationPct[k] *
                                         item->GetItemSuffixFactor()) /
                                        10000);
                break;
              }
            }
          }
        }

        LOG_DEBUG("entities.player.items", "Adding %u to stat nb %u",
                  enchant_amount, enchant_spell_id);
        switch (enchant_spell_id) {
        case ITEM_MOD_MANA:
          LOG_DEBUG("entities.player.items", "+ %u MANA", enchant_amount);
          HandleStatFlatModifier(UNIT_MOD_MANA, BASE_VALUE,
                                 float(enchant_amount), apply);
          break;
        case ITEM_MOD_HEALTH:
          LOG_DEBUG("entities.player.items", "+ %u HEALTH", enchant_amount);
          HandleStatFlatModifier(UNIT_MOD_HEALTH, BASE_VALUE,
                                 float(enchant_amount), apply);
          break;
        case ITEM_MOD_AGILITY:
          LOG_DEBUG("entities.player.items", "+ %u AGILITY", enchant_amount);
          HandleStatFlatModifier(UNIT_MOD_STAT_AGILITY, TOTAL_VALUE,
                                 float(enchant_amount), apply);
          UpdateStatBuffMod(STAT_AGILITY);
          break;
        case ITEM_MOD_STRENGTH:
          LOG_DEBUG("entities.player.items", "+ %u STRENGTH", enchant_amount);
          HandleStatFlatModifier(UNIT_MOD_STAT_STRENGTH, TOTAL_VALUE,
                                 float(enchant_amount), apply);
          UpdateStatBuffMod(STAT_STRENGTH);
          break;
        case ITEM_MOD_INTELLECT:
          LOG_DEBUG("entities.player.items", "+ %u INTELLECT", enchant_amount);
          HandleStatFlatModifier(UNIT_MOD_STAT_INTELLECT, TOTAL_VALUE,
                                 float(enchant_amount), apply);
          UpdateStatBuffMod(STAT_INTELLECT);
          break;
        case ITEM_MOD_SPIRIT:
          LOG_DEBUG("entities.player.items", "+ %u SPIRIT", enchant_amount);
          HandleStatFlatModifier(UNIT_MOD_STAT_SPIRIT, TOTAL_VALUE,
                                 float(enchant_amount), apply);
          UpdateStatBuffMod(STAT_SPIRIT);
          break;
        case ITEM_MOD_STAMINA:
          LOG_DEBUG("entities.player.items", "+ %u STAMINA", enchant_amount);
          HandleStatFlatModifier(UNIT_MOD_STAT_STAMINA, TOTAL_VALUE,
                                 float(enchant_amount), apply);
          UpdateStatBuffMod(STAT_STAMINA);
          break;
        case ITEM_MOD_DEFENSE_SKILL_RATING:
          ApplyRatingMod(CR_DEFENSE_SKILL, enchant_amount, apply);
          LOG_DEBUG("entities.player.items", "+ %u DEFENCE", enchant_amount);
          break;
        case ITEM_MOD_DODGE_RATING:
          ApplyRatingMod(CR_DODGE, enchant_amount, apply);
          LOG_DEBUG("entities.player.items", "+ %u DODGE", enchant_amount);
          break;
        case ITEM_MOD_PARRY_RATING:
          ApplyRatingMod(CR_PARRY, enchant_amount, apply);
          LOG_DEBUG("entities.player.items", "+ %u PARRY", enchant_amount);
          break;
        case ITEM_MOD_BLOCK_RATING:
          ApplyRatingMod(CR_BLOCK, enchant_amount, apply);
          LOG_DEBUG("entities.player.items", "+ %u SHIELD_BLOCK",
                    enchant_amount);
          break;
        case ITEM_MOD_HIT_MELEE_RATING:
          ApplyRatingMod(CR_HIT_MELEE, enchant_amount, apply);
          LOG_DEBUG("entities.player.items", "+ %u MELEE_HIT", enchant_amount);
          break;
        case ITEM_MOD_HIT_RANGED_RATING:
          ApplyRatingMod(CR_HIT_RANGED, enchant_amount, apply);
          LOG_DEBUG("entities.player.items", "+ %u RANGED_HIT", enchant_amount);
          break;
        case ITEM_MOD_HIT_SPELL_RATING:
          ApplyRatingMod(CR_HIT_SPELL, enchant_amount, apply);
          LOG_DEBUG("entities.player.items", "+ %u SPELL_HIT", enchant_amount);
          break;
        case ITEM_MOD_CRIT_MELEE_RATING:
          ApplyRatingMod(CR_CRIT_MELEE, enchant_amount, apply);
          LOG_DEBUG("entities.player.items", "+ %u MELEE_CRIT", enchant_amount);
          break;
        case ITEM_MOD_CRIT_RANGED_RATING:
          ApplyRatingMod(CR_CRIT_RANGED, enchant_amount, apply);
          LOG_DEBUG("entities.player.items", "+ %u RANGED_CRIT",
                    enchant_amount);
          break;
        case ITEM_MOD_CRIT_SPELL_RATING:
          ApplyRatingMod(CR_CRIT_SPELL, enchant_amount, apply);
          LOG_DEBUG("entities.player.items", "+ %u SPELL_CRIT", enchant_amount);
          break;
          // Values from ITEM_STAT_MELEE_HA_RATING to
          // ITEM_MOD_HASTE_RANGED_RATING are never used in Enchantments case
          // ITEM_MOD_HIT_TAKEN_MELEE_RATING:
          //     ApplyRatingMod(CR_HIT_TAKEN_MELEE, enchant_amount, apply);
          //     break;
          // case ITEM_MOD_HIT_TAKEN_RANGED_RATING:
          //     ApplyRatingMod(CR_HIT_TAKEN_RANGED, enchant_amount, apply);
          //     break;
          // case ITEM_MOD_HIT_TAKEN_SPELL_RATING:
          //     ApplyRatingMod(CR_HIT_TAKEN_SPELL, enchant_amount, apply);
          //     break;
          // case ITEM_MOD_CRIT_TAKEN_MELEE_RATING:
          //     ApplyRatingMod(CR_CRIT_TAKEN_MELEE, enchant_amount, apply);
          //     break;
          // case ITEM_MOD_CRIT_TAKEN_RANGED_RATING:
          //     ApplyRatingMod(CR_CRIT_TAKEN_RANGED, enchant_amount, apply);
          //     break;
          // case ITEM_MOD_CRIT_TAKEN_SPELL_RATING:
          //     ApplyRatingMod(CR_CRIT_TAKEN_SPELL, enchant_amount, apply);
          //     break;
          // case ITEM_MOD_HASTE_MELEE_RATING:
          //     ApplyRatingMod(CR_HASTE_MELEE, enchant_amount, apply);
          //     break;
          // case ITEM_MOD_HASTE_RANGED_RATING:
          //     ApplyRatingMod(CR_HASTE_RANGED, enchant_amount, apply);
          //     break;
        case ITEM_MOD_HASTE_SPELL_RATING:
          ApplyRatingMod(CR_HASTE_SPELL, enchant_amount, apply);
          break;
        case ITEM_MOD_HIT_RATING:
          ApplyRatingMod(CR_HIT_MELEE, enchant_amount, apply);
          ApplyRatingMod(CR_HIT_RANGED, enchant_amount, apply);
          ApplyRatingMod(CR_HIT_SPELL, enchant_amount, apply);
          LOG_DEBUG("entities.player.items", "+ %u HIT", enchant_amount);
          break;
        case ITEM_MOD_CRIT_RATING:
          ApplyRatingMod(CR_CRIT_MELEE, enchant_amount, apply);
          ApplyRatingMod(CR_CRIT_RANGED, enchant_amount, apply);
          ApplyRatingMod(CR_CRIT_SPELL, enchant_amount, apply);
          LOG_DEBUG("entities.player.items", "+ %u CRITICAL", enchant_amount);
          break;
          // case ITEM_MOD_HIT_TAKEN_RATING: // Unused since 3.3.5
          //     ApplyRatingMod(CR_HIT_TAKEN_MELEE, enchant_amount, apply);
          //     ApplyRatingMod(CR_HIT_TAKEN_RANGED, enchant_amount, apply);
          //     ApplyRatingMod(CR_HIT_TAKEN_SPELL, enchant_amount, apply);
          //     break;
          // case ITEM_MOD_CRIT_TAKEN_RATING: // Unused since 3.3.5
          //     ApplyRatingMod(CR_CRIT_TAKEN_MELEE, enchant_amount, apply);
          //     ApplyRatingMod(CR_CRIT_TAKEN_RANGED, enchant_amount, apply);
          //     ApplyRatingMod(CR_CRIT_TAKEN_SPELL, enchant_amount, apply);
          //     break;
        case ITEM_MOD_RESILIENCE_RATING:
          ApplyRatingMod(CR_RESILIENCE_PLAYER_DAMAGE_TAKEN, enchant_amount,
                         apply);
          LOG_DEBUG("entities.player.items", "+ %u RESILIENCE", enchant_amount);
          break;
        case ITEM_MOD_HASTE_RATING:
          ApplyRatingMod(CR_HASTE_MELEE, enchant_amount, apply);
          ApplyRatingMod(CR_HASTE_RANGED, enchant_amount, apply);
          ApplyRatingMod(CR_HASTE_SPELL, enchant_amount, apply);
          LOG_DEBUG("entities.player.items", "+ %u HASTE", enchant_amount);
          break;
        case ITEM_MOD_EXPERTISE_RATING:
          ApplyRatingMod(CR_EXPERTISE, enchant_amount, apply);
          LOG_DEBUG("entities.player.items", "+ %u EXPERTISE", enchant_amount);
          break;
        case ITEM_MOD_ATTACK_POWER:
          HandleStatFlatModifier(UNIT_MOD_ATTACK_POWER, TOTAL_VALUE,
                                 float(enchant_amount), apply);
          HandleStatFlatModifier(UNIT_MOD_ATTACK_POWER_RANGED, TOTAL_VALUE,
                                 float(enchant_amount), apply);
          LOG_DEBUG("entities.player.items", "+ %u ATTACK_POWER",
                    enchant_amount);
          break;
        case ITEM_MOD_RANGED_ATTACK_POWER:
          HandleStatFlatModifier(UNIT_MOD_ATTACK_POWER_RANGED, TOTAL_VALUE,
                                 float(enchant_amount), apply);
          LOG_DEBUG("entities.player.items", "+ %u RANGED_ATTACK_POWER",
                    enchant_amount);
          break;
        case ITEM_MOD_MANA_REGENERATION:
          ApplyManaRegenBonus(enchant_amount, apply);
          LOG_DEBUG("entities.player.items", "+ %u MANA_REGENERATION",
                    enchant_amount);
          break;
          // case ITEM_MOD_ARMOR_PENETRATION_RATING:
          //     ApplyRatingMod(CR_ARMOR_PENETRATION, enchant_amount, apply);
          //     LOG_DEBUG("entities.player.items", "+ %u ARMOR PENETRATION",
          //     enchant_amount); break;
        case ITEM_MOD_SPELL_POWER:
          ApplySpellPowerBonus(enchant_amount, apply);
          LOG_DEBUG("entities.player.items", "+ %u SPELL_POWER",
                    enchant_amount);
          break;
        case ITEM_MOD_HEALTH_REGEN:
          ApplyHealthRegenBonus(enchant_amount, apply);
          LOG_DEBUG("entities.player.items", "+ %u HEALTH_REGENERATION",
                    enchant_amount);
          break;
        case ITEM_MOD_SPELL_PENETRATION:
          ApplySpellPenetrationBonus(enchant_amount, apply);
          LOG_DEBUG("entities.player.items", "+ %u SPELL_PENETRATION",
                    enchant_amount);
          break;
        case ITEM_MOD_BLOCK_VALUE:
          HandleBaseModFlatValue(SHIELD_BLOCK_VALUE, float(enchant_amount),
                                 apply);
          LOG_DEBUG("entities.player.items", "+ %u BLOCK_VALUE",
                    enchant_amount);
          break;
        case ITEM_MOD_MASTERY_RATING:
          ApplyRatingMod(CR_MASTERY, enchant_amount, apply);
          LOG_DEBUG("entities.player.items", "+ %u MASTERY", enchant_amount);
          break;
        case ITEM_MOD_EXTRA_ARMOR:
          HandleStatFlatModifier(UNIT_MOD_ARMOR, BASE_VALUE,
                                 float(enchant_amount), apply);
          LOG_DEBUG("entities.player.items", "+ %u EXTRA_ARMOR",
                    enchant_amount);
          break;
        default:
          break;
        }
        break;
      }
      case ITEM_ENCHANTMENT_TYPE_TOTEM: // Shaman Rockbiter Weapon
      {
        WeaponAttackType const attackType =
            Player::GetAttackBySlot(item->GetSlot());
        if (attackType != MAX_ATTACK)
          UpdateDamageDoneMods(attackType);
        break;
      }
      case ITEM_ENCHANTMENT_TYPE_USE_SPELL:
        // processed in Player::CastItemUseSpell
        break;
      case ITEM_ENCHANTMENT_TYPE_PRISMATIC_SOCKET:
        // nothing do..
        break;
      default:
        LOG_ERROR("entities.player",
                  "Player::ApplyEnchantment: Unknown item enchantment (ID: %u, "
                  "DisplayType: %u) for player '%s' (%s)",
                  enchant_id, enchant_display_type, GetName().c_str(),
                  GetGUID().ToString().c_str());
        break;
      }
    }
  }

  // visualize enchantment at player and equipped items
  if (slot == PERM_ENCHANTMENT_SLOT)
    SetUInt16Value(PLAYER_VISIBLE_ITEM_1_ENCHANTMENT + (item->GetSlot() * 2), 0,
                   apply ? item->GetEnchantmentId(slot) : 0);

  if (slot == TEMP_ENCHANTMENT_SLOT)
    SetUInt16Value(PLAYER_VISIBLE_ITEM_1_ENCHANTMENT + (item->GetSlot() * 2), 1,
                   apply ? item->GetEnchantmentId(slot) : 0);

  if (apply_dur) {
    if (apply) {
      // set duration
      uint32 duration = item->GetEnchantmentDuration(slot);
      if (duration > 0)
        AddEnchantmentDuration(item, slot, duration);
    } else {
      // duration == 0 will remove EnchantDuration
      AddEnchantmentDuration(item, slot, 0);
    }
  }
}

void Player::UpdateSkillEnchantments(uint16 skill_id, uint16 curr_value,
                                     uint16 new_value) {
  for (uint8 i = 0; i < INVENTORY_SLOT_BAG_END; ++i) {
    if (m_items[i]) {
      for (uint8 slot = 0; slot < MAX_ENCHANTMENT_SLOT; ++slot) {
        if (slot > PRISMATIC_ENCHANTMENT_SLOT &&
            slot < PROP_ENCHANTMENT_SLOT_0) // not holding enchantment id
          continue;

        uint32 ench_id = m_items[i]->GetEnchantmentId(EnchantmentSlot(slot));
        if (!ench_id)
          continue;

        SpellItemEnchantmentEntry const *Enchant =
            sSpellItemEnchantmentStore.LookupEntry(ench_id);
        if (!Enchant)
          return;

        if (Enchant->RequiredSkillID == skill_id) {
          // Checks if the enchantment needs to be applied or removed
          if (curr_value < Enchant->RequiredSkillRank &&
              new_value >= Enchant->RequiredSkillRank)
            ApplyEnchantment(m_items[i], EnchantmentSlot(slot), true);
          else if (new_value < Enchant->RequiredSkillRank &&
                   curr_value >= Enchant->RequiredSkillRank)
            ApplyEnchantment(m_items[i], EnchantmentSlot(slot), false);
        }

        // If we're dealing with a gem inside a prismatic socket we need to
        // check the prismatic socket requirements rather than the gem
        // requirements itself. If the socket has no color it is a prismatic
        // socket.
        if ((slot == SOCK_ENCHANTMENT_SLOT || slot == SOCK_ENCHANTMENT_SLOT_2 ||
             slot == SOCK_ENCHANTMENT_SLOT_3) &&
            !m_items[i]->GetTemplate()->GetSocketColor(slot -
                                                       SOCK_ENCHANTMENT_SLOT)) {
          SpellItemEnchantmentEntry const *pPrismaticEnchant =
              sSpellItemEnchantmentStore.LookupEntry(
                  m_items[i]->GetEnchantmentId(PRISMATIC_ENCHANTMENT_SLOT));

          if (pPrismaticEnchant &&
              pPrismaticEnchant->RequiredSkillID == skill_id) {
            if (curr_value < pPrismaticEnchant->RequiredSkillRank &&
                new_value >= pPrismaticEnchant->RequiredSkillRank)
              ApplyEnchantment(m_items[i], EnchantmentSlot(slot), true);
            else if (new_value < pPrismaticEnchant->RequiredSkillRank &&
                     curr_value >= pPrismaticEnchant->RequiredSkillRank)
              ApplyEnchantment(m_items[i], EnchantmentSlot(slot), false);
          }
        }
      }
    }
  }
}

void Player::SendEnchantmentDurations() {
  for (EnchantDurationList::const_iterator itr = m_enchantDuration.begin();
       itr != m_enchantDuration.end(); ++itr)
    GetSession()->SendItemEnchantTimeUpdate(GetGUID(), itr->item->GetGUID(),
                                            itr->slot,
                                            uint32(itr->leftduration) / 1000);
}

void Player::SendItemDurations() {
  for (ItemDurationList::const_iterator itr = m_itemDuration.begin();
       itr != m_itemDuration.end(); ++itr)
    (*itr)->SendTimeUpdate(this);
}

void Player::SendNewItem(Item *item, uint32 count, bool received, bool created,
                         bool broadcast, bool sendChatMessage) {
  if (!item) // prevent crash
    return;

  // last check 2.0.10
  WorldPacket data(SMSG_ITEM_PUSH_RESULT,
                   (8 + 4 + 4 + 4 + 1 + 4 + 4 + 4 + 4 + 4));
  data << uint64(GetGUID());         // player GUID
  data << uint32(received);          // 0=looted, 1=from npc
  data << uint32(created);           // 0=received, 1=created
  data << uint32(sendChatMessage);   // bool print message to chat
  data << uint8(item->GetBagSlot()); // bagslot
  // item slot, but when added to stack: 0xFFFFFFFF
  data << uint32((item->GetCount() == count) ? item->GetSlot() : -1);
  data << uint32(item->GetEntry());               // item id
  data << uint32(item->GetItemSuffixFactor());    // SuffixFactor
  data << int32(item->GetItemRandomPropertyId()); // random item property id
  data << uint32(count);                          // count of items
  data << uint32(GetItemCount(item->GetEntry())); // count of items in inventory

  if (broadcast && GetGroup())
    GetGroup()->BroadcastPacket(&data, true);
  else
    SendDirectMessage(&data);
}

/*********************************************************/
/***                    GOSSIP SYSTEM                  ***/
/*********************************************************/

void Player::PrepareGossipMenu(WorldObject *source, uint32 menuId /*= 0*/,
                               bool showQuests /*= false*/) {
  PlayerMenu *menu = PlayerTalkClass;
  menu->ClearMenus();

  menu->GetGossipMenu().SetMenuId(menuId);

  GossipMenuItemsMapBounds menuItemBounds =
      sObjectMgr->GetGossipMenuItemsMapBounds(menuId);

  // if default menuId and no menu options exist for this, use options from
  // default options
  if (menuItemBounds.first == menuItemBounds.second &&
      menuId == GetDefaultGossipMenuForSource(source))
    menuItemBounds = sObjectMgr->GetGossipMenuItemsMapBounds(0);

  uint32 npcflags = 0;

  if (source->GetTypeId() == TYPEID_UNIT) {
    npcflags = source->GetUInt32Value(UNIT_NPC_FLAGS);
    if (showQuests && npcflags & UNIT_NPC_FLAG_QUESTGIVER)
      PrepareQuestMenu(source->GetGUID());
  } else if (source->GetTypeId() == TYPEID_GAMEOBJECT)
    if (showQuests &&
        source->ToGameObject()->GetGoType() == GAMEOBJECT_TYPE_QUESTGIVER)
      PrepareQuestMenu(source->GetGUID());

  for (GossipMenuItemsContainer::const_iterator itr = menuItemBounds.first;
       itr != menuItemBounds.second; ++itr) {
    bool canTalk = true;
    if (!sConditionMgr->IsObjectMeetToConditions(this, source,
                                                 itr->second.Conditions))
      continue;

    if (Creature *creature = source->ToCreature()) {
      if (!(itr->second.OptionNpcFlag & npcflags))
        continue;

      switch (itr->second.OptionType) {
      case GOSSIP_OPTION_SPIRITHEALER:
        if (!isDead())
          canTalk = false;
        break;
      case GOSSIP_OPTION_LEARNDUALSPEC:
      case GOSSIP_OPTION_DUALSPEC_INFO:
        if (!(GetSpecsCount() == 1 && creature->CanResetTalents(this) &&
              !(getLevel() < sWorld->getIntConfig(CONFIG_MIN_DUALSPEC_LEVEL))))
          canTalk = false;
        break;
      case GOSSIP_OPTION_UNLEARNTALENTS:
        if (!creature->CanResetTalents(this))
          canTalk = false;
        break;
      case GOSSIP_OPTION_UNLEARNPETTALENTS:
        if (!GetPet() || GetPet()->getPetType() != HUNTER_PET ||
            GetPet()->m_spells.size() <= 1 || !creature->CanResetTalents(this))
          canTalk = false;
        break;
      case GOSSIP_OPTION_TAXIVENDOR:
        if (GetSession()->SendLearnNewTaxiNode(creature))
          return;
        break;
      case GOSSIP_OPTION_BATTLEFIELD:
        if (!creature->isCanInteractWithBattleMaster(this, false))
          canTalk = false;
        break;
      case GOSSIP_OPTION_STABLEPET:
        if (getClass() != CLASS_HUNTER)
          canTalk = false;
        break;
      case GOSSIP_OPTION_ARMORER:
      case GOSSIP_OPTION_QUESTGIVER:
        canTalk = false;
        break;
      case GOSSIP_OPTION_TRAINER:
        if (creature->GetCreatureTemplate()->trainer_class &&
            !creature->IsClassTrainerOf(this))
          canTalk = false;
        break;
      case GOSSIP_OPTION_GOSSIP:
      case GOSSIP_OPTION_SPIRITGUIDE:
      case GOSSIP_OPTION_INNKEEPER:
      case GOSSIP_OPTION_BANKER:
      case GOSSIP_OPTION_PETITIONER:
      case GOSSIP_OPTION_TABARDDESIGNER:
      case GOSSIP_OPTION_AUCTIONEER:
      case GOSSIP_OPTION_VENDOR:
        break; // no checks
      case GOSSIP_OPTION_OUTDOORPVP:
        if (!sOutdoorPvPMgr->CanTalkTo(this, creature, itr->second))
          canTalk = false;
        break;
      default:
        LOG_ERROR("sql.sql",
                  "Creature entry %u has unknown gossip option %u for menu %u.",
                  creature->GetEntry(), itr->second.OptionType,
                  itr->second.MenuID);
        canTalk = false;
        break;
      }
    } else if (GameObject *go = source->ToGameObject()) {
      switch (itr->second.OptionType) {
      case GOSSIP_OPTION_GOSSIP:
        if (go->GetGoType() != GAMEOBJECT_TYPE_QUESTGIVER &&
            go->GetGoType() != GAMEOBJECT_TYPE_GOOBER)
          canTalk = false;
        break;
      default:
        canTalk = false;
        break;
      }
    }

    if (canTalk) {
      std::string strOptionText, strBoxText;
      BroadcastText const *optionBroadcastText =
          sObjectMgr->GetBroadcastText(itr->second.OptionBroadcastTextID);
      BroadcastText const *boxBroadcastText =
          sObjectMgr->GetBroadcastText(itr->second.BoxBroadcastTextID);
      LocaleConstant locale = GetSession()->GetSessionDbLocaleIndex();

      if (optionBroadcastText)
        strOptionText = optionBroadcastText->GetText(locale, getGender());
      else
        strOptionText = itr->second.OptionText;

      if (boxBroadcastText)
        strBoxText = boxBroadcastText->GetText(locale, getGender());
      else
        strBoxText = itr->second.BoxText;

      if (locale != DEFAULT_LOCALE) {
        if (!optionBroadcastText) {
          /// Find localizations from database.
          if (GossipMenuItemsLocale const *gossipMenuLocale =
                  sObjectMgr->GetGossipMenuItemsLocale(menuId,
                                                       itr->second.OptionID))
            ObjectMgr::GetLocaleString(gossipMenuLocale->OptionText, locale,
                                       strOptionText);
        }

        if (!boxBroadcastText) {
          /// Find localizations from database.
          if (GossipMenuItemsLocale const *gossipMenuLocale =
                  sObjectMgr->GetGossipMenuItemsLocale(menuId,
                                                       itr->second.OptionID))
            ObjectMgr::GetLocaleString(gossipMenuLocale->BoxText, locale,
                                       strBoxText);
        }
      }

      menu->GetGossipMenu().AddMenuItem(
          itr->second.OptionID, itr->second.OptionIcon, strOptionText, 0,
          itr->second.OptionType, strBoxText, itr->second.BoxMoney,
          itr->second.BoxCoded);
      menu->GetGossipMenu().AddGossipMenuItemData(itr->second.OptionID,
                                                  itr->second.ActionMenuID,
                                                  itr->second.ActionPoiID);
    }
  }
}

void Player::SendPreparedGossip(WorldObject *source) {
  if (!source)
    return;

  if (source->GetTypeId() == TYPEID_UNIT) {
    // in case no gossip flag and quest menu not empty, open quest menu (client
    // expect gossip menu with this flag)
    if (!source->ToCreature()->HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP) &&
        !PlayerTalkClass->GetQuestMenu().Empty()) {
      SendPreparedQuest(source->GetGUID());
      return;
    }
  } else if (source->GetTypeId() == TYPEID_GAMEOBJECT) {
    // probably need to find a better way here
    if (!PlayerTalkClass->GetGossipMenu().GetMenuId() &&
        !PlayerTalkClass->GetQuestMenu().Empty()) {
      SendPreparedQuest(source->GetGUID());
      return;
    }
  }

  // in case non empty gossip menu (that not included quests list size) show it
  // (quest entries from quest menu will be included in list)

  uint32 textId = GetGossipTextId(source);

  if (uint32 menuId = PlayerTalkClass->GetGossipMenu().GetMenuId())
    textId = GetGossipTextId(menuId, source);

  PlayerTalkClass->SendGossipMenu(textId, source->GetGUID());
}

void Player::OnGossipSelect(WorldObject *source, uint32 gossipListId,
                            uint32 menuId) {
  GossipMenu &gossipMenu = PlayerTalkClass->GetGossipMenu();

  // if not same, then something funky is going on
  if (menuId != gossipMenu.GetMenuId())
    return;

  GossipMenuItem const *item = gossipMenu.GetItem(gossipListId);
  if (!item)
    return;

  uint32 gossipOptionId = item->OptionType;
  ObjectGuid guid = source->GetGUID();

  if (source->GetTypeId() == TYPEID_GAMEOBJECT) {
    if (gossipOptionId > GOSSIP_OPTION_QUESTGIVER) {
      LOG_ERROR("entities.player",
                "Player '%s' (%s) requests invalid gossip option for "
                "GameObject (Entry: %u)",
                GetName().c_str(), GetGUID().ToString().c_str(),
                source->GetEntry());
      return;
    }
  }

  GossipMenuItemData const *menuItemData = gossipMenu.GetItemData(gossipListId);
  if (!menuItemData)
    return;

  int32 cost = int32(item->BoxMoney);
  if (!HasEnoughMoney(int64(cost))) {
    SendBuyError(BUY_ERR_NOT_ENOUGHT_MONEY, nullptr, 0, 0);
    PlayerTalkClass->SendCloseGossip();
    return;
  }

  switch (gossipOptionId) {
  case GOSSIP_OPTION_GOSSIP:
  case GOSSIP_OPTION_DUALSPEC_INFO: {
    if (menuItemData->GossipActionPoi)
      PlayerTalkClass->SendPointOfInterest(menuItemData->GossipActionPoi);

    if (menuItemData->GossipActionMenuId) {
      PrepareGossipMenu(source, menuItemData->GossipActionMenuId);
      SendPreparedGossip(source);
    }

    break;
  }
  case GOSSIP_OPTION_OUTDOORPVP:
    sOutdoorPvPMgr->HandleGossipOption(this, source->ToCreature(),
                                       gossipListId);
    break;
  case GOSSIP_OPTION_SPIRITHEALER:
    if (isDead())
      source->ToCreature()->CastSpell(source->ToCreature(), 17251, GetGUID());
    break;
  case GOSSIP_OPTION_QUESTGIVER:
    PrepareQuestMenu(guid);
    SendPreparedQuest(guid);
    break;
  case GOSSIP_OPTION_VENDOR:
  case GOSSIP_OPTION_ARMORER:
    GetSession()->SendListInventory(guid);
    break;
  case GOSSIP_OPTION_STABLEPET:
    GetSession()->SendStablePet(guid);
    break;
  case GOSSIP_OPTION_TRAINER:
    GetSession()->SendTrainerList(
        source->ToCreature(), sObjectMgr->GetCreatureTrainerForGossipOption(
                                  source->GetEntry(), menuId, gossipListId));
    break;
  case GOSSIP_OPTION_LEARNDUALSPEC:
    if (GetSpecsCount() == 1 &&
        getLevel() >= sWorld->getIntConfig(CONFIG_MIN_DUALSPEC_LEVEL)) {
      // Cast spells that teach dual spec
      // Both are also ImplicitTarget self and must be cast by player
      CastSpell(this, 63680, GetGUID());
      CastSpell(this, 63624, GetGUID());

      PrepareGossipMenu(source, menuItemData->GossipActionMenuId);
      SendPreparedGossip(source);
    }
    break;
  case GOSSIP_OPTION_UNLEARNTALENTS:
    PlayerTalkClass->SendCloseGossip();
    SendTalentWipeConfirm(guid);
    break;
  case GOSSIP_OPTION_UNLEARNPETTALENTS:
    PlayerTalkClass->SendCloseGossip();
    ResetPetTalents();
    break;
  case GOSSIP_OPTION_TAXIVENDOR:
    GetSession()->SendTaxiMenu(source->ToCreature());
    break;
  case GOSSIP_OPTION_INNKEEPER:
    PlayerTalkClass->SendCloseGossip();
    SetBindPoint(guid);
    break;
  case GOSSIP_OPTION_BANKER:
    GetSession()->SendShowBank(guid);
    break;
  case GOSSIP_OPTION_PETITIONER:
    PlayerTalkClass->SendCloseGossip();
    GetSession()->SendPetitionShowList(guid);
    break;
  case GOSSIP_OPTION_TABARDDESIGNER:
    PlayerTalkClass->SendCloseGossip();
    GetSession()->SendTabardVendorActivate(guid);
    break;
  case GOSSIP_OPTION_AUCTIONEER:
    GetSession()->SendAuctionHello(guid, source->ToCreature());
    break;
  case GOSSIP_OPTION_SPIRITGUIDE:
    PrepareGossipMenu(source);
    SendPreparedGossip(source);
    break;
  case GOSSIP_OPTION_BATTLEFIELD: {
    BattlegroundTypeId bgTypeId =
        sBattlegroundMgr->GetBattleMasterBG(source->GetEntry());

    if (bgTypeId == BATTLEGROUND_TYPE_NONE) {
      LOG_ERROR("entities.player",
                "Player '%s' (%s) requested battlegroundlist from an invalid "
                "creature (%s)",
                GetName().c_str(), GetGUID().ToString().c_str(),
                source->GetGUID().ToString().c_str());
      return;
    }

    sBattlegroundMgr->SendBattlegroundList(source->GetGUID(), this,
                                           BattlegroundTypeId(bgTypeId));
    break;
  }
  }

  ModifyMoney(-cost);
}

uint32 Player::GetGossipTextId(WorldObject *source) {
  if (!source)
    return DEFAULT_GOSSIP_MESSAGE;

  return GetGossipTextId(GetDefaultGossipMenuForSource(source), source);
}

uint32 Player::GetGossipTextId(uint32 menuId, WorldObject *source) {
  uint32 textId = DEFAULT_GOSSIP_MESSAGE;

  if (!menuId)
    return textId;

  GossipMenusMapBounds menuBounds = sObjectMgr->GetGossipMenusMapBounds(menuId);

  for (GossipMenusContainer::const_iterator itr = menuBounds.first;
       itr != menuBounds.second; ++itr) {
    if (sConditionMgr->IsObjectMeetToConditions(this, source,
                                                itr->second.Conditions))
      textId = itr->second.TextID;
  }

  return textId;
}

uint32 Player::GetDefaultGossipMenuForSource(WorldObject *source) {
  switch (source->GetTypeId()) {
  case TYPEID_UNIT:
    return source->ToCreature()->GetCreatureTemplate()->GossipMenuId;
  case TYPEID_GAMEOBJECT:
    return source->ToGameObject()->GetGOInfo()->GetGossipMenuId();
  default:
    break;
  }

  return 0;
}

/*********************************************************/
/***                    QUEST SYSTEM                   ***/
/*********************************************************/

void Player::PrepareQuestMenu(ObjectGuid guid) {
  QuestRelationResult objectQR;
  QuestRelationResult objectQIR;

  // pets also can have quests
  Creature *creature = ObjectAccessor::GetCreatureOrPetOrVehicle(*this, guid);
  if (creature) {
    objectQR = sObjectMgr->GetCreatureQuestRelations(creature->GetEntry());
    objectQIR =
        sObjectMgr->GetCreatureQuestInvolvedRelations(creature->GetEntry());
  } else {
    // we should obtain map pointer from GetMap() in 99% of cases. Special case
    // only for quests which cast teleport spells on player
    Map *_map =
        IsInWorld() ? GetMap() : sMapMgr->FindMap(GetMapId(), GetInstanceId());
    ASSERT(_map);
    GameObject *gameObject = _map->GetGameObject(guid);
    if (gameObject) {
      objectQR = sObjectMgr->GetGOQuestRelations(gameObject->GetEntry());
      objectQIR =
          sObjectMgr->GetGOQuestInvolvedRelations(gameObject->GetEntry());
    } else
      return;
  }

  QuestMenu &qm = PlayerTalkClass->GetQuestMenu();
  qm.ClearMenu();

  for (uint32 quest_id : objectQIR) {
    QuestStatus status = GetQuestStatus(quest_id);
    if (status == QUEST_STATUS_COMPLETE)
      qm.AddMenuItem(quest_id, 4);
    else if (status == QUEST_STATUS_INCOMPLETE)
      qm.AddMenuItem(quest_id, 4);
    // else if (status == QUEST_STATUS_AVAILABLE)
    //     qm.AddMenuItem(quest_id, 2);
  }

  for (uint32 quest_id : objectQR) {
    Quest const *quest = sObjectMgr->GetQuestTemplate(quest_id);
    if (!quest)
      continue;

    if (!CanTakeQuest(quest, false))
      continue;

    if (quest->IsAutoComplete() &&
        (!quest->IsRepeatable() || quest->IsDaily() || quest->IsWeekly() ||
         quest->IsMonthly()))
      qm.AddMenuItem(quest_id, 0);
    else if (quest->IsAutoComplete())
      qm.AddMenuItem(quest_id, 4);
    else if (GetQuestStatus(quest_id) == QUEST_STATUS_NONE)
      qm.AddMenuItem(quest_id, 2);
  }
}

void Player::SendPreparedQuest(ObjectGuid guid) {
  QuestMenu &questMenu = PlayerTalkClass->GetQuestMenu();
  if (questMenu.Empty())
    return;

  // single element case
  if (questMenu.GetMenuItemCount() == 1) {
    QuestMenuItem const &qmi0 = questMenu.GetItem(0);
    uint32 questId = qmi0.QuestId;

    // Auto open -- maybe also should verify there is no greeting
    if (Quest const *quest = sObjectMgr->GetQuestTemplate(questId)) {
      if (qmi0.QuestIcon == 4)
        PlayerTalkClass->SendQuestGiverRequestItems(
            quest, guid, CanRewardQuest(quest, false), true);
      // Send completable on repeatable and autoCompletable quest if player
      // don't have quest
      /// @todo verify if check for !quest->IsDaily() is really correct
      /// (possibly not)
      else {
        Object *object = ObjectAccessor::GetObjectByTypeMask(
            *this, guid, TYPEMASK_UNIT | TYPEMASK_GAMEOBJECT | TYPEMASK_ITEM);
        if (!object || (!object->hasQuest(questId) &&
                        !object->hasInvolvedQuest(questId))) {
          PlayerTalkClass->SendCloseGossip();
          return;
        }

        if (quest->IsAutoAccept() && CanAddQuest(quest, true) &&
            CanTakeQuest(quest, true))
          AddQuestAndCheckCompletion(quest, object);

        if (quest->IsAutoComplete() && quest->IsRepeatable() &&
            !quest->IsDailyOrWeekly())
          PlayerTalkClass->SendQuestGiverRequestItems(
              quest, guid, CanCompleteRepeatableQuest(quest), true);
        else {
          SetPopupQuestId(0);
          PlayerTalkClass->SendQuestGiverQuestDetails(quest, guid, true, false);
        }
      }
    }
  }
  // multiple entries
  else {
    QEmote qe;
    qe._Delay = 0;
    qe._Emote = 0;
    std::string title = "";

    // need pet case for some quests
    Creature *creature = ObjectAccessor::GetCreatureOrPetOrVehicle(*this, guid);
    if (creature) {
      uint32 textid = GetGossipTextId(creature);
      GossipText const *gossiptext = sObjectMgr->GetGossipText(textid);
      if (!gossiptext) {
        qe._Delay = 0; // TEXTEMOTE_MESSAGE;              //zyg: player emote
        qe._Emote = 0; // TEXTEMOTE_HELLO;                //zyg: NPC emote
        title.clear();
      } else {
        qe = gossiptext->Options[0].Emotes[0];

        if (!gossiptext->Options[0].Text_0.empty()) {
          title = gossiptext->Options[0].Text_0;

          LocaleConstant localeConstant =
              GetSession()->GetSessionDbLocaleIndex();
          if (localeConstant != LOCALE_enUS)
            if (NpcTextLocale const *nl = sObjectMgr->GetNpcTextLocale(textid))
              ObjectMgr::GetLocaleString(nl->Text_0[0], localeConstant, title);
        } else {
          title = gossiptext->Options[0].Text_1;

          LocaleConstant localeConstant =
              GetSession()->GetSessionDbLocaleIndex();
          if (localeConstant != LOCALE_enUS)
            if (NpcTextLocale const *nl = sObjectMgr->GetNpcTextLocale(textid))
              ObjectMgr::GetLocaleString(nl->Text_1[0], localeConstant, title);
        }
      }
    }
    PlayerTalkClass->SendQuestGiverQuestList(qe, title, guid);
  }
}

bool Player::IsActiveQuest(uint32 quest_id) const {
  return m_QuestStatus.find(quest_id) != m_QuestStatus.end();
}

Quest const *Player::GetNextQuest(ObjectGuid guid, Quest const *quest) const {
  QuestRelationResult quests;
  uint32 nextQuestID = quest->GetNextQuestInChain();

  switch (guid.GetHigh()) {
  case HighGuid::Player:
    ASSERT(quest->HasFlag(QUEST_FLAGS_AUTOCOMPLETE));
    return sObjectMgr->GetQuestTemplate(nextQuestID);
  case HighGuid::Unit:
  case HighGuid::Pet:
  case HighGuid::Vehicle: {
    if (Creature *creature =
            ObjectAccessor::GetCreatureOrPetOrVehicle(*this, guid))
      quests = sObjectMgr->GetCreatureQuestRelations(creature->GetEntry());
    else
      return nullptr;
    break;
  }
  case HighGuid::GameObject: {
    // we should obtain map pointer from GetMap() in 99% of cases. Special case
    // only for quests which cast teleport spells on player
    Map *_map =
        IsInWorld() ? GetMap() : sMapMgr->FindMap(GetMapId(), GetInstanceId());
    ASSERT(_map);
    if (GameObject *gameObject = _map->GetGameObject(guid))
      quests = sObjectMgr->GetGOQuestRelations(gameObject->GetEntry());
    else
      return nullptr;
    break;
  }
  default:
    return nullptr;
  }

  // for unit and go state
  if (nextQuestID)
    if (quests.HasQuest(nextQuestID))
      return sObjectMgr->GetQuestTemplate(nextQuestID);

  return nullptr;
}

bool Player::CanSeeStartQuest(Quest const *quest) const {
  if (!DisableMgr::IsDisabledFor(DISABLE_TYPE_QUEST, quest->GetQuestId(),
                                 this) &&
      SatisfyQuestClass(quest, false) && SatisfyQuestRace(quest, false) &&
      SatisfyQuestSkill(quest, false) &&
      SatisfyQuestExclusiveGroup(quest, false) &&
      SatisfyQuestReputation(quest, false) &&
      SatisfyQuestDependentQuests(quest, false) &&
      SatisfyQuestDay(quest, false) && SatisfyQuestWeek(quest, false) &&
      SatisfyQuestMonth(quest, false) && SatisfyQuestSeasonal(quest, false)) {
    return getLevel() +
               sWorld->getIntConfig(CONFIG_QUEST_HIGH_LEVEL_HIDE_DIFF) >=
           quest->GetMinLevel();
  }

  return false;
}

bool Player::CanTakeQuest(Quest const *quest, bool msg) const {
  return !DisableMgr::IsDisabledFor(DISABLE_TYPE_QUEST, quest->GetQuestId(),
                                    this) &&
         SatisfyQuestStatus(quest, msg) &&
         SatisfyQuestExclusiveGroup(quest, msg) &&
         SatisfyQuestClass(quest, msg) && SatisfyQuestRace(quest, msg) &&
         SatisfyQuestLevel(quest, msg) && SatisfyQuestSkill(quest, msg) &&
         SatisfyQuestReputation(quest, msg) &&
         SatisfyQuestDependentQuests(quest, msg) &&
         SatisfyQuestTimed(quest, msg) && SatisfyQuestDay(quest, msg) &&
         SatisfyQuestWeek(quest, msg) && SatisfyQuestMonth(quest, msg) &&
         SatisfyQuestSeasonal(quest, msg) && SatisfyQuestConditions(quest, msg);
}

bool Player::CanAddQuest(Quest const *quest, bool msg) const {
  if (!SatisfyQuestLog(msg))
    return false;

  uint32 srcitem = quest->GetSrcItemId();
  if (srcitem > 0) {
    uint32 count = quest->GetSrcItemCount();
    ItemPosCountVec dest;
    InventoryResult msg2 =
        CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, srcitem, count);

    // player already have max number (in most case 1) source item, no
    // additional item needed and quest can be added.
    if (msg2 == EQUIP_ERR_ITEM_MAX_COUNT)
      return true;
    if (msg2 != EQUIP_ERR_OK) {
      SendEquipError(msg2, nullptr, nullptr, srcitem);
      return false;
    }
  }
  return true;
}

bool Player::CanCompleteQuest(uint32 quest_id) {
  if (quest_id) {
    Quest const *qInfo = sObjectMgr->GetQuestTemplate(quest_id);
    if (!qInfo)
      return false;

    if (!qInfo->IsRepeatable() && GetQuestRewardStatus(quest_id))
      return false; // not allow re-complete quest

    // auto complete quest
    if (qInfo->IsAutoComplete() && CanTakeQuest(qInfo, false))
      return true;

    QuestStatusMap::iterator itr = m_QuestStatus.find(quest_id);
    if (itr == m_QuestStatus.end())
      return false;

    QuestStatusData &q_status = itr->second;

    if (q_status.Status == QUEST_STATUS_INCOMPLETE) {
      if (qInfo->HasSpecialFlag(QUEST_SPECIAL_FLAGS_DELIVER)) {
        for (uint8 i = 0; i < QUEST_ITEM_OBJECTIVES_COUNT; i++) {
          if (qInfo->RequiredItemCount[i] != 0 &&
              q_status.ItemCount[i] < qInfo->RequiredItemCount[i])
            return false;
        }
      }

      if (qInfo->HasSpecialFlag(QUEST_SPECIAL_FLAGS_KILL |
                                QUEST_SPECIAL_FLAGS_CAST |
                                QUEST_SPECIAL_FLAGS_SPEAKTO)) {
        for (uint8 i = 0; i < QUEST_OBJECTIVES_COUNT; i++) {
          if (qInfo->RequiredNpcOrGo[i] == 0)
            continue;

          if (qInfo->RequiredNpcOrGoCount[i] != 0 &&
              q_status.CreatureOrGOCount[i] < qInfo->RequiredNpcOrGoCount[i])
            return false;
        }
      }

      if (qInfo->HasSpecialFlag(QUEST_SPECIAL_FLAGS_PLAYER_KILL))
        if (qInfo->GetPlayersSlain() != 0 &&
            q_status.PlayerCount < qInfo->GetPlayersSlain())
          return false;

      if (qInfo->HasSpecialFlag(QUEST_SPECIAL_FLAGS_EXPLORATION_OR_EVENT) &&
          !q_status.Explored)
        return false;

      if (qInfo->HasSpecialFlag(QUEST_SPECIAL_FLAGS_TIMED) &&
          q_status.Timer == 0)
        return false;

      if (qInfo->GetRewOrReqMoney() < 0) {
        if (!HasEnoughMoney(-int64(qInfo->GetRewOrReqMoney())))
          return false;
      }

      uint32 repFacId = qInfo->GetRepObjectiveFaction();
      if (repFacId && GetReputationMgr().GetReputation(repFacId) <
                          qInfo->GetRepObjectiveValue())
        return false;

      return true;
    }
  }
  return false;
}

bool Player::CanCompleteRepeatableQuest(Quest const *quest) {
  // Solve problem that player don't have the quest and try complete it.
  // if repeatable she must be able to complete event if player don't have it.
  // Seem that all repeatable quest are DELIVER Flag so, no need to add more.
  if (!CanTakeQuest(quest, false))
    return false;

  if (quest->HasSpecialFlag(QUEST_SPECIAL_FLAGS_DELIVER))
    for (uint8 i = 0; i < QUEST_ITEM_OBJECTIVES_COUNT; i++)
      if (quest->RequiredItemId[i] && quest->RequiredItemCount[i] &&
          !HasItemCount(quest->RequiredItemId[i], quest->RequiredItemCount[i]))
        return false;

  if (!CanRewardQuest(quest, false))
    return false;

  return true;
}

bool Player::CanRewardQuest(Quest const *quest, bool msg) {
  // not auto complete quest and not completed quest (only cheating case, then
  // ignore without message)
  if (!quest->IsDFQuest() && !quest->IsAutoComplete() &&
      GetQuestStatus(quest->GetQuestId()) != QUEST_STATUS_COMPLETE)
    return false;

  // daily quest can't be rewarded (25 daily quest already completed)
  if (!SatisfyQuestDay(quest, true) || !SatisfyQuestWeek(quest, true) ||
      !SatisfyQuestMonth(quest, true) || !SatisfyQuestSeasonal(quest, true))
    return false;

  // rewarded and not repeatable quest (only cheating case, then ignore without
  // message)
  if (GetQuestRewardStatus(quest->GetQuestId()))
    return false;

  // prevent receive reward with quest items in bank
  if (quest->HasSpecialFlag(QUEST_SPECIAL_FLAGS_DELIVER)) {
    for (uint8 i = 0; i < QUEST_ITEM_OBJECTIVES_COUNT; i++) {
      if (quest->RequiredItemCount[i] != 0 &&
          GetItemCount(quest->RequiredItemId[i]) <
              quest->RequiredItemCount[i]) {
        if (msg)
          SendEquipError(EQUIP_ERR_ITEM_NOT_FOUND, nullptr, nullptr,
                         quest->RequiredItemId[i]);
        return false;
      }
    }
  }

  for (uint8 i = 0; i < QUEST_REQUIRED_CURRENCY_COUNT; i++)
    if (quest->RequiredCurrencyId[i] &&
        !HasCurrency(quest->RequiredCurrencyId[i],
                     quest->RequiredCurrencyCount[i]))
      return false;

  // prevent receive reward with low money and GetRewOrReqMoney() < 0
  if (quest->GetRewOrReqMoney() < 0 &&
      !HasEnoughMoney(-int64(quest->GetRewOrReqMoney())))
    return false;

  // dungeon finder quests cannot be rewarded when hit weekly currency limit
  if (quest->IsDFQuest())
    for (uint8 i = 0; i < QUEST_REWARD_CURRENCY_COUNT; i++)
      if (CurrencyTypesEntry const *currency =
              sCurrencyTypesStore.LookupEntry(quest->RewardCurrencyId[i]))
        if (GetCurrencyOnWeek(quest->RewardCurrencyId[i], false) ==
            GetCurrencyWeekCap(currency))
          return false;

  return true;
}

void Player::AddQuestAndCheckCompletion(Quest const *quest,
                                        Object *questGiver) {
  AddQuest(quest, questGiver);

  if (CanCompleteQuest(quest->GetQuestId()))
    CompleteQuest(quest->GetQuestId());

  if (!questGiver)
    return;

  switch (questGiver->GetTypeId()) {
  case TYPEID_UNIT:
    PlayerTalkClass->ClearMenus();
    questGiver->ToCreature()->AI()->QuestAccept(this, quest);
    break;
  case TYPEID_ITEM:
  case TYPEID_CONTAINER: {
    Item *item = static_cast<Item *>(questGiver);
    sScriptMgr->OnQuestAccept(this, item, quest);

    // There are two cases where the source item is not destroyed when the quest
    // is accepted:
    // - It is required to finish the quest, and is an unique item
    // - It is the same item present in the source item field (item that would
    // be given on quest accept)
    bool destroyItem = true;

    for (int i = 0; i < QUEST_ITEM_OBJECTIVES_COUNT; ++i) {
      if (quest->RequiredItemId[i] == item->GetEntry() &&
          item->GetTemplate()->GetMaxCount() > 0) {
        destroyItem = false;
        break;
      }
    }

    if (quest->GetSrcItemId() == item->GetEntry())
      destroyItem = false;

    if (destroyItem)
      DestroyItem(item->GetBagSlot(), item->GetSlot(), true);

    break;
  }
  case TYPEID_GAMEOBJECT:
    PlayerTalkClass->ClearMenus();
    questGiver->ToGameObject()->AI()->QuestAccept(this, quest);
    break;
  default:
    break;
  }
}

bool Player::CanRewardQuest(Quest const *quest, uint32 reward, bool msg) {
  // prevent receive reward with quest items in bank or for not completed quest
  if (!CanRewardQuest(quest, msg))
    return false;

  ItemPosCountVec dest;
  if (quest->GetRewChoiceItemsCount() > 0) {
    if (quest->RewardChoiceItemId[reward]) {
      InventoryResult res = CanStoreNewItem(
          NULL_BAG, NULL_SLOT, dest, quest->RewardChoiceItemId[reward],
          quest->RewardChoiceItemCount[reward]);
      if (res != EQUIP_ERR_OK) {
        if (msg)
          SendQuestFailed(quest->GetQuestId(), res);

        return false;
      }
    }
  }

  if (quest->GetRewItemsCount() > 0) {
    for (uint32 i = 0; i < quest->GetRewItemsCount(); ++i) {
      if (quest->RewardItemId[i]) {
        InventoryResult res =
            CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, quest->RewardItemId[i],
                            quest->RewardItemIdCount[i]);
        if (res != EQUIP_ERR_OK) {
          if (msg)
            SendQuestFailed(quest->GetQuestId(), res);

          return false;
        }
      }
    }
  }

  return true;
}

void Player::AddQuest(Quest const *quest, Object *questGiver) {
  uint16 log_slot = FindQuestSlot(0);

  if (log_slot >=
      MAX_QUEST_LOG_SIZE) // Player does not have any free slot in the quest log
    return;

  uint32 quest_id = quest->GetQuestId();

  // if not exist then created with set uState == NEW and rewarded=false
  QuestStatusData &questStatusData = m_QuestStatus[quest_id];

  // check for repeatable quests status reset
  questStatusData.Status = QUEST_STATUS_INCOMPLETE;
  questStatusData.Explored = false;

  if (quest->HasSpecialFlag(QUEST_SPECIAL_FLAGS_DELIVER)) {
    for (uint8 i = 0; i < QUEST_ITEM_OBJECTIVES_COUNT; ++i)
      questStatusData.ItemCount[i] = 0;
  }

  if (quest->HasSpecialFlag(QUEST_SPECIAL_FLAGS_KILL |
                            QUEST_SPECIAL_FLAGS_CAST |
                            QUEST_SPECIAL_FLAGS_SPEAKTO)) {
    for (uint8 i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
      questStatusData.CreatureOrGOCount[i] = 0;
  }

  if (quest->HasSpecialFlag(QUEST_SPECIAL_FLAGS_PLAYER_KILL))
    questStatusData.PlayerCount = 0;

  GiveQuestSourceItem(quest);
  AdjustQuestReqItemCount(quest, questStatusData);

  if (quest->GetRepObjectiveFaction())
    if (FactionEntry const *factionEntry =
            sFactionStore.LookupEntry(quest->GetRepObjectiveFaction()))
      GetReputationMgr().SetVisible(factionEntry);

  if (quest->GetRepObjectiveFaction2())
    if (FactionEntry const *factionEntry =
            sFactionStore.LookupEntry(quest->GetRepObjectiveFaction2()))
      GetReputationMgr().SetVisible(factionEntry);

  uint32 qtime = 0;
  if (quest->HasSpecialFlag(QUEST_SPECIAL_FLAGS_TIMED)) {
    uint32 timeAllowed = quest->GetTimeAllowed();

    // shared timed quest
    if (questGiver && questGiver->GetTypeId() == TYPEID_PLAYER)
      timeAllowed =
          questGiver->ToPlayer()->getQuestStatusMap()[quest_id].Timer /
          IN_MILLISECONDS;

    AddTimedQuest(quest_id);
    questStatusData.Timer = timeAllowed * IN_MILLISECONDS;
    qtime = static_cast<uint32>(GameTime::GetGameTime()) + timeAllowed;
  } else
    questStatusData.Timer = 0;

  if (quest->HasFlag(QUEST_FLAGS_FLAGS_PVP)) {
    pvpInfo.IsHostile = true;
    UpdatePvPState();
  }

  SetQuestSlot(log_slot, quest_id, qtime);

  m_QuestStatusSave[quest_id] = QUEST_DEFAULT_SAVE_TYPE;

  StartTimedAchievement(ACHIEVEMENT_TIMED_TYPE_QUEST, quest_id);

  SendQuestUpdate(quest_id);

  SendQuestGiverStatusMultiple();

  if (sWorld->getBoolConfig(
          CONFIG_QUEST_ENABLE_QUEST_TRACKER)) // check if Quest Tracker is
                                              // enabled
  {
    // prepare Quest Tracker data
    CharacterDatabasePreparedStatement *stmt =
        CharacterDatabase.GetPreparedStatement(CHAR_INS_QUEST_TRACK);
    stmt->SetData(0, quest_id);
    stmt->SetData(1, GetGUID().GetCounter());
    stmt->SetData(2, GitRevision::GetHash());
    stmt->SetData(3, GitRevision::GetDate());

    // add to Quest Tracker
    CharacterDatabase.Execute(stmt);
  }

  sScriptMgr->OnQuestStatusChange(this, quest_id);
}

void Player::CompleteQuest(uint32 quest_id) {
  if (quest_id) {
    SetQuestStatus(quest_id, QUEST_STATUS_COMPLETE);

    uint16 log_slot = FindQuestSlot(quest_id);
    if (log_slot < MAX_QUEST_LOG_SIZE)
      SetQuestSlotState(log_slot, QUEST_STATE_COMPLETE);

    if (Quest const *qInfo = sObjectMgr->GetQuestTemplate(quest_id))
      if (qInfo->HasFlag(QUEST_FLAGS_TRACKING))
        RewardQuest(qInfo, 0, this, false);
  }

  if (sWorld->getBoolConfig(
          CONFIG_QUEST_ENABLE_QUEST_TRACKER)) // check if Quest Tracker is
                                              // enabled
  {
    // prepare Quest Tracker datas
    CharacterDatabasePreparedStatement *stmt =
        CharacterDatabase.GetPreparedStatement(
            CHAR_UPD_QUEST_TRACK_COMPLETE_TIME);
    stmt->SetData(0, quest_id);
    stmt->SetData(1, GetGUID().GetCounter());

    // add to Quest Tracker
    CharacterDatabase.Execute(stmt);
  }
}

void Player::IncompleteQuest(uint32 quest_id) {
  if (quest_id) {
    SetQuestStatus(quest_id, QUEST_STATUS_INCOMPLETE);

    uint16 log_slot = FindQuestSlot(quest_id);
    if (log_slot < MAX_QUEST_LOG_SIZE)
      RemoveQuestSlotState(log_slot, QUEST_STATE_COMPLETE);
  }
}

void Player::RewardQuest(Quest const *quest, uint32 reward, Object *questGiver,
                         bool announce) {
  // this THING should be here to protect code from quest, which cast on player
  // far teleport as a reward should work fine, cause far teleport will be
  // executed in Player::Update()
  SetCanDelayTeleport(true);

  uint32 quest_id = quest->GetQuestId();

  for (uint8 i = 0; i < QUEST_ITEM_OBJECTIVES_COUNT; ++i) {
    if (ItemTemplate const *itemTemplate =
            sObjectMgr->GetItemTemplate(quest->RequiredItemId[i])) {
      if (quest->RequiredItemCount[i] > 0 &&
          itemTemplate->GetBonding() == BIND_QUEST && !quest->IsRepeatable() &&
          !HasQuestForItem(quest->RequiredItemId[i], quest_id, true))
        DestroyItemCount(quest->RequiredItemId[i], 9999, true);
      else
        DestroyItemCount(quest->RequiredItemId[i], quest->RequiredItemCount[i],
                         true);
    }
  }
  for (uint8 i = 0; i < QUEST_REQUIRED_CURRENCY_COUNT; ++i)
    if (quest->RequiredCurrencyId[i])
      ModifyCurrency(quest->RequiredCurrencyId[i],
                     -int32(quest->RequiredCurrencyCount[i]));

  for (uint8 i = 0; i < QUEST_SOURCE_ITEM_IDS_COUNT; ++i) {
    if (ItemTemplate const *itemTemplate =
            sObjectMgr->GetItemTemplate(quest->ItemDrop[i])) {
      if (quest->ItemDropQuantity[i] > 0 &&
          itemTemplate->GetBonding() == BIND_QUEST && !quest->IsRepeatable() &&
          !HasQuestForItem(quest->ItemDrop[i], quest_id))
        DestroyItemCount(quest->ItemDrop[i], 9999, true);
      else
        DestroyItemCount(quest->ItemDrop[i], quest->ItemDropQuantity[i], true);
    }
  }

  RemoveTimedQuest(quest_id);

  if (quest->GetRewItemsCount() > 0) {
    for (uint32 i = 0; i < quest->GetRewItemsCount(); ++i) {
      if (uint32 itemId = quest->RewardItemId[i]) {
        ItemPosCountVec dest;
        if (CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, itemId,
                            quest->RewardItemIdCount[i]) == EQUIP_ERR_OK) {
          Item *item = StoreNewItem(dest, itemId, true,
                                    GenerateItemRandomPropertyId(itemId));
          SendNewItem(item, quest->RewardItemIdCount[i], true, false, false,
                      false);
        } else if (quest->IsDFQuest())
          SendItemRetrievalMail(itemId, quest->RewardItemIdCount[i]);
      }
    }
  }

  for (uint8 i = 0; i < QUEST_REWARD_CURRENCY_COUNT; ++i) {
    if (quest->RewardCurrencyId[i]) {
      uint32 rewardCurrencyCount = quest->RewardCurrencyCount[i];

      CurrencyTypesEntry const *currency =
          sCurrencyTypesStore.LookupEntry(quest->RewardCurrencyId[i]);
      if (currency->Flags & CURRENCY_FLAG_HIGH_PRECISION)
        rewardCurrencyCount = rewardCurrencyCount * CURRENCY_PRECISION;

      ModifyCurrency(quest->RewardCurrencyId[i], rewardCurrencyCount,
                     !quest->IsDFQuest());
    }
  }

  if (uint32 skill = quest->GetRewardSkillId())
    UpdateSkillPro(skill, 1000, quest->GetRewardSkillPoints());

  if (quest->GetRewChoiceItemsCount() > 0) {
    if (uint32 itemId = quest->RewardChoiceItemId[reward]) {
      ItemPosCountVec dest;
      if (CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, itemId,
                          quest->RewardChoiceItemCount[reward]) ==
          EQUIP_ERR_OK) {
        Item *item = StoreNewItem(dest, itemId, true,
                                  GenerateItemRandomPropertyId(itemId));
        SendNewItem(item, quest->RewardChoiceItemCount[reward], true, false,
                    false, false);
      }
    }
  }

  uint16 log_slot = FindQuestSlot(quest_id);
  if (log_slot < MAX_QUEST_LOG_SIZE)
    SetQuestSlot(log_slot, 0);

  bool rewarded = IsQuestRewarded(quest_id) && !quest->IsDFQuest();

  // Not give XP in case already completed once repeatable quest
  uint32 XP =
      rewarded
          ? 0
          : uint32(quest->GetXPReward(this) * sWorld->getRate(RATE_XP_QUEST));

  // handle SPELL_AURA_MOD_XP_QUEST_PCT auras
  Unit::AuraEffectList const &ModXPPctAuras =
      GetAuraEffectsByType(SPELL_AURA_MOD_XP_QUEST_PCT);
  for (Unit::AuraEffectList::const_iterator i = ModXPPctAuras.begin();
       i != ModXPPctAuras.end(); ++i)
    AddPct(XP, (*i)->GetAmount());

  if (!IsMaxLevel())
    GiveXP(XP, nullptr);

  if (Guild *guild = GetGuild()) {
    uint32 _xp = quest->GetXPReward(this);
    uint32 guildRep = std::max(uint32(1), uint32(_xp / 450));
    guild->GiveXP(uint32(_xp * sWorld->getRate(RATE_XP_QUEST) *
                         sWorld->getRate(RATE_XP_QUEST_GUILD_MODIFIER)),
                  this);
    guild->GiveReputation(guildRep, this);
    guild->UpdateAchievementCriteria(
        ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUESTS_GUILD, 0, 0, 0, nullptr,
        this);
  }

  // Give player extra money if GetRewOrReqMoney > 0 and get ReqMoney if
  // negative
  if (int32 moneyRew = quest->GetRewOrReqMoney(this)) {
    ModifyMoney(moneyRew);

    if (moneyRew > 0)
      UpdateAchievementCriteria(
          ACHIEVEMENT_CRITERIA_TYPE_MONEY_FROM_QUEST_REWARD, uint32(moneyRew));
  }

  // honor reward
  if (uint32 honor = quest->CalculateHonorGain(getLevel()))
    RewardHonor(nullptr, 0, honor);

  // title reward
  if (quest->GetCharTitleId()) {
    if (CharTitlesEntry const *titleEntry =
            sCharTitlesStore.LookupEntry(quest->GetCharTitleId()))
      SetTitle(titleEntry);
  }

  if (uint32 talents = quest->GetBonusTalents()) {
    AddQuestRewardedTalentCount(talents);
    InitTalentForLevel();
  }

  if (uint32 skill = quest->GetRewardSkillId()) {
    if (HasSkill(skill) &&
        (GetSkillValue(skill) + quest->GetRewardSkillPoints()) <=
            GetMaxSkillValue(skill))
      SetSkill(skill, GetSkillStep(skill),
               GetSkillValue(skill) + quest->GetRewardSkillPoints(),
               GetMaxSkillValue(skill));
  }

  // Send reward mail
  if (uint32 mail_template_id = quest->GetRewMailTemplateId()) {
    /// @todo Poor design of mail system
    CharacterDatabaseTransaction trans = CharacterDatabase.BeginTransaction();
    if (uint32 questMailSender = quest->GetRewMailSenderEntry())
      MailDraft(mail_template_id)
          .SendMailTo(trans, this, questMailSender, MAIL_CHECK_MASK_HAS_BODY,
                      quest->GetRewMailDelaySecs());
    else
      MailDraft(mail_template_id)
          .SendMailTo(trans, this, questGiver, MAIL_CHECK_MASK_HAS_BODY,
                      quest->GetRewMailDelaySecs());
    CharacterDatabase.CommitTransaction(trans);
  }

  if (quest->IsDaily() && !quest->IsDFQuest()) {
    SetDailyQuestStatus(quest_id);
    if (quest->IsDaily()) {
      UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_DAILY_QUEST,
                                quest_id);
      UpdateAchievementCriteria(
          ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_DAILY_QUEST_DAILY, quest_id);
    }
  } else if (quest->IsWeekly())
    SetWeeklyQuestStatus(quest_id);
  else if (quest->IsMonthly())
    SetMonthlyQuestStatus(quest_id);
  else if (quest->IsSeasonal())
    SetSeasonalQuestStatus(quest_id);

  RemoveActiveQuest(quest_id, false);
  if (quest->CanIncreaseRewardedQuestCounters())
    SetRewardedQuest(quest_id);

  if (announce)
    SendQuestReward(quest, questGiver ? questGiver->ToCreature() : nullptr, XP);

  RewardReputation(quest);

  // cast spells after mark quest complete (some spells have quest completed
  // state requirements in spell_area data)
  if (quest->GetRewSpellCast() > 0) {
    SpellInfo const *spellInfo =
        ASSERT_NOTNULL(sSpellMgr->GetSpellInfo(quest->GetRewSpellCast()));
    if (questGiver->isType(TYPEMASK_UNIT) &&
        !spellInfo->HasEffect(SPELL_EFFECT_LEARN_SPELL) &&
        !spellInfo->HasEffect(SPELL_EFFECT_CREATE_ITEM) &&
        !spellInfo->IsSelfCast() &&
        !quest->HasFlag(QUEST_FLAGS_PLAYER_CAST_ON_COMPLETE)) {
      if (Creature *creature = GetMap()->GetCreature(questGiver->GetGUID()))
        creature->CastSpell(this, quest->GetRewSpellCast(), true);
    } else
      CastSpell(this, quest->GetRewSpellCast(), true);
  } else if (quest->GetRewSpell() > 0) {
    SpellInfo const *spellInfo = sSpellMgr->GetSpellInfo(quest->GetRewSpell());
    if (questGiver->isType(TYPEMASK_UNIT) &&
        !spellInfo->HasEffect(SPELL_EFFECT_LEARN_SPELL) &&
        !spellInfo->HasEffect(SPELL_EFFECT_CREATE_ITEM) &&
        !spellInfo->IsSelfCast() &&
        !quest->HasFlag(QUEST_FLAGS_PLAYER_CAST_ON_COMPLETE)) {
      if (Creature *creature = GetMap()->GetCreature(questGiver->GetGUID()))
        creature->CastSpell(this, quest->GetRewSpell(), true);
    } else
      CastSpell(this, quest->GetRewSpell(), true);
  }

  if (quest->GetZoneOrSort() > 0)
    UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUESTS_IN_ZONE,
                              quest->GetZoneOrSort());
  UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST_COUNT);
  UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST,
                            quest->GetQuestId());

  // make full db save
  SaveToDB(false);

  if (quest->HasFlag(QUEST_FLAGS_FLAGS_PVP)) {
    pvpInfo.IsHostile = pvpInfo.IsInHostileArea || HasPvPForcingQuest();
    UpdatePvPState();
  }

  SendQuestUpdate(quest_id);

  // Update QuestGivers
  SendQuestGiverStatusMultiple();

  // lets remove flag for delayed teleports
  SetCanDelayTeleport(false);

  sScriptMgr->OnQuestStatusChange(this, quest_id);
}

void Player::SetRewardedQuest(uint32 quest_id) {
  m_RewardedQuests.insert(quest_id);
  m_RewardedQuestsSave[quest_id] = QUEST_DEFAULT_SAVE_TYPE;
}

void Player::FailQuest(uint32 questId) {
  if (Quest const *quest = sObjectMgr->GetQuestTemplate(questId)) {
    QuestStatus qStatus = GetQuestStatus(questId);

    // we can only fail incomplete quest or...
    if (qStatus != QUEST_STATUS_INCOMPLETE) {
      // completed timed quest with no requirements
      if (qStatus != QUEST_STATUS_COMPLETE ||
          !quest->HasSpecialFlag(QUEST_SPECIAL_FLAGS_TIMED) ||
          !quest->HasSpecialFlag(QUEST_SPECIAL_FLAGS_COMPLETED_AT_START))
        return;
    }

    SetQuestStatus(questId, QUEST_STATUS_FAILED);

    uint16 log_slot = FindQuestSlot(questId);

    if (log_slot < MAX_QUEST_LOG_SIZE) {
      SetQuestSlotTimer(log_slot, 1);
      SetQuestSlotState(log_slot, QUEST_STATE_FAIL);
    }

    if (quest->HasSpecialFlag(QUEST_SPECIAL_FLAGS_TIMED)) {
      QuestStatusData &q_status = m_QuestStatus[questId];

      RemoveTimedQuest(questId);
      q_status.Timer = 0;

      SendQuestTimerFailed(questId);
    } else
      SendQuestFailed(questId);

    // Destroy quest items on quest failure.
    for (uint8 i = 0; i < QUEST_ITEM_OBJECTIVES_COUNT; ++i)
      if (ItemTemplate const *itemTemplate =
              sObjectMgr->GetItemTemplate(quest->RequiredItemId[i]))
        if (quest->RequiredItemCount[i] > 0 &&
            itemTemplate->GetBonding() == BIND_QUEST)
          DestroyItemCount(quest->RequiredItemId[i], 9999, true);

    for (uint8 i = 0; i < QUEST_SOURCE_ITEM_IDS_COUNT; ++i)
      if (ItemTemplate const *itemTemplate =
              sObjectMgr->GetItemTemplate(quest->ItemDrop[i]))
        if (quest->ItemDropQuantity[i] > 0 &&
            itemTemplate->GetBonding() == BIND_QUEST)
          DestroyItemCount(quest->ItemDrop[i], 9999, true);
  }
}

void Player::AbandonQuest(uint32 questId) {
  if (Quest const *quest = sObjectMgr->GetQuestTemplate(questId)) {
    // Destroy quest items on quest abandon.
    for (uint8 i = 0; i < QUEST_ITEM_OBJECTIVES_COUNT; ++i)
      if (ItemTemplate const *itemTemplate =
              sObjectMgr->GetItemTemplate(quest->RequiredItemId[i]))
        if (quest->RequiredItemCount[i] > 0 &&
            itemTemplate->GetBonding() == BIND_QUEST)
          DestroyItemCount(quest->RequiredItemId[i], 9999, true);

    for (uint8 i = 0; i < QUEST_SOURCE_ITEM_IDS_COUNT; ++i)
      if (ItemTemplate const *itemTemplate =
              sObjectMgr->GetItemTemplate(quest->ItemDrop[i]))
        if (quest->ItemDropQuantity[i] > 0 &&
            itemTemplate->GetBonding() == BIND_QUEST)
          DestroyItemCount(quest->ItemDrop[i], 9999, true);
  }
}

bool Player::SatisfyQuestSkill(Quest const *qInfo, bool msg) const {
  uint32 skill = qInfo->GetRequiredSkill();

  // skip 0 case RequiredSkill
  if (skill == 0)
    return true;

  // check skill value
  if (GetSkillValue(skill) < qInfo->GetRequiredSkillValue()) {
    if (msg) {
      SendCanTakeQuestResponse(INVALIDREASON_DONT_HAVE_REQ);
      LOG_DEBUG("misc",
                "Player::SatisfyQuestSkill: Sent INVALIDREASON_DONT_HAVE_REQ "
                "(QuestID: %u) because player '%s' (%s) doesn't have the "
                "required skill value.",
                qInfo->GetQuestId(), GetName().c_str(),
                GetGUID().ToString().c_str());
    }

    return false;
  }

  return true;
}

bool Player::SatisfyQuestLevel(Quest const *qInfo, bool msg) const {
  if (getLevel() < qInfo->GetMinLevel()) {
    if (msg) {
      SendCanTakeQuestResponse(INVALIDREASON_QUEST_FAILED_LOW_LEVEL);
      LOG_DEBUG("misc",
                "Player::SatisfyQuestLevel: Sent "
                "INVALIDREASON_QUEST_FAILED_LOW_LEVEL (QuestID: %u) because "
                "player '%s' (%s) doesn't have the required (min) level.",
                qInfo->GetQuestId(), GetName().c_str(),
                GetGUID().ToString().c_str());
    }
    return false;
  }

  if (qInfo->GetMaxLevel() > 0 && getLevel() > qInfo->GetMaxLevel()) {
    if (msg) {
      SendCanTakeQuestResponse(
          INVALIDREASON_DONT_HAVE_REQ); // There doesn't seem to be a specific
                                        // response for too high player level
      LOG_DEBUG("misc",
                "Player::SatisfyQuestLevel: Sent INVALIDREASON_DONT_HAVE_REQ "
                "(QuestID: %u) because player '%s' (%s) doesn't have the "
                "required (max) level.",
                qInfo->GetQuestId(), GetName().c_str(),
                GetGUID().ToString().c_str());
    }
    return false;
  }
  return true;
}

bool Player::SatisfyQuestLog(bool msg) const {
  // exist free slot
  if (FindQuestSlot(0) < MAX_QUEST_LOG_SIZE)
    return true;

  if (msg) {
    WorldPackets::Quest::QuestLogFull packet;
    SendDirectMessage(packet.Write());
  }
  return false;
}

bool Player::SatisfyQuestDependentQuests(Quest const *qInfo, bool msg) const {
  return SatisfyQuestPreviousQuest(qInfo, msg) &&
         SatisfyQuestDependentPreviousQuests(qInfo, msg) &&
         SatisfyQuestBreadcrumbQuest(qInfo, msg) &&
         SatisfyQuestDependentBreadcrumbQuests(qInfo, msg);
}

bool Player::SatisfyQuestPreviousQuest(Quest const *qInfo, bool msg) const {
  // No previous quest (might be first quest in a series)
  if (!qInfo->GetPrevQuestId())
    return true;

  uint32 prevId = std::abs(qInfo->GetPrevQuestId());
  // If positive previous quest rewarded, return true
  if (qInfo->GetPrevQuestId() > 0 && m_RewardedQuests.count(prevId) > 0)
    return true;

  // If negative previous quest active, return true
  if (qInfo->GetPrevQuestId() < 0 &&
      GetQuestStatus(prevId) == QUEST_STATUS_INCOMPLETE)
    return true;

  // Has positive prev. quest in non-rewarded state
  // and negative prev. quest in non-active state
  if (msg) {
    SendCanTakeQuestResponse(INVALIDREASON_DONT_HAVE_REQ);
    LOG_DEBUG("misc",
              "Player::SatisfyQuestPreviousQuest: Sent "
              "INVALIDREASON_DONT_HAVE_REQ (QuestID: %u) because player '%s' "
              "(%s) doesn't have required quest %u.",
              qInfo->GetQuestId(), GetName().c_str(),
              GetGUID().ToString().c_str(), prevId);
  }

  return false;
}

bool Player::SatisfyQuestDependentPreviousQuests(Quest const *qInfo,
                                                 bool msg) const {
  // No previous quest (might be first quest in a series)
  if (qInfo->DependentPreviousQuests.empty())
    return true;

  for (uint32 prevId : qInfo->DependentPreviousQuests) {
    // checked in startup
    Quest const *questInfo = sObjectMgr->GetQuestTemplate(prevId);
    ASSERT(questInfo);

    // If any of the previous quests completed, return true
    if (IsQuestRewarded(prevId)) {
      // skip one-from-all exclusive group
      if (questInfo->GetExclusiveGroup() >= 0)
        return true;

      // each-from-all exclusive group (< 0)
      // can be start if only all quests in prev quest exclusive group completed
      // and rewarded
      auto bounds = sObjectMgr->GetExclusiveQuestGroupBounds(
          questInfo->GetExclusiveGroup());
      for (auto itr = bounds.first; itr != bounds.second; ++itr) {
        // skip checked quest id, only state of other quests in group is
        // interesting
        uint32 exclusiveQuestId = itr->second;
        if (exclusiveQuestId == prevId)
          continue;

        // alternative quest from group also must be completed and rewarded
        // (reported)
        if (!IsQuestRewarded(exclusiveQuestId)) {
          if (msg) {
            SendCanTakeQuestResponse(INVALIDREASON_DONT_HAVE_REQ);
            LOG_DEBUG("misc",
                      "Player::SatisfyQuestDependentPreviousQuests: Sent "
                      "INVALIDREASON_DONT_HAVE_REQ (QuestID: %u) because "
                      "player '%s' (%s) doesn't have the required quest (1).",
                      qInfo->GetQuestId(), GetName().c_str(),
                      GetGUID().ToString().c_str());
          }

          return false;
        }
      }

      return true;
    }
  }

  // Has only prev. quests in non-rewarded state
  if (msg) {
    SendCanTakeQuestResponse(INVALIDREASON_DONT_HAVE_REQ);
    LOG_DEBUG("misc",
              "Player::SatisfyQuestDependentPreviousQuests: Sent "
              "INVALIDREASON_DONT_HAVE_REQ (QuestID: %u) because player '%s' "
              "(%s) doesn't have required quest (2).",
              qInfo->GetQuestId(), GetName().c_str(),
              GetGUID().ToString().c_str());
  }

  return false;
}

bool Player::SatisfyQuestBreadcrumbQuest(Quest const *qInfo, bool msg) const {
  uint32 breadcrumbTargetQuestId = std::abs(qInfo->GetBreadcrumbForQuestId());

  // If this is not a breadcrumb quest.
  if (!breadcrumbTargetQuestId)
    return true;

  // If the target quest is not available
  if (!CanTakeQuest(sObjectMgr->GetQuestTemplate(breadcrumbTargetQuestId),
                    false)) {
    if (msg) {
      SendCanTakeQuestResponse(INVALIDREASON_DONT_HAVE_REQ);
      LOG_DEBUG("misc",
                "Player::SatisfyQuestBreadcrumbQuest: Sent "
                "INVALIDREASON_DONT_HAVE_REQ (QuestID: %u) because target "
                "quest (QuestID: %u) is not available to player '%s' (%s).",
                qInfo->GetQuestId(), breadcrumbTargetQuestId, GetName().c_str(),
                GetGUID().ToString().c_str());
    }

    return false;
  }

  return true;
}

bool Player::SatisfyQuestDependentBreadcrumbQuests(Quest const *qInfo,
                                                   bool msg) const {
  for (uint32 breadcrumbQuestId : qInfo->DependentBreadcrumbQuests) {
    QuestStatus status = GetQuestStatus(breadcrumbQuestId);
    // If any of the breadcrumb quests are in the quest log, return false.
    if (status == QUEST_STATUS_INCOMPLETE || status == QUEST_STATUS_COMPLETE ||
        status == QUEST_STATUS_FAILED) {
      if (msg) {
        SendCanTakeQuestResponse(INVALIDREASON_DONT_HAVE_REQ);
        LOG_DEBUG(
            "misc",
            "Player::SatisfyQuestDependentBreadcrumbQuests: Sent "
            "INVALIDREASON_DONT_HAVE_REQ (QuestID: %u) because player '%s' "
            "(%s) has a breadcrumb quest towards this quest in the quest log.",
            qInfo->GetQuestId(), GetName().c_str(),
            GetGUID().ToString().c_str());
      }

      return false;
    }
  }
  return true;
}

bool Player::SatisfyQuestClass(Quest const *qInfo, bool msg) const {
  uint32 reqClass = qInfo->GetAllowableClasses();

  if (reqClass == 0)
    return true;

  if ((reqClass & getClassMask()) == 0) {
    if (msg) {
      SendCanTakeQuestResponse(INVALIDREASON_DONT_HAVE_REQ);
      LOG_DEBUG(
          "misc",
          "Player::SatisfyQuestClass: Sent INVALIDREASON_DONT_HAVE_REQ "
          "(QuestID: %u) because player '%s' (%s) doesn't have required class.",
          qInfo->GetQuestId(), GetName().c_str(), GetGUID().ToString().c_str());
    }

    return false;
  }

  return true;
}

bool Player::SatisfyQuestRace(Quest const *qInfo, bool msg) const {
  uint32 reqraces = qInfo->GetAllowableRaces();
  if (reqraces == 0)
    return true;
  if ((reqraces & getRaceMask()) == 0) {
    if (msg) {
      SendCanTakeQuestResponse(INVALIDREASON_QUEST_FAILED_WRONG_RACE);
      LOG_DEBUG("misc",
                "Player::SatisfyQuestRace: Sent "
                "INVALIDREASON_QUEST_FAILED_WRONG_RACE (QuestID: %u) because "
                "player '%s' (%s) doesn't have required race.",
                qInfo->GetQuestId(), GetName().c_str(),
                GetGUID().ToString().c_str());
    }
    return false;
  }
  return true;
}

bool Player::SatisfyQuestReputation(Quest const *qInfo, bool msg) const {
  uint32 fIdMin = qInfo->GetRequiredMinRepFaction(); // Min required rep
  if (fIdMin && GetReputationMgr().GetReputation(fIdMin) <
                    qInfo->GetRequiredMinRepValue()) {
    if (msg) {
      SendCanTakeQuestResponse(INVALIDREASON_DONT_HAVE_REQ);
      LOG_DEBUG("misc",
                "Player::SatisfyQuestReputation: Sent "
                "INVALIDREASON_DONT_HAVE_REQ (QuestID: %u) because player '%s' "
                "(%s) doesn't required reputation (min).",
                qInfo->GetQuestId(), GetName().c_str(),
                GetGUID().ToString().c_str());
    }
    return false;
  }

  uint32 fIdMax = qInfo->GetRequiredMaxRepFaction(); // Max required rep
  if (fIdMax && GetReputationMgr().GetReputation(fIdMax) >=
                    qInfo->GetRequiredMaxRepValue()) {
    if (msg) {
      SendCanTakeQuestResponse(INVALIDREASON_DONT_HAVE_REQ);
      LOG_DEBUG(
          "misc",
          "SatisfyQuestReputation: Sent INVALIDREASON_DONT_HAVE_REQ (QuestID: "
          "%u) because player '%s' (%s) doesn't required reputation (max).",
          qInfo->GetQuestId(), GetName().c_str(), GetGUID().ToString().c_str());
    }
    return false;
  }

  // ReputationObjective2 does not seem to be an objective requirement but a
  // requirement to be able to accept the quest
  uint32 fIdObj = qInfo->GetRepObjectiveFaction2();
  if (fIdObj && GetReputationMgr().GetReputation(fIdObj) >=
                    qInfo->GetRepObjectiveValue2()) {
    if (msg) {
      SendCanTakeQuestResponse(INVALIDREASON_DONT_HAVE_REQ);
      LOG_DEBUG("misc",
                "SatisfyQuestReputation: Sent INVALIDREASON_DONT_HAVE_REQ "
                "(questId: %u) because player does not have the required "
                "reputation (ReputationObjective2).",
                qInfo->GetQuestId());
    }
    return false;
  }

  return true;
}

bool Player::SatisfyQuestStatus(Quest const *qInfo, bool msg) const {
  if (GetQuestStatus(qInfo->GetQuestId()) == QUEST_STATUS_REWARDED) {
    if (msg) {
      SendCanTakeQuestResponse(INVALIDREASON_QUEST_ALREADY_DONE);
      LOG_DEBUG(
          "misc",
          "Player::SatisfyQuestStatus: Sent QUEST_STATUS_REWARDED (QuestID: "
          "%u) because player '%s' (%s) quest status is already REWARDED.",
          qInfo->GetQuestId(), GetName().c_str(), GetGUID().ToString().c_str());
    }
    return false;
  }

  if (GetQuestStatus(qInfo->GetQuestId()) != QUEST_STATUS_NONE) {
    if (msg) {
      SendCanTakeQuestResponse(INVALIDREASON_QUEST_ALREADY_ON);
      LOG_DEBUG(
          "misc",
          "Player::SatisfyQuestStatus: Sent INVALIDREASON_QUEST_ALREADY_ON "
          "(QuestID: %u) because player '%s' (%s) quest status is not NONE.",
          qInfo->GetQuestId(), GetName().c_str(), GetGUID().ToString().c_str());
    }
    return false;
  }
  return true;
}

bool Player::SatisfyQuestConditions(Quest const *qInfo, bool msg) const {
  if (!sConditionMgr->IsObjectMeetingNotGroupedConditions(
          CONDITION_SOURCE_TYPE_QUEST_AVAILABLE, qInfo->GetQuestId(),
          const_cast<Player *>(this))) {
    if (msg) {
      SendCanTakeQuestResponse(INVALIDREASON_DONT_HAVE_REQ);
      LOG_DEBUG(
          "misc",
          "Player::SatisfyQuestConditions: Sent INVALIDREASON_DONT_HAVE_REQ "
          "(QuestID: %u) because player '%s' (%s) doesn't meet conditions.",
          qInfo->GetQuestId(), GetName().c_str(), GetGUID().ToString().c_str());
    }
    LOG_DEBUG("condition",
              "Player::SatisfyQuestConditions: conditions not met for quest %u",
              qInfo->GetQuestId());
    return false;
  }
  return true;
}

bool Player::SatisfyQuestTimed(Quest const *qInfo, bool msg) const {
  if (!m_timedquests.empty() &&
      qInfo->HasSpecialFlag(QUEST_SPECIAL_FLAGS_TIMED)) {
    if (msg) {
      SendCanTakeQuestResponse(INVALIDREASON_QUEST_ONLY_ONE_TIMED);
      LOG_DEBUG(
          "misc",
          "Player::SatisfyQuestTimed: Sent INVALIDREASON_QUEST_ONLY_ONE_TIMED "
          "(QuestID: %u) because player '%s' (%s) is already on a timed quest.",
          qInfo->GetQuestId(), GetName().c_str(), GetGUID().ToString().c_str());
    }
    return false;
  }
  return true;
}

bool Player::SatisfyQuestExclusiveGroup(Quest const *qInfo, bool msg) const {
  // non positive exclusive group, if > 0 then can be start if any other quest
  // in exclusive group already started/completed
  if (qInfo->GetExclusiveGroup() <= 0)
    return true;

  auto bounds =
      sObjectMgr->GetExclusiveQuestGroupBounds(qInfo->GetExclusiveGroup());
  for (auto itr = bounds.first; itr != bounds.second; ++itr) {
    uint32 exclude_Id = itr->second;

    // skip checked quest id, only state of other quests in group is interesting
    if (exclude_Id == qInfo->GetQuestId())
      continue;

    // not allow have daily quest if daily quest from exclusive group already
    // recently completed
    Quest const *Nquest = sObjectMgr->GetQuestTemplate(exclude_Id);
    ASSERT(Nquest);
    if (!SatisfyQuestDay(Nquest, false) || !SatisfyQuestWeek(Nquest, false) ||
        !SatisfyQuestSeasonal(Nquest, false)) {
      if (msg) {
        SendCanTakeQuestResponse(INVALIDREASON_DONT_HAVE_REQ);
        LOG_DEBUG("misc",
                  "Player::SatisfyQuestExclusiveGroup: Sent "
                  "INVALIDREASON_DONT_HAVE_REQ (QuestID: %u) because player "
                  "'%s' (%s) already did daily quests in exclusive group.",
                  qInfo->GetQuestId(), GetName().c_str(),
                  GetGUID().ToString().c_str());
      }

      return false;
    }

    // alternative quest already started or completed - but don't check rewarded
    // states if both are repeatable
    if (GetQuestStatus(exclude_Id) != QUEST_STATUS_NONE ||
        (!(qInfo->IsRepeatable() && Nquest->IsRepeatable()) &&
         GetQuestRewardStatus(exclude_Id))) {
      if (msg) {
        SendCanTakeQuestResponse(INVALIDREASON_DONT_HAVE_REQ);
        LOG_DEBUG("misc",
                  "Player::SatisfyQuestExclusiveGroup: Sent "
                  "INVALIDREASON_DONT_HAVE_REQ (QuestID: %u) because player "
                  "'%s' (%s) already did quest in exclusive group.",
                  qInfo->GetQuestId(), GetName().c_str(),
                  GetGUID().ToString().c_str());
      }
      return false;
    }
  }
  return true;
}

bool Player::SatisfyQuestDay(Quest const *qInfo, bool msg) const {
  if (!qInfo->IsDaily() && !qInfo->IsDFQuest())
    return true;

  bool have_slot = false;
  for (uint32 quest_daily_idx = 0; quest_daily_idx < PLAYER_MAX_DAILY_QUESTS;
       ++quest_daily_idx) {
    uint32 id = GetUInt32Value(PLAYER_FIELD_DAILY_QUESTS_1 + quest_daily_idx);
    if (qInfo->GetQuestId() == id)
      return false;

    if (!id)
      have_slot = true;
  }

  if (!have_slot) {
    if (msg) {
      SendCanTakeQuestResponse(INVALIDREASON_DAILY_QUESTS_REMAINING);
      LOG_DEBUG(
          "misc",
          "SatisfyQuestDay: Sent INVALIDREASON_DAILY_QUESTS_REMAINING "
          "(questId: %u) because player already did all possible quests today.",
          qInfo->GetQuestId());
    }
    return false;
  }

  return true;
}

bool Player::SatisfyQuestWeek(Quest const *qInfo, bool /*msg*/) const {
  if (!qInfo->IsWeekly() || m_weeklyquests.empty())
    return true;

  // if not found in cooldown list
  return m_weeklyquests.find(qInfo->GetQuestId()) == m_weeklyquests.end();
}

bool Player::SatisfyQuestSeasonal(Quest const *qInfo, bool /*msg*/) const {
  if (!qInfo->IsSeasonal() || m_seasonalquests.empty())
    return true;

  auto itr = m_seasonalquests.find(qInfo->GetEventIdForQuest());
  if (itr == m_seasonalquests.end() || itr->second.empty())
    return true;

  // if not found in cooldown list
  return itr->second.find(qInfo->GetQuestId()) == itr->second.end();
}

bool Player::SatisfyQuestMonth(Quest const *qInfo, bool /*msg*/) const {
  if (!qInfo->IsMonthly() || m_monthlyquests.empty())
    return true;

  // if not found in cooldown list
  return m_monthlyquests.find(qInfo->GetQuestId()) == m_monthlyquests.end();
}

bool Player::GiveQuestSourceItem(Quest const *quest) {
  uint32 srcitem = quest->GetSrcItemId();
  if (srcitem > 0) {
    // Don't give source item if it is the same item used to start the quest
    ItemTemplate const *itemTemplate = sObjectMgr->GetItemTemplate(srcitem);
    if (quest->GetQuestId() == itemTemplate->GetStartQuest())
      return true;

    uint32 count = quest->GetSrcItemCount();
    if (count <= 0)
      count = 1;

    ItemPosCountVec dest;
    InventoryResult msg =
        CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, srcitem, count);
    if (msg == EQUIP_ERR_OK) {
      Item *item = StoreNewItem(dest, srcitem, true);
      SendNewItem(item, count, true, false);
      return true;
    }
    // player already have max amount required item, just report success
    else if (msg == EQUIP_ERR_ITEM_MAX_COUNT)
      return true;

    SendEquipError(msg, nullptr, nullptr, srcitem);
    return false;
  }

  return true;
}

bool Player::TakeQuestSourceItem(uint32 questId, bool msg) {
  Quest const *quest = sObjectMgr->GetQuestTemplate(questId);
  if (quest) {
    uint32 srcItemId = quest->GetSrcItemId();
    ItemTemplate const *item = sObjectMgr->GetItemTemplate(srcItemId);

    if (srcItemId > 0) {
      uint32 count = quest->GetSrcItemCount();
      if (count <= 0)
        count = 1;

      // There are two cases where the source item is not destroyed:
      // - Item cannot be unequipped (example: non-empty bags)
      // - The source item is the item that started the quest, so the player is
      // supposed to keep it (otherwise it was already destroyed in
      // AddQuestAndCheckCompletion())
      InventoryResult res = CanUnequipItems(srcItemId, count);
      if (res != EQUIP_ERR_OK) {
        if (msg)
          SendEquipError(res, nullptr, nullptr, srcItemId);
        return false;
      }

      ASSERT(item);
      if (item->GetStartQuest() != questId)
        DestroyItemCount(srcItemId, count, true, true);
    }
  }

  return true;
}

bool Player::GetQuestRewardStatus(uint32 quest_id) const {
  Quest const *qInfo = sObjectMgr->GetQuestTemplate(quest_id);
  if (qInfo) {
    if (qInfo->IsSeasonal() && !qInfo->IsRepeatable())
      return !SatisfyQuestSeasonal(qInfo, false);

    // for repeatable quests: rewarded field is set after first reward only to
    // prevent getting XP more than once
    if (!qInfo->IsRepeatable())
      return IsQuestRewarded(quest_id);

    return false;
  }
  return false;
}

QuestStatus Player::GetQuestStatus(uint32 quest_id) const {
  if (quest_id) {
    QuestStatusMap::const_iterator itr = m_QuestStatus.find(quest_id);
    if (itr != m_QuestStatus.end())
      return itr->second.Status;

    if (GetQuestRewardStatus(quest_id))
      return QUEST_STATUS_REWARDED;
  }
  return QUEST_STATUS_NONE;
}

bool Player::CanShareQuest(uint32 quest_id) const {
  Quest const *qInfo = sObjectMgr->GetQuestTemplate(quest_id);
  if (qInfo && qInfo->HasFlag(QUEST_FLAGS_SHARABLE)) {
    QuestStatusMap::const_iterator itr = m_QuestStatus.find(quest_id);
    if (itr != m_QuestStatus.end()) {
      // in pool and not currently available (wintergrasp weekly, dalaran
      // weekly) - can't share
      if (sQuestPoolMgr->IsQuestActive(quest_id)) {
        SendPushToPartyResponse(this, QUEST_PARTY_MSG_CANT_BE_SHARED_TODAY);
        return false;
      }

      return true;
    }
  }
  return false;
}

void Player::SetQuestStatus(uint32 questId, QuestStatus status,
                            bool update /*= true*/) {
  if (Quest const *quest = sObjectMgr->GetQuestTemplate(questId)) {
    m_QuestStatus[questId].Status = status;

    if (!quest->IsAutoComplete())
      m_QuestStatusSave[questId] = QUEST_DEFAULT_SAVE_TYPE;
  }

  if (update)
    SendQuestUpdate(questId);

  sScriptMgr->OnQuestStatusChange(this, questId);
}

void Player::RemoveActiveQuest(uint32 questId, bool update /*= true*/) {
  QuestStatusMap::iterator itr = m_QuestStatus.find(questId);
  if (itr != m_QuestStatus.end()) {
    m_QuestStatus.erase(itr);
    m_QuestStatusSave[questId] = QUEST_DELETE_SAVE_TYPE;
  }

  if (update)
    SendQuestUpdate(questId);
}

void Player::RemoveRewardedQuest(uint32 questId, bool update /*= true*/) {
  RewardedQuestSet::iterator rewItr = m_RewardedQuests.find(questId);
  if (rewItr != m_RewardedQuests.end()) {
    m_RewardedQuests.erase(rewItr);
    m_RewardedQuestsSave[questId] = QUEST_FORCE_DELETE_SAVE_TYPE;
  }

  // Remove seasonal quest also
  Quest const *qInfo = sObjectMgr->GetQuestTemplate(questId);
  if (qInfo->IsSeasonal()) {
    uint16 eventId = qInfo->GetEventIdForQuest();
    if (m_seasonalquests.find(eventId) != m_seasonalquests.end()) {
      m_seasonalquests[eventId].erase(questId);
      m_SeasonalQuestChanged = true;
    }
  }

  if (update)
    SendQuestUpdate(questId);
}

void Player::SendQuestUpdate(uint32 questId) {
  SpellAreaForQuestMapBounds saBounds =
      sSpellMgr->GetSpellAreaForQuestMapBounds(questId);

  if (saBounds.first != saBounds.second) {
    std::set<uint32> aurasToRemove, aurasToCast;
    uint32 zone = 0, area = 0;
    GetZoneAndAreaId(zone, area);

    for (SpellAreaForQuestMap::const_iterator itr = saBounds.first;
         itr != saBounds.second; ++itr) {
      if (itr->second->flags & SPELL_AREA_FLAG_AUTOREMOVE &&
          !itr->second->IsFitToRequirements(this, zone, area))
        aurasToRemove.insert(itr->second->spellId);
      else if (itr->second->flags & SPELL_AREA_FLAG_AUTOCAST)
        aurasToCast.insert(itr->second->spellId);
    }

    // Auras matching the requirements will be inside the aurasToCast container.
    // Auras not matching the requirements may prevent using auras matching the
    // requirements. aurasToCast will erase conflicting auras in aurasToRemove
    // container to handle spells used by multiple quests.

    for (auto itr = aurasToRemove.begin(); itr != aurasToRemove.end();) {
      bool auraRemoved = false;

      for (const auto i : aurasToCast) {
        if (*itr == i) {
          itr = aurasToRemove.erase(itr);
          auraRemoved = true;
          break;
        }
      }

      if (!auraRemoved)
        ++itr;
    }

    for (auto spellId : aurasToCast)
      if (!HasAura(spellId))
        CastSpell(this, spellId, true);

    for (auto spellId : aurasToRemove)
      RemoveAurasDueToSpell(spellId);
  }

  UpdateVisibleGameobjectsOrSpellClicks();
  PhasingHandler::OnConditionChange(this);
}

bool Player::SatisfyFirstLFGReward(uint32 dungeonId, uint8 maxRewCount) const {
  LFGRewardStatusMap::const_iterator lfgdungeon =
      m_lfgrewardstatus.find(dungeonId);
  if (lfgdungeon != m_lfgrewardstatus.end())
    return lfgdungeon->second.CompletionsThisPeriod &&
           lfgdungeon->second.CompletionsThisPeriod < maxRewCount;

  return true;
}

uint8 Player::GetFirstRewardCountForDungeonId(uint32 dungeonId) {
  LFGRewardStatusMap::const_iterator lfgdungeon =
      m_lfgrewardstatus.find(dungeonId);
  if (lfgdungeon != m_lfgrewardstatus.end())
    return lfgdungeon->second.CompletionsThisPeriod;

  return 0;
}

QuestGiverStatus Player::GetQuestDialogStatus(Object *questgiver) {
  QuestRelationResult qr, qir;

  switch (questgiver->GetTypeId()) {
  case TYPEID_GAMEOBJECT: {
    if (auto questStatus =
            questgiver->ToGameObject()->AI()->GetDialogStatus(this))
      return *questStatus;
    qr = sObjectMgr->GetGOQuestRelations(questgiver->GetEntry());
    qir = sObjectMgr->GetGOQuestInvolvedRelations(questgiver->GetEntry());
    break;
  }
  case TYPEID_UNIT: {
    if (auto questStatus =
            questgiver->ToCreature()->AI()->GetDialogStatus(this))
      return *questStatus;
    qr = sObjectMgr->GetCreatureQuestRelations(questgiver->GetEntry());
    qir = sObjectMgr->GetCreatureQuestInvolvedRelations(questgiver->GetEntry());
    break;
  }
  default:
    // it's impossible, but check
    LOG_ERROR("entities.player.quest",
              "GetQuestDialogStatus called for unexpected type %u",
              questgiver->GetTypeId());
    return DIALOG_STATUS_NONE;
  }

  QuestGiverStatus result = DIALOG_STATUS_NONE;

  for (uint32 questId : qir) {
    QuestGiverStatus result2 = DIALOG_STATUS_NONE;
    Quest const *quest = sObjectMgr->GetQuestTemplate(questId);
    if (!quest)
      continue;

    QuestStatus status = GetQuestStatus(questId);
    if (status == QUEST_STATUS_COMPLETE && !GetQuestRewardStatus(questId))
      result2 = DIALOG_STATUS_REWARD;
    else if (status == QUEST_STATUS_INCOMPLETE)
      result2 = DIALOG_STATUS_INCOMPLETE;

    if (quest->IsAutoComplete() && CanTakeQuest(quest, false) &&
        quest->IsRepeatable() && !quest->IsDailyOrWeekly())
      result2 = DIALOG_STATUS_REWARD_REP;

    if (result2 > result)
      result = result2;
  }

  for (uint32 questId : qr) {
    QuestGiverStatus result2 = DIALOG_STATUS_NONE;
    Quest const *quest = sObjectMgr->GetQuestTemplate(questId);
    if (!quest)
      continue;

    if (!sConditionMgr->IsObjectMeetingNotGroupedConditions(
            CONDITION_SOURCE_TYPE_QUEST_AVAILABLE, quest->GetQuestId(), this))
      continue;

    QuestStatus status = GetQuestStatus(questId);
    if (status == QUEST_STATUS_NONE) {
      if (CanSeeStartQuest(quest)) {
        if (SatisfyQuestLevel(quest, false)) {
          bool isNotLowLevelQuest =
              getLevel() <=
              (GetQuestLevel(quest) +
               sWorld->getIntConfig(CONFIG_QUEST_LOW_LEVEL_HIDE_DIFF));
          if (quest->IsRepeatable()) {
            if (quest->IsDaily()) {
              if (isNotLowLevelQuest)
                result2 = DIALOG_STATUS_AVAILABLE_REP;
              else
                result2 = DIALOG_STATUS_LOW_LEVEL_AVAILABLE_REP;
            } else if (quest->IsWeekly() || quest->IsMonthly()) {
              if (isNotLowLevelQuest)
                result2 = DIALOG_STATUS_AVAILABLE;
              else
                result2 = DIALOG_STATUS_LOW_LEVEL_AVAILABLE;
            } else if (quest->IsAutoComplete()) {
              if (isNotLowLevelQuest)
                result2 = DIALOG_STATUS_REWARD_REP;
              else
                result2 = DIALOG_STATUS_LOW_LEVEL_REWARD_REP;
            } else {
              if (isNotLowLevelQuest)
                result2 = DIALOG_STATUS_AVAILABLE;
              else
                result2 = DIALOG_STATUS_LOW_LEVEL_AVAILABLE;
            }
          } else if (isNotLowLevelQuest)
            result2 = DIALOG_STATUS_AVAILABLE;
          else
            result2 = DIALOG_STATUS_LOW_LEVEL_AVAILABLE;
        } else
          result2 = DIALOG_STATUS_UNAVAILABLE;
      }
    }

    if (result2 > result)
      result = result2;
  }

  return result;
}

// not used in Firelands, but used in scripting code
uint16 Player::GetReqKillOrCastCurrentCount(uint32 quest_id,
                                            int32 entry) const {
  Quest const *qInfo = sObjectMgr->GetQuestTemplate(quest_id);
  if (!qInfo)
    return 0;

  auto itr = m_QuestStatus.find(quest_id);
  if (itr != m_QuestStatus.end())
    for (uint8 j = 0; j < QUEST_OBJECTIVES_COUNT; ++j)
      if (qInfo->RequiredNpcOrGo[j] == entry)
        return itr->second.CreatureOrGOCount[j];

  return 0;
}

void Player::AdjustQuestReqItemCount(Quest const *quest,
                                     QuestStatusData &questStatusData) {
  if (quest->HasSpecialFlag(QUEST_SPECIAL_FLAGS_DELIVER)) {
    for (uint8 i = 0; i < QUEST_ITEM_OBJECTIVES_COUNT; ++i) {
      uint32 reqitemcount = quest->RequiredItemCount[i];
      if (reqitemcount != 0) {
        uint32 curitemcount = GetItemCount(quest->RequiredItemId[i], true);

        questStatusData.ItemCount[i] = std::min(curitemcount, reqitemcount);
        m_QuestStatusSave[quest->GetQuestId()] = QUEST_DEFAULT_SAVE_TYPE;
      }
    }
  }
}

uint16 Player::FindQuestSlot(uint32 quest_id) const {
  for (uint16 i = 0; i < MAX_QUEST_LOG_SIZE; ++i)
    if (GetQuestSlotQuestId(i) == quest_id)
      return i;

  return MAX_QUEST_LOG_SIZE;
}

uint32 Player::GetQuestSlotQuestId(uint16 slot) const {
  return GetUInt32Value(PLAYER_QUEST_LOG_1_1 + slot * MAX_QUEST_OFFSET +
                        QUEST_ID_OFFSET);
}

uint32 Player::GetQuestSlotState(uint16 slot) const {
  return GetUInt32Value(PLAYER_QUEST_LOG_1_1 + slot * MAX_QUEST_OFFSET +
                        QUEST_STATE_OFFSET);
}

uint16 Player::GetQuestSlotCounter(uint16 slot, uint8 counter) const {
  return (uint16)(GetUInt64Value(PLAYER_QUEST_LOG_1_1 +
                                 slot * MAX_QUEST_OFFSET +
                                 QUEST_COUNTS_OFFSET) >>
                  (counter * 16));
}

uint32 Player::GetQuestSlotTime(uint16 slot) const {
  return GetUInt32Value(PLAYER_QUEST_LOG_1_1 + slot * MAX_QUEST_OFFSET +
                        QUEST_TIME_OFFSET);
}

void Player::SetQuestSlot(uint16 slot, uint32 quest_id, uint32 timer /*= 0*/) {
  SetUInt32Value(PLAYER_QUEST_LOG_1_1 + slot * MAX_QUEST_OFFSET +
                     QUEST_ID_OFFSET,
                 quest_id);
  SetUInt32Value(
      PLAYER_QUEST_LOG_1_1 + slot * MAX_QUEST_OFFSET + QUEST_STATE_OFFSET, 0);
  SetUInt32Value(
      PLAYER_QUEST_LOG_1_1 + slot * MAX_QUEST_OFFSET + QUEST_COUNTS_OFFSET, 0);
  SetUInt32Value(PLAYER_QUEST_LOG_1_1 + slot * MAX_QUEST_OFFSET +
                     QUEST_COUNTS_OFFSET + 1,
                 0);
  SetUInt32Value(PLAYER_QUEST_LOG_1_1 + slot * MAX_QUEST_OFFSET +
                     QUEST_TIME_OFFSET,
                 timer);
}

void Player::SetQuestSlotCounter(uint16 slot, uint8 counter, uint16 count) {
  uint64 val = GetUInt64Value(PLAYER_QUEST_LOG_1_1 + slot * MAX_QUEST_OFFSET +
                              QUEST_COUNTS_OFFSET);
  val &= ~((uint64)0xFFFF << (counter * 16));
  val |= ((uint64)count << (counter * 16));
  SetUInt64Value(PLAYER_QUEST_LOG_1_1 + slot * MAX_QUEST_OFFSET +
                     QUEST_COUNTS_OFFSET,
                 val);
}

void Player::SetQuestSlotState(uint16 slot, uint32 state) {
  SetFlag(PLAYER_QUEST_LOG_1_1 + slot * MAX_QUEST_OFFSET + QUEST_STATE_OFFSET,
          state);
}

void Player::RemoveQuestSlotState(uint16 slot, uint32 state) {
  RemoveFlag(PLAYER_QUEST_LOG_1_1 + slot * MAX_QUEST_OFFSET +
                 QUEST_STATE_OFFSET,
             state);
}

void Player::SetQuestSlotTimer(uint16 slot, uint32 timer) {
  SetUInt32Value(PLAYER_QUEST_LOG_1_1 + slot * MAX_QUEST_OFFSET +
                     QUEST_TIME_OFFSET,
                 timer);
}

void Player::SwapQuestSlot(uint16 slot1, uint16 slot2) {
  for (int i = 0; i < MAX_QUEST_OFFSET; ++i) {
    uint32 temp1 =
        GetUInt32Value(PLAYER_QUEST_LOG_1_1 + MAX_QUEST_OFFSET * slot1 + i);
    uint32 temp2 =
        GetUInt32Value(PLAYER_QUEST_LOG_1_1 + MAX_QUEST_OFFSET * slot2 + i);

    SetUInt32Value(PLAYER_QUEST_LOG_1_1 + MAX_QUEST_OFFSET * slot1 + i, temp2);
    SetUInt32Value(PLAYER_QUEST_LOG_1_1 + MAX_QUEST_OFFSET * slot2 + i, temp1);
  }
}

void Player::AreaExploredOrEventHappens(uint32 questId) {
  if (questId) {
    if (Quest const *qInfo = sObjectMgr->GetQuestTemplate(questId)) {
      uint16 log_slot = FindQuestSlot(questId);
      if (log_slot < MAX_QUEST_LOG_SIZE) {
        QuestStatusData &q_status = m_QuestStatus[questId];

        // Dont complete failed quest
        if (!q_status.Explored && q_status.Status != QUEST_STATUS_FAILED) {
          q_status.Explored = true;
          m_QuestStatusSave[questId] = QUEST_DEFAULT_SAVE_TYPE;

          SendQuestComplete(qInfo);
        }
      }
      if (CanCompleteQuest(questId))
        CompleteQuest(questId);
    }
  }
}

// not used in Firelandsd, function for external script library
void Player::GroupEventHappens(uint32 questId,
                               WorldObject const *pEventObject) {
  if (Group *group = GetGroup()) {
    for (GroupReference *itr = group->GetFirstMember(); itr != nullptr;
         itr = itr->next()) {
      Player *player = itr->GetSource();

      // for any leave or dead (with not released body) group member at
      // appropriate distance
      if (player && player->IsAtGroupRewardDistance(pEventObject) &&
          !player->GetCorpse())
        player->AreaExploredOrEventHappens(questId);
    }
  } else
    AreaExploredOrEventHappens(questId);
}

void Player::ItemAddedQuestCheck(uint32 entry, uint32 count) {
  for (uint8 i = 0; i < MAX_QUEST_LOG_SIZE; ++i) {
    uint32 questid = GetQuestSlotQuestId(i);
    if (questid == 0)
      continue;

    QuestStatusData &q_status = m_QuestStatus[questid];

    if (q_status.Status != QUEST_STATUS_INCOMPLETE)
      continue;

    Quest const *qInfo = sObjectMgr->GetQuestTemplate(questid);
    if (!qInfo || !qInfo->HasSpecialFlag(QUEST_SPECIAL_FLAGS_DELIVER))
      continue;

    for (uint8 j = 0; j < QUEST_ITEM_OBJECTIVES_COUNT; ++j) {
      uint32 reqitem = qInfo->RequiredItemId[j];
      if (reqitem == entry) {
        uint32 reqitemcount = qInfo->RequiredItemCount[j];
        uint16 curitemcount = q_status.ItemCount[j];
        if (curitemcount < reqitemcount) {
          q_status.ItemCount[j] =
              std::min<uint16>(q_status.ItemCount[j] + count, reqitemcount);
          m_QuestStatusSave[questid] = QUEST_DEFAULT_SAVE_TYPE;
        }
        if (CanCompleteQuest(questid))
          CompleteQuest(questid);
      }
    }
  }
  UpdateVisibleGameobjectsOrSpellClicks();
}

void Player::ItemRemovedQuestCheck(uint32 entry, uint32 count) {
  for (uint8 i = 0; i < MAX_QUEST_LOG_SIZE; ++i) {
    uint32 questid = GetQuestSlotQuestId(i);
    if (!questid)
      continue;

    Quest const *qInfo = sObjectMgr->GetQuestTemplate(questid);
    if (!qInfo)
      continue;

    if (!qInfo->HasSpecialFlag(QUEST_SPECIAL_FLAGS_DELIVER))
      continue;

    for (uint8 j = 0; j < QUEST_ITEM_OBJECTIVES_COUNT; ++j) {
      uint32 reqitem = qInfo->RequiredItemId[j];
      if (reqitem == entry) {
        QuestStatusData &q_status = m_QuestStatus[questid];

        uint32 reqItemCount = qInfo->RequiredItemCount[j];
        uint16 questStatusItemCount = q_status.ItemCount[j];
        uint16 newItemCount =
            (count > questStatusItemCount) ? 0 : questStatusItemCount - count;

        if (questStatusItemCount >=
            reqItemCount) // we may have more than what the status shows, we
                          // don't need reduce by count
          newItemCount = GetItemCount(entry, false);

        newItemCount = std::min<uint16>(newItemCount, reqItemCount);
        if (newItemCount != q_status.ItemCount[j]) {
          q_status.ItemCount[j] = newItemCount;
          m_QuestStatusSave[questid] = QUEST_DEFAULT_SAVE_TYPE;
          IncompleteQuest(questid);
        }
      }
    }
  }
  UpdateVisibleGameobjectsOrSpellClicks();
}

void Player::KilledMonster(CreatureTemplate const *cInfo, ObjectGuid guid) {
  ASSERT(cInfo);

  if (cInfo->Entry)
    KilledMonsterCredit(cInfo->Entry, guid);

  for (uint8 i = 0; i < MAX_KILL_CREDIT; ++i)
    if (cInfo->KillCredit[i])
      KilledMonsterCredit(cInfo->KillCredit[i], ObjectGuid::Empty);
}

void Player::KilledMonsterCredit(uint32 entry,
                                 ObjectGuid guid /*= ObjectGuid::Empty*/) {
  uint16 addkillcount = 1;
  uint32 real_entry = entry;
  Creature *killed = nullptr;
  if (guid) {
    killed = GetMap()->GetCreature(guid);
    if (killed && killed->GetEntry())
      real_entry = killed->GetEntry();
  }

  StartTimedAchievement(ACHIEVEMENT_TIMED_TYPE_CREATURE,
                        real_entry); // MUST BE CALLED FIRST
  UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE, real_entry,
                            addkillcount, 0, killed);

  for (uint8 i = 0; i < MAX_QUEST_LOG_SIZE; ++i) {
    uint32 questid = GetQuestSlotQuestId(i);
    if (!questid)
      continue;

    Quest const *qInfo = sObjectMgr->GetQuestTemplate(questid);
    if (!qInfo)
      continue;
    // just if !ingroup || !noraidgroup || raidgroup
    QuestStatusData &q_status = m_QuestStatus[questid];
    if (q_status.Status == QUEST_STATUS_INCOMPLETE &&
        (!GetGroup() || !GetGroup()->isRaidGroup() ||
         qInfo->IsAllowedInRaid(GetMap()->GetDifficulty()))) {
      if (qInfo->HasSpecialFlag(QUEST_SPECIAL_FLAGS_KILL) /*&& !qInfo->HasSpecialFlag(QUEST_SPECIAL_FLAGS_CAST)*/)
            {
        for (uint8 j = 0; j < QUEST_OBJECTIVES_COUNT; ++j) {
          // skip GO activate objective or none
          if (qInfo->RequiredNpcOrGo[j] <= 0)
            continue;

          uint32 reqkill = qInfo->RequiredNpcOrGo[j];

          if (reqkill == real_entry) {
            uint32 reqkillcount = qInfo->RequiredNpcOrGoCount[j];
            uint16 curkillcount = q_status.CreatureOrGOCount[j];
            if (curkillcount < reqkillcount) {
              q_status.CreatureOrGOCount[j] = curkillcount + addkillcount;

              m_QuestStatusSave[questid] = QUEST_DEFAULT_SAVE_TYPE;

              uint16 log_slot = FindQuestSlot(questid);
              if (log_slot < MAX_QUEST_LOG_SIZE)
                SetQuestSlotCounter(log_slot, j, curkillcount + addkillcount);

              SendQuestUpdateAddCredit(qInfo, guid, j,
                                       curkillcount + addkillcount);
            }
            if (CanCompleteQuest(questid))
              CompleteQuest(questid);

            // same objective target can be in many active quests, but not in 2
            // objectives for single quest (code optimization).
            break;
          }
        }
      }
    }
  }
}

void Player::KilledPlayerCredit(uint16 count) {
  for (uint8 i = 0; i < MAX_QUEST_LOG_SIZE; ++i) {
    uint32 questid = GetQuestSlotQuestId(i);
    if (!questid)
      continue;

    Quest const *qInfo = sObjectMgr->GetQuestTemplate(questid);
    if (!qInfo)
      continue;
    // just if !ingroup || !noraidgroup || raidgroup
    QuestStatusData &q_status = m_QuestStatus[questid];
    if (q_status.Status == QUEST_STATUS_INCOMPLETE &&
        (!GetGroup() || !GetGroup()->isRaidGroup() ||
         qInfo->IsAllowedInRaid(GetMap()->GetDifficulty()))) {
      // PvP Killing quest require player to be in same zone as quest zone (only
      // 2 quests so no doubt)
      if (qInfo->HasSpecialFlag(QUEST_SPECIAL_FLAGS_PLAYER_KILL) &&
          GetZoneId() == static_cast<uint32>(qInfo->GetZoneOrSort())) {
        KilledPlayerCreditForQuest(count, qInfo);
        break; // there is only one quest per zone
      }
    }
  }
}

void Player::KilledPlayerCreditForQuest(uint16 count, Quest const *quest) {
  uint32 const questId = quest->GetQuestId();
  auto it = m_QuestStatus.find(questId);
  if (it == m_QuestStatus.end())
    return;
  QuestStatusData &questStatus = it->second;

  uint16 curKill = questStatus.PlayerCount;
  uint32 reqKill = quest->GetPlayersSlain();

  if (curKill < reqKill) {
    count = std::min<uint16>(reqKill - curKill, count);
    questStatus.PlayerCount = curKill + count;

    m_QuestStatusSave[quest->GetQuestId()] = QUEST_DEFAULT_SAVE_TYPE;

    uint16 log_slot = FindQuestSlot(quest->GetQuestId());
    if (log_slot < MAX_QUEST_LOG_SIZE)
      SetQuestSlotCounter(log_slot, QUEST_PVP_KILL_SLOT, curKill + count);

    SendQuestUpdateAddPlayer(quest, curKill + count);
  }

  if (CanCompleteQuest(questId))
    CompleteQuest(questId);
}

void Player::KillCreditGO(uint32 entry, ObjectGuid guid) {
  uint16 addCastCount = 1;
  for (uint8 i = 0; i < MAX_QUEST_LOG_SIZE; ++i) {
    uint32 questid = GetQuestSlotQuestId(i);
    if (!questid)
      continue;

    Quest const *qInfo = sObjectMgr->GetQuestTemplate(questid);
    if (!qInfo)
      continue;

    QuestStatusData &q_status = m_QuestStatus[questid];

    if (q_status.Status == QUEST_STATUS_INCOMPLETE) {
      if (qInfo->HasSpecialFlag(QUEST_SPECIAL_FLAGS_CAST) /*&& !qInfo->HasSpecialFlag(QUEST_SPECIAL_FLAGS_KILL)*/)
            {
        for (uint8 j = 0; j < QUEST_OBJECTIVES_COUNT; ++j) {
          uint32 reqTarget = 0;

          // GO activate objective
          if (qInfo->RequiredNpcOrGo[j] < 0)
            // checked at quest_template loading
            reqTarget = -qInfo->RequiredNpcOrGo[j];

          // other not this creature/GO related objectives
          if (reqTarget != entry)
            continue;

          uint32 reqCastCount = qInfo->RequiredNpcOrGoCount[j];
          uint16 curCastCount = q_status.CreatureOrGOCount[j];
          if (curCastCount < reqCastCount) {
            q_status.CreatureOrGOCount[j] = curCastCount + addCastCount;

            m_QuestStatusSave[questid] = QUEST_DEFAULT_SAVE_TYPE;

            uint16 log_slot = FindQuestSlot(questid);
            if (log_slot < MAX_QUEST_LOG_SIZE)
              SetQuestSlotCounter(log_slot, j, curCastCount + addCastCount);

            SendQuestUpdateAddCredit(qInfo, guid, j,
                                     curCastCount + addCastCount);
          }

          if (CanCompleteQuest(questid))
            CompleteQuest(questid);

          // same objective target can be in many active quests, but not in 2
          // objectives for single quest (code optimization).
          break;
        }
      }
    }
  }
}

void Player::TalkedToCreature(uint32 entry, ObjectGuid guid) {
  uint16 addTalkCount = 1;
  for (uint8 i = 0; i < MAX_QUEST_LOG_SIZE; ++i) {
    uint32 questid = GetQuestSlotQuestId(i);
    if (!questid)
      continue;

    Quest const *qInfo = sObjectMgr->GetQuestTemplate(questid);
    if (!qInfo)
      continue;

    QuestStatusData &q_status = m_QuestStatus[questid];

    if (q_status.Status == QUEST_STATUS_INCOMPLETE) {
      if (qInfo->HasSpecialFlag(QUEST_SPECIAL_FLAGS_KILL |
                                QUEST_SPECIAL_FLAGS_CAST |
                                QUEST_SPECIAL_FLAGS_SPEAKTO)) {
        for (uint8 j = 0; j < QUEST_OBJECTIVES_COUNT; ++j) {
          // skip gameobject objectives
          if (qInfo->RequiredNpcOrGo[j] < 0)
            continue;

          uint32 reqTarget = 0;

          if (qInfo->RequiredNpcOrGo[j] > 0) // creature activate objectives
            // checked at quest_template loading
            reqTarget = qInfo->RequiredNpcOrGo[j];
          else
            continue;

          if (reqTarget == entry) {
            uint32 reqTalkCount = qInfo->RequiredNpcOrGoCount[j];
            uint16 curTalkCount = q_status.CreatureOrGOCount[j];
            if (curTalkCount < reqTalkCount) {
              q_status.CreatureOrGOCount[j] = curTalkCount + addTalkCount;

              m_QuestStatusSave[questid] = QUEST_DEFAULT_SAVE_TYPE;

              uint16 log_slot = FindQuestSlot(questid);
              if (log_slot < MAX_QUEST_LOG_SIZE)
                SetQuestSlotCounter(log_slot, j, curTalkCount + addTalkCount);

              SendQuestUpdateAddCredit(qInfo, guid, j,
                                       curTalkCount + addTalkCount);
            }
            if (CanCompleteQuest(questid))
              CompleteQuest(questid);

            // same objective target can be in many active quests, but not in 2
            // objectives for single quest (code optimization).
            continue;
          }
        }
      }
    }
  }
}

void Player::MoneyChanged(uint64 value) {
  for (uint8 i = 0; i < MAX_QUEST_LOG_SIZE; ++i) {
    uint32 questid = GetQuestSlotQuestId(i);
    if (!questid)
      continue;

    Quest const *qInfo = sObjectMgr->GetQuestTemplate(questid);
    if (qInfo && qInfo->GetRewOrReqMoney() < 0) {
      QuestStatusData &q_status = m_QuestStatus[questid];

      if (q_status.Status == QUEST_STATUS_INCOMPLETE) {
        if (int64(value) >= -int64(qInfo->GetRewOrReqMoney())) {
          if (CanCompleteQuest(questid))
            CompleteQuest(questid);
        }
      } else if (q_status.Status == QUEST_STATUS_COMPLETE) {
        if (int64(value) < -int64(qInfo->GetRewOrReqMoney()))
          IncompleteQuest(questid);
      }
    }
  }
}

void Player::ReputationChanged(FactionEntry const *factionEntry) {
  for (uint8 i = 0; i < MAX_QUEST_LOG_SIZE; ++i) {
    if (uint32 questid = GetQuestSlotQuestId(i)) {
      if (Quest const *qInfo = sObjectMgr->GetQuestTemplate(questid)) {
        if (qInfo->GetRepObjectiveFaction() == factionEntry->ID) {
          QuestStatusData &q_status = m_QuestStatus[questid];
          if (q_status.Status == QUEST_STATUS_INCOMPLETE) {
            if (GetReputationMgr().GetReputation(factionEntry) >=
                qInfo->GetRepObjectiveValue())
              if (CanCompleteQuest(questid))
                CompleteQuest(questid);
          } else if (q_status.Status == QUEST_STATUS_COMPLETE) {
            if (GetReputationMgr().GetReputation(factionEntry) <
                qInfo->GetRepObjectiveValue())
              IncompleteQuest(questid);
          }
        }
      }
    }
  }
}

void Player::ReputationChanged2(FactionEntry const *factionEntry) {
  for (uint8 i = 0; i < MAX_QUEST_LOG_SIZE; ++i) {
    if (uint32 questid = GetQuestSlotQuestId(i)) {
      if (Quest const *qInfo = sObjectMgr->GetQuestTemplate(questid)) {
        if (qInfo->GetRepObjectiveFaction2() == factionEntry->ID) {
          QuestStatusData &q_status = m_QuestStatus[questid];
          if (q_status.Status == QUEST_STATUS_INCOMPLETE) {
            if (GetReputationMgr().GetReputation(factionEntry) >=
                qInfo->GetRepObjectiveValue2())
              if (CanCompleteQuest(questid))
                CompleteQuest(questid);
          } else if (q_status.Status == QUEST_STATUS_COMPLETE) {
            if (GetReputationMgr().GetReputation(factionEntry) <
                qInfo->GetRepObjectiveValue2())
              IncompleteQuest(questid);
          }
        }
      }
    }
  }
}

bool Player::HasQuestForItem(uint32 itemid, uint32 excludeQuestId /* 0 */,
                             bool turnIn /* false */) const {
  for (uint8 i = 0; i < MAX_QUEST_LOG_SIZE; ++i) {
    uint32 questid = GetQuestSlotQuestId(i);
    if (questid == 0 || questid == excludeQuestId)
      continue;

    QuestStatusMap::const_iterator qs_itr = m_QuestStatus.find(questid);
    if (qs_itr == m_QuestStatus.end())
      continue;

    QuestStatusData const &q_status = qs_itr->second;

    if ((q_status.Status == QUEST_STATUS_INCOMPLETE) ||
        (turnIn && q_status.Status == QUEST_STATUS_COMPLETE)) {
      Quest const *qinfo = sObjectMgr->GetQuestTemplate(questid);
      if (!qinfo)
        continue;

      // hide quest if player is in raid-group and quest is no raid quest
      if (GetGroup() && GetGroup()->isRaidGroup() &&
          !qinfo->IsAllowedInRaid(GetMap()->GetDifficulty()))
        if (!InBattleground()) // there are two ways.. we can make every
                               // bg-quest a raidquest, or add this code here..
                               // i don't know if this can be exploited by other
                               // quests, but i think all other quests depend on
                               // a specific area.. but keep this in mind, if
                               // something strange happens later
          continue;

      // There should be no mixed ReqItem/ReqSource drop
      // This part for ReqItem drop
      for (uint8 j = 0; j < QUEST_ITEM_OBJECTIVES_COUNT; ++j) {
        if ((itemid == qinfo->RequiredItemId[j] &&
             q_status.ItemCount[j] < qinfo->RequiredItemCount[j]) ||
            (turnIn && q_status.ItemCount[j] >= qinfo->RequiredItemCount[j]))
          return true;
      }
      // This part - for ReqSource
      for (uint8 j = 0; j < QUEST_SOURCE_ITEM_IDS_COUNT; ++j) {
        // examined item is a source item
        if (qinfo->ItemDrop[j] == itemid) {
          ItemTemplate const *pProto = sObjectMgr->GetItemTemplate(itemid);

          uint32 ownedCount = GetItemCount(itemid, true);
          // 'unique' item
          if ((pProto->GetMaxCount() && ownedCount < pProto->GetMaxCount()) ||
              (turnIn && ownedCount >= pProto->GetMaxCount()))
            return true;

          // allows custom amount drop when not 0
          if (qinfo->ItemDropQuantity[j]) {
            if (ownedCount < qinfo->ItemDropQuantity[j] || turnIn)
              return true;
          } else if (ownedCount < pProto->GetMaxStackSize())
            return true;
        }
      }
    }
  }
  return false;
}

void Player::SendQuestComplete(Quest const *quest) const {
  if (quest) {
    WorldPackets::Quest::QuestUpdateComplete packet;
    packet.QuestID = quest->GetQuestId();
    SendDirectMessage(packet.Write());
  }
}

void Player::SendQuestReward(Quest const *quest, Creature const *questGiver,
                             uint32 xp) {
  uint32 questId = quest->GetQuestId();
  sGameEventMgr->HandleQuestComplete(questId);

  uint32 XP = 0;
  if (!IsMaxLevel())
    XP = xp;

  WorldPackets::Quest::QuestGiverQuestComplete packet;
  packet.QuestID = questId;
  packet.XPReward = XP;
  packet.MoneyReward = quest->GetRewOrReqMoney(this);
  packet.NumSkillUpsReward = quest->GetRewardSkillPoints();
  packet.TalentReward = quest->GetBonusTalents();
  packet.SkillLineIDReward = quest->GetRewardSkillId();

  if (questGiver) {
    if (questGiver->IsGossip() &&
        quest->HasFlag(QUEST_FLAGS_LAUNCH_GOSSIP_COMPLETE))
      packet.LaunchGossip = true;
    else if (quest->GetNextQuestInChain() &&
             !quest->HasFlag(QUEST_FLAGS_AUTOCOMPLETE))
      if (Quest const *rewardQuest =
              sObjectMgr->GetQuestTemplate(quest->GetNextQuestInChain()))
        packet.UseQuestReward = CanTakeQuest(rewardQuest, false);
  }

  SendDirectMessage(packet.Write());
}

void Player::SendQuestFailed(uint32 questId, InventoryResult reason) const {
  if (questId) {
    WorldPackets::Quest::QuestGiverQuestFailed packet;
    packet.QuestID = questId;
    packet.Reason = reason;
    SendDirectMessage(packet.Write());
  }
}

void Player::SendQuestTimerFailed(uint32 questId) const {
  if (questId) {
    WorldPackets::Quest::QuestUpdateFailedTimer packet;
    packet.QuestID = questId;
    SendDirectMessage(packet.Write());
  }
}

void Player::SendCanTakeQuestResponse(QuestFailedReason reason) const {
  WorldPackets::Quest::QuestGiverInvalidQuest packet;
  packet.Reason = reason;
  SendDirectMessage(packet.Write());
}

void Player::SendQuestConfirmAccept(Quest const *quest,
                                    Player *receiver) const {
  if (!receiver)
    return;

  WorldPackets::Quest::QuestConfirmAcceptResponse packet;

  packet.QuestTitle = quest->GetTitle();
  uint32 questID = quest->GetQuestId();

  LocaleConstant localeConstant =
      receiver->GetSession()->GetSessionDbLocaleIndex();
  if (localeConstant != LOCALE_enUS)
    if (QuestLocale const *pLocale = sObjectMgr->GetQuestLocale(questID))
      ObjectMgr::GetLocaleString(pLocale->Title, localeConstant,
                                 packet.QuestTitle);

  packet.QuestID = questID;
  packet.InitiatedBy = GetGUID();

  receiver->GetSession()->SendPacket(packet.Write());
}

void Player::SendPushToPartyResponse(Player const *player, uint8 msg) const {
  if (player) {
    WorldPackets::Quest::QuestPushResultResponse packet;
    packet.SenderGUID = player->GetGUID();
    packet.Result = msg;
    SendDirectMessage(packet.Write());
  }
}

void Player::SendQuestUpdateAddCredit(Quest const *quest, ObjectGuid guid,
                                      uint32 creatureOrGOIdx, uint16 count) {
  int32 entry = quest->RequiredNpcOrGo[creatureOrGOIdx];
  if (entry < 0)
    // client expected gameobject template id in form (id|0x80000000)
    entry = (-entry) | 0x80000000;

  WorldPackets::Quest::QuestUpdateAddCredit packet;
  packet.VictimGUID = guid;
  packet.QuestID = quest->GetQuestId();
  packet.ObjectID = entry;
  packet.Count = count;
  packet.Required = quest->RequiredNpcOrGoCount[creatureOrGOIdx];
  SendDirectMessage(packet.Write());
}

void Player::SendQuestUpdateAddPlayer(Quest const *quest, uint16 newCount) {
  WorldPackets::Quest::QuestUpdateAddPvPCredit packet;
  packet.QuestID = quest->GetQuestId();
  packet.Count = newCount;
  packet.Required = quest->GetPlayersSlain();
  SendDirectMessage(packet.Write());
}

void Player::SendQuestGiverStatusMultiple() {
  WorldPackets::Quest::QuestGiverStatusMultiple response;

  for (auto itr = m_clientGUIDs.begin(); itr != m_clientGUIDs.end(); ++itr) {
    if (itr->IsAnyTypeCreature()) {
      // need also pet quests case support
      Creature *questgiver =
          ObjectAccessor::GetCreatureOrPetOrVehicle(*this, *itr);
      if (!questgiver || questgiver->IsHostileTo(this))
        continue;
      if (!questgiver->HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER))
        continue;

      response.QuestGiver.emplace_back(questgiver->GetGUID(),
                                       GetQuestDialogStatus(questgiver));
    } else if (itr->IsGameObject()) {
      GameObject *questgiver = GetMap()->GetGameObject(*itr);
      if (!questgiver || questgiver->GetGoType() != GAMEOBJECT_TYPE_QUESTGIVER)
        continue;

      response.QuestGiver.emplace_back(questgiver->GetGUID(),
                                       GetQuestDialogStatus(questgiver));
    }
  }

  SendDirectMessage(response.Write());
}

bool Player::HasPvPForcingQuest() const {
  for (uint8 i = 0; i < MAX_QUEST_LOG_SIZE; ++i) {
    uint32 questId = GetQuestSlotQuestId(i);
    if (questId == 0)
      continue;

    Quest const *quest = sObjectMgr->GetQuestTemplate(questId);
    if (!quest)
      continue;

    if (quest->HasFlag(QUEST_FLAGS_FLAGS_PVP))
      return true;
  }

  return false;
}

/*********************************************************/
/***                   LOAD SYSTEM                     ***/
/*********************************************************/

void Player::Initialize(ObjectGuid::LowType guid) {
  Object::_Create(guid, 0, HighGuid::Player);
}

void Player::_LoadDeclinedNames(PreparedQueryResult result) {
  if (!result)
    return;

  delete m_declinedname;
  m_declinedname = new DeclinedName;
  for (uint8 i = 0; i < MAX_DECLINED_NAME_CASES; ++i)
    m_declinedname->name[i] = (*result)[i].Get<std::string>();
}

void Player::_LoadArenaTeamInfo(PreparedQueryResult result) {
  // arenateamid, played_week, played_season, personal_rating
  memset((void *)&m_uint32Values[PLAYER_FIELD_ARENA_TEAM_INFO_1_1], 0,
         sizeof(uint32) * MAX_ARENA_SLOT * ARENA_TEAM_END);

  uint16 personalRatingCache[] = {0, 0, 0};

  if (result) {
    do {
      Field *fields = result->Fetch();

      uint32 arenaTeamId = fields[0].Get<uint32>();

      ArenaTeam *arenaTeam = sArenaTeamMgr->GetArenaTeamById(arenaTeamId);
      if (!arenaTeam) {
        LOG_ERROR("entities.player",
                  "Player::_LoadArenaTeamInfo: couldn't load arenateam %u",
                  arenaTeamId);
        continue;
      }

      uint8 arenaSlot = arenaTeam->GetSlot();

      personalRatingCache[arenaSlot] = fields[4].Get<uint16>();

      SetArenaTeamInfoField(arenaSlot, ARENA_TEAM_ID, arenaTeamId);
      SetArenaTeamInfoField(arenaSlot, ARENA_TEAM_TYPE, arenaTeam->GetType());
      SetArenaTeamInfoField(arenaSlot, ARENA_TEAM_MEMBER,
                            (arenaTeam->GetCaptain() == GetGUID()) ? 0 : 1);
      SetArenaTeamInfoField(arenaSlot, ARENA_TEAM_GAMES_WEEK,
                            uint32(fields[1].Get<uint16>()));
      SetArenaTeamInfoField(arenaSlot, ARENA_TEAM_GAMES_SEASON,
                            uint32(fields[2].Get<uint16>()));
      SetArenaTeamInfoField(arenaSlot, ARENA_TEAM_WINS_SEASON,
                            uint32(fields[3].Get<uint16>()));
    } while (result->NextRow());
  }

  for (uint8 slot = 0; slot <= 2; ++slot) {
    SetArenaTeamInfoField(slot, ARENA_TEAM_PERSONAL_RATING,
                          uint32(personalRatingCache[slot]));
  }
}

void Player::_LoadEquipmentSets(PreparedQueryResult result) {
  // SetPQuery(PLAYER_LOGIN_QUERY_LOADEQUIPMENTSETS,   "SELECT setguid,
  // setindex, name, iconname, item0, item1, item2, item3, item4, item5, item6,
  // item7, item8, item9, item10, item11, item12, item13, item14, item15,
  // item16, item17, item18 FROM character_equipmentsets WHERE guid = '%u' ORDER
  // BY setindex", GUID_LOPART(m_guid));
  if (!result)
    return;

  do {
    Field *fields = result->Fetch();
    EquipmentSetInfo eqSet;

    eqSet.Data.Guid = fields[0].Get<uint64>();
    eqSet.Data.SetID = fields[1].Get<uint8>();
    eqSet.Data.SetName = fields[2].Get<std::string>();
    eqSet.Data.SetIcon = fields[3].Get<std::string>();
    eqSet.Data.IgnoreMask = fields[4].Get<uint32>();
    eqSet.State = EQUIPMENT_SET_UNCHANGED;

    for (uint32 i = 0; i < EQUIPMENT_SLOT_END; ++i)
      if (ObjectGuid::LowType guid = fields[5 + i].Get<uint32>())
        eqSet.Data.Pieces[i] = ObjectGuid::Create<HighGuid::Item>(guid);

    // client limit
    if (eqSet.Data.SetID >= MAX_EQUIPMENT_SET_INDEX)
      continue;

    _equipmentSets[eqSet.Data.Guid] = eqSet;
  } while (result->NextRow());
}

void Player::_LoadBGData(PreparedQueryResult result) {
  if (!result)
    return;

  Field *fields = result->Fetch();
  // Expecting only one row
  //        0           1     2      3      4      5      6          7 8 9
  // SELECT instanceId, team, joinX, joinY, joinZ, joinO, joinMapId, taxiStart,
  // taxiEnd, mountSpell FROM character_battleground_data WHERE guid = ?

  m_bgData.bgInstanceID = fields[0].Get<uint32>();
  m_bgData.bgTeam = fields[1].Get<uint16>();
  m_bgData.joinPos = WorldLocation(fields[6].Get<uint16>(), // Map
                                   fields[2].Get<float>(),  // X
                                   fields[3].Get<float>(),  // Y
                                   fields[4].Get<float>(),  // Z
                                   fields[5].Get<float>()); // Orientation
  m_bgData.taxiPath[0] = fields[7].Get<uint32>();
  m_bgData.taxiPath[1] = fields[8].Get<uint32>();
  m_bgData.mountSpell = fields[9].Get<uint32>();
}

bool Player::LoadPositionFromDB(uint32 &mapid, float &x, float &y, float &z,
                                float &o, bool &in_flight, ObjectGuid guid) {
  CharacterDatabasePreparedStatement *stmt =
      CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHAR_POSITION);
  stmt->SetData(0, guid.GetCounter());
  PreparedQueryResult result = CharacterDatabase.Query(stmt);

  if (!result)
    return false;

  Field *fields = result->Fetch();

  x = fields[0].Get<float>();
  y = fields[1].Get<float>();
  z = fields[2].Get<float>();
  o = fields[3].Get<float>();
  mapid = fields[4].Get<uint16>();
  in_flight = !fields[5].Get<std::string>().empty();

  return true;
}

void Player::SetHomebind(WorldLocation const &loc, uint32 areaId) {
  loc.GetPosition(m_homebindX, m_homebindY, m_homebindZ);
  m_homebindMapId = loc.GetMapId();
  m_homebindAreaId = areaId;

  // update sql homebind
  CharacterDatabasePreparedStatement *stmt =
      CharacterDatabase.GetPreparedStatement(CHAR_UPD_PLAYER_HOMEBIND);
  stmt->SetData(0, m_homebindMapId);
  stmt->SetData(1, m_homebindAreaId);
  stmt->SetData(2, m_homebindX);
  stmt->SetData(3, m_homebindY);
  stmt->SetData(4, m_homebindZ);
  stmt->SetData(5, GetGUID().GetCounter());
  CharacterDatabase.Execute(stmt);
}

void Player::SendBindPointUpdate() const {
  WorldPackets::Misc::BindPointUpdate packet;
  packet.BindPosition = Position(m_homebindX, m_homebindY, m_homebindZ);
  packet.BindMapID = m_homebindMapId;
  packet.BindAreaID = m_homebindAreaId;
  SendDirectMessage(packet.Write());
}

bool Player::IsLoading() const { return GetSession()->PlayerLoading(); }

bool Player::LoadFromDB(ObjectGuid guid,
                        CharacterDatabaseQueryHolder const &holder) {
  ////                                                     0     1        2 3 4
  /// 5       6      7   8      9     10    11         12         13 14 15 16
  // QueryResult* result = CharacterDatabase.PQuery("SELECT guid, account, name,
  // race, class, gender, level, xp, money, skin, face, hairStyle, hairColor,
  // facialStyle, bankSlots, restState, playerFlags, "
  //  17         18          19          20   21           22        23 24 25 26
  //  27           28                 29
  //"position_x, position_y, position_z, map, orientation, taximask, cinematic,
  // totaltime, leveltime, rest_bonus, logout_time, is_logout_resting,
  // resettalents_cost, "
  // 30                 31          32       33       34       35       36 37 38
  // 39        40    41      42                 43         44
  //"resettalents_time, talentTree, trans_x, trans_y, trans_z, trans_o,
  // transguid, extra_flags, stable_slots, at_login, zone, online,
  // death_expire_time, taxi_path, instance_mode_mask, "
  // 45          46          47              48           49              50
  //"totalKills, todayKills, yesterdayKills, chosenTitle, watchedFaction, drunk,
  //"
  // 51      52      53      54      55      56      57           58         59
  // 60             61
  //"health, power1, power2, power3, power4, power5, instance_id, speccount,
  // activespec, exploredZones, equipmentCache, "
  // 62           63          64               65
  //"knownTitles, actionBars, grantableLevels, fishing_steps FROM characters
  // WHERE guid = '%u'", guid);

  PreparedQueryResult result =
      holder.GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_FROM);
  if (!result) {
    std::string name = "<unknown>";
    sCharacterCache->GetCharacterNameByGuid(guid, name);
    LOG_ERROR("entities.player",
              "Player::LoadFromDB: Player '%s' (%s) not found in table "
              "`characters`, can't load. ",
              name.c_str(), guid.ToString().c_str());
    return false;
  }

  Field *fields = result->Fetch();

  uint32 dbAccountId = fields[1].Get<uint32>();

  // check if the character's account in the db and the logged in account match.
  // player should be able to load/delete character only with correct account!
  if (dbAccountId != GetSession()->GetAccountId()) {
    LOG_ERROR("entities.player",
              "Player::LoadFromDB: Player (%s) loading from wrong account (is: "
              "%u, should be: %u)",
              guid.ToString().c_str(), GetSession()->GetAccountId(),
              dbAccountId);
    return false;
  }

  if (holder.GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_BANNED)) {
    LOG_ERROR("entities.player",
              "Player::LoadFromDB: Player (%s) is banned, can't load.",
              guid.ToString().c_str());
    return false;
  }

  Object::_Create(guid.GetCounter(), 0, HighGuid::Player);

  m_name = fields[2].Get<std::string>();

  // check name limitations
  if (ObjectMgr::CheckPlayerName(m_name, GetSession()->GetSessionDbcLocale()) !=
          CHAR_NAME_SUCCESS ||
      (!GetSession()->HasPermission(
           rbac::RBAC_PERM_SKIP_CHECK_CHARACTER_CREATION_RESERVEDNAME) &&
       sObjectMgr->IsReservedName(m_name))) {
    CharacterDatabasePreparedStatement *stmt =
        CharacterDatabase.GetPreparedStatement(CHAR_UPD_ADD_AT_LOGIN_FLAG);
    stmt->SetData(0, uint16(AT_LOGIN_RENAME));
    stmt->SetData(1, guid.GetCounter());
    CharacterDatabase.Execute(stmt);
    return false;
  }

  // overwrite possible wrong/corrupted guid
  SetGuidValue(OBJECT_FIELD_GUID, guid);

  uint8 gender = fields[5].Get<uint8>();
  if (!IsValidGender(gender)) {
    LOG_ERROR(
        "entities.player",
        "Player::LoadFromDB: Player (%s) has wrong gender (%u), can't load.",
        guid.ToString().c_str(), gender);
    return false;
  }

  SetByteValue(UNIT_FIELD_BYTES_0, UNIT_BYTES_0_OFFSET_RACE,
               fields[3].Get<uint8>());
  SetByteValue(UNIT_FIELD_BYTES_0, UNIT_BYTES_0_OFFSET_CLASS,
               fields[4].Get<uint8>());
  SetByteValue(UNIT_FIELD_BYTES_0, UNIT_BYTES_0_OFFSET_GENDER, gender);

  // check if race/class combination is valid
  PlayerInfo const *info = sObjectMgr->GetPlayerInfo(getRace(), getClass());
  if (!info) {
    LOG_ERROR("entities.player",
              "Player::LoadFromDB: Player (%s) has wrong race/class (%u/%u), "
              "can't load.",
              guid.ToString().c_str(), getRace(), getClass());
    return false;
  }

  SetUInt32Value(UNIT_FIELD_LEVEL, fields[6].Get<uint8>());
  SetUInt32Value(PLAYER_XP, fields[7].Get<uint32>());

  _LoadIntoDataField(fields[60].Get<std::string>(), PLAYER_EXPLORED_ZONES_1,
                     PLAYER_EXPLORED_ZONES_SIZE);
  _LoadIntoDataField(fields[62].Get<std::string>(), PLAYER__FIELD_KNOWN_TITLES,
                     KNOWN_TITLES_SIZE * 2);

  SetObjectScale(1.0f);
  SetFloatValue(UNIT_FIELD_HOVERHEIGHT, 1.0f);

  // load achievements before anything else to prevent multiple gains for the
  // same achievement/criteria on every loading (as loading does call
  // UpdateAchievementCriteria)
  m_achievementMgr->LoadFromDB(
      holder.GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_ACHIEVEMENTS),
      holder.GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_CRITERIA_PROGRESS));

  uint64 money = fields[8].Get<uint64>();
  if (money > MAX_MONEY_AMOUNT)
    money = MAX_MONEY_AMOUNT;
  SetMoney(money);

  SetByteValue(PLAYER_BYTES, PLAYER_BYTES_OFFSET_SKIN_ID,
               fields[9].Get<uint8>());
  SetByteValue(PLAYER_BYTES, PLAYER_BYTES_OFFSET_FACE_ID,
               fields[10].Get<uint8>());
  SetByteValue(PLAYER_BYTES, PLAYER_BYTES_OFFSET_HAIR_STYLE_ID,
               fields[11].Get<uint8>());
  SetByteValue(PLAYER_BYTES, PLAYER_BYTES_OFFSET_HAIR_COLOR_ID,
               fields[12].Get<uint8>());
  SetByteValue(PLAYER_BYTES_2, PLAYER_BYTES_2_OFFSET_FACIAL_STYLE,
               fields[13].Get<uint8>());
  SetByteValue(PLAYER_BYTES_2, PLAYER_BYTES_2_OFFSET_BANK_BAG_SLOTS,
               fields[14].Get<uint8>());
  SetByteValue(PLAYER_BYTES_2, PLAYER_BYTES_2_OFFSET_REST_STATE,
               fields[15].Get<uint8>());
  SetByteValue(PLAYER_BYTES_3, PLAYER_BYTES_3_OFFSET_GENDER,
               fields[5].Get<uint8>());
  SetByteValue(PLAYER_BYTES_3, PLAYER_BYTES_3_OFFSET_INEBRIATION,
               fields[50].Get<uint8>());

  if (!ValidateAppearance(
          fields[3].Get<uint8>(), // race
          fields[4].Get<uint8>(), // class
          gender,
          GetByteValue(PLAYER_BYTES,
                       PLAYER_BYTES_OFFSET_HAIR_STYLE_ID), // hair type
          GetByteValue(PLAYER_BYTES,
                       PLAYER_BYTES_OFFSET_HAIR_COLOR_ID),         // hair color
          GetByteValue(PLAYER_BYTES, PLAYER_BYTES_OFFSET_FACE_ID), // face
          GetByteValue(PLAYER_BYTES_2,
                       PLAYER_BYTES_2_OFFSET_FACIAL_STYLE), // facial hair
          GetByteValue(PLAYER_BYTES,
                       PLAYER_BYTES_OFFSET_SKIN_ID))) // skin color
  {
    LOG_ERROR("entities.player",
              "Player::LoadFromDB: Player (%s) has wrong Appearance values "
              "(Hair/Skin/Color), can't load.",
              guid.ToString().c_str());
    return false;
  }

  SetUInt32Value(PLAYER_FLAGS, fields[16].Get<uint32>());
  SetInt32Value(PLAYER_FIELD_WATCHED_FACTION_INDEX, fields[49].Get<uint32>());

  // set which actionbars the client has active - DO NOT REMOVE EVER AGAIN (can
  // be changed though, if it does change fieldwise)
  SetByteValue(PLAYER_FIELD_BYTES, PLAYER_FIELD_BYTES_OFFSET_ACTION_BAR_TOGGLES,
               fields[63].Get<uint8>());

  m_fishingSteps = fields[65].Get<uint8>();

  InitDisplayIds();

  // cleanup inventory related item value fields (it will be filled correctly in
  // _LoadInventory)
  for (uint8 slot = EQUIPMENT_SLOT_START; slot < EQUIPMENT_SLOT_END; ++slot) {
    SetGuidValue(PLAYER_FIELD_INV_SLOT_HEAD + (slot * 2), ObjectGuid::Empty);
    SetVisibleItemSlot(slot, nullptr);

    delete m_items[slot];
    m_items[slot] = nullptr;
  }

  LOG_DEBUG("entities.player.loading",
            "Player::LoadFromDB: Load Basic value of player '%s' is: ",
            m_name.c_str());
  outDebugValues();

  // Need to call it to initialize m_team (m_team can be calculated from race)
  // Other way is to saves m_team into characters table.
  SetFactionForRace(getRace());

  // load home bind and check in same time class/race pair, it used later for
  // restore broken positions
  if (!_LoadHomeBind(
          holder.GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_HOME_BIND)))
    return false;

  InitializeSkillFields();
  InitPrimaryProfessions(); // to max set before any spell loaded

  // init saved position, and fix it later if problematic
  ObjectGuid::LowType transLowGUID = fields[36].Get<uint32>();

  Relocate(fields[17].Get<float>(), fields[18].Get<float>(),
           fields[19].Get<float>(), fields[21].Get<float>());

  uint32 mapId = fields[20].Get<uint16>();
  uint32 instanceId = fields[57].Get<uint32>();

  uint32 dungeonDiff = fields[44].Get<uint8>() & 0x0F;
  if (dungeonDiff >= MAX_DUNGEON_DIFFICULTY)
    dungeonDiff = DUNGEON_DIFFICULTY_NORMAL;
  uint32 raidDiff = (fields[44].Get<uint8>() >> 4) & 0x0F;
  if (raidDiff >= MAX_RAID_DIFFICULTY)
    raidDiff = RAID_DIFFICULTY_10MAN_NORMAL;
  SetDungeonDifficulty(Difficulty(dungeonDiff)); // may be changed in _LoadGroup
  SetRaidDifficulty(Difficulty(raidDiff));       // may be changed in _LoadGroup

  std::string taxi_nodes = fields[43].Get<std::string>();

#define RelocateToHomebind()                                                   \
  {                                                                            \
    mapId = m_homebindMapId;                                                   \
    instanceId = 0;                                                            \
    Relocate(m_homebindX, m_homebindY, m_homebindZ);                           \
  }

  _LoadGroup(holder.GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_GROUP));

  _LoadArenaTeamInfo(
      holder.GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_ARENA_INFO));

  // check arena teams integrity
  for (uint32 arena_slot = 0; arena_slot < MAX_ARENA_SLOT; ++arena_slot) {
    uint32 arena_team_id = GetArenaTeamId(arena_slot);
    if (!arena_team_id)
      continue;

    if (ArenaTeam *at = sArenaTeamMgr->GetArenaTeamById(arena_team_id))
      if (at->IsMember(GetGUID()))
        continue;

    // arena team not exist or not member, cleanup fields
    for (int j = 0; j < 6; ++j)
      SetArenaTeamInfoField(arena_slot, ArenaTeamInfoType(j), 0);
  }

  _LoadCurrency(holder.GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_CURRENCY));
  SetUInt32Value(PLAYER_FIELD_LIFETIME_HONORABLE_KILLS,
                 fields[45].Get<uint32>());
  SetUInt16Value(PLAYER_FIELD_KILLS, 0, fields[46].Get<uint16>());
  SetUInt16Value(PLAYER_FIELD_KILLS, 1, fields[47].Get<uint16>());

  _LoadBoundInstances(
      holder.GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_BOUND_INSTANCES));
  _LoadInstanceTimeRestrictions(
      holder.GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_INSTANCE_LOCK_TIMES));
  _LoadBGData(holder.GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_BG_DATA));

  GetSession()->SetPlayer(this);
  MapEntry const *mapEntry = sMapStore.LookupEntry(mapId);
  Map *map = nullptr;
  bool player_at_bg = false;
  if (!mapEntry || !IsPositionValid()) {
    LOG_ERROR(
        "entities.player",
        "Player::LoadFromDB: Player (%s) has invalid coordinates (MapId: %u X: "
        "%f Y: %f Z: %f O: %f). Teleport to default race/class locations.",
        guid.ToString().c_str(), mapId, GetPositionX(), GetPositionY(),
        GetPositionZ(), GetOrientation());
    RelocateToHomebind();
  }
  // Player was saved in Arena or Bg
  else if (mapEntry->IsBattlegroundOrArena()) {
    Battleground *currentBg = nullptr;
    if (m_bgData.bgInstanceID) // saved in Battleground
      currentBg = sBattlegroundMgr->GetBattleground(m_bgData.bgInstanceID,
                                                    BATTLEGROUND_TYPE_NONE);

    player_at_bg = currentBg && currentBg->IsPlayerInBattleground(GetGUID());

    if (player_at_bg && currentBg->GetStatus() != STATUS_WAIT_LEAVE) {
      map = currentBg->GetBgMap();

      BattlegroundQueueTypeId bgQueueTypeId = sBattlegroundMgr->BGQueueTypeId(
          currentBg->GetTypeID(), currentBg->GetArenaType());
      AddBattlegroundQueueId(bgQueueTypeId);

      m_bgData.bgTypeID = currentBg->GetTypeID();

      // join player to battleground group
      currentBg->EventPlayerLoggedIn(this);

      SetInviteForBattlegroundQueueType(bgQueueTypeId,
                                        currentBg->GetInstanceID());
    }
    // Bg was not found - go to Entry Point
    else {
      // leave bg
      if (player_at_bg) {
        player_at_bg = false;
        currentBg->RemovePlayerAtLeave(GetGUID(), false, true);
      }

      // Do not look for instance if bg not found
      WorldLocation const &_loc = GetBattlegroundEntryPoint();
      mapId = _loc.GetMapId();
      instanceId = 0;

      // Db field type is type int16, so it can never be MAPID_INVALID
      // if (mapId == MAPID_INVALID) -- code kept for reference
      if (int16(mapId) == int16(-1)) // Battleground Entry Point not found (???)
      {
        LOG_ERROR("entities.player",
                  "Player::LoadFromDB: Player (%s) was in BG in database, but "
                  "BG was not found and entry point was invalid! Teleport to "
                  "default race/class locations.",
                  guid.ToString().c_str());
        RelocateToHomebind();
      } else
        Relocate(&_loc);

      // We are not in BG anymore
      m_bgData.bgInstanceID = 0;
    }
  }
  // currently we do not support transport in bg
  else if (transLowGUID) {
    ObjectGuid transGUID(HighGuid::Mo_Transport, transLowGUID);

    Transport *transport = nullptr;
    if (Map *transportMap = sMapMgr->CreateMap(mapId, this, instanceId)) {
      if (Transport *transportOnMap = transportMap->GetTransport(transGUID)) {
        if (transportOnMap->GetExpectedMapId() != mapId) {
          mapId = transportOnMap->GetExpectedMapId();
          instanceId = 0;
          transportMap = sMapMgr->CreateMap(mapId, this, instanceId);
          if (transportMap)
            transport = transportMap->GetTransport(transGUID);
        } else
          transport = transportOnMap;
      }
    }

    if (transport) {
      float x = fields[32].Get<float>(), y = fields[33].Get<float>(),
            z = fields[34].Get<float>(), o = fields[35].Get<float>();
      m_movementInfo.transport.pos.Relocate(x, y, z, o);
      transport->CalculatePassengerPosition(x, y, z, &o);

      if (!Firelands::IsValidMapCoord(x, y, z, o) ||
          // transport size limited
          std::fabs(m_movementInfo.transport.pos.GetPositionX()) > 250.0f ||
          std::fabs(m_movementInfo.transport.pos.GetPositionY()) > 250.0f ||
          std::fabs(m_movementInfo.transport.pos.GetPositionZ()) > 250.0f) {
        LOG_ERROR(
            "entities.player",
            "Player::LoadFromDB: Player (%s) has invalid transport coordinates "
            "(X: %f Y: %f Z: %f O: %f). Teleport to bind location.",
            guid.ToString().c_str(), x, y, z, o);

        m_movementInfo.transport.Reset();

        RelocateToHomebind();
      } else {
        Relocate(x, y, z, o);
        mapId = transport->GetMapId();

        transport->AddPassenger(this);
      }
    } else {
      LOG_ERROR("entities.player",
                "Player::LoadFromDB: Player (%s) has problems with transport "
                "guid (%u). Teleport to bind location.",
                guid.ToString().c_str(), transLowGUID);

      RelocateToHomebind();
    }
  }
  // currently we do not support taxi in instance
  else if (!taxi_nodes.empty()) {
    instanceId = 0;

    // Not finish taxi flight path
    if (m_bgData.HasTaxiPath()) {
      for (int i = 0; i < 2; ++i)
        m_taxi.AddTaxiDestination(m_bgData.taxiPath[i]);
    } else if (!m_taxi.LoadTaxiDestinationsFromString(taxi_nodes, GetTeam())) {
      // problems with taxi path loading
      TaxiNodesEntry const *nodeEntry = nullptr;
      if (uint32 node_id = m_taxi.GetTaxiSource())
        nodeEntry = sTaxiNodesStore.LookupEntry(node_id);

      if (!nodeEntry) // don't know taxi start node, teleport to homebind
      {
        LOG_ERROR("entities.player",
                  "Player::LoadFromDB: Player (%s) has wrong data in taxi "
                  "destination list, teleport to homebind.",
                  GetGUID().ToString().c_str());
        RelocateToHomebind();
      } else // has start node, teleport to it
      {
        LOG_ERROR("entities.player",
                  "Player::LoadFromDB: Player (%s) has too short taxi "
                  "destination list, teleport to original node.",
                  GetGUID().ToString().c_str());
        mapId = nodeEntry->ContinentID;
        Relocate(nodeEntry->Pos.X, nodeEntry->Pos.Y, nodeEntry->Pos.Z, 0.0f);
      }
      m_taxi.ClearTaxiDestinations();
    }

    if (uint32 node_id = m_taxi.GetTaxiSource()) {
      // save source node as recall coord to prevent recall and fall from sky
      TaxiNodesEntry const *nodeEntry = sTaxiNodesStore.LookupEntry(node_id);
      if (nodeEntry && nodeEntry->ContinentID == GetMapId()) {
        ASSERT(nodeEntry); // checked in m_taxi.LoadTaxiDestinationsFromString
        mapId = nodeEntry->ContinentID;
        Relocate(nodeEntry->Pos.X, nodeEntry->Pos.Y, nodeEntry->Pos.Z, 0.0f);
      }

      // flight will started later
    }
  }

  // Map could be changed before
  mapEntry = sMapStore.LookupEntry(mapId);
  // client without expansion support
  if (mapEntry) {
    if (GetSession()->GetExpansion() < mapEntry->Expansion()) {
      LOG_DEBUG("entities.player.loading",
                "Player::LoadFromDB: Player '%s' (%s) using client without "
                "required expansion tried login at non accessible map %u",
                GetName().c_str(), GetGUID().ToString().c_str(), mapId);
      RelocateToHomebind();
    }

    // fix crash (because of if (Map* map = _FindMap(instanceId)) in
    // MapInstanced::CreateInstance)
    if (instanceId)
      if (InstanceSave *save = GetInstanceSave(mapId, mapEntry->IsRaid()))
        if (save->GetInstanceId() != instanceId)
          instanceId = 0;
  }

  // NOW player must have valid map
  // load the player's map here if it's not already loaded
  if (!map)
    map = sMapMgr->CreateMap(mapId, this, instanceId);
  AreaTriggerStruct const *areaTrigger = nullptr;
  bool check = false;

  if (!map) {
    areaTrigger = sObjectMgr->GetGoBackTrigger(mapId);
    check = true;
  } else if (map->IsDungeon()) // if map is dungeon...
  {
    if (Map::EnterState denyReason =
            ((InstanceMap *)map)
                ->CannotEnter(this)) // ... and can't enter map, then look for
                                     // entry point.
    {
      switch (denyReason) {
      case Map::CANNOT_ENTER_DIFFICULTY_UNAVAILABLE:
        SendTransferAborted(map->GetId(), TRANSFER_ABORT_DIFFICULTY,
                            map->GetDifficulty());
        break;
      case Map::CANNOT_ENTER_INSTANCE_BIND_MISMATCH:
        SendTransferAborted(map->GetId(),
                            TRANSFER_ABORT_LOCKED_TO_DIFFERENT_INSTANCE);
        break;
      case Map::CANNOT_ENTER_TOO_MANY_INSTANCES:
        SendTransferAborted(map->GetId(), TRANSFER_ABORT_TOO_MANY_INSTANCES);
        break;
      case Map::CANNOT_ENTER_MAX_PLAYERS:
        SendTransferAborted(map->GetId(), TRANSFER_ABORT_MAX_PLAYERS);
        break;
      case Map::CANNOT_ENTER_ZONE_IN_COMBAT:
        SendTransferAborted(map->GetId(), TRANSFER_ABORT_ZONE_IN_COMBAT);
        break;
      default:
        break;
      }
      areaTrigger = sObjectMgr->GetGoBackTrigger(mapId);
      check = true;
    } else if (instanceId && !sInstanceSaveMgr->GetInstanceSave(
                                 instanceId)) // ... and instance is reseted
                                              // then look for entrance.
    {
      areaTrigger = sObjectMgr->GetMapEntranceTrigger(mapId);
      check = true;
    }
  }

  if (check) // in case of special event when creating map...
  {
    if (areaTrigger) // ... if we have an areatrigger, then relocate to new
                     // map/coordinates.
    {
      Relocate(areaTrigger->target_X, areaTrigger->target_Y,
               areaTrigger->target_Z, GetOrientation());
      if (mapId != areaTrigger->target_mapId) {
        mapId = areaTrigger->target_mapId;
        map = sMapMgr->CreateMap(mapId, this);
      }
    } else {
      LOG_ERROR("entities.player",
                "Player::LoadFromDB: Player '%s' (%s) Map: %u, X: %f, Y: %f, "
                "Z: %f, O: %f. Areatrigger not found.",
                m_name.c_str(), guid.ToString().c_str(), mapId, GetPositionX(),
                GetPositionY(), GetPositionZ(), GetOrientation());
      RelocateToHomebind();
      map = nullptr;
    }
  }

  if (!map) {
    mapId = info->mapId;
    Relocate(info->positionX, info->positionY, info->positionZ, 0.0f);
    map = sMapMgr->CreateMap(mapId, this);
    if (!map) {
      LOG_ERROR("entities.player",
                "Player::LoadFromDB: Player '%s' (%s) Map: %u, X: %f, Y: %f, "
                "Z: %f, O: %f. Invalid default map coordinates or instance "
                "couldn't be created.",
                m_name.c_str(), guid.ToString().c_str(), mapId, GetPositionX(),
                GetPositionY(), GetPositionZ(), GetOrientation());
      return false;
    }
  }

  SetMap(map);
  StoreRaidMapDifficulty();

  // now that map position is determined, check instance validity
  if (!CheckInstanceValidity(true) && !IsInstanceLoginGameMasterException())
    m_InstanceValid = false;

  if (player_at_bg)
    map->ToBattlegroundMap()->GetBG()->AddPlayer(this);

  // randomize first save time in range [CONFIG_INTERVAL_SAVE] around
  // [CONFIG_INTERVAL_SAVE] this must help in case next save after mass player
  // load after server startup
  m_nextSave = urand(m_nextSave / 2, m_nextSave * 3 / 2);

  SaveRecallPosition();

  time_t now = GameTime::GetGameTime();
  time_t logoutTime = time_t(fields[27].Get<uint32>());

  // since last logout (in seconds)
  uint32 time_diff = uint32(
      now - logoutTime); // uint64 is excessive for a time_diff in seconds..
                         // uint32 allows for 136~ year difference.

  // set value, including drunk invisibility detection
  // calculate sobering. after 15 minutes logged out, the player will be sober
  // again
  uint8 newDrunkValue = 0;
  if (time_diff < uint32(GetDrunkValue()) * 9)
    newDrunkValue = GetDrunkValue() - time_diff / 9;

  SetDrunkValue(newDrunkValue);

  m_cinematic = fields[23].Get<uint8>();
  m_Played_time[PLAYED_TIME_TOTAL] = fields[24].Get<uint32>();
  m_Played_time[PLAYED_TIME_LEVEL] = fields[25].Get<uint32>();

  SetTalentResetCost(fields[29].Get<uint32>());
  SetTalentResetTime(time_t(fields[30].Get<uint32>()));

  SetSpecsCount(fields[58].Get<uint8>());
  SetActiveSpec(fields[59].Get<uint8>());

  // sanity check
  if (GetSpecsCount() > MAX_TALENT_SPECS || GetActiveSpec() > MAX_TALENT_SPEC ||
      GetSpecsCount() < MIN_TALENT_SPECS) {
    SetActiveSpec(0);
    LOG_ERROR("entities.player",
              "Player %s(GUID: %u) has SpecCount = %u and ActiveSpec = %u.",
              GetName().c_str(), GetGUID().GetCounter(), GetSpecsCount(),
              GetActiveSpec());
  }

  // Only load selected specializations, learning mastery spells requires this
  std::vector<std::string_view> talentTrees =
      Firelands::Tokenize(fields[31].Get<std::string_view>(), ' ', false);

  for (uint8 i = 0; i < MAX_TALENT_SPECS; ++i) {
    if (i >= talentTrees.size())
      break;

    Optional<uint32> talentTree = Firelands::StringTo<uint32>(talentTrees[i]);
    if (sTalentTabStore.LookupEntry(*talentTree))
      SetPrimaryTalentTree(i, *talentTree);
  }

  m_taxi.LoadTaxiMask(
      fields[22].Get<std::string>()); // must be before InitTaxiNodesForLevel

  uint32 extraflags = fields[37].Get<uint16>();

  m_stableSlots = fields[38].Get<uint8>();
  if (m_stableSlots > MAX_PET_STABLES) {
    LOG_ERROR("entities.player",
              "Player::LoadFromDB: Player (%s) can't have more stable slots "
              "than %u, but has %u in DB",
              GetGUID().ToString().c_str(), MAX_PET_STABLES,
              uint32(m_stableSlots));
    m_stableSlots = MAX_PET_STABLES;
  }

  m_atLoginFlags = fields[39].Get<uint16>();

  if (HasAtLoginFlag(AT_LOGIN_RENAME)) {
    LOG_INFO("entities.player.cheat",
             "Player::LoadFromDB: Player (%s) tried to login while forced to "
             "rename, can't load.'",
             GetGUID().ToString().c_str());
    return false;
  }

  // Honor system
  // Update Honor kills data
  m_lastHonorUpdateTime = logoutTime;
  UpdateHonorFields();

  m_deathExpireTime = time_t(fields[42].Get<uint32>());

  if (m_deathExpireTime > now + MAX_DEATH_COUNT * DEATH_EXPIRE_STEP)
    m_deathExpireTime = now + MAX_DEATH_COUNT * DEATH_EXPIRE_STEP - 1;

  // clear channel spell data (if saved at channel spell casting)
  SetChannelObjectGuid(ObjectGuid::Empty);
  SetUInt32Value(UNIT_CHANNEL_SPELL, 0);

  // clear charm/summon related fields
  SetOwnerGUID(ObjectGuid::Empty);
  SetGuidValue(UNIT_FIELD_CHARMEDBY, ObjectGuid::Empty);
  SetGuidValue(UNIT_FIELD_CHARM, ObjectGuid::Empty);
  SetGuidValue(UNIT_FIELD_SUMMON, ObjectGuid::Empty);
  SetGuidValue(PLAYER_FARSIGHT, ObjectGuid::Empty);
  SetCreatorGUID(ObjectGuid::Empty);

  RemoveFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FORCE_MOVEMENT);

  // reset some aura modifiers before aura apply
  SetUInt32Value(PLAYER_TRACK_CREATURES, 0);
  SetUInt32Value(PLAYER_TRACK_RESOURCES, 0);

  // make sure the unit is considered out of combat for proper loading
  ClearInCombat();

  // make sure the unit is considered not in duel for proper loading
  SetGuidValue(PLAYER_DUEL_ARBITER, ObjectGuid::Empty);
  SetUInt32Value(PLAYER_DUEL_TEAM, 0);

  // reset stats before loading any modifiers
  InitStatsForLevel();
  InitGlyphsForLevel();
  InitTaxiNodesForLevel();
  InitRunes();

  // rest bonus can only be calculated after InitStatsForLevel()
  m_rest_bonus = fields[26].Get<float>();

  if (time_diff > 0) {
    // speed collect rest bonus in offline, in logout, far from tavern, city
    // (section/in hour)
    float bubble0 = 0.031f;
    // speed collect rest bonus in offline, in logout, in tavern, city
    // (section/in hour)
    float bubble1 = 0.125f;
    float bubble =
        fields[28].Get<uint8>() > 0
            ? bubble1 * sWorld->getRate(RATE_REST_OFFLINE_IN_TAVERN_OR_CITY)
            : bubble0 * sWorld->getRate(RATE_REST_OFFLINE_IN_WILDERNESS);

    SetRestBonus(GetRestBonus() +
                 time_diff *
                     ((float)GetUInt32Value(PLAYER_NEXT_LEVEL_XP) / 72000) *
                     bubble);
  }

  // load skills after InitStatsForLevel because it triggering aura apply also
  _LoadSkills(holder.GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_SKILLS));
  UpdateSkillsForLevel(); // update skills after load, to make sure they are
                          // correctly update at player load

  // apply original stats mods before spell loading or item equipment that call
  // before equip _RemoveStatsMods()

  // mails are loaded only when needed ;-) - when player in game click on
  // mailbox. _LoadMail();

  UpdateDisplayPower();
  _LoadTalents(holder.GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_TALENTS));
  _LoadSpells(holder.GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_SPELLS));

  _LoadGlyphs(holder.GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_GLYPHS));
  _LoadAuras(holder.GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_AURAS),
             time_diff);
  _LoadGlyphAuras();
  // add ghost flag (must be after aura load: PLAYER_FLAGS_GHOST set in aura)
  if (HasFlag(PLAYER_FLAGS, PLAYER_FLAGS_GHOST))
    m_deathState = DEAD;

  // after spell load, learn rewarded spell if need also
  _LoadQuestStatus(
      holder.GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_QUEST_STATUS));
  _LoadQuestStatusRewarded(
      holder.GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_QUEST_STATUS_REW));
  _LoadDailyQuestStatus(
      holder.GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_DAILY_QUEST_STATUS));
  _LoadWeeklyQuestStatus(
      holder.GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_WEEKLY_QUEST_STATUS));
  _LoadSeasonalQuestStatus(
      holder.GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_SEASONAL_QUEST_STATUS));
  _LoadMonthlyQuestStatus(
      holder.GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_MONTHLY_QUEST_STATUS));
  _LoadLFGRewardStatus(
      holder.GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_LFG_REWARD_STATUS));
  _LoadRandomBGStatus(
      holder.GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_RANDOM_BG));

  // after spell and quest load
  InitTalentForLevel();
  LearnDefaultSkills();
  LearnCustomSpells();

  // must be before inventory (some items required reputation check)
  m_reputationMgr->LoadFromDB(
      holder.GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_REPUTATION));

  _LoadInventory(holder.GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_INVENTORY),
                 time_diff);

  if (IsVoidStorageUnlocked())
    _LoadVoidStorage(
        holder.GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_VOID_STORAGE));

  // update items with duration and realtime
  UpdateItemDuration(time_diff, true);

  _LoadActions(holder.GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_ACTIONS));

  // unread mails and next delivery time, actual mails not loaded
  _LoadMail(holder.GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_MAILS),
            holder.GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_MAIL_ITEMS));

  m_social = sSocialMgr->LoadFromDB(
      holder.GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_SOCIAL_LIST), GetGUID());

  // check PLAYER_CHOSEN_TITLE compatibility with PLAYER__FIELD_KNOWN_TITLES
  // note: PLAYER__FIELD_KNOWN_TITLES updated at quest status loaded
  uint32 curTitle = fields[48].Get<uint32>();
  if (curTitle && !HasTitle(curTitle))
    curTitle = 0;

  SetUInt32Value(PLAYER_CHOSEN_TITLE, curTitle);

  // has to be called after last Relocate() in Player::LoadFromDB
  SetFallInformation(0, GetPositionZ());

  GetSpellHistory()->LoadFromDB<Player>(
      holder.GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_SPELL_COOLDOWNS));

  uint32 savedHealth = fields[51].Get<uint32>();
  if (!savedHealth)
    m_deathState = CORPSE;

  // Spell code allow apply any auras to dead character in load time in
  // aura/spell/item loading Do now before stats re-calculation cleanup for
  // ghost state unexpected auras
  if (!IsAlive())
    RemoveAllAurasOnDeath();
  else
    RemoveAllAurasRequiringDeadTarget();

  // apply all stat bonuses from items and auras
  SetCanModifyStats(true);
  UpdateAllStats();

  // restore remembered power/health values (but not more max values)
  SetHealth(savedHealth);
  uint32 loadedPowers = 0;
  for (uint32 i = 0; i < MAX_POWERS; ++i) {
    if (GetPowerIndex(Powers(i)) != MAX_POWERS) {
      uint32 savedPower = fields[52 + loadedPowers].Get<uint32>();
      uint32 maxPower = GetUInt32Value(UNIT_FIELD_MAXPOWER1 + loadedPowers);
      SetPower(Powers(i), (savedPower > maxPower) ? maxPower : savedPower);
      if (++loadedPowers >= MAX_POWERS_PER_CLASS)
        break;
    }
  }

  for (; loadedPowers < MAX_POWERS_PER_CLASS; ++loadedPowers)
    SetUInt32Value(UNIT_FIELD_POWER1 + loadedPowers, 0);

  SetPower(POWER_ECLIPSE, 0);

  // Verify loaded talent specializations
  for (uint8 i = 0; i < MAX_TALENT_SPECS; ++i) {
    if (i >= talentTrees.size()) {
      break;
    }

    Optional<uint32> talentTree = Firelands::StringTo<uint32>(talentTrees[i]);
    if (*talentTree != 0 && !sTalentTabStore.LookupEntry(*talentTree) &&
        i == GetActiveSpec()) {
      SetAtLoginFlag(AT_LOGIN_RESET_TALENTS); // invalid tree, reset talents
    }
  }

  // We neeed to update the armor specialization after the talenttree was set
  UpdateArmorSpecialization();

  LOG_DEBUG(
      "entities.player.loading",
      "The value of player %s after load item and aura is: ", m_name.c_str());
  outDebugValues();

  // GM state
  if (GetSession()->HasPermission(rbac::RBAC_PERM_RESTORE_SAVED_GM_STATE)) {
    switch (sWorld->getIntConfig(CONFIG_GM_LOGIN_STATE)) {
    default:
    case 0:
      break; // disable
    case 1:
      SetGameMaster(true);
      break; // enable
    case 2:  // save state
      if (extraflags & PLAYER_EXTRA_GM_ON)
        SetGameMaster(true);
      break;
    }

    switch (sWorld->getIntConfig(CONFIG_GM_VISIBLE_STATE)) {
    default:
    case 0:
      SetGMVisible(false);
      break; // invisible
    case 1:
      break; // visible
    case 2:  // save state
      if (extraflags & PLAYER_EXTRA_GM_INVISIBLE)
        SetGMVisible(false);
      break;
    }

    switch (sWorld->getIntConfig(CONFIG_GM_CHAT)) {
    default:
    case 0:
      break; // disable
    case 1:
      SetGMChat(true);
      break; // enable
    case 2:  // save state
      if (extraflags & PLAYER_EXTRA_GM_CHAT)
        SetGMChat(true);
      break;
    }

    switch (sWorld->getIntConfig(CONFIG_GM_WHISPERING_TO)) {
    default:
    case 0:
      break; // disable
    case 1:
      SetAcceptWhispers(true);
      break; // enable
    case 2:  // save state
      if (extraflags & PLAYER_EXTRA_ACCEPT_WHISPERS)
        SetAcceptWhispers(true);
      break;
    }
  }

  // RaF stuff.
  m_grantableLevels = fields[64].Get<uint8>();
  if (GetSession()->IsARecruiter() || (GetSession()->GetRecruiterId() != 0))
    SetFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_REFER_A_FRIEND);

  if (m_grantableLevels > 0)
    SetByteValue(PLAYER_FIELD_BYTES,
                 PLAYER_FIELD_BYTES_OFFSET_RAF_GRANTABLE_LEVEL, 0x01);

  _LoadDeclinedNames(
      holder.GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_DECLINED_NAMES));

  m_achievementMgr->CheckAllAchievementCriteria(this);

  _LoadEquipmentSets(
      holder.GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_EQUIPMENT_SETS));

  _LoadCUFProfiles(
      holder.GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_CUF_PROFILES));

  return true;
}

void Player::_LoadCUFProfiles(PreparedQueryResult result) {
  if (!result)
    return;

  do {
    // SELECT id, name, frameHeight, frameWidth, sortBy, healthText,
    // boolOptions, unk146, unk147, unk148, unk150, unk152, unk154 FROM
    // character_cuf_profiles WHERE guid = ?
    Field *fields = result->Fetch();

    uint8 id = fields[0].Get<uint8>();
    std::string name = fields[1].Get<std::string>();
    uint16 frameHeight = fields[2].Get<uint16>();
    uint16 frameWidth = fields[3].Get<uint16>();
    uint8 sortBy = fields[4].Get<uint8>();
    uint8 healthText = fields[5].Get<uint8>();
    uint32 boolOptions = fields[6].Get<uint32>();
    uint8 topPoint = fields[7].Get<uint8>();
    uint8 bottomPoint = fields[8].Get<uint8>();
    uint8 leftPoint = fields[9].Get<uint8>();
    uint16 topOffset = fields[10].Get<uint16>();
    uint16 bottomOffset = fields[11].Get<uint16>();
    uint16 leftOffset = fields[12].Get<uint16>();

    if (id > MAX_CUF_PROFILES) {
      LOG_ERROR("entities.player",
                "Player::_LoadCUFProfiles - Player (GUID: %u, name: %s) has an "
                "CUF profile with invalid id (id: %u), max is %i.",
                GetGUID().GetCounter(), GetName().c_str(), id,
                MAX_CUF_PROFILES);
      continue;
    }

    _CUFProfiles[id] = Firelands::make_unique<CUFProfile>(
        name, frameHeight, frameWidth, sortBy, healthText, boolOptions,
        topPoint, bottomPoint, leftPoint, topOffset, bottomOffset, leftOffset);
  } while (result->NextRow());
}

bool Player::isAllowedToLoot(Creature const *creature) {
  if (!creature->isDead() || !creature->IsDamageEnoughForLootingAndReward())
    return false;

  if (HasPendingBind())
    return false;

  Loot const *loot = &creature->loot;
  if (loot->isLooted()) // nothing to loot or everything looted.
    return false;
  if (!loot->hasItemForAll() &&
      !loot->hasItemFor(this)) // no loot in creature for this player
    return false;

  if (loot->loot_type == LOOT_SKINNING)
    return creature->GetLootRecipientGUID() == GetGUID();

  Group *thisGroup = GetGroup();
  if (!thisGroup)
    return this == creature->GetLootRecipient();
  else if (thisGroup != creature->GetLootRecipientGroup())
    return false;

  switch (thisGroup->GetLootMethod()) {
  case MASTER_LOOT:
  case FREE_FOR_ALL:
    return true;
  case ROUND_ROBIN:
    // may only loot if the player is the loot roundrobin player
    // or if there are free/quest/conditional item for the player
    if (loot->roundRobinPlayer.IsEmpty() || loot->roundRobinPlayer == GetGUID())
      return true;

    return loot->hasItemFor(this);
  case GROUP_LOOT:
  case NEED_BEFORE_GREED:
    // may only loot if the player is the loot roundrobin player
    // or item over threshold (so roll(s) can be launched)
    // or if there are free/quest/conditional item for the player
    if (loot->roundRobinPlayer.IsEmpty() || loot->roundRobinPlayer == GetGUID())
      return true;

    if (loot->hasOverThresholdItem())
      return true;

    return loot->hasItemFor(this);
  }

  return false;
}

void Player::_LoadActions(PreparedQueryResult result) {
  m_actionButtons.clear();

  if (result) {
    do {
      Field *fields = result->Fetch();
      uint8 button = fields[0].Get<uint8>();
      uint32 action = fields[1].Get<uint32>();
      uint8 type = fields[2].Get<uint8>();

      if (ActionButton *ab = addActionButton(button, action, type))
        ab->uState = ACTIONBUTTON_UNCHANGED;
      else {
        LOG_DEBUG(
            "entities.player",
            "Player::_LoadActions: Player '%s' (%s) has an invalid action "
            "button (Button: %u, Action: %u, Type: %u). It will be deleted at "
            "next save. This can be due to a player changing their talents.",
            GetName().c_str(), GetGUID().ToString().c_str(), button, action,
            type);

        // Will be deleted in DB at next save (it can create data until save but
        // marked as deleted).
        m_actionButtons[button].uState = ACTIONBUTTON_DELETED;
      }
    } while (result->NextRow());
  }
}

void Player::_LoadAuras(PreparedQueryResult result, uint32 timediff) {
  LOG_DEBUG("entities.player.loading",
            "Player::_LoadAuras: Loading auras for %s",
            GetGUID().ToString().c_str());

  /*                                                           0       1 2 3 4
  5      6       7         8              9            10 QueryResult* result =
  CharacterDatabase.PQuery("SELECT casterGuid, spell, effectMask,
  recalculateMask, stackCount, amount0, amount1, amount2, base_amount0,
  base_amount1, base_amount2, 11          12          13 maxDuration,
  remainTime, remainCharges FROM character_aura WHERE guid = '%u'",
  GetGUID().GetCounter());
  */

  if (result) {
    do {
      Field *fields = result->Fetch();
      int32 damage[3];
      int32 baseDamage[3];
      ObjectGuid caster_guid(fields[0].Get<uint64>());
      uint32 spellid = fields[1].Get<uint32>();
      uint8 effmask = fields[2].Get<uint8>();
      uint8 recalculatemask = fields[3].Get<uint8>();
      uint8 stackcount = fields[4].Get<uint8>();
      damage[0] = fields[5].Get<int32>();
      damage[1] = fields[6].Get<int32>();
      damage[2] = fields[7].Get<int32>();
      baseDamage[0] = fields[8].Get<int32>();
      baseDamage[1] = fields[9].Get<int32>();
      baseDamage[2] = fields[10].Get<int32>();
      int32 maxduration = fields[11].Get<int32>();
      int32 remaintime = fields[12].Get<int32>();
      uint8 remaincharges = fields[13].Get<uint8>();

      SpellInfo const *spellInfo = sSpellMgr->GetSpellInfo(spellid);
      if (!spellInfo) {
        LOG_ERROR("entities.player",
                  "Player::_LoadAuras: Player '%s' (%s) has an invalid aura "
                  "(SpellID: %u), ignoring.",
                  GetName().c_str(), GetGUID().ToString().c_str(), spellid);
        continue;
      }

      // negative effects should continue counting down after logout
      if (remaintime != -1 &&
          ((!spellInfo->IsPositive() && spellInfo->Id != 15007) ||
           spellInfo->HasAttribute(
               SPELL_ATTR4_FADES_WHILE_LOGGED_OUT))) // Resurrection sickness
                                                     // should not fade while
                                                     // logged out
      {
        if (remaintime / IN_MILLISECONDS <= int32(timediff))
          continue;

        remaintime -= timediff * IN_MILLISECONDS;
      }

      // prevent wrong values of remaincharges
      if (spellInfo->ProcCharges) {
        // we have no control over the order of applying auras and modifiers
        // allow auras to have more charges than value in SpellInfo
        if (remaincharges <= 0 /* || remaincharges > spellproto->procCharges*/)
          remaincharges = spellInfo->ProcCharges;
      } else
        remaincharges = 0;

      if (Aura *aura = Aura::TryCreate(spellInfo, effmask, this, nullptr,
                                       &baseDamage[0], nullptr, caster_guid)) {
        if (!aura->CanBeSaved()) {
          aura->Remove();
          continue;
        }

        aura->SetLoadedState(maxduration, remaintime, remaincharges, stackcount,
                             recalculatemask, &damage[0]);
        aura->ApplyForTargets();
        LOG_DEBUG("entities.player",
                  "Player::_LoadAuras: Added aura (SpellID: %u, EffectMask: "
                  "%u) to player '%s (%s)",
                  spellInfo->Id, effmask, GetName().c_str(),
                  GetGUID().ToString().c_str());
      }
    } while (result->NextRow());
  }
}

void Player::_LoadGlyphAuras() {
  for (uint8 i = 0; i < MAX_GLYPH_SLOT_INDEX; ++i) {
    if (uint32 glyph = GetGlyph(GetActiveSpec(), i)) {
      if (GlyphPropertiesEntry const *gp =
              sGlyphPropertiesStore.LookupEntry(glyph)) {
        if (GlyphSlotEntry const *gs =
                sGlyphSlotStore.LookupEntry(GetGlyphSlot(i))) {
          if (gp->GlyphSlotFlags == gs->Type) {
            CastSpell(this, gp->SpellID, true);
            continue;
          } else
            LOG_ERROR("entities.player",
                      "Player::_LoadGlyphAuras: Player '%s' (%s) has glyph "
                      "with typeflags %u in slot with typeflags %u, removing.",
                      GetName().c_str(), GetGUID().ToString().c_str(),
                      gp->GlyphSlotFlags, gs->Type);
        } else
          LOG_ERROR("entities.player",
                    "Player::_LoadGlyphAuras: Player '%s' (%s) has not "
                    "existing glyph slot entry %u on index %u",
                    GetName().c_str(), GetGUID().ToString().c_str(),
                    GetGlyphSlot(i), i);
      } else
        LOG_ERROR("entities.player",
                  "Player::_LoadGlyphAuras: Player '%s' (%s) has not existing "
                  "glyph entry %u on index %u",
                  GetName().c_str(), GetGUID().ToString().c_str(), glyph, i);

      // On any error remove glyph
      SetGlyph(i, 0);
    }
  }
}

void Player::LoadCorpse(PreparedQueryResult result) {
  if (IsAlive() || HasAtLoginFlag(AT_LOGIN_RESURRECT))
    SpawnCorpseBones(false);

  if (!IsAlive()) {
    if (HasAtLoginFlag(AT_LOGIN_RESURRECT))
      ResurrectPlayer(0.5f);
    else if (result) {
      Field *fields = result->Fetch();
      _corpseLocation.WorldRelocate(
          fields[0].Get<uint16>(), fields[1].Get<float>(),
          fields[2].Get<float>(), fields[3].Get<float>(),
          fields[4].Get<float>());
      ApplyModByteFlag(
          PLAYER_FIELD_BYTES, PLAYER_FIELD_BYTES_OFFSET_FLAGS,
          PLAYER_FIELD_BYTE_RELEASE_TIMER,
          !sMapStore.LookupEntry(_corpseLocation.GetMapId())->Instanceable());
    }
  }

  RemoveAtLoginFlag(AT_LOGIN_RESURRECT);
}

void Player::_LoadInventory(PreparedQueryResult result, uint32 timeDiff) {
  // QueryResult* result = CharacterDatabase.PQuery("SELECT data, text, bag,
  // slot, item, item_template FROM character_inventory JOIN item_instance ON
  // character_inventory.item = item_instance.guid WHERE
  // character_inventory.guid = '%u' ORDER BY bag, slot",
  // GetGUID().GetCounter()); NOTE: the "order by `bag`" is important because it
  // makes sure the bagMap is filled before items in the bags are loaded NOTE2:
  // the "order by `slot`" is needed because mainhand weapons are (wrongly?)
  // expected to be equipped before offhand items (@todo fixme)

  if (result) {
    uint32 zoneId = GetZoneId();

    std::map<ObjectGuid::LowType, Bag *> bagMap; // fast guid lookup for bags
    std::map<ObjectGuid::LowType, Item *>
        invalidBagMap; // fast guid lookup for bags
    std::list<Item *> problematicItems;
    CharacterDatabaseTransaction trans = CharacterDatabase.BeginTransaction();

    // Prevent items from being added to the queue while loading
    m_itemUpdateQueueBlocked = true;
    do {
      Field *fields = result->Fetch();
      if (Item *item = _LoadItem(trans, zoneId, timeDiff, fields)) {
        ObjectGuid::LowType bagGuid = fields[12].Get<uint32>();
        uint8 slot = fields[13].Get<uint8>();

        InventoryResult err = EQUIP_ERR_OK;
        // Item is not in bag
        if (!bagGuid) {
          item->SetContainer(nullptr);
          item->SetSlot(slot);

          if (IsInventoryPos(INVENTORY_SLOT_BAG_0, slot)) {
            ItemPosCountVec dest;
            err = CanStoreItem(INVENTORY_SLOT_BAG_0, slot, dest, item, false);
            if (err == EQUIP_ERR_OK)
              item = StoreItem(dest, item, true);
          } else if (IsEquipmentPos(INVENTORY_SLOT_BAG_0, slot)) {
            uint16 dest;
            err = CanEquipItem(slot, dest, item, false, false);
            if (err == EQUIP_ERR_OK)
              QuickEquipItem(dest, item);
          } else if (IsBankPos(INVENTORY_SLOT_BAG_0, slot)) {
            ItemPosCountVec dest;
            err = CanBankItem(INVENTORY_SLOT_BAG_0, slot, dest, item, false,
                              false);
            if (err == EQUIP_ERR_OK)
              item = BankItem(dest, item, true);
          }

          // Remember bags that may contain items in them
          if (err == EQUIP_ERR_OK) {
            if (IsBagPos(item->GetPos()))
              if (Bag *pBag = item->ToBag())
                bagMap[item->GetGUID().GetCounter()] = pBag;
          } else if (IsBagPos(item->GetPos()))
            if (item->IsBag())
              invalidBagMap[item->GetGUID().GetCounter()] = item;
        } else {
          item->SetSlot(NULL_SLOT);
          // Item is in the bag, find the bag
          std::map<ObjectGuid::LowType, Bag *>::iterator itr =
              bagMap.find(bagGuid);
          if (itr != bagMap.end()) {
            ItemPosCountVec dest;
            err = CanStoreItem(itr->second->GetSlot(), slot, dest, item);
            if (err == EQUIP_ERR_OK)
              item = StoreItem(dest, item, true);
          } else if (invalidBagMap.find(bagGuid) != invalidBagMap.end()) {
            std::map<ObjectGuid::LowType, Item *>::iterator invalidBagItr =
                invalidBagMap.find(bagGuid);
            if (std::find(problematicItems.begin(), problematicItems.end(),
                          invalidBagItr->second) != problematicItems.end())
              err = EQUIP_ERR_INTERNAL_BAG_ERROR;
          } else {
            LOG_ERROR("entities.player",
                      "Player::_LoadInventory: Player '%s' (%s) has item (%s, "
                      "entry: %u) which doesnt have a valid bag (Bag %u, slot: "
                      "%u). Possible cheat?",
                      GetName().c_str(), GetGUID().ToString().c_str(),
                      item->GetGUID().ToString().c_str(), item->GetEntry(),
                      bagGuid, slot);
            item->DeleteFromInventoryDB(trans);
            delete item;
            continue;
          }
        }

        // Item's state may have changed after storing
        if (err == EQUIP_ERR_OK)
          item->SetState(ITEM_UNCHANGED, this);
        else {
          LOG_ERROR("entities.player",
                    "Player::_LoadInventory: Player '%s' (%s) has item (%s, "
                    "entry: %u) which can't be loaded into inventory (Bag %u, "
                    "slot: %u) by reason %u. Item will be sent by mail.",
                    GetName().c_str(), GetGUID().ToString().c_str(),
                    item->GetGUID().ToString().c_str(), item->GetEntry(),
                    bagGuid, slot, uint32(err));
          item->DeleteFromInventoryDB(trans);
          problematicItems.push_back(item);
        }
      }
    } while (result->NextRow());

    m_itemUpdateQueueBlocked = false;

    // Send problematic items by mail
    while (!problematicItems.empty()) {
      std::string subject =
          GetSession()->GetFirelandsString(LANG_NOT_EQUIPPED_ITEM);

      MailDraft draft(subject, "There were problems with equipping item(s).");
      for (uint8 i = 0; !problematicItems.empty() && i < MAX_MAIL_ITEMS; ++i) {
        draft.AddItem(problematicItems.front());
        problematicItems.pop_front();
      }
      draft.SendMailTo(trans, this, MailSender(this, MAIL_STATIONERY_GM),
                       MAIL_CHECK_MASK_COPIED);
    }
    CharacterDatabase.CommitTransaction(trans);
  }
  // if (IsAlive())
  _ApplyAllItemMods();
}

void Player::_LoadVoidStorage(PreparedQueryResult result) {
  if (!result)
    return;

  do {
    // SELECT itemid, itemEntry, slot,  creatorGuid FROM character_void_storage
    // WHERE playerGuid = ?
    Field *fields = result->Fetch();

    uint64 itemId = fields[0].Get<uint64>();
    uint32 itemEntry = fields[1].Get<uint32>();
    uint8 slot = fields[2].Get<uint8>();
    ObjectGuid creatorGuid(HighGuid::Player, fields[3].Get<uint32>());
    ItemRandomEnchantmentId randomProperty(
        ItemRandomEnchantmentType(fields[4].Get<uint8>()),
        fields[5].Get<uint32>());
    uint32 suffixFactor = fields[6].Get<uint32>();

    if (!itemId) {
      LOG_ERROR("entities.player",
                "Player::_LoadVoidStorage - Player (GUID: %u, name: %s) has an "
                "item with an invalid id (item id: " UI64FMTD ", entry: %u).",
                GetGUID().GetCounter(), GetName().c_str(), itemId, itemEntry);
      continue;
    }

    if (!sObjectMgr->GetItemTemplate(itemEntry)) {
      LOG_ERROR("entities.player",
                "Player::_LoadVoidStorage - Player (GUID: %u, name: %s) has an "
                "item with an invalid entry (item id: " UI64FMTD
                ", entry: %u).",
                GetGUID().GetCounter(), GetName().c_str(), itemId, itemEntry);
      continue;
    }

    if (slot >= VOID_STORAGE_MAX_SLOT) {
      LOG_ERROR(
          "entities.player",
          "Player::_LoadVoidStorage - Player (GUID: %u, name: %s) has an item "
          "with an invalid slot (item id: " UI64FMTD ", entry: %u, slot: %u).",
          GetGUID().GetCounter(), GetName().c_str(), itemId, itemEntry, slot);
      continue;
    }

    std::string name;
    if (creatorGuid &&
        !sCharacterCache->GetCharacterNameByGuid(creatorGuid, name)) {
      LOG_ERROR(
          "entities.player",
          "Player::_LoadVoidStorage - Player (GUID: %u, name: %s) has an item "
          "with an invalid creator guid, set to 0 (item id: " UI64FMTD
          ", entry: %u, creator: %s).",
          GetGUID().GetCounter(), GetName().c_str(), itemId, itemEntry,
          creatorGuid.ToString().c_str());
      creatorGuid.Clear();
    }

    _voidStorageItems[slot] = new VoidStorageItem(
        itemId, itemEntry, creatorGuid, randomProperty, suffixFactor);
  } while (result->NextRow());
}

Item *Player::_LoadItem(CharacterDatabaseTransaction &trans, uint32 zoneId,
                        uint32 timeDiff, Field *fields) {
  Item *item = nullptr;
  ObjectGuid::LowType itemGuid = fields[14].Get<uint32>();
  uint32 itemEntry = fields[15].Get<uint32>();
  if (ItemTemplate const *proto = sObjectMgr->GetItemTemplate(itemEntry)) {
    bool remove = false;
    item = NewItemOrBag(proto);
    if (item->LoadFromDB(itemGuid, GetGUID(), fields, itemEntry)) {
      CharacterDatabasePreparedStatement *stmt;

      // Do not allow to have item limited to another map/zone in alive state
      if (IsAlive() && item->IsLimitedToAnotherMapOrZone(GetMapId(), zoneId)) {
        LOG_DEBUG("entities.player.loading",
                  "Player::_LoadInventory: player (GUID: %u, name: '%s', map: "
                  "%u) has item (GUID: %u, entry: %u) limited to another map "
                  "(%u). Deleting item.",
                  GetGUID().GetCounter(), GetName().c_str(), GetMapId(),
                  item->GetGUID().GetCounter(), item->GetEntry(), zoneId);
        remove = true;
      }
      // "Conjured items disappear if you are logged out for more than 15
      // minutes"
      else if (timeDiff > 15 * MINUTE &&
               proto->GetFlags() & ITEM_FLAG_CONJURED) {
        LOG_DEBUG("entities.player.loading",
                  "Player::_LoadInventory: player (GUID: %u, name: '%s', diff: "
                  "%u) has conjured item (GUID: %u, entry: %u) with expired "
                  "lifetime (15 minutes). Deleting item.",
                  GetGUID().GetCounter(), GetName().c_str(), timeDiff,
                  item->GetGUID().GetCounter(), item->GetEntry());
        remove = true;
      } else if (item->HasFlag(ITEM_FIELD_FLAGS, ITEM_FIELD_FLAG_REFUNDABLE)) {
        if (item->GetPlayedTime(this) > (2 * HOUR)) {
          LOG_DEBUG("entities.player.loading",
                    "Player::_LoadInventory: player (GUID: %u, name: '%s') has "
                    "item (GUID: %u, entry: %u) with expired refund time (%u). "
                    "Deleting refund data and removing refundable flag.",
                    GetGUID().GetCounter(), GetName().c_str(),
                    item->GetGUID().GetCounter(), item->GetEntry(),
                    item->GetPlayedTime(this));

          stmt = CharacterDatabase.GetPreparedStatement(
              CHAR_DEL_ITEM_REFUND_INSTANCE);
          stmt->SetData(0, item->GetGUID().GetCounter());
          trans->Append(stmt);

          item->RemoveFlag(ITEM_FIELD_FLAGS, ITEM_FIELD_FLAG_REFUNDABLE);
        } else {
          stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_ITEM_REFUNDS);
          stmt->SetData(0, item->GetGUID().GetCounter());
          stmt->SetData(1, GetGUID().GetCounter());
          if (PreparedQueryResult result = CharacterDatabase.Query(stmt)) {
            item->SetRefundRecipient((*result)[0].Get<uint32>());
            item->SetPaidMoney((*result)[1].Get<uint32>());
            item->SetPaidExtendedCost((*result)[2].Get<uint16>());
            AddRefundReference(item->GetGUID());
          } else {
            LOG_DEBUG(
                "entities.player.loading",
                "Player::_LoadInventory: player (GUID: %u, name: '%s') has "
                "item (GUID: %u, entry: %u) with refundable flags, but without "
                "data in item_refund_instance. Removing flag.",
                GetGUID().GetCounter(), GetName().c_str(),
                item->GetGUID().GetCounter(), item->GetEntry());
            item->RemoveFlag(ITEM_FIELD_FLAGS, ITEM_FIELD_FLAG_REFUNDABLE);
          }
        }
      } else if (item->HasFlag(ITEM_FIELD_FLAGS,
                               ITEM_FIELD_FLAG_BOP_TRADEABLE)) {
        stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_ITEM_BOP_TRADE);
        stmt->SetData(0, item->GetGUID().GetCounter());
        if (PreparedQueryResult result = CharacterDatabase.Query(stmt)) {
          std::string_view strGUID = (*result)[0].Get<std::string_view>();
          std::vector<std::string_view> GUIDlist =
              Firelands::Tokenize(strGUID, ' ', false);
          GuidSet looters;

          for (int itr = 0; itr < ((int)GUIDlist.size()); ++itr) {
            looters.insert(ObjectGuid::Create<HighGuid::Player>(
                *Firelands::StringTo<uint32>(GUIDlist[itr])));
          }

          if (looters.size() > 1 &&
              item->GetTemplate()->GetMaxStackSize() == 1 &&
              item->IsSoulBound()) {
            item->SetSoulboundTradeable(looters);
            AddTradeableItem(item);
          } else
            item->ClearSoulboundTradeable(this);
        } else {
          LOG_DEBUG(
              "entities.player.loading",
              "Player::_LoadInventory: player (GUID: %u, name: '%s') has item "
              "(GUID: %u, entry: %u) with ITEM_FIELD_FLAG_BOP_TRADEABLE flag, "
              "but without data in item_soulbound_trade_data. Removing flag.",
              GetGUID().GetCounter(), GetName().c_str(),
              item->GetGUID().GetCounter(), item->GetEntry());
          item->RemoveFlag(ITEM_FIELD_FLAGS, ITEM_FIELD_FLAG_BOP_TRADEABLE);
        }
      } else if (proto->GetHolidayID()) {
        remove = true;
        GameEventMgr::GameEventDataMap const &events =
            sGameEventMgr->GetEventMap();
        GameEventMgr::ActiveEvents const &activeEventsList =
            sGameEventMgr->GetActiveEventList();
        for (GameEventMgr::ActiveEvents::const_iterator itr =
                 activeEventsList.begin();
             itr != activeEventsList.end(); ++itr) {
          if (uint32(events[*itr].holiday_id) == proto->GetHolidayID()) {
            remove = false;
            break;
          }
        }
      }
    } else {
      LOG_ERROR(
          "entities.player",
          "Player::_LoadInventory: player (GUID: %u, name: '%s') has a broken "
          "item (GUID: %u, entry: %u) in inventory. Deleting item.",
          GetGUID().GetCounter(), GetName().c_str(), itemGuid, itemEntry);
      remove = true;
    }
    // Remove item from inventory if necessary
    if (remove) {
      Item::DeleteFromInventoryDB(trans, itemGuid);
      item->FSetState(ITEM_REMOVED);
      item->SaveToDB(trans); // it also deletes item object!
      item = nullptr;
    }
  } else {
    LOG_ERROR("entities.player",
              "Player::_LoadInventory: player (GUID: %u, name: '%s') has an "
              "unknown item (entry: %u) in inventory. Deleting item.",
              GetGUID().GetCounter(), GetName().c_str(), itemEntry);
    Item::DeleteFromInventoryDB(trans, itemGuid);
    Item::DeleteFromDB(trans, itemGuid);
  }
  return item;
}

// load mailed item which should receive current player
Item *Player::_LoadMailedItem(ObjectGuid const &playerGuid, Player *player,
                              uint32 mailId, Mail *mail, Field *fields) {
  ObjectGuid::LowType itemGuid = fields[12].Get<uint32>();
  uint32 itemEntry = fields[13].Get<uint32>();

  ItemTemplate const *proto = sObjectMgr->GetItemTemplate(itemEntry);
  if (!proto) {
    LOG_ERROR("entities.player",
              "Player '%s' (%s) has unknown item in mailed items (GUID: %u, "
              "Entry: %u) in mail (%u), deleted.",
              player ? player->GetName().c_str() : "<unknown>",
              playerGuid.ToString().c_str(), itemGuid, itemEntry, mailId);

    CharacterDatabaseTransaction trans = CharacterDatabase.BeginTransaction();

    CharacterDatabasePreparedStatement *stmt =
        CharacterDatabase.GetPreparedStatement(CHAR_DEL_INVALID_MAIL_ITEM);
    stmt->SetData(0, itemGuid);
    trans->Append(stmt);

    Item::DeleteFromDB(trans, itemGuid);

    CharacterDatabase.CommitTransaction(trans);
    return nullptr;
  }

  Item *item = NewItemOrBag(proto);

  ObjectGuid ownerGuid =
      fields[14].Get<uint32>()
          ? ObjectGuid::Create<HighGuid::Player>(fields[14].Get<uint32>())
          : ObjectGuid::Empty;
  if (!item->LoadFromDB(itemGuid, ownerGuid, fields, itemEntry)) {
    LOG_ERROR("entities.player",
              "Player::_LoadMailedItems: Item (GUID: %u) in mail (%u) doesn't "
              "exist, deleted from mail.",
              itemGuid, mailId);

    CharacterDatabasePreparedStatement *stmt =
        CharacterDatabase.GetPreparedStatement(CHAR_DEL_MAIL_ITEM);
    stmt->SetData(0, itemGuid);
    CharacterDatabase.Execute(stmt);

    item->FSetState(ITEM_REMOVED);

    CharacterDatabaseTransaction temp = CharacterDatabaseTransaction(nullptr);
    item->SaveToDB(temp); // it also deletes item object !
    return nullptr;
  }

  if (mail)
    mail->AddItem(itemGuid, itemEntry);

  if (player)
    player->AddMItem(item);

  return item;
}

void Player::_LoadMail(PreparedQueryResult mailsResult,
                       PreparedQueryResult mailItemsResult) {
  m_mail.clear();

  std::unordered_map<uint32, Mail *> mailById;

  if (mailsResult) {
    do {
      Field *fields = mailsResult->Fetch();
      Mail *m = new Mail;

      m->messageID = fields[0].Get<uint32>();
      m->messageType = fields[1].Get<uint8>();
      m->sender = fields[2].Get<uint32>();
      m->receiver = fields[3].Get<uint32>();
      m->subject = fields[4].Get<std::string>();
      m->body = fields[5].Get<std::string>();
      m->expire_time = time_t(fields[6].Get<uint32>());
      m->deliver_time = time_t(fields[7].Get<uint32>());
      m->money = fields[8].Get<uint64>();
      m->COD = fields[9].Get<uint64>();
      m->checked = fields[10].Get<uint8>();
      m->stationery = fields[11].Get<uint8>();
      m->mailTemplateId = fields[12].Get<int16>();

      if (m->mailTemplateId &&
          !sMailTemplateStore.LookupEntry(m->mailTemplateId)) {
        LOG_ERROR("entities.player",
                  "Player::_LoadMail: Mail (%u) has nonexistent MailTemplateId "
                  "(%u), remove at load",
                  m->messageID, m->mailTemplateId);
        m->mailTemplateId = 0;
      }

      m->state = MAIL_STATE_UNCHANGED;
      m_mail.push_back(m);
      mailById[m->messageID] = m;
    } while (mailsResult->NextRow());
  }

  if (mailItemsResult) {
    do {
      Field *fields = mailItemsResult->Fetch();
      uint32 mailId = fields[15].Get<uint32>();
      _LoadMailedItem(GetGUID(), this, mailId, mailById[mailId], fields);
    } while (mailItemsResult->NextRow());
  }

  UpdateNextMailTimeAndUnreads();
}

void Player::LoadPet() {
  // fixme: the pet should still be loaded if the player is not in world
  //  just not added to the map
  if (IsInWorld()) {
    Pet *pet = new Pet(this);
    if (!pet->LoadPetData(this, 0, 0, true))
      delete pet;
  }
}

void Player::LoadPetsFromDB(PreparedQueryResult result) {
  if (!result)
    return;

  do {
    Field *fields = result->Fetch();

    PlayerPetData *playerPetData = new PlayerPetData();

    uint8 slot = fields[7].Get<uint8>();
    uint32 petId = fields[0].Get<uint32>();

    if (slot > PET_SLOT_LAST) {
      LOG_ERROR("sql.sql", "Player::LoadPetsFromDB: bad slot %u for pet %u!",
                slot, petId);
      continue;
    }

    playerPetData->PetId = petId;
    playerPetData->CreatureId = fields[1].Get<uint32>();
    playerPetData->Owner = fields[2].Get<uint64>();
    playerPetData->DisplayId = fields[3].Get<uint32>();
    playerPetData->Petlevel = fields[4].Get<uint16>();
    playerPetData->PetExp = fields[5].Get<uint32>();
    playerPetData->Reactstate = ReactStates(fields[6].Get<uint8>());
    playerPetData->Slot = slot;
    playerPetData->Name = fields[8].Get<std::string>();
    playerPetData->Renamed = fields[9].Get<bool>();
    playerPetData->Active = fields[10].Get<bool>();
    playerPetData->SavedHealth = fields[11].Get<uint32>();
    playerPetData->SavedMana = fields[12].Get<uint32>();
    playerPetData->Actionbar = fields[13].Get<std::string>();
    playerPetData->Timediff = fields[14].Get<uint32>();
    playerPetData->SummonSpellId = fields[15].Get<uint32>();
    playerPetData->Type = PetType(fields[16].Get<uint8>());

    PlayerPetDataStore.push_back(playerPetData);

  } while (result->NextRow());
}

PlayerPetData *Player::GetPlayerPetDataById(uint32 petId) {
  for (PlayerPetData *p : PlayerPetDataStore)
    if (p->PetId == petId)
      return p;

  return nullptr;
}

PlayerPetData *Player::GetPlayerPetDataBySlot(uint8 slot) {
  for (PlayerPetData *p : PlayerPetDataStore)
    if (p->Slot == slot)
      return p;

  return nullptr;
}

PlayerPetData *Player::GetPlayerPetDataByCreatureId(uint32 creatureId) {
  for (PlayerPetData *p : PlayerPetDataStore)
    if (p->CreatureId == creatureId)
      return p;

  return nullptr;
}

PlayerPetData *Player::GetPlayerPetDataCurrent() {
  for (PlayerPetData *p : PlayerPetDataStore)
    if (p->Active == true)
      return p;

  return nullptr;
}

Optional<uint8> Player::GetFirstUnusedActivePetSlot() {
  std::set<uint8> unusedActiveSlot = {0, 1, 2, 3, 4}; // unfiltered

  for (PlayerPetData *p : PlayerPetDataStore)
    if (unusedActiveSlot.find(p->Slot) != unusedActiveSlot.end())
      unusedActiveSlot.erase(p->Slot);

  if (!unusedActiveSlot.empty())
    return *unusedActiveSlot.begin();

  return Optional<uint8>{};
}

Optional<uint8> Player::GetFirstUnusedPetSlot() {
  std::set<uint8> unusedSlot;

  for (uint8 i = 0; i < PET_SLOT_LAST;
       i++) // 4 MAX StableSlots (256 theoretically)
    unusedSlot.insert(i);

  for (PlayerPetData *p : PlayerPetDataStore)
    if (unusedSlot.find(p->Slot) != unusedSlot.end())
      unusedSlot.erase(p->Slot);

  if (!unusedSlot.empty())
    return *unusedSlot.begin();

  return Optional<uint8>{};
}

void Player::DeleteFromPlayerPetDataStore(uint32 petNumber) {
  for (uint8 i = 0; i < PlayerPetDataStore.size(); ++i) {
    if (PlayerPetDataStore[i]->PetId == petNumber) {
      delete PlayerPetDataStore[i];
      PlayerPetDataStore.erase(PlayerPetDataStore.begin() + (i--));
    }
  }
}

void Player::AddToPlayerPetDataStore(PlayerPetData *playerPetData) {
  PlayerPetDataStore.push_back(playerPetData);
}

void Player::_LoadQuestStatus(PreparedQueryResult result) {
  uint16 slot = 0;

  ////                                                       0      1       2 3
  /// 4           5          6         7           8           9           10
  // QueryResult* result = CharacterDatabase.PQuery("SELECT quest, status,
  // explored, timer, mobcount1, mobcount2, mobcount3, mobcount4, itemcount1,
  // itemcount2, itemcount3,
  //                                                     11          12 13 14
  //                                                 itemcount4, itemcount5,
  //                                                 itemcount6, playercount
  //                                                 FROM character_queststatus
  //                                                 WHERE guid = '%u'",
  //                                                 GetGUID().GetCounter());

  if (result) {
    do {
      Field *fields = result->Fetch();

      uint32 quest_id = fields[0].Get<uint32>();
      // used to be new, no delete?
      Quest const *quest = sObjectMgr->GetQuestTemplate(quest_id);
      if (quest) {
        // find or create
        QuestStatusData &questStatusData = m_QuestStatus[quest_id];

        uint8 qstatus = fields[1].Get<uint8>();
        if (qstatus < MAX_QUEST_STATUS)
          questStatusData.Status = QuestStatus(qstatus);
        else {
          questStatusData.Status = QUEST_STATUS_INCOMPLETE;
          LOG_ERROR(
              "entities.player",
              "Player::_LoadQuestStatus: Player '%s' (%s) has invalid quest %d "
              "status (%u), replaced by QUEST_STATUS_INCOMPLETE(3).",
              +GetName().c_str(), GetGUID().ToString().c_str(), quest_id,
              qstatus);
        }

        questStatusData.Explored = (fields[2].Get<uint8>() > 0);

        time_t quest_time = time_t(fields[3].Get<uint32>());

        if (quest->HasSpecialFlag(QUEST_SPECIAL_FLAGS_TIMED) &&
            !GetQuestRewardStatus(quest_id)) {
          AddTimedQuest(quest_id);

          if (quest_time <= GameTime::GetGameTime())
            questStatusData.Timer = 1;
          else
            questStatusData.Timer = uint32(
                (quest_time - GameTime::GetGameTime()) * IN_MILLISECONDS);
        } else
          quest_time = 0;

        for (uint32 i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
          questStatusData.CreatureOrGOCount[i] = fields[4 + i].Get<uint16>();

        for (uint32 i = 0; i < QUEST_ITEM_OBJECTIVES_COUNT; ++i)
          questStatusData.ItemCount[i] = fields[8 + i].Get<uint16>();

        questStatusData.PlayerCount = fields[14].Get<uint16>();

        // add to quest log
        if (slot < MAX_QUEST_LOG_SIZE &&
            questStatusData.Status != QUEST_STATUS_NONE) {
          SetQuestSlot(slot, quest_id,
                       uint32(quest_time)); // cast can't be helped

          if (questStatusData.Status == QUEST_STATUS_COMPLETE)
            SetQuestSlotState(slot, QUEST_STATE_COMPLETE);
          else if (questStatusData.Status == QUEST_STATUS_FAILED)
            SetQuestSlotState(slot, QUEST_STATE_FAIL);

          for (uint8 idx = 0; idx < QUEST_OBJECTIVES_COUNT; ++idx)
            if (questStatusData.CreatureOrGOCount[idx])
              SetQuestSlotCounter(slot, idx,
                                  questStatusData.CreatureOrGOCount[idx]);

          if (questStatusData.PlayerCount)
            SetQuestSlotCounter(slot, QUEST_PVP_KILL_SLOT,
                                questStatusData.PlayerCount);

          ++slot;
        }

        LOG_DEBUG("entities.player.loading",
                  "Player::_LoadQuestStatus: Quest status is {%u} for quest "
                  "{%u} for player (%s)",
                  questStatusData.Status, quest_id,
                  GetGUID().ToString().c_str());
      }
    } while (result->NextRow());
  }

  // clear quest log tail
  for (uint16 i = slot; i < MAX_QUEST_LOG_SIZE; ++i)
    SetQuestSlot(i, 0);
}

void Player::_LoadQuestStatusRewarded(PreparedQueryResult result) {
  // SELECT quest FROM character_queststatus_rewarded WHERE guid = ?

  if (result) {
    do {
      Field *fields = result->Fetch();

      uint32 quest_id = fields[0].Get<uint32>();
      // used to be new, no delete?
      Quest const *quest = sObjectMgr->GetQuestTemplate(quest_id);
      if (quest) {
        // learn rewarded spell if unknown
        LearnQuestRewardedSpells(quest);

        // set rewarded title if any
        if (quest->GetCharTitleId()) {
          if (CharTitlesEntry const *titleEntry =
                  sCharTitlesStore.LookupEntry(quest->GetCharTitleId()))
            SetTitle(titleEntry);
        }

        if (uint32 talents = quest->GetBonusTalents())
          AddQuestRewardedTalentCount(talents);

        if (quest->CanIncreaseRewardedQuestCounters())
          m_RewardedQuests.insert(quest_id);
      }
    } while (result->NextRow());
  }
}

void Player::_LoadDailyQuestStatus(PreparedQueryResult result) {
  for (uint32 quest_daily_idx = 0; quest_daily_idx < PLAYER_MAX_DAILY_QUESTS;
       ++quest_daily_idx)
    SetUInt32Value(PLAYER_FIELD_DAILY_QUESTS_1 + quest_daily_idx, 0);

  // QueryResult* result = CharacterDatabase.PQuery("SELECT quest, time FROM
  // character_queststatus_daily WHERE guid = '%u'", GetGUID().GetCounter());

  if (result) {
    uint32 quest_daily_idx = 0;

    do {
      Field *fields = result->Fetch();

      if (quest_daily_idx >=
          PLAYER_MAX_DAILY_QUESTS) // max amount with exist data in query
      {
        LOG_ERROR("entities.player",
                  "Player (GUID: %u) has more than 25 daily quest records in "
                  "`charcter_queststatus_daily`",
                  GetGUID().GetCounter());
        break;
      }

      uint32 quest_id = fields[0].Get<uint32>();

      // save _any_ from daily quest times (it must be after last reset anyway)
      m_lastDailyQuestTime = time_t(fields[1].Get<uint32>());

      Quest const *quest = sObjectMgr->GetQuestTemplate(quest_id);
      if (!quest)
        continue;

      SetUInt32Value(PLAYER_FIELD_DAILY_QUESTS_1 + quest_daily_idx, quest_id);
      ++quest_daily_idx;

      LOG_DEBUG("entities.player.loading",
                "Player::_LoadDailyQuestStatus: Loaded daily quest cooldown "
                "(QuestID: %u) for player '%s' (%s)",
                quest_id, GetName().c_str(), GetGUID().ToString().c_str());
    } while (result->NextRow());
  }

  m_DailyQuestChanged = false;
}

void Player::_LoadWeeklyQuestStatus(PreparedQueryResult result) {
  m_weeklyquests.clear();

  if (result) {
    do {
      Field *fields = result->Fetch();
      uint32 quest_id = fields[0].Get<uint32>();
      Quest const *quest = sObjectMgr->GetQuestTemplate(quest_id);
      if (!quest)
        continue;

      m_weeklyquests.insert(quest_id);

      LOG_DEBUG("entities.player.loading",
                "Player::_LoadWeeklyQuestStatus: Loaded weekly quest cooldown "
                "(QuestID: %u) for player '%s' (%s)",
                quest_id, GetName().c_str(), GetGUID().ToString().c_str());
    } while (result->NextRow());
  }

  m_WeeklyQuestChanged = false;
}

void Player::_LoadSeasonalQuestStatus(PreparedQueryResult result) {
  m_seasonalquests.clear();

  if (result) {
    do {
      Field *fields = result->Fetch();
      uint32 quest_id = fields[0].Get<uint32>();
      uint32 event_id = fields[1].Get<uint32>();
      Quest const *quest = sObjectMgr->GetQuestTemplate(quest_id);
      if (!quest)
        continue;

      m_seasonalquests[event_id].insert(quest_id);
      LOG_DEBUG("entities.player.loading",
                "Player::_LoadSeasonalQuestStatus: Loaded seasonal quest "
                "cooldown (QuestID: %u) for player '%s' (%s)",
                quest_id, GetName().c_str(), GetGUID().ToString().c_str());
    } while (result->NextRow());
  }

  m_SeasonalQuestChanged = false;
}

void Player::_LoadMonthlyQuestStatus(PreparedQueryResult result) {
  m_monthlyquests.clear();

  if (result) {
    do {
      Field *fields = result->Fetch();
      uint32 quest_id = fields[0].Get<uint32>();
      Quest const *quest = sObjectMgr->GetQuestTemplate(quest_id);
      if (!quest)
        continue;

      m_monthlyquests.insert(quest_id);
      LOG_DEBUG("entities.player.loading",
                "Player::_LoadMonthlyQuestStatus: Loaded monthly quest "
                "cooldown (QuestID: %u) for player '%s' (%s)",
                quest_id, GetName().c_str(), GetGUID().ToString().c_str());
    } while (result->NextRow());
  }

  m_MonthlyQuestChanged = false;
}

void Player::_LoadLFGRewardStatus(PreparedQueryResult result) {
  m_lfgrewardstatus.clear();

  if (result) {
    do {
      Field *fields = result->Fetch();
      uint32 dungeon_id = fields[0].Get<uint16>();
      uint8 reward_count = fields[1].Get<uint8>();
      uint8 daily_reset = fields[2].Get<uint8>();

      m_lfgrewardstatus[dungeon_id] = LFGRewardInfo(reward_count, daily_reset);
      LOG_DEBUG("entities.player.loading",
                "Player::_LFGQuestStatus: Loaded LFG quest first reward "
                "cooldown (DungeonID: %u) for player '%s' (%s)",
                dungeon_id, GetName().c_str(), GetGUID().ToString().c_str());
    } while (result->NextRow());
  }

  m_LFGRewardStatusChanged = false;
}

void Player::_LoadSpells(PreparedQueryResult result) {
  // QueryResult* result = CharacterDatabase.PQuery("SELECT spell, active,
  // disabled FROM character_spell WHERE guid = '%u'", GetGUID().GetCounter());

  if (result) {
    do
      AddSpell((*result)[0].Get<uint32>(), (*result)[1].Get<bool>(), false,
               false, (*result)[2].Get<bool>(), true);
    while (result->NextRow());
  }
}

void Player::_LoadGroup(PreparedQueryResult result) {
  // QueryResult* result = CharacterDatabase.PQuery("SELECT guid FROM
  // group_member WHERE memberGuid=%u", GetGUID().GetCounter());
  if (result) {
    if (Group *group =
            sGroupMgr->GetGroupByDbStoreId((*result)[0].Get<uint32>())) {
      if (group->IsLeader(GetGUID()))
        SetFlag(PLAYER_FLAGS, PLAYER_FLAGS_GROUP_LEADER);

      uint8 subgroup = group->GetMemberGroup(GetGUID());
      SetGroup(group, subgroup);
      if (getLevel() >= LEVELREQUIREMENT_HEROIC) {
        // the group leader may change the instance difficulty while the player
        // is offline
        SetDungeonDifficulty(group->GetDungeonDifficulty());
        SetRaidDifficulty(group->GetRaidDifficulty());
      }
    }
  }

  if (!GetGroup() || !GetGroup()->IsLeader(GetGUID()))
    RemoveFlag(PLAYER_FLAGS, PLAYER_FLAGS_GROUP_LEADER);
}

void Player::_LoadBoundInstances(PreparedQueryResult result) {
  for (uint8 i = 0; i < MAX_DIFFICULTY; ++i)
    m_boundInstances[i].clear();

  Group *group = GetGroup();

  //         0          1    2           3            4          5
  // SELECT id, permanent, map, difficulty, extendState, resettime FROM
  // character_instance LEFT JOIN instance ON instance = id WHERE guid = ?
  if (result) {
    do {
      Field *fields = result->Fetch();

      bool perm = fields[1].Get<bool>();
      uint32 mapId = fields[2].Get<uint16>();
      uint32 instanceId = fields[0].Get<uint32>();
      uint8 difficulty = fields[3].Get<uint8>();
      BindExtensionState extendState =
          BindExtensionState(fields[4].Get<uint8>());

      time_t resetTime = time_t(fields[5].Get<uint64>());
      // the resettime for normal instances is only saved when the InstanceSave
      // is unloaded so the value read from the DB may be wrong here but only if
      // the InstanceSave is loaded and in that case it is not used

      bool deleteInstance = false;

      MapEntry const *mapEntry = sMapStore.LookupEntry(mapId);
      std::string mapname = mapEntry ? mapEntry->MapName : "Unknown";

      if (!mapEntry || !mapEntry->IsDungeon()) {
        LOG_ERROR("entities.player",
                  "Player::_LoadBoundInstances: Player '%s' (%s) has bind to "
                  "not existed or not dungeon map %d (%s)",
                  GetName().c_str(), GetGUID().ToString().c_str(), mapId,
                  mapname.c_str());
        deleteInstance = true;
      } else if (difficulty >= MAX_DIFFICULTY) {
        LOG_ERROR("entities.player",
                  "Player::_LoadBoundInstances: player '%s' (%s) has bind to "
                  "not existed difficulty %d instance for map %u (%s)",
                  GetName().c_str(), GetGUID().ToString().c_str(), difficulty,
                  mapId, mapname.c_str());
        deleteInstance = true;
      } else {
        MapDifficulty const *mapDiff =
            sDBCManager.GetMapDifficultyData(mapId, Difficulty(difficulty));
        if (!mapDiff) {
          LOG_ERROR("entities.player",
                    "Player::_LoadBoundInstances: player '%s' (%s) has bind to "
                    "not existed difficulty %d instance for map %u (%s)",
                    GetName().c_str(), GetGUID().ToString().c_str(), difficulty,
                    mapId, mapname.c_str());
          deleteInstance = true;
        } else if (!perm && group) {
          LOG_ERROR(
              "entities.player",
              "Player::_LoadBoundInstances: player '%s' (%s) is in group %s "
              "but has a non-permanent character bind to map %d (%s), %d, %d",
              GetName().c_str(), GetGUID().ToString().c_str(),
              group->GetGUID().ToString().c_str(), mapId, mapname.c_str(),
              instanceId, difficulty);
          deleteInstance = true;
        }
      }

      if (deleteInstance) {
        CharacterDatabasePreparedStatement *stmt =
            CharacterDatabase.GetPreparedStatement(
                CHAR_DEL_CHAR_INSTANCE_BY_INSTANCE_GUID);

        stmt->SetData(0, GetGUID().GetCounter());
        stmt->SetData(1, instanceId);

        CharacterDatabase.Execute(stmt);

        continue;
      }

      // since non permanent binds are always solo bind, they can always be
      // reset
      if (InstanceSave *save = sInstanceSaveMgr->AddInstanceSave(
              mapId, instanceId, Difficulty(difficulty), resetTime, !perm,
              true))
        BindToInstance(save, perm, extendState, true);
    } while (result->NextRow());
  }
}

InstancePlayerBind *Player::GetBoundInstance(uint32 mapid,
                                             Difficulty difficulty,
                                             bool withExpired) {
  // some instances only have one difficulty
  MapDifficulty const *mapDiff =
      sDBCManager.GetDownscaledMapDifficultyData(mapid, difficulty);
  if (!mapDiff)
    return nullptr;

  BoundInstancesMap::iterator itr = m_boundInstances[difficulty].find(mapid);
  if (itr != m_boundInstances[difficulty].end())
    if (itr->second.extendState || withExpired)
      return &itr->second;
  return nullptr;
}

InstancePlayerBind const *Player::GetBoundInstance(uint32 mapid,
                                                   Difficulty difficulty,
                                                   bool withExpired) const {
  // some instances only have one difficulty
  MapDifficulty const *mapDiff =
      sDBCManager.GetDownscaledMapDifficultyData(mapid, difficulty);
  if (!mapDiff)
    return nullptr;

  BoundInstancesMap::const_iterator itr =
      m_boundInstances[difficulty].find(mapid);
  if (itr != m_boundInstances[difficulty].end())
    if (itr->second.extendState || withExpired)
      return &itr->second;
  return nullptr;
}

InstanceSave *Player::GetInstanceSave(uint32 mapid, bool raid) {
  InstancePlayerBind *pBind = GetBoundInstance(mapid, GetDifficulty(raid));
  InstanceSave *pSave = pBind ? pBind->save : nullptr;
  if (!pBind || !pBind->perm)
    if (Group *group = GetGroup())
      if (InstanceGroupBind *groupBind =
              group->GetBoundInstance(GetDifficulty(raid), mapid))
        pSave = groupBind->save;

  return pSave;
}

void Player::UnbindInstance(uint32 mapid, Difficulty difficulty, bool unload) {
  BoundInstancesMap::iterator itr = m_boundInstances[difficulty].find(mapid);
  UnbindInstance(itr, difficulty, unload);
}

void Player::UnbindInstance(BoundInstancesMap::iterator &itr,
                            Difficulty difficulty, bool unload) {
  if (itr != m_boundInstances[difficulty].end()) {
    if (!unload) {
      CharacterDatabasePreparedStatement *stmt =
          CharacterDatabase.GetPreparedStatement(
              CHAR_DEL_CHAR_INSTANCE_BY_INSTANCE_GUID);

      stmt->SetData(0, GetGUID().GetCounter());
      stmt->SetData(1, itr->second.save->GetInstanceId());

      CharacterDatabase.Execute(stmt);
    }

    if (itr->second.perm)
      GetSession()->SendCalendarRaidLockout(itr->second.save, false);

    itr->second.save->RemovePlayer(this); // save can become invalid
    m_boundInstances[difficulty].erase(itr++);
  }
}

InstancePlayerBind *Player::BindToInstance(InstanceSave *save, bool permanent,
                                           BindExtensionState extendState,
                                           bool load) {
  if (save) {
    InstancePlayerBind &bind =
        m_boundInstances[save->GetDifficulty()][save->GetMapId()];
    if (extendState ==
        EXTEND_STATE_KEEP) // special flag, keep the player's current extend
                           // state when updating for new boss down
    {
      if (save == bind.save)
        extendState = bind.extendState;
      else
        extendState = EXTEND_STATE_NORMAL;
    }
    if (!load) {
      if (bind.save) {
        // update the save when the group kills a boss
        if (permanent != bind.perm || save != bind.save ||
            extendState != bind.extendState) {
          CharacterDatabasePreparedStatement *stmt =
              CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_INSTANCE);

          stmt->SetData(0, save->GetInstanceId());
          stmt->SetData(1, permanent);
          stmt->SetData(2, extendState);
          stmt->SetData(3, GetGUID().GetCounter());
          stmt->SetData(4, bind.save->GetInstanceId());

          CharacterDatabase.Execute(stmt);
        }
      } else {
        CharacterDatabasePreparedStatement *stmt =
            CharacterDatabase.GetPreparedStatement(CHAR_INS_CHAR_INSTANCE);

        stmt->SetData(0, GetGUID().GetCounter());
        stmt->SetData(1, save->GetInstanceId());
        stmt->SetData(2, permanent);
        stmt->SetData(3, extendState);

        CharacterDatabase.Execute(stmt);
      }
    }

    if (bind.save != save) {
      if (bind.save)
        bind.save->RemovePlayer(this);
      save->AddPlayer(this);
    }

    if (permanent)
      save->SetCanReset(false);

    bind.save = save;
    bind.perm = permanent;
    bind.extendState = extendState;
    if (!load)
      LOG_DEBUG("maps",
                "Player::BindToInstance: Player '%s' (%s) is now bound to map "
                "(ID: %d, Instance: %d, Difficulty: %d)",
                GetName().c_str(), GetGUID().ToString().c_str(),
                save->GetMapId(), save->GetInstanceId(), save->GetDifficulty());
    sScriptMgr->OnPlayerBindToInstance(
        this, save->GetDifficulty(), save->GetMapId(), permanent, extendState);
    return &bind;
  }

  return nullptr;
}

void Player::BindToInstance() {
  InstanceSave *mapSave = sInstanceSaveMgr->GetInstanceSave(_pendingBindId);
  if (!mapSave) // it seems sometimes mapSave is nullptr, but I did not check
                // why
    return;

  WorldPackets::Instance::InstanceSaveCreated instanceSaveCreated;
  SendDirectMessage(instanceSaveCreated.Write());
  if (!IsGameMaster()) {
    BindToInstance(mapSave, true, EXTEND_STATE_KEEP);
    GetSession()->SendCalendarRaidLockout(mapSave, true);
  }
}

void Player::SetPendingBind(uint32 instanceId, uint32 bindTimer) {
  _pendingBindId = instanceId;
  _pendingBindTimer = bindTimer;
}

void Player::SendRaidInfo() {
  WorldPackets::Instance::InstanceInfo instanceInfo;

  time_t now = GameTime::GetGameTime();

  for (uint8 i = 0; i < MAX_DIFFICULTY; ++i) {
    for (BoundInstancesMap::iterator itr = m_boundInstances[i].begin();
         itr != m_boundInstances[i].end(); ++itr) {
      InstancePlayerBind const &bind = itr->second;
      if (bind.perm) {
        InstanceSave *save = bind.save;

        WorldPackets::Instance::InstanceLock lockInfos;

        Difficulty difficulty = save->GetDifficulty();
        lockInfos.Heroic = (difficulty == RAID_DIFFICULTY_10MAN_HEROIC ||
                            difficulty == RAID_DIFFICULTY_25MAN_HEROIC);

        if (lockInfos.Heroic)
          lockInfos.Difficulty = difficulty == RAID_DIFFICULTY_10MAN_HEROIC
                                     ? RAID_DIFFICULTY_10MAN_NORMAL
                                     : RAID_DIFFICULTY_25MAN_NORMAL;
        else
          lockInfos.Difficulty = difficulty;

        lockInfos.InstanceID = save->GetInstanceId();
        lockInfos.MapID = save->GetMapId();
        if (bind.extendState != EXTEND_STATE_EXTENDED)
          lockInfos.TimeRemaining = save->GetResetTime() - now;
        else
          lockInfos.TimeRemaining = sInstanceSaveMgr->GetSubsequentResetTime(
                                        save->GetMapId(), save->GetDifficulty(),
                                        save->GetResetTime()) -
                                    now;

        if (Map *map =
                sMapMgr->FindMap(save->GetMapId(), save->GetInstanceId()))
          if (InstanceScript *instanceScript =
                  ((InstanceMap *)map)->GetInstanceScript())
            lockInfos.CompletedMask =
                instanceScript->GetCompletedEncounterMask();

        lockInfos.Locked = bind.extendState != EXTEND_STATE_EXPIRED;
        lockInfos.Extended = bind.extendState == EXTEND_STATE_EXTENDED;

        instanceInfo.LockList.push_back(lockInfos);
      }
    }
  }

  SendDirectMessage(instanceInfo.Write());
}

/*
- called on every successful teleportation to a map
*/
void Player::SendSavedInstances() {
  bool hasBeenSaved = false;

  for (uint8 i = 0; i < MAX_DIFFICULTY; ++i) {
    for (BoundInstancesMap::iterator itr = m_boundInstances[i].begin();
         itr != m_boundInstances[i].end(); ++itr) {
      if (itr->second.perm) // only permanent binds are sent
      {
        hasBeenSaved = true;
        break;
      }
    }
  }

  // Send opcode SMSG_UPDATE_INSTANCE_OWNERSHIP. true or false means, whether
  // you have current raid/heroic instances
  WorldPackets::Instance::UpdateInstanceOwnership updateInstanceOwnership;
  updateInstanceOwnership.IOwnInstance = hasBeenSaved;
  SendDirectMessage(updateInstanceOwnership.Write());

  if (!hasBeenSaved)
    return;

  for (uint8 i = 0; i < MAX_DIFFICULTY; ++i) {
    for (BoundInstancesMap::iterator itr = m_boundInstances[i].begin();
         itr != m_boundInstances[i].end(); ++itr) {
      if (itr->second.perm) {
        WorldPackets::Instance::UpdateLastInstance packet;
        packet.MapID = itr->second.save->GetMapId();
        SendDirectMessage(packet.Write());
      }
    }
  }
}

bool Player::Satisfy(AccessRequirement const *ar, uint32 target_map,
                     bool report) {
  if (!IsGameMaster() && ar) {
    uint8 LevelMin = 0;
    uint8 LevelMax = 0;

    MapEntry const *mapEntry = sMapStore.LookupEntry(target_map);
    if (!mapEntry)
      return false;

    if (DisableMgr::IsDisabledFor(DISABLE_TYPE_MAP, target_map, this)) {
      GetSession()->SendAreaTriggerMessage(
          "%s", GetSession()->GetFirelandsString(LANG_INSTANCE_CLOSED));
      return false;
    }

    if (!sWorld->getBoolConfig(CONFIG_INSTANCE_IGNORE_LEVEL)) {
      if (ar->levelMin && getLevel() < ar->levelMin)
        LevelMin = ar->levelMin;
      if (ar->levelMax && getLevel() > ar->levelMax)
        LevelMax = ar->levelMax;
    }

    uint32 missingItem = 0;
    if (ar->item) {
      if (!HasItemCount(ar->item) && (!ar->item2 || !HasItemCount(ar->item2)))
        missingItem = ar->item;
    } else if (ar->item2 && !HasItemCount(ar->item2))
      missingItem = ar->item2;

    uint32 missingQuest = 0;
    if (GetTeam() == ALLIANCE && ar->quest_A &&
        !GetQuestRewardStatus(ar->quest_A))
      missingQuest = ar->quest_A;
    else if (GetTeam() == HORDE && ar->quest_H &&
             !GetQuestRewardStatus(ar->quest_H))
      missingQuest = ar->quest_H;

    uint32 missingAchievement = 0;
    Player *leader = this;
    ObjectGuid leaderGuid =
        GetGroup() ? GetGroup()->GetLeaderGUID() : GetGUID();
    if (leaderGuid != GetGUID())
      leader = ObjectAccessor::FindPlayer(leaderGuid);

    if (ar->achievement)
      if (!leader || !leader->HasAchieved(ar->achievement))
        missingAchievement = ar->achievement;

    bool heroicExclusive = ar->heroicExclusive;

    Difficulty target_difficulty = GetDifficulty(mapEntry->IsRaid());
    MapDifficulty const *mapDiff = sDBCManager.GetDownscaledMapDifficultyData(
        target_map, target_difficulty);
    if (!mapDiff)
      return false;

    if (LevelMin || LevelMax || missingItem || missingQuest ||
        missingAchievement || heroicExclusive) {
      if (report && mapDiff->hasErrorMessage)
        SendTransferAborted(target_map, TRANSFER_ABORT_DIFFICULTY,
                            target_difficulty);
      return false;
    }
  }
  return true;
}

bool Player::IsInstanceLoginGameMasterException() const {
  if (CanBeGameMaster()) {
    ChatHandler(GetSession())
        .SendSysMessage(LANG_INSTANCE_LOGIN_GAMEMASTER_EXCEPTION);
    return true;
  } else
    return false;
}

bool Player::CheckInstanceValidity(bool /*isLogin*/) {
  // game masters' instances are always valid
  if (IsGameMaster())
    return true;

  // non-instances are always valid
  Map *map = FindMap();
  if (!map || !map->IsDungeon())
    return true;

  // raid instances require the player to be in a raid group to be valid
  if (map->IsRaid() && !sWorld->getBoolConfig(CONFIG_INSTANCE_IGNORE_RAID))
    if (!GetGroup() || !GetGroup()->isRaidGroup())
      return false;

  if (Group *group = GetGroup()) {
    // check if player's group is bound to this instance
    InstanceGroupBind *bind =
        group->GetBoundInstance(map->GetDifficulty(), map->GetId());
    if (!bind || !bind->save ||
        bind->save->GetInstanceId() != map->GetInstanceId())
      return false;

    Map::PlayerList const &players = map->GetPlayers();
    if (!players.isEmpty())
      for (Map::PlayerList::const_iterator it = players.begin();
           it != players.end(); ++it) {
        if (Player *otherPlayer = it->GetSource()) {
          if (otherPlayer->IsGameMaster())
            continue;
          if (!otherPlayer->m_InstanceValid) // ignore players that currently
                                             // have a homebind timer active
            continue;
          if (group != otherPlayer->GetGroup())
            return false;

          Map::PlayerList const &players = map->GetPlayers();
          if (!players.isEmpty())
            for (Map::PlayerList::const_iterator it = players.begin();
                 it != players.end(); ++it) {
              if (Player *otherPlayer = it->GetSource()) {
                if (otherPlayer->IsGameMaster())
                  continue;
                if (!otherPlayer
                         ->m_InstanceValid) // ignore players that currently
                                            // have a homebind timer active
                  continue;
                if (group != otherPlayer->GetGroup())
                  return false;
              }
            }
        }
      }
  } else {
    // instance is invalid if we are not grouped and there are other players
    if (map->GetPlayersCountExceptGMs() > 1)
      return false;

    // check if the player is bound to this instance
    InstancePlayerBind *bind =
        GetBoundInstance(map->GetId(), map->GetDifficulty());
    if (!bind || !bind->save ||
        bind->save->GetInstanceId() != map->GetInstanceId())
      return false;
  }

  return true;
}

bool Player::CheckInstanceCount(uint32 instanceId) const {
  if (_instanceResetTimes.size() <
      sWorld->getIntConfig(CONFIG_MAX_INSTANCES_PER_HOUR))
    return true;
  return _instanceResetTimes.find(instanceId) != _instanceResetTimes.end();
}

void Player::AddInstanceEnterTime(uint32 instanceId, time_t enterTime) {
  if (_instanceResetTimes.find(instanceId) == _instanceResetTimes.end())
    _instanceResetTimes.insert(
        InstanceTimeMap::value_type(instanceId, enterTime + HOUR));
}

bool Player::_LoadHomeBind(PreparedQueryResult result) {
  PlayerInfo const *info = sObjectMgr->GetPlayerInfo(getRace(), getClass());
  if (!info) {
    LOG_ERROR("entities.player",
              "Player (Name %s) has incorrect race/class (%u/%u) pair. Can't "
              "be loaded.",
              GetName().c_str(), uint32(getRace()), uint32(getClass()));
    return false;
  }

  bool ok = false;
  // SELECT mapId, zoneId, posX, posY, posZ FROM character_homebind WHERE guid =
  // ?
  if (result) {
    Field *fields = result->Fetch();

    m_homebindMapId = fields[0].Get<uint16>();
    m_homebindAreaId = fields[1].Get<uint16>();
    m_homebindX = fields[2].Get<float>();
    m_homebindY = fields[3].Get<float>();
    m_homebindZ = fields[4].Get<float>();

    MapEntry const *bindMapEntry = sMapStore.LookupEntry(m_homebindMapId);

    // accept saved data only for valid position (and non instanceable), and
    // accessable
    if (MapManager::IsValidMapCoord(m_homebindMapId, m_homebindX, m_homebindY,
                                    m_homebindZ) &&
        !bindMapEntry->Instanceable() &&
        GetSession()->GetExpansion() >= bindMapEntry->Expansion())
      ok = true;
    else {
      CharacterDatabasePreparedStatement *stmt =
          CharacterDatabase.GetPreparedStatement(CHAR_DEL_PLAYER_HOMEBIND);
      stmt->SetData(0, GetGUID().GetCounter());
      CharacterDatabase.Execute(stmt);
    }
  }

  if (!ok) {
    m_homebindMapId = info->mapId;
    m_homebindAreaId = info->areaId;
    m_homebindX = info->positionX;
    m_homebindY = info->positionY;
    m_homebindZ = info->positionZ;

    CharacterDatabasePreparedStatement *stmt =
        CharacterDatabase.GetPreparedStatement(CHAR_INS_PLAYER_HOMEBIND);
    stmt->SetData(0, GetGUID().GetCounter());
    stmt->SetData(1, m_homebindMapId);
    stmt->SetData(2, m_homebindAreaId);
    stmt->SetData(3, m_homebindX);
    stmt->SetData(4, m_homebindY);
    stmt->SetData(5, m_homebindZ);
    CharacterDatabase.Execute(stmt);
  }

  LOG_DEBUG("entities.player",
            "Player::_LoadHomeBind: Setting home position (MapID: %u, AreaID: "
            "%u, X: %f, Y: %f, Z: %f) of player '%s' (%s)",
            m_homebindMapId, m_homebindAreaId, m_homebindX, m_homebindY,
            m_homebindZ, GetName().c_str(), GetGUID().ToString().c_str());

  return true;
}

/*********************************************************/
/***                   SAVE SYSTEM                     ***/
/*********************************************************/

void Player::SaveToDB(bool create /*=false*/) {
  CharacterDatabaseTransaction trans = CharacterDatabase.BeginTransaction();

  SaveToDB(trans, create);

  CharacterDatabase.CommitTransaction(trans);
}

void Player::SaveToDB(CharacterDatabaseTransaction trans,
                      bool create /* = false */) {
  // delay auto save at any saves (manual, in code, or autosave)
  m_nextSave = sWorld->getIntConfig(CONFIG_INTERVAL_SAVE);

  // lets allow only players in world to be saved
  if (IsBeingTeleportedFar()) {
    ScheduleDelayedOperation(DELAYED_SAVE_PLAYER);
    return;
  }

  // first save/honor gain after midnight will also update the player's honor
  // fields
  UpdateHonorFields();

  LOG_DEBUG(
      "entities.unit",
      "Player::SaveToDB: The value of player %s at save: ", m_name.c_str());
  outDebugValues();

  if (!create)
    sScriptMgr->OnPlayerSave(this);

  CharacterDatabasePreparedStatement *stmt = nullptr;
  uint8 index = 0;

  stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_FISHINGSTEPS);
  stmt->SetData(0, GetGUID().GetCounter());
  trans->Append(stmt);

  auto finiteAlways = [](float f) { return std::isfinite(f) ? f : 0.0f; };

  if (create) {
    //! Insert query
    /// @todo: Filter out more redundant fields that can take their default
    /// value at player create
    stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_CHARACTER);
    stmt->SetData(index++, GetGUID().GetCounter());
    stmt->SetData(index++, GetSession()->GetAccountId());
    stmt->SetData(index++, GetName());
    stmt->SetData(index++, getRace());
    stmt->SetData(index++, getClass());
    stmt->SetData(
        index++,
        GetByteValue(
            PLAYER_BYTES_3,
            PLAYER_BYTES_3_OFFSET_GENDER)); // save gender from PLAYER_BYTES_3,
                                            // UNIT_BYTES_0 changes with every
                                            // transform effect
    stmt->SetData(index++, getLevel());
    stmt->SetData(index++, GetUInt32Value(PLAYER_XP));
    stmt->SetData(index++, GetMoney());
    stmt->SetData(index++,
                  GetByteValue(PLAYER_BYTES, PLAYER_BYTES_OFFSET_SKIN_ID));
    stmt->SetData(index++,
                  GetByteValue(PLAYER_BYTES, PLAYER_BYTES_OFFSET_FACE_ID));
    stmt->SetData(
        index++, GetByteValue(PLAYER_BYTES, PLAYER_BYTES_OFFSET_HAIR_STYLE_ID));
    stmt->SetData(
        index++, GetByteValue(PLAYER_BYTES, PLAYER_BYTES_OFFSET_HAIR_COLOR_ID));
    stmt->SetData(index++, GetByteValue(PLAYER_BYTES_2,
                                        PLAYER_BYTES_2_OFFSET_FACIAL_STYLE));
    stmt->SetData(index++, GetByteValue(PLAYER_BYTES_2,
                                        PLAYER_BYTES_2_OFFSET_BANK_BAG_SLOTS));
    stmt->SetData(index++, GetByteValue(PLAYER_BYTES_2,
                                        PLAYER_BYTES_2_OFFSET_REST_STATE));
    stmt->SetData(index++, GetUInt32Value(PLAYER_FLAGS));
    stmt->SetData(index++, (uint16)GetMapId());
    stmt->SetData(index++, (uint32)GetInstanceId());
    stmt->SetData(index++, (uint8(GetDungeonDifficulty()) |
                            uint8(GetRaidDifficulty()) << 4));
    stmt->SetData(index++, finiteAlways(GetPositionX()));
    stmt->SetData(index++, finiteAlways(GetPositionY()));
    stmt->SetData(index++, finiteAlways(GetPositionZ()));
    stmt->SetData(index++, finiteAlways(GetOrientation()));
    stmt->SetData(index++, finiteAlways(GetTransOffsetX()));
    stmt->SetData(index++, finiteAlways(GetTransOffsetY()));
    stmt->SetData(index++, finiteAlways(GetTransOffsetZ()));
    stmt->SetData(index++, finiteAlways(GetTransOffsetO()));
    ObjectGuid::LowType transLowGUID = 0;
    if (Transport *transport = dynamic_cast<Transport *>(GetTransport()))
      transLowGUID = transport->GetGUID().GetCounter();
    stmt->SetData(index++, transLowGUID);

    std::ostringstream ss;
    ss << m_taxi;
    stmt->SetData(index++, ss.str());
    stmt->SetData(index++, m_cinematic);
    stmt->SetData(index++, m_Played_time[PLAYED_TIME_TOTAL]);
    stmt->SetData(index++, m_Played_time[PLAYED_TIME_LEVEL]);
    stmt->SetData(index++, finiteAlways(m_rest_bonus));
    stmt->SetData(index++, uint32(GameTime::GetGameTime()));
    stmt->SetData(index++,
                  (HasFlag(PLAYER_FLAGS, PLAYER_FLAGS_RESTING) ? 1 : 0));
    // save, far from tavern/city
    // save, but in tavern/city
    stmt->SetData(index++, GetTalentResetCost());
    stmt->SetData(index++, GetTalentResetTime());

    ss.str("");
    for (uint8 i = 0; i < MAX_TALENT_SPECS; ++i)
      ss << GetPrimaryTalentTree(i) << " ";
    stmt->SetData(index++, ss.str());
    stmt->SetData(index++, (uint16)m_ExtraFlags);
    stmt->SetData(index++, m_stableSlots);
    stmt->SetData(index++, (uint16)m_atLoginFlags);
    stmt->SetData(index++, GetZoneId());
    stmt->SetData(index++, uint32(m_deathExpireTime));

    ss.str("");
    ss << m_taxi.SaveTaxiDestinationsToString();

    stmt->SetData(index++, ss.str());
    stmt->SetData(index++,
                  GetUInt32Value(PLAYER_FIELD_LIFETIME_HONORABLE_KILLS));
    stmt->SetData(index++, GetUInt16Value(PLAYER_FIELD_KILLS, 0));
    stmt->SetData(index++, GetUInt16Value(PLAYER_FIELD_KILLS, 1));
    stmt->SetData(index++, GetUInt32Value(PLAYER_CHOSEN_TITLE));
    stmt->SetData(index++, GetUInt32Value(PLAYER_FIELD_WATCHED_FACTION_INDEX));
    stmt->SetData(index++, GetDrunkValue());
    stmt->SetData(index++, GetHealth());

    uint32 storedPowers = 0;
    for (uint32 i = 0; i < MAX_POWERS; ++i) {
      if (GetPowerIndex(Powers(i)) != MAX_POWERS) {
        stmt->SetData(index++,
                      GetUInt32Value(UNIT_FIELD_POWER1 + storedPowers));
        if (++storedPowers >= MAX_POWERS_PER_CLASS)
          break;
      }
    }

    for (; storedPowers < MAX_POWERS_PER_CLASS; ++storedPowers)
      stmt->SetData(index++, 0);

    stmt->SetData(index++, GetSession()->GetLatency());

    stmt->SetData(index++, GetSpecsCount());
    stmt->SetData(index++, GetActiveSpec());

    ss.str("");
    for (uint32 i = 0; i < PLAYER_EXPLORED_ZONES_SIZE; ++i)
      ss << GetUInt32Value(PLAYER_EXPLORED_ZONES_1 + i) << ' ';
    stmt->SetData(index++, ss.str());

    ss.str("");
    // cache equipment...
    for (uint32 i = 0; i < EQUIPMENT_SLOT_END * 2; ++i)
      ss << GetUInt32Value(PLAYER_VISIBLE_ITEM_1_ENTRYID + i) << ' ';

    // ...and bags for enum opcode
    for (uint32 i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; ++i) {
      if (Item *item = GetItemByPos(INVENTORY_SLOT_BAG_0, i))
        ss << item->GetEntry();
      else
        ss << '0';
      ss << " 0 ";
    }
    stmt->SetData(index++, ss.str());

    ss.str("");
    for (uint32 i = 0; i < KNOWN_TITLES_SIZE * 2; ++i)
      ss << GetUInt32Value(PLAYER__FIELD_KNOWN_TITLES + i) << ' ';

    stmt->SetData(index++, ss.str());
    stmt->SetData(index++,
                  GetByteValue(PLAYER_FIELD_BYTES,
                               PLAYER_FIELD_BYTES_OFFSET_ACTION_BAR_TOGGLES));
    stmt->SetData(index++, m_grantableLevels);
    stmt->SetData(index++, GetAchievementPoints());
  } else {
    // Update query
    stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHARACTER);
    stmt->SetData(index++, GetName());
    stmt->SetData(index++, getRace());
    stmt->SetData(index++, getClass());
    stmt->SetData(
        index++,
        GetByteValue(
            PLAYER_BYTES_3,
            PLAYER_BYTES_3_OFFSET_GENDER)); // save gender from PLAYER_BYTES_3,
                                            // UNIT_BYTES_0 changes with every
                                            // transform effect
    stmt->SetData(index++, getLevel());
    stmt->SetData(index++, GetUInt32Value(PLAYER_XP));
    stmt->SetData(index++, GetMoney());
    stmt->SetData(index++,
                  GetByteValue(PLAYER_BYTES, PLAYER_BYTES_OFFSET_SKIN_ID));
    stmt->SetData(index++,
                  GetByteValue(PLAYER_BYTES, PLAYER_BYTES_OFFSET_FACE_ID));
    stmt->SetData(
        index++, GetByteValue(PLAYER_BYTES, PLAYER_BYTES_OFFSET_HAIR_STYLE_ID));
    stmt->SetData(
        index++, GetByteValue(PLAYER_BYTES, PLAYER_BYTES_OFFSET_HAIR_COLOR_ID));
    stmt->SetData(index++, GetByteValue(PLAYER_BYTES_2,
                                        PLAYER_BYTES_2_OFFSET_FACIAL_STYLE));
    stmt->SetData(index++, GetByteValue(PLAYER_BYTES_2,
                                        PLAYER_BYTES_2_OFFSET_BANK_BAG_SLOTS));
    stmt->SetData(index++, GetByteValue(PLAYER_BYTES_2,
                                        PLAYER_BYTES_2_OFFSET_REST_STATE));
    stmt->SetData(index++, GetUInt32Value(PLAYER_FLAGS));

    if (!IsBeingTeleported()) {
      stmt->SetData(index++, (uint16)GetMapId());
      stmt->SetData(index++, (uint32)GetInstanceId());
      stmt->SetData(index++, (uint8(GetDungeonDifficulty()) |
                              uint8(GetRaidDifficulty()) << 4));
      stmt->SetData(index++, finiteAlways(GetPositionX()));
      stmt->SetData(index++, finiteAlways(GetPositionY()));
      stmt->SetData(index++, finiteAlways(GetPositionZ()));
      stmt->SetData(index++, finiteAlways(GetOrientation()));
    } else {
      stmt->SetData(index++, (uint16)GetTeleportDest().GetMapId());
      stmt->SetData(index++, (uint32)0);
      stmt->SetData(index++, (uint8(GetDungeonDifficulty()) |
                              uint8(GetRaidDifficulty()) << 4));
      stmt->SetData(index++, finiteAlways(GetTeleportDest().GetPositionX()));
      stmt->SetData(index++, finiteAlways(GetTeleportDest().GetPositionY()));
      stmt->SetData(index++, finiteAlways(GetTeleportDest().GetPositionZ()));
      stmt->SetData(index++, finiteAlways(GetTeleportDest().GetOrientation()));
    }

    stmt->SetData(index++, finiteAlways(GetTransOffsetX()));
    stmt->SetData(index++, finiteAlways(GetTransOffsetY()));
    stmt->SetData(index++, finiteAlways(GetTransOffsetZ()));
    stmt->SetData(index++, finiteAlways(GetTransOffsetO()));
    ObjectGuid::LowType transLowGUID = 0;
    if (Transport *transport = dynamic_cast<Transport *>(GetTransport()))
      transLowGUID = transport->GetGUID().GetCounter();
    stmt->SetData(index++, transLowGUID);

    std::ostringstream ss;
    ss << m_taxi;
    stmt->SetData(index++, ss.str());
    stmt->SetData(index++, m_cinematic);
    stmt->SetData(index++, m_Played_time[PLAYED_TIME_TOTAL]);
    stmt->SetData(index++, m_Played_time[PLAYED_TIME_LEVEL]);
    stmt->SetData(index++, finiteAlways(m_rest_bonus));
    stmt->SetData(index++, uint32(GameTime::GetGameTime()));
    stmt->SetData(index++,
                  (HasFlag(PLAYER_FLAGS, PLAYER_FLAGS_RESTING) ? 1 : 0));
    // save, far from tavern/city
    // save, but in tavern/city
    stmt->SetData(index++, GetTalentResetCost());
    stmt->SetData(index++, GetTalentResetTime());

    ss.str("");
    for (uint8 i = 0; i < MAX_TALENT_SPECS; ++i)
      ss << GetPrimaryTalentTree(i) << " ";
    stmt->SetData(index++, ss.str());
    stmt->SetData(index++, (uint16)m_ExtraFlags);
    stmt->SetData(index++, m_stableSlots);
    stmt->SetData(index++, (uint16)m_atLoginFlags);
    stmt->SetData(index++, GetZoneId());
    stmt->SetData(index++, uint32(m_deathExpireTime));

    ss.str("");
    ss << m_taxi.SaveTaxiDestinationsToString();

    stmt->SetData(index++, ss.str());
    stmt->SetData(index++,
                  GetUInt32Value(PLAYER_FIELD_LIFETIME_HONORABLE_KILLS));
    stmt->SetData(index++, GetUInt16Value(PLAYER_FIELD_KILLS, 0));
    stmt->SetData(index++, GetUInt16Value(PLAYER_FIELD_KILLS, 1));
    stmt->SetData(index++, GetUInt32Value(PLAYER_CHOSEN_TITLE));
    stmt->SetData(index++, GetUInt32Value(PLAYER_FIELD_WATCHED_FACTION_INDEX));
    stmt->SetData(index++, GetDrunkValue());
    stmt->SetData(index++, GetHealth());

    uint32 storedPowers = 0;
    for (uint32 i = 0; i < MAX_POWERS; ++i) {
      if (GetPowerIndex(Powers(i)) != MAX_POWERS) {
        stmt->SetData(index++,
                      GetUInt32Value(UNIT_FIELD_POWER1 + storedPowers));
        if (++storedPowers >= MAX_POWERS_PER_CLASS)
          break;
      }
    }

    for (; storedPowers < MAX_POWERS_PER_CLASS; ++storedPowers)
      stmt->SetData(index++, 0);

    stmt->SetData(index++, GetSession()->GetLatency());

    stmt->SetData(index++, GetSpecsCount());
    stmt->SetData(index++, GetActiveSpec());

    ss.str("");
    for (uint32 i = 0; i < PLAYER_EXPLORED_ZONES_SIZE; ++i)
      ss << GetUInt32Value(PLAYER_EXPLORED_ZONES_1 + i) << ' ';
    stmt->SetData(index++, ss.str());

    ss.str("");
    // cache equipment...
    for (uint32 i = 0; i < EQUIPMENT_SLOT_END * 2; ++i)
      ss << GetUInt32Value(PLAYER_VISIBLE_ITEM_1_ENTRYID + i) << ' ';

    // ...and bags for enum opcode
    for (uint32 i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; ++i) {
      if (Item *item = GetItemByPos(INVENTORY_SLOT_BAG_0, i))
        ss << item->GetEntry();
      else
        ss << '0';
      ss << " 0 ";
    }

    stmt->SetData(index++, ss.str());

    ss.str("");
    for (uint32 i = 0; i < KNOWN_TITLES_SIZE * 2; ++i)
      ss << GetUInt32Value(PLAYER__FIELD_KNOWN_TITLES + i) << ' ';

    stmt->SetData(index++, ss.str());
    stmt->SetData(index++,
                  GetByteValue(PLAYER_FIELD_BYTES,
                               PLAYER_FIELD_BYTES_OFFSET_ACTION_BAR_TOGGLES));
    stmt->SetData(index++, m_grantableLevels);
    stmt->SetData(index++, GetAchievementPoints());

    stmt->SetData(index++,
                  IsInWorld() && !GetSession()->PlayerLogout() ? 1 : 0);
    // Index
    stmt->SetData(index++, GetGUID().GetCounter());
  }

  trans->Append(stmt);

  if (m_fishingSteps != 0) {
    stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_CHAR_FISHINGSTEPS);
    index = 0;
    stmt->SetData(index++, GetGUID().GetCounter());
    stmt->SetData(index++, m_fishingSteps);
    trans->Append(stmt);
  }

  if (m_mailsUpdated) // save mails only when needed
    _SaveMail(trans);

  _SaveBGData(trans);
  _SaveInventory(trans);
  _SaveVoidStorage(trans);
  _SaveQuestStatus(trans);
  _SaveDailyQuestStatus(trans);
  _SaveWeeklyQuestStatus(trans);
  _SaveSeasonalQuestStatus(trans);
  _SaveMonthlyQuestStatus(trans);
  _SaveLFGRewardStatus(trans);
  _SaveTalents(trans);
  _SaveSpells(trans);
  GetSpellHistory()->SaveToDB<Player>(trans);
  _SaveActions(trans);
  _SaveAuras(trans);
  _SaveSkills(trans);
  m_achievementMgr->SaveToDB(trans);
  m_reputationMgr->SaveToDB(trans);
  _SaveEquipmentSets(trans);
  GetSession()->SaveTutorialsData(
      trans); // changed only while character in game
  _SaveGlyphs(trans);
  _SaveInstanceTimeRestrictions(trans);
  _SaveCurrency(trans);
  _SaveCUFProfiles(trans);

  // check if stats should only be saved on logout
  // save stats can be out of transaction
  if (m_session->isLogingOut() ||
      !sWorld->getBoolConfig(CONFIG_STATS_SAVE_ONLY_ON_LOGOUT))
    _SaveStats(trans);

  // save pet (hunter pet level and experience and all type pets health/mana).
  if (Pet *pet = GetPet())
    pet->SavePetToDB(PET_SAVE_CURRENT_STATE);
}

// fast save function for item/money cheating preventing - save only inventory
// and money state
void Player::SaveInventoryAndGoldToDB(CharacterDatabaseTransaction &trans) {
  _SaveInventory(trans);
  _SaveCurrency(trans);
  SaveGoldToDB(trans);
}

void Player::SaveGoldToDB(CharacterDatabaseTransaction &trans) const {
  CharacterDatabasePreparedStatement *stmt =
      CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_MONEY);
  stmt->SetData(0, GetMoney());
  stmt->SetData(1, GetGUID().GetCounter());
  trans->Append(stmt);
}

void Player::_SaveActions(CharacterDatabaseTransaction &trans) {
  CharacterDatabasePreparedStatement *stmt;

  for (ActionButtonList::iterator itr = m_actionButtons.begin();
       itr != m_actionButtons.end();) {
    switch (itr->second.uState) {
    case ACTIONBUTTON_NEW:
      stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_CHAR_ACTION);
      stmt->SetData(0, GetGUID().GetCounter());
      stmt->SetData(1, GetActiveSpec());
      stmt->SetData(2, itr->first);
      stmt->SetData(3, itr->second.GetAction());
      stmt->SetData(4, uint8(itr->second.GetType()));
      trans->Append(stmt);

      itr->second.uState = ACTIONBUTTON_UNCHANGED;
      ++itr;
      break;
    case ACTIONBUTTON_CHANGED:
      stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_ACTION);
      stmt->SetData(0, itr->second.GetAction());
      stmt->SetData(1, uint8(itr->second.GetType()));
      stmt->SetData(2, GetGUID().GetCounter());
      stmt->SetData(3, itr->first);
      stmt->SetData(4, GetActiveSpec());
      trans->Append(stmt);

      itr->second.uState = ACTIONBUTTON_UNCHANGED;
      ++itr;
      break;
    case ACTIONBUTTON_DELETED:
      stmt = CharacterDatabase.GetPreparedStatement(
          CHAR_DEL_CHAR_ACTION_BY_BUTTON_SPEC);
      stmt->SetData(0, GetGUID().GetCounter());
      stmt->SetData(1, itr->first);
      stmt->SetData(2, GetActiveSpec());
      trans->Append(stmt);

      m_actionButtons.erase(itr++);
      break;
    default:
      ++itr;
      break;
    }
  }
}

void Player::_SaveAuras(CharacterDatabaseTransaction &trans) {
  CharacterDatabasePreparedStatement *stmt =
      CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_AURA);
  stmt->SetData(0, GetGUID().GetCounter());
  trans->Append(stmt);

  for (AuraMap::const_iterator itr = m_ownedAuras.begin();
       itr != m_ownedAuras.end(); ++itr) {
    if (!itr->second->CanBeSaved())
      continue;

    Aura *aura = itr->second;

    int32 damage[MAX_SPELL_EFFECTS];
    int32 baseDamage[MAX_SPELL_EFFECTS];
    uint8 effMask = 0;
    uint8 recalculateMask = 0;
    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i) {
      if (AuraEffect const *effect = aura->GetEffect(i)) {
        baseDamage[i] = effect->GetBaseAmount();
        damage[i] = effect->GetAmount();
        effMask |= 1 << i;
        if (effect->CanBeRecalculated())
          recalculateMask |= 1 << i;
      } else {
        baseDamage[i] = 0;
        damage[i] = 0;
      }
    }

    uint8 index = 0;
    stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_AURA);
    stmt->SetData(index++, GetGUID().GetCounter());
    stmt->SetData(index++, itr->second->GetCasterGUID().GetRawValue());
    stmt->SetData(index++, itr->second->GetId());
    stmt->SetData(index++, effMask);
    stmt->SetData(index++, recalculateMask);
    stmt->SetData(index++, itr->second->GetStackAmount());
    stmt->SetData(index++, damage[0]);
    stmt->SetData(index++, damage[1]);
    stmt->SetData(index++, damage[2]);
    stmt->SetData(index++, baseDamage[0]);
    stmt->SetData(index++, baseDamage[1]);
    stmt->SetData(index++, baseDamage[2]);
    stmt->SetData(index++, itr->second->GetMaxDuration());
    stmt->SetData(index++, itr->second->GetDuration());
    stmt->SetData(index, itr->second->GetCharges());
    trans->Append(stmt);
  }
}

void Player::_SaveInventory(CharacterDatabaseTransaction &trans) {
  CharacterDatabasePreparedStatement *stmt;
  // force items in buyback slots to new state
  // and remove those that aren't already
  for (uint8 i = BUYBACK_SLOT_START; i < BUYBACK_SLOT_END; ++i) {
    Item *item = m_items[i];
    if (!item)
      continue;

    if (item->GetState() == ITEM_NEW) {
      if (ItemTemplate const *itemTemplate = item->GetTemplate())
        if (itemTemplate->GetFlags() & ITEM_FLAG_HAS_LOOT)
          sLootItemStorage->RemoveStoredLootForContainer(
              item->GetGUID().GetCounter());

      continue;
    }

    stmt =
        CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_INVENTORY_BY_ITEM);
    stmt->SetData(0, item->GetGUID().GetCounter());
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_ITEM_INSTANCE);
    stmt->SetData(0, item->GetGUID().GetCounter());
    trans->Append(stmt);
    m_items[i]->FSetState(ITEM_NEW);

    if (ItemTemplate const *itemTemplate = item->GetTemplate())
      if (itemTemplate->GetFlags() & ITEM_FLAG_HAS_LOOT)
        sLootItemStorage->RemoveStoredLootForContainer(
            item->GetGUID().GetCounter());
  }

  // Updated played time for refundable items. We don't do this in
  // Player::Update because there's simply no need for it, the client auto
  // counts down in real time after having received the initial played time on
  // the first SMSG_ITEM_REFUND_INFO_RESPONSE packet. Item::UpdatePlayedTime is
  // only called when needed, which is in DB saves, and item refund info
  // requests.
  GuidSet::iterator i_next;
  for (GuidSet::iterator itr = m_refundableItems.begin();
       itr != m_refundableItems.end(); itr = i_next) {
    // use copy iterator because itr may be invalid after operations in this
    // loop
    i_next = itr;
    ++i_next;

    Item *iPtr = GetItemByGuid(*itr);
    if (iPtr) {
      iPtr->UpdatePlayedTime(this);
      continue;
    } else {
      LOG_ERROR("entities.player",
                "Player::_SaveInventory: Can't find item (%s) in refundable "
                "storage for player '%s' (%s), removing.",
                itr->ToString().c_str(), GetName().c_str(),
                GetGUID().ToString().c_str());
      m_refundableItems.erase(itr);
    }
  }

  // update enchantment durations
  for (EnchantDurationList::iterator itr = m_enchantDuration.begin();
       itr != m_enchantDuration.end(); ++itr)
    itr->item->SetEnchantmentDuration(itr->slot, itr->leftduration, this);

  // if no changes
  if (m_itemUpdateQueue.empty())
    return;

  ObjectGuid::LowType lowGuid = GetGUID().GetCounter();
  for (size_t i = 0; i < m_itemUpdateQueue.size(); ++i) {
    Item *item = m_itemUpdateQueue[i];
    if (!item)
      continue;

    Bag *container = item->GetContainer();
    ObjectGuid::LowType bag_guid =
        container ? container->GetGUID().GetCounter() : 0;

    if (item->GetState() != ITEM_REMOVED) {
      Item *test = GetItemByPos(item->GetBagSlot(), item->GetSlot());
      if (test == nullptr) {
        ObjectGuid::LowType bagTestGUID = 0;
        if (Item *test2 =
                GetItemByPos(INVENTORY_SLOT_BAG_0, item->GetBagSlot()))
          bagTestGUID = test2->GetGUID().GetCounter();

        LOG_ERROR("entities.player",
                  "Player::_SaveInventory: Player '%s' (%s) has incorrect "
                  "values (Bag: %u, Slot: %u) for the item (%s, State: %d). "
                  "The player doesn't have an item at that position.",
                  GetName().c_str(), GetGUID().ToString().c_str(),
                  item->GetBagSlot(), item->GetSlot(),
                  item->GetGUID().ToString().c_str(), (int32)item->GetState());
        // according to the test that was just performed nothing should be in
        // this slot, delete
        stmt = CharacterDatabase.GetPreparedStatement(
            CHAR_DEL_CHAR_INVENTORY_BY_BAG_SLOT);
        stmt->SetData(0, bagTestGUID);
        stmt->SetData(1, item->GetSlot());
        stmt->SetData(2, lowGuid);
        trans->Append(stmt);

        RemoveTradeableItem(item);
        RemoveEnchantmentDurationsReferences(item);
        RemoveItemDurations(item);

        // also THIS item should be somewhere else, cheat attempt
        item->FSetState(ITEM_REMOVED); // we are IN updateQueue right now, can't
                                       // use SetState which modifies the queue
        DeleteRefundReference(item->GetGUID());
        // don't skip, let the switch delete it
        // continue;
      } else if (test != item) {
        LOG_ERROR(
            "entities.player",
            "Player::_SaveInventory: Player '%s' (%s) has incorrect values "
            "(Bag: %u, Slot: %u) for the item (%s). %s is there instead!",
            GetName().c_str(), GetGUID().ToString().c_str(), item->GetBagSlot(),
            item->GetSlot(), item->GetGUID().ToString().c_str(),
            test->GetGUID().ToString().c_str());
        // save all changes to the item...
        if (item->GetState() !=
            ITEM_NEW) // only for existing items, no duplicates
          item->SaveToDB(trans);
        // ...but do not save position in inventory
        continue;
      }
    }

    switch (item->GetState()) {
    case ITEM_NEW:
    case ITEM_CHANGED:
      stmt = CharacterDatabase.GetPreparedStatement(CHAR_REP_INVENTORY_ITEM);
      stmt->SetData(0, lowGuid);
      stmt->SetData(1, bag_guid);
      stmt->SetData(2, item->GetSlot());
      stmt->SetData(3, item->GetGUID().GetCounter());
      trans->Append(stmt);
      break;
    case ITEM_REMOVED:
      stmt = CharacterDatabase.GetPreparedStatement(
          CHAR_DEL_CHAR_INVENTORY_BY_ITEM);
      stmt->SetData(0, item->GetGUID().GetCounter());
      trans->Append(stmt);
      break;
    case ITEM_UNCHANGED:
      break;
    }

    item->SaveToDB(trans); // item have unchanged inventory record and can be
                           // save standalone
  }
  m_itemUpdateQueue.clear();
}

void Player::_SaveVoidStorage(CharacterDatabaseTransaction &trans) {
  CharacterDatabasePreparedStatement *stmt = nullptr;
  uint32 lowGuid = GetGUID().GetCounter();

  for (uint8 i = 0; i < VOID_STORAGE_MAX_SLOT; ++i) {
    if (!_voidStorageItems[i]) // unused item
    {
      // DELETE FROM void_storage WHERE slot = ? AND playerGuid = ?
      stmt = CharacterDatabase.GetPreparedStatement(
          CHAR_DEL_CHAR_VOID_STORAGE_ITEM_BY_SLOT);
      stmt->SetData(0, i);
      stmt->SetData(1, lowGuid);
    } else {
      // REPLACE INTO character_inventory (itemId, playerGuid, itemEntry, slot,
      // creatorGuid) VALUES (?, ?, ?, ?, ?)
      stmt = CharacterDatabase.GetPreparedStatement(
          CHAR_REP_CHAR_VOID_STORAGE_ITEM);
      stmt->SetData(0, _voidStorageItems[i]->ItemId);
      stmt->SetData(1, lowGuid);
      stmt->SetData(2, _voidStorageItems[i]->ItemEntry);
      stmt->SetData(3, i);
      stmt->SetData(4, _voidStorageItems[i]->CreatorGuid.GetCounter());
      stmt->SetData(5, uint8(_voidStorageItems[i]->ItemRandomPropertyId.Type));
      stmt->SetData(6, _voidStorageItems[i]->ItemRandomPropertyId.Id);
      stmt->SetData(7, _voidStorageItems[i]->ItemSuffixFactor);
    }

    trans->Append(stmt);
  }
}

void Player::_SaveCUFProfiles(CharacterDatabaseTransaction &trans) {
  CharacterDatabasePreparedStatement *stmt = nullptr;
  uint32 lowGuid = GetGUID().GetCounter();

  for (uint8 i = 0; i < MAX_CUF_PROFILES; ++i) {
    if (!_CUFProfiles[i]) // unused profile
    {
      // DELETE FROM character_cuf_profiles WHERE guid = ? and id = ?
      stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_CUF_PROFILES);
      stmt->SetData(0, lowGuid);
      stmt->SetData(1, i);
    } else {
      // REPLACE INTO character_cuf_profiles (guid, id, name, frameHeight,
      // frameWidth, sortBy, healthText, boolOptions, unk146, unk147, unk148,
      // unk150, unk152, unk154) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?,
      // ?)
      stmt = CharacterDatabase.GetPreparedStatement(CHAR_REP_CHAR_CUF_PROFILES);
      stmt->SetData(0, lowGuid);
      stmt->SetData(1, i);
      stmt->SetData(2, _CUFProfiles[i]->ProfileName);
      stmt->SetData(3, _CUFProfiles[i]->FrameHeight);
      stmt->SetData(4, _CUFProfiles[i]->FrameWidth);
      stmt->SetData(5, _CUFProfiles[i]->SortBy);
      stmt->SetData(6, _CUFProfiles[i]->HealthText);
      stmt->SetData(
          7,
          static_cast<int>(
              _CUFProfiles[i]->BoolOptions.to_ulong())); // 27 of 32 fields
                                                         // used, fits in an int
      stmt->SetData(8, _CUFProfiles[i]->TopPoint);
      stmt->SetData(9, _CUFProfiles[i]->BottomPoint);
      stmt->SetData(10, _CUFProfiles[i]->LeftPoint);
      stmt->SetData(11, _CUFProfiles[i]->TopOffset);
      stmt->SetData(12, _CUFProfiles[i]->BottomOffset);
      stmt->SetData(13, _CUFProfiles[i]->LeftOffset);
    }

    trans->Append(stmt);
  }
}

void Player::_SaveMail(CharacterDatabaseTransaction &trans) {
  CharacterDatabasePreparedStatement *stmt;

  for (PlayerMails::iterator itr = m_mail.begin(); itr != m_mail.end(); ++itr) {
    Mail *m = (*itr);
    if (m->state == MAIL_STATE_CHANGED) {
      stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_MAIL);
      stmt->SetData(0, uint8(m->HasItems() ? 1 : 0));
      stmt->SetData(1, uint32(m->expire_time));
      stmt->SetData(2, uint32(m->deliver_time));
      stmt->SetData(3, m->money);
      stmt->SetData(4, m->COD);
      stmt->SetData(5, uint8(m->checked));
      stmt->SetData(6, m->messageID);

      trans->Append(stmt);

      if (!m->removedItems.empty()) {
        for (std::vector<uint32>::iterator itr2 = m->removedItems.begin();
             itr2 != m->removedItems.end(); ++itr2) {
          stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_MAIL_ITEM);
          stmt->SetData(0, *itr2);
          trans->Append(stmt);
        }
        m->removedItems.clear();
      }
      m->state = MAIL_STATE_UNCHANGED;
    } else if (m->state == MAIL_STATE_DELETED) {
      if (m->HasItems()) {
        for (MailItemInfoVec::iterator itr2 = m->items.begin();
             itr2 != m->items.end(); ++itr2) {
          stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_ITEM_INSTANCE);
          stmt->SetData(0, itr2->item_guid);
          trans->Append(stmt);
        }
      }
      stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_MAIL_BY_ID);
      stmt->SetData(0, m->messageID);
      trans->Append(stmt);

      stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_MAIL_ITEM_BY_ID);
      stmt->SetData(0, m->messageID);
      trans->Append(stmt);
    }
  }

  // deallocate deleted mails...
  for (PlayerMails::iterator itr = m_mail.begin(); itr != m_mail.end();) {
    if ((*itr)->state == MAIL_STATE_DELETED) {
      Mail *m = *itr;
      m_mail.erase(itr);
      delete m;
      itr = m_mail.begin();
    } else
      ++itr;
  }

  m_mailsUpdated = false;
}

void Player::_SaveQuestStatus(CharacterDatabaseTransaction &trans) {
  bool isTransaction = bool(trans);
  if (!isTransaction)
    trans = CharacterDatabase.BeginTransaction();

  QuestStatusSaveMap::iterator saveItr;
  QuestStatusMap::iterator statusItr;
  CharacterDatabasePreparedStatement *stmt;

  bool keepAbandoned = !(sWorld->GetCleaningFlags() &
                         CharacterDatabaseCleaner::CLEANING_FLAG_QUESTSTATUS);

  for (saveItr = m_QuestStatusSave.begin(); saveItr != m_QuestStatusSave.end();
       ++saveItr) {
    if (saveItr->second == QUEST_DEFAULT_SAVE_TYPE) {
      statusItr = m_QuestStatus.find(saveItr->first);
      if (statusItr != m_QuestStatus.end() &&
          (keepAbandoned || statusItr->second.Status != QUEST_STATUS_NONE)) {
        uint8 index = 0;
        stmt =
            CharacterDatabase.GetPreparedStatement(CHAR_REP_CHAR_QUESTSTATUS);

        stmt->SetData(index++, GetGUID().GetCounter());
        stmt->SetData(index++, statusItr->first);
        stmt->SetData(index++, uint8(statusItr->second.Status));
        stmt->SetData(index++, statusItr->second.Explored);
        stmt->SetData(index++,
                      uint32(statusItr->second.Timer / IN_MILLISECONDS +
                             GameTime::GetGameTime()));

        for (uint8 i = 0; i < QUEST_OBJECTIVES_COUNT; i++)
          stmt->SetData(index++, statusItr->second.CreatureOrGOCount[i]);

        for (uint8 i = 0; i < QUEST_ITEM_OBJECTIVES_COUNT; i++)
          stmt->SetData(index++, statusItr->second.ItemCount[i]);

        stmt->SetData(index, statusItr->second.PlayerCount);
        trans->Append(stmt);
      }
    } else {
      stmt = CharacterDatabase.GetPreparedStatement(
          CHAR_DEL_CHAR_QUESTSTATUS_BY_QUEST);
      stmt->SetData(0, GetGUID().GetCounter());
      stmt->SetData(1, saveItr->first);
      trans->Append(stmt);
    }
  }

  m_QuestStatusSave.clear();

  for (saveItr = m_RewardedQuestsSave.begin();
       saveItr != m_RewardedQuestsSave.end(); ++saveItr) {
    if (saveItr->second == QUEST_DEFAULT_SAVE_TYPE) {
      stmt = CharacterDatabase.GetPreparedStatement(
          CHAR_INS_CHAR_QUESTSTATUS_REWARDED);
      stmt->SetData(0, GetGUID().GetCounter());
      stmt->SetData(1, saveItr->first);
      trans->Append(stmt);

    } else if (saveItr->second == QUEST_FORCE_DELETE_SAVE_TYPE ||
               !keepAbandoned) {
      stmt = CharacterDatabase.GetPreparedStatement(
          CHAR_DEL_CHAR_QUESTSTATUS_REWARDED_BY_QUEST);
      stmt->SetData(0, GetGUID().GetCounter());
      stmt->SetData(1, saveItr->first);
      trans->Append(stmt);
    }
  }

  m_RewardedQuestsSave.clear();

  if (!isTransaction)
    CharacterDatabase.CommitTransaction(trans);
}

void Player::_SaveDailyQuestStatus(CharacterDatabaseTransaction &trans) {
  if (!m_DailyQuestChanged)
    return;

  m_DailyQuestChanged = false;

  // save last daily quest time for all quests: we need only mostly reset time
  // for reset check anyway

  // we don't need transactions here.
  CharacterDatabasePreparedStatement *stmt =
      CharacterDatabase.GetPreparedStatement(
          CHAR_DEL_CHARACTER_QUESTSTATUS_DAILY);
  stmt->SetData(0, GetGUID().GetCounter());
  trans->Append(stmt);

  for (uint32 quest_daily_idx = 0; quest_daily_idx < PLAYER_MAX_DAILY_QUESTS;
       ++quest_daily_idx) {
    if (GetUInt32Value(PLAYER_FIELD_DAILY_QUESTS_1 + quest_daily_idx)) {
      stmt = CharacterDatabase.GetPreparedStatement(
          CHAR_INS_CHARACTER_QUESTSTATUS_DAILY);
      stmt->SetData(0, GetGUID().GetCounter());
      stmt->SetData(
          1, GetUInt32Value(PLAYER_FIELD_DAILY_QUESTS_1 + quest_daily_idx));
      stmt->SetData(2, uint64(m_lastDailyQuestTime));
      trans->Append(stmt);
    }
  }
}

void Player::_SaveWeeklyQuestStatus(CharacterDatabaseTransaction &trans) {
  if (!m_WeeklyQuestChanged || m_weeklyquests.empty())
    return;

  // we don't need transactions here.
  CharacterDatabasePreparedStatement *stmt =
      CharacterDatabase.GetPreparedStatement(
          CHAR_DEL_CHARACTER_QUESTSTATUS_WEEKLY);
  stmt->SetData(0, GetGUID().GetCounter());
  trans->Append(stmt);

  for (QuestSet::const_iterator iter = m_weeklyquests.begin();
       iter != m_weeklyquests.end(); ++iter) {
    uint32 questId = *iter;

    stmt = CharacterDatabase.GetPreparedStatement(
        CHAR_INS_CHARACTER_QUESTSTATUS_WEEKLY);
    stmt->SetData(0, GetGUID().GetCounter());
    stmt->SetData(1, questId);
    trans->Append(stmt);
  }

  m_WeeklyQuestChanged = false;
}

void Player::_SaveSeasonalQuestStatus(CharacterDatabaseTransaction &trans) {
  if (!m_SeasonalQuestChanged)
    return;

  // we don't need transactions here.
  CharacterDatabasePreparedStatement *stmt =
      CharacterDatabase.GetPreparedStatement(
          CHAR_DEL_CHARACTER_QUESTSTATUS_SEASONAL);
  stmt->SetData(0, GetGUID().GetCounter());
  trans->Append(stmt);

  m_SeasonalQuestChanged = false;

  if (m_seasonalquests.empty())
    return;

  for (SeasonalEventQuestMap::const_iterator iter = m_seasonalquests.begin();
       iter != m_seasonalquests.end(); ++iter) {
    uint16 eventId = iter->first;

    for (SeasonalQuestSet::const_iterator itr = iter->second.begin();
         itr != iter->second.end(); ++itr) {
      uint32 questId = *itr;

      stmt = CharacterDatabase.GetPreparedStatement(
          CHAR_INS_CHARACTER_QUESTSTATUS_SEASONAL);
      stmt->SetData(0, GetGUID().GetCounter());
      stmt->SetData(1, questId);
      stmt->SetData(2, eventId);
      trans->Append(stmt);
    }
  }
}

void Player::_SaveMonthlyQuestStatus(CharacterDatabaseTransaction &trans) {
  if (!m_MonthlyQuestChanged || m_monthlyquests.empty())
    return;

  // we don't need transactions here.
  CharacterDatabasePreparedStatement *stmt =
      CharacterDatabase.GetPreparedStatement(
          CHAR_DEL_CHARACTER_QUESTSTATUS_MONTHLY);
  stmt->SetData(0, GetGUID().GetCounter());
  trans->Append(stmt);

  for (QuestSet::const_iterator iter = m_monthlyquests.begin();
       iter != m_monthlyquests.end(); ++iter) {
    uint32 questId = *iter;

    stmt = CharacterDatabase.GetPreparedStatement(
        CHAR_INS_CHARACTER_QUESTSTATUS_MONTHLY);
    stmt->SetData(0, GetGUID().GetCounter());
    stmt->SetData(1, questId);
    trans->Append(stmt);
  }

  m_MonthlyQuestChanged = false;
}

void Player::_SaveLFGRewardStatus(CharacterDatabaseTransaction &trans) {
  if (!m_LFGRewardStatusChanged || m_lfgrewardstatus.empty())
    return;

  // we don't need transactions here.
  CharacterDatabasePreparedStatement *stmt =
      CharacterDatabase.GetPreparedStatement(
          CHAR_DEL_CHARACTER_REWARDSTATUS_LFG);
  stmt->SetData(0, GetGUID().GetCounter());
  trans->Append(stmt);

  for (LFGRewardStatusMap::const_iterator itr = m_lfgrewardstatus.begin();
       itr != m_lfgrewardstatus.end(); ++itr) {
    uint32 dungeonId = itr->first;
    uint8 rewardCount = itr->second.CompletionsThisPeriod;
    uint8 dailyReset = itr->second.IsDaily;

    stmt = CharacterDatabase.GetPreparedStatement(
        CHAR_INS_CHARACTER_REWARDSTATUS_LFG);
    stmt->SetData(0, GetGUID().GetCounter());
    stmt->SetData(1, dungeonId);
    stmt->SetData(2, rewardCount);
    stmt->SetData(3, dailyReset);
    trans->Append(stmt);
  }

  m_LFGRewardStatusChanged = false;
}

void Player::_SaveSkills(CharacterDatabaseTransaction &trans) {
  CharacterDatabasePreparedStatement *stmt;
  // we don't need transactions here.
  for (SkillStatusMap::iterator itr = mSkillStatus.begin();
       itr != mSkillStatus.end();) {
    if (itr->second.uState == SKILL_UNCHANGED) {
      ++itr;
      continue;
    }

    uint16 field = itr->second.pos / 2;
    uint8 offset = itr->second.pos & 1;

    uint16 value = GetUInt16Value(PLAYER_SKILL_RANK_0 + field, offset);
    uint16 max = GetUInt16Value(PLAYER_SKILL_MAX_RANK_0 + field, offset);

    switch (itr->second.uState) {
    case SKILL_NEW:
      stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_CHAR_SKILLS);
      stmt->SetData(0, GetGUID().GetCounter());
      stmt->SetData(1, uint16(itr->first));
      stmt->SetData(2, value);
      stmt->SetData(3, max);
      trans->Append(stmt);
      break;
    case SKILL_CHANGED:
      stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_SKILLS);
      stmt->SetData(0, value);
      stmt->SetData(1, max);
      stmt->SetData(2, GetGUID().GetCounter());
      stmt->SetData(3, uint16(itr->first));
      trans->Append(stmt);
      break;
    case SKILL_DELETED:
      stmt =
          CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_SKILL_BY_SKILL);
      stmt->SetData(0, GetGUID().GetCounter());
      stmt->SetData(1, uint16(itr->first));
      trans->Append(stmt);
      break;
    default:
      break;
    }

    itr->second.uState = SKILL_UNCHANGED;
    ++itr;
  }
}

void Player::_SaveSpells(CharacterDatabaseTransaction &trans) {
  CharacterDatabasePreparedStatement *stmt;

  for (PlayerSpellMap::iterator itr = m_spells.begin();
       itr != m_spells.end();) {
    if (itr->second.state == PLAYERSPELL_REMOVED ||
        itr->second.state == PLAYERSPELL_CHANGED) {
      stmt =
          CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_SPELL_BY_SPELL);
      stmt->SetData(0, itr->first);
      stmt->SetData(1, GetGUID().GetCounter());
      trans->Append(stmt);
    }

    // add only changed/new not dependent spells
    if (!itr->second.dependent && (itr->second.state == PLAYERSPELL_NEW ||
                                   itr->second.state == PLAYERSPELL_CHANGED)) {
      stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_CHAR_SPELL);
      stmt->SetData(0, GetGUID().GetCounter());
      stmt->SetData(1, itr->first);
      stmt->SetData(2, itr->second.active);
      stmt->SetData(3, itr->second.disabled);
      trans->Append(stmt);
    }

    if (itr->second.state == PLAYERSPELL_REMOVED) {
      itr = m_spells.erase(itr);
      continue;
    }

    if (itr->second.state != PLAYERSPELL_TEMPORARY)
      itr->second.state = PLAYERSPELL_UNCHANGED;

    ++itr;
  }
}

// save player stats -- only for external usage
// real stats will be recalculated on player login
void Player::_SaveStats(CharacterDatabaseTransaction &trans) const {
  // check if stat saving is enabled and if char level is high enough
  if (!sWorld->getIntConfig(CONFIG_MIN_LEVEL_STAT_SAVE) ||
      getLevel() < sWorld->getIntConfig(CONFIG_MIN_LEVEL_STAT_SAVE))
    return;

  CharacterDatabasePreparedStatement *stmt;

  stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_STATS);
  stmt->SetData(0, GetGUID().GetCounter());
  trans->Append(stmt);

  uint8 index = 0;

  stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_CHAR_STATS);
  stmt->SetData(index++, GetGUID().GetCounter());
  stmt->SetData(index++, GetMaxHealth());

  for (uint8 i = 0; i < MAX_POWERS_PER_CLASS; ++i)
    stmt->SetData(index++, GetMaxPower(Powers(i)));

  for (uint8 i = 0; i < MAX_STATS; ++i)
    stmt->SetData(index++, GetStat(Stats(i)));

  for (int i = 0; i < MAX_SPELL_SCHOOL; ++i)
    stmt->SetData(index++, GetResistance(SpellSchools(i)));

  stmt->SetData(index++, GetFloatValue(PLAYER_BLOCK_PERCENTAGE));
  stmt->SetData(index++, GetFloatValue(PLAYER_DODGE_PERCENTAGE));
  stmt->SetData(index++, GetFloatValue(PLAYER_PARRY_PERCENTAGE));
  stmt->SetData(index++, GetFloatValue(PLAYER_CRIT_PERCENTAGE));
  stmt->SetData(index++, GetFloatValue(PLAYER_RANGED_CRIT_PERCENTAGE));
  stmt->SetData(index++, GetFloatValue(PLAYER_SPELL_CRIT_PERCENTAGE1));
  stmt->SetData(index++, GetUInt32Value(UNIT_FIELD_ATTACK_POWER));
  stmt->SetData(index++, GetUInt32Value(UNIT_FIELD_RANGED_ATTACK_POWER));
  stmt->SetData(index++, GetBaseSpellPowerBonus());
  stmt->SetData(index++, GetUInt32Value(PLAYER_FIELD_COMBAT_RATING_1 +
                                        CR_RESILIENCE_PLAYER_DAMAGE_TAKEN));

  trans->Append(stmt);
}

void Player::outDebugValues() const {
  if (!sLog->ShouldLog("entities.unit", LOG_LEVEL_DEBUG))
    return;

  LOG_DEBUG("entities.unit", "HP is: \t\t\t%u\t\tMP is: \t\t\t%u",
            GetMaxHealth(), GetMaxPower(POWER_MANA));
  LOG_DEBUG("entities.unit", "AGILITY is: \t\t%f\t\tSTRENGTH is: \t\t%f",
            GetStat(STAT_AGILITY), GetStat(STAT_STRENGTH));
  LOG_DEBUG("entities.unit", "INTELLECT is: \t\t%f\t\tSPIRIT is: \t\t%f",
            GetStat(STAT_INTELLECT), GetStat(STAT_SPIRIT));
  LOG_DEBUG("entities.unit", "STAMINA is: \t\t%f", GetStat(STAT_STAMINA));
  LOG_DEBUG("entities.unit", "Armor is: \t\t%u\t\tBlock is: \t\t%f", GetArmor(),
            GetFloatValue(PLAYER_BLOCK_PERCENTAGE));
  LOG_DEBUG("entities.unit", "HolyRes is: \t\t%u\t\tFireRes is: \t\t%u",
            GetResistance(SPELL_SCHOOL_HOLY), GetResistance(SPELL_SCHOOL_FIRE));
  LOG_DEBUG("entities.unit", "NatureRes is: \t\t%u\t\tFrostRes is: \t\t%u",
            GetResistance(SPELL_SCHOOL_NATURE),
            GetResistance(SPELL_SCHOOL_FROST));
  LOG_DEBUG("entities.unit", "ShadowRes is: \t\t%u\t\tArcaneRes is: \t\t%u",
            GetResistance(SPELL_SCHOOL_SHADOW),
            GetResistance(SPELL_SCHOOL_ARCANE));
  LOG_DEBUG("entities.unit", "MIN_DAMAGE is: \t\t%f\tMAX_DAMAGE is: \t\t%f",
            GetFloatValue(UNIT_FIELD_MINDAMAGE),
            GetFloatValue(UNIT_FIELD_MAXDAMAGE));
  LOG_DEBUG("entities.unit",
            "MIN_OFFHAND_DAMAGE is: \t%f\tMAX_OFFHAND_DAMAGE is: \t%f",
            GetFloatValue(UNIT_FIELD_MINOFFHANDDAMAGE),
            GetFloatValue(UNIT_FIELD_MAXOFFHANDDAMAGE));
  LOG_DEBUG("entities.unit",
            "MIN_RANGED_DAMAGE is: \t%f\tMAX_RANGED_DAMAGE is: \t%f",
            GetFloatValue(UNIT_FIELD_MINRANGEDDAMAGE),
            GetFloatValue(UNIT_FIELD_MAXRANGEDDAMAGE));
  LOG_DEBUG("entities.unit",
            "ATTACK_TIME is: \t%u\t\tRANGE_ATTACK_TIME is: \t%u",
            GetAttackTime(BASE_ATTACK), GetAttackTime(RANGED_ATTACK));
}

/*********************************************************/
/***               FLOOD FILTER SYSTEM                 ***/
/*********************************************************/

void Player::UpdateSpeakTime() {
  // ignore chat spam protection for GMs in any mode
  if (GetSession()->HasPermission(rbac::RBAC_PERM_SKIP_CHECK_CHAT_SPAM))
    return;

  time_t current = GameTime::GetGameTime();
  if (m_speakTime > current) {
    uint32 max_count = sWorld->getIntConfig(CONFIG_CHATFLOOD_MESSAGE_COUNT);
    if (!max_count)
      return;

    ++m_speakCount;
    if (m_speakCount >= max_count) {
      // prevent overwrite mute time, if message send just before mutes set, for
      // example.
      time_t new_mute =
          current + sWorld->getIntConfig(CONFIG_CHATFLOOD_MUTE_TIME);
      if (GetSession()->m_muteTime < new_mute)
        GetSession()->m_muteTime = new_mute;

      m_speakCount = 0;
    }
  } else
    m_speakCount = 1;

  m_speakTime = current + sWorld->getIntConfig(CONFIG_CHATFLOOD_MESSAGE_DELAY);
}

bool Player::CanSpeak() const {
  return GetSession()->m_muteTime <= time(nullptr);
}

/*********************************************************/
/***              LOW LEVEL FUNCTIONS:Notifiers        ***/
/*********************************************************/

void Player::SendAttackSwingNotInRange() const {
  WorldPacket data(SMSG_ATTACKSWING_NOTINRANGE, 0);
  SendDirectMessage(&data);
}

void Player::SavePositionInDB(WorldLocation const &loc, uint16 zoneId,
                              ObjectGuid guid,
                              CharacterDatabaseTransaction &trans) {
  CharacterDatabasePreparedStatement *stmt =
      CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHARACTER_POSITION);

  stmt->SetData(0, loc.GetPositionX());
  stmt->SetData(1, loc.GetPositionY());
  stmt->SetData(2, loc.GetPositionZ());
  stmt->SetData(3, loc.GetOrientation());
  stmt->SetData(4, uint16(loc.GetMapId()));
  stmt->SetData(5, zoneId);
  stmt->SetData(6, guid.GetCounter());

  CharacterDatabase.ExecuteOrAppend(trans, stmt);
}

void Player::Customize(CharacterCustomizeInfo const *customizeInfo,
                       CharacterDatabaseTransaction &trans) {
  CharacterDatabasePreparedStatement *stmt =
      CharacterDatabase.GetPreparedStatement(CHAR_UPD_GENDER_AND_APPEARANCE);

  stmt->SetData(0, customizeInfo->Gender);
  stmt->SetData(1, customizeInfo->Skin);
  stmt->SetData(2, customizeInfo->Face);
  stmt->SetData(3, customizeInfo->HairStyle);
  stmt->SetData(4, customizeInfo->HairColor);
  stmt->SetData(5, customizeInfo->FacialHair);
  stmt->SetData(6, customizeInfo->Guid.GetCounter());

  CharacterDatabase.ExecuteOrAppend(trans, stmt);
}

void Player::SendAttackSwingDeadTarget() const {
  WorldPacket data(SMSG_ATTACKSWING_DEADTARGET, 0);
  SendDirectMessage(&data);
}

void Player::SendAttackSwingCantAttack() const {
  WorldPacket data(SMSG_ATTACKSWING_CANT_ATTACK, 0);
  SendDirectMessage(&data);
}

void Player::SendAttackSwingCancelAttack() const {
  WorldPacket data(SMSG_CANCEL_COMBAT, 0);
  SendDirectMessage(&data);
}

void Player::SendAttackSwingBadFacingAttack() const {
  WorldPacket data(SMSG_ATTACKSWING_BADFACING, 0);
  SendDirectMessage(&data);
}

void Player::SendAutoRepeatCancel(Unit *target) {
  WorldPacket data(SMSG_CANCEL_AUTO_REPEAT, target->GetPackGUID().size());
  data << target->GetPackGUID(); // may be it's target guid
  SendMessageToSet(&data, true);
}

void Player::SendExplorationExperience(uint32 Area, uint32 Experience) const {
  WorldPacket data(SMSG_EXPLORATION_EXPERIENCE, 8);
  data << uint32(Area);
  data << uint32(Experience);
  SendDirectMessage(&data);
}

void Player::SendDungeonDifficulty(bool IsInGroup) const {
  uint8 val = 0x00000001;
  WorldPacket data(MSG_SET_DUNGEON_DIFFICULTY, 12);
  data << (uint32)GetDungeonDifficulty();
  data << uint32(val);
  data << uint32(IsInGroup);
  SendDirectMessage(&data);
}

void Player::SendRaidDifficulty(bool IsInGroup, int32 forcedDifficulty) const {
  uint8 val = 0x00000001;
  WorldPacket data(MSG_SET_RAID_DIFFICULTY, 12);
  data << uint32(forcedDifficulty == -1 ? GetRaidDifficulty()
                                        : forcedDifficulty);
  data << uint32(val);
  data << uint32(IsInGroup);
  SendDirectMessage(&data);
}

void Player::SendResetFailedNotify(uint32 /*mapid*/) const {
  WorldPacket data(SMSG_RESET_FAILED_NOTIFY, 4);
  SendDirectMessage(&data);
}

/// Reset all solo instances and optionally send a message on success for each
void Player::ResetInstances(uint8 method, bool isRaid) {
  // method can be INSTANCE_RESET_ALL, INSTANCE_RESET_CHANGE_DIFFICULTY,
  // INSTANCE_RESET_GROUP_JOIN

  // we assume that when the difficulty changes, all instances that can be reset
  // will be
  Difficulty diff = GetDifficulty(isRaid);

  for (BoundInstancesMap::iterator itr = m_boundInstances[diff].begin();
       itr != m_boundInstances[diff].end();) {
    InstanceSave *p = itr->second.save;
    MapEntry const *entry = sMapStore.LookupEntry(itr->first);
    if (!entry || entry->IsRaid() != isRaid || !p->CanReset()) {
      ++itr;
      continue;
    }

    if (method == INSTANCE_RESET_ALL) {
      // the "reset all instances" method can only reset normal maps
      if (entry->MapType == MAP_RAID || diff == DUNGEON_DIFFICULTY_HEROIC) {
        ++itr;
        continue;
      }
    }

    // if the map is loaded, reset it
    Map *map = sMapMgr->FindMap(p->GetMapId(), p->GetInstanceId());
    if (map && map->IsDungeon())
      if (!map->ToInstanceMap()->Reset(method)) {
        ++itr;
        continue;
      }

    // since this is a solo instance there should not be any players inside
    if (method == INSTANCE_RESET_ALL ||
        method == INSTANCE_RESET_CHANGE_DIFFICULTY)
      SendResetInstanceSuccess(p->GetMapId());

    p->DeleteFromDB();
    m_boundInstances[diff].erase(itr++);

    // the following should remove the instance save from the manager and delete
    // it as well
    p->RemovePlayer(this);
  }
}

void Player::SendResetInstanceSuccess(uint32 MapId) const {
  WorldPacket data(SMSG_INSTANCE_RESET, 4);
  data << uint32(MapId);
  SendDirectMessage(&data);
}

void Player::SendResetInstanceFailed(uint32 reason, uint32 MapId) const {
  /*reasons for instance reset failure:
  // 0: There are players inside the instance.
  // 1: There are players offline in your party.
  // 2>: There are players in your party attempting to zone into an instance.
  */
  WorldPacket data(SMSG_INSTANCE_RESET_FAILED, 8);
  data << uint32(reason);
  data << uint32(MapId);
  SendDirectMessage(&data);
}

/*********************************************************/
/***              Update timers                        ***/
/*********************************************************/

/// checks the 15 afk reports per 5 minutes limit
void Player::UpdateAfkReport(time_t currTime) {
  if (m_bgData.bgAfkReportedTimer <= currTime) {
    m_bgData.bgAfkReportedCount = 0;
    m_bgData.bgAfkReportedTimer = currTime + 5 * MINUTE;
  }
}

void Player::SetContestedPvP(Player *attackedPlayer) {
  if (attackedPlayer &&
      (attackedPlayer == this || (duel && duel->opponent == attackedPlayer)))
    return;

  SetContestedPvPTimer(30000);
  if (!HasUnitState(UNIT_STATE_ATTACK_PLAYER)) {
    AddUnitState(UNIT_STATE_ATTACK_PLAYER);
    SetFlag(PLAYER_FLAGS, PLAYER_FLAGS_CONTESTED_PVP);
    // call MoveInLineOfSight for nearby contested guards
    Firelands::AIRelocationNotifier notifier(*this);
    Cell::VisitWorldObjects(this, notifier, GetVisibilityRange());
  }
  for (Unit *unit : m_Controlled) {
    if (!unit->HasUnitState(UNIT_STATE_ATTACK_PLAYER)) {
      unit->AddUnitState(UNIT_STATE_ATTACK_PLAYER);
      Firelands::AIRelocationNotifier notifier(*unit);
      Cell::VisitWorldObjects(this, notifier, GetVisibilityRange());
    }
  }
}

void Player::UpdateContestedPvP(uint32 diff) {
  if (!m_contestedPvPTimer || IsInCombat())
    return;

  if (m_contestedPvPTimer <= diff)
    ResetContestedPvP();
  else
    m_contestedPvPTimer -= diff;
}

void Player::ResetContestedPvP() {
  ClearUnitState(UNIT_STATE_ATTACK_PLAYER);
  RemoveFlag(PLAYER_FLAGS, PLAYER_FLAGS_CONTESTED_PVP);
  m_contestedPvPTimer = 0;
}

void Player::UpdatePvPFlag(time_t currTime) {
  if (!IsPvP())
    return;

  if (!pvpInfo.EndTimer || currTime < (pvpInfo.EndTimer + 300) ||
      pvpInfo.IsHostile)
    return;

  UpdatePvP(false);
}

void Player::UpdateDuelFlag(time_t currTime) {
  if (!duel || duel->startTimer == 0 || currTime < duel->startTimer + 3)
    return;

  sScriptMgr->OnPlayerDuelStart(this, duel->opponent);

  SetUInt32Value(PLAYER_DUEL_TEAM, 1);
  duel->opponent->SetUInt32Value(PLAYER_DUEL_TEAM, 2);

  duel->startTimer = 0;
  duel->startTime = currTime;
  duel->opponent->duel->startTimer = 0;
  duel->opponent->duel->startTime = currTime;
}

Pet *Player::GetPet() const {
  if (ObjectGuid pet_guid = GetPetGUID()) {
    if (!pet_guid.IsPet())
      return nullptr;

    Pet *pet = ObjectAccessor::GetPet(*this, pet_guid);

    if (!pet)
      return nullptr;

    if (IsInWorld())
      return pet;

    // there may be a guardian in this slot
    // LOG_ERROR("entities.player", "Player::GetPet: Pet %u does not exist.",
    // GUID_LOPART(pet_guid)); const_cast<Player*>(this)->SetPetGUID(0);
  }

  return nullptr;
}

void Player::RemovePet(Pet *pet, PetSaveMode mode, bool returnreagent) {
  if (!pet)
    pet = GetPet();

  if (pet) {
    LOG_DEBUG("entities.pet",
              "Player::RemovePet: Player '%s' (%s), Pet (Entry: %u, Mode: %u, "
              "ReturnReagent: %u)",
              GetName().c_str(), GetGUID().ToString().c_str(), pet->GetEntry(),
              mode, returnreagent);

    if (pet->m_removed)
      return;
  }

  if (returnreagent && (pet || m_temporaryUnsummonedPetNumber) &&
      !InBattleground()) {
    // returning of reagents only for players, so best done here
    uint32 spellId =
        pet ? pet->GetUInt32Value(UNIT_CREATED_BY_SPELL) : m_oldpetspell;
    SpellInfo const *spellInfo = sSpellMgr->GetSpellInfo(spellId);

    if (spellInfo) {
      for (uint32 i = 0; i < MAX_SPELL_REAGENTS; ++i) {
        if (spellInfo->Reagent[i] > 0) {
          ItemPosCountVec dest; // for succubus, voidwalker, felhunter and
                                // felguard credit soulshard when despawn reason
                                // other than death (out of range, logout)
          InventoryResult msg =
              CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, spellInfo->Reagent[i],
                              spellInfo->ReagentCount[i]);
          if (msg == EQUIP_ERR_OK) {
            Item *item = StoreNewItem(dest, spellInfo->Reagent[i], true);
            if (IsInWorld())
              SendNewItem(item, spellInfo->ReagentCount[i], true, false);
          }
        }
      }
    }
    m_temporaryUnsummonedPetNumber = 0;
  }

  if (!pet || pet->GetOwnerOrCreatorGUID() != GetGUID())
    return;

  pet->CombatStop();

  if (returnreagent) {
    switch (pet->GetEntry()) {
      // warlock pets except imp are removed(?) when logging out
    case 1860:
    case 1863:
    case 417:
    case 17252:
      mode = PET_SAVE_DISMISS;
      break;
    }
  }

  pet->SavePetToDB(mode);

  SetMinion(pet, false);

  pet->AddObjectToRemoveList();
  pet->m_removed = true;

  if (pet->isControlled()) {
    WorldPacket data(SMSG_PET_SPELLS, 8);
    data << uint64(0);
    SendDirectMessage(&data);

    if (GetGroup())
      SetGroupUpdateFlag(GROUP_UPDATE_PET);
  }
}

void Player::AddPetAura(PetAura const *petSpell) {
  m_petAuras.insert(petSpell);
  if (Pet *pet = GetPet())
    pet->CastPetAura(petSpell);
}

void Player::RemovePetAura(PetAura const *petSpell) {
  m_petAuras.erase(petSpell);
  if (Pet *pet = GetPet())
    pet->RemoveAurasDueToSpell(petSpell->GetAura(pet->GetEntry()));
}

void Player::StopCastingCharm() {
  Unit *charm = GetCharmed();
  if (!charm)
    return;

  if (charm->GetTypeId() == TYPEID_UNIT) {
    if (charm->ToCreature()->HasUnitTypeMask(UNIT_MASK_PUPPET))
      static_cast<Puppet *>(charm)->UnSummon();
    else if (charm->IsVehicle()) {
      ExitVehicle();

      // Temporary for issue https://github.com/Firelands Core/Firelands
      // Core/issues/24876
      if (!GetCharmedGUID().IsEmpty() &&
          !charm->HasAuraTypeWithCaster(SPELL_AURA_CONTROL_VEHICLE,
                                        GetGUID())) {
        LOG_FATAL(
            "entities.player",
            "Player::StopCastingCharm Player '%s' (%s) is not able to uncharm "
            "vehicle (%s) because of missing SPELL_AURA_CONTROL_VEHICLE",
            GetName().c_str(), GetGUID().ToString().c_str(),
            GetCharmedGUID().ToString().c_str());

        // attempt to recover from missing HandleAuraControlVehicle unapply
        // handling THIS IS A HACK, NEED TO FIND HOW IS IT EVEN POSSBLE TO NOT
        // HAVE THE AURA
        _ExitVehicle();
      }
    }
  }
  if (GetCharmedGUID())
    charm->RemoveCharmAuras();

  if (GetCharmedGUID()) {
    LOG_FATAL("entities.player",
              "Player::StopCastingCharm: Player '%s' (%s) is not able to "
              "uncharm unit (%s)",
              GetName().c_str(), GetGUID().ToString().c_str(),
              GetCharmedGUID().ToString().c_str());
    if (!charm->GetCharmerGUID().IsEmpty()) {
      LOG_FATAL("entities.player",
                "Player::StopCastingCharm: Charmed unit has charmer %s",
                charm->GetCharmerGUID().ToString().c_str());
      ABORT();
    }

    SetCharm(charm, false);
  }
}

void Player::Say(std::string const &text, Language language,
                 WorldObject const * /*= nullptr*/) {
  std::string _text(text);
  sScriptMgr->OnPlayerChat(this, CHAT_MSG_SAY, language, _text);

  WorldPacket data;
  ChatHandler::BuildChatPacket(data, CHAT_MSG_SAY, language, this, this, _text);
  SendMessageToSetInRange(
      &data, sWorld->getFloatConfig(CONFIG_LISTEN_RANGE_SAY), true);
}

void Player::Say(uint32 textId, WorldObject const *target /*= nullptr*/) {
  Talk(textId, CHAT_MSG_SAY, sWorld->getFloatConfig(CONFIG_LISTEN_RANGE_SAY),
       target);
}

void Player::Yell(std::string const &text, Language language,
                  WorldObject const * /*= nullptr*/) {
  std::string _text(text);
  sScriptMgr->OnPlayerChat(this, CHAT_MSG_YELL, language, _text);

  WorldPacket data;
  ChatHandler::BuildChatPacket(data, CHAT_MSG_YELL, language, this, this,
                               _text);
  SendMessageToSetInRange(
      &data, sWorld->getFloatConfig(CONFIG_LISTEN_RANGE_YELL), true);
}

void Player::Yell(uint32 textId, WorldObject const *target /*= nullptr*/) {
  Talk(textId, CHAT_MSG_YELL, sWorld->getFloatConfig(CONFIG_LISTEN_RANGE_YELL),
       target);
}

void Player::TextEmote(std::string const &text,
                       WorldObject const * /*= nullptr*/, bool /*= false*/) {
  std::string _text(text);
  sScriptMgr->OnPlayerChat(this, CHAT_MSG_EMOTE, LANG_UNIVERSAL, _text);

  WorldPacket data;
  ChatHandler::BuildChatPacket(data, CHAT_MSG_EMOTE, LANG_UNIVERSAL, this, this,
                               _text);
  SendMessageToSetInRange(
      &data, sWorld->getFloatConfig(CONFIG_LISTEN_RANGE_TEXTEMOTE), true,
      !GetSession()->HasPermission(rbac::RBAC_PERM_TWO_SIDE_INTERACTION_CHAT));
}

void Player::TextEmote(uint32 textId, WorldObject const *target /*= nullptr*/,
                       bool /*isBossEmote = false*/) {
  Talk(textId, CHAT_MSG_EMOTE,
       sWorld->getFloatConfig(CONFIG_LISTEN_RANGE_TEXTEMOTE), target);
}

void Player::WhisperAddon(std::string const &text, const std::string &prefix,
                          Player *receiver) {
  std::string _text(text);
  sScriptMgr->OnPlayerChat(this, CHAT_MSG_WHISPER, uint32(LANG_ADDON), _text,
                           receiver);

  if (!receiver->GetSession()->IsAddonRegistered(prefix))
    return;

  WorldPacket data;
  ChatHandler::BuildChatPacket(data, CHAT_MSG_WHISPER, LANG_ADDON, this, this,
                               text, 0, "", DEFAULT_LOCALE, prefix);
  receiver->SendDirectMessage(&data);
}

void Player::Whisper(std::string const &text, Language language, Player *target,
                     bool isBossWhisper) {
  ASSERT(target);

  bool isAddonMessage = language == LANG_ADDON;

  if (!isAddonMessage)         // if not addon data
    language = LANG_UNIVERSAL; // whispers should always be readable

  std::string _text(text);
  sScriptMgr->OnPlayerChat(
      this, isBossWhisper ? CHAT_MSG_RAID_BOSS_WHISPER : CHAT_MSG_WHISPER,
      language, _text, target);

  WorldPacket data;
  ChatHandler::BuildChatPacket(
      data, isBossWhisper ? CHAT_MSG_RAID_BOSS_WHISPER : CHAT_MSG_WHISPER,
      Language(language), this, this, _text);
  target->SendDirectMessage(&data);

  // rest stuff shouldn't happen in case of addon message
  if (isAddonMessage)
    return;

  ChatHandler::BuildChatPacket(data, CHAT_MSG_WHISPER_INFORM,
                               Language(language), target, target, _text);
  SendDirectMessage(&data);

  if (!isAcceptWhispers() && !IsGameMaster() && !target->IsGameMaster()) {
    SetAcceptWhispers(true);
    ChatHandler(GetSession()).SendSysMessage(LANG_COMMAND_WHISPERON);
  }

  // announce afk or dnd message
  if (target->isAFK())
    ChatHandler(GetSession())
        .PSendSysMessage(LANG_PLAYER_AFK, target->GetName().c_str(),
                         target->autoReplyMsg.c_str());
  else if (target->isDND())
    ChatHandler(GetSession())
        .PSendSysMessage(LANG_PLAYER_DND, target->GetName().c_str(),
                         target->autoReplyMsg.c_str());
}

void Player::Whisper(uint32 textId, Player *target, bool isBossWhisper) {
  if (!target)
    return;

  BroadcastText const *bct = sObjectMgr->GetBroadcastText(textId);
  if (!bct) {
    LOG_ERROR("entities.unit",
              "WorldObject::MonsterWhisper: `broadcast_text` was not %u found",
              textId);
    return;
  }

  LocaleConstant locale = target->GetSession()->GetSessionDbLocaleIndex();
  WorldPacket data;
  ChatHandler::BuildChatPacket(
      data, isBossWhisper ? CHAT_MSG_RAID_BOSS_WHISPER : CHAT_MSG_WHISPER,
      LANG_UNIVERSAL, this, target, bct->GetText(locale, getGender()), 0, "",
      locale);
  target->SendDirectMessage(&data);
}

Item *Player::GetMItem(uint32 id) {
  ItemMap::const_iterator itr = mMitems.find(id);
  return itr != mMitems.end() ? itr->second : nullptr;
}

void Player::AddMItem(Item *it) {
  ASSERT(it);
  // ASSERT deleted, because items can be added before loading
  mMitems[it->GetGUID().GetCounter()] = it;
}

bool Player::RemoveMItem(uint32 id) { return mMitems.erase(id) ? true : false; }

void Player::SendOnCancelExpectedVehicleRideAura() const {
  WorldPacket data(SMSG_ON_CANCEL_EXPECTED_RIDE_VEHICLE_AURA, 0);
  SendDirectMessage(&data);
}

bool Player::CanControlPet(uint32 spellId) const {
  // Hunters and Warlocks cannot control their pets until they learned their
  // spell
  if (spellId) {
    switch (spellId) {
    case 93321: // Control Pet
      return getClass() == CLASS_HUNTER;
    case 93375: // Control Demon
      return getClass() == CLASS_WARLOCK;
    }
  } else {
    if (getClass() == CLASS_HUNTER && !HasAura(93321))
      return false;
    if (getClass() == CLASS_WARLOCK && !HasAura(93375))
      return false;
  }

  return true;
}

void Player::PetSpellInitialize() {
  // Do not send the pet action bar when Hunters or Warlocks cannot control
  // their pet
  if (!CanControlPet())
    return;

  Pet *pet = GetPet();

  if (!pet)
    return;

  CharmInfo *charmInfo = pet->GetCharmInfo();

  WorldPacket data(SMSG_PET_SPELLS,
                   8 + 2 + 4 + 4 + 4 * MAX_UNIT_ACTION_BAR_INDEX + 1 + 1);
  data << uint64(pet->GetGUID());
  data << uint16(pet->GetCreatureTemplate()
                     ->family); // creature family (required for pet talents)
  data << uint32(pet->GetDuration());
  data << uint8(pet->GetReactState());
  data << uint8(charmInfo->GetCommandState());
  data << uint16(0); // Flags, mostly unknown

  LOG_DEBUG(
      "entities.pet",
      "Player::PetspellInitialize: Creating spellgroups for summoned pet");

  // action bar loop
  charmInfo->BuildActionBar(&data);

  size_t spellsCountPos = data.wpos();

  // spells count
  uint8 addlist = 0;
  data << uint8(addlist); // placeholder

  if (pet->IsPermanentPetFor(this)) {
    // spells loop
    for (PetSpellMap::iterator itr = pet->m_spells.begin();
         itr != pet->m_spells.end(); ++itr) {
      if (itr->second.state == PETSPELL_REMOVED)
        continue;

      // Do not send this spells, they are used indirectly
      if (sSpellMgr->GetSpellInfo(itr->first)
              ->HasAttribute(SPELL_ATTR4_HIDDEN_IN_SPELLBOOK))
        continue;

      data << uint32(MAKE_UNIT_ACTION_BUTTON(itr->first, itr->second.active));
      ++addlist;
    }
  }

  data.put<uint8>(spellsCountPos, addlist);

  // Cooldowns
  pet->GetSpellHistory()->WritePacket<Pet>(data);

  SendDirectMessage(&data);
}

void Player::PossessSpellInitialize() {
  Unit *charm = GetCharmed();
  if (!charm)
    return;

  CharmInfo *charmInfo = charm->GetCharmInfo();

  if (!charmInfo) {
    LOG_ERROR("entities.player",
              "Player::PossessSpellInitialize: charm (%s) has no charminfo!",
              charm->GetGUID().ToString().c_str());
    return;
  }

  WorldPacket data(SMSG_PET_SPELLS,
                   8 + 2 + 4 + 4 + 4 * MAX_UNIT_ACTION_BAR_INDEX + 1 + 1);
  data << uint64(charm->GetGUID());
  data << uint16(0);
  data << uint32(0);
  data << uint32(0);

  charmInfo->BuildActionBar(&data);

  data << uint8(0); // spells count
  data << uint8(0); // cooldowns count

  SendDirectMessage(&data);
}

void Player::VehicleSpellInitialize() {
  Creature *vehicle = GetVehicleCreatureBase();
  if (!vehicle)
    return;

  uint8 cooldownCount = vehicle->GetSpellHistory()->GetCooldownsSizeForPacket();

  WorldPacket data(SMSG_PET_SPELLS, 8 + 2 + 4 + 4 + 4 * 10 + 1 + 1 +
                                        cooldownCount * (4 + 2 + 4 + 4));
  data << uint64(vehicle->GetGUID()); // Guid
  data << uint16(0);                  // Pet Family (0 for all vehicles)
  data << uint32(vehicle->IsSummon() ? vehicle->ToTempSummon()->GetTimer()
                                     : 0); // Duration
  // The following three segments are read by the client as one uint32
  data << uint8(vehicle->GetReactState()); // React State
  data << uint8(0);                        // Command State
  data << uint16(0x800); // DisableActions (set for all vehicles)

  for (uint32 i = 0; i < MAX_CREATURE_SPELLS; ++i) {
    uint32 spellId = vehicle->m_spells[i];
    SpellInfo const *spellInfo = sSpellMgr->GetSpellInfo(spellId);
    if (!spellInfo) {
      data << uint16(0) << uint8(0) << uint8(i + 8);
      continue;
    }

    if (!sConditionMgr->IsObjectMeetingVehicleSpellConditions(
            vehicle->GetEntry(), spellId, this, vehicle)) {
      LOG_DEBUG("condition",
                "Player::VehicleSpellInitialize: Player '%s' (%s) doesn't meet "
                "conditions for vehicle (Entry: %u, Spell: %u)",
                GetName().c_str(), GetGUID().ToString().c_str(),
                vehicle->ToCreature()->GetEntry(), spellId);
      data << uint16(0) << uint8(0) << uint8(i + 8);
      continue;
    }

    if (spellInfo->IsPassive())
      vehicle->CastSpell(vehicle, spellId, true);

    data << uint32(MAKE_UNIT_ACTION_BUTTON(spellId, i + 8));
  }

  for (uint32 i = MAX_CREATURE_SPELLS; i < MAX_SPELL_CONTROL_BAR; ++i)
    data << uint32(0);

  data << uint8(0); // Auras?

  // Cooldowns
  vehicle->GetSpellHistory()->WritePacket<Pet>(data);
  SendDirectMessage(&data);
}

void Player::CharmSpellInitialize() {
  Unit *charm = GetFirstControlled();
  if (!charm)
    return;

  CharmInfo *charmInfo = charm->GetCharmInfo();
  if (!charmInfo) {
    LOG_ERROR("entities.player",
              "Player::CharmSpellInitialize(): Player '%s' (%s) has a charm "
              "(%s) but no no charminfo!",
              GetName().c_str(), GetGUID().ToString().c_str(),
              charm->GetGUID().ToString().c_str());
    return;
  }

  uint8 addlist = 0;
  if (charm->GetTypeId() != TYPEID_PLAYER) {
    // CreatureInfo const* cinfo = charm->ToCreature()->GetCreatureTemplate();
    // if (cinfo && cinfo->type == CREATURE_TYPE_DEMON && getClass() ==
    // CLASS_WARLOCK)
    {
      for (uint32 i = 0; i < MAX_SPELL_CHARM; ++i)
        if (charmInfo->GetCharmSpell(i)->GetAction())
          ++addlist;
    }
  }

  WorldPacket data(SMSG_PET_SPELLS, 8 + 2 + 4 + 4 +
                                        4 * MAX_UNIT_ACTION_BAR_INDEX + 1 +
                                        4 * addlist + 1);
  data << uint64(charm->GetGUID());
  data << uint16(0);
  data << uint32(0);

  if (charm->GetTypeId() != TYPEID_PLAYER)
    data << uint8(charm->ToCreature()->GetReactState())
         << uint8(charmInfo->GetCommandState()) << uint16(0);
  else
    data << uint32(0);

  charmInfo->BuildActionBar(&data);

  data << uint8(addlist);

  if (addlist) {
    for (uint32 i = 0; i < MAX_SPELL_CHARM; ++i) {
      CharmSpellInfo *cspell = charmInfo->GetCharmSpell(i);
      if (cspell->GetAction())
        data << uint32(cspell->packedData);
    }
  }

  data << uint8(0); // cooldowns count

  SendDirectMessage(&data);
}

void Player::SendRemoveControlBar() const {
  WorldPacket data(SMSG_PET_SPELLS, 8);
  data << uint64(0);
  SendDirectMessage(&data);
}

bool Player::IsAffectedBySpellmod(SpellInfo const *spellInfo,
                                  SpellModifier *mod, Spell *spell) {
  if (!mod || !spellInfo)
    return false;

  // First time this aura applies a mod to us and is out of charges
  if (spell && mod->ownerAura->IsUsingCharges() &&
      !mod->ownerAura->GetCharges() &&
      !spell->m_appliedMods.count(mod->ownerAura))
    return false;

  // +duration to infinite duration spells making them limited
  if (mod->op == SPELLMOD_DURATION && spellInfo->GetDuration() == -1)
    return false;

  return spellInfo->IsAffectedBySpellMod(mod);
}

template <class T>
void Player::ApplySpellMod(uint32 spellId, SpellModOp op, T &basevalue,
                           Spell *spell /*= nullptr*/) const {
  SpellInfo const *spellInfo = sSpellMgr->GetSpellInfo(spellId);
  if (!spellInfo)
    return;
  float totalmul = 1.0f;
  int32 totalflat = 0;

  // Drop charges for triggering spells instead of triggered ones
  if (m_spellModTakingSpell)
    spell = m_spellModTakingSpell;

  switch (op) {
    // special case, if a mod makes spell instant, only consume that mod
  case SPELLMOD_CASTING_TIME: {
    SpellModifier *modInstantSpell = nullptr;
    for (SpellModifier *mod : m_spellMods[op][SPELLMOD_PCT]) {
      if (!IsAffectedBySpellmod(spellInfo, mod, spell))
        continue;

      if (mod->type == SPELLMOD_PCT && basevalue < T(10000) &&
          mod->value <= -100) {
        modInstantSpell = mod;
        break;
      }
    }

    if (modInstantSpell) {
      Player::ApplyModToSpell(modInstantSpell, spell);
      basevalue = T(0);
      return;
    }
    break;
  }
  // special case if two mods apply 100% critical chance, only consume one
  case SPELLMOD_CRITICAL_CHANCE: {
    SpellModifier *modCritical = nullptr;
    for (SpellModifier *mod : m_spellMods[op][SPELLMOD_FLAT]) {
      if (!IsAffectedBySpellmod(spellInfo, mod, spell))
        continue;

      if (mod->type == SPELLMOD_FLAT && mod->value >= 100) {
        modCritical = mod;
        break;
      }
    }

    if (modCritical) {
      Player::ApplyModToSpell(modCritical, spell);
      basevalue = T(100);
      return;
    }
    break;
  }
  default:
    break;
  }

  for (SpellModifier *mod : m_spellMods[op][SPELLMOD_FLAT]) {
    if (!IsAffectedBySpellmod(spellInfo, mod, spell))
      continue;

    if (mod->value == 0)
      continue;

    totalflat += mod->value;
    Player::ApplyModToSpell(mod, spell);
  }

  for (SpellModifier *mod : m_spellMods[op][SPELLMOD_PCT]) {
    if (!IsAffectedBySpellmod(spellInfo, mod, spell))
      continue;

    // skip percent mods for null basevalue (most important for spell mods with
    // charges)
    if (basevalue + totalflat == T(0))
      continue;

    if (mod->value == 0)
      continue;

    // special case (skip > 10sec spell casts for instant cast setting)
    if (mod->op == SPELLMOD_CASTING_TIME && basevalue >= T(10000) &&
        mod->value <= -100)
      continue;

    totalmul *= 1.0f + CalculatePct(1.0f, mod->value);
    Player::ApplyModToSpell(mod, spell);
  }

  basevalue = T(float(basevalue + totalflat) * totalmul);
}

template FC_GAME_API void Player::ApplySpellMod(uint32 spellId, SpellModOp op,
                                                int32 &basevalue,
                                                Spell *spell) const;
template FC_GAME_API void Player::ApplySpellMod(uint32 spellId, SpellModOp op,
                                                uint32 &basevalue,
                                                Spell *spell) const;
template FC_GAME_API void Player::ApplySpellMod(uint32 spellId, SpellModOp op,
                                                float &basevalue,
                                                Spell *spell) const;

void Player::AddSpellMod(SpellModifier *mod, bool apply) {
  LOG_DEBUG("spells", "Player::AddSpellMod: Player '%s' (%s), SpellID: %d",
            GetName().c_str(), GetGUID().ToString().c_str(), mod->spellId);

  /// First, manipulate our spellmodifier container
  if (apply)
    m_spellMods[mod->op][mod->type].insert(mod);
  else
    m_spellMods[mod->op][mod->type].erase(mod);

  /// Now, send spellmodifier packet
  if (!IsLoading()) {
    OpcodeServer opcode = (mod->type == SPELLMOD_FLAT)
                              ? SMSG_SET_FLAT_SPELL_MODIFIER
                              : SMSG_SET_PCT_SPELL_MODIFIER;

    WorldPackets::Spells::SetSpellModifier packet(opcode);

    /// @todo Implement sending of bulk modifiers instead of single
    packet.Modifiers.resize(1);
    WorldPackets::Spells::SpellModifier &spellMod = packet.Modifiers[0];

    spellMod.ModIndex = mod->op;

    for (uint8 eff = 0; eff < 96; ++eff) {
      flag96 mask;
      mask[eff / 32] = 1u << (eff % 32);
      if (mod->mask & mask) {
        WorldPackets::Spells::SpellModifierData modData;

        if (mod->type == SPELLMOD_FLAT) {
          modData.ModifierValue = 0.0f;
          for (SpellModifier *mod : m_spellMods[mod->op][SPELLMOD_FLAT])
            if (mod->mask & mask)
              modData.ModifierValue += mod->value;
        } else {
          modData.ModifierValue = 1.0f;
          for (SpellModifier *mod : m_spellMods[mod->op][SPELLMOD_PCT])
            if (mod->mask & mask)
              modData.ModifierValue *= 1.0f + CalculatePct(1.0f, mod->value);
        }

        modData.ClassIndex = eff;

        spellMod.ModifierData.push_back(modData);
      }
    }

    SendDirectMessage(packet.Write());
  }
}

bool Player::HasSpellModApplied(SpellModifier *mod, Spell *spell) {
  if (!spell)
    return false;

  return spell->m_appliedMods.count(mod->ownerAura) != 0;
}

void Player::ApplyModToSpell(SpellModifier *mod, Spell *spell) {
  if (!spell)
    return;

  // don't do anything with no charges
  if (mod->ownerAura->IsUsingCharges() && !mod->ownerAura->GetCharges())
    return;

  // register inside spell, proc system uses this to drop charges
  spell->m_appliedMods.insert(mod->ownerAura);
}

void Player::SetSpellModTakingSpell(Spell *spell, bool apply) {
  if (apply && m_spellModTakingSpell != nullptr)
    return;

  if (!apply && (!m_spellModTakingSpell || m_spellModTakingSpell != spell))
    return;

  m_spellModTakingSpell = apply ? spell : nullptr;
}

void Player::SendSpellModifiers() const {
  WorldPackets::Spells::SetSpellModifier flatMods(SMSG_SET_FLAT_SPELL_MODIFIER);
  WorldPackets::Spells::SetSpellModifier pctMods(SMSG_SET_PCT_SPELL_MODIFIER);
  for (uint8 i = 0; i < MAX_SPELLMOD; ++i) {
    WorldPackets::Spells::SpellModifier flatMod;
    flatMod.ModifierData.resize(96);
    WorldPackets::Spells::SpellModifier pctMod;
    pctMod.ModifierData.resize(96);
    flatMod.ModIndex = pctMod.ModIndex = i;
    for (uint8 j = 0; j < 96; ++j) {
      flag96 mask;
      mask[j / 32] = 1u << (j % 32);

      flatMod.ModifierData[j].ClassIndex = j;
      flatMod.ModifierData[j].ModifierValue = 0.0f;
      pctMod.ModifierData[j].ClassIndex = j;
      pctMod.ModifierData[j].ModifierValue = 0.0f;

      for (SpellModifier *mod : m_spellMods[i][SPELLMOD_FLAT])
        if (mod->mask & mask)
          flatMod.ModifierData[j].ModifierValue += mod->value;

      for (SpellModifier *mod : m_spellMods[i][SPELLMOD_PCT])
        if (mod->mask & mask)
          pctMod.ModifierData[j].ModifierValue *=
              1.0f + CalculatePct(1.0f, mod->value);
    }

    flatMod.ModifierData.erase(
        std::remove_if(flatMod.ModifierData.begin(), flatMod.ModifierData.end(),
                       [](WorldPackets::Spells::SpellModifierData const &mod) {
                         return G3D::fuzzyEq(mod.ModifierValue, 0.0f);
                       }),
        flatMod.ModifierData.end());

    pctMod.ModifierData.erase(
        std::remove_if(pctMod.ModifierData.begin(), pctMod.ModifierData.end(),
                       [](WorldPackets::Spells::SpellModifierData const &mod) {
                         return G3D::fuzzyEq(mod.ModifierValue, 1.0f);
                       }),
        pctMod.ModifierData.end());

    flatMods.Modifiers.emplace_back(std::move(flatMod));
    pctMods.Modifiers.emplace_back(std::move(pctMod));
  }

  if (!flatMods.Modifiers.empty())
    SendDirectMessage(flatMods.Write());

  if (!pctMods.Modifiers.empty())
    SendDirectMessage(pctMods.Write());
}

// send Proficiency
void Player::SendProficiency(ItemClass itemClass,
                             uint32 itemSubclassMask) const {
  WorldPackets::Item::SetProficiency packet;
  packet.ProficiencyClass = itemClass;
  packet.ProficiencyMask = itemSubclassMask;
  SendDirectMessage(packet.Write());
}

void Player::RemovePetitionsAndSigns(ObjectGuid guid, CharterTypes type) {
  sPetitionMgr->RemoveSignaturesBySignerAndType(guid, type);
  sPetitionMgr->RemovePetitionsByOwnerAndType(guid, type);
}

void Player::LeaveAllArenaTeams(ObjectGuid guid) {
  CharacterCacheEntry const *characterInfo =
      sCharacterCache->GetCharacterCacheByGuid(guid);
  if (!characterInfo)
    return;

  for (uint8 i = 0; i < MAX_ARENA_SLOT; ++i) {
    uint32 arenaTeamId = characterInfo->ArenaTeamId[i];
    if (arenaTeamId != 0) {
      ArenaTeam *arenaTeam = sArenaTeamMgr->GetArenaTeamById(arenaTeamId);
      if (arenaTeam)
        arenaTeam->DelMember(guid, true);
    }
  }
}

void Player::SetRestBonus(float rest_bonus_new) {
  // Prevent resting on max level
  if (IsMaxLevel())
    rest_bonus_new = 0;

  if (rest_bonus_new < 0)
    rest_bonus_new = 0;

  float rest_bonus_max = (float)GetUInt32Value(PLAYER_NEXT_LEVEL_XP) * 1.5f / 2;

  if (rest_bonus_new > rest_bonus_max)
    m_rest_bonus = rest_bonus_max;
  else
    m_rest_bonus = rest_bonus_new;

  // update data for client
  if ((GetsRecruitAFriendBonus(true) &&
       (GetSession()->IsARecruiter() || GetSession()->GetRecruiterId() != 0)))
    SetByteValue(PLAYER_BYTES_2, PLAYER_BYTES_2_OFFSET_REST_STATE,
                 REST_STATE_RAF_LINKED);
  else {
    if (m_rest_bonus > 10)
      SetByteValue(PLAYER_BYTES_2, PLAYER_BYTES_2_OFFSET_REST_STATE,
                   REST_STATE_RESTED);
    else if (m_rest_bonus <= 1)
      SetByteValue(PLAYER_BYTES_2, PLAYER_BYTES_2_OFFSET_REST_STATE,
                   REST_STATE_NOT_RAF_LINKED);
  }

  // RestTickUpdate
  SetUInt32Value(PLAYER_REST_STATE_EXPERIENCE, uint32(m_rest_bonus));
}

bool Player::ActivateTaxiPathTo(std::vector<uint32> const &nodes,
                                Creature *npc /*= nullptr*/,
                                uint32 spellid /*= 0*/) {
  if (nodes.size() < 2)
    return false;

  // not let cheating with start flight in time of logout process || while in
  // combat || has type state: stunned || has type state: root
  if (GetSession()->isLogingOut() || IsInCombat() ||
      HasUnitState(UNIT_STATE_STUNNED) || HasUnitState(UNIT_STATE_ROOT)) {
    GetSession()->SendActivateTaxiReply(ERR_TAXIPLAYERBUSY);
    return false;
  }

  if (HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_REMOVE_CLIENT_CONTROL))
    return false;

  // taximaster case
  if (npc) {
    // not let cheating with start flight mounted
    if (IsMounted()) {
      GetSession()->SendActivateTaxiReply(ERR_TAXIPLAYERALREADYMOUNTED);
      return false;
    }

    if (IsInDisallowedMountForm()) {
      GetSession()->SendActivateTaxiReply(ERR_TAXIPLAYERSHAPESHIFTED);
      return false;
    }

    // not let cheating with start flight in time of logout process || if
    // casting not finished || while in combat || if not use Spell's with
    // EffectSendTaxi
    if (IsNonMeleeSpellCast(false)) {
      GetSession()->SendActivateTaxiReply(ERR_TAXIPLAYERBUSY);
      return false;
    }
  }
  // cast case or scripted call case
  else {
    RemoveAurasByType(SPELL_AURA_MOUNTED);

    if (IsInDisallowedMountForm())
      RemoveAurasByType(SPELL_AURA_MOD_SHAPESHIFT);

    if (Spell *spell = GetCurrentSpell(CURRENT_GENERIC_SPELL))
      if (spell->m_spellInfo->Id != spellid)
        InterruptSpell(CURRENT_GENERIC_SPELL, false);

    InterruptSpell(CURRENT_AUTOREPEAT_SPELL, false);

    if (Spell *spell = GetCurrentSpell(CURRENT_CHANNELED_SPELL))
      if (spell->m_spellInfo->Id != spellid)
        InterruptSpell(CURRENT_CHANNELED_SPELL, true);
  }

  uint32 sourcenode = nodes[0];

  // starting node too far away (cheat?)
  TaxiNodesEntry const *node = sTaxiNodesStore.LookupEntry(sourcenode);
  if (!node) {
    GetSession()->SendActivateTaxiReply(ERR_TAXINOSUCHPATH);
    return false;
  }

  // Prepare to flight start now

  // stop combat at start taxi flight if any
  CombatStop();

  StopCastingCharm();
  StopCastingBindSight();
  ExitVehicle();

  // stop trade (client cancel trade at taxi map open but cheating tools can be
  // used for reopen it)
  TradeCancel(true);

  // clean not finished taxi path if any
  m_taxi.ClearTaxiDestinations();

  // 0 element current node
  m_taxi.AddTaxiDestination(sourcenode);

  // fill destinations path tail
  uint32 sourcepath = 0;
  uint32 totalcost = 0;
  uint32 firstcost = 0;

  uint32 prevnode = sourcenode;
  uint32 lastnode;

  for (uint32 i = 1; i < nodes.size(); ++i) {
    uint32 path, cost;

    lastnode = nodes[i];
    sObjectMgr->GetTaxiPath(prevnode, lastnode, path, cost);

    if (!path) {
      m_taxi.ClearTaxiDestinations();
      return false;
    }

    totalcost += cost;
    if (i == 1)
      firstcost = cost;

    if (prevnode == sourcenode)
      sourcepath = path;

    m_taxi.AddTaxiDestination(lastnode);

    prevnode = lastnode;
  }

  // get mount model (in case non taximaster (npc == nullptr) allow more wide
  // lookup)
  //
  // Hack-Fix for Alliance not being able to use Acherus taxi. There is
  // only one mount ID for both sides. Probably not good to use 315 in case DBC
  // nodes change but I couldn't find a suitable alternative. OK to use class
  // because only DK can use this taxi.
  uint32 mount_display_id = sObjectMgr->GetTaxiMountDisplayId(
      sourcenode, GetTeam(),
      npc == nullptr ||
          (sourcenode == 315 && getClass() == CLASS_DEATH_KNIGHT));

  // in spell case allow 0 model
  if ((mount_display_id == 0 && spellid == 0) || sourcepath == 0) {
    GetSession()->SendActivateTaxiReply(ERR_TAXIUNSPECIFIEDSERVERERROR);
    m_taxi.ClearTaxiDestinations();
    return false;
  }

  uint64 money = GetMoney();

  if (npc) {
    float discount = GetReputationPriceDiscount(npc);
    totalcost = uint32(ceil(totalcost * discount));
    firstcost = uint32(round(firstcost * discount));
  }

  if (money < totalcost) {
    GetSession()->SendActivateTaxiReply(ERR_TAXINOTENOUGHMONEY);
    m_taxi.ClearTaxiDestinations();
    return false;
  }

  // Checks and preparations done, DO FLIGHT
  UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_FLIGHT_PATHS_TAKEN, 1);

  // prevent stealth flight
  // RemoveAurasWithInterruptFlags(SpellAuraInterruptFlags::Interacting);

  if (sWorld->getBoolConfig(CONFIG_INSTANT_TAXI)) {
    TaxiNodesEntry const *lastPathNode =
        sTaxiNodesStore.LookupEntry(nodes[nodes.size() - 1]);
    ASSERT(lastPathNode);
    m_taxi.ClearTaxiDestinations();
    ModifyMoney(-(int64)totalcost);
    UpdateAchievementCriteria(
        ACHIEVEMENT_CRITERIA_TYPE_GOLD_SPENT_FOR_TRAVELLING, totalcost);
    TeleportTo(lastPathNode->ContinentID, lastPathNode->Pos.X,
               lastPathNode->Pos.Y, lastPathNode->Pos.Z, GetOrientation());
    return false;
  } else {
    ModifyMoney(-(int64)firstcost);
    UpdateAchievementCriteria(
        ACHIEVEMENT_CRITERIA_TYPE_GOLD_SPENT_FOR_TRAVELLING, firstcost);
    GetSession()->SendActivateTaxiReply(ERR_TAXIOK);
    GetSession()->SendDoFlight(mount_display_id, sourcepath);
  }
  return true;
}

bool Player::ActivateTaxiPathTo(uint32 taxi_path_id, uint32 spellid /*= 0*/) {
  TaxiPathEntry const *entry = sTaxiPathStore.LookupEntry(taxi_path_id);
  if (!entry)
    return false;

  std::vector<uint32> nodes;

  nodes.resize(2);
  nodes[0] = entry->FromTaxiNode;
  nodes[1] = entry->ToTaxiNode;

  return ActivateTaxiPathTo(nodes, nullptr, spellid);
}

void Player::CleanupAfterTaxiFlight() {
  m_taxi.ClearTaxiDestinations(); // not destinations, clear source node
  Dismount();
  RemoveFlag(UNIT_FIELD_FLAGS,
             UNIT_FLAG_REMOVE_CLIENT_CONTROL | UNIT_FLAG_TAXI_FLIGHT);
}

void Player::ContinueTaxiFlight() const {
  uint32 sourceNode = m_taxi.GetTaxiSource();
  if (!sourceNode)
    return;

  LOG_DEBUG("entities.unit",
            "Player::ContinueTaxiFlight: Restart %s taxi flight",
            GetGUID().ToString().c_str());

  uint32 mountDisplayId =
      sObjectMgr->GetTaxiMountDisplayId(sourceNode, GetTeam(), true);
  if (!mountDisplayId)
    return;

  uint32 path = m_taxi.GetCurrentTaxiPath();

  // search appropriate start path node
  uint32 startNode = 0;

  TaxiPathNodeList const &nodeList = sTaxiPathNodesByPath[path];

  float distPrev;
  float distNext = GetExactDistSq(nodeList[0]->Loc.X, nodeList[0]->Loc.Y,
                                  nodeList[0]->Loc.Z);

  for (uint32 i = 1; i < nodeList.size(); ++i) {
    TaxiPathNodeEntry const *node = nodeList[i];
    TaxiPathNodeEntry const *prevNode = nodeList[i - 1];

    // skip nodes at another map
    if (node->ContinentID != GetMapId())
      continue;

    distPrev = distNext;

    distNext = GetExactDistSq(node->Loc.X, node->Loc.Y, node->Loc.Z);

    float distNodes =
        (node->Loc.X - prevNode->Loc.X) * (node->Loc.X - prevNode->Loc.X) +
        (node->Loc.Y - prevNode->Loc.Y) * (node->Loc.Y - prevNode->Loc.Y) +
        (node->Loc.Z - prevNode->Loc.Z) * (node->Loc.Z - prevNode->Loc.Z);

    if (distNext + distPrev < distNodes) {
      startNode = i;
      break;
    }
  }

  GetSession()->SendDoFlight(mountDisplayId, path, startNode);
}

void Player::SendTaxiNodeStatusMultiple() {
  for (auto itr = m_clientGUIDs.begin(); itr != m_clientGUIDs.end(); ++itr) {
    if (!itr->IsCreature())
      continue;
    Creature *creature = ObjectAccessor::GetCreature(*this, *itr);
    if (!creature || creature->IsHostileTo(this))
      continue;
    if (!creature->HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_FLIGHTMASTER))
      continue;
    uint32 nearestNode = sObjectMgr->GetNearestTaxiNode(
        creature->GetPositionX(), creature->GetPositionY(),
        creature->GetPositionZ(), creature->GetMapId(), GetTeam());
    if (!nearestNode)
      continue;
    WorldPacket data(SMSG_TAXINODE_STATUS, 9);
    data << *itr;
    data << uint8(m_taxi.IsTaximaskNodeKnown(nearestNode) ? 1 : 2);
    SendDirectMessage(&data);
  }
}

void Player::InitDataForForm(bool reapplyMods) {
  ShapeshiftForm form = GetShapeshiftForm();

  SpellShapeshiftFormEntry const *ssEntry =
      sSpellShapeshiftFormStore.LookupEntry(form);
  if (ssEntry && ssEntry->CombatRoundTime) {
    SetAttackTime(BASE_ATTACK, ssEntry->CombatRoundTime);
    SetAttackTime(OFF_ATTACK, ssEntry->CombatRoundTime);
    SetAttackTime(RANGED_ATTACK, BASE_ATTACK_TIME);
  } else
    SetRegularAttackTime();

  UpdateDisplayPower();

  // update auras at form change, ignore this at mods reapply (.reset stats/etc)
  // when form not change.
  if (!reapplyMods)
    UpdateEquipSpellsAtFormChange();

  UpdateAttackPowerAndDamage();
  UpdateAttackPowerAndDamage(true);
}

void Player::InitDisplayIds() {
  PlayerInfo const *info = sObjectMgr->GetPlayerInfo(getRace(), getClass());
  if (!info) {
    LOG_ERROR("entities.player",
              "Player::InitDisplayIds: Player '%s' (%s) has incorrect "
              "race/class pair. Can't init display ids.",
              GetName().c_str(), GetGUID().ToString().c_str());
    return;
  }

  uint8 gender = GetByteValue(PLAYER_BYTES_3, PLAYER_BYTES_3_OFFSET_GENDER);
  switch (gender) {
  case GENDER_FEMALE:
    SetDisplayId(info->displayId_f);
    SetNativeDisplayId(info->displayId_f);
    break;
  case GENDER_MALE:
    SetDisplayId(info->displayId_m);
    SetNativeDisplayId(info->displayId_m);
    break;
  default:
    LOG_ERROR("entities.player",
              "Player::InitDisplayIds: Player '%s' (%s) has invalid gender %u",
              GetName().c_str(), GetGUID().ToString().c_str(), gender);
  }
}

inline bool
Player::_StoreOrEquipNewItem(uint32 vendorslot, uint32 item, uint32 count,
                             uint8 bag, uint8 slot, int64 price,
                             ItemTemplate const *pProto, Creature *pVendor,
                             VendorItem const *crItem, bool bStore) {
  uint32 stacks = count / pProto->GetBuyCount();
  ItemPosCountVec vDest;
  uint16 uiDest = 0;
  InventoryResult msg = bStore ? CanStoreNewItem(bag, slot, vDest, item, count)
                               : CanEquipNewItem(slot, uiDest, item, false);
  if (msg != EQUIP_ERR_OK) {
    SendEquipError(msg, nullptr, nullptr, item);
    return false;
  }

  ModifyMoney(-price);

  if (crItem->ExtendedCost) // case for new honor system
  {
    ItemExtendedCostEntry const *iece =
        sItemExtendedCostStore.LookupEntry(crItem->ExtendedCost);
    ASSERT(iece);
    for (int i = 0; i < MAX_ITEM_EXT_COST_CURRENCIES; ++i) {
      if (iece->RequiredItem[i])
        DestroyItemCount(iece->RequiredItem[i],
                         iece->RequiredItemCount[i] * stacks, true);
    }

    for (int i = 0; i < MAX_ITEM_EXT_COST_CURRENCIES; ++i) {
      if (iece->RequirementFlags &
          (ITEM_EXT_COST_CURRENCY_REQ_IS_SEASON_EARNED_1 << i))
        continue;

      if (iece->RequiredCurrency[i])
        ModifyCurrency(iece->RequiredCurrency[i],
                       -int32(iece->RequiredCurrencyCount[i] * stacks), true,
                       true);
    }
  }

  Item *it = bStore ? StoreNewItem(vDest, item, true,
                                   GenerateItemRandomPropertyId(item))
                    : EquipNewItem(uiDest, item, true);
  if (it) {
    uint32 new_count = pVendor->UpdateVendorItemCurrentCount(crItem, count);

    WorldPacket data(SMSG_BUY_ITEM, (8 + 4 + 4 + 4));
    data << uint64(pVendor->GetGUID());
    data << uint32(vendorslot + 1); // numbered from 1 at client
    data << int32(crItem->maxcount > 0 ? new_count : 0xFFFFFFFF);
    data << uint32(count);
    SendDirectMessage(&data);
    SendNewItem(it, count, true, false, false);

    if (!bStore)
      AutoUnequipOffhandIfNeed();

    if (pProto->GetFlags() & ITEM_FLAG_ITEM_PURCHASE_RECORD &&
        crItem->ExtendedCost && pProto->GetMaxStackSize() == 1) {
      it->SetFlag(ITEM_FIELD_FLAGS, ITEM_FIELD_FLAG_REFUNDABLE);
      it->SetRefundRecipient(GetGUID().GetCounter());
      it->SetPaidMoney(uint32(price));
      it->SetPaidExtendedCost(crItem->ExtendedCost);
      it->SaveRefundDataToDB();
      AddRefundReference(it->GetGUID());
    }
  }
  return true;
}

bool Player::BuyCurrencyFromVendorSlot(ObjectGuid vendorGuid, uint32 vendorSlot,
                                       uint32 currency, uint32 count) {
  // cheating attempt
  if (count < 1)
    count = 1;

  if (!IsAlive())
    return false;

  CurrencyTypesEntry const *proto = sCurrencyTypesStore.LookupEntry(currency);
  if (!proto) {
    SendBuyError(BUY_ERR_CANT_FIND_ITEM, nullptr, currency, 0);
    return false;
  }

  Creature *creature =
      GetNPCIfCanInteractWith(vendorGuid, UNIT_NPC_FLAG_VENDOR);
  if (!creature) {
    LOG_DEBUG("network",
              "WORLD: BuyCurrencyFromVendorSlot - %s not found or you can't "
              "interact with him.",
              vendorGuid.ToString().c_str());
    SendBuyError(BUY_ERR_DISTANCE_TOO_FAR, nullptr, currency, 0);
    return false;
  }

  VendorItemData const *vItems = creature->GetVendorItems();
  if (!vItems || vItems->Empty()) {
    SendBuyError(BUY_ERR_CANT_FIND_ITEM, creature, currency, 0);
    return false;
  }

  if (vendorSlot >= vItems->GetItemCount()) {
    SendBuyError(BUY_ERR_CANT_FIND_ITEM, creature, currency, 0);
    return false;
  }

  VendorItem const *crItem = vItems->GetItem(vendorSlot);
  // store diff item (cheating)
  if (!crItem || crItem->item != currency ||
      crItem->Type != ITEM_VENDOR_TYPE_CURRENCY) {
    SendBuyError(BUY_ERR_CANT_FIND_ITEM, creature, currency, 0);
    return false;
  }

  if (count % crItem->maxcount) {
    SendEquipError(EQUIP_ERR_CANT_BUY_QUANTITY, nullptr, nullptr);
    return false;
  }

  uint32 stacks = count / crItem->maxcount;
  ItemExtendedCostEntry const *iece = nullptr;
  if (crItem->ExtendedCost) {
    iece = sItemExtendedCostStore.LookupEntry(crItem->ExtendedCost);
    if (!iece) {
      LOG_ERROR("entities.player",
                "Currency %u have wrong ExtendedCost field value %u", currency,
                crItem->ExtendedCost);
      return false;
    }

    for (uint8 i = 0; i < MAX_ITEM_EXT_COST_ITEMS; ++i) {
      if (iece->RequiredItem[i] &&
          !HasItemCount(iece->RequiredItem[i],
                        (iece->RequiredItemCount[i] * stacks))) {
        SendEquipError(EQUIP_ERR_VENDOR_MISSING_TURNINS, nullptr, nullptr);
        return false;
      }
    }

    for (uint8 i = 0; i < MAX_ITEM_EXT_COST_CURRENCIES; ++i) {
      if (!iece->RequiredCurrency[i])
        continue;

      CurrencyTypesEntry const *entry =
          sCurrencyTypesStore.LookupEntry(iece->RequiredCurrency[i]);
      if (!entry) {
        SendBuyError(BUY_ERR_CANT_FIND_ITEM, creature, currency,
                     0); // Find correct error
        return false;
      }

      if (iece->RequirementFlags &
          (ITEM_EXT_COST_CURRENCY_REQ_IS_SEASON_EARNED_1 << i)) {
        // Not implemented
        SendEquipError(EQUIP_ERR_VENDOR_MISSING_TURNINS, nullptr,
                       nullptr); // Find correct error
        return false;
      } else if (!HasCurrency(iece->RequiredCurrency[i],
                              (iece->RequiredCurrencyCount[i] * stacks))) {
        SendEquipError(EQUIP_ERR_VENDOR_MISSING_TURNINS, nullptr,
                       nullptr); // Find correct error
        return false;
      }
    }

    // check for personal arena rating requirement
    if (GetMaxPersonalArenaRatingRequirement(iece->RequiredArenaSlot) <
        iece->RequiredPersonalArenaRating) {
      // probably not the proper equip err
      SendEquipError(EQUIP_ERR_CANT_EQUIP_RANK, nullptr, nullptr);
      return false;
    }

    if (iece->RequiredFactionId &&
        uint32(GetReputationRank(iece->RequiredFactionId)) <
            iece->RequiredFactionStanding) {
      SendBuyError(BUY_ERR_REPUTATION_REQUIRE, creature, currency, 0);
      return false;
    }

    if (iece->RequirementFlags & ITEM_EXT_COST_FLAG_REQUIRE_GUILD &&
        !GetGuildId()) {
      SendEquipError(EQUIP_ERR_VENDOR_MISSING_TURNINS, nullptr,
                     nullptr); // Find correct error
      return false;
    }

    if (iece->RequiredGuildLevel &&
        iece->RequiredGuildLevel < GetGuildLevel()) {
      SendEquipError(EQUIP_ERR_VENDOR_MISSING_TURNINS, nullptr,
                     nullptr); // Find correct error
      return false;
    }

    if (iece->RequiredAchievement && !HasAchieved(iece->RequiredAchievement)) {
      SendEquipError(EQUIP_ERR_VENDOR_MISSING_TURNINS, nullptr,
                     nullptr); // Find correct error
      return false;
    }
  } else // currencies have no price defined, can only be bought with
         // ExtendedCost
  {
    SendBuyError(BUY_ERR_CANT_FIND_ITEM, nullptr, currency, 0);
    return false;
  }

  ModifyCurrency(currency, count, true, true);
  for (uint8 i = 0; i < MAX_ITEM_EXT_COST_ITEMS; ++i) {
    if (!iece->RequiredItem[i])
      continue;

    DestroyItemCount(iece->RequiredItem[i], iece->RequiredItemCount[i] * stacks,
                     true);
  }

  for (uint8 i = 0; i < MAX_ITEM_EXT_COST_CURRENCIES; ++i) {
    if (!iece->RequiredCurrency[i])
      continue;

    if (iece->RequirementFlags &
        (ITEM_EXT_COST_CURRENCY_REQ_IS_SEASON_EARNED_1 << i))
      continue;

    ModifyCurrency(iece->RequiredCurrency[i],
                   -int32(iece->RequiredCurrencyCount[i]) * stacks, false,
                   true);
  }

  return true;
}

// Return true is the bought item has a max count to force refresh of window by
// caller
bool Player::BuyItemFromVendorSlot(ObjectGuid vendorguid, uint32 vendorslot,
                                   uint32 item, uint32 count, uint8 bag,
                                   uint8 slot) {
  // cheating attempt
  if (count < 1)
    count = 1;

  // cheating attempt
  if (slot > MAX_BAG_SIZE && slot != NULL_SLOT)
    return false;

  if (!IsAlive())
    return false;

  ItemTemplate const *pProto = sObjectMgr->GetItemTemplate(item);
  if (!pProto) {
    SendBuyError(BUY_ERR_CANT_FIND_ITEM, nullptr, item, 0);
    return false;
  }

  if (!(pProto->GetAllowableClass() & getClassMask()) &&
      pProto->GetBonding() == BIND_ON_ACQUIRE && !IsGameMaster()) {
    SendBuyError(BUY_ERR_CANT_FIND_ITEM, nullptr, item, 0);
    return false;
  }

  if (!IsGameMaster() && ((pProto->GetFlags2() & ITEM_FLAG2_FACTION_HORDE &&
                           GetTeam() == ALLIANCE) ||
                          (pProto->GetFlags2() == ITEM_FLAG2_FACTION_ALLIANCE &&
                           GetTeam() == HORDE)))
    return false;

  Creature *creature =
      GetNPCIfCanInteractWith(vendorguid, UNIT_NPC_FLAG_VENDOR);
  if (!creature) {
    LOG_DEBUG("network",
              "Player::BuyItemFromVendorSlot: Vendor (%s) not found or player "
              "'%s' (%s) can't interact with him.",
              vendorguid.ToString().c_str(), GetName().c_str(),
              GetGUID().ToString().c_str());
    SendBuyError(BUY_ERR_DISTANCE_TOO_FAR, nullptr, item, 0);
    return false;
  }

  if (!sConditionMgr->IsObjectMeetingVendorItemConditions(
          creature->GetEntry(), item, this, creature)) {
    LOG_DEBUG("condition",
              "Player::BuyItemFromVendorSlot: Player '%s' (%s) doesn't meed "
              "conditions for creature (Entry: %u, Item: %u)",
              GetName().c_str(), GetGUID().ToString().c_str(),
              creature->GetEntry(), item);
    SendBuyError(BUY_ERR_CANT_FIND_ITEM, creature, item, 0);
    return false;
  }

  VendorItemData const *vItems = creature->GetVendorItems();
  if (!vItems || vItems->Empty()) {
    SendBuyError(BUY_ERR_CANT_FIND_ITEM, creature, item, 0);
    return false;
  }

  if (vendorslot >= vItems->GetItemCount()) {
    SendBuyError(BUY_ERR_CANT_FIND_ITEM, creature, item, 0);
    return false;
  }

  VendorItem const *crItem = vItems->GetItem(vendorslot);
  // store diff item (cheating)
  if (!crItem || crItem->item != item) {
    SendBuyError(BUY_ERR_CANT_FIND_ITEM, creature, item, 0);
    return false;
  }

  // check current item amount if it limited
  if (crItem->maxcount != 0) {
    if (creature->GetVendorItemCurrentCount(crItem) < count) {
      SendBuyError(BUY_ERR_ITEM_ALREADY_SOLD, creature, item, 0);
      return false;
    }
  }

  if (pProto->GetRequiredReputationFaction() &&
      (uint32(GetReputationRank(pProto->GetRequiredReputationFaction())) <
       pProto->GetRequiredReputationRank())) {
    SendBuyError(BUY_ERR_REPUTATION_REQUIRE, creature, item, 0);
    return false;
  }

  if (crItem->ExtendedCost) {
    // Can only buy full stacks for extended cost
    if (count % pProto->GetBuyCount()) {
      SendEquipError(EQUIP_ERR_CANT_BUY_QUANTITY, nullptr, nullptr);
      return false;
    }

    uint32 stacks = count / pProto->GetBuyCount();
    ItemExtendedCostEntry const *iece =
        sItemExtendedCostStore.LookupEntry(crItem->ExtendedCost);
    if (!iece) {
      LOG_ERROR("entities.player",
                "Player::BuyItemFromVendorSlot: Item %u has wrong ExtendedCost "
                "field value %u",
                pProto->GetId(), crItem->ExtendedCost);
      return false;
    }

    for (uint8 i = 0; i < MAX_ITEM_EXT_COST_ITEMS; ++i) {
      if (iece->RequiredItem[i] &&
          !HasItemCount(iece->RequiredItem[i],
                        iece->RequiredItemCount[i] * stacks)) {
        SendEquipError(EQUIP_ERR_VENDOR_MISSING_TURNINS, nullptr, nullptr);
        return false;
      }
    }

    for (uint8 i = 0; i < MAX_ITEM_EXT_COST_CURRENCIES; ++i) {
      if (!iece->RequiredCurrency[i])
        continue;

      CurrencyTypesEntry const *entry =
          sCurrencyTypesStore.LookupEntry(iece->RequiredCurrency[i]);
      if (!entry) {
        SendBuyError(BUY_ERR_CANT_FIND_ITEM, creature, item, 0);
        return false;
      }

      if (iece->RequirementFlags &
          (ITEM_EXT_COST_CURRENCY_REQ_IS_SEASON_EARNED_1 << i)) {
        if (GetCurrencyOnSeason(entry->ID) < iece->RequiredCurrencyCount[0]) {
          SendEquipError(EQUIP_ERR_VENDOR_MISSING_TURNINS, NULL, NULL);
          return false;
        }

        if (!HasCurrency(iece->RequiredCurrency[1],
                         iece->RequiredCurrencyCount[1] * stacks)) {
          SendEquipError(EQUIP_ERR_VENDOR_MISSING_TURNINS, NULL, NULL);
          return false;
        }
      } else if (!HasCurrency(iece->RequiredCurrency[i],
                              iece->RequiredCurrencyCount[i] * stacks)) {
        SendEquipError(EQUIP_ERR_VENDOR_MISSING_TURNINS, nullptr, nullptr);
        return false;
      }
    }

    // check for personal arena rating requirement
    if (GetMaxPersonalArenaRatingRequirement(iece->RequiredArenaSlot) <
        iece->RequiredPersonalArenaRating) {
      // probably not the proper equip err
      SendEquipError(EQUIP_ERR_CANT_EQUIP_RANK, nullptr, nullptr);
      return false;
    }

    if (iece->RequiredFactionId &&
        uint32(GetReputationRank(iece->RequiredFactionId)) <
            iece->RequiredFactionStanding) {
      SendBuyError(BUY_ERR_REPUTATION_REQUIRE, creature, item, 0);
      return false;
    }

    if (iece->RequirementFlags & ITEM_EXT_COST_FLAG_REQUIRE_GUILD &&
        !GetGuildId()) {
      SendEquipError(EQUIP_ERR_VENDOR_MISSING_TURNINS, nullptr,
                     nullptr); // Find correct error
      return false;
    }

    if (iece->RequiredGuildLevel &&
        iece->RequiredGuildLevel < GetGuildLevel()) {
      SendEquipError(EQUIP_ERR_VENDOR_MISSING_TURNINS, nullptr,
                     nullptr); // Find correct error
      return false;
    }

    if (iece->RequiredAchievement && !HasAchieved(iece->RequiredAchievement)) {
      SendEquipError(EQUIP_ERR_VENDOR_MISSING_TURNINS, nullptr,
                     nullptr); // Find correct error
      return false;
    }
  }

  uint64 price = 0;
  if (crItem->IsGoldRequired(pProto) &&
      pProto->GetBuyPrice() >
          0) // Assume price cannot be negative (do not know why it is int32)
  {
    double buyPricePerItem =
        double(pProto->GetBuyPrice()) / pProto->GetBuyCount();
    uint32 maxCount = MAX_MONEY_AMOUNT / buyPricePerItem;
    if (count > maxCount) {
      LOG_INFO("entities.player.cheat",
               "Player::BuyItemFromVendorSlot: Player '%s' (%s) tried to buy "
               "item (ItemID: %u, Count: %u), causing overflow",
               GetName().c_str(), GetGUID().ToString().c_str(), pProto->GetId(),
               count);
      count = maxCount;
    }
    price = uint64(buyPricePerItem *
                   count); // it should not exceed MAX_MONEY_AMOUNT

    // reputation discount
    price = uint64(floor(price * GetReputationPriceDiscount(creature)));

    if (int32 priceMod =
            GetTotalAuraModifier(SPELL_AURA_MOD_VENDOR_ITEMS_PRICES))
      price -= CalculatePct(price, priceMod);

    if (!HasEnoughMoney(price)) {
      SendBuyError(BUY_ERR_NOT_ENOUGHT_MONEY, creature, item, 0);
      return false;
    }
  }

  if ((bag == NULL_BAG && slot == NULL_SLOT) || IsInventoryPos(bag, slot)) {
    if (!_StoreOrEquipNewItem(vendorslot, item, count, bag, slot, price, pProto,
                              creature, crItem, true))
      return false;
  } else if (IsEquipmentPos(bag, slot)) {
    if (count != 1) {
      SendEquipError(EQUIP_ERR_NOT_EQUIPPABLE, nullptr, nullptr);
      return false;
    }
    if (!_StoreOrEquipNewItem(vendorslot, item, count, bag, slot, price, pProto,
                              creature, crItem, false))
      return false;
  } else {
    SendEquipError(EQUIP_ERR_WRONG_SLOT, nullptr, nullptr);
    return false;
  }

  if (crItem->maxcount != 0) // bought
  {
    if (pProto->GetQuality() > ITEM_QUALITY_EPIC ||
        (pProto->GetQuality() == ITEM_QUALITY_EPIC &&
         pProto->GetBaseItemLevel() >=
             MinNewsItemLevel[sWorld->getIntConfig(CONFIG_EXPANSION)]))
      if (Guild *guild = GetGuild())
        guild->AddGuildNews(GUILD_NEWS_ITEM_PURCHASED, GetGUID(), 0, item);
    return true;
  }

  return false;
}

uint32 Player::GetMaxPersonalArenaRatingRequirement(uint32 minarenaslot) const {
  // returns the maximal personal arena rating that can be used to purchase
  // items requiring this condition the personal rating of the arena team must
  // match the required limit as well so return max[in
  // arenateams](min(personalrating[teamtype], teamrating[teamtype]))
  uint32 max_personal_rating = 0;
  for (uint8 i = minarenaslot; i < MAX_ARENA_SLOT; ++i) {
    if (ArenaTeam *at = sArenaTeamMgr->GetArenaTeamById(GetArenaTeamId(i))) {
      uint32 p_rating = GetArenaPersonalRating(i);
      uint32 t_rating = at->GetRating();
      p_rating = p_rating < t_rating ? p_rating : t_rating;
      if (max_personal_rating < p_rating)
        max_personal_rating = p_rating;
    }
  }
  return max_personal_rating;
}

void Player::UpdateHomebindTime(uint32 time) {
  // GMs never get homebind timer online
  if (m_InstanceValid || IsGameMaster()) {
    if (m_HomebindTimer) // instance valid, but timer not reset
      SendRaidGroupOnlyMessage(RAID_GROUP_ERR_NONE, 0);

    // instance is valid, reset homebind timer
    m_HomebindTimer = 0;
  } else if (m_HomebindTimer > 0) {
    if (time >= m_HomebindTimer) {
      // teleport to nearest graveyard
      RepopAtGraveyard();
    } else
      m_HomebindTimer -= time;
  } else {
    // instance is invalid, start homebind timer
    m_HomebindTimer = 60000;
    // send message to player
    SendRaidGroupOnlyMessage(RAID_GROUP_ERR_REQUIREMENTS_UNMATCH,
                             m_HomebindTimer);
    LOG_DEBUG("maps",
              "Player::UpdateHomebindTime: Player '%s' (%s) will be teleported "
              "to homebind in 60 seconds",
              GetName().c_str(), GetGUID().ToString().c_str());
  }
}

void Player::UpdatePvPState(bool onlyFFA) {
  /// @todo should we always synchronize UNIT_FIELD_BYTES_2, 1 of controller and
  /// controlled?
  // no, we shouldn't, those are checked for affecting player by client
  if (!pvpInfo.IsInNoPvPArea && !IsGameMaster() &&
      (pvpInfo.IsInFFAPvPArea || sWorld->IsFFAPvPRealm())) {
    if (!IsFFAPvP()) {
      SetByteFlag(UNIT_FIELD_BYTES_2, UNIT_BYTES_2_OFFSET_PVP_FLAG,
                  UNIT_BYTE2_FLAG_FFA_PVP);
      for (ControlList::iterator itr = m_Controlled.begin();
           itr != m_Controlled.end(); ++itr)
        (*itr)->SetByteValue(UNIT_FIELD_BYTES_2, UNIT_BYTES_2_OFFSET_PVP_FLAG,
                             UNIT_BYTE2_FLAG_FFA_PVP);
    }
  } else if (IsFFAPvP()) {
    RemoveByteFlag(UNIT_FIELD_BYTES_2, UNIT_BYTES_2_OFFSET_PVP_FLAG,
                   UNIT_BYTE2_FLAG_FFA_PVP);
    for (ControlList::iterator itr = m_Controlled.begin();
         itr != m_Controlled.end(); ++itr)
      (*itr)->RemoveByteFlag(UNIT_FIELD_BYTES_2, UNIT_BYTES_2_OFFSET_PVP_FLAG,
                             UNIT_BYTE2_FLAG_FFA_PVP);
  }

  if (onlyFFA)
    return;

  if (pvpInfo.IsHostile) // in hostile area
  {
    if (!IsPvP() || pvpInfo.EndTimer)
      UpdatePvP(true, true);
  } else // in friendly area
  {
    if (IsPvP() && !HasFlag(PLAYER_FLAGS, PLAYER_FLAGS_IN_PVP) &&
        !pvpInfo.EndTimer)
      pvpInfo.EndTimer = GameTime::GetGameTime(); // start toggle-off
  }
}

void Player::SetPvP(bool state) {
  Unit::SetPvP(state);
  for (ControlList::iterator itr = m_Controlled.begin();
       itr != m_Controlled.end(); ++itr)
    (*itr)->SetPvP(state);
}

void Player::UpdatePvP(bool state, bool _override) {
  if (!state || _override) {
    SetPvP(state);
    pvpInfo.EndTimer = 0;
  } else {
    pvpInfo.EndTimer = GameTime::GetGameTime();
    SetPvP(state);
  }
}

void Player::UpdatePotionCooldown(Spell *spell) {
  // no potion used i combat or still in combat
  if (!m_lastPotionId || IsInCombat())
    return;

  // Call not from spell cast, send cooldown event for item spells if no in
  // combat
  if (!spell) {
    // spell/item pair let set proper cooldown (except non-existing charged
    // spell cooldown spellmods for potions)
    if (ItemTemplate const *proto = sObjectMgr->GetItemTemplate(m_lastPotionId))
      for (ItemEffect const &effect : proto->Effects)
        if (effect.SpellID && effect.Trigger == ITEM_SPELLTRIGGER_ON_USE)
          if (SpellInfo const *spellInfo =
                  sSpellMgr->GetSpellInfo(effect.SpellID))
            GetSpellHistory()->SendCooldownEvent(spellInfo, m_lastPotionId);
  }
  // from spell cases (m_lastPotionId set in Spell::SendSpellCooldown)
  else {
    if (spell->IsIgnoringCooldowns())
      return;
    else
      GetSpellHistory()->SendCooldownEvent(spell->m_spellInfo, m_lastPotionId,
                                           spell);
  }

  m_lastPotionId = 0;
}

void Player::SetResurrectRequestData(Unit *caster, uint32 health, uint32 mana,
                                     uint32 appliedAura) {
  ASSERT(!IsResurrectRequested());
  _resurrectionData = new ResurrectionData();
  _resurrectionData->GUID = caster->GetGUID();
  _resurrectionData->Location.WorldRelocate(*caster);
  _resurrectionData->Health = health;
  _resurrectionData->Mana = mana;
  _resurrectionData->Aura = appliedAura;
}
// slot to be excluded while counting
bool Player::EnchantmentFitsRequirements(uint32 enchantmentcondition,
                                         int8 slot) const {
  if (!enchantmentcondition)
    return true;

  SpellItemEnchantmentConditionEntry const *Condition =
      sSpellItemEnchantmentConditionStore.LookupEntry(enchantmentcondition);

  if (!Condition)
    return true;

  uint8 curcount[4] = {0, 0, 0, 0};

  // counting current equipped gem colors
  for (uint8 i = EQUIPMENT_SLOT_START; i < EQUIPMENT_SLOT_END; ++i) {
    if (i == slot)
      continue;
    Item *pItem2 = GetItemByPos(INVENTORY_SLOT_BAG_0, i);
    if (pItem2 && !pItem2->IsBroken() &&
        pItem2->GetTemplate()->GetSocketColor(0)) {
      for (uint32 enchant_slot = SOCK_ENCHANTMENT_SLOT;
           enchant_slot < SOCK_ENCHANTMENT_SLOT + 3; ++enchant_slot) {
        uint32 enchant_id =
            pItem2->GetEnchantmentId(EnchantmentSlot(enchant_slot));
        if (!enchant_id)
          continue;

        SpellItemEnchantmentEntry const *enchantEntry =
            sSpellItemEnchantmentStore.LookupEntry(enchant_id);
        if (!enchantEntry)
          continue;

        uint32 gemid = enchantEntry->Src_itemID;
        if (!gemid)
          continue;

        ItemTemplate const *gemProto = sObjectMgr->GetItemTemplate(gemid);
        if (!gemProto)
          continue;

        GemPropertiesEntry const *gemProperty =
            sGemPropertiesStore.LookupEntry(gemProto->GetGemProperties());
        if (!gemProperty)
          continue;

        uint8 GemColor = gemProperty->Type;

        for (uint8 b = 0, tmpcolormask = 1; b < 4; b++, tmpcolormask <<= 1) {
          if (tmpcolormask & GemColor)
            ++curcount[b];
        }
      }
    }
  }

  bool activate = true;

  for (uint8 i = 0; i < 3; i++) {
    if (!Condition->Color[i])
      continue;

    uint32 _cur_gem = curcount[Condition->Color[i] - 1];

    // if have <CompareColor> use them as count, else use <value> from Condition
    uint32 _cmp_gem = Condition->CompareColor[i]
                          ? curcount[Condition->CompareColor[i] - 1]
                          : Condition->Value[i];

    switch (Condition->Comparator[i]) {
    case 2: // requires less <color> than (<value> || <comparecolor>) gems
      activate &= (_cur_gem < _cmp_gem) ? true : false;
      break;
    case 3: // requires more <color> than (<value> || <comparecolor>) gems
      activate &= (_cur_gem > _cmp_gem) ? true : false;
      break;
    case 5: // requires at least <color> than (<value> || <comparecolor>) gems
      activate &= (_cur_gem >= _cmp_gem) ? true : false;
      break;
    }
  }

  LOG_DEBUG(
      "entities.player.items",
      "Player::EnchantmentFitsRequirements: Checking Condition %u, there are "
      "%u Meta Gems, %u Red Gems, %u Yellow Gems and %u Blue Gems, Activate:%s",
      enchantmentcondition, curcount[0], curcount[1], curcount[2], curcount[3],
      activate ? "yes" : "no");

  return activate;
}

void Player::CorrectMetaGemEnchants(uint8 exceptslot, bool apply) {
  // cycle all equipped items
  for (uint32 slot = EQUIPMENT_SLOT_START; slot < EQUIPMENT_SLOT_END; ++slot) {
    // enchants for the slot being socketed are handled by Player::ApplyItemMods
    if (slot == exceptslot)
      continue;

    Item *pItem = GetItemByPos(INVENTORY_SLOT_BAG_0, slot);

    if (!pItem || !pItem->GetTemplate()->GetSocketColor(0))
      continue;

    for (uint32 enchant_slot = SOCK_ENCHANTMENT_SLOT;
         enchant_slot < SOCK_ENCHANTMENT_SLOT + 3; ++enchant_slot) {
      uint32 enchant_id =
          pItem->GetEnchantmentId(EnchantmentSlot(enchant_slot));
      if (!enchant_id)
        continue;

      SpellItemEnchantmentEntry const *enchantEntry =
          sSpellItemEnchantmentStore.LookupEntry(enchant_id);
      if (!enchantEntry)
        continue;

      uint32 condition = enchantEntry->Condition_ID;
      if (condition) {
        // was enchant active with/without item?
        bool wasactive =
            EnchantmentFitsRequirements(condition, apply ? exceptslot : -1);
        // should it now be?
        if (wasactive ^
            EnchantmentFitsRequirements(condition, apply ? -1 : exceptslot)) {
          // ignore item gem conditions
          // if state changed, (dis)apply enchant
          ApplyEnchantment(pItem, EnchantmentSlot(enchant_slot), !wasactive,
                           true, true);
        }
      }
    }
  }
}

// if false -> then toggled off if was on| if true -> toggled on if was off AND
// meets requirements
void Player::ToggleMetaGemsActive(uint8 exceptslot, bool apply) {
  // cycle all equipped items
  for (int slot = EQUIPMENT_SLOT_START; slot < EQUIPMENT_SLOT_END; ++slot) {
    // enchants for the slot being socketed are handled by
    // WorldSession::HandleSocketOpcode(WorldPacket& recvData)
    if (slot == exceptslot)
      continue;

    Item *pItem = GetItemByPos(INVENTORY_SLOT_BAG_0, slot);

    if (!pItem ||
        !pItem->GetTemplate()->GetSocketColor(
            0)) // if item has no sockets or no item is equipped go to next item
      continue;

    // cycle all (gem)enchants
    for (uint32 enchant_slot = SOCK_ENCHANTMENT_SLOT;
         enchant_slot < SOCK_ENCHANTMENT_SLOT + 3; ++enchant_slot) {
      uint32 enchant_id =
          pItem->GetEnchantmentId(EnchantmentSlot(enchant_slot));
      if (!enchant_id) // if no enchant go to next enchant(slot)
        continue;

      SpellItemEnchantmentEntry const *enchantEntry =
          sSpellItemEnchantmentStore.LookupEntry(enchant_id);
      if (!enchantEntry)
        continue;

      // only metagems to be (de)activated, so only enchants with condition
      uint32 condition = enchantEntry->Condition_ID;
      if (condition)
        ApplyEnchantment(pItem, EnchantmentSlot(enchant_slot), apply);
    }
  }
}

void Player::SetBattlegroundEntryPoint() {
  // Taxi path store
  if (!m_taxi.empty()) {
    m_bgData.mountSpell = 0;
    m_bgData.taxiPath[0] = m_taxi.GetTaxiSource();
    m_bgData.taxiPath[1] = m_taxi.GetTaxiDestination();

    // On taxi we don't need check for dungeon
    m_bgData.joinPos = WorldLocation(GetMapId(), GetPositionX(), GetPositionY(),
                                     GetPositionZ(), GetOrientation());
  } else {
    m_bgData.ClearTaxiPath();

    // Mount spell id storing
    if (IsMounted()) {
      AuraEffectList const &auras = GetAuraEffectsByType(SPELL_AURA_MOUNTED);
      if (!auras.empty())
        m_bgData.mountSpell = (*auras.begin())->GetId();
    } else
      m_bgData.mountSpell = 0;

    // If map is dungeon find linked graveyard
    if (GetMap()->IsDungeon()) {
      if (const WorldSafeLocsEntry *entry =
              sObjectMgr->GetClosestGraveyard(*this, GetTeam(), this))
        m_bgData.joinPos = WorldLocation(entry->Continent, entry->Loc.X,
                                         entry->Loc.Y, entry->Loc.Z, 0.0f);
      else
        LOG_ERROR("entities.player",
                  "Player::SetBattlegroundEntryPoint: Dungeon (MapID: %u) has "
                  "no linked graveyard, setting home location as entry point.",
                  GetMapId());
    }
    // If new entry point is not BG or arena set it
    else if (!GetMap()->IsBattlegroundOrArena())
      m_bgData.joinPos =
          WorldLocation(GetMapId(), GetPositionX(), GetPositionY(),
                        GetPositionZ(), GetOrientation());
  }

  if (m_bgData.joinPos.m_mapId ==
      MAPID_INVALID) // In error cases use homebind position
    m_bgData.joinPos = WorldLocation(m_homebindMapId, m_homebindX, m_homebindY,
                                     m_homebindZ, 0.0f);
}

void Player::GetLFGLeavePoint(Position *pos) {
  if (pos) {
    pos->Relocate(m_bgData.leavePos.m_positionX, m_bgData.leavePos.m_positionY,
                  m_bgData.leavePos.m_positionZ,
                  m_bgData.leavePos.GetOrientation());
    _hasValidLFGLeavePoint = false;
  }
}

bool Player::HasValidLFGLeavePoint(uint32 mapid) {
  return _hasValidLFGLeavePoint &&
         MapManager::IsValidMapCoord(mapid, m_bgData.leavePos.m_positionX,
                                     m_bgData.leavePos.m_positionY,
                                     m_bgData.leavePos.m_positionZ,
                                     m_bgData.leavePos.GetOrientation());
}

void Player::SetLFGLeavePoint() {
  m_bgData.leavePos = WorldLocation(GetMapId(), GetPositionX(), GetPositionY(),
                                    GetPositionZ(), GetOrientation());
  _hasValidLFGLeavePoint = true;
}

void Player::SetBGTeam(uint32 team) {
  m_bgData.bgTeam = team;
  SetByteValue(PLAYER_BYTES_3, PLAYER_BYTES_3_OFFSET_ARENA_FACTION,
               uint8(team == ALLIANCE ? 1 : 0));
}

uint32 Player::GetBGTeam() const {
  return m_bgData.bgTeam ? m_bgData.bgTeam : GetTeam();
}

void Player::LeaveBattleground(bool teleportToEntryPoint) {
  if (Battleground *bg = GetBattleground()) {
    bg->RemovePlayerAtLeave(GetGUID(), teleportToEntryPoint, true);

    // call after remove to be sure that player resurrected for correct cast
    if (bg->isBattleground() && !IsGameMaster() &&
        sWorld->getBoolConfig(CONFIG_BATTLEGROUND_CAST_DESERTER)) {
      if (bg->GetStatus() == STATUS_IN_PROGRESS ||
          bg->GetStatus() == STATUS_WAIT_JOIN) {
        // lets check if player was teleported from BG and schedule delayed
        // Deserter spell cast
        if (IsBeingTeleportedFar()) {
          ScheduleDelayedOperation(DELAYED_SPELL_CAST_DESERTER);
          return;
        }

        CastSpell(this, 26013, true); // Deserter
      }
    }

    // track if player leaves the BG while inside it
    if (bg->isBattleground() &&
        sWorld->getBoolConfig(CONFIG_BATTLEGROUND_TRACK_DESERTERS) &&
        (bg->GetStatus() == STATUS_IN_PROGRESS ||
         bg->GetStatus() == STATUS_WAIT_JOIN)) {
      CharacterDatabasePreparedStatement *stmt =
          CharacterDatabase.GetPreparedStatement(CHAR_INS_DESERTER_TRACK);
      stmt->SetData(0, GetGUID().GetCounter());
      stmt->SetData(1, BG_DESERTION_TYPE_LEAVE_BG);
      CharacterDatabase.Execute(stmt);
    }
  }
}

bool Player::CanJoinToBattleground(Battleground const *bg) const {
  // check Deserter debuff
  if (HasAura(26013))
    return false;

  if (bg->isArena() &&
      !GetSession()->HasPermission(rbac::RBAC_PERM_JOIN_ARENAS))
    return false;

  if (bg->IsRandom() &&
      !GetSession()->HasPermission(rbac::RBAC_PERM_JOIN_RANDOM_BG))
    return false;

  if (!GetSession()->HasPermission(rbac::RBAC_PERM_JOIN_NORMAL_BG))
    return false;

  return true;
}

bool Player::CanReportAfkDueToLimit() {
  // a player can complain about 15 people per 5 minutes
  if (m_bgData.bgAfkReportedCount++ >= 15)
    return false;

  return true;
}

/// This player has been blamed to be inactive in a battleground
void Player::ReportedAfkBy(Player *reporter) {
  Battleground *bg = GetBattleground();
  // Battleground also must be in progress!
  if (!bg || bg != reporter->GetBattleground() ||
      GetTeam() != reporter->GetTeam() || bg->GetStatus() != STATUS_IN_PROGRESS)
    return;

  // check if player has 'Idle' or 'Inactive' debuff
  if (m_bgData.bgAfkReporter.find(reporter->GetGUID().GetCounter()) ==
          m_bgData.bgAfkReporter.end() &&
      !HasAura(43680) && !HasAura(43681) &&
      reporter->CanReportAfkDueToLimit()) {
    m_bgData.bgAfkReporter.insert(reporter->GetGUID().GetCounter());
    // by default 3 players have to complain to apply debuff
    if (m_bgData.bgAfkReporter.size() >=
        sWorld->getIntConfig(CONFIG_BATTLEGROUND_REPORT_AFK)) {
      // cast 'Idle' spell
      CastSpell(this, 43680, true);
      m_bgData.bgAfkReporter.clear();
    }
  }
}

WorldLocation Player::GetStartPosition() const {
  PlayerInfo const *info = sObjectMgr->GetPlayerInfo(getRace(), getClass());
  ASSERT(info);
  uint32 mapId = info->mapId;
  if (getClass() == CLASS_DEATH_KNIGHT && HasSpell(50977))
    mapId = 0;
  return WorldLocation(mapId, info->positionX, info->positionY, info->positionZ,
                       0);
}

bool Player::HaveAtClient(Object const *u) const {
  return u == this || m_clientGUIDs.find(u->GetGUID()) != m_clientGUIDs.end();
}

bool Player::IsNeverVisible() const {
  if (Unit::IsNeverVisible())
    return true;

  if (GetSession()->PlayerLogout() || GetSession()->PlayerLoading())
    return true;

  return false;
}

bool Player::CanAlwaysSee(WorldObject const *obj) const {
  // Always can see self
  if (GetCharmedOrSelf() == obj)
    return true;

  if (ObjectGuid guid = GetGuidValue(PLAYER_FARSIGHT))
    if (obj->GetGUID() == guid)
      return true;

  return false;
}

bool Player::IsAlwaysDetectableFor(WorldObject const *seer) const {
  if (Unit::IsAlwaysDetectableFor(seer))
    return true;

  if (Player const *seerPlayer = seer->ToPlayer())
    if (IsGroupVisibleFor(seerPlayer))
      return !(seerPlayer->duel && seerPlayer->duel->startTime != 0 &&
               seerPlayer->duel->opponent == this);

  return false;
}

bool Player::IsVisibleGloballyFor(Player const *u) const {
  if (!u)
    return false;

  // Always can see self
  if (u == this)
    return true;

  // Visible units, always are visible for all players
  if (IsVisible())
    return true;

  // GMs are visible for higher gms (or players are visible for gms)
  if (!AccountMgr::IsPlayerAccount(u->GetSession()->GetSecurity()))
    return GetSession()->GetSecurity() <= u->GetSession()->GetSecurity();

  // non faction visibility non-breakable for non-GMs
  return false;
}

template <class T>
inline void UpdateVisibilityOf_helper(GuidUnorderedSet &s64, T *target,
                                      std::set<Unit *> & /*v*/) {
  s64.insert(target->GetGUID());
}

template <>
inline void UpdateVisibilityOf_helper(GuidUnorderedSet &s64, Creature *target,
                                      std::set<Unit *> &v) {
  s64.insert(target->GetGUID());
  v.insert(target);
}

template <>
inline void UpdateVisibilityOf_helper(GuidUnorderedSet &s64, Player *target,
                                      std::set<Unit *> &v) {
  s64.insert(target->GetGUID());
  v.insert(target);
}

template <class T>
inline void BeforeVisibilityDestroy(T * /*t*/, Player * /*p*/) {}

template <>
inline void BeforeVisibilityDestroy<Creature>(Creature *t, Player *p) {
  if (p->GetPetGUID() == t->GetGUID() && t->IsPet())
    t->ToPet()->Remove(PET_SAVE_DISMISS, true);
}

void Player::UpdateVisibilityOf(WorldObject *target) {
  if (HaveAtClient(target)) {
    if (!CanSeeOrDetect(target, false, true)) {
      if (target->GetTypeId() == TYPEID_UNIT)
        BeforeVisibilityDestroy<Creature>(target->ToCreature(), this);

      if (!target->IsDestroyedObject())
        target->SendOutOfRangeForPlayer(this);
      else
        target->DestroyForPlayer(this);

      m_clientGUIDs.erase(target->GetGUID());

#ifdef FIRELANDS_DEBUG
      LOG_DEBUG(
          "maps",
          "Object %u (Type: %u) out of range for player %u. Distance = %f",
          target->GetGUID().GetCounter(), target->GetTypeId(),
          GetGUID().GetCounter(), GetDistance(target));
#endif
    }
  } else {
    if (CanSeeOrDetect(target, false, true)) {
      target->SendUpdateToPlayer(this);
      m_clientGUIDs.insert(target->GetGUID());

#ifdef FIRELANDS_DEBUG
      LOG_DEBUG(
          "maps",
          "Object %u (Type: %u) is visible now for player %u. Distance = %f",
          target->GetGUID().GetCounter(), target->GetTypeId(),
          GetGUID().GetCounter(), GetDistance(target));
#endif

      // target aura duration for caster show only if target exist at caster
      // client send data at target visibility change (adding to client)
      if (target->isType(TYPEMASK_UNIT))
        SendInitialVisiblePackets(static_cast<Unit *>(target));
    }
  }
}

void Player::UpdateTriggerVisibility() {
  if (m_clientGUIDs.empty())
    return;

  if (!IsInWorld())
    return;

  UpdateData udata(GetMapId());
  WorldPacket packet;
  for (auto itr = m_clientGUIDs.begin(); itr != m_clientGUIDs.end(); ++itr) {
    if (itr->IsCreatureOrVehicle()) {
      Creature *creature = GetMap()->GetCreature(*itr);
      // Update fields of triggers, transformed units or unselectable units
      // (values dependent on GM state)
      if (!creature ||
          (!creature->IsTrigger() &&
           !creature->HasAuraType(SPELL_AURA_TRANSFORM) &&
           !creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE)))
        continue;

      creature->SetFieldNotifyFlag(UF_FLAG_PUBLIC);
      creature->BuildValuesUpdateBlockForPlayer(&udata, this);
      creature->RemoveFieldNotifyFlag(UF_FLAG_PUBLIC);
    } else if (itr->IsGameObject()) {
      GameObject *go = GetMap()->GetGameObject(*itr);
      if (!go)
        continue;

      go->SetFieldNotifyFlag(UF_FLAG_PUBLIC);
      go->BuildValuesUpdateBlockForPlayer(&udata, this);
      go->RemoveFieldNotifyFlag(UF_FLAG_PUBLIC);
    }
  }

  if (!udata.HasData())
    return;

  udata.BuildPacket(&packet);
  SendDirectMessage(&packet);
}

void Player::SendInitialVisiblePackets(Unit *target) const {
  SendAurasForTarget(target);
  if (target->IsAlive()) {
    if (target->HasUnitState(UNIT_STATE_MELEE_ATTACKING) && target->GetVictim())
      target->SendMeleeAttackStart(target->GetVictim());
  }
}

template <class T>
void Player::UpdateVisibilityOf(T *target, UpdateData &data,
                                std::set<Unit *> &visibleNow) {
  if (HaveAtClient(target)) {
    if (!CanSeeOrDetect(target, false, true)) {
      BeforeVisibilityDestroy<T>(target, this);

      if (!target->IsDestroyedObject())
        target->BuildOutOfRangeUpdateBlock(&data);
      else
        target->DestroyForPlayer(this);

      m_clientGUIDs.erase(target->GetGUID());

#ifdef FIRELANDS_DEBUG
      LOG_DEBUG("maps",
                "Object %u (Type: %u, Entry: %u) is out of range for player "
                "%u. Distance = %f",
                target->GetGUID().GetCounter(), target->GetTypeId(),
                target->GetEntry(), GetGUID().GetCounter(),
                GetDistance(target));
#endif
    }
  } else {
    if (CanSeeOrDetect(target, false, true)) {
      target->BuildCreateUpdateBlockForPlayer(&data, this);
      UpdateVisibilityOf_helper(m_clientGUIDs, target, visibleNow);

#ifdef FIRELANDS_DEBUG
      LOG_DEBUG("maps",
                "Object %u (Type: %u, Entry: %u) is visible now for player %u. "
                "Distance = %f",
                target->GetGUID().GetCounter(), target->GetTypeId(),
                target->GetEntry(), GetGUID().GetCounter(),
                GetDistance(target));
#endif
    }
  }
}

template void Player::UpdateVisibilityOf(Player *target, UpdateData &data,
                                         std::set<Unit *> &visibleNow);
template void Player::UpdateVisibilityOf(Creature *target, UpdateData &data,
                                         std::set<Unit *> &visibleNow);
template void Player::UpdateVisibilityOf(Corpse *target, UpdateData &data,
                                         std::set<Unit *> &visibleNow);
template void Player::UpdateVisibilityOf(GameObject *target, UpdateData &data,
                                         std::set<Unit *> &visibleNow);
template void Player::UpdateVisibilityOf(DynamicObject *target,
                                         UpdateData &data,
                                         std::set<Unit *> &visibleNow);
template void Player::UpdateVisibilityOf(AreaTrigger *target, UpdateData &data,
                                         std::set<Unit *> &visibleNow);

void Player::UpdateObjectVisibility(bool forced) {
  // Prevent updating visibility if player is not in world (example: LoadFromDB
  // sets drunkstate which updates invisibility while player is not in map)
  if (!IsInWorld())
    return;

  if (!forced)
    AddToNotify(NOTIFY_VISIBILITY_CHANGED);
  else {
    Unit::UpdateObjectVisibility(true);
    UpdateVisibilityForPlayer();
  }
}

void Player::UpdateVisibilityForPlayer() {
  // updates visibility of all objects around point of view for current player
  Firelands::VisibleNotifier notifier(*this);
  Cell::VisitAllObjects(m_seer, notifier, GetSightRange());
  notifier.SendToSelf(); // send gathered data
}

void Player::InitPrimaryProfessions() {
  SetFreePrimaryProfessions(
      sWorld->getIntConfig(CONFIG_MAX_PRIMARY_TRADE_SKILL));
}

bool Player::ModifyMoney(int64 amount, bool sendError /*= true*/) {
  if (!amount)
    return true;

  sScriptMgr->OnPlayerMoneyChanged(this, amount);

  if (amount < 0)
    SetMoney(GetMoney() > uint64(-amount) ? GetMoney() + amount : 0);
  else {
    if (GetMoney() <= MAX_MONEY_AMOUNT - static_cast<uint64>(amount))
      SetMoney(GetMoney() + amount);
    else {
      sScriptMgr->OnPlayerMoneyLimit(this, amount);

      if (sendError)
        SendEquipError(EQUIP_ERR_TOO_MUCH_GOLD, nullptr, nullptr);
      return false;
    }
  }

  return true;
}

bool Player::HasEnoughMoney(int64 amount) const {
  if (amount > 0)
    return (GetMoney() >= (uint64)amount);
  return true;
}

void Player::SetMoney(uint64 value) {
  SetUInt64Value(PLAYER_FIELD_COINAGE, value);
  MoneyChanged(value);
  UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_GOLD_VALUE_OWNED);
}

bool Player::IsQuestRewarded(uint32 quest_id) const {
  return m_RewardedQuests.find(quest_id) != m_RewardedQuests.end();
}

Unit *Player::GetSelectedUnit() const {
  if (ObjectGuid selectionGUID = GetTarget())
    return ObjectAccessor::GetUnit(*this, selectionGUID);
  return nullptr;
}

Player *Player::GetSelectedPlayer() const {
  if (ObjectGuid selectionGUID = GetTarget())
    return ObjectAccessor::GetPlayer(*this, selectionGUID);
  return nullptr;
}

void Player::SendComboPoints() {

  Unit *combotarget = ObjectAccessor::GetUnit(*this, m_comboTarget);
  if (combotarget) {
    WorldPacket data;
    Player *movingMe = GetCharmerOrSelfPlayer();
    if (movingMe != this) {
      data.Initialize(SMSG_PET_UPDATE_COMBO_POINTS,
                      movingMe->GetPackGUID().size() +
                          combotarget->GetPackGUID().size() + 1);
      data << movingMe->GetPackGUID();
    } else
      data.Initialize(SMSG_UPDATE_COMBO_POINTS,
                      combotarget->GetPackGUID().size() + 1);
    data << combotarget->GetPackGUID();
    data << uint8(m_comboPoints);
    SendDirectMessage(&data);
  }
}

void Player::AddComboPoints(Unit *target, int8 count, Spell *spell) {
  if (!count)
    return;

  int8 *comboPoints = spell ? &spell->m_comboPointGain : &m_comboPoints;

  // without combo points lost (duration checked in aura)
  RemoveAurasByType(SPELL_AURA_RETAIN_COMBO_POINTS);

  if (target->GetGUID() == m_comboTarget)
    *comboPoints += count;
  else {
    if (m_comboTarget)
      if (Unit *target2 = ObjectAccessor::GetUnit(*this, m_comboTarget))
        target2->RemoveComboPointHolder(GetGUID());

    // Spells will always add value to m_comboPoints eventualy, so it must be
    // cleared first
    if (spell)
      m_comboPoints = 0;

    m_comboTarget = target->GetGUID();
    *comboPoints = count;

    target->AddComboPointHolder(GetGUID());
  }

  if (*comboPoints > 5)
    *comboPoints = 5;
  else if (*comboPoints < 0)
    *comboPoints = 0;

  if (!spell)
    SendComboPoints();
}

void Player::GainSpellComboPoints(int8 count) {
  if (!count)
    return;

  m_comboPoints += count;
  if (m_comboPoints > 5)
    m_comboPoints = 5;
  else if (m_comboPoints < 0)
    m_comboPoints = 0;

  SendComboPoints();
}

void Player::ClearComboPoints() {
  if (!m_comboTarget)
    return;

  // without combopoints lost (duration checked in aura)
  RemoveAurasByType(SPELL_AURA_RETAIN_COMBO_POINTS);

  m_comboPoints = 0;

  SendComboPoints();

  if (Unit *target = ObjectAccessor::GetUnit(*this, m_comboTarget))
    target->RemoveComboPointHolder(GetGUID());

  m_comboTarget.Clear();
}

bool Player::IsInGroup(ObjectGuid groupGuid) const {
  if (Group const *group = GetGroup())
    if (group->GetGUID() == groupGuid)
      return true;

  if (Group const *group = GetOriginalGroup())
    if (group->GetGUID() == groupGuid)
      return true;

  return false;
}

void Player::SetGroup(Group *group, int8 subgroup) {
  if (group == nullptr)
    m_group.unlink();
  else {
    // never use SetGroup without a subgroup unless you specify nullptr for
    // group
    ASSERT(subgroup >= 0);
    m_group.link(group, this);
    m_group.setSubGroup((uint8)subgroup);
  }

  UpdateObjectVisibility(false);
}

void Player::SendInitialPacketsBeforeAddToMap(bool firstLogin /*= false*/) {
  /// SMSG_CONTROL_UPDATE
  SetClientControl(this, true);

  /// SMSG_BIND_POINT_UPDATE
  SendBindPointUpdate();

  /// SMSG_WORLD_SERVER_INFO
  WorldPackets::Misc::WorldServerInfo worldServerInfo;
  worldServerInfo.IsTournamentRealm = 0; /// @todo
  // worldServerInfo.RestrictedAccountMaxLevel; /// @todo
  // worldServerInfo.RestrictedAccountMaxMoney; /// @todo
  worldServerInfo.DifficultyID = GetMap()->GetDifficulty();
  worldServerInfo.WeeklyReset = sWorld->GetNextWeeklyQuestsResetTime() - WEEK;
  SendDirectMessage(worldServerInfo.Write());

  /// SMSG_SET_PROFICIENCY
  SendProficiency(ITEM_CLASS_WEAPON, m_WeaponProficiency);
  SendProficiency(ITEM_CLASS_ARMOR, m_ArmorProficiency);

  /// SMSG_SEND_KNOWN_SPELLS
  SendKnownSpells(firstLogin);

  /// SMSG_SEND_UNLEARN_SPELLS
  SendDirectMessage(WorldPackets::Spells::SendUnlearnSpells().Write());

  /// SMSG_UPDATE_TALENT_DATA
  SendTalentsInfoData(false);

  /// SMSG_UPDATE_ACTION_BUTTONS
  SendInitialActionButtons();

  /// SMSG_INITIALIZE_FACTIONS
  m_reputationMgr->SendInitialReputations();

  /// MSG_SET_DUNGEON_DIFFICULTY
  // Handled in WorldSession::HandlePlayerLogin

  /// SMSG_ACCOUNT_DATA_TIMES
  // Handled in WorldSession::HandlePlayerLogin

  /// Pass 'this' as argument because we're not stored in ObjectAccessor yet
  GetSocial()->SendSocialList(this, SOCIAL_FLAG_ALL);

  /// SMSG_MOTD
  // Handled in WorldSession::HandlePlayerLogin

  /// SMSG_FEATURE_SYSTEM_STATUS
  // Handled in WorldSession::HandlePlayerLogin

  /// SMSG_RESYNC_RUNES
  if (getClass() == CLASS_DEATH_KNIGHT) {
    // Initialize rune cooldowns
    ResyncRunes();

    // Send already converted runes
    SendConvertedRunes();
  }

  // Spell modifiers
  SendSpellModifiers();

  /// SMSG_SET_FORCED_REACTIONS
  GetReputationMgr().SendForceReactions();

  /// SMSG_SETUP_CURRENCY
  SendCurrencies();

  /// SMSG_WEEKLY_SPELL_USAGE

  ///  SMSG_ALL_ACHIEVEMENT_DATA
  m_achievementMgr->SendAllAchievementData(this);

  /// SMSG_LOGIN_SET_TIME_SPEED
  static float const TimeSpeed = 0.016666667535901069f;
  WorldPackets::Misc::LoginSetTimeSpeed loginSetTimeSpeed;
  loginSetTimeSpeed.NewSpeed = TimeSpeed;
  loginSetTimeSpeed.GameTime = GameTime::GetGameTime();
  loginSetTimeSpeed.GameTimeHolidayOffset = 0; /// @todo
  SendDirectMessage(loginSetTimeSpeed.Write());

  /// SMSG_EQUIPMENT_SET_LIST
  SendEquipmentSetList();

  // SMSG_TALENTS_INFO x 2 for pet (unspent points and talents in separate
  // packets...) SMSG_PET_GUIDS SMSG_UPDATE_WORLD_STATE SMSG_POWER_UPDATE

  GetGameClient()->SetMovedUnit(this, true);
}

void Player::SendInitialPacketsAfterAddToMap() {
  UpdateVisibilityForPlayer();

  // update zone
  uint32 newzone, newarea;
  GetZoneAndAreaId(newzone, newarea);
  UpdateZone(newzone,
             newarea); // also calls SendInitWorldStates and SendZoneDynamicInfo

  GetSession()->ResetTimeSync();
  GetSession()->SendTimeSync();

  GetSession()->SendLoadCUFProfiles();

  CastSpell(this, 836, true); // LOGINEFFECT

  // set some aura effects that send packet to player client after add player to
  // map SendMessageToSet not send it to player not it map, only for aura that
  // not changed anything at re-apply same auras state lost at far teleport,
  // send it one more time in this case also
  static const AuraType auratypes[] = {
      SPELL_AURA_MOD_FEAR,   SPELL_AURA_TRANSFORM,
      SPELL_AURA_WATER_WALK, SPELL_AURA_FEATHER_FALL,
      SPELL_AURA_HOVER,      SPELL_AURA_SAFE_FALL,
      SPELL_AURA_FLY,        SPELL_AURA_MOD_INCREASE_MOUNTED_FLIGHT_SPEED,
      SPELL_AURA_NONE};
  for (AuraType const *itr = &auratypes[0]; itr && itr[0] != SPELL_AURA_NONE;
       ++itr) {
    Unit::AuraEffectList const &auraList = GetAuraEffectsByType(*itr);
    if (!auraList.empty())
      auraList.front()->HandleEffect(this, AURA_EFFECT_HANDLE_SEND_FOR_CLIENT,
                                     true);
  }

  if (HasAuraType(SPELL_AURA_MOD_STUN))
    SetRooted(true);

  // manual send package (have code in HandleEffect(this,
  // AURA_EFFECT_HANDLE_SEND_FOR_CLIENT, true); that must not be re-applied.
  if (HasAuraType(SPELL_AURA_MOD_ROOT))
    SetRooted(true, true);

  SendAurasForTarget(this);
  SendEnchantmentDurations(); // must be after add to map
  SendItemDurations();        // must be after add to map
  SendQuestGiverStatusMultiple();
  SendTaxiNodeStatusMultiple();

  // raid downscaling - send difficulty to player
  if (GetMap()->IsRaid()) {
    if (GetMap()->GetDifficulty() != GetRaidDifficulty()) {
      StoreRaidMapDifficulty();
      SendRaidDifficulty(GetGroup() != nullptr, GetStoredRaidDifficulty());
    }
  } else if (GetRaidDifficulty() != GetStoredRaidDifficulty())
    SendRaidDifficulty(GetGroup() != nullptr);

  if (GetPlayerSharingQuest()) {
    if (Quest const *quest = sObjectMgr->GetQuestTemplate(GetSharedQuestID())) {
      SetPopupQuestId(0);
      PlayerTalkClass->SendQuestGiverQuestDetails(quest, GetGUID(), true,
                                                  false);
    } else
      ClearQuestSharingInfo();
  }

  PhasingHandler::OnMapChange(this);
}

void Player::SendUpdateToOutOfRangeGroupMembers() {
  if (m_groupUpdateMask == GROUP_UPDATE_FLAG_NONE)
    return;

  if (Group *group = GetGroup())
    group->UpdatePlayerOutOfRange(this);

  m_groupUpdateMask = GROUP_UPDATE_FLAG_NONE;
  m_auraRaidUpdateMask = 0;
  if (Pet *pet = GetPet())
    pet->ResetAuraUpdateMaskForRaid();
}

void Player::SendTransferAborted(uint32 mapid, TransferAbortReason reason,
                                 uint8 arg) const {
  WorldPackets::Movement::TransferAborted packet;
  packet.MapID = mapid;
  packet.TransfertAbort = reason; // transfer abort reason
  packet.Arg = arg;
  SendDirectMessage(packet.Write());
}

void Player::SendInstanceResetWarning(uint32 mapid, Difficulty difficulty,
                                      uint32 time, bool welcome) const {
  // type of warning, based on the time remaining until reset
  uint32 type;
  if (welcome)
    type = RAID_INSTANCE_WELCOME;
  else if (time > 21600)
    type = RAID_INSTANCE_WELCOME;
  else if (time > 3600)
    type = RAID_INSTANCE_WARNING_HOURS;
  else if (time > 300)
    type = RAID_INSTANCE_WARNING_MIN;
  else
    type = RAID_INSTANCE_WARNING_MIN_SOON;

  WorldPackets::Instance::RaidInstanceMessage raidInstanceMessage;
  raidInstanceMessage.Type = type;
  raidInstanceMessage.MapID = mapid;
  raidInstanceMessage.Difficulty = difficulty;
  raidInstanceMessage.TimeLeft = time;
  if (InstancePlayerBind const *bind = GetBoundInstance(mapid, difficulty))
    raidInstanceMessage.Locked = bind->perm;
  else
    raidInstanceMessage.Locked = false;
  raidInstanceMessage.Extended = false;
  SendDirectMessage(raidInstanceMessage.Write());
}

void Player::ApplyEquipCooldown(Item *pItem) {
  if (pItem->GetTemplate()->GetFlags() & ITEM_FLAG_NO_EQUIP_COOLDOWN)
    return;

  std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
  for (ItemEffect const &effect : pItem->GetTemplate()->Effects) {
    // no spell
    if (effect.SpellID <= 0)
      continue;

    // apply proc cooldown to equip auras if we have any
    if (effect.Trigger == ITEM_SPELLTRIGGER_ON_EQUIP) {
      SpellProcEntry const *procEntry =
          sSpellMgr->GetSpellProcEntry(effect.SpellID);
      if (!procEntry)
        continue;

      if (Aura *itemAura = GetAura(effect.SpellID, GetGUID(), pItem->GetGUID()))
        itemAura->AddProcCooldown(now + procEntry->Cooldown);
      continue;
    }

    // wrong triggering type (note: ITEM_SPELLTRIGGER_ON_NO_DELAY_USE not have
    // cooldown)
    if (effect.Trigger != ITEM_SPELLTRIGGER_ON_USE)
      continue;

    // Don't replace longer cooldowns by equip cooldown if we have any.
    if (GetSpellHistory()->GetRemainingCooldown(
            sSpellMgr->AssertSpellInfo(effect.SpellID)) > 30 * IN_MILLISECONDS)
      continue;

    GetSpellHistory()->AddCooldown(effect.SpellID, pItem->GetEntry(),
                                   std::chrono::seconds(30));

    WorldPacket data(SMSG_ITEM_COOLDOWN, 8 + 4);
    data << uint64(pItem->GetGUID());
    data << uint32(effect.SpellID);
    SendDirectMessage(&data);
  }
}

void Player::ResetSpells(bool myClassOnly) {
  // not need after this call
  if (HasAtLoginFlag(AT_LOGIN_RESET_SPELLS))
    RemoveAtLoginFlag(AT_LOGIN_RESET_SPELLS, true);

  // make full copy of map (spells removed and marked as deleted at another
  // spell remove and we can't use original map for safe iterative with visit
  // each spell at loop end
  PlayerSpellMap smap = GetSpellMap();

  uint32 family;

  if (myClassOnly) {
    ChrClassesEntry const *clsEntry = sChrClassesStore.LookupEntry(getClass());
    if (!clsEntry)
      return;
    family = clsEntry->SpellClassSet;

    for (PlayerSpellMap::const_iterator iter = smap.begin(); iter != smap.end();
         ++iter) {
      SpellInfo const *spellInfo = sSpellMgr->GetSpellInfo(iter->first);
      if (!spellInfo)
        continue;

      // skip server-side/triggered spells
      if (spellInfo->SpellLevel == 0)
        continue;

      // skip wrong class/race skills
      if (!IsSpellFitByClassAndRace(spellInfo->Id))
        continue;

      // skip other spell families
      if (spellInfo->SpellFamilyName != family)
        continue;

      // skip spells with first rank learned as talent (and all talents then
      // also)
      uint32 firstRank = spellInfo->GetFirstRankSpell()->Id;
      if (sDBCManager.GetTalentSpellCost(firstRank) > 0)
        continue;

      // skip broken spells
      if (!SpellMgr::IsSpellValid(spellInfo, this, false))
        continue;
    }
  } else
    for (PlayerSpellMap::const_iterator iter = smap.begin(); iter != smap.end();
         ++iter)
      RemoveSpell(iter->first, false,
                  false); // only iter->first can be accessed, object by
                          // iter->second can be deleted already

  LearnDefaultSkills();
  LearnCustomSpells();
  LearnQuestRewardedSpells();
}

void Player::LearnCustomSpells() {
  if (!sWorld->getBoolConfig(CONFIG_START_ALL_SPELLS))
    return;

  // learn default race/class spells
  PlayerInfo const *info = sObjectMgr->GetPlayerInfo(getRace(), getClass());
  ASSERT(info);
  for (PlayerCreateInfoSpells::const_iterator itr = info->customSpells.begin();
       itr != info->customSpells.end(); ++itr) {
    uint32 tspell = *itr;
    LOG_DEBUG("entities.player.loading",
              "Player::LearnCustomSpells: Player '%s' (%s, Class: %u Race: "
              "%u): Adding initial spell (SpellID: %u)",
              GetName().c_str(), GetGUID().ToString().c_str(),
              uint32(getClass()), uint32(getRace()), tspell);
    if (!IsInWorld()) // will send in INITIAL_SPELLS in list anyway at map add
      AddSpell(tspell, true, true, true, false);
    else // but send in normal spell in game learn case
      LearnSpell(tspell, true);
  }
}

void Player::LearnDefaultSkills() {
  // learn default race/class skills
  PlayerInfo const *info = sObjectMgr->GetPlayerInfo(getRace(), getClass());
  ASSERT(info);
  for (PlayerCreateInfoSkills::const_iterator itr = info->skills.begin();
       itr != info->skills.end(); ++itr) {
    uint32 skillId = itr->SkillId;
    if (HasSkill(skillId))
      continue;

    LearnDefaultSkill(skillId, itr->Rank);
  }
}

void Player::LearnDefaultSkill(uint32 skillId, uint16 rank) {
  SkillRaceClassInfoEntry const *rcInfo =
      sDBCManager.GetSkillRaceClassInfo(skillId, getRace(), getClass());
  if (!rcInfo)
    return;

  LOG_DEBUG("entities.player.loading",
            "PLAYER (Class: %u Race: %u): Adding initial skill, id = %u",
            uint32(getClass()), uint32(getRace()), skillId);
  switch (GetSkillRangeType(rcInfo)) {
  case SKILL_RANGE_LANGUAGE:
    SetSkill(skillId, 0, 300, 300);
    break;
  case SKILL_RANGE_LEVEL: {
    uint16 skillValue = 1;
    uint16 maxValue = GetMaxSkillValueForLevel();
    if (rcInfo->Flags & SKILL_FLAG_ALWAYS_MAX_VALUE)
      skillValue = maxValue;
    else if (getClass() == CLASS_DEATH_KNIGHT)
      skillValue = std::min(std::max<uint16>({1, uint16((getLevel() - 1) * 5)}),
                            maxValue);
    else if (skillId == SKILL_FIST_WEAPONS)
      skillValue = std::max<uint16>(1, GetSkillValue(SKILL_UNARMED));
    else if (skillId == SKILL_LOCKPICKING)
      skillValue = std::max<uint16>(1, GetSkillValue(SKILL_LOCKPICKING));

    SetSkill(skillId, 0, skillValue, maxValue);
    break;
  }
  case SKILL_RANGE_MONO:
    SetSkill(skillId, 0, 1, 1);
    break;
  case SKILL_RANGE_RANK: {
    if (!rank)
      break;

    SkillTiersEntry const *tier =
        sSkillTiersStore.LookupEntry(rcInfo->SkillTierID);
    uint16 maxValue = tier->Value[std::max<int32>(rank - 1, 0)];
    uint16 skillValue = 1;
    if (rcInfo->Flags & SKILL_FLAG_ALWAYS_MAX_VALUE)
      skillValue = maxValue;
    else if (getClass() == CLASS_DEATH_KNIGHT)
      skillValue =
          std::min(std::max<uint16>({uint16(1), uint16((getLevel() - 1) * 5)}),
                   maxValue);

    SetSkill(skillId, 1, skillValue, maxValue);
    break;
  }
  default:
    break;
  }
}

void Player::LearnQuestRewardedSpells(Quest const *quest) {
  int32 spell_id = quest->GetRewSpellCast();
  uint32 src_spell_id = quest->GetSrcSpell();

  // skip quests without rewarded spell
  if (!spell_id)
    return;

  // if RewSpellCast = -1 we remove aura do to SrcSpell from player.
  if (spell_id == -1 && src_spell_id) {
    RemoveAurasDueToSpell(src_spell_id);
    return;
  }

  SpellInfo const *spellInfo = sSpellMgr->GetSpellInfo(spell_id);
  if (!spellInfo)
    return;

  // check learned spells state
  bool found = false;
  for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i) {
    if (spellInfo->Effects[i].Effect == SPELL_EFFECT_LEARN_SPELL &&
        !HasSpell(spellInfo->Effects[i].TriggerSpell)) {
      found = true;
      break;
    }
  }

  // skip quests with not teaching spell or already known spell
  if (!found)
    return;

  uint32 learned_0 = spellInfo->Effects[0].TriggerSpell;
  if (!HasSpell(learned_0)) {
    SpellInfo const *learnedInfo = sSpellMgr->GetSpellInfo(learned_0);
    if (!learnedInfo)
      return;

    // profession specialization can be re-learned from npc
    if (learnedInfo->Effects[0].Effect == SPELL_EFFECT_TRADE_SKILL &&
        learnedInfo->Effects[1].Effect == 0 && !learnedInfo->SpellLevel)
      return;
  }

  CastSpell(this, spell_id, true);
}

void Player::LearnQuestRewardedSpells() {
  // learn spells received from quest completing
  for (RewardedQuestSet::const_iterator itr = m_RewardedQuests.begin();
       itr != m_RewardedQuests.end(); ++itr) {
    Quest const *quest = sObjectMgr->GetQuestTemplate(*itr);
    if (!quest)
      continue;

    LearnQuestRewardedSpells(quest);
  }
}

void Player::LearnSkillRewardedSpells(uint32 skillId, uint32 skillValue) {
  uint32 raceMask = getRaceMask();
  uint32 classMask = getClassMask();
  std::vector<SkillLineAbilityEntry const *> const *skillLineAbilities =
      sDBCManager.GetSkillLineAbilitiesBySkill(skillId);
  if (!skillLineAbilities)
    return;

  for (SkillLineAbilityEntry const *ability : *skillLineAbilities) {
    SpellInfo const *spellInfo = sSpellMgr->GetSpellInfo(ability->Spell);
    if (!spellInfo)
      continue;

    switch (ability->AcquireMethod) {
    case SKILL_LINE_ABILITY_LEARNED_ON_SKILL_VALUE:
    case SKILL_LINE_ABILITY_LEARNED_ON_SKILL_LEARN:
      break;
    default:
      continue;
    }

    // AcquireMethod == 2 && NumSkillUps == 1 --> automatically learn riding
    // skill spell, else we skip it (client shows riding in spellbook as
    // trainable).
    if (skillId == SKILL_RIDING &&
        (ability->AcquireMethod != SKILL_LINE_ABILITY_LEARNED_ON_SKILL_LEARN ||
         ability->NumSkillUps != 1))
      continue;

    // Check race if set
    if (ability->RaceMask && !(ability->RaceMask & raceMask))
      continue;

    // Check class if set
    if (ability->ClassMask && !(ability->ClassMask & classMask))
      continue;

    // check level, skip class spells if not high enough
    if (getLevel() < spellInfo->SpellLevel)
      continue;

    // need unlearn spell
    if (skillValue < ability->MinSkillLineRank &&
        ability->AcquireMethod == SKILL_LINE_ABILITY_LEARNED_ON_SKILL_VALUE)
      RemoveSpell(ability->Spell);

    // need learn
    else if (!IsInWorld())
      AddSpell(ability->Spell, true, true, true, false, false,
               ability->SkillLine);
    else
      LearnSpell(ability->Spell, true, ability->SkillLine);
  }
}

uint32 Player::GetSkillRank(uint8 pos) const {
  uint16 field = pos / 2;
  uint8 offset = pos & 1;

  return int32(GetUInt16Value(PLAYER_SKILL_RANK_0 + field, offset));
}

void Player::SetSkillLineId(uint8 pos, uint16 value) {
  uint16 field = pos / 2;
  uint8 offset = pos & 1;
  SetUInt16Value(PLAYER_SKILL_LINEID_0 + field, offset, value);
}

void Player::SetSkillStep(uint8 pos, uint16 value) {
  uint16 field = pos / 2;
  uint8 offset = pos & 1;
  SetUInt16Value(PLAYER_SKILL_STEP_0 + field, offset, value);
}
void Player::SetSkillRank(uint8 pos, uint16 value) {
  uint16 field = pos / 2;
  uint8 offset = pos & 1;
  SetUInt16Value(PLAYER_SKILL_RANK_0 + field, offset, value);
}

void Player::SetSkillMaxRank(uint8 pos, uint16 value) {
  uint16 field = pos / 2;
  uint8 offset = pos & 1;
  SetUInt16Value(PLAYER_SKILL_MAX_RANK_0 + field, offset, value);
}

void Player::SetSkillTempBonus(uint8 pos, uint16 value) {
  uint16 field = pos / 2;
  uint8 offset = pos & 1;
  SetUInt16Value(PLAYER_SKILL_MODIFIER_0 + field, offset, value);
}

void Player::SetSkillPermBonus(uint8 pos, uint16 value) {
  uint16 field = pos / 2;
  uint8 offset = pos & 1;
  SetUInt16Value(PLAYER_SKILL_TALENT_0 + field, offset, value);
}

void Player::SendAurasForTarget(Unit *target) const {
  if (!target || target->GetVisibleAuras()->empty()) // speedup things
    return;

  /*! Blizz sends certain movement packets sometimes even before CreateObject
      These movement packets are usually found in SMSG_COMPRESSED_MOVES
  */
  if (target->HasAuraType(SPELL_AURA_FEATHER_FALL))
    target->SetFeatherFall(true, true);

  if (target->HasAuraType(SPELL_AURA_WATER_WALK))
    target->SetWaterWalking(true, true);

  if (target->HasAuraType(SPELL_AURA_HOVER))
    target->SetHover(true, true);

  Unit::VisibleAuraMap const *visibleAuras = target->GetVisibleAuras();

  WorldPackets::Spells::AuraUpdateAll update;
  update.UnitGUID = target->GetGUID();
  update.Auras.reserve(visibleAuras->size());

  for (Unit::VisibleAuraMap::const_iterator itr = visibleAuras->begin();
       itr != visibleAuras->end(); ++itr) {
    WorldPackets::Spells::AuraInfo auraInfo;
    AuraApplication *auraApp = itr->second;
    auraApp->BuildUpdatePacket(auraInfo, false);
    update.Auras.push_back(auraInfo);
  }
  GetSession()->SendPacket(update.Write());
}

void Player::SetDailyQuestStatus(uint32 quest_id) {
  if (sObjectMgr->GetQuestTemplate(quest_id)) {
    for (uint32 quest_daily_idx = 0; quest_daily_idx < PLAYER_MAX_DAILY_QUESTS;
         ++quest_daily_idx) {
      if (!GetUInt32Value(PLAYER_FIELD_DAILY_QUESTS_1 + quest_daily_idx)) {
        SetUInt32Value(PLAYER_FIELD_DAILY_QUESTS_1 + quest_daily_idx, quest_id);
        m_lastDailyQuestTime = GameTime::GetGameTime(); // last daily quest time
        m_DailyQuestChanged = true;
        break;
      }
    }
  }
}

bool Player::IsDailyQuestDone(uint32 quest_id) {
  bool found = false;
  if (sObjectMgr->GetQuestTemplate(quest_id)) {
    for (uint32 quest_daily_idx = 0; quest_daily_idx < PLAYER_MAX_DAILY_QUESTS;
         ++quest_daily_idx) {
      if (GetUInt32Value(PLAYER_FIELD_DAILY_QUESTS_1 + quest_daily_idx) ==
          quest_id) {
        found = true;
        break;
      }
    }
  }

  return found;
}

void Player::SetWeeklyQuestStatus(uint32 quest_id) {
  m_weeklyquests.insert(quest_id);
  m_WeeklyQuestChanged = true;
}

void Player::SetSeasonalQuestStatus(uint32 quest_id) {
  Quest const *quest = sObjectMgr->GetQuestTemplate(quest_id);
  if (!quest)
    return;

  m_seasonalquests[quest->GetEventIdForQuest()].insert(quest_id);
  m_SeasonalQuestChanged = true;
}

void Player::SetMonthlyQuestStatus(uint32 quest_id) {
  m_monthlyquests.insert(quest_id);
  m_MonthlyQuestChanged = true;
}

void Player::SetLFGRewardStatus(uint32 dungeon_id, bool daily_reset) {
  LFGRewardStatusMap::iterator lfgdungeon = m_lfgrewardstatus.find(dungeon_id);

  if (lfgdungeon != m_lfgrewardstatus.end())
    lfgdungeon->second.CompletionsThisPeriod++;
  else
    m_lfgrewardstatus[dungeon_id] = LFGRewardInfo(1, daily_reset);

  m_LFGRewardStatusChanged = true;
}

void Player::ResetDailyQuestStatus() {
  for (uint32 quest_daily_idx = 0; quest_daily_idx < PLAYER_MAX_DAILY_QUESTS;
       ++quest_daily_idx)
    SetUInt32Value(PLAYER_FIELD_DAILY_QUESTS_1 + quest_daily_idx, 0);

  // DB data deleted in caller
  m_DailyQuestChanged = false;
  m_lastDailyQuestTime = 0;
}

void Player::ResetWeeklyQuestStatus() {
  if (m_weeklyquests.empty())
    return;

  m_weeklyquests.clear();
  // DB data deleted in caller
  m_WeeklyQuestChanged = false;
}

void Player::ResetSeasonalQuestStatus(uint16 event_id) {
  if (m_seasonalquests.empty() || m_seasonalquests[event_id].empty())
    return;

  m_seasonalquests.erase(event_id);
  // DB data deleted in caller
  m_SeasonalQuestChanged = false;
}

void Player::ResetMonthlyQuestStatus() {
  if (m_monthlyquests.empty())
    return;

  m_monthlyquests.clear();
  // DB data deleted in caller
  m_MonthlyQuestChanged = false;
}

void Player::ResetWeeklyLFGRewardStatus() {
  for (auto itr = m_lfgrewardstatus.begin(); itr != m_lfgrewardstatus.end();) {
    if (!itr->second.IsDaily) {
      itr = m_lfgrewardstatus.erase(itr);
      continue;
    }
    itr++;
  }

  // DB data deleted in caller
  m_LFGRewardStatusChanged = false;
}

void Player::ResetDailyLFGRewardStatus() {
  for (auto itr = m_lfgrewardstatus.begin(); itr != m_lfgrewardstatus.end();) {
    if (itr->second.IsDaily) {
      itr = m_lfgrewardstatus.erase(itr);
      continue;
    }
    itr++;
  }

  // DB data deleted in caller
  m_LFGRewardStatusChanged = false;
}

Battleground *Player::GetBattleground() const {
  if (GetBattlegroundId() == 0)
    return nullptr;

  return sBattlegroundMgr->GetBattleground(GetBattlegroundId(),
                                           m_bgData.bgTypeID);
}

uint32 Player::GetBattlegroundQueueJoinTime(uint32 bgTypeId) const {
  std::map<uint32, uint32>::const_iterator itr =
      m_bgData.bgQueuesJoinedTime.find(bgTypeId);
  if (itr != m_bgData.bgQueuesJoinedTime.end())
    return itr->second;

  return GameTime::GetGameTimeMS();
}

void Player::AddBattlegroundQueueJoinTime(uint32 bgTypeId, uint32 joinTime) {
  m_bgData.bgQueuesJoinedTime[bgTypeId] = joinTime;
}

void Player::RemoveBattlegroundQueueJoinTime(uint32 bgTypeId) {
  std::map<uint32, uint32>::const_iterator itr =
      m_bgData.bgQueuesJoinedTime.find(bgTypeId);
  if (itr != m_bgData.bgQueuesJoinedTime.end())
    m_bgData.bgQueuesJoinedTime.erase(itr->second);
}

bool Player::InBattlegroundQueue() const {
  for (uint8 i = 0; i < PLAYER_MAX_BATTLEGROUND_QUEUES; ++i)
    if (m_bgBattlegroundQueueID[i].bgQueueTypeId != BATTLEGROUND_QUEUE_NONE)
      return true;
  return false;
}

BattlegroundQueueTypeId Player::GetBattlegroundQueueTypeId(uint32 index) const {
  return m_bgBattlegroundQueueID[index].bgQueueTypeId;
}

uint32
Player::GetBattlegroundQueueIndex(BattlegroundQueueTypeId bgQueueTypeId) const {
  for (uint8 i = 0; i < PLAYER_MAX_BATTLEGROUND_QUEUES; ++i)
    if (m_bgBattlegroundQueueID[i].bgQueueTypeId == bgQueueTypeId)
      return i;
  return PLAYER_MAX_BATTLEGROUND_QUEUES;
}

bool Player::IsInvitedForBattlegroundQueueType(
    BattlegroundQueueTypeId bgQueueTypeId) const {
  for (uint8 i = 0; i < PLAYER_MAX_BATTLEGROUND_QUEUES; ++i)
    if (m_bgBattlegroundQueueID[i].bgQueueTypeId == bgQueueTypeId)
      return m_bgBattlegroundQueueID[i].invitedToInstance != 0;
  return false;
}

bool Player::InBattlegroundQueueForBattlegroundQueueType(
    BattlegroundQueueTypeId bgQueueTypeId) const {
  return GetBattlegroundQueueIndex(bgQueueTypeId) <
         PLAYER_MAX_BATTLEGROUND_QUEUES;
}

void Player::SetBattlegroundId(uint32 val, BattlegroundTypeId bgTypeId) {
  m_bgData.bgInstanceID = val;
  m_bgData.bgTypeID = bgTypeId;
}

uint32 Player::AddBattlegroundQueueId(BattlegroundQueueTypeId val) {
  for (uint8 i = 0; i < PLAYER_MAX_BATTLEGROUND_QUEUES; ++i) {
    if (m_bgBattlegroundQueueID[i].bgQueueTypeId == BATTLEGROUND_QUEUE_NONE ||
        m_bgBattlegroundQueueID[i].bgQueueTypeId == val) {
      m_bgBattlegroundQueueID[i].bgQueueTypeId = val;
      m_bgBattlegroundQueueID[i].invitedToInstance = 0;
      return i;
    }
  }
  return PLAYER_MAX_BATTLEGROUND_QUEUES;
}

bool Player::HasFreeBattlegroundQueueId() const {
  for (uint8 i = 0; i < PLAYER_MAX_BATTLEGROUND_QUEUES; ++i)
    if (m_bgBattlegroundQueueID[i].bgQueueTypeId == BATTLEGROUND_QUEUE_NONE)
      return true;
  return false;
}

void Player::RemoveBattlegroundQueueId(BattlegroundQueueTypeId val) {
  for (uint8 i = 0; i < PLAYER_MAX_BATTLEGROUND_QUEUES; ++i) {
    if (m_bgBattlegroundQueueID[i].bgQueueTypeId == val) {
      m_bgBattlegroundQueueID[i].bgQueueTypeId = BATTLEGROUND_QUEUE_NONE;
      m_bgBattlegroundQueueID[i].invitedToInstance = 0;
      return;
    }
  }
}

void Player::SetInviteForBattlegroundQueueType(
    BattlegroundQueueTypeId bgQueueTypeId, uint32 instanceId) {
  for (uint8 i = 0; i < PLAYER_MAX_BATTLEGROUND_QUEUES; ++i)
    if (m_bgBattlegroundQueueID[i].bgQueueTypeId == bgQueueTypeId)
      m_bgBattlegroundQueueID[i].invitedToInstance = instanceId;
}

bool Player::IsInvitedForBattlegroundInstance(uint32 instanceId) const {
  for (uint8 i = 0; i < PLAYER_MAX_BATTLEGROUND_QUEUES; ++i)
    if (m_bgBattlegroundQueueID[i].invitedToInstance == instanceId)
      return true;
  return false;
}

bool Player::InArena() const {
  Battleground *bg = GetBattleground();
  if (!bg || !bg->isArena())
    return false;

  return true;
}

bool Player::GetBGAccessByLevel(BattlegroundTypeId bgTypeId) const {
  // get a template bg instead of running one
  Battleground *bg = sBattlegroundMgr->GetBattlegroundTemplate(bgTypeId);
  if (!bg)
    return false;

  // limit check leel to dbc compatible level range
  uint32 level = getLevel();
  if (level > DEFAULT_MAX_LEVEL)
    level = DEFAULT_MAX_LEVEL;

  if (level < bg->GetMinLevel() || level > bg->GetMaxLevel())
    return false;

  return true;
}

float Player::GetReputationPriceDiscount(Creature const *creature) const {
  ChrRacesEntry const *raceEntry = sChrRacesStore.LookupEntry(getRace());
  if (raceEntry->Flags & 0x100)
    return 0.8f;

  FactionTemplateEntry const *vendor_faction =
      creature->GetFactionTemplateEntry();
  if (!vendor_faction || !vendor_faction->Faction)
    return 1.0f;

  ReputationRank rank = GetReputationRank(vendor_faction->Faction);
  if (rank <= REP_NEUTRAL)
    return 1.0f;

  return 1.0f - 0.05f * (rank - REP_NEUTRAL);
}

Player *Player::GetTrader() const {
  return m_trade ? m_trade->GetTrader() : nullptr;
}

bool Player::IsSpellFitByClassAndRace(uint32 spell_id) const {
  uint32 racemask = getRaceMask();
  uint32 classmask = getClassMask();

  SkillLineAbilityMapBounds bounds =
      sSpellMgr->GetSkillLineAbilityMapBounds(spell_id);
  if (bounds.first == bounds.second)
    return true;

  for (SkillLineAbilityMap::const_iterator _spell_idx = bounds.first;
       _spell_idx != bounds.second; ++_spell_idx) {
    // skip wrong race skills
    if (_spell_idx->second->RaceMask &&
        (_spell_idx->second->RaceMask & racemask) == 0)
      continue;

    // skip wrong class skills
    if (_spell_idx->second->ClassMask &&
        (_spell_idx->second->ClassMask & classmask) == 0)
      continue;

    // skip wrong class and race skill saved in SkillRaceClassInfo.dbc
    if (!sDBCManager.GetSkillRaceClassInfo(_spell_idx->second->SkillLine,
                                           getRace(), getClass()))
      continue;

    return true;
  }

  return false;
}

bool Player::HasQuestForGO(int32 GOId) const {
  for (uint8 i = 0; i < MAX_QUEST_LOG_SIZE; ++i) {
    uint32 questid = GetQuestSlotQuestId(i);
    if (questid == 0)
      continue;

    QuestStatusMap::const_iterator qs_itr = m_QuestStatus.find(questid);
    if (qs_itr == m_QuestStatus.end())
      continue;

    QuestStatusData const &qs = qs_itr->second;

    if (qs.Status == QUEST_STATUS_INCOMPLETE) {
      Quest const *qinfo = sObjectMgr->GetQuestTemplate(questid);
      if (!qinfo)
        continue;

      if (GetGroup() && GetGroup()->isRaidGroup() &&
          !qinfo->IsAllowedInRaid(GetMap()->GetDifficulty()))
        continue;

      for (uint8 j = 0; j < QUEST_OBJECTIVES_COUNT; ++j) {
        if (qinfo->RequiredNpcOrGo[j] >= 0) // skip non GO case
          continue;

        if ((-1) * GOId == qinfo->RequiredNpcOrGo[j] &&
            qs.CreatureOrGOCount[j] < qinfo->RequiredNpcOrGoCount[j])
          return true;
      }
    }
  }
  return false;
}

void Player::UpdateVisibleGameobjectsOrSpellClicks() {
  if (m_clientGUIDs.empty())
    return;

  UpdateData udata(GetMapId());
  WorldPacket packet;
  for (auto itr = m_clientGUIDs.begin(); itr != m_clientGUIDs.end(); ++itr) {
    if (itr->IsGameObject()) {
      if (GameObject *obj = ObjectAccessor::GetGameObject(*this, *itr))
        obj->BuildValuesUpdateBlockForPlayer(&udata, this);
    } else if (itr->IsCreatureOrVehicle()) {
      Creature *obj = ObjectAccessor::GetCreatureOrPetOrVehicle(*this, *itr);
      if (!obj)
        continue;

      // check if this unit requires quest specific flags
      if (!obj->HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK))
        continue;

      SpellClickInfoMapBounds clickPair =
          sObjectMgr->GetSpellClickInfoMapBounds(obj->GetEntry());
      for (SpellClickInfoContainer::const_iterator _itr = clickPair.first;
           _itr != clickPair.second; ++_itr) {
        if (sConditionMgr->GetConditionsForSpellClickEvent(
                obj->GetEntry(), _itr->second.spellId)) {
          obj->BuildValuesUpdateBlockForPlayer(&udata, this);
          break;
        }
      }
    }
  }
  udata.BuildPacket(&packet);
  SendDirectMessage(&packet);
}

bool Player::HasSummonPending() const {
  return m_summon_expire >= GameTime::GetGameTime();
}

void Player::SendSummonRequestFrom(Unit *summoner) {
  if (!summoner)
    return;

  // Player already has active summon request
  if (HasSummonPending())
    return;

  // Evil Twin (ignore player summon, but hide this for summoner)
  if (HasAura(23445))
    return;

  m_summon_expire = GameTime::GetGameTime() + MAX_PLAYER_SUMMON_DELAY;
  m_summon_location.WorldRelocate(*summoner);

  WorldPacket data(SMSG_SUMMON_REQUEST, 8 + 4 + 4);
  data << uint64(summoner->GetGUID());   // summoner guid
  data << uint32(summoner->GetZoneId()); // summoner zone
  data << uint32(MAX_PLAYER_SUMMON_DELAY *
                 IN_MILLISECONDS); // auto decline after msecs
  SendDirectMessage(&data);
}

void Player::SummonIfPossible(bool agree) {
  if (!agree) {
    m_summon_expire = 0;
    return;
  }

  // expire and auto declined
  if (m_summon_expire < GameTime::GetGameTime())
    return;

  // stop taxi flight at summon
  if (IsInFlight()) {
    GetMotionMaster()->MovementExpired();
    CleanupAfterTaxiFlight();
  }

  // drop flag at summon
  // this code can be reached only when GM is summoning player who carries flag,
  // because player should be immune to summoning spells when he carries flag
  if (Battleground *bg = GetBattleground())
    bg->EventPlayerDroppedFlag(this);

  m_summon_expire = 0;

  UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_ACCEPTED_SUMMONINGS, 1);
  RemoveAurasWithInterruptFlags(SpellAuraInterruptFlags::Summon);

  TeleportTo(m_summon_location);
}

void Player::RemoveItemDurations(Item *item) {
  for (ItemDurationList::iterator itr = m_itemDuration.begin();
       itr != m_itemDuration.end(); ++itr) {
    if (*itr == item) {
      m_itemDuration.erase(itr);
      break;
    }
  }
}

void Player::AddItemDurations(Item *item) {
  if (item->GetUInt32Value(ITEM_FIELD_DURATION)) {
    m_itemDuration.push_back(item);
    item->SendTimeUpdate(this);
  }
}

void Player::AutoUnequipOffhandIfNeed(bool force /*= false*/) {
  Item *offItem = GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
  if (!offItem)
    return;

  // unequip offhand weapon if player doesn't have dual wield anymore
  if (!CanDualWield() &&
      (offItem->GetTemplate()->GetInventoryType() == INVTYPE_WEAPONOFFHAND ||
       offItem->GetTemplate()->GetInventoryType() == INVTYPE_WEAPON))
    force = true;

  // need unequip offhand for 2h-weapon without TitanGrip (in any from hands)
  if (!force &&
      (CanTitanGrip() ||
       (offItem->GetTemplate()->GetInventoryType() != INVTYPE_2HWEAPON &&
        !IsTwoHandUsed())))
    return;

  ItemPosCountVec off_dest;
  if (CanStoreItem(NULL_BAG, NULL_SLOT, off_dest, offItem, false) ==
      EQUIP_ERR_OK) {
    RemoveItem(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND, true);
    StoreItem(off_dest, offItem, true);
  } else {
    MoveItemFromInventory(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND, true);
    CharacterDatabaseTransaction trans = CharacterDatabase.BeginTransaction();
    offItem->DeleteFromInventoryDB(
        trans); // deletes item from character's inventory
    offItem->SaveToDB(
        trans); // recursive and not have transaction guard into self, item not
                // in inventory and can be save standalone

    std::string subject =
        GetSession()->GetFirelandsString(LANG_NOT_EQUIPPED_ITEM);
    MailDraft(subject,
              "There were problems with equipping one or several items")
        .AddItem(offItem)
        .SendMailTo(trans, this, MailSender(this, MAIL_STATIONERY_GM),
                    MAIL_CHECK_MASK_COPIED);

    CharacterDatabase.CommitTransaction(trans);
  }
}

OutdoorPvP *Player::GetOutdoorPvP() const {
  return sOutdoorPvPMgr->GetOutdoorPvPToZoneId(GetZoneId());
}

bool Player::HasItemFitToSpellRequirements(SpellInfo const *spellInfo,
                                           Item const *ignoreItem) const {
  if (spellInfo->EquippedItemClass < 0)
    return true;

  // scan other equipped items for same requirements (mostly 2 daggers/etc)
  // for optimize check 2 used cases only
  switch (spellInfo->EquippedItemClass) {
  case ITEM_CLASS_WEAPON: {
    for (uint8 i = EQUIPMENT_SLOT_MAINHAND; i < EQUIPMENT_SLOT_TABARD; ++i)
      if (Item *item = GetUseableItemByPos(INVENTORY_SLOT_BAG_0, i))
        if (item != ignoreItem && item->IsFitToSpellRequirements(spellInfo))
          return true;
    break;
  }
  case ITEM_CLASS_ARMOR: {
    // most used check: shield only
    if (spellInfo->EquippedItemSubClassMask &
        ((1 << ITEM_SUBCLASS_ARMOR_BUCKLER) |
         (1 << ITEM_SUBCLASS_ARMOR_SHIELD))) {
      if (Item *item =
              GetUseableItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND))
        if (item != ignoreItem && item->IsFitToSpellRequirements(spellInfo))
          return true;

      // special check to filter things like Shield Wall, the aura is not
      // permanent and must stay even without required item
      if (!spellInfo->IsPassive()) {
        for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
          if (spellInfo->Effects[i].IsAura())
            return true;
      }
    }

    // tabard not have dependent spells
    for (uint8 i = EQUIPMENT_SLOT_START; i < EQUIPMENT_SLOT_MAINHAND; ++i)
      if (Item *item = GetUseableItemByPos(INVENTORY_SLOT_BAG_0, i))
        if (item != ignoreItem && item->IsFitToSpellRequirements(spellInfo))
          return true;

    // ranged slot can have some armor subclasses
    if (Item *item =
            GetUseableItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_RANGED))
      if (item != ignoreItem && item->IsFitToSpellRequirements(spellInfo))
        return true;
    break;
  }
  default:
    LOG_ERROR("entities.player",
              "Player::HasItemFitToSpellRequirements: Not handled spell "
              "requirement for item class %u",
              spellInfo->EquippedItemClass);
    break;
  }

  return false;
}

bool Player::HasAllItemsToFitToSpellRequirements(SpellInfo const *spellInfo) {
  uint8 count = 0;

  for (uint8 slot = EQUIPMENT_SLOT_START; slot <= EQUIPMENT_SLOT_HANDS; ++slot)
    if (Item *item = GetUseableItemByPos(INVENTORY_SLOT_BAG_0, slot))
      if (item->IsFitToSpellRequirements(spellInfo))
        ++count;

  if (count >= 8)
    return true;

  return false;
}

bool Player::CanNoReagentCast(SpellInfo const *spellInfo) const {
  // don't take reagents for spells with SPELL_ATTR5_NO_REAGENT_WHILE_PREP
  if (spellInfo->HasAttribute(SPELL_ATTR5_NO_REAGENT_WHILE_PREP) &&
      HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PREPARATION))
    return true;

  // Check no reagent use mask
  flag96 noReagentMask;
  noReagentMask[0] = GetUInt32Value(PLAYER_NO_REAGENT_COST_1);
  noReagentMask[1] = GetUInt32Value(PLAYER_NO_REAGENT_COST_1 + 1);
  noReagentMask[2] = GetUInt32Value(PLAYER_NO_REAGENT_COST_1 + 2);
  if (spellInfo->SpellFamilyFlags & noReagentMask)
    return true;

  return false;
}

void Player::RemoveItemDependentAurasAndCasts(Item *pItem) {
  for (AuraMap::iterator itr = m_ownedAuras.begin();
       itr != m_ownedAuras.end();) {
    Aura *aura = itr->second;

    // skip not self applied auras
    SpellInfo const *spellInfo = aura->GetSpellInfo();
    if (aura->GetCasterGUID() != GetGUID()) {
      ++itr;
      continue;
    }

    // skip if not item dependent or have alternative item
    if (HasItemFitToSpellRequirements(spellInfo, pItem)) {
      ++itr;
      continue;
    }

    // no alt item, remove aura, restart check
    RemoveOwnedAura(itr);
  }

  // currently cast spells can be dependent from item
  for (uint32 i = 0; i < CURRENT_MAX_SPELL; ++i)
    if (Spell *spell = GetCurrentSpell(CurrentSpellTypes(i)))
      if (spell->getState() != SPELL_STATE_DELAYED &&
          !HasItemFitToSpellRequirements(spell->m_spellInfo, pItem))
        InterruptSpell(CurrentSpellTypes(i));
}

uint32 Player::GetResurrectionSpellId() {
  // search priceless resurrection possibilities
  uint32 prio = 0;
  uint32 spell_id = 0;
  AuraEffectList const &dummyAuras = GetAuraEffectsByType(SPELL_AURA_DUMMY);
  for (AuraEffectList::const_iterator itr = dummyAuras.begin();
       itr != dummyAuras.end(); ++itr) {
    // Soulstone Resurrection                           // prio: 3 (max, non
    // death persistent)
    if (prio < 2 && (*itr)->GetSpellInfo()->SpellVisual[0] == 99 &&
        (*itr)->GetSpellInfo()->SpellIconID == 92) {
      switch ((*itr)->GetId()) {
      case 20707:
        spell_id = 3026;
        break; // rank 1
      case 20762:
        spell_id = 20758;
        break; // rank 2
      case 20763:
        spell_id = 20759;
        break; // rank 3
      case 20764:
        spell_id = 20760;
        break; // rank 4
      case 20765:
        spell_id = 20761;
        break; // rank 5
      case 27239:
        spell_id = 27240;
        break; // rank 6
      case 47883:
        spell_id = 47882;
        break; // rank 7
      default:
        LOG_ERROR("entities.player", "Unhandled spell %u: S.Resurrection",
                  (*itr)->GetId());
        continue;
      }

      prio = 3;
    }
    // Twisting Nether                                  // prio: 2 (max)
    else if ((*itr)->GetId() == 23701 && roll_chance_i(10)) {
      prio = 2;
      spell_id = 23700;
    }
  }

  // Reincarnation (passive spell)  // prio: 1                  // Glyph of
  // Renewed Life
  if (prio < 1 && HasSpell(20608) && !GetSpellHistory()->HasCooldown(21169) &&
      (HasAura(58059) || HasItemCount(17030)))
    spell_id = 21169;

  return spell_id;
}

// Used in triggers for check "Only to targets that grant experience or honor"
// req
bool Player::isHonorOrXPTarget(Unit const *victim) const {
  uint8 v_level = victim->getLevel();
  uint8 k_grey = Firelands::XP::GetGrayLevel(getLevel());

  // Victim level less gray level
  if (v_level <= k_grey)
    return false;

  if (Creature const *creature = victim->ToCreature()) {
    if (creature->IsCritter() || creature->IsTotem())
      return false;
  }
  return true;
}

bool Player::GetsRecruitAFriendBonus(bool forXP) {
  bool recruitAFriend = false;
  if (getLevel() <= sWorld->getIntConfig(
                        CONFIG_MAX_RECRUIT_A_FRIEND_BONUS_PLAYER_LEVEL) ||
      !forXP) {
    if (Group *group = GetGroup()) {
      for (GroupReference *itr = group->GetFirstMember(); itr != nullptr;
           itr = itr->next()) {
        Player *player = itr->GetSource();
        if (!player)
          continue;

        if (!player->IsAtRecruitAFriendDistance(this))
          continue; // member (alive or dead) or his corpse at req. distance

        if (forXP) {
          // level must be allowed to get RaF bonus
          if (player->getLevel() >
              sWorld->getIntConfig(
                  CONFIG_MAX_RECRUIT_A_FRIEND_BONUS_PLAYER_LEVEL))
            continue;

          // level difference must be small enough to get RaF bonus, UNLESS we
          // are lower level
          if (player->getLevel() < getLevel())
            if (uint8(getLevel() - player->getLevel()) >
                sWorld->getIntConfig(
                    CONFIG_MAX_RECRUIT_A_FRIEND_BONUS_PLAYER_LEVEL_DIFFERENCE))
              continue;
        }

        bool ARecruitedB = (player->GetSession()->GetRecruiterId() ==
                            GetSession()->GetAccountId());
        bool BRecruitedA = (GetSession()->GetRecruiterId() ==
                            player->GetSession()->GetAccountId());
        if (ARecruitedB || BRecruitedA) {
          recruitAFriend = true;
          break;
        }
      }
    }
  }
  return recruitAFriend;
}

void Player::RewardPlayerAndGroupAtKill(Unit *victim, bool isBattleGround) {
  KillRewarder(this, victim, isBattleGround).Reward();
}

void Player::RewardPlayerAndGroupAtEvent(uint32 creature_id,
                                         WorldObject *pRewardSource) {
  if (!pRewardSource)
    return;

  ObjectGuid creature_guid = (pRewardSource->GetTypeId() == TYPEID_UNIT)
                                 ? pRewardSource->GetGUID()
                                 : ObjectGuid::Empty;

  // prepare data for near group iteration
  if (Group *group = GetGroup()) {
    for (GroupReference *itr = group->GetFirstMember(); itr != nullptr;
         itr = itr->next()) {
      Player *player = itr->GetSource();
      if (!player)
        continue;

      if (!player->IsAtGroupRewardDistance(pRewardSource))
        continue; // member (alive or dead) or his corpse at req. distance

      // quest objectives updated only for alive group member or dead but with
      // not released body
      if (player->IsAlive() || !player->GetCorpse())
        player->KilledMonsterCredit(creature_id, creature_guid);
    }
  } else // if (!group)
    KilledMonsterCredit(creature_id, creature_guid);
}

bool Player::IsAtGroupRewardDistance(WorldObject const *pRewardSource) const {
  if (!pRewardSource || !IsInMap(pRewardSource))
    return false;

  WorldObject const *player = GetCorpse();
  if (!player || IsAlive())
    player = this;

  if (pRewardSource->GetMap()->IsDungeon())
    return true;

  return pRewardSource->GetDistance(player) <=
         sWorld->getFloatConfig(CONFIG_GROUP_XP_DISTANCE);
}

bool Player::IsAtRecruitAFriendDistance(WorldObject const *pOther) const {
  if (!pOther || !IsInMap(pOther))
    return false;

  WorldObject const *player = GetCorpse();
  if (!player || IsAlive())
    player = this;

  return pOther->GetDistance(player) <=
         sWorld->getFloatConfig(CONFIG_MAX_RECRUIT_A_FRIEND_DISTANCE);
}

void Player::ResurrectUsingRequestData() {
  // Teleport before resurrecting by player, otherwise the player might get
  // attacked from creatures near his corpse
  float x, y, z, o;
  _resurrectionData->Location.GetPosition(x, y, z, o);
  TeleportTo(_resurrectionData->Location.GetMapId(), x, y, z, o);

  if (IsBeingTeleported()) {
    ScheduleDelayedOperation(DELAYED_RESURRECT_PLAYER);
    return;
  }

  uint32 resurrectHealth = _resurrectionData->Health;
  uint32 resurrectMana = _resurrectionData->Mana;

  ResurrectPlayer(0.0f, false);

  SetHealth(resurrectHealth);
  SetPower(POWER_MANA, resurrectMana);

  SetPower(POWER_RAGE, 0);
  SetFullPower(POWER_ENERGY);
  SetFullPower(POWER_FOCUS);
  SetPower(POWER_ECLIPSE, 0);

  if (uint32 aura = _resurrectionData->Aura) {
    CastSpellExtraArgs args(TRIGGERED_FULL_MASK);
    args.SetOriginalCaster(_resurrectionData->GUID);
    CastSpell(this, aura, args);
  }

  SpawnCorpseBones();
}

void Player::SetClientControl(Unit *target, bool allowMove) {
  // a player can never client control nothing
  ASSERT(target);

  // don't allow possession to be overridden
  if (target->HasUnitState(UNIT_STATE_CHARMED) &&
      (GetGUID() != target->GetCharmerGUID())) {
    LOG_ERROR("entities.player",
              "Player '%s' attempt to client control '%s', which is charmed by "
              "GUID %s",
              GetName().c_str(), target->GetName().c_str(),
              target->GetCharmerGUID().ToString().c_str());
    return;
  }

  // still affected by some aura that shouldn't allow control, only allow on
  // last such aura to be removed
  if (target->HasUnitState(UNIT_STATE_FLEEING | UNIT_STATE_CONFUSED))
    allowMove = false;

  WorldPacket data(SMSG_CLIENT_CONTROL_UPDATE,
                   target->GetPackGUID().size() + 1);
  data << target->GetPackGUID();
  data << uint8(allowMove ? 1 : 0);
  SendDirectMessage(&data);

  WorldObject *viewpoint = GetViewpoint();
  if (!viewpoint)
    viewpoint = this;
  if (target != viewpoint) {
    if (viewpoint != this)
      SetViewpoint(viewpoint, false);
    if (target != this)
      SetViewpoint(target, true);
  }

  GetGameClient()->SetMovedUnit(target, allowMove);

  if (allowMove)
    SendDirectMessage(
        WorldPackets::Movement::MoveSetActiveMover(target->GetGUID()).Write());
}

void Player::UpdateZoneDependentAuras(uint32 newZone) {
  // Some spells applied at enter into zone (with subzones), aura removed in
  // UpdateAreaDependentAuras that called always at zone->area update
  SpellAreaForAreaMapBounds saBounds =
      sSpellMgr->GetSpellAreaForAreaMapBounds(newZone);
  for (SpellAreaForAreaMap::const_iterator itr = saBounds.first;
       itr != saBounds.second; ++itr)
    if (itr->second->flags & SPELL_AREA_FLAG_AUTOCAST &&
        itr->second->IsFitToRequirements(this, newZone, 0))
      if (!HasAura(itr->second->spellId))
        CastSpell(this, itr->second->spellId, true);
}

void Player::UpdateAreaDependentAuras(uint32 newArea) {
  // remove auras from spells with area limitations
  for (AuraMap::iterator iter = m_ownedAuras.begin();
       iter != m_ownedAuras.end();) {
    // use m_zoneUpdateId for speed: UpdateArea called from UpdateZone or
    // instead UpdateZone in both cases m_zoneUpdateId up-to-date
    if (iter->second->GetSpellInfo()->CheckLocation(
            GetMapId(), m_zoneUpdateId, newArea, this) != SPELL_CAST_OK)
      RemoveOwnedAura(iter);
    else
      ++iter;
  }

  // UpdateZoneDependentAuras takes care of zone specific auras so skip
  // excessive casting if the area Id is the same as its parent.
  if (m_zoneUpdateId == newArea)
    return;

  // some auras applied at subzone enter
  SpellAreaForAreaMapBounds saBounds =
      sSpellMgr->GetSpellAreaForAreaMapBounds(newArea);
  for (SpellAreaForAreaMap::const_iterator itr = saBounds.first;
       itr != saBounds.second; ++itr)
    if (itr->second->flags & SPELL_AREA_FLAG_AUTOCAST &&
        itr->second->IsFitToRequirements(this, m_zoneUpdateId, newArea))
      if (!HasAura(itr->second->spellId))
        CastSpell(this, itr->second->spellId, true);
}

uint32 Player::GetCorpseReclaimDelay(bool pvp) const {
  if (pvp) {
    if (!sWorld->getBoolConfig(CONFIG_DEATH_CORPSE_RECLAIM_DELAY_PVP))
      return copseReclaimDelay[0];
  } else if (!sWorld->getBoolConfig(CONFIG_DEATH_CORPSE_RECLAIM_DELAY_PVE))
    return 0;

  time_t now = GameTime::GetGameTime();
  // 0..2 full period
  // should be ceil(x)-1 but not floor(x)
  uint64 count = (now < m_deathExpireTime - 1)
                     ? (m_deathExpireTime - 1 - now) / DEATH_EXPIRE_STEP
                     : 0;
  return copseReclaimDelay[count];
}

void Player::UpdateCorpseReclaimDelay() {
  bool pvp = (m_ExtraFlags & PLAYER_EXTRA_PVP_DEATH) != 0;

  if ((pvp && !sWorld->getBoolConfig(CONFIG_DEATH_CORPSE_RECLAIM_DELAY_PVP)) ||
      (!pvp && !sWorld->getBoolConfig(CONFIG_DEATH_CORPSE_RECLAIM_DELAY_PVE)))
    return;

  time_t now = GameTime::GetGameTime();

  if (now < m_deathExpireTime) {
    // full and partly periods 1..3
    uint64 count = (m_deathExpireTime - now) / DEATH_EXPIRE_STEP + 1;

    if (count < MAX_DEATH_COUNT)
      m_deathExpireTime = now + (count + 1) * DEATH_EXPIRE_STEP;
    else
      m_deathExpireTime = now + MAX_DEATH_COUNT * DEATH_EXPIRE_STEP;
  } else
    m_deathExpireTime = now + DEATH_EXPIRE_STEP;
}

int32 Player::CalculateCorpseReclaimDelay(bool load) const {
  Corpse *corpse = GetCorpse();

  if (load && !corpse)
    return -1;

  bool pvp = corpse ? corpse->GetType() == CORPSE_RESURRECTABLE_PVP
                    : (m_ExtraFlags & PLAYER_EXTRA_PVP_DEATH) != 0;

  uint32 delay;

  if (load) {
    if (corpse->GetGhostTime() > m_deathExpireTime)
      return -1;

    uint64 count = 0;

    if ((pvp && sWorld->getBoolConfig(CONFIG_DEATH_CORPSE_RECLAIM_DELAY_PVP)) ||
        (!pvp &&
         sWorld->getBoolConfig(CONFIG_DEATH_CORPSE_RECLAIM_DELAY_PVE))) {
      count = (m_deathExpireTime - corpse->GetGhostTime()) / DEATH_EXPIRE_STEP;

      if (count >= MAX_DEATH_COUNT)
        count = MAX_DEATH_COUNT - 1;
    }

    time_t expected_time = corpse->GetGhostTime() + copseReclaimDelay[count];
    time_t now = GameTime::GetGameTime();

    if (now >= expected_time)
      return -1;

    delay = expected_time - now;
  } else
    delay = GetCorpseReclaimDelay(pvp);

  return delay * IN_MILLISECONDS;
}

void Player::SendCorpseReclaimDelay(uint32 delay) const {
  WorldPackets::Misc::CorpseReclaimDelay packet;
  packet.Remaining = delay;
  SendDirectMessage(packet.Write());
}

Player *Player::GetNextRandomRaidMember(float radius) {
  Group *group = GetGroup();
  if (!group)
    return nullptr;

  std::vector<Player *> nearMembers;
  nearMembers.reserve(group->GetMembersCount());

  for (GroupReference *itr = group->GetFirstMember(); itr != nullptr;
       itr = itr->next()) {
    Player *Target = itr->GetSource();

    // IsHostileTo check duel and controlled by enemy
    if (Target && Target != this && IsWithinDistInMap(Target, radius) &&
        !Target->HasInvisibilityAura() && !IsHostileTo(Target))
      nearMembers.push_back(Target);
  }

  if (nearMembers.empty())
    return nullptr;

  uint32 randTarget = urand(0, nearMembers.size() - 1);
  return nearMembers[randTarget];
}

PartyResult Player::CanUninviteFromGroup(ObjectGuid guidMember) const {
  Group const *grp = GetGroup();
  if (!grp)
    return ERR_NOT_IN_GROUP;

  if (grp->isLFGGroup()) {
    ObjectGuid gguid = grp->GetGUID();
    if (!sLFGMgr->GetKicksLeft(gguid))
      return ERR_PARTY_LFG_BOOT_LIMIT;

    lfg::LfgState state = sLFGMgr->GetState(gguid);
    if (sLFGMgr->IsVoteKickActive(gguid))
      return ERR_PARTY_LFG_BOOT_IN_PROGRESS;

    if (grp->GetMembersCount() <= lfg::LFG_GROUP_KICK_VOTES_NEEDED)
      return ERR_PARTY_LFG_BOOT_TOO_FEW_PLAYERS;

    if (state == lfg::LFG_STATE_FINISHED_DUNGEON)
      return ERR_PARTY_LFG_BOOT_DUNGEON_COMPLETE;

    if (grp->isRollLootActive())
      return ERR_PARTY_LFG_BOOT_LOOT_ROLLS;

    /// @todo Should also be sent when anyone has recently left combat, with an
    /// aprox ~5 seconds timer.
    for (GroupReference const *itr = grp->GetFirstMember(); itr != nullptr;
         itr = itr->next())
      if (itr->GetSource() && itr->GetSource()->IsInMap(this) &&
          itr->GetSource()->IsInCombat())
        return ERR_PARTY_LFG_BOOT_IN_COMBAT;

    /* Missing support for these types
        return ERR_PARTY_LFG_BOOT_COOLDOWN_S;
        return ERR_PARTY_LFG_BOOT_NOT_ELIGIBLE_S;
    */
  } else {
    if (!grp->IsLeader(GetGUID()) && !grp->IsAssistant(GetGUID()))
      return ERR_NOT_LEADER;

    if (InBattleground())
      return ERR_INVITE_RESTRICTED;

    if (grp->IsLeader(guidMember))
      return ERR_NOT_LEADER;
  }

  return ERR_PARTY_RESULT_OK;
}

bool Player::isUsingLfg() const {
  return sLFGMgr->GetState(GetGUID()) != lfg::LFG_STATE_NONE;
}

bool Player::inRandomLfgDungeon() const {
  if (sLFGMgr->selectedRandomLfgDungeon(GetGUID())) {
    Map const *map = GetMap();
    return sLFGMgr->inLfgDungeonMap(GetGUID(), map->GetId(),
                                    map->GetDifficulty());
  }

  return false;
}

void Player::SetBattlegroundOrBattlefieldRaid(Group *group, int8 subgroup) {
  // we must move references from m_group to m_originalGroup
  SetOriginalGroup(GetGroup(), GetSubGroup());

  m_group.unlink();
  m_group.link(group, this);
  m_group.setSubGroup((uint8)subgroup);
}

void Player::RemoveFromBattlegroundOrBattlefieldRaid() {
  // remove existing reference
  m_group.unlink();
  if (Group *group = GetOriginalGroup()) {
    m_group.link(group, this);
    m_group.setSubGroup(GetOriginalSubGroup());
  }
  SetOriginalGroup(nullptr);
}

void Player::SetOriginalGroup(Group *group, int8 subgroup) {
  if (group == nullptr)
    m_originalGroup.unlink();
  else {
    // never use SetOriginalGroup without a subgroup unless you specify nullptr
    // for group
    ASSERT(subgroup >= 0);
    m_originalGroup.link(group, this);
    m_originalGroup.setSubGroup((uint8)subgroup);
  }
}

void Player::ProcessTerrainStatusUpdate(
    ZLiquidStatus oldLiquidStatus, Optional<LiquidData> const &newLiquidData) {
  // process liquid auras using generic unit code
  Unit::ProcessTerrainStatusUpdate(oldLiquidStatus, newLiquidData);

  // player specific logic for mirror timers
  if (GetLiquidStatus() && newLiquidData) {
    // Breath bar state (under water in any liquid type)
    if (newLiquidData->type_flags & MAP_ALL_LIQUIDS) {
      if (GetLiquidStatus() & LIQUID_MAP_UNDER_WATER)
        m_MirrorTimerFlags |= UNDERWATER_INWATER;
      else
        m_MirrorTimerFlags &= ~UNDERWATER_INWATER;
    }

    // Fatigue bar state (if not on flight path or transport)
    if ((newLiquidData->type_flags & MAP_LIQUID_TYPE_DARK_WATER) &&
        !IsInFlight() && !GetTransport())
      m_MirrorTimerFlags |= UNDERWATER_INDARKWATER;
    else
      m_MirrorTimerFlags &= ~UNDERWATER_INDARKWATER;

    // Lava state (any contact)
    if (newLiquidData->type_flags & MAP_LIQUID_TYPE_MAGMA) {
      if (GetLiquidStatus() & MAP_LIQUID_STATUS_IN_CONTACT)
        m_MirrorTimerFlags |= UNDERWATER_INLAVA;
      else
        m_MirrorTimerFlags &= ~UNDERWATER_INLAVA;
    }

    // Slime state (any contact)
    if (newLiquidData->type_flags & MAP_LIQUID_TYPE_SLIME) {
      if (GetLiquidStatus() & MAP_LIQUID_STATUS_IN_CONTACT)
        m_MirrorTimerFlags |= UNDERWATER_INSLIME;
      else
        m_MirrorTimerFlags &= ~UNDERWATER_INSLIME;
    }
  } else
    m_MirrorTimerFlags &= ~(UNDERWATER_INWATER | UNDERWATER_INLAVA |
                            UNDERWATER_INSLIME | UNDERWATER_INDARKWATER);
}

void Player::AtExitCombat() {
  Unit::AtExitCombat();
  UpdatePotionCooldown();

  if (getClass() == CLASS_DEATH_KNIGHT)
    for (uint8 i = 0; i < MAX_RUNES; ++i) {
      SetRuneTimer(i, 0xFFFFFFFF);
      SetLastRuneGraceTimer(i, 0);
    }
}

void Player::SetCanParry(bool value) {
  if (m_canParry == value)
    return;

  m_canParry = value;
  UpdateParryPercentage();
}

void Player::SetCanBlock(bool value) {
  if (m_canBlock == value)
    return;

  m_canBlock = value;
  UpdateBlockPercentage();
}

bool ItemPosCount::isContainedIn(std::vector<ItemPosCount> const &vec) const {
  for (ItemPosCountVec::const_iterator itr = vec.begin(); itr != vec.end();
       ++itr)
    if (itr->pos == pos)
      return true;
  return false;
}

void Player::StopCastingBindSight() const {
  if (WorldObject *target = GetViewpoint()) {
    if (target->isType(TYPEMASK_UNIT)) {
      static_cast<Unit *>(target)->RemoveAurasByType(SPELL_AURA_BIND_SIGHT,
                                                     GetGUID());
      static_cast<Unit *>(target)->RemoveAurasByType(SPELL_AURA_MOD_POSSESS,
                                                     GetGUID());
    }
  }
}

void Player::SetViewpoint(WorldObject *target, bool apply) {
  if (apply) {
    LOG_DEBUG("maps",
              "Player::CreateViewpoint: Player '%s' (%s) creates seer (Entry: "
              "%u, TypeId: %u).",
              GetName().c_str(), GetGUID().ToString().c_str(),
              target->GetEntry(), target->GetTypeId());

    if (!AddGuidValue(PLAYER_FARSIGHT, target->GetGUID())) {
      LOG_FATAL(
          "entities.player",
          "Player::CreateViewpoint: Player '%s' (%s) cannot add new viewpoint!",
          GetName().c_str(), GetGUID().ToString().c_str());
      return;
    }

    // farsight dynobj or puppet may be very far away
    UpdateVisibilityOf(target);

    if (target->isType(TYPEMASK_UNIT) && target != GetVehicleBase())
      static_cast<Unit *>(target)->AddPlayerToVision(this);
    SetSeer(target);
  } else {
    LOG_DEBUG("maps", "Player::CreateViewpoint: Player %s removed seer",
              GetName().c_str());

    if (!RemoveGuidValue(PLAYER_FARSIGHT, target->GetGUID())) {
      LOG_FATAL("entities.player",
                "Player::CreateViewpoint: Player '%s' (%s) cannot remove "
                "current viewpoint!",
                GetName().c_str(), GetGUID().ToString().c_str());
      return;
    }

    if (target->isType(TYPEMASK_UNIT) && target != GetVehicleBase())
      static_cast<Unit *>(target)->RemovePlayerFromVision(this);

    // must immediately set seer back otherwise may crash
    SetSeer(this);

    // WorldPacket data(SMSG_CLEAR_FAR_SIGHT_IMMEDIATE, 0);
    // SendDirectMessage(&data);
  }
}

WorldObject *Player::GetViewpoint() const {
  if (ObjectGuid guid = GetGuidValue(PLAYER_FARSIGHT))
    return static_cast<WorldObject *>(
        ObjectAccessor::GetObjectByTypeMask(*this, guid, TYPEMASK_SEER));
  return nullptr;
}

bool Player::CanUseBattlegroundObject(GameObject *gameobject) const {
  // It is possible to call this method with a null pointer, only skipping
  // faction check.
  if (gameobject) {
    FactionTemplateEntry const *playerFaction = GetFactionTemplateEntry();
    FactionTemplateEntry const *faction =
        sFactionTemplateStore.LookupEntry(gameobject->GetFaction());

    if (playerFaction && faction && !playerFaction->IsFriendlyTo(*faction))
      return false;
  }

  // BUG: sometimes when player clicks on flag in AB - client won't send
  // gameobject_use, only gameobject_report_use packet Note: Mount, stealth and
  // invisibility will be removed when used
  return (
      !isTotalImmune() && // Damage immune
      !HasAura(
          SPELL_RECENTLY_DROPPED_FLAG) && // Still has recently held flag debuff
      IsAlive());                         // Alive
}

bool Player::CanCaptureTowerPoint() const {
  return (!HasStealthAura() &&      // not stealthed
          !HasInvisibilityAura() && // not invisible
          IsAlive());               // live player
}

uint32 Player::GetBarberShopCost(uint8 newhairstyle, uint8 newhaircolor,
                                 uint8 newfacialhair,
                                 BarberShopStyleEntry const *newSkin) const {
  uint8 level = getLevel();

  if (level > GT_MAX_LEVEL)
    level = GT_MAX_LEVEL; // max level in this dbc

  uint8 hairstyle =
      GetByteValue(PLAYER_BYTES, PLAYER_BYTES_OFFSET_HAIR_STYLE_ID);
  uint8 haircolor =
      GetByteValue(PLAYER_BYTES, PLAYER_BYTES_OFFSET_HAIR_COLOR_ID);
  uint8 facialhair =
      GetByteValue(PLAYER_BYTES_2, PLAYER_BYTES_2_OFFSET_FACIAL_STYLE);
  uint8 skincolor = GetByteValue(PLAYER_BYTES, PLAYER_BYTES_OFFSET_SKIN_ID);

  if ((hairstyle == newhairstyle) && (haircolor == newhaircolor) &&
      (facialhair == newfacialhair) &&
      (!newSkin || (newSkin->Data == skincolor)))
    return 0;

  GtBarberShopCostBaseEntry const *bsc =
      sGtBarberShopCostBaseStore.LookupEntry(level - 1);

  if (!bsc) // shouldn't happen
    return 0xFFFFFFFF;

  float cost = 0;

  if (hairstyle != newhairstyle)
    cost += bsc->cost; // full price

  if ((haircolor != newhaircolor) && (hairstyle == newhairstyle))
    cost += bsc->cost * 0.5f; // +1/2 of price

  if (facialhair != newfacialhair)
    cost += bsc->cost * 0.75f; // +3/4 of price

  if (newSkin && skincolor != newSkin->Data)
    cost += bsc->cost * 0.75f; // +5/6 of price

  return uint32(cost);
}

void Player::InitGlyphsForLevel() {
  uint32 slot = 0;
  for (uint32 i = 0;
       i < sGlyphSlotStore.GetNumRows() && slot < MAX_GLYPH_SLOT_INDEX; ++i)
    if (GlyphSlotEntry const *gs = sGlyphSlotStore.LookupEntry(i))
      SetGlyphSlot(slot++, gs->ID);

  uint8 level = getLevel();
  uint32 slotMask = 0;

  if (level >= 25)
    slotMask |= 0x01 | 0x02 | 0x40;
  if (level >= 50)
    slotMask |= 0x04 | 0x08 | 0x80;
  if (level >= 75)
    slotMask |= 0x10 | 0x20 | 0x100;

  SetUInt32Value(PLAYER_GLYPHS_ENABLED, slotMask);
}

void Player::SetGlyph(uint8 slot, uint32 glyph) {
  _talentMgr->SpecInfo[GetActiveSpec()].Glyphs[slot] = glyph;
  SetUInt32Value(PLAYER_FIELD_GLYPHS_1 + slot, glyph);
}

bool Player::isTotalImmune() const {
  AuraEffectList const &immune =
      GetAuraEffectsByType(SPELL_AURA_SCHOOL_IMMUNITY);

  uint32 immuneMask = 0;
  for (AuraEffectList::const_iterator itr = immune.begin(); itr != immune.end();
       ++itr) {
    immuneMask |= (*itr)->GetMiscValue();
    if (immuneMask & SPELL_SCHOOL_MASK_ALL) // total immunity
      return true;
  }
  return false;
}

bool Player::HasTitle(uint32 bitIndex) const {
  if (bitIndex > MAX_TITLE_INDEX)
    return false;

  uint32 fieldIndexOffset = bitIndex / 32;
  uint32 flag = 1 << (bitIndex % 32);
  return HasFlag(PLAYER__FIELD_KNOWN_TITLES + fieldIndexOffset, flag);
}

bool Player::HasTitle(CharTitlesEntry const *title) const {
  return HasTitle(title->Mask_ID);
}

void Player::SetTitle(CharTitlesEntry const *title, bool lost) {
  uint32 fieldIndexOffset = title->Mask_ID / 32;
  uint32 flag = 1 << (title->Mask_ID % 32);

  if (lost) {
    if (!HasFlag(PLAYER__FIELD_KNOWN_TITLES + fieldIndexOffset, flag))
      return;

    RemoveFlag(PLAYER__FIELD_KNOWN_TITLES + fieldIndexOffset, flag);
  } else {
    if (HasFlag(PLAYER__FIELD_KNOWN_TITLES + fieldIndexOffset, flag))
      return;

    SetFlag(PLAYER__FIELD_KNOWN_TITLES + fieldIndexOffset, flag);
  }

  WorldPacket data(SMSG_TITLE_EARNED, 4 + 4);
  data << uint32(title->Mask_ID);
  data << uint32(lost ? 0 : 1); // 1 - earned, 0 - lost
  SendDirectMessage(&data);
}

bool Player::isTotalImmunity() const {
  AuraEffectList const &immune =
      GetAuraEffectsByType(SPELL_AURA_SCHOOL_IMMUNITY);

  for (AuraEffectList::const_iterator itr = immune.begin(); itr != immune.end();
       ++itr) {
    if (((*itr)->GetMiscValue() & SPELL_SCHOOL_MASK_ALL) != 0) // total immunity
    {
      return true;
    }
    if (((*itr)->GetMiscValue() & SPELL_SCHOOL_MASK_NORMAL) !=
        0) // physical damage immunity
    {
      for (AuraEffectList::const_iterator i = immune.begin(); i != immune.end();
           ++i) {
        if (((*i)->GetMiscValue() & SPELL_SCHOOL_MASK_MAGIC) !=
            0) // magic immunity
        {
          return true;
        }
      }
    }
  }
  return false;
}

void Player::SetRuneConvertAura(uint8 index, AuraEffect const *aura,
                                AuraType auraType, SpellInfo const *spellInfo) {
  m_runes->runes[index].ConvertAura = aura;
  m_runes->runes[index].ConvertAuraType = auraType;
  m_runes->runes[index].ConvertAuraInfo = spellInfo;
}

void Player::AddRuneByAuraEffect(uint8 index, RuneType newType,
                                 AuraEffect const *aura, AuraType auraType,
                                 SpellInfo const *spellInfo) {
  SetRuneConvertAura(index, aura, auraType, spellInfo);
  ConvertRune(index, newType);
}

void Player::SetRuneCooldown(uint8 index, uint32 cooldown) {
  if (AuraEffect *aurEff =
          GetAuraEffect(SPELL_AURA_DUMMY, SPELLFAMILY_GENERIC, 2636, EFFECT_2))
    cooldown *= 1.0 - (aurEff->GetAmount() / 100);

  m_runes->runes[index].Cooldown = cooldown;
  m_runes->SetRuneState(index, (cooldown == 0) ? true : false);
}

void Player::RemoveRunesByAuraEffect(AuraEffect const *aura) {
  for (uint8 i = 0; i < MAX_RUNES; ++i) {
    if (m_runes->runes[i].ConvertAura == aura) {
      ConvertRune(i, GetBaseRune(i));
      SetRuneConvertAura(i, nullptr, SPELL_AURA_NONE, nullptr);
    }
  }
}

void Player::RestoreBaseRune(uint8 index) {
  SpellInfo const *spellInfo = m_runes->runes[index].ConvertAuraInfo;
  AuraType type = m_runes->runes[index].ConvertAuraType;
  // If rune was converted by a non-pasive aura that still active we should keep
  // it converted
  if (spellInfo) {
    if (!(spellInfo->Attributes & SPELL_ATTR0_PASSIVE))
      return;

    // Don't even convert aura for passive convertion
    if (spellInfo->IsPassive() && type == SPELL_AURA_CONVERT_RUNE)
      return;
  }

  ConvertRune(index, GetBaseRune(index));
  SetRuneConvertAura(index, nullptr, SPELL_AURA_NONE, nullptr);
}

void Player::ConvertRune(uint8 index, RuneType newType) {
  SetCurrentRune(index, newType);

  WorldPackets::Spells::ConvertRune packet;
  packet.Index = index;
  packet.Rune = newType;

  SendDirectMessage(packet.Write());
}

void Player::ResyncRunes() {
  WorldPackets::Spells::ResyncRunes packet;
  for (uint8 i = 0; i < MAX_RUNES; ++i) {
    WorldPackets::Spells::ResyncRune resyncRune;
    resyncRune.RuneType = GetCurrentRune(i);
    resyncRune.Cooldown =
        uint8(GetRuneCooldown(i) * uint32(255) / uint32(RUNE_BASE_COOLDOWN));
    packet.Runes.emplace_back(resyncRune);
  }

  SendDirectMessage(packet.Write());
}

void Player::SendConvertedRunes() {
  for (uint8 i = 0; i < MAX_RUNES; ++i) {
    if (GetBaseRune(i) != GetCurrentRune(i)) {
      WorldPackets::Spells::ConvertRune convertRune;
      convertRune.Index = i;
      convertRune.Rune = GetCurrentRune(i);
      SendDirectMessage(convertRune.Write());
    }
  }
}

void Player::AddRunePower(uint8 mask) {
  WorldPackets::Spells::AddRunePower packet;
  packet.AddedRunesMask = mask; // mask (0x00-0x3F probably)
  SendDirectMessage(packet.Write());
}

static RuneType runeSlotTypes[MAX_RUNES] = {
    /*0*/ RUNE_BLOOD,
    /*1*/ RUNE_BLOOD,
    /*2*/ RUNE_UNHOLY,
    /*3*/ RUNE_UNHOLY,
    /*4*/ RUNE_FROST,
    /*5*/ RUNE_FROST};

void Player::InitRunes() {
  if (getClass() != CLASS_DEATH_KNIGHT)
    return;

  m_runes = new Runes;

  m_runes->runeState = 0;
  m_runes->lastUsedRune = RUNE_BLOOD;
  m_runes->lastUsedRuneMask = 0;

  for (uint8 i = 0; i < MAX_RUNES; ++i) {
    SetBaseRune(i, runeSlotTypes[i]);    // init base types
    SetCurrentRune(i, runeSlotTypes[i]); // init current types
    SetRuneCooldown(i, 0);               // reset cooldowns
    SetRuneConvertAura(i, nullptr, SPELL_AURA_NONE, nullptr);
    m_runes->SetRuneState(i);
  }

  for (uint8 i = 0; i < NUM_RUNE_TYPES; ++i)
    SetFloatValue(PLAYER_RUNE_REGEN_1 + i,
                  0.1f); // set a base regen timer equal to 10 sec
}

bool Player::IsBaseRuneSlotsOnCooldown(RuneType runeType) const {
  for (uint8 i = 0; i < MAX_RUNES; ++i)
    if (GetBaseRune(i) == runeType && GetRuneCooldown(i) == 0)
      return false;

  return true;
}

void Player::AutoStoreLoot(uint8 bag, uint8 slot, uint32 loot_id,
                           LootStore const &store, bool broadcast) {
  Loot loot;
  loot.FillLoot(loot_id, store, this, true);

  uint32 max_slot = loot.GetMaxSlotInLootFor(this);
  for (uint32 i = 0; i < max_slot; ++i) {
    LootItem *lootItem = loot.LootItemInSlot(i, this);

    ItemPosCountVec dest;
    InventoryResult msg =
        CanStoreNewItem(bag, slot, dest, lootItem->itemid, lootItem->count);
    if (msg != EQUIP_ERR_OK && slot != NULL_SLOT)
      msg = CanStoreNewItem(bag, NULL_SLOT, dest, lootItem->itemid,
                            lootItem->count);
    if (msg != EQUIP_ERR_OK && bag != NULL_BAG)
      msg = CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, lootItem->itemid,
                            lootItem->count);
    if (msg != EQUIP_ERR_OK) {
      SendEquipError(msg, nullptr, nullptr, lootItem->itemid);
      continue;
    }

    Item *pItem =
        StoreNewItem(dest, lootItem->itemid, true, lootItem->randomPropertyId);
    SendNewItem(pItem, lootItem->count, false, false, broadcast);
  }
}

void Player::StoreLootItem(uint8 lootSlot, Loot *loot, GameObject *go) {
  NotNormalLootItem *qitem = nullptr;
  NotNormalLootItem *ffaitem = nullptr;
  NotNormalLootItem *conditem = nullptr;

  LootItem *item =
      loot->LootItemInSlot(lootSlot, this, &qitem, &ffaitem, &conditem);

  if (!item) {
    SendEquipError(EQUIP_ERR_LOOT_GONE, nullptr, nullptr);
    return;
  }

  if (!item->AllowedForPlayer(this)) {
    SendLootRelease(GetLootGUID());
    return;
  }

  // questitems use the blocked field for other purposes
  if (!qitem && item->is_blocked) {
    SendLootRelease(GetLootGUID());
    return;
  }

  // dont allow protected item to be looted by someone else
  if (!item->rollWinnerGUID.IsEmpty() && item->rollWinnerGUID != GetGUID()) {
    SendLootRelease(GetLootGUID());
    return;
  }

  ItemPosCountVec dest;
  InventoryResult msg =
      CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, item->itemid, item->count);
  if (msg == EQUIP_ERR_OK) {
    GuidSet looters = item->GetAllowedLooters();
    Item *newitem =
        StoreNewItem(dest, item->itemid, true, item->randomPropertyId, looters);

    if (qitem) {
      qitem->is_looted = true;
      // freeforall is 1 if everyone's supposed to get the quest item.
      if (item->freeforall || loot->GetPlayerQuestItems().size() == 1)
        SendNotifyLootItemRemoved(lootSlot);
      else
        loot->NotifyQuestItemRemoved(qitem->index);
    } else {
      if (ffaitem) {
        // freeforall case, notify only one player of the removal
        ffaitem->is_looted = true;
        SendNotifyLootItemRemoved(lootSlot);
      } else {
        // not freeforall, notify everyone
        if (conditem)
          conditem->is_looted = true;
        loot->NotifyItemRemoved(lootSlot);
      }
    }

    // if only one person is supposed to loot the item, then set it to looted
    if (!item->freeforall)
      item->is_looted = true;

    --loot->unlootedCount;

    if (ItemTemplate const *proto = sObjectMgr->GetItemTemplate(item->itemid))
      if ((proto->GetQuality() > ITEM_QUALITY_EPIC // always log legendary items
           || (proto->GetQuality() == ITEM_QUALITY_EPIC &&
               proto->GetBaseItemLevel() >=
                   MinNewsItemLevel[sWorld->getIntConfig(
                       CONFIG_EXPANSION)])) // log epic items when within item
                                            // level range
          && !proto->GetDuration()) // log items only when they are permanent
                                    // (skipping legendary items from Kael'thas
                                    // encounter for example)
        if (Guild *guild = GetGuild())
          guild->AddGuildNews(GUILD_NEWS_ITEM_LOOTED, GetGUID(), 0,
                              item->itemid);

    SendNewItem(newitem, uint32(item->count), false, false, true);
    UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_LOOT_ITEM, item->itemid,
                              item->count);
    UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_LOOT_TYPE, item->itemid,
                              item->count, loot->loot_type);
    UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_LOOT_EPIC_ITEM,
                              item->itemid, item->count);
    UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_CATCH_FROM_POOL,
                              item->itemid, item->count, 0, nullptr, go);

    // LootItem is being removed (looted) from the container, delete it from the
    // DB.
    if (loot->containerID > 0)
      sLootItemStorage->RemoveStoredLootItemForContainer(
          loot->containerID, item->itemid, item->count);

  } else
    SendEquipError(msg, nullptr, nullptr, item->itemid);
}

uint32 Player::CalculateTalentsPoints() const {
  // this dbc file has entries only up to level 100
  NumTalentsAtLevelEntry const *count =
      sNumTalentsAtLevelStore.LookupEntry(std::min<uint32>(getLevel(), 100));
  if (!count)
    return 0;

  float baseForLevel = count->NumberOfTalents;

  if (getClass() != CLASS_DEATH_KNIGHT || GetMapId() != 609)
    return uint32(baseForLevel * sWorld->getRate(RATE_TALENT));

  // Death Knight starting level
  // hardcoded here - number of quest awarded talents is equal to number of
  // talents any other class would have at level 55
  if (getLevel() < 55)
    return 0;

  NumTalentsAtLevelEntry const *dkBase =
      sNumTalentsAtLevelStore.LookupEntry(55);
  if (!dkBase)
    return 0;

  float talentPointsForLevel = count->NumberOfTalents - dkBase->NumberOfTalents;
  talentPointsForLevel += float(GetQuestRewardedTalentCount());

  if (talentPointsForLevel > baseForLevel)
    talentPointsForLevel = baseForLevel;

  return uint32(talentPointsForLevel * sWorld->getRate(RATE_TALENT));
}

void Player::LearnSpellHighestRank(uint32 spellid) {
  LearnSpell(spellid, false);

  if (uint32 next = sSpellMgr->GetNextSpellInChain(spellid))
    LearnSpellHighestRank(next);
}

void Player::_LoadSkills(PreparedQueryResult result) {
  //                                                           0      1      2
  // SetPQuery(PLAYER_LOGIN_QUERY_LOADSKILLS,          "SELECT skill, value, max
  // FROM character_skills WHERE guid = '%u'", GUID_LOPART(m_guid));

  uint8 professionCount = 0;
  std::unordered_map<uint32, uint32> loadedSkillValues;
  if (result) {
    do {
      if (mSkillStatus.size() >= PLAYER_MAX_SKILLS) // client limit
      {
        LOG_ERROR(
            "entities.player",
            "Player::_LoadSkills: Player '%s' (%s) has more than %u skills.",
            GetName().c_str(), GetGUID().ToString().c_str(), PLAYER_MAX_SKILLS);
        break;
      }

      Field *fields = result->Fetch();
      uint16 skill = fields[0].Get<uint16>();
      uint16 value = fields[1].Get<uint16>();
      uint16 max = fields[2].Get<uint16>();

      SkillRaceClassInfoEntry const *rcEntry =
          sDBCManager.GetSkillRaceClassInfo(skill, getRace(), getClass());
      if (!rcEntry) {
        LOG_ERROR("entities.player",
                  "Player::_LoadSkills: Player '%s' (%s, Race: %u, Class: %u) "
                  "has forbidden skill %u for his race/class combination",
                  GetName().c_str(), GetGUID().ToString().c_str(),
                  uint32(getRace()), uint32(getClass()), skill);

        mSkillStatus.insert(SkillStatusMap::value_type(
            skill, SkillStatusData(mSkillStatus.size(), SKILL_DELETED)));
        continue;
      }

      // set fixed skill ranges
      switch (GetSkillRangeType(rcEntry)) {
      case SKILL_RANGE_LANGUAGE: // 300..300
        value = max = 300;
        break;
      case SKILL_RANGE_MONO: // 1..1, grey monolite bar
        value = max = 1;
        break;
      case SKILL_RANGE_LEVEL:
        max = GetMaxSkillValueForLevel();
        break;
      default:
        break;
      }

      auto skillItr = mSkillStatus.find(skill);
      if (skillItr == mSkillStatus.end())
        skillItr = mSkillStatus
                       .insert(SkillStatusMap::value_type(
                           skill, SkillStatusData(mSkillStatus.size(),
                                                  SKILL_UNCHANGED)))
                       .first;

      uint16 step = 0;

      if (SkillLineEntry const *skillLine =
              sSkillLineStore.LookupEntry(rcEntry->SkillID)) {
        if (skillLine->CategoryID == SKILL_CATEGORY_SECONDARY)
          step = max / 75;

        if (skillLine->CategoryID == SKILL_CATEGORY_PROFESSION) {
          step = max / 75;

          if (professionCount < 2)
            SetUInt32Value(PLAYER_PROFESSION_SKILL_LINE_1 + professionCount++,
                           skill);
        }
      }

      SetSkillLineId(skillItr->second.pos, skill);
      SetSkillStep(skillItr->second.pos, step);
      SetSkillRank(skillItr->second.pos, value);
      SetSkillMaxRank(skillItr->second.pos, max);
      SetSkillTempBonus(skillItr->second.pos, 0);
      SetSkillPermBonus(skillItr->second.pos, 0);

      loadedSkillValues[skill] = value;

      if (skill == SKILL_ARCHAEOLOGY)
        _archaeology->Initialize();
    } while (result->NextRow());
  }

  // Learn skill rewarded spells after all skills have been loaded to prevent
  // learning a skill from them before its loaded with proper value from DB
  for (auto &skill : loadedSkillValues)
    LearnSkillRewardedSpells(skill.first, skill.second);

  if (HasSkill(SKILL_FIST_WEAPONS))
    SetSkill(SKILL_FIST_WEAPONS, 0, GetSkillValue(SKILL_UNARMED),
             GetMaxSkillValueForLevel());
}

InventoryResult Player::CanEquipUniqueItem(Item *pItem, uint8 eslot,
                                           uint32 limit_count) const {
  ItemTemplate const *pProto = pItem->GetTemplate();

  // proto based limitations
  if (InventoryResult res = CanEquipUniqueItem(pProto, eslot, limit_count))
    return res;

  // check unique-equipped on gems
  for (uint32 enchant_slot = SOCK_ENCHANTMENT_SLOT;
       enchant_slot < SOCK_ENCHANTMENT_SLOT + 3; ++enchant_slot) {
    uint32 enchant_id = pItem->GetEnchantmentId(EnchantmentSlot(enchant_slot));
    if (!enchant_id)
      continue;
    SpellItemEnchantmentEntry const *enchantEntry =
        sSpellItemEnchantmentStore.LookupEntry(enchant_id);
    if (!enchantEntry)
      continue;

    ItemTemplate const *pGem =
        sObjectMgr->GetItemTemplate(enchantEntry->Src_itemID);
    if (!pGem)
      continue;

    // include for check equip another gems with same limit category for not
    // equipped item (and then not counted)
    uint32 gem_limit_count =
        !pItem->IsEquipped() && pGem->GetItemLimitCategory()
            ? pItem->GetGemCountWithLimitCategory(pGem->GetItemLimitCategory())
            : 1;

    if (InventoryResult res = CanEquipUniqueItem(pGem, eslot, gem_limit_count))
      return res;
  }

  return EQUIP_ERR_OK;
}

InventoryResult Player::CanEquipUniqueItem(ItemTemplate const *itemProto,
                                           uint8 except_slot,
                                           uint32 limit_count) const {
  // check unique-equipped on item
  if (itemProto->GetFlags() & ITEM_FLAG_UNIQUE_EQUIPPABLE) {
    // there is an equip limit on this item
    if (HasItemOrGemWithIdEquipped(itemProto->GetId(), 1, except_slot))
      return EQUIP_ERR_ITEM_UNIQUE_EQUIPPABLE;
  }

  // check unique-equipped limit
  if (itemProto->GetItemLimitCategory()) {
    ItemLimitCategoryEntry const *limitEntry =
        sItemLimitCategoryStore.LookupEntry(itemProto->GetItemLimitCategory());
    if (!limitEntry)
      return EQUIP_ERR_NOT_EQUIPPABLE;

    // NOTE: limitEntry->mode not checked because if item have have-limit then
    // it applied and to equip case

    if (limit_count > limitEntry->Quantity)
      return EQUIP_ERR_ITEM_MAX_LIMIT_CATEGORY_EQUIPPED_EXCEEDED_IS;

    // there is an equip limit on this item
    if (HasItemWithLimitCategoryEquipped(itemProto->GetItemLimitCategory(),
                                         limitEntry->Quantity - limit_count + 1,
                                         except_slot))
      return EQUIP_ERR_ITEM_MAX_LIMIT_CATEGORY_EQUIPPED_EXCEEDED_IS;
    else if (HasGemWithLimitCategoryEquipped(
                 itemProto->GetItemLimitCategory(),
                 limitEntry->Quantity - limit_count + 1, except_slot))
      return EQUIP_ERR_ITEM_MAX_COUNT_EQUIPPED_SOCKETED;
  }

  return EQUIP_ERR_OK;
}

void Player::SetFallInformation(uint32 time, float z) {
  m_lastFallTime = time;
  m_lastFallZ = z;
}

void Player::HandleFall(MovementInfo const &movementInfo) {
  // calculate total z distance of the fall
  float z_diff = m_lastFallZ - movementInfo.pos.GetPositionZ();
  // LOG_DEBUG("misc", "zDiff = %f", z_diff);

  // Players with low fall distance, Feather Fall or physical immunity (charges
  // used) are ignored
  //  14.57 can be calculated by resolving damageperc formula below to 0
  if (z_diff >= 14.57f && !isDead() && !IsGameMaster() &&
      !HasAuraType(SPELL_AURA_HOVER) && !HasAuraType(SPELL_AURA_FEATHER_FALL) &&
      !HasAuraType(SPELL_AURA_FLY) &&
      !IsImmunedToDamage(SPELL_SCHOOL_MASK_NORMAL)) {
    // Safe fall, fall height reduction
    int32 safe_fall = GetTotalAuraModifier(SPELL_AURA_SAFE_FALL);

    float damageperc = 0.018f * (z_diff - safe_fall) - 0.2426f;

    if (damageperc > 0) {
      uint32 damage = (uint32)(damageperc * GetMaxHealth() *
                               sWorld->getRate(RATE_DAMAGE_FALL));

      if (GetCommandStatus(CHEAT_GOD))
        damage = 0;

      float height = movementInfo.pos.m_positionZ;
      UpdateGroundPositionZ(movementInfo.pos.m_positionX,
                            movementInfo.pos.m_positionY, height);

      if (damage > 0) {
        // Prevent fall damage from being more than the player maximum health
        if (damage > GetMaxHealth())
          damage = GetMaxHealth();

        // Gust of Wind
        if (HasAura(43621))
          damage = GetMaxHealth() / 2;

        uint32 original_health = GetHealth();
        uint32 final_damage = EnvironmentalDamage(DAMAGE_FALL, damage);

        // recheck alive, might have died of EnvironmentalDamage, avoid cases
        // when player die in fact like Spirit of Redemption case
        if (IsAlive() && final_damage < original_health)
          UpdateAchievementCriteria(
              ACHIEVEMENT_CRITERIA_TYPE_FALL_WITHOUT_DYING,
              uint32(z_diff * 100));
      }

      // Z given by moveinfo, LastZ, FallTime, WaterZ, MapZ, Damage, Safefall
      // reduction
      LOG_DEBUG("entities.player",
                "FALLDAMAGE z=%f sz=%f pZ=%f FallTime=%d mZ=%f damage=%d SF=%d",
                movementInfo.pos.GetPositionZ(), height, GetPositionZ(),
                movementInfo.jump.fallTime, height, damage, safe_fall);
    }
  }
}

bool Player::SetDisableGravity(bool disable, bool /*packetOnly = false*/,
                               bool /*updateAnimationTier = true*/) {
  if (disable == IsGravityDisabled())
    return false;

  if (IsMovedByClient() && IsInWorld())
    MovementPacketSender::SendMovementFlagChangeToMover(
        this, MOVEMENTFLAG_DISABLE_GRAVITY, disable);

  Unit::SetDisableGravity(disable, false, false);

  return true;
}

bool Player::SetCanFly(bool enable, bool /*packetOnly = false*/) {
  if (enable == HasUnitMovementFlag(MOVEMENTFLAG_CAN_FLY))
    return false;

  if (IsMovedByClient() && IsInWorld())
    MovementPacketSender::SendMovementFlagChangeToMover(
        this, MOVEMENTFLAG_CAN_FLY, enable);

  Unit::SetCanFly(enable);

  return true;
}

bool Player::SetCanTransitionBetweenSwimAndFly(bool enable) {
  if (enable == HasExtraUnitMovementFlag(MOVEMENTFLAG2_CAN_SWIM_TO_FLY_TRANS))
    return false;

  if (IsMovedByClient() && IsInWorld())
    MovementPacketSender::SendMovementFlagChangeToMover(
        this, MOVEMENTFLAG2_CAN_SWIM_TO_FLY_TRANS, enable);

  Unit::SetCanTransitionBetweenSwimAndFly(enable);

  return true;
}

void Player::SendMovementSetCollisionHeight(
    float height, UpdateCollisionHeightReason reason) {
  if (IsMovedByClient() && IsInWorld())
    MovementPacketSender::SendHeightChangeToMover(this, height, reason);
}

void Player::ResetAchievements() { m_achievementMgr->Reset(); }

void Player::SendRespondInspectAchievements(Player *player) const {
  m_achievementMgr->SendAchievementInfo(player);
}

uint32 Player::GetAchievementPoints() const {
  return m_achievementMgr->GetAchievementPoints();
}

bool Player::HasAchieved(uint32 achievementId) const {
  return m_achievementMgr->HasAchieved(achievementId);
}

void Player::StartTimedAchievement(AchievementCriteriaTimedTypes type,
                                   uint32 entry, uint32 timeLost /* = 0*/) {
  m_achievementMgr->StartTimedAchievement(type, entry, timeLost);
}

void Player::RemoveTimedAchievement(AchievementCriteriaTimedTypes type,
                                    uint32 entry) {
  m_achievementMgr->RemoveTimedAchievement(type, entry);
}

void Player::ResetAchievementCriteria(
    AchievementCriteriaCondition condition, uint64 value,
    bool evenIfCriteriaComplete /* = false*/) {
  m_achievementMgr->ResetAchievementCriteria(condition, value,
                                             evenIfCriteriaComplete);
}

void Player::UpdateAchievementCriteria(AchievementCriteriaTypes type,
                                       uint64 miscValue1 /*= 0*/,
                                       uint64 miscValue2 /*= 0*/,
                                       uint64 miscValue3 /*= 0*/,
                                       Unit *unit /*= nullptr*/,
                                       GameObject *go /*= nullptr*/) {
  m_achievementMgr->UpdateAchievementCriteria(type, miscValue1, miscValue2,
                                              miscValue3, unit, this);
  Guild *guild = sGuildMgr->GetGuildById(GetGuildId());
  if (!guild)
    return;

  // Update only individual achievement criteria here, otherwise we may get
  // multiple updates from a single boss kill
  if (sAchievementMgr->IsGroupCriteriaType(type))
    return;

  guild->UpdateAchievementCriteria(type, miscValue1, miscValue2, miscValue3,
                                   unit, this, go);
}

void Player::CompletedAchievement(AchievementEntry const *entry) {
  m_achievementMgr->CompletedAchievement(entry, this);
}

bool Player::LearnTalent(uint32 talentId, uint32 talentRank) {
  uint32 CurTalentPoints = GetFreeTalentPoints();

  if (CurTalentPoints == 0)
    return false;

  if (talentRank >= MAX_TALENT_RANK)
    return false;

  TalentEntry const *talentInfo = sTalentStore.LookupEntry(talentId);

  if (!talentInfo)
    return false;

  TalentTabEntry const *talentTabInfo =
      sTalentTabStore.LookupEntry(talentInfo->TabID);

  if (!talentTabInfo)
    return false;

  // prevent learn talent for different class (cheating)
  if ((getClassMask() & talentTabInfo->ClassMask) == 0)
    return false;

  // find current max talent rank (0~5)
  uint8 curtalent_maxrank = 0; // 0 = not learned any rank
  for (int8 rank = MAX_TALENT_RANK - 1; rank >= 0; --rank) {
    if (talentInfo->SpellRank[rank] && HasSpell(talentInfo->SpellRank[rank])) {
      curtalent_maxrank = (rank + 1);
      break;
    }
  }

  // we already have same or higher talent rank learned
  if (curtalent_maxrank >= (talentRank + 1))
    return false;

  // check if we have enough talent points
  if (CurTalentPoints < (talentRank - curtalent_maxrank + 1))
    return false;

  // Check if it requires another talent
  if (talentInfo->PrereqTalent[0] > 0) {
    if (TalentEntry const *depTalentInfo =
            sTalentStore.LookupEntry(talentInfo->PrereqTalent[0])) {
      bool hasEnoughRank = false;
      for (uint8 rank = talentInfo->PrereqRank[0]; rank < MAX_TALENT_RANK;
           rank++) {
        if (depTalentInfo->SpellRank[rank] != 0)
          if (HasSpell(depTalentInfo->SpellRank[rank]))
            hasEnoughRank = true;
      }
      if (!hasEnoughRank)
        return false;
    }
  }

  // Find out how many points we have in this field
  uint32 spentPoints = 0;
  uint32 primaryTreeTalents = 0;
  uint32 primaryTreeId = GetPrimaryTalentTree(GetActiveSpec());
  bool isMainTree = primaryTreeId == talentInfo->TabID || !primaryTreeId;

  if (talentInfo->TierID > 0 || !isMainTree) {
    for (uint32 i = 0; i < sTalentStore.GetNumRows();
         i++) // Loop through all talents.
    {
      if (TalentEntry const *tmpTalent =
              sTalentStore.LookupEntry(i)) // Someday, someone needs to revamp
                                           // the way talents are tracked
      {
        for (uint8 rank = 0; rank < MAX_TALENT_RANK; ++rank) {
          if (tmpTalent->SpellRank[rank] != 0) {
            if (HasSpell(tmpTalent->SpellRank[rank])) {
              if (tmpTalent->TabID == talentInfo->TabID)
                spentPoints += (rank + 1);
              if (tmpTalent->TabID == primaryTreeId)
                primaryTreeTalents += (rank + 1);
            }
          }
        }
      }
    }
  }

  // not have required min points spent in talent tree
  if (spentPoints < (talentInfo->TierID * MAX_TALENT_RANK))
    return false;

  // player has not spent 31 talents in main tree before attempting to learn
  // other tree's talents
  if (!isMainTree && primaryTreeTalents < REQ_PRIMARY_TREE_TALENTS)
    return false;

  // spell not set in talent.dbc
  uint32 spellid = talentInfo->SpellRank[talentRank];
  if (spellid == 0) {
    LOG_ERROR("entities.player",
              "Player::LearnTalent: Talent.dbc has no spellInfo for talent: %u "
              "(spell id = 0)",
              talentId);
    return false;
  }

  // already known
  if (HasSpell(spellid))
    return false;

  // learn! (other talent ranks will unlearned at learning)
  LearnSpell(spellid, false);
  AddTalent(spellid, GetActiveSpec(), true);

  LOG_DEBUG("entities.player", "TalentID: %u Rank: %u Spell: %u Spec: %u\n",
            talentId, talentRank, spellid, GetActiveSpec());

  // set talent tree for player
  if (!GetPrimaryTalentTree(GetActiveSpec())) {
    SetPrimaryTalentTree(GetActiveSpec(), talentInfo->TabID);
    std::vector<uint32> const *specSpells =
        sDBCManager.GetTalentTreePrimarySpells(talentInfo->TabID);
    if (specSpells)
      for (size_t i = 0; i < specSpells->size(); ++i)
        LearnSpell(specSpells->at(i), false);

    if (CanUseMastery())
      for (uint32 i = 0; i < MAX_MASTERY_SPELLS; ++i)
        if (SpellInfo const *masterySpell =
                sSpellMgr->GetSpellInfo(talentTabInfo->MasterySpellID[i]))
          if (masterySpell->IsPassive() &&
              HandlePassiveSpellLearn(masterySpell))
            CastSpell(this, masterySpell->Id, true);
  }

  // update free talent points
  SetFreeTalentPoints(CurTalentPoints - (talentRank - curtalent_maxrank + 1));
  return true;
}

void Player::LearnPetTalent(ObjectGuid petGuid, uint32 talentId,
                            uint32 talentRank) {
  Pet *pet = GetPet();

  if (!pet)
    return;

  if (petGuid != pet->GetGUID())
    return;

  uint32 CurTalentPoints = pet->GetFreeTalentPoints();

  if (CurTalentPoints == 0)
    return;

  if (talentRank >= MAX_PET_TALENT_RANK)
    return;

  TalentEntry const *talentInfo = sTalentStore.LookupEntry(talentId);

  if (!talentInfo)
    return;

  TalentTabEntry const *talentTabInfo =
      sTalentTabStore.LookupEntry(talentInfo->TabID);

  if (!talentTabInfo)
    return;

  CreatureTemplate const *ci = pet->GetCreatureTemplate();

  if (!ci)
    return;

  CreatureFamilyEntry const *pet_family =
      sCreatureFamilyStore.LookupEntry(ci->family);

  if (!pet_family)
    return;

  if (pet_family->PetTalentType < 0) // not hunter pet
    return;

  // prevent learn talent for different family (cheating)
  if (!((1 << pet_family->PetTalentType) & talentTabInfo->CategoryEnumID))
    return;

  // find current max talent rank (0~5)
  uint8 curtalent_maxrank = 0; // 0 = not learned any rank
  for (int8 rank = MAX_TALENT_RANK - 1; rank >= 0; --rank) {
    if (talentInfo->SpellRank[rank] &&
        pet->HasSpell(talentInfo->SpellRank[rank])) {
      curtalent_maxrank = (rank + 1);
      break;
    }
  }

  // we already have same or higher talent rank learned
  if (curtalent_maxrank >= (talentRank + 1))
    return;

  // check if we have enough talent points
  if (CurTalentPoints < (talentRank - curtalent_maxrank + 1))
    return;

  // Check if it requires another talent
  if (talentInfo->PrereqTalent[0] > 0) {
    if (TalentEntry const *depTalentInfo =
            sTalentStore.LookupEntry(talentInfo->PrereqTalent[0])) {
      bool hasEnoughRank = false;
      for (uint8 rank = talentInfo->PrereqRank[0]; rank < MAX_TALENT_RANK;
           rank++) {
        if (depTalentInfo->SpellRank[rank] != 0)
          if (pet->HasSpell(depTalentInfo->SpellRank[rank]))
            hasEnoughRank = true;
      }
      if (!hasEnoughRank)
        return;
    }
  }

  // Find out how many points we have in this field
  uint32 spentPoints = 0;

  uint32 tTab = talentInfo->TabID;
  if (talentInfo->TierID > 0) {
    uint32 numRows = sTalentStore.GetNumRows();
    for (uint32 i = 0; i < numRows; ++i) // Loop through all talents.
    {
      // Someday, someone needs to revamp
      TalentEntry const *tmpTalent = sTalentStore.LookupEntry(i);
      if (tmpTalent) // the way talents are tracked
      {
        if (tmpTalent->TabID == tTab) {
          for (uint8 rank = 0; rank < MAX_TALENT_RANK; rank++) {
            if (tmpTalent->SpellRank[rank] != 0) {
              if (pet->HasSpell(tmpTalent->SpellRank[rank])) {
                spentPoints += (rank + 1);
              }
            }
          }
        }
      }
    }
  }

  // not have required min points spent in talent tree
  if (spentPoints < (talentInfo->TierID * MAX_PET_TALENT_RANK))
    return;

  // spell not set in talent.dbc
  uint32 spellid = talentInfo->SpellRank[talentRank];
  if (spellid == 0) {
    LOG_ERROR("entities.player",
              "Talent.dbc contains talent: %u Rank: %u spell id = 0", talentId,
              talentRank);
    return;
  }

  // already known
  if (pet->HasSpell(spellid))
    return;

  // learn! (other talent ranks will unlearned at learning)
  pet->learnSpell(spellid);
  LOG_DEBUG("entities.player", "PetTalentID: %u Rank: %u Spell: %u\n", talentId,
            talentRank, spellid);

  // update free talent points
  pet->SetFreeTalentPoints(CurTalentPoints -
                           (talentRank - curtalent_maxrank + 1));
}

void Player::AddKnownCurrency(uint32 itemId) {
  if (CurrencyTypesEntry const *ctEntry =
          sCurrencyTypesStore.LookupEntry(itemId))
    SetFlag64(0, (1LL << (ctEntry->ID - 1)));
}

void Player::UpdateFallInformationIfNeed(MovementInfo const &minfo,
                                         uint16 opcode) {
  if (m_lastFallTime >= minfo.jump.fallTime ||
      m_lastFallZ <= minfo.pos.GetPositionZ() || opcode == MSG_MOVE_FALL_LAND)
    SetFallInformation(minfo.jump.fallTime, minfo.pos.GetPositionZ());
}

void Player::UnsummonPetTemporaryIfAny() {
  Pet *pet = GetPet();
  if (!pet)
    return;

  if (!m_temporaryUnsummonedPetNumber && pet->isControlled() &&
      !pet->isTemporarySummoned()) {
    m_temporaryUnsummonedPetNumber = pet->GetCharmInfo()->GetPetNumber();
    m_oldpetspell = pet->GetUInt32Value(UNIT_CREATED_BY_SPELL);
  }

  RemovePet(pet, PET_SAVE_TEMP_UNSUMMON);
}

void Player::ResummonPetTemporaryUnSummonedIfAny() {
  if (!m_temporaryUnsummonedPetNumber)
    return;

  // not resummon in not appropriate state
  if (IsPetNeedBeTemporaryUnsummoned())
    return;

  if (GetPetGUID())
    return;

  Pet *NewPet = new Pet(this);
  if (!NewPet->LoadPetData(this, 0, m_temporaryUnsummonedPetNumber, false))
    delete NewPet;

  m_temporaryUnsummonedPetNumber = 0;
}

bool Player::IsPetNeedBeTemporaryUnsummoned() const {
  return !IsInWorld() || !IsAlive() || IsMounted() /*+in flight*/;
}

bool Player::CanSeeSpellClickOn(Creature const *c) const {
  if (!c->HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK))
    return false;

  SpellClickInfoMapBounds clickPair =
      sObjectMgr->GetSpellClickInfoMapBounds(c->GetEntry());
  if (clickPair.first == clickPair.second)
    return true;

  for (SpellClickInfoContainer::const_iterator itr = clickPair.first;
       itr != clickPair.second; ++itr) {
    if (!itr->second.IsFitToRequirements(this, c))
      return false;

    if (sConditionMgr->IsObjectMeetingSpellClickConditions(
            c->GetEntry(), itr->second.spellId, const_cast<Player *>(this),
            const_cast<Creature *>(c)))
      return true;
  }

  return false;
}

uint8 Player::GetExpansionForFaction(uint32 faction) const {
  FactionEntry const *baseFaction = sFactionStore.LookupEntry(faction);
  if (!baseFaction || !baseFaction->ParentFactionID)
    return 0;

  FactionEntry const *expansionFaction =
      sFactionStore.LookupEntry(baseFaction->ParentFactionID);
  if (!expansionFaction)
    return 0;

  return expansionFaction->Expansion;
}

void Player::BuildPlayerTalentsInfoData(WorldPacket *data) {
  *data << uint32(GetFreeTalentPoints()); // unspentTalentPoints
  *data << uint8(GetSpecsCount());        // talent group count (0, 1 or 2)
  *data << uint8(GetActiveSpec());        // talent group index (0 or 1)

  if (GetSpecsCount()) {
    if (GetSpecsCount() > MAX_TALENT_SPECS)
      SetSpecsCount(MAX_TALENT_SPECS);

    // loop through all specs (only 1 for now)
    for (uint8 specIdx = 0; specIdx < GetSpecsCount(); ++specIdx) {
      *data << uint32(GetPrimaryTalentTree(specIdx));
      uint8 talentIdCount = 0;
      size_t pos = data->wpos();
      *data << uint8(talentIdCount); // [PH], talentIdCount

      // find class talent tabs (all players have 3 talent tabs)
      uint32 const *talentTabIds = sDBCManager.GetTalentTabPages(getClass());

      for (uint8 i = 0; i < MAX_TALENT_TABS; ++i) {
        uint32 talentTabId = talentTabIds[i];

        for (uint32 talentId = 0; talentId < sTalentStore.GetNumRows();
             ++talentId) {
          TalentEntry const *talentInfo = sTalentStore.LookupEntry(talentId);
          if (!talentInfo)
            continue;

          // skip another tab talents
          if (talentInfo->TabID != talentTabId)
            continue;

          // find max talent rank (0~4)
          int8 curtalent_maxrank = -1;
          for (int8 rank = MAX_TALENT_RANK - 1; rank >= 0; --rank) {
            if (talentInfo->SpellRank[rank] &&
                HasTalent(talentInfo->SpellRank[rank], specIdx)) {
              curtalent_maxrank = rank;
              break;
            }
          }

          // not learned talent
          if (curtalent_maxrank < 0)
            continue;

          *data << uint32(talentInfo->ID);   // Talent.dbc
          *data << uint8(curtalent_maxrank); // talentMaxRank (0-4)

          ++talentIdCount;
        }
      }

      data->put<uint8>(pos, talentIdCount); // put real count

      *data << uint8(MAX_GLYPH_SLOT_INDEX); // glyphs count

      for (uint8 i = 0; i < MAX_GLYPH_SLOT_INDEX; ++i)
        *data << uint16(GetGlyph(specIdx, i)); // GlyphProperties.dbc
    }
  }
}

void Player::BuildPetTalentsInfoData(WorldPacket *data) {
  uint32 unspentTalentPoints = 0;
  size_t pointsPos = data->wpos();
  *data << uint32(unspentTalentPoints); // [PH], unspentTalentPoints

  uint8 talentIdCount = 0;
  size_t countPos = data->wpos();
  *data << uint8(talentIdCount); // [PH], talentIdCount

  Pet *pet = GetPet();
  if (!pet)
    return;

  unspentTalentPoints = pet->GetFreeTalentPoints();

  data->put<uint32>(pointsPos, unspentTalentPoints); // put real points

  CreatureTemplate const *ci = pet->GetCreatureTemplate();
  if (!ci)
    return;

  CreatureFamilyEntry const *pet_family =
      sCreatureFamilyStore.LookupEntry(ci->family);
  if (!pet_family || pet_family->PetTalentType < 0)
    return;

  for (uint32 talentTabId = 1; talentTabId < sTalentTabStore.GetNumRows();
       ++talentTabId) {
    TalentTabEntry const *talentTabInfo =
        sTalentTabStore.LookupEntry(talentTabId);
    if (!talentTabInfo)
      continue;

    if (!((1 << pet_family->PetTalentType) & talentTabInfo->CategoryEnumID))
      continue;

    for (uint32 talentId = 0; talentId < sTalentStore.GetNumRows();
         ++talentId) {
      TalentEntry const *talentInfo = sTalentStore.LookupEntry(talentId);
      if (!talentInfo)
        continue;

      // skip another tab talents
      if (talentInfo->TabID != talentTabId)
        continue;

      // find max talent rank (0~4)
      int8 curtalent_maxrank = -1;
      for (int8 rank = MAX_TALENT_RANK - 1; rank >= 0; --rank) {
        if (talentInfo->SpellRank[rank] &&
            pet->HasSpell(talentInfo->SpellRank[rank])) {
          curtalent_maxrank = rank;
          break;
        }
      }

      // not learned talent
      if (curtalent_maxrank < 0)
        continue;

      *data << uint32(talentInfo->ID);   // Talent.dbc
      *data << uint8(curtalent_maxrank); // talentMaxRank (0-4)

      ++talentIdCount;
    }

    data->put<uint8>(countPos, talentIdCount); // put real count

    break;
  }
}

void Player::SendTalentsInfoData(bool pet) {
  WorldPacket data(SMSG_TALENTS_INFO, 50);
  data << uint8(pet ? 1 : 0);
  if (pet)
    BuildPetTalentsInfoData(&data);
  else
    BuildPlayerTalentsInfoData(&data);
  SendDirectMessage(&data);
}

void Player::BuildEnchantmentsInfoData(WorldPacket *data) {
  uint32 slotUsedMask = 0;
  size_t slotUsedMaskPos = data->wpos();
  *data << uint32(slotUsedMask); // slotUsedMask < 0x80000

  for (uint32 i = 0; i < EQUIPMENT_SLOT_END; ++i) {
    Item *item = GetItemByPos(INVENTORY_SLOT_BAG_0, i);

    if (!item)
      continue;

    slotUsedMask |= (1 << i);

    *data << uint32(item->GetEntry()); // item entry

    uint16 enchantmentMask = 0;
    size_t enchantmentMaskPos = data->wpos();
    *data << uint16(enchantmentMask); // enchantmentMask < 0x1000

    for (uint32 j = 0; j < MAX_ENCHANTMENT_SLOT; ++j) {
      uint32 enchId = item->GetEnchantmentId(EnchantmentSlot(j));

      if (!enchId)
        continue;

      enchantmentMask |= (1 << j);

      *data << uint16(enchId);
    }

    data->put<uint16>(enchantmentMaskPos, enchantmentMask);

    *data << uint16(item->GetItemRandomPropertyId());
    *data << item->GetGuidValue(ITEM_FIELD_CREATOR).WriteAsPacked();
    *data << uint32(item->GetItemSuffixFactor());
  }

  data->put<uint32>(slotUsedMaskPos, slotUsedMask);
}

void Player::SendEquipmentSetList() {
  uint32 count = 0;
  WorldPacket data(SMSG_EQUIPMENT_SET_LIST, 1000); // guess size
  size_t count_pos = data.wpos();
  data << uint32(count); // count placeholder
  for (EquipmentSetContainer::value_type const &eqSet : _equipmentSets) {
    if (eqSet.second.State == EQUIPMENT_SET_DELETED)
      continue;

    data.appendPackGUID(eqSet.first);
    data << uint32(eqSet.second.Data.SetID);
    data << eqSet.second.Data.SetName;
    data << eqSet.second.Data.SetIcon;
    for (uint32 i = 0; i < EQUIPMENT_SLOT_END; ++i) {
      // ignored slots stored in IgnoreMask, client wants "1" as raw GUID, so no
      // HighGuid::Item
      if (eqSet.second.Data.IgnoreMask & (1 << i))
        data.appendPackGUID(uint64(1));
      else
        data.appendPackGUID(eqSet.second.Data.Pieces[i]);
    }

    ++count; // client have limit but it checked at loading and set
  }
  data.put<uint32>(count_pos, count);
  SendDirectMessage(&data);
}

void Player::SetEquipmentSet(EquipmentSetInfo::EquipmentSetData const &eqSet) {
  if (eqSet.Guid != 0) {
    // something wrong...
    auto itr = _equipmentSets.find(eqSet.Guid);
    if (itr == _equipmentSets.end() || itr->second.Data.Guid != eqSet.Guid) {
      LOG_ERROR("entities.player",
                "Player::SetEquipmentSet: Player '%s' (%s) tried to save "
                "nonexistent equipment set " UI64FMTD " (index: %u)",
                GetName().c_str(), GetGUID().ToString().c_str(), eqSet.Guid,
                eqSet.SetID);
      return;
    }
  }

  uint64 setGuid =
      (eqSet.Guid != 0) ? eqSet.Guid : sObjectMgr->GenerateEquipmentSetGuid();

  EquipmentSetInfo &eqSlot = _equipmentSets[setGuid];
  eqSlot.Data = eqSet;

  if (eqSet.Guid == 0) {
    eqSlot.Data.Guid = setGuid;

    WorldPacket data(SMSG_EQUIPMENT_SET_SAVED, 4 + 1);
    data << uint32(eqSlot.Data.SetID);
    data.appendPackGUID(eqSlot.Data.Guid);
    SendDirectMessage(&data);
  }

  eqSlot.State = (eqSlot.State == EQUIPMENT_SET_NEW ? EQUIPMENT_SET_NEW
                                                    : EQUIPMENT_SET_CHANGED);
}

void Player::_SaveEquipmentSets(CharacterDatabaseTransaction &trans) {
  for (EquipmentSetContainer::iterator itr = _equipmentSets.begin();
       itr != _equipmentSets.end();) {
    EquipmentSetInfo &eqSet = itr->second;
    CharacterDatabasePreparedStatement *stmt;
    uint8 j = 0;
    switch (eqSet.State) {
    case EQUIPMENT_SET_UNCHANGED:
      ++itr;
      break; // nothing do
    case EQUIPMENT_SET_CHANGED:
      stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_EQUIP_SET);
      stmt->SetData(j++, eqSet.Data.SetName);
      stmt->SetData(j++, eqSet.Data.SetIcon);
      stmt->SetData(j++, eqSet.Data.IgnoreMask);
      for (uint8 i = 0; i < EQUIPMENT_SLOT_END; ++i)
        stmt->SetData(j++, eqSet.Data.Pieces[i].GetCounter());
      stmt->SetData(j++, GetGUID().GetCounter());
      stmt->SetData(j++, eqSet.Data.Guid);
      stmt->SetData(j, eqSet.Data.SetID);
      trans->Append(stmt);
      eqSet.State = EQUIPMENT_SET_UNCHANGED;
      ++itr;
      break;
    case EQUIPMENT_SET_NEW:
      stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_EQUIP_SET);
      stmt->SetData(j++, GetGUID().GetCounter());
      stmt->SetData(j++, eqSet.Data.Guid);
      stmt->SetData(j++, eqSet.Data.SetID);
      stmt->SetData(j++, eqSet.Data.SetName);
      stmt->SetData(j++, eqSet.Data.SetIcon);
      stmt->SetData(j++, eqSet.Data.IgnoreMask);
      for (uint8 i = 0; i < EQUIPMENT_SLOT_END; ++i)
        stmt->SetData(j++, eqSet.Data.Pieces[i].GetCounter());
      trans->Append(stmt);
      eqSet.State = EQUIPMENT_SET_UNCHANGED;
      ++itr;
      break;
    case EQUIPMENT_SET_DELETED:
      stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_EQUIP_SET);
      stmt->SetData(0, eqSet.Data.Guid);
      trans->Append(stmt);
      itr = _equipmentSets.erase(itr);
      break;
    }
  }
}

void Player::_SaveBGData(CharacterDatabaseTransaction &trans) {
  CharacterDatabasePreparedStatement *stmt =
      CharacterDatabase.GetPreparedStatement(CHAR_DEL_PLAYER_BGDATA);
  stmt->SetData(0, GetGUID().GetCounter());
  trans->Append(stmt);
  /* guid, bgInstanceID, bgTeam, x, y, z, o, map, taxi[0], taxi[1], mountSpell
   */
  stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_PLAYER_BGDATA);
  stmt->SetData(0, GetGUID().GetCounter());
  stmt->SetData(1, m_bgData.bgInstanceID);
  stmt->SetData(2, m_bgData.bgTeam);
  stmt->SetData(3, m_bgData.joinPos.GetPositionX());
  stmt->SetData(4, m_bgData.joinPos.GetPositionY());
  stmt->SetData(5, m_bgData.joinPos.GetPositionZ());
  stmt->SetData(6, m_bgData.joinPos.GetOrientation());
  stmt->SetData(7, m_bgData.joinPos.GetMapId());
  stmt->SetData(8, m_bgData.taxiPath[0]);
  stmt->SetData(9, m_bgData.taxiPath[1]);
  stmt->SetData(10, m_bgData.mountSpell);
  trans->Append(stmt);
}

void Player::DeleteEquipmentSet(uint64 setGuid) {
  for (auto itr = _equipmentSets.begin(); itr != _equipmentSets.end();) {
    if (itr->second.Data.Guid == setGuid) {
      if (itr->second.State == EQUIPMENT_SET_NEW)
        itr = _equipmentSets.erase(itr);
      else
        itr->second.State = EQUIPMENT_SET_DELETED;
      break;
    }
    ++itr;
  }
}

void Player::RemoveAtLoginFlag(AtLoginFlags flags, bool persist /*= false*/) {
  m_atLoginFlags &= ~flags;

  if (persist) {
    CharacterDatabasePreparedStatement *stmt =
        CharacterDatabase.GetPreparedStatement(CHAR_UPD_REM_AT_LOGIN_FLAG);

    stmt->SetData(0, uint16(flags));
    stmt->SetData(1, GetGUID().GetCounter());

    CharacterDatabase.Execute(stmt);
  }
}

void Player::ResetMap() {
  // this may be called during Map::Update
  // after decrement+unlink, ++m_mapRefIter will continue correctly
  // when the first element of the list is being removed
  // nocheck_prev will return the padding element of the RefManager
  // instead of nullptr in the case of prev
  GetMap()->UpdateIteratorBack(this);
  Unit::ResetMap();
  GetMapRef().unlink();
}

void Player::SetMap(Map *map) {
  Unit::SetMap(map);
  m_mapRef.link(map, this);
}

void Player::_LoadGlyphs(PreparedQueryResult result) {
  // SELECT talentGroup, glyph1, glyph2, glyph3, glyph4, glyph5, glyph6 from
  // character_glyphs WHERE guid = '%u'
  if (!result)
    return;

  do {
    Field *fields = result->Fetch();

    uint8 spec = fields[0].Get<uint8>();
    if (spec >= GetSpecsCount())
      continue;

    for (uint8 i = 0; i < MAX_GLYPH_SLOT_INDEX; ++i)
      _talentMgr->SpecInfo[spec].Glyphs[i] = fields[i + 1].Get<uint16>();
  } while (result->NextRow());
}

void Player::_SaveGlyphs(CharacterDatabaseTransaction &trans) const {
  CharacterDatabasePreparedStatement *stmt =
      CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_GLYPHS);
  stmt->SetData(0, GetGUID().GetCounter());
  trans->Append(stmt);

  for (uint8 spec = 0; spec < GetSpecsCount(); ++spec) {
    uint8 index = 0;

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_CHAR_GLYPHS);
    stmt->SetData(index++, GetGUID().GetCounter());

    stmt->SetData(index++, spec);

    for (uint8 i = 0; i < MAX_GLYPH_SLOT_INDEX; ++i)
      stmt->SetData(index++, uint16(GetGlyph(spec, i)));

    trans->Append(stmt);
  }
}

void Player::_LoadTalents(PreparedQueryResult result) {
  // SetPQuery(PLAYER_LOGIN_QUERY_LOADTALENTS, "SELECT spell, talentGroup FROM
  // character_talent WHERE guid = '%u'", GUID_LOPART(m_guid));
  if (result) {
    do
      AddTalent((*result)[0].Get<uint32>(), (*result)[1].Get<uint8>(), false);
    while (result->NextRow());
  }
}

void Player::_SaveTalents(CharacterDatabaseTransaction &trans) {
  CharacterDatabasePreparedStatement *stmt;

  for (uint8 i = 0; i < MAX_TALENT_SPECS; ++i) {
    for (PlayerTalentMap::iterator itr = GetTalentMap(i)->begin();
         itr != GetTalentMap(i)->end();) {
      if (itr->second->state == PLAYERSPELL_REMOVED ||
          itr->second->state == PLAYERSPELL_CHANGED) {
        stmt = CharacterDatabase.GetPreparedStatement(
            CHAR_DEL_CHAR_TALENT_BY_SPELL_SPEC);
        stmt->SetData(0, GetGUID().GetCounter());
        stmt->SetData(1, itr->first);
        stmt->SetData(2, itr->second->spec);
        trans->Append(stmt);
      }

      if (itr->second->state == PLAYERSPELL_NEW ||
          itr->second->state == PLAYERSPELL_CHANGED) {
        stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_CHAR_TALENT);
        stmt->SetData(0, GetGUID().GetCounter());
        stmt->SetData(1, itr->first);
        stmt->SetData(2, itr->second->spec);
        trans->Append(stmt);
      }

      if (itr->second->state == PLAYERSPELL_REMOVED) {
        delete itr->second;
        GetTalentMap(i)->erase(itr++);
      } else {
        itr->second->state = PLAYERSPELL_UNCHANGED;
        ++itr;
      }
    }
  }
}

void Player::UpdateSpecCount(uint8 count) {
  uint32 curCount = GetSpecsCount();
  if (curCount == count)
    return;

  if (GetActiveSpec() >= count)
    ActivateSpec(0);

  CharacterDatabaseTransaction trans = CharacterDatabase.BeginTransaction();
  CharacterDatabasePreparedStatement *stmt;

  // Copy spec data
  if (count > curCount) {
    _SaveActions(trans); // make sure the button list is cleaned up
    for (ActionButtonList::iterator itr = m_actionButtons.begin();
         itr != m_actionButtons.end(); ++itr) {
      stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_CHAR_ACTION);
      stmt->SetData(0, GetGUID().GetCounter());
      stmt->SetData(1, 1);
      stmt->SetData(2, itr->first);
      stmt->SetData(3, itr->second.GetAction());
      stmt->SetData(4, uint8(itr->second.GetType()));
      trans->Append(stmt);
    }
  }
  // Delete spec data for removed spec.
  else if (count < curCount) {
    _SaveActions(trans);

    stmt = CharacterDatabase.GetPreparedStatement(
        CHAR_DEL_CHAR_ACTION_EXCEPT_SPEC);
    stmt->SetData(0, GetActiveSpec());
    stmt->SetData(1, GetGUID().GetCounter());
    trans->Append(stmt);
  }

  CharacterDatabase.CommitTransaction(trans);

  SetSpecsCount(count);

  SendTalentsInfoData(false);
}

void Player::ActivateSpec(uint8 spec) {
  if (GetActiveSpec() == spec)
    return;

  if (spec > GetSpecsCount())
    return;

  if (IsNonMeleeSpellCast(false))
    InterruptNonMeleeSpells(false);

  CharacterDatabaseTransaction trans = CharacterDatabase.BeginTransaction();
  _SaveActions(trans);
  CharacterDatabase.CommitTransaction(trans);

  // TO-DO: We need more research to know what happens with warlock's reagent
  if (Pet *pet = GetPet())
    RemovePet(pet, PET_SAVE_DISMISS);

  ClearAllReactives();
  UnsummonAllTotems();
  ExitVehicle();
  RemoveAllControlled();

  // remove limited target auras at other targets
  AurasBySpellIdMap &ltAurasBySpellId = GetAllLimitedCastAuras();
  for (AurasBySpellIdMap::iterator itr = ltAurasBySpellId.begin();
       itr != ltAurasBySpellId.end(); itr++) {
    AuraList &list = itr->second;
    for (AuraList::iterator iter = list.begin(); iter != list.end();) {
      Aura *aura = *iter;
      if (aura->GetUnitOwner() != this) {
        aura->Remove();
        iter = list.begin();
      } else
        ++iter;
    }
  }

  /*RemoveAllAurasOnDeath();
  if (GetPet())
      GetPet()->RemoveAllAurasOnDeath();*/

  // RemoveAllAuras(GetGUID(), nullptr, false, true); // removes too many auras
  // ExitVehicle(); // should be impossible to switch specs from inside a
  // vehicle..

  // Let client clear his current Actions
  SendActionButtons(2);
  // m_actionButtons.clear() is called in the next _LoadActionButtons
  for (uint32 talentId = 0; talentId < sTalentStore.GetNumRows(); ++talentId) {
    TalentEntry const *talentInfo = sTalentStore.LookupEntry(talentId);

    if (!talentInfo)
      continue;

    TalentTabEntry const *talentTabInfo =
        sTalentTabStore.LookupEntry(talentInfo->TabID);

    if (!talentTabInfo)
      continue;

    // unlearn only talents for character class
    // some spell learned by one class as normal spells or know at creation but
    // another class learn it as talent, to prevent unexpected lost normal
    // learned spell skip another class talents
    if ((getClassMask() & talentTabInfo->ClassMask) == 0)
      continue;

    for (int8 rank = MAX_TALENT_RANK - 1; rank >= 0; --rank) {
      // skip non-existing talent ranks
      if (talentInfo->SpellRank[rank] == 0)
        continue;
      RemoveSpell(talentInfo->SpellRank[rank],
                  true); // removes the talent, and all dependant, learned, and
                         // chained spells..
      if (SpellInfo const *_spellEntry =
              sSpellMgr->GetSpellInfo(talentInfo->SpellRank[rank]))
        for (uint8 i = 0; i < MAX_SPELL_EFFECTS;
             ++i) // search through the SpellInfo for valid trigger spells
          if (_spellEntry->Effects[i].TriggerSpell > 0 &&
              _spellEntry->Effects[i].Effect == SPELL_EFFECT_LEARN_SPELL)
            RemoveSpell(_spellEntry->Effects[i].TriggerSpell,
                        true); // and remove any spells that the talent teaches
      // if this talent rank can be found in the PlayerTalentMap, mark the
      // talent as removed so it gets deleted
      // PlayerTalentMap::iterator plrTalent =
      // m_talents[m_activeSpec]->find(talentInfo->RankID[rank]); if (plrTalent
      // != m_talents[m_activeSpec]->end())
      //    plrTalent->second->state = PLAYERSPELL_REMOVED;
    }
  }

  // Remove spec specific spells
  for (uint32 i = 0; i < MAX_TALENT_TABS; ++i) {
    uint32 const *talentTabs = sDBCManager.GetTalentTabPages(getClass());
    std::vector<uint32> const *specSpells =
        sDBCManager.GetTalentTreePrimarySpells(talentTabs[i]);
    if (specSpells)
      for (size_t i = 0; i < specSpells->size(); ++i)
        RemoveSpell(specSpells->at(i), true);

    TalentTabEntry const *talentTabInfo =
        sTalentTabStore.LookupEntry(talentTabs[i]);
    for (uint32 i = 0; i < MAX_MASTERY_SPELLS; ++i)
      if (uint32 mastery = talentTabInfo->MasterySpellID[i])
        RemoveSpell(mastery, true);
  }

  // set glyphs
  for (uint8 slot = 0; slot < MAX_GLYPH_SLOT_INDEX; ++slot)
    // remove secondary glyph
    if (uint32 oldglyph = GetGlyph(GetActiveSpec(), slot))
      if (GlyphPropertiesEntry const *old_gp =
              sGlyphPropertiesStore.LookupEntry(oldglyph))
        RemoveAurasDueToSpell(old_gp->SpellID);

  SetActiveSpec(spec);
  uint32 spentTalents = 0;

  for (uint32 talentId = 0; talentId < sTalentStore.GetNumRows(); ++talentId) {
    TalentEntry const *talentInfo = sTalentStore.LookupEntry(talentId);

    if (!talentInfo)
      continue;

    TalentTabEntry const *talentTabInfo =
        sTalentTabStore.LookupEntry(talentInfo->TabID);

    if (!talentTabInfo)
      continue;

    // learn only talents for character class
    if ((getClassMask() & talentTabInfo->ClassMask) == 0)
      continue;

    // learn highest talent rank that exists in newly activated spec
    for (int8 rank = MAX_TALENT_RANK - 1; rank >= 0; --rank) {
      // skip non-existing talent ranks
      if (talentInfo->SpellRank[rank] == 0)
        continue;
      // if the talent can be found in the newly activated PlayerTalentMap
      if (HasTalent(talentInfo->SpellRank[rank], GetActiveSpec())) {
        LearnSpell(talentInfo->SpellRank[rank],
                   false);          // add the talent to the PlayerSpellMap
        spentTalents += (rank + 1); // increment the spentTalents count
      }
    }
  }

  std::vector<uint32> const *specSpells =
      sDBCManager.GetTalentTreePrimarySpells(
          GetPrimaryTalentTree(GetActiveSpec()));
  if (specSpells)
    for (size_t i = 0; i < specSpells->size(); ++i)
      LearnSpell(specSpells->at(i), false);

  if (CanUseMastery())
    if (TalentTabEntry const *talentTabInfo =
            sTalentTabStore.LookupEntry(GetPrimaryTalentTree(GetActiveSpec())))
      for (uint32 i = 0; i < MAX_MASTERY_SPELLS; ++i)
        if (uint32 mastery = talentTabInfo->MasterySpellID[i])
          LearnSpell(mastery, false);

  // set glyphs
  for (uint8 slot = 0; slot < MAX_GLYPH_SLOT_INDEX; ++slot) {
    uint32 glyph = GetGlyph(GetActiveSpec(), slot);

    // apply primary glyph
    if (glyph)
      if (GlyphPropertiesEntry const *gp =
              sGlyphPropertiesStore.LookupEntry(glyph))
        CastSpell(this, gp->SpellID, true);

    SetGlyph(slot, glyph);
  }

  SetUsedTalentCount(spentTalents);
  InitTalentForLevel();

  // load them asynchronously
  {
    CharacterDatabasePreparedStatement *stmt =
        CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_ACTIONS_SPEC);
    stmt->SetData(0, GetGUID().GetCounter());
    stmt->SetData(1, GetActiveSpec());

    WorldSession *mySess = GetSession();
    mySess->GetQueryProcessor().AddCallback(
        CharacterDatabase.AsyncQuery(stmt).WithPreparedCallback(
            [mySess](PreparedQueryResult result) {
              // safe callback, we can't pass this pointer directly
              // in case player logs out before db response (player would be
              // deleted in that case)
              if (Player *thisPlayer = mySess->GetPlayer())
                thisPlayer->LoadActions(result);
            }));
  }

  Powers pw = GetPowerType();
  if (pw != POWER_MANA)
    SetPower(POWER_MANA,
             0); // Mana must be 0 even if it isn't the active power type.

  SetPower(pw, 0);

  Unit::AuraEffectList const &shapeshiftAuras =
      GetAuraEffectsByType(SPELL_AURA_MOD_SHAPESHIFT);
  for (AuraEffect *aurEff : shapeshiftAuras) {
    aurEff->HandleShapeshiftBoosts(this, false);
    aurEff->HandleShapeshiftBoosts(this, true);
  }

  if (!sTalentTabStore.LookupEntry(GetPrimaryTalentTree(GetActiveSpec())))
    ResetTalents(true);
}

void Player::LoadActions(PreparedQueryResult result) {
  if (result)
    _LoadActions(result);

  SendActionButtons(1);
}

void Player::SetReputation(uint32 factionentry, uint32 value) {
  GetReputationMgr().SetReputation(sFactionStore.LookupEntry(factionentry),
                                   value);
}
uint32 Player::GetReputation(uint32 factionentry) const {
  return GetReputationMgr().GetReputation(
      sFactionStore.LookupEntry(factionentry));
}

std::string Player::GetGuildName() const {
  return GetGuildId() ? sGuildMgr->GetGuildById(GetGuildId())->GetName() : "";
}

void Player::SendDuelCountdown(uint32 counter) {
  WorldPacket data(SMSG_DUEL_COUNTDOWN, 4);
  data << uint32(counter); // seconds
  SendDirectMessage(&data);
}

void Player::AddRefundReference(ObjectGuid it) { m_refundableItems.insert(it); }

void Player::DeleteRefundReference(ObjectGuid it) {
  GuidSet::iterator itr = m_refundableItems.find(it);
  if (itr != m_refundableItems.end())
    m_refundableItems.erase(itr);
}

void Player::SendRefundInfo(Item *item) {
  // This function call unsets ITEM_FLAGS_REFUNDABLE if played time is over 2
  // hours.
  item->UpdatePlayedTime(this);

  if (!item->HasFlag(ITEM_FIELD_FLAGS, ITEM_FIELD_FLAG_REFUNDABLE)) {
    LOG_DEBUG("entities.player.items", "Item refund: item not refundable!");
    return;
  }

  if (GetGUID().GetCounter() !=
      item->GetRefundRecipient()) // Formerly refundable item got traded
  {
    LOG_DEBUG("entities.player.items", "Item refund: item was traded!");
    item->SetNotRefundable(this);
    return;
  }

  ItemExtendedCostEntry const *iece =
      sItemExtendedCostStore.LookupEntry(item->GetPaidExtendedCost());
  if (!iece) {
    LOG_DEBUG("entities.player.items",
              "Item refund: cannot find extendedcost data.");
    return;
  }

  ObjectGuid guid = item->GetGUID();
  WorldPacket data(SMSG_ITEM_REFUND_INFO_RESPONSE,
                   8 + 4 + 4 + 4 + 4 * 4 + 4 * 4 + 4 + 4);
  data.WriteBit(guid[3]);
  data.WriteBit(guid[5]);
  data.WriteBit(guid[7]);
  data.WriteBit(guid[6]);
  data.WriteBit(guid[2]);
  data.WriteBit(guid[4]);
  data.WriteBit(guid[0]);
  data.WriteBit(guid[1]);
  data.FlushBits();

  data.WriteByteSeq(guid[7]);
  data << uint32(std::max<uint32>(
      0, 2 * HOUR * IN_MILLISECONDS -
             item->GetPlayedTime(this) * IN_MILLISECONDS)); // Refund Time Left
  for (uint8 i = 0; i < MAX_ITEM_EXT_COST_ITEMS; ++i)       // item cost data
  {
    data << uint32(iece->RequiredItemCount[i]);
    data << uint32(iece->RequiredItem[i]);
  }

  data.WriteByteSeq(guid[6]);
  data.WriteByteSeq(guid[4]);
  data.WriteByteSeq(guid[3]);
  data.WriteByteSeq(guid[2]);
  for (uint8 i = 0; i < MAX_ITEM_EXT_COST_CURRENCIES; ++i) // currency cost data
  {
    if (iece->RequirementFlags &
        (ITEM_EXT_COST_CURRENCY_REQ_IS_SEASON_EARNED_1 << i)) {
      data << uint32(0);
      data << uint32(0);
      continue;
    }

    CurrencyTypesEntry const *currencyType =
        sCurrencyTypesStore.LookupEntry(iece->RequiredCurrency[i]);
    uint32 precision =
        (currencyType && currencyType->Flags & CURRENCY_FLAG_HIGH_PRECISION)
            ? CURRENCY_PRECISION
            : 1;

    data << uint32(iece->RequiredCurrencyCount[i] / precision);
    data << uint32(iece->RequiredCurrency[i]);
  }

  data.WriteByteSeq(guid[1]);
  data.WriteByteSeq(guid[5]);
  data << uint32(0);
  data.WriteByteSeq(guid[0]);
  data << uint32(item->GetPaidMoney()); // money cost
  SendDirectMessage(&data);
}

bool Player::AddItem(uint32 itemId, uint32 count) {
  uint32 noSpaceForCount = 0;
  ItemPosCountVec dest;
  InventoryResult msg = CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, itemId,
                                        count, &noSpaceForCount);
  if (msg != EQUIP_ERR_OK)
    count -= noSpaceForCount;

  if (count == 0 || dest.empty()) {
    /// @todo Send to mailbox if no space
    ChatHandler(GetSession())
        .PSendSysMessage("You don't have any space in your bags.");
    return false;
  }

  Item *item =
      StoreNewItem(dest, itemId, true, GenerateItemRandomPropertyId(itemId));
  if (item)
    SendNewItem(item, count, true, false);
  else
    return false;
  return true;
}

void Player::SendItemRefundResult(Item *item, ItemExtendedCostEntry const *iece,
                                  uint8 error) {
  ObjectGuid guid = item->GetGUID();
  WorldPacket data(SMSG_ITEM_REFUND_RESULT, 1 + 1 + 8 + 4 * 8 + 4 + 4 * 8 + 1);
  data.WriteBit(guid[4]);
  data.WriteBit(guid[5]);
  data.WriteBit(guid[1]);
  data.WriteBit(guid[6]);
  data.WriteBit(guid[7]);
  data.WriteBit(guid[0]);
  data.WriteBit(guid[3]);
  data.WriteBit(guid[2]);
  data.WriteBit(!error);
  data.WriteBit(item->GetPaidMoney() > 0);
  data.FlushBits();
  if (!error) {
    for (uint8 i = 0; i < MAX_ITEM_EXT_COST_CURRENCIES; ++i) {
      if (iece->RequirementFlags &
          (ITEM_EXT_COST_CURRENCY_REQ_IS_SEASON_EARNED_1 << i)) {
        data << uint32(0);
        data << uint32(0);
        continue;
      }

      CurrencyTypesEntry const *currencyType =
          sCurrencyTypesStore.LookupEntry(iece->RequiredCurrency[i]);
      uint32 precision =
          (currencyType && currencyType->Flags & CURRENCY_FLAG_HIGH_PRECISION)
              ? CURRENCY_PRECISION
              : 1;

      data << uint32(iece->RequiredCurrencyCount[i] / precision);
      data << uint32(iece->RequiredCurrency[i]);
    }

    data << uint32(item->GetPaidMoney()); // money cost

    for (uint8 i = 0; i < MAX_ITEM_EXT_COST_ITEMS; ++i) // item cost data
    {
      data << uint32(iece->RequiredItemCount[i]);
      data << uint32(iece->RequiredItem[i]);
    }
  }

  data.WriteByteSeq(guid[0]);
  data.WriteByteSeq(guid[3]);
  data.WriteByteSeq(guid[1]);
  data.WriteByteSeq(guid[6]);
  data.WriteByteSeq(guid[4]);
  data.WriteByteSeq(guid[2]);
  data.WriteByteSeq(guid[7]);
  data.WriteByteSeq(guid[5]);

  data << uint8(error); // error code
  SendDirectMessage(&data);
}

void Player::RefundItem(Item *item) {
  if (!item->HasFlag(ITEM_FIELD_FLAGS, ITEM_FIELD_FLAG_REFUNDABLE)) {
    LOG_DEBUG("entities.player.items", "Item refund: item not refundable!");
    return;
  }

  if (item->IsRefundExpired(this)) // item refund has expired
  {
    item->SetNotRefundable(this);
    SendItemRefundResult(item, nullptr, 10);
    return;
  }

  if (GetGUID().GetCounter() !=
      item->GetRefundRecipient()) // Formerly refundable item got traded
  {
    LOG_DEBUG("entities.player.items", "Item refund: item was traded!");
    item->SetNotRefundable(this);
    return;
  }

  ItemExtendedCostEntry const *iece =
      sItemExtendedCostStore.LookupEntry(item->GetPaidExtendedCost());
  if (!iece) {
    LOG_DEBUG("entities.player.items",
              "Item refund: cannot find extendedcost data.");
    return;
  }

  bool store_error = false;
  for (uint8 i = 0; i < MAX_ITEM_EXT_COST_ITEMS; ++i) {
    uint32 count = iece->RequiredItemCount[i];
    uint32 itemid = iece->RequiredItem[i];

    if (count && itemid) {
      ItemPosCountVec dest;
      InventoryResult msg =
          CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, itemid, count);
      if (msg != EQUIP_ERR_OK) {
        store_error = true;
        break;
      }
    }
  }

  if (store_error) {
    SendItemRefundResult(item, iece, 10);
    return;
  }

  SendItemRefundResult(item, iece, 0);

  uint32 moneyRefund =
      item->GetPaidMoney(); // item-> will be invalidated in DestroyItem

  // Save all relevant data to DB to prevent desynchronisation exploits
  CharacterDatabaseTransaction trans = CharacterDatabase.BeginTransaction();

  // Delete any references to the refund data
  item->SetNotRefundable(this, true, &trans);

  // Destroy item
  DestroyItem(item->GetBagSlot(), item->GetSlot(), true);

  // Grant back extendedcost items
  for (uint8 i = 0; i < MAX_ITEM_EXT_COST_ITEMS; ++i) {
    uint32 count = iece->RequiredItemCount[i];
    uint32 itemid = iece->RequiredItem[i];
    if (count && itemid) {
      ItemPosCountVec dest;
      InventoryResult msg =
          CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, itemid, count);
      ASSERT(msg == EQUIP_ERR_OK); /// Already checked before
      Item *it = StoreNewItem(dest, itemid, true);
      SendNewItem(it, count, true, false, true);
    }
  }

  // Grant back currencies
  for (uint8 i = 0; i < MAX_ITEM_EXT_COST_CURRENCIES; ++i) {
    if (iece->RequirementFlags &
        (ITEM_EXT_COST_CURRENCY_REQ_IS_SEASON_EARNED_1 << i))
      continue;

    uint32 count = iece->RequiredCurrencyCount[i];
    uint32 currencyid = iece->RequiredCurrency[i];
    if (iece->RequirementFlags &
        (ITEM_EXT_COST_CURRENCY_REQ_IS_SEASON_EARNED_1 << i))
      continue;

    if (count && currencyid)
      ModifyCurrency(currencyid, count, false, false, true);
  }

  // Grant back money
  if (moneyRefund)
    ModifyMoney(int64(moneyRefund)); // Saved in SaveInventoryAndGoldToDB

  SaveInventoryAndGoldToDB(trans);

  CharacterDatabase.CommitTransaction(trans);
}

void Player::SendItemRetrievalMail(uint32 itemEntry, uint32 count) {
  MailSender sender(MAIL_CREATURE, 34337 /* The Postmaster */);
  MailDraft draft(
      "Recovered Item",
      "We recovered a lost item in the twisting nether and noted that it was "
      "yours.$B$BPlease find said object enclosed."); // This is the text used
                                                      // in Cataclysm, it
                                                      // probably wasn't
                                                      // changed.
  CharacterDatabaseTransaction trans = CharacterDatabase.BeginTransaction();

  if (Item *item = Item::CreateItem(itemEntry, count, nullptr)) {
    item->SaveToDB(trans);
    draft.AddItem(item);
  }

  draft.SendMailTo(trans, MailReceiver(this, GetGUID().GetCounter()), sender);
  CharacterDatabase.CommitTransaction(trans);
}

void Player::SetRandomWinner(bool isWinner) {
  m_IsBGRandomWinner = isWinner;
  if (m_IsBGRandomWinner) {
    CharacterDatabasePreparedStatement *stmt =
        CharacterDatabase.GetPreparedStatement(CHAR_INS_BATTLEGROUND_RANDOM);

    stmt->SetData(0, GetGUID().GetCounter());

    CharacterDatabase.Execute(stmt);
  }
}

void Player::_LoadRandomBGStatus(PreparedQueryResult result) {
  // QueryResult result = CharacterDatabase.PQuery("SELECT guid FROM
  // character_battleground_random WHERE guid = '%u'", GetGUID().GetCounter());

  if (result)
    m_IsBGRandomWinner = true;
}

float Player::GetAverageItemLevel() const {
  auto getEffectiveItemLevel = [](uint32 itemLevel, uint32 *pValue,
                                  uint32 *pMinValue) -> void {
    if (pMinValue != nullptr) {
      if (itemLevel <= *pValue) {
        if (itemLevel >= *pMinValue)
          *pMinValue = itemLevel;
      } else {
        *pMinValue = *pValue;
        *pValue = itemLevel;
      }
    } else {
      if (*pValue > itemLevel)
        itemLevel = *pValue;

      *pValue = itemLevel;
    }
  };

  uint32 itemLevelPerInvType[MAX_INVTYPE] = {0};

  uint32 fingerItemLevel = 0;
  uint32 trinketItemLevel = 0;
  uint32 weaponItemLevel = 0;
  uint32 weapon2HItemLevel = 0;

  for (uint8 i = EQUIPMENT_SLOT_START; i < EQUIPMENT_SLOT_END; ++i) {
    const ItemTemplate *itemTemplate =
        m_items[i] == nullptr ? nullptr : m_items[i]->GetTemplate();
    if (itemTemplate == nullptr)
      continue;

    uint32 *pEffective = nullptr;
    switch (itemTemplate->GetInventoryType()) {
    case INVTYPE_FINGER:
      pEffective = &fingerItemLevel;
      break;
    case INVTYPE_TRINKET:
      pEffective = &trinketItemLevel;
      break;
    case INVTYPE_WEAPON:
      pEffective = &weaponItemLevel;
      break;
    case INVTYPE_2HWEAPON:
      pEffective = &weapon2HItemLevel;
      break;
    }

    getEffectiveItemLevel(
        static_cast<uint32>(itemTemplate->GetBaseItemLevel()),
        &itemLevelPerInvType[itemTemplate->GetInventoryType()], pEffective);
  }

  uint32 maxRangedThrownRelicRangedRight =
      std::max({itemLevelPerInvType[INVTYPE_RANGED],
                itemLevelPerInvType[INVTYPE_RANGEDRIGHT],
                itemLevelPerInvType[INVTYPE_RELIC],
                itemLevelPerInvType[INVTYPE_THROWN]});

  uint32 offHandItemLevel = 0;
  uint32 mainHandItemLevel = 0;

  if (getClass() == CLASS_WARRIOR || getClass() == CLASS_ROGUE ||
      getClass() == CLASS_SHAMAN || getClass() == CLASS_HUNTER ||
      getClass() == CLASS_DEATH_KNIGHT) {
    uint8 const weaponSorterSize = 6;
    std::array<uint32, weaponSorterSize> weaponSorter;
    weaponSorter[0] = itemLevelPerInvType[INVTYPE_WEAPONMAINHAND];

    weaponSorter[1] = std::max({itemLevelPerInvType[INVTYPE_SHIELD],
                                itemLevelPerInvType[INVTYPE_WEAPONOFFHAND],
                                itemLevelPerInvType[INVTYPE_HOLDABLE]});

    weaponSorter[2] = itemLevelPerInvType[INVTYPE_WEAPON];
    weaponSorter[3] = weaponItemLevel;

    if (getClass() == CLASS_WARRIOR) {
      weaponSorter[4] = itemLevelPerInvType[INVTYPE_2HWEAPON];
      weaponSorter[5] = weapon2HItemLevel;
    } else {
      weaponSorter[4] = 0;
      weaponSorter[5] = 0;
    }

    std::sort(weaponSorter.begin(), weaponSorter.end(),
              [](uint32 lhs, uint32 rhs) -> bool { return rhs < lhs; });

    offHandItemLevel = weaponSorter[1];
    mainHandItemLevel = weaponSorter[0];
  } else {
    mainHandItemLevel = std::max(itemLevelPerInvType[INVTYPE_WEAPON],
                                 itemLevelPerInvType[INVTYPE_WEAPONMAINHAND]);
    offHandItemLevel = std::max(itemLevelPerInvType[INVTYPE_SHIELD],
                                itemLevelPerInvType[INVTYPE_HOLDABLE]);
  }

  uint32 twoHandedWeapon = itemLevelPerInvType[INVTYPE_2HWEAPON];
  itemLevelPerInvType[INVTYPE_NON_EQUIP] = 0;
  itemLevelPerInvType[INVTYPE_BODY] = 0;
  itemLevelPerInvType[INVTYPE_RANGED] = 0;
  itemLevelPerInvType[INVTYPE_THROWN] = 0;
  itemLevelPerInvType[INVTYPE_AMMO] = 0;
  itemLevelPerInvType[INVTYPE_RANGEDRIGHT] = 0;
  itemLevelPerInvType[INVTYPE_RELIC] = 0;
  itemLevelPerInvType[INVTYPE_2HWEAPON] = 0;
  itemLevelPerInvType[INVTYPE_WEAPON] = 0;
  itemLevelPerInvType[INVTYPE_SHIELD] = 0;
  itemLevelPerInvType[INVTYPE_WEAPONMAINHAND] = 0;
  itemLevelPerInvType[INVTYPE_WEAPONOFFHAND] = 0;
  itemLevelPerInvType[INVTYPE_HOLDABLE] = 0;
  itemLevelPerInvType[INVTYPE_BAG] = 0;
  itemLevelPerInvType[INVTYPE_TABARD] = 0;
  itemLevelPerInvType[INVTYPE_CHEST] = std::max(
      itemLevelPerInvType[INVTYPE_CHEST], itemLevelPerInvType[INVTYPE_ROBE]);
  itemLevelPerInvType[INVTYPE_ROBE] = 0;

  uint32 itemLevelAccumulated = 0;
  for (uint8 i = 0; i < MAX_INVTYPE; ++i)
    itemLevelAccumulated += itemLevelPerInvType[i];

  itemLevelAccumulated +=
      fingerItemLevel + trinketItemLevel + maxRangedThrownRelicRangedRight;
  uint32 finalSum = mainHandItemLevel + itemLevelAccumulated + offHandItemLevel;
  float oneHandingScore = finalSum / 17.0f; // One handing
  float twoHandingScore =
      (twoHandedWeapon + itemLevelAccumulated) / 16.0f; // two handing

  if (oneHandingScore <= twoHandingScore)
    return twoHandingScore;

  return oneHandingScore;
}

void Player::_LoadInstanceTimeRestrictions(PreparedQueryResult result) {
  if (!result)
    return;

  do {
    Field *fields = result->Fetch();
    _instanceResetTimes.insert(InstanceTimeMap::value_type(
        fields[0].Get<uint32>(), fields[1].Get<uint64>()));
  } while (result->NextRow());
}

void Player::_SaveInstanceTimeRestrictions(
    CharacterDatabaseTransaction &trans) {
  if (_instanceResetTimes.empty())
    return;

  CharacterDatabasePreparedStatement *stmt =
      CharacterDatabase.GetPreparedStatement(
          CHAR_DEL_ACCOUNT_INSTANCE_LOCK_TIMES);
  stmt->SetData(0, GetSession()->GetAccountId());
  trans->Append(stmt);

  for (InstanceTimeMap::const_iterator itr = _instanceResetTimes.begin();
       itr != _instanceResetTimes.end(); ++itr) {
    stmt = CharacterDatabase.GetPreparedStatement(
        CHAR_INS_ACCOUNT_INSTANCE_LOCK_TIMES);
    stmt->SetData(0, GetSession()->GetAccountId());
    stmt->SetData(1, itr->first);
    stmt->SetData(2, uint32(itr->second));
    trans->Append(stmt);
  }
}

bool Player::IsInWhisperWhiteList(ObjectGuid guid) {
  for (GuidList::const_iterator itr = WhisperList.begin();
       itr != WhisperList.end(); ++itr)
    if (*itr == guid)
      return true;

  return false;
}

void Player::UpdateArmorSpecialization() {
  // Remove Auras  with SPELL_ATTR8_ARMOR_SPECIALIZATION
  for (AuraApplicationMap::iterator itr = m_appliedAuras.begin();
       itr != m_appliedAuras.end();) {
    SpellInfo const *spell = itr->second->GetBase()->GetSpellInfo();
    if (spell->AttributesEx8 & SPELL_ATTR8_ARMOR_SPECIALIZATION)
      RemoveAura(itr);
    else
      ++itr;
  }

  for (uint8 id = 0; id < MAX_ARMOR_SPECIALIZATION_IDS; id++) {
    if (HasSpell(ArmorSpecializationIds[id])) {
      CastSpell(this, ArmorSpecializationIds[id], TRIGGERED_FULL_MASK);
      break;
    }
  }
}

uint8 Player::GetNextVoidStorageFreeSlot() const {
  for (uint8 i = 0; i < VOID_STORAGE_MAX_SLOT; ++i)
    if (!_voidStorageItems[i]) // unused item
      return i;

  return VOID_STORAGE_MAX_SLOT;
}

uint8 Player::GetNumOfVoidStorageFreeSlots() const {
  uint8 count = 0;

  for (uint8 i = 0; i < VOID_STORAGE_MAX_SLOT; ++i)
    if (!_voidStorageItems[i])
      count++;

  return count;
}

uint8 Player::AddVoidStorageItem(VoidStorageItem const &item) {
  int8 slot = GetNextVoidStorageFreeSlot();

  if (slot >= VOID_STORAGE_MAX_SLOT) {
    GetSession()->SendVoidStorageTransferResult(VOID_TRANSFER_ERROR_FULL);
    return 255;
  }

  _voidStorageItems[slot] =
      new VoidStorageItem(item.ItemId, item.ItemEntry, item.CreatorGuid,
                          item.ItemRandomPropertyId, item.ItemSuffixFactor);
  return slot;
}

void Player::AddVoidStorageItemAtSlot(uint8 slot, const VoidStorageItem &item) {
  if (slot >= VOID_STORAGE_MAX_SLOT) {
    GetSession()->SendVoidStorageTransferResult(VOID_TRANSFER_ERROR_FULL);
    return;
  }

  if (_voidStorageItems[slot]) {
    LOG_ERROR("misc",
              "Player::AddVoidStorageItemAtSlot - Player (GUID: %u, name: %s) "
              "tried to add an item to an used slot (item id: " UI64FMTD
              ", entry: %u, slot: %u).",
              GetGUID().GetCounter(), GetName().c_str(),
              _voidStorageItems[slot]->ItemId,
              _voidStorageItems[slot]->ItemEntry, slot);
    GetSession()->SendVoidStorageTransferResult(
        VOID_TRANSFER_ERROR_INTERNAL_ERROR_1);
    return;
  }

  _voidStorageItems[slot] =
      new VoidStorageItem(item.ItemId, item.ItemId, item.CreatorGuid,
                          item.ItemRandomPropertyId, item.ItemSuffixFactor);
}

void Player::DeleteVoidStorageItem(uint8 slot) {
  if (slot >= VOID_STORAGE_MAX_SLOT) {
    GetSession()->SendVoidStorageTransferResult(
        VOID_TRANSFER_ERROR_INTERNAL_ERROR_1);
    return;
  }

  delete _voidStorageItems[slot];
  _voidStorageItems[slot] = nullptr;
}

bool Player::SwapVoidStorageItem(uint8 oldSlot, uint8 newSlot) {
  if (oldSlot >= VOID_STORAGE_MAX_SLOT || newSlot >= VOID_STORAGE_MAX_SLOT ||
      oldSlot == newSlot)
    return false;

  std::swap(_voidStorageItems[newSlot], _voidStorageItems[oldSlot]);
  return true;
}

VoidStorageItem *Player::GetVoidStorageItem(uint8 slot) const {
  if (slot >= VOID_STORAGE_MAX_SLOT) {
    GetSession()->SendVoidStorageTransferResult(
        VOID_TRANSFER_ERROR_INTERNAL_ERROR_1);
    return nullptr;
  }

  return _voidStorageItems[slot];
}

VoidStorageItem *Player::GetVoidStorageItem(uint64 id, uint8 &slot) const {
  for (uint8 i = 0; i < VOID_STORAGE_MAX_SLOT; ++i) {
    if (_voidStorageItems[i] && _voidStorageItems[i]->ItemId == id) {
      slot = i;
      return _voidStorageItems[i];
    }
  }

  return nullptr;
}

std::string Player::GetMapAreaAndZoneString() const {
  uint32 areaId = GetAreaId();
  std::string areaName = "Unknown";
  std::string zoneName = "Unknown";
  if (AreaTableEntry const *area = sAreaTableStore.LookupEntry(areaId)) {
    int locale = GetSession()->GetSessionDbcLocale();
    areaName = area->AreaName[locale];
    if (AreaTableEntry const *zone =
            sAreaTableStore.LookupEntry(area->ParentAreaID))
      zoneName = zone->AreaName[locale];
  }

  std::ostringstream str;
  str << "Map: " << GetMapId() << " ("
      << (FindMap() ? FindMap()->GetMapName() : "Unknown")
      << ") Area: " << areaId << " (" << areaName.c_str()
      << ") Zone: " << zoneName.c_str();
  return str.str();
}

std::string Player::GetCoordsMapAreaAndZoneString() const {
  std::ostringstream str;
  str << Position::ToString() << " " << GetMapAreaAndZoneString();
  return str.str();
}

Guild *Player::GetGuild() {
  uint32 guildId = GetGuildId();
  return guildId ? sGuildMgr->GetGuildById(guildId) : nullptr;
}

Pet *Player::SummonPet(uint32 entry, float x, float y, float z, float ang,
                       PetType petType, uint32 duration) {
  Pet *pet = new Pet(this, petType);

  pet->Relocate(x, y, z, ang);
  if (!pet->IsPositionValid()) {
    LOG_ERROR("misc",
              "Player::SummonPet: Pet (%s, Entry: %d) not summoned. Suggested "
              "coordinates aren't valid (X: %f Y: %f)",
              pet->GetGUID().ToString().c_str(), pet->GetEntry(),
              pet->GetPositionX(), pet->GetPositionY());
    delete pet;
    return nullptr;
  }

  bool hasPetData = pet->LoadPetData(this, entry);

  if (!hasPetData && petType == HUNTER_PET) {
    delete pet;
    return nullptr;
  }

  if (!hasPetData) {
    Map *map = GetMap();
    uint32 pet_number = sObjectMgr->GeneratePetNumber();
    if (!pet->Create(map->GenerateLowGuid<HighGuid::Pet>(), map, entry,
                     pet_number)) {
      LOG_ERROR("misc", "Player::SummonPet: No such creature entry %u", entry);
      delete pet;
      return nullptr;
    }

    // generate new name for summon pet
    std::string new_name = sObjectMgr->GeneratePetName(entry);
    if (!new_name.empty())
      pet->SetName(new_name);

    pet->SetCreatorGUID(GetGUID());
    pet->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, GetFaction());

    pet->SetUInt64Value(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_NONE);
    pet->SetUInt32Value(UNIT_FIELD_BYTES_1, 0);
    pet->InitStatsForLevel(getLevel());
    pet->SetReactState(REACT_ASSIST);

    SetMinion(pet, true);

    switch (petType) {
    case SUMMON_PET:
      pet->GetCharmInfo()->SetPetNumber(pet_number, true);
      pet->SetByteValue(UNIT_FIELD_BYTES_0, UNIT_BYTES_0_OFFSET_CLASS,
                        CLASS_MAGE);
      pet->SetUInt32Value(UNIT_FIELD_PETEXPERIENCE, 0);
      pet->SetUInt32Value(UNIT_FIELD_PETNEXTLEVELEXP, 1000);
      pet->SetUInt32Value(
          UNIT_FIELD_PET_NAME_TIMESTAMP,
          uint32(GameTime::GetGameTime())); // cast can't be helped in this case
      break;
    default:
      break;
    }

    map->AddToMap(pet->ToCreature());

    switch (petType) {
    case SUMMON_PET:
      pet->InitPetCreateSpells();
      pet->SavePetToDB(PET_SAVE_NEW_PET);
      PetSpellInitialize();
      GetSession()->SendPetAdded(
          pet->GetSlot(), pet->GetCharmInfo()->GetPetNumber(), pet->GetEntry(),
          pet->getLevel(), pet->GetName());
      break;
    default:
      break;
    }

    // Update all stats after we have applied pet scaling auras to make sure we
    // have all fields initialized properly
    pet->UpdateAllStats();
    pet->SetFullHealth();
    pet->SetPower(POWER_MANA, pet->GetMaxPower(POWER_MANA));

    if (pet->IsHunterPet())
      pet->CastSpell(pet, SPELL_PET_ENERGIZE, true);
    else if (pet->IsPetGhoul()) {
      pet->CastSpell(pet, SPELL_PET_RISEN_GHOUL_SPAWN_IN, true);
      pet->CastSpell(pet, SPELL_PET_RISEN_GHOUL_SELF_STUN, true);
    }
  }

  PhasingHandler::InheritPhaseShift(pet, this);

  if (duration > 0)
    pet->SetDuration(duration);

  // ObjectAccessor::UpdateObjectVisibility(pet);

  return pet;
}

bool Player::CanUseMastery() const {
  return HasSpell(MasterySpells[getClass()]);
}

void Player::ValidateMovementInfo(MovementInfo *mi) {
  //! Anti-cheat checks. Please keep them in seperate if () blocks to maintain a
  //! clear overview. Might be subject to latency, so just remove improper
  //! flags.
#ifdef FIRELANDS_DEBUG
#define REMOVE_VIOLATING_FLAGS(check, maskToRemove)                            \
  {                                                                            \
    if (check) {                                                               \
      LOG_DEBUG("entities.unit",                                               \
                "Player::ValidateMovementInfo: Violation of MovementFlags "    \
                "found (%s). "                                                 \
                "MovementFlags: %u, MovementFlags2: %u for player %s. Mask "   \
                "%u will be removed.",                                         \
                STRINGIZE(check), mi->GetMovementFlags(),                      \
                          mi->GetExtraMovementFlags(),                         \
                          GetGUID().ToString().c_str(), maskToRemove);         \
      mi->RemoveMovementFlag((maskToRemove));                                  \
    }                                                                          \
  }
#else
#define REMOVE_VIOLATING_FLAGS(check, maskToRemove)                            \
  if (check)                                                                   \
    mi->RemoveMovementFlag((maskToRemove));
#endif

  if (mi->guid.IsEmpty()) {
    LOG_ERROR("entities.unit",
              "WorldSession::ReadMovementInfo: mi->guid is empty");
    return;
  }

  Unit *mover = GetCharmedOrSelf()->GetGUID() == mi->guid
                    ? GetCharmedOrSelf()
                    : ObjectAccessor::GetUnit(*GetCharmedOrSelf(), mi->guid);
  if (!mover) {
    LOG_ERROR("entities.unit",
              "WorldSession::ReadMovementInfo: If the server allows the unit "
              "(GUID %s) to be moved by the client of player %s, the unit "
              "should still exist!",
              mi->guid.ToString().c_str(),
              GetCharmedOrSelf()->GetGUID().ToString().c_str());
    return;
  }

  if (!GetCharmedOrSelf()->GetVehicleBase() ||
      !(GetCharmedOrSelf()->GetVehicle()->GetVehicleInfo()->Flags &
        VEHICLE_FLAG_FIXED_POSITION))
    REMOVE_VIOLATING_FLAGS(mi->HasMovementFlag(MOVEMENTFLAG_ROOT),
                           MOVEMENTFLAG_ROOT);

  /*! This must be a packet spoofing attempt. MOVEMENTFLAG_ROOT sent from the
     client is not valid in conjunction with any of the moving movement flags
     such as MOVEMENTFLAG_FORWARD. It will freeze clients that receive this
     player's movement info.
  */
  REMOVE_VIOLATING_FLAGS(mi->HasMovementFlag(MOVEMENTFLAG_ROOT) &&
                             mi->HasMovementFlag(MOVEMENTFLAG_MASK_MOVING),
                         MOVEMENTFLAG_MASK_MOVING);

  //! Cannot hover without SPELL_AURA_HOVER
  REMOVE_VIOLATING_FLAGS(mi->HasMovementFlag(MOVEMENTFLAG_HOVER) &&
                             !mover->HasAuraType(SPELL_AURA_HOVER),
                         MOVEMENTFLAG_HOVER);

  //! Cannot ascend and descend at the same time
  REMOVE_VIOLATING_FLAGS(mi->HasMovementFlag(MOVEMENTFLAG_ASCENDING) &&
                             mi->HasMovementFlag(MOVEMENTFLAG_DESCENDING),
                         MOVEMENTFLAG_ASCENDING | MOVEMENTFLAG_DESCENDING);

  //! Cannot move left and right at the same time
  REMOVE_VIOLATING_FLAGS(mi->HasMovementFlag(MOVEMENTFLAG_LEFT) &&
                             mi->HasMovementFlag(MOVEMENTFLAG_RIGHT),
                         MOVEMENTFLAG_LEFT | MOVEMENTFLAG_RIGHT);

  //! Cannot strafe left and right at the same time
  REMOVE_VIOLATING_FLAGS(mi->HasMovementFlag(MOVEMENTFLAG_STRAFE_LEFT) &&
                             mi->HasMovementFlag(MOVEMENTFLAG_STRAFE_RIGHT),
                         MOVEMENTFLAG_STRAFE_LEFT | MOVEMENTFLAG_STRAFE_RIGHT);

  //! Cannot pitch up and down at the same time
  REMOVE_VIOLATING_FLAGS(mi->HasMovementFlag(MOVEMENTFLAG_PITCH_UP) &&
                             mi->HasMovementFlag(MOVEMENTFLAG_PITCH_DOWN),
                         MOVEMENTFLAG_PITCH_UP | MOVEMENTFLAG_PITCH_DOWN);

  //! Cannot move forwards and backwards at the same time
  REMOVE_VIOLATING_FLAGS(mi->HasMovementFlag(MOVEMENTFLAG_FORWARD) &&
                             mi->HasMovementFlag(MOVEMENTFLAG_BACKWARD),
                         MOVEMENTFLAG_FORWARD | MOVEMENTFLAG_BACKWARD);

  //! Cannot walk on water without SPELL_AURA_WATER_WALK except for ghosts
  REMOVE_VIOLATING_FLAGS(mi->HasMovementFlag(MOVEMENTFLAG_WATERWALKING) &&
                             !mover->HasAuraType(SPELL_AURA_WATER_WALK) &&
                             !mover->HasAuraType(SPELL_AURA_GHOST),
                         MOVEMENTFLAG_WATERWALKING);

  //! Cannot feather fall without SPELL_AURA_FEATHER_FALL
  REMOVE_VIOLATING_FLAGS(
      mi->HasMovementFlag(MOVEMENTFLAG_FALLING_SLOW) &&
          !GetCharmedOrSelf()->HasAuraType(SPELL_AURA_FEATHER_FALL),
      MOVEMENTFLAG_FALLING_SLOW);

  /*! Cannot fly if no fly auras present. Exception is being a GM.
      Note that we check for account level instead of Player::IsGameMaster()
     because in some situations it may be feasable to use .gm fly on as a GM
     without having .gm on, e.g. aerial combat.
  */

  REMOVE_VIOLATING_FLAGS(
      mi->HasMovementFlag(MOVEMENTFLAG_FLYING | MOVEMENTFLAG_CAN_FLY) &&
          GetSession()->GetSecurity() == SEC_PLAYER &&
          !mover->HasAuraType(SPELL_AURA_FLY) &&
          !mover->HasAuraType(SPELL_AURA_MOD_INCREASE_MOUNTED_FLIGHT_SPEED),
      MOVEMENTFLAG_FLYING | MOVEMENTFLAG_CAN_FLY);

  REMOVE_VIOLATING_FLAGS(mi->HasMovementFlag(MOVEMENTFLAG_DISABLE_GRAVITY |
                                             MOVEMENTFLAG_CAN_FLY) &&
                             mi->HasMovementFlag(MOVEMENTFLAG_FALLING),
                         MOVEMENTFLAG_FALLING);

  REMOVE_VIOLATING_FLAGS(mi->HasMovementFlag(MOVEMENTFLAG_SPLINE_ELEVATION) &&
                             G3D::fuzzyEq(mi->splineElevation, 0.0f),
                         MOVEMENTFLAG_SPLINE_ELEVATION);

  // Client first checks if spline elevation != 0, then verifies flag presence
  if (G3D::fuzzyNe(mi->splineElevation, 0.0f))
    mi->AddMovementFlag(MOVEMENTFLAG_SPLINE_ELEVATION);

#undef REMOVE_VIOLATING_FLAGS
}

void Player::ReadMovementInfo(
    WorldPacket &data, MovementInfo *mi,
    Movement::ExtraMovementStatusElement *extras /*= nullptr*/) {
  MovementStatusElements const *sequence = GetMovementStatusElementsSequence(
      static_cast<OpcodeClient>(data.GetOpcode()));
  if (!sequence) {
    LOG_ERROR(
        "network",
        "Player::ReadMovementInfo: No movement sequence found for opcode %s",
        GetOpcodeNameForLogging(static_cast<OpcodeClient>(data.GetOpcode()))
            .c_str());
    return;
  }

  bool hasMovementFlags = false;
  bool hasMovementFlags2 = false;
  bool hasTimestamp = false;
  bool hasOrientation = false;
  bool hasTransportData = false;
  bool hasTransportTime2 = false;
  bool hasTransportVehicleId = false;
  bool hasPitch = false;
  bool hasFallData = false;
  bool hasFallDirection = false;
  bool hasSplineElevation = false;

  ObjectGuid guid;
  ObjectGuid tguid;

  for (; *sequence != MSEEnd; ++sequence) {
    MovementStatusElements const &element = *sequence;

    switch (element) {
    case MSEHasGuidByte0:
    case MSEHasGuidByte1:
    case MSEHasGuidByte2:
    case MSEHasGuidByte3:
    case MSEHasGuidByte4:
    case MSEHasGuidByte5:
    case MSEHasGuidByte6:
    case MSEHasGuidByte7:
      guid[element - MSEHasGuidByte0] = data.ReadBit();
      break;
    case MSEHasTransportGuidByte0:
    case MSEHasTransportGuidByte1:
    case MSEHasTransportGuidByte2:
    case MSEHasTransportGuidByte3:
    case MSEHasTransportGuidByte4:
    case MSEHasTransportGuidByte5:
    case MSEHasTransportGuidByte6:
    case MSEHasTransportGuidByte7:
      if (hasTransportData)
        tguid[element - MSEHasTransportGuidByte0] = data.ReadBit();
      break;
    case MSEGuidByte0:
    case MSEGuidByte1:
    case MSEGuidByte2:
    case MSEGuidByte3:
    case MSEGuidByte4:
    case MSEGuidByte5:
    case MSEGuidByte6:
    case MSEGuidByte7:
      data.ReadByteSeq(guid[element - MSEGuidByte0]);
      break;
    case MSETransportGuidByte0:
    case MSETransportGuidByte1:
    case MSETransportGuidByte2:
    case MSETransportGuidByte3:
    case MSETransportGuidByte4:
    case MSETransportGuidByte5:
    case MSETransportGuidByte6:
    case MSETransportGuidByte7:
      if (hasTransportData)
        data.ReadByteSeq(tguid[element - MSETransportGuidByte0]);
      break;
    case MSEHasMovementFlags:
      hasMovementFlags = !data.ReadBit();
      break;
    case MSEHasMovementFlags2:
      hasMovementFlags2 = !data.ReadBit();
      break;
    case MSEHasTimestamp:
      hasTimestamp = !data.ReadBit();
      break;
    case MSEHasOrientation:
      hasOrientation = !data.ReadBit();
      break;
    case MSEHasTransportData:
      hasTransportData = data.ReadBit();
      break;
    case MSEHasTransportTime2:
      if (hasTransportData)
        hasTransportTime2 = data.ReadBit();
      break;
    case MSEHasVehicleId:
      if (hasTransportData)
        hasTransportVehicleId = data.ReadBit();
      break;
    case MSEHasPitch:
      hasPitch = !data.ReadBit();
      break;
    case MSEHasFallData:
      hasFallData = data.ReadBit();
      break;
    case MSEHasFallDirection:
      if (hasFallData)
        hasFallDirection = data.ReadBit();
      break;
    case MSEHasSplineElevation:
      hasSplineElevation = !data.ReadBit();
      break;
    case MSEHasSpline:
      data.ReadBit();
      break;
    case MSEHasHeightChangeFailed:
      data.ReadBit();
      break;
    case MSEMovementFlags:
      if (hasMovementFlags)
        mi->flags = data.ReadBits(30);
      break;
    case MSEMovementFlags2:
      if (hasMovementFlags2)
        mi->flags2 = data.ReadBits(12);
      break;
    case MSETimestamp:
      if (hasTimestamp)
        data >> mi->time;
      break;
    case MSEPositionX:
      data >> mi->pos.m_positionX;
      break;
    case MSEPositionY:
      data >> mi->pos.m_positionY;
      break;
    case MSEPositionZ:
      data >> mi->pos.m_positionZ;
      break;
    case MSEOrientation:
      if (hasOrientation)
        mi->pos.SetOrientation(data.read<float>());
      break;
    case MSETransportPositionX:
      if (hasTransportData)
        data >> mi->transport.pos.m_positionX;
      break;
    case MSETransportPositionY:
      if (hasTransportData)
        data >> mi->transport.pos.m_positionY;
      break;
    case MSETransportPositionZ:
      if (hasTransportData)
        data >> mi->transport.pos.m_positionZ;
      break;
    case MSETransportOrientation:
      if (hasTransportData)
        mi->transport.pos.SetOrientation(data.read<float>());
      break;
    case MSETransportSeat:
      if (hasTransportData)
        data >> mi->transport.seat;
      break;
    case MSETransportTime:
      if (hasTransportData)
        data >> mi->transport.time;
      break;
    case MSETransportTime2:
      if (hasTransportData && hasTransportTime2)
        data >> mi->transport.time2;
      break;
    case MSETransportVehicleId:
      if (hasTransportData && hasTransportVehicleId)
        data >> mi->transport.vehicleId;
      break;
    case MSEPitch:
      if (hasPitch)
        mi->pitch = G3D::wrap(data.read<float>(), float(-M_PI), float(M_PI));
      break;
    case MSEFallTime:
      if (hasFallData)
        data >> mi->jump.fallTime;
      break;
    case MSEFallVerticalSpeed:
      if (hasFallData)
        data >> mi->jump.zspeed;
      break;
    case MSEFallCosAngle:
      if (hasFallData && hasFallDirection)
        data >> mi->jump.cosAngle;
      break;
    case MSEFallSinAngle:
      if (hasFallData && hasFallDirection)
        data >> mi->jump.sinAngle;
      break;
    case MSEFallHorizontalSpeed:
      if (hasFallData && hasFallDirection)
        data >> mi->jump.xyspeed;
      break;
    case MSESplineElevation:
      if (hasSplineElevation)
        data >> mi->splineElevation;
      break;
    case MSECounter:
      data >> mi->movementCounter;
      break;
    case MSEZeroBit:
    case MSEOneBit:
      data.ReadBit();
      break;
    case MSEExtraElement:
      extras->ReadNextElement(data);
      break;
    default:
      ASSERT(Movement::PrintInvalidSequenceElement(element, __FUNCTION__));
      break;
    }
  }

  mi->guid = guid;
  mi->transport.guid = tguid;

  ValidateMovementInfo(mi);
}

void Player::SendSupercededSpell(uint32 oldSpell, uint32 newSpell) const {
  WorldPackets::Spells::SupercededSpells packet;
  packet.SpellID = newSpell;
  packet.Superceded = oldSpell;
  SendDirectMessage(packet.Write());
}

bool Player::ValidateAppearance(uint8 race, uint8 class_, uint8 gender,
                                uint8 hairID, uint8 hairColor, uint8 faceID,
                                uint8 facialStyle, uint8 skinColor,
                                bool create /*=false*/) {
  // Check skin color
  // For Skin type is always 0
  if (CharSectionsEntry const *entry = sDBCManager.GetCharSectionEntry(
          race, SECTION_TYPE_SKIN, gender, 0,
          skinColor)) { // Skin Color defined as Face color, too, we check skin
                        // & face in one pass
    if (CharSectionsEntry const *entry2 = sDBCManager.GetCharSectionEntry(
            race, SECTION_TYPE_FACE, gender, faceID, skinColor)) {
      // Check DeathKnight exclusive
      if (((entry->Flags & SECTION_FLAG_DEATH_KNIGHT) ||
           (entry2->Flags & SECTION_FLAG_DEATH_KNIGHT)) &&
          class_ != CLASS_DEATH_KNIGHT)
        return false;
      if (create && !((entry->Flags & SECTION_FLAG_PLAYER) &&
                      (entry2->Flags & SECTION_FLAG_PLAYER)))
        return false;
    } else
      return false;
  } else
    return false;

  // These combinations don't have an entry of Type SECTION_TYPE_FACIAL_HAIR,
  // exclude them from that check
  bool excludeCheck = (race == RACE_TAUREN) || (race == RACE_DRAENEI) ||
                      (gender == GENDER_FEMALE && race != RACE_NIGHTELF &&
                       race != RACE_UNDEAD_PLAYER);

  // Check Hair
  if (CharSectionsEntry const *entry = sDBCManager.GetCharSectionEntry(
          race, SECTION_TYPE_HAIR, gender, hairID, hairColor)) {
    if ((entry->Flags & SECTION_FLAG_DEATH_KNIGHT) &&
        class_ != CLASS_DEATH_KNIGHT)
      return false;
    if (create && !(entry->Flags & SECTION_FLAG_PLAYER))
      return false;

    if (!excludeCheck) {
      if (CharSectionsEntry const *entry2 = sDBCManager.GetCharSectionEntry(
              race, SECTION_TYPE_FACIAL_HAIR, gender, facialStyle, hairColor)) {
        if ((entry2->Flags & SECTION_FLAG_DEATH_KNIGHT) &&
            class_ != CLASS_DEATH_KNIGHT)
          return false;
        if (create && !(entry2->Flags & SECTION_FLAG_PLAYER))
          return false;
      } else
        return false;
    } else {
      // @TODO: Bound checking for facialStyle ID (used clientside for markings,
      // tauren beard, etc.) Not present in DBC
    }
  } else
    return false;

  return true;
}

struct CategoryCooldownInfo {
  CategoryCooldownInfo(uint32 category, int32 cooldown)
      : Category(category), ModCooldown(cooldown) {}

  uint32 Category = 0;   ///< SpellCategory Id
  int32 ModCooldown = 0; ///< Reduced Cooldown in ms
};

void Player::SendSpellCategoryCooldowns() const {
  WorldPackets::Spells::CategoryCooldown cooldowns;

  Unit::AuraEffectList const &categoryCooldownAuras =
      GetAuraEffectsByType(SPELL_AURA_MOD_SPELL_CATEGORY_COOLDOWN);
  for (AuraEffect *aurEff : categoryCooldownAuras) {
    uint32 categoryId = aurEff->GetMiscValue();
    auto cItr = std::find_if(
        cooldowns.CategoryCooldowns.begin(), cooldowns.CategoryCooldowns.end(),
        [categoryId](
            WorldPackets::Spells::CategoryCooldown::CategoryCooldownInfo const
                &cooldown) { return cooldown.Category == categoryId; });

    if (cItr == cooldowns.CategoryCooldowns.end())
      cooldowns.CategoryCooldowns.emplace_back(categoryId,
                                               -aurEff->GetAmount());
    else
      cItr->ModCooldown -= aurEff->GetAmount();
  }

  SendDirectMessage(cooldowns.Write());
}

void Player::SendRaidGroupOnlyMessage(RaidGroupReason reason,
                                      int32 delay) const {
  WorldPackets::Instance::RaidGroupOnly raidGroupOnly;
  raidGroupOnly.Delay = delay;
  raidGroupOnly.Reason = reason;

  SendDirectMessage(raidGroupOnly.Write());
}

void Player::SetActiveSpec(uint8 spec) {
  _talentMgr->ActiveSpec = spec;
  UpdateArmorSpecialization();
  RemoveAurasWithInterruptFlags(SpellAuraInterruptFlags2::ChangeSpec);
}

void Player::SetRestFlag(RestFlag restFlag, uint32 triggerId /*= 0*/) {
  uint32 oldRestMask = _restFlagMask;
  _restFlagMask |= restFlag;

  if (!oldRestMask &&
      _restFlagMask) // only set flag/time on the first rest state
  {
    _restTime = GameTime::GetGameTime();
    SetFlag(PLAYER_FLAGS, PLAYER_FLAGS_RESTING);
  }

  if (triggerId)
    inn_triggerId = triggerId;
}

void Player::RemoveRestFlag(RestFlag restFlag) {
  uint32 oldRestMask = _restFlagMask;
  _restFlagMask &= ~restFlag;

  if (oldRestMask &&
      !_restFlagMask) // only remove flag/time on the last rest state remove
  {
    _restTime = 0;
    RemoveFlag(PLAYER_FLAGS, PLAYER_FLAGS_RESTING);
  }
}

uint32 Player::DoRandomRoll(uint32 minimum, uint32 maximum) {
  ASSERT(maximum <= 10000);

  uint32 roll = urand(minimum, maximum);

  WorldPacket data(MSG_RANDOM_ROLL, 4 + 4 + 4 + 8);
  data << uint32(minimum);
  data << uint32(maximum);
  data << uint32(roll);
  data << GetGUID();
  if (Group *group = GetGroup())
    group->BroadcastPacket(&data, false);
  else
    SendDirectMessage(&data);

  return roll;
}

void Player::RemoveSocial() {
  sSocialMgr->RemovePlayerSocial(GetGUID());
  m_social = nullptr;
}

void Player::SendTamePetFailure(PetTameFailureReason reason) {
  WorldPacket data(SMSG_PET_TAME_FAILURE, 1);
  data << uint8(reason);
  SendDirectMessage(&data);
}

GameClient *Player::GetGameClient() const {
  return GetSession()->GetGameClient();
}
