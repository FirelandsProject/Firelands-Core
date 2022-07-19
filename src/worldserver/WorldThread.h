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

 /// \addtogroup worldserver
 /// @{
 /// \file

#ifndef FIRELANDS_H_WORLDTHREAD
#define FIRELANDS_H_WORLDTHREAD

#include <ace/INET_Addr.h>
#include <ace/Task.h>

#include "Common.h"
/**
 * @brief Heartbeat thread for the World
 *
 */
class WorldThread : public ACE_Task_Base
{
public:
    explicit WorldThread(uint16 port, const char* host);
    virtual int open(void*) override;
    virtual int svc() override;
private:
    ACE_INET_Addr listen_addr;
};
#endif
/// @}
