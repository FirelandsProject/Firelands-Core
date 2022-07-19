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

#ifndef FIRELANDS_NULLCREATUREAI_H
#define FIRELANDS_NULLCREATUREAI_H

#include "CreatureAI.h"

class NullCreatureAI : public CreatureAI
{
    public:

        explicit NullCreatureAI(Creature* c) : CreatureAI(c) {}
        ~NullCreatureAI();

        void MoveInLineOfSight(Unit*) override {}
        void AttackStart(Unit*) override {}
        void AttackedBy(Unit*) override {}
        void EnterEvadeMode() override {}

        bool IsVisible(Unit*) const override { return false;  }

        void UpdateAI(const uint32) override {}
        static int Permissible(const Creature*) { return PERMIT_BASE_IDLE;  }
};
#endif
