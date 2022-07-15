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

#include "Banner.h"
#include "GitRevision.h"
#include "StringFormat.h"

void Firelands::Banner::Show(std::string applicationName, void(*log)(std::string text), void(*logExtraInfo)())
{
    log(Firelands::StringFormat("%s (%s)", GitRevision::GetFullVersion(), applicationName).c_str());
    log("\033[0;35m<Ctrl-C> to stop.\n\033[0m");
    log("\033[0;33m _____  ____  ____     ___  _       ____  ____   ___   _____        __   ___   ____     ___      \033[0m");
    log("\033[0;33m|     ||    ||    \\   /  _]| |     /    ||    \\ |   \\ / ___/       /  ] /   \\ |    \\   /  _]\033[0m");
    log("\033[0;33m|   __| |  | |  D  ) /  [_ | |    |  o  ||  _  ||    (   \\_       /  / |     ||  D  ) /  [_     \033[0m");
    log("\033[0;33m|  |_   |  | |    / |    _]| |___ |     ||  |  ||  D  \\__  |     /  /  |  O  ||    / |    _]    \033[0m");
    log("\033[0;33m|   _]  |  | |    \\ |   [_ |     ||  _  ||  |  ||     /  \\ |    /   \\_ |     ||    \\ |   [_  \033[0m");
    log("\033[0;33m|  |    |  | |  .  \\|     ||     ||  |  ||  |  ||     \\    |    \\     ||     ||  .  \\|     | \033[0m");
    log("\033[0;33m|__|   |____||__|\\_||_____||_____||__|__||__|__||_____|\\___|     \\____| \\___/ |__|\\_||_____|\033[0m");
    log(applicationName + "                                                                                              \n");
    log("https://github.com/FirelandsProject/Firelands-Core                                                              \n");

    if (logExtraInfo)
        logExtraInfo();
}
