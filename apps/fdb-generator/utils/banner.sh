# This file is part of the Firelands Core Project. See AUTHORS file for Copyright information
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

# Colors Section config
black="\033[0;30m"
red="\033[0;31m"
green="\033[0;32m"
brown_orange="\033[0;33m"
blue="\033[0;34m"
purple="\033[0;35m"
cyan="\033[0;36m"
light_gray="\033[0;37m"
dark_gray="\033[1;30m"
light_red="\033[1;31m"
light_green="\033[1;32m"
yellow="\033[1;33m"
light_blue="\033[1;34m"
light_purple="\033[1;35m"
light_cyan="\033[1;36m"
white="\033[1;37m"
reset="\033[0m"

print_menu(){
    echo -e "${brown_orange} _____  ____  ____     ___  _       ____  ____   ___   _____        __   ___   ____     ___ ${reset}"
    echo -e "${brown_orange}|     ||    ||    \   /  _]| |     /    ||    \ |   \ / ___/       /  ] /   \ |    \   /  _]${reset}"
    echo -e "${brown_orange}|   __| |  | |  D  ) /  [_ | |    |  o  ||  _  ||    (   \_       /  / |     ||  D  ) /  [_ ${reset}"
    echo -e "${brown_orange}|  |_   |  | |    / |    _]| |___ |     ||  |  ||  D  \__  |     /  /  |  O  ||    / |    _]${reset}"
    echo -e "${brown_orange}|   _]  |  | |    \ |   [_ |     ||  _  ||  |  ||     /  \ |    /   \_ |     ||    \ |   [_ ${reset}"
    echo -e "${brown_orange}|  |    |  | |  .  \|     ||     ||  |  ||  |  ||     \    |    \     ||     ||  .  \|     |${reset}"
    echo -e "${brown_orange}|__|   |____||__|\_||_____||_____||__|__||__|__||_____|\___|     \____| \___/ |__|\_||_____|${reset}"
    echo -e "${brown_orange}                                                                                            ${reset}"
    echo -e "${cyan}FDB Generator                                                                       ${reset}"
    echo -e "${cyan}https://github.com/FirelandsProject/Firelands-Core                                  ${reset}"
    echo    "===========================================================================================================        "
    echo ""
}
