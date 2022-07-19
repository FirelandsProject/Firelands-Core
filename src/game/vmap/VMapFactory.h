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

#ifndef FIRELANDS_H_VMAPFACTORY
#define FIRELANDS_H_VMAPFACTORY

#include "IVMapManager.h"

/**
This is the access point to the VMapManager.
*/

namespace VMAP
{
    //===========================================================

    /**
     * @brief
     *
     */
    class VMapFactory
    {
        public:
            /**
             * @brief
             *
             * @return IVMapManager
             */
            static IVMapManager* createOrGetVMapManager();
            /**
             * @brief
             *
             */
            static void clear();

            /**
             * @brief
             *
             * @param pSpellIdString
             */
            static void preventSpellsFromBeingTestedForLoS(const char* pSpellIdString);
            /**
             * @brief
             *
             * @param pSpellId
             * @return bool
             */
            static bool checkSpellForLoS(unsigned int pSpellId);

            static void chompAndTrim(std::string& str);
            static bool getNextId(const std::string& pString, unsigned int& pStartPos, unsigned int& pId);
    };
}
#endif
