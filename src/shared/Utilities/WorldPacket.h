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

#ifndef _FIRELANDS_WORLDPACKET_H
#define _FIRELANDS_WORLDPACKET_H

#include "Common.h"
#include "ByteBuffer.h"
#include "Opcodes.h"

// Note: m_opcode and size stored in platfom dependent format
// ignore endianess until send, and converted at receive
/**
 * @brief
 *
 */
class WorldPacket : public ByteBuffer
{
    public:
        /**
         * @brief just container for later use
         *
         */
        WorldPacket() : ByteBuffer(0), m_opcode(MSG_NULL_ACTION)
        {
        }
        /**
         * @brief
         *
         * @param opcode
         * @param res
         */
        explicit WorldPacket(Opcodes opcode, size_t res = 200) : ByteBuffer(res), m_opcode(opcode) { }
        /**
         * @brief copy constructor
         *
         * @param packet
         */
        WorldPacket(const WorldPacket& packet) : ByteBuffer(packet), m_opcode(packet.m_opcode)
        {
        }

        /**
         * @brief
         *
         * @param opcode
         * @param newres
         */
        void Initialize(Opcodes opcode, size_t newres = 200)
        {
            clear();
            _storage.reserve(newres);
            m_opcode = opcode;
        }

        /**
         * @brief
         *
         * @return uint16
         */
        Opcodes GetOpcode() const { return m_opcode; }
        /**
         * @brief
         *
         * @param opcode
         */
        void SetOpcode(Opcodes opcode) { m_opcode = opcode; }
        /**
         * @brief
         *
         * @return const char
         */
        inline const char* GetOpcodeName() const { return LookupOpcodeName(m_opcode); }

    protected:
        Opcodes m_opcode;
};
#endif
