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

#ifndef FIRELANDS_OBJECTLIFETIME_H
#define FIRELANDS_OBJECTLIFETIME_H

#include <stdexcept>
#include "Platform/Define.h"

/**
 * @brief
 *
 */
typedef void (* Destroyer)(void);

namespace Firelands
{
    /**
     * @brief
     *
     * @param (func)()
     */
    void  at_exit(void (*func)());

    template<class T>
    /**
     * @brief
     *
     */
    class ObjectLifeTime
    {
        public:

            /**
             * @brief
             *
             * @param (destroyer)()
             */
            static void ScheduleCall(void (*destroyer)())
            {
                at_exit(destroyer);
            }

            /**
             * @brief
             *
             */
            DECLSPEC_NORETURN static void OnDeadReference() ATTR_NORETURN;
    };

    template <class T>
    /**
     * @brief We don't handle Dead Reference for now
     *
     */
    void ObjectLifeTime<T>::OnDeadReference()           // We don't handle Dead Reference for now
    {
        throw std::runtime_error("Dead Reference");
    }
}

#endif
