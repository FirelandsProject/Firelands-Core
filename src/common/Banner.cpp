/*
 * This file is part of the FirelandsCore Project. See AUTHORS file for Copyright information
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

#include "Banner.h"
#include "GitRevision.h"
#include "StringFormat.h"

void Firelands::Banner::Show(char const* applicationName, void(*log)(char const* text), void(*logExtraInfo)())
{
    log(Firelands::StringFormat("%s (%s)", GitRevision::GetFullVersion(), applicationName).c_str());
    log("<Ctrl-C> to stop.\n");
    log(" _____  ____  ____     ___  _       ____  ____   ___   _____        __   ___   ____     ___ ");
    log("|     ||    ||    \\   /  _]| |     /    ||    \\ |   \\ / ___/       /  ] /   \\ |    \\   /  _]");
    log("|   __| |  | |  D  ) /  [_ | |    |  o  ||  _  ||    (   \\_       /  / |     ||  D  ) /  [_ ");
    log("|  |_   |  | |    / |    _]| |___ |     ||  |  ||  D  \\__  |     /  /  |  O  ||    / |    _]");
    log("|   _]  |  | |    \\ |   [_ |     ||  _  ||  |  ||     /  \\ |    /   \\_ |     ||    \\ |   [_ ");
    log("|  |    |  | |  .  \\|     ||     ||  |  ||  |  ||     \\    |    \\     ||     ||  .  \\|     |");
    log("|__|   |____||__|\\_||_____||_____||__|__||__|__||_____|\\___|     \\____| \\___/ |__|\\_||_____|");
    log("https://github.com/seobryn/Firelands-Core                                                  \n");

    if (logExtraInfo)
        logExtraInfo();
}
