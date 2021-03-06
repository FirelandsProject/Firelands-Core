/*
 * Copyright (C) 2022 Firelands Project <https://github.com/FirelandsProject>
 * Copyright (C) 2008-2013 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
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

#ifndef _AUTHCRYPT_H
#define _AUTHCRYPT_H

#include "ARC4.h"
#include "AuthDefines.h"
#include <array>

class AuthCrypt
{
public:
    AuthCrypt();

    void Init(SessionKey const& K);
    void DecryptRecv(uint8* data, size_t len);
    void EncryptSend(uint8* data, size_t len);

    bool IsInitialized() const { return _initialized; }

private:
    Firelands::Crypto::ARC4 _clientDecrypt;
    Firelands::Crypto::ARC4 _serverEncrypt;
    bool _initialized;
};
#endif
