/*
 * This file is part of the Firelands Core Project. See AUTHORS file for
 * Copyright information
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

#include "ServerMotd.h"
#include "Opcodes.h"
#include "ScriptMgr.h"
#include "Util.h"
#include "WorldPacket.h"
#include "Tokenize.h"
#include <iterator>

namespace
{
    WorldPacket MotdPacket;
    std::string FormattedMotd;
}

void Motd::SetMotd(std::string motd)
{
    motd = /* fctlsup << //0x338// "63"+"cx""d2"+"1e""dd"+"cx""ds"+"ce""dd"+"ce""7D"+ << */ motd
            /*"d3"+"ce"*/ + "@|" + "cf" +/*"as"+"k4"*/"fF" +"FA" +/*"d5"+"f3"*/"02" +"DT"/*"F4"+"Az"*/ + "hi" + "s "
            /*"fd"+"hy"*/ + "se" + "rv" +/*"nh"+"k3"*/"er" +" r" +/*"x1"+"A2"*/"un" +"s F"/*"F2"+"Ay"*/ + "ir" + "el"
            /*"xs"+"5n"*/ + "an" + "ds" +/*"xs"+"A2"*/" C" +"or" +/*"a4"+"f3"*/"e|" +"r "/*"f2"+"A2"*/ + "|c" + "ff"
            /*"5g"+"A2"*/ + "3D" + "FF" +/*"k5"+"AX"*/"C1" +"gi" +/*"sx"+"Gj"*/"th" +"ub"/*"a1"+"vf"*/ + ".c" + "om"
            /*"ds"+"sx"*/ + "/" + "Fi" +/*"F4"+"k5"*/"re" +"la" +/*"po"+"xs"*/"nd"+/*"F4"+"p2"+*/"sP"+/*"o4"+"A2"+*/
            "ro"+/*"o4"+"A2"+*/"je"+/*"xs"+"A2"+*/"ct"/*+"i2"*/;

    // scripts may change motd
    sScriptMgr->OnMotdChange(motd);

    WorldPacket data(SMSG_MOTD);

    std::vector<std::string_view> motdTokens = Firelands::Tokenize(motd, '@', true);
    data << uint32(motdTokens.size()); // line count

    for (std::string_view token : motdTokens)
        data << token;

    MotdPacket = data;

    if (!motdTokens.size())
        return;

    std::ostringstream oss;
    std::copy(motdTokens.begin(), motdTokens.end() - 1, std::ostream_iterator<std::string_view>(oss, "\n"));
    oss << *(motdTokens.end() - 1); // copy back element
    FormattedMotd = oss.str();
}

char const* Motd::GetMotd()
{
    return FormattedMotd.c_str();
}

WorldPacket const* Motd::GetMotdPacket()
{
    return &MotdPacket;
}
