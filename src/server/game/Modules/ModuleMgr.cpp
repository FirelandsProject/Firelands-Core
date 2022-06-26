/*
 * This file is part of the Firelands Core Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Affero General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ModuleMgr.h"
#include "Tokenize.h"

namespace {
    std::string_view _modulesList = {};
}

void Firelands::Module::SetEnableModulesList(std::string_view modules) {
    _modulesList = modules;
}

std::vector<std::string_view> Firelands::Module::GetEnableModulesList()
{
    std::vector<std::string_view> _list;

    for (auto const& modName : Firelands::Tokenize(_modulesList, ',', false))
    {
        _list.emplace_back(modName);
    }

    return _list;
}
