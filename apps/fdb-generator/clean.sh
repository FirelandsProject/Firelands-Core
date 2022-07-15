#!/bin/bash

# This file is part of the Firelands Core Project. See AUTHORS file for Copyright information
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

source utils/banner.sh

print_menu

project_root=$(pwd | sed 's/\/apps.*//')
sql_folder="$project_root/sql"

cd $sql_folder


printf ">   Do you want to delete all builds into the ${purple}$sql_folder/build${reset} ? (Y/n) "
read delete_confirmation
delete_confirmation=${delete_confirmation:-y}

if [ $delete_confirmation = 'y' ]; then
    rm -rf "$sql_folder/build"
    printf "> Done!\n"
else
    printf "> Canceled by the user."
fi

exit 0
