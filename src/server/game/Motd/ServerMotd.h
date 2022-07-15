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

#ifndef __FIRELANDS_SERVER_MOTD_h__
#define __FIRELANDS_SERVER_MOTD_h__

#include "Define.h"
#include <string>

class WorldPacket;

namespace Motd
{
    /// Set a new Message of the Day
    void SetMotd(std::string motd);

    /// Get the current Message of the Day
    char const* GetMotd();

    /// Get the motd packet to send at login
    WorldPacket const* GetMotdPacket();
}

#endif //__FIRELANDS_SERVER_MOTD_h__