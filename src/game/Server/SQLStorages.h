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

#ifndef FIRELANDS_SQLSTORAGES_H
#define FIRELANDS_SQLSTORAGES_H

#include "Common.h"
#include "Database/SQLStorage.h"

extern SQLStorage sCreatureStorage;
extern SQLStorage sCreatureDataAddonStorage;
extern SQLStorage sCreatureInfoAddonStorage;
extern SQLStorage sCreatureModelStorage;
extern SQLStorage sEquipmentStorage;
extern SQLStorage sPageTextStore;
extern SQLStorage sItemStorage;
extern SQLStorage sInstanceTemplate;
extern SQLStorage sWorldTemplate;
extern SQLStorage sConditionStorage;

extern SQLHashStorage sSpellTemplate;
extern SQLHashStorage sGOStorage;
extern SQLHashStorage sGameObjectDataAddonStorage;
extern SQLHashStorage sCreatureTemplateSpellsStorage;

extern SQLMultiStorage sVehicleAccessoryStorage;
extern SQLMultiStorage sSpellScriptTargetStorage;

#endif
