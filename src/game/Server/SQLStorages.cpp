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

#include "SQLStorages.h"

// The meaning of the format strings:
//
// taken from DBCFileLoader.h
//
// enum FieldFormat
// {
//     DBC_FF_NA = 'x',                                        // ignore/ default, 4 byte size, in Source String means field is ignored, in Dest String means field is filled with default value
//     DBC_FF_NA_BYTE = 'X',                                   // ignore/ default, 1 byte size, see above
//     DBC_FF_NA_FLOAT = 'F',                                  // ignore/ default,  float size, see above
//     DBC_FF_NA_POINTER = 'p',                                // fill default value into dest, pointer size, Use this only with static data (otherwise mem-leak)
//     DBC_FF_STRING = 's',                                    // char*
//     DBC_FF_FLOAT = 'f',                                     // float
//     DBC_FF_INT = 'i',                                       // uint32
//     DBC_FF_BYTE = 'b',                                      // uint8
//     DBC_FF_SORT = 'd',                                      // sorted by this field, field is not included
//     DBC_FF_IND = 'n',                                       // the same,but parsed to data
//     DBC_FF_LOGIC = 'l'                                      // Logical (boolean)
// };
//

//                                 1        10        20        30        40        50        60        70        80        90        100       110       120       130       140       150       160
const char CreatureInfosrcfmt[] = "isssiiiiiiiiiiifiiiiiiiiiiiffiiiffffffiiiiffffiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiis";
const char CreatureInfodstfmt[] = "isssiiiiiiiiiiifiiiiiiiiiiiffiiiffffffiiiiffffiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiis";
const char CreatureDataAddonInfofmt[] = "iiibbiis";
const char CreatureModelfmt[] = "iffbii";
const char CreatureInfoAddonInfofmt[] = "iiibbiis";
const char GameObjectInfoAddonInfofmt[] = "iffff";
const char EquipmentInfofmt[] = "iiii";
const char GameObjectInfosrcfmt[] = "iiissssiifiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii";
const char GameObjectInfodstfmt[] = "iiissssiifiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii";
//                                  1        10        20        30        40        50        60        70        80        90        100       110       120       130       140       150       160
const char ItemPrototypesrcfmt[] = "iiiisiiiiffiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiifiiifiiiiiifiiiiiifiiiiiifiiiiiifiiiisiiiiiiiiiiiiiiiiiiiiiiiifiiiiifiiiii";
const char ItemPrototypedstfmt[] = "iiiisiiiiffiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiifiiifiiiiiifiiiiiifiiiiiifiiiiiifiiiisiiiiiiiiiiiiiiiiiiiiiiiifiiiiifiiiii";
const char PageTextfmt[] = "isi";
const char InstanceTemplatesrcfmt[] = "iiii";
const char InstanceTemplatedstfmt[] = "iiii";
const char WorldTemplatesrcfmt[] = "is";
const char WorldTemplatedstfmt[] = "ii";
const char ConditionsSrcFmt[] = "iiiix";
const char ConditionsDstFmt[] = "iiiix";
const char SpellTemplatesrcfmt[] = "iiiiiiiiiiiiiiiix";
//                                  0         10        20        30        40        50        60        70        80        90        100       110       120       130       140       150       160     170       180  185
const char SpellTemplatedstfmt[] = "ixxxiiiixxxxxxxxxxxxxxxxxxxxxxxxiixxxxixxxxxxFxxxxxxxxxxxxxxxxxxxxxxixxxxxFFFxxxxxxixxixxixxixxxxxFFFxxxxxxixxixxixxFFFxxxxxxxxxxxxxppppppppppppppppppppppppppppppppxxxxxxxxxxxFFFxxxxxx";
const char VehicleAccessorySrcFmt[] = "iiix";
const char VehicleAccessoryDstFmt[] = "iii";
const char CreatureTemplateSpellsFmt[] = "iiiiiiiiiii";
const char SpellScriptTargetFmt[] = "iiii";

SQLStorage sCreatureStorage(CreatureInfosrcfmt, CreatureInfodstfmt, "entry", "creature_template");
SQLStorage sCreatureDataAddonStorage(CreatureDataAddonInfofmt, "guid", "creature_addon");
SQLStorage sCreatureModelStorage(CreatureModelfmt, "modelid", "creature_model_info");
SQLStorage sCreatureInfoAddonStorage(CreatureInfoAddonInfofmt, "entry", "creature_template_addon");
SQLStorage sEquipmentStorage(EquipmentInfofmt, "entry", "creature_equip_template");
SQLStorage sItemStorage(ItemPrototypesrcfmt, ItemPrototypedstfmt, "entry", "item_template");
SQLStorage sPageTextStore(PageTextfmt, "entry", "page_text");
SQLStorage sInstanceTemplate(InstanceTemplatesrcfmt, InstanceTemplatedstfmt, "map", "instance_template");
SQLStorage sWorldTemplate(WorldTemplatesrcfmt, WorldTemplatedstfmt, "map", "world_template");
SQLStorage sConditionStorage(ConditionsSrcFmt, ConditionsDstFmt, "condition_entry", "conditions");

SQLHashStorage sGameObjectDataAddonStorage(GameObjectInfoAddonInfofmt, "guid", "gameobject_addon");
SQLHashStorage sGOStorage(GameObjectInfosrcfmt, GameObjectInfodstfmt, "entry", "gameobject_template");
SQLHashStorage sSpellTemplate(SpellTemplatesrcfmt, SpellTemplatedstfmt, "id", "spell_template");
SQLHashStorage sCreatureTemplateSpellsStorage(CreatureTemplateSpellsFmt, "entry", "creature_template_spells");

SQLMultiStorage sVehicleAccessoryStorage(VehicleAccessorySrcFmt, VehicleAccessoryDstFmt, "vehicle_entry", "vehicle_accessory");
SQLMultiStorage sSpellScriptTargetStorage(SpellScriptTargetFmt, "entry", "spell_script_target");
