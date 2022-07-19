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

#ifndef WDTFILE_H
#define WDTFILE_H

#include <string>
#include "mpqfile.h"
#include "wmo.h"
#include "stdlib.h"

class ADTFile;

/**
 * @brief
 *
 */
class WDTFile
{
    public:
        /**
         * @brief
         *
         * @param file_name
         * @param file_name1
         */
        WDTFile(char* file_name, char* file_name1);
        /**
         * @brief
         *
         */
        ~WDTFile(void);
        /**
         * @brief
         *
         * @param map_id
         * @param mapID
         * @return bool
         */
        bool init(char* map_id, unsigned int mapID);

        std::string* gWmoInstansName; /**< TODO */
        int gnWMO, nMaps; /**< TODO */

        /**
         * @brief
         *
         * @param x
         * @param z
         * @return ADTFile
         */
        ADTFile* GetMap(int x, int z);

    private:
        MPQFile WDT; /**< TODO */
        bool maps[64][64]; /**< TODO */
        std::string filename; /**< TODO */
};

#endif
