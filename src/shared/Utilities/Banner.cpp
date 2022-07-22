#include "Utilities/Banner.h"

int GetCoreNumber()
{
#if defined(CLASSIC)
    return 0;
#elif defined(TBC)
    return 1;
#elif defined(WOTLK)
    return 2;
#elif defined(CATA)
    return 3;
#elif defined(MOP)
    return 4;
#elif defined(WOD)
    return 5;
#elif defined(LEGION)
    return 6;
#else
    return -1;
#endif
}

void Firelands::Banner::Show() {
    int coreNumber = GetCoreNumber();
    switch (coreNumber)
    {
    case 0: // CLASSIC
    case 1: // TBC
    case 2: // WOTLK
    case 3: // CATA
    case 4: // MOP
        sLog.outString("<Ctrl-C> to stop.\n"
            "_____  ____  ____     ___  _       ____  ____   ___   _____        __   ___   ____     ___       \n"
            "|     ||    ||    \\   /  _]| |     /    ||    \\ |   \\ / ___/       /  ] /   \\ |    \\   /  _]\n"
            "|   __| |  | |  D  ) /  [_ | |    |  o  ||  _  ||    (   \\_       /  / |     ||  D  ) /  [_     \n"
            "|  |_   |  | |    / |    _]| |___ |     ||  |  ||  D  \\__  |     /  /  |  O  ||    / |    _]    \n"
            "|   _]  |  | |    \\ |   [_ |     ||  _  ||  |  ||     /  \\ |    /   \\_ |     ||    \\ |   [_  \n"
            "|  |    |  | |  .  \\|     ||     ||  |  ||  |  ||     \\    |    \\     ||     ||  .  \\|     | \n"
            "|__|   |____||__|\\_||_____||_____||__|__||__|__||_____|\\___|     \\____| \\___/ |__|\\_||_____|\n"
            " Powered By Firelands Core Project                                                               \n"
            " ________________________________________________________________________________________________\n"
            "\n"
            " Website/Forum/Wiki/Issue Tracker: http://github.com/FirelandsProject                            \n"
            " ________________________________________________________________________________________________\n"
            "\n");
        break;
    }
};
