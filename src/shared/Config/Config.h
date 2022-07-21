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

#ifndef CONFIG_H
#define CONFIG_H

#include "Common/Common.h"
#include <Policies/Singleton.h>
#include "Platform/Define.h"
#include <mutex>

#include <string>
#include <unordered_map>

class ACE_Configuration_Heap;

/**
 * @brief
 *
 */
class Config
{
    public:
        /**
         * @brief
         *
         */
        Config();
        /**
         * @brief
         *
         */
        ~Config();

        /**
         * @brief
         *
         * @param file
         * @return bool
         */
        bool SetSource(const char* file);
        /**
         * @brief
         *
         * @return bool
         */
        bool Reload();

        /**
         * @brief
         *
         * @param name
         * @param def
         * @return std::string
         */
        std::string GetStringDefault(const char* name, const char* def);
        /**
         * @brief
         *
         * @param name
         * @param def
         * @return bool
         */
        bool GetBoolDefault(const char* name, const bool def = false);
        /**
         * @brief
         *
         * @param name
         * @param def
         * @return int32
         */
        int32 GetIntDefault(const char* name, const int32 def);
        int64 GetInt64Default(const char* name, const int64 def);
        /**
         * @brief
         *
         * @param name
         * @param def
         * @return float
         */
        float GetFloatDefault(const char* name, const float def);

        /**
         * @brief
         *
         * @return std::string
         */
        std::string GetFilename() const { return mFilename; }

    private:

        std::string mFilename; /**< TODO */
        ACE_Configuration_Heap* mConf; /**< TODO */
};

#define sConfig Firelands::Singleton<Config>::Instance()

#endif