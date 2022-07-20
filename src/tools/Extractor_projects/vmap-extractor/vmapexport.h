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

#ifndef VMAPEXPORT_H
#define VMAPEXPORT_H

#include <string>
#include <set>

 /**
  * @brief
  *
  */
typedef std::set<std::string> StringSet;

/**
 * @brief
 *
 */
enum ModelFlags
{
    MOD_M2 = 1,
    MOD_WORLDSPAWN = 1 << 1,
    MOD_HAS_BOUND = 1 << 2
};

extern const char* szWorkDirWmo;
extern const char* szRawVMAPMagic;                          // vmap magic string for extracted raw vmap data

/**
 * @brief Test if the specified file exists in the building directory
 *
 * @param file
 * @return bool
 */
bool FileExists(const char* file);
void strToLower(char* str);

bool ExtractSingleWmo(std::string& fname);

/* @param fname = original path of the model, cleaned with FixNameCase and FixNameSpaces*/
bool ExtractSingleModel(std::string& fname);

void ExtractGameobjectModels();

#endif
