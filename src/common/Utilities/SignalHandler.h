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

#ifndef _SIGNAL_HANDLER_H_
#define _SIGNAL_HANDLER_H_

#include <csignal>
#include <mutex>
#include <unordered_set>

namespace Firelands
{
    /// Handle termination signals
    class SignalHandler
    {
    private:
        std::unordered_set<int> _handled;
        mutable std::mutex _mutex;

    public:
        bool handle_signal(int sig, void (*func)(int))
        {
            std::lock_guard lock(_mutex);

            if (_handled.find(sig) != _handled.end())
            {
                return false;
            }

            _handled.insert(sig);
            signal(sig, func);
            return true;
        }

        ~SignalHandler()
        {
            for (auto const& sig : _handled)
            {
                signal(sig, nullptr);
            }
        }
    };
}

#endif // _SIGNAL_HANDLER_H_
