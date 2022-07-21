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

#ifndef _FIRELANDS_MOVESPLINEINIT_ARGS_H
#define _FIRELANDS_MOVESPLINEINIT_ARGS_H

#include "MoveSplineFlag.h"
#include <G3D/Vector3.h>

class Unit;

namespace Movement
{
    /**
     * @brief
     *
     */
    typedef std::vector<Vector3> PointsArray;

    /**
     * @brief
     *
     */
    union FacingInfo
    {
        /**
         * @brief
         *
         */
        struct
        {
            float x, y, z; /**< TODO */
        } f; /**< TODO */
        uint64  target; /**< TODO */
        float   angle; /**< TODO */

        /**
         * @brief
         *
         * @param o
         */
        FacingInfo(float o) : angle(o) {}
        /**
         * @brief
         *
         * @param t
         */
        FacingInfo(uint64 t) : target(t) {}
        /**
         * @brief
         *
         */
        FacingInfo() {}
    };

    /**
     * @brief
     *
     */
    struct MoveSplineInitArgs
    {
            /**
             * @brief
             *
             * @param path_capacity
             */
            MoveSplineInitArgs(size_t path_capacity = 16) : path_Idx_offset(0),
                velocity(0.f), parabolic_amplitude(0.f), time_perc(0.f), splineId(0), initialOrientation(0.f)
            {
                path.reserve(path_capacity);
            }

            PointsArray path; /**< TODO */
            FacingInfo facing; /**< TODO */
            MoveSplineFlag flags; /**< TODO */
            int32 path_Idx_offset; /**< TODO */
            float velocity; /**< TODO */
            float parabolic_amplitude;
            float time_perc;
            uint32 splineId; /**< TODO */
            float initialOrientation;

            /**
             * @brief Returns true to show that the arguments were configured correctly and MoveSpline initialization will succeed.
             *
             * @param unit
             * @return bool
             */
            bool Validate(Unit* unit) const;
        private:
            /**
             * @brief
             *
             * @return bool
             */
            bool _checkPathBounds() const;
    };
}

#endif // _FIRELANDS_MOVESPLINEINIT_ARGS_H