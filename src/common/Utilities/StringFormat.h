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

#ifndef FirelandsCore_STRING_FORMAT_H
#define FirelandsCore_STRING_FORMAT_H

#include "Define.h"
#include <fmt/format.h>
#include <fmt/printf.h>

namespace Firelands
{
    /// Default FC string format function.
    template<typename Format, typename... Args>
    inline std::string StringFormat(Format&& fmt, Args&&... args)
    {
        return fmt::sprintf(std::forward<Format>(fmt), std::forward<Args>(args)...);
    }

    // Default string format function.
    template<typename... Args>
    inline std::string StringFormatFmt(std::string_view fmt, Args&&... args)
    {
        try
        {
            return fmt::format(fmt, std::forward<Args>(args)...);
        }
        catch (const fmt::format_error& formatError)
        {
            return fmt::format("An error occurred formatting string \"{}\": {}", fmt, formatError.what());
        }
    }

    /// Returns true if the given char pointer is null.
    inline bool IsFormatEmptyOrNull(char const* fmt)
    {
        return fmt == nullptr;
    }

    /// Returns true if the given std::string is empty.
    inline bool IsFormatEmptyOrNull(std::string const& fmt)
    {
        return fmt.empty();
    }
}

namespace Firelands::String
{
    template<class Str>
    FC_COMMON_API Str Trim(const Str& s, const std::locale& loc = std::locale());

    FC_COMMON_API std::string TrimRightInPlace(std::string& str);
}

#endif
