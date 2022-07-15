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

#ifndef FirelandsCore_Banner_h__
#define FirelandsCore_Banner_h__

#include "Define.h"
#include <iostream>

namespace Firelands {
namespace Banner {
FC_COMMON_API void Show(std::string applicationName,
                        void (*log)(std::string text), void (*logExtraInfo)());
}
} // namespace Firelands

#endif // FirelandsCore_Banner_h__
