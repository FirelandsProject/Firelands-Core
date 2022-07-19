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

#ifndef FIRELANDS_H_AUTHCRYPT
#define FIRELANDS_H_AUTHCRYPT

#include "Common/Common.h"
#include "ARC4.h"

class BigNumber;

/**
 * @brief
 *
 */
class AuthCrypt
{
    public:
        /**
         * @brief
         *
         */
        AuthCrypt();
        /**
         * @brief
         *
         */
        ~AuthCrypt();


        /**
         * @brief
         *
         */
        void Init(BigNumber* K);
        /**
         * @brief
         *
         * @param
         * @param size_t
         */
        void DecryptRecv(uint8*, size_t);
        /**
         * @brief
         *
         * @param
         * @param size_t
         */
        void EncryptSend(uint8*, size_t);

        /**
         * @brief
         *
         * @return bool
         */
        bool IsInitialized() { return _initialized; }

    private:
        ARC4 _clientDecrypt;
        ARC4 _serverEncrypt;
        bool _initialized; /**< TODO */
};
#endif
