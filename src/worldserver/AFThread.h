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

#ifndef ANTIFREEZE_THREAD
#define ANTIFREEZE_THREAD

#include "ace/Task.h"
#include "Common.h"

class AntiFreezeThread : public ACE_Task_Base
{
    public:
        explicit AntiFreezeThread(uint32 delay);
        virtual int open(void*) override;
        virtual int svc() override;

    private:
        uint32 m_loops;
        uint32 m_lastchange;
        uint32 w_loops;
        uint32 w_lastchange;
        uint32 delaytime_;
};

#endif