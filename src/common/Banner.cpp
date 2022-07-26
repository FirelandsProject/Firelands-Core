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

#include "Banner.h"
#include "GitRevision.h"
#include "StringFormat.h"

void Firelands::Banner::Show(std::string_view applicationName, void(*log)(std::string_view text), void(*logExtraInfo)())
{
    log(Firelands::StringFormatFmt("{} ({})", GitRevision::GetFullVersion(), applicationName));
    log("<Ctrl-C> to stop.\n");
    log("________                     ___                         ___            ");
    log("`MMMMMMM 68b                 `MM                         `MM            ");
    log(" MM    \\ Y89                  MM                          MM           ");
    log(" MM      ___ ___  __   ____   MM    ___   ___  __     ____MM   ____     ");
    log(" MM   ,  `MM `MM 6MM  6MMMMb  MM  6MMMMb  `MM 6MMb   6MMMMMM  6MMMMb\\  ");
    log(" MMMMMM   MM  MM69 \" 6M'  `Mb MM 8M'  `Mb  MMM9 `Mb 6M'  `MM MM'    `  ");
    log(" MM   `   MM  MM'    MM    MM MM     ,oMM  MM'   MM MM    MM YM.        ");
    log(" MM       MM  MM     MMMMMMMM MM ,6MM9'MM  MM    MM MM    MM  YMMMMb    ");
    log(" MM       MM  MM     MM       MM MM'   MM  MM    MM MM    MM      `Mb   ");
    log(" MM       MM  MM     YM    d9 MM MM.  ,MM  MM    MM YM.  ,MM L    ,MM   ");
    log("_MM_     _MM__MM_     YMMMM9 _MM_`YMMM9'Yb_MM_  _MM_ YMMMMMM_MYMMMM9    ");
    log("                                                                      \n");
    log("   ____                                                                 ");
    log("  6MMMMb/                                                               ");
    log(" 8P    YM                                                               ");
    log("6M      Y   _____  ___  __   ____                                       ");
    log("MM         6MMMMMb `MM 6MM  6MMMMb                                      ");
    log("MM        6M'   `Mb MM69 \" 6M'  `Mb                                    ");
    log("MM        MM     MM MM'    MM    MM                                     ");
    log("MM        MM     MM MM     MMMMMMMM                                     ");
    log("YM      6 MM     MM MM     MM                                           ");
    log(" 8b    d9 YM.   ,M9 MM     YM    d9                                     ");
    log("  YMMMM9   YMMMMM9 _MM_     YMMMM9                                      ");
    log("\n");
    log("     FirelandsCata 4.3.4  -  https://github.com/FirelandsProjec\n");

    if (logExtraInfo)
    {
        logExtraInfo();
    }

    log(" ");
}
