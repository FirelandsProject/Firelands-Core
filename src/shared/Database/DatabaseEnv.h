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

#if !defined(DATABASEENV_H)
#define DATABASEENV_H

#include "Common/Common.h"
#include "Log/Log.h"
#include "Utilities/Errors.h"

#include "Database/Field.h"
#include "Database/QueryResult.h"

#include "Database/QueryResultMysql.h"
#include "Database/Database.h"
#include "Database/DatabaseMysql.h"
/**
 * @brief
 *
 */
typedef DatabaseMysql DatabaseType;
#define _LIKE_           "LIKE"
#define _CONCAT3_(A,B,C) "CONCAT( " A " , " B " , " C " )"
#define _OFFSET_         "LIMIT %d,1"

extern DatabaseType WorldDatabase; /**< TODO */
extern DatabaseType CharacterDatabase; /**< TODO */
extern DatabaseType LoginDatabase; /**< TODO */

#endif
