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

#include "LogMessage.h"
#include "StringFormat.h"
#include "Timer.h"

LogMessage::LogMessage(LogLevel _level, std::string const& _type, std::string_view _text)
    : level(_level), type(_type), text(std::string(_text)), mtime(GetEpochTime()) { }

LogMessage::LogMessage(LogLevel _level, std::string const& _type, std::string_view _text, std::string_view _param1)
    : level(_level), type(_type), text(std::string(_text)), param1(std::string(_param1)), mtime(GetEpochTime()) { }

std::string LogMessage::getTimeStr(Seconds time)
{
    return Firelands::Time::TimeToTimestampStr(time, "%Y-%m-%d %X");
}

std::string LogMessage::getTimeStr() const
{
    return getTimeStr(mtime);
}
