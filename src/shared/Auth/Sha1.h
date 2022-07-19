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

#ifndef _AUTH_SHA1_H
#define _AUTH_SHA1_H

#include "Common/Common.h"
#include <openssl/sha.h>
#include <openssl/crypto.h>

class BigNumber;

/**
 * @brief
 *
 */
class Sha1Hash
{
    public:
        /**
         * @brief
         *
         */
        Sha1Hash();
        /**
         * @brief
         *
         */
        ~Sha1Hash();

        /**
         * @brief
         *
         * @param bn0...
         */
        void UpdateBigNumbers(BigNumber* bn0, ...);

        /**
         * @brief
         *
         * @param dta
         * @param len
         */
        void UpdateData(const uint8* dta, int len);
        /**
         * @brief
         *
         * @param str
         */
        void UpdateData(const std::string& str);

        /**
         * @brief
         *
         */
        void Initialize();
        /**
         * @brief
         *
         */
        void Finalize();

        /**
         * @brief
         *
         * @return uint8
         */
        uint8* GetDigest(void) { return mDigest; };
        /**
         * @brief
         *
         * @return int
         */
        int GetLength(void) { return SHA_DIGEST_LENGTH; };

    private:
        SHA_CTX mC; /**< TODO */
        uint8 mDigest[SHA_DIGEST_LENGTH]; /**< TODO */
};
#endif
