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

#ifndef _FIRELANDS_GAMEOBJECTMODEL_H
#define _FIRELANDS_GAMEOBJECTMODEL_H

#include <G3D/Matrix3.h>
#include <G3D/Vector3.h>
#include <G3D/AABox.h>
#include <G3D/Ray.h>
#include <G3D/Quat.h>
#include "DBCStructure.h"
#include "GameObject.h"

#include "Platform/Define.h"

namespace VMAP
{
    class WorldModel;
}

/**
 * @brief
 *
 */
class GameObjectModel
{
        uint32 phasemask;
        G3D::AABox iBound; /**< TODO */
        G3D::Matrix3 iInvRot; /**< TODO */
        G3D::Vector3 iPos; /**< TODO */
        //G3D::Vector3 iRot;
        float iInvScale; /**< TODO */
        float iScale; /**< TODO */
        VMAP::WorldModel* iModel; /**< TODO */

        /**
         * @brief
         *
         */
        GameObjectModel() : phasemask(0), iModel(NULL) {}
        /**
         * @brief
         *
         * @param pGo
         * @param info
         * @return bool
         */
        bool initialize(const GameObject* const pGo, const GameObjectDisplayInfoEntry* info);

    public:
        std::string name; /**< TODO */

        /**
         * @brief
         *
         * @return const G3D::AABox
         */
        const G3D::AABox& GetBounds() const { return iBound; }

        /**
         * @brief
         *
         */
        ~GameObjectModel();

        /**
         * @brief
         *
         * @return const G3D::Vector3
         */
        const G3D::Vector3& GetPosition() const { return iPos;}

        /** Enables\disables collision. */
        /**
         * @brief
         *
         */
        void disable() { phasemask = 0;}
        /**
         * @brief
         *
         * @param enabled
         */
        void enable(uint32 ph_mask) { phasemask = ph_mask;}

        /**
         * @brief
         *
         * @param Ray
         * @param MaxDist
         * @param StopAtFirstHit
         * @return bool
         */
        bool IntersectRay(const G3D::Ray& Ray, float& MaxDist, bool StopAtFirstHit, uint32 phaseMask) const;

        /**
         * @brief
         *
         * @param pGo
         * @return GameObjectModel
         */
        static GameObjectModel* Create(const GameObject* const pGo);
};
#endif
