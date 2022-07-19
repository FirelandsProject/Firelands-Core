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

#ifndef FIRELANDS_H_BATTLEGROUNDRV
#define FIRELANDS_H_BATTLEGROUNDRV

class BattleGround;

class BattleGroundRVScore : public BattleGroundScore
{
    public:
        BattleGroundRVScore() {};
        virtual ~BattleGroundRVScore() {};
        // TODO fix me
};

class BattleGroundRV : public BattleGround
{
        friend class BattleGroundMgr;

    public:
        BattleGroundRV();

        /**
         * @brief inherited from BattlegroundClass
         *
         * @param plr
         */
        virtual void AddPlayer(Player* plr) override;
};
#endif
