/*
 * Copyright (C) 2022 Firelands Project <https://github.com/FirelandsProject>
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

#include "ChatCommandHelpers.h"
#include "Chat.h"
#include "ObjectMgr.h"

void Firelands::Impl::ChatCommands::SendErrorMessageToHandler(ChatHandler* handler, std::string_view str)
{
    handler->SendSysMessage(str);
    handler->SetSentErrorMessage(true);
}

char const* Firelands::Impl::ChatCommands::GetFirelandsString(ChatHandler const* handler, FirelandsStrings which)
{
    return handler->GetFirelandsString(which);
}
