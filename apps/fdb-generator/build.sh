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

# static variables definition
WATERMARK_TEXT="/* This file was generated with FDB Generator and is part of the Firelands Core Project */"

source utils/banner.sh
print_menu

# Only pause to take actions
read -p "Press a key to start building database or <Ctrl-C> to close."

clear
print_menu

# Paths and variable definition
project_root=$(pwd | sed 's/\/apps.*//')
timestamp=$(date +"%Y_%d_%m_%H%M%S")
mysql_version=$(mysql -V | sed 's/for .*//' | sed 's/[a-zA-Z ]*//')
mysql_version_number=$(echo "${mysql_version}" | tr . 0)
build_directory="$project_root/data/sql/build/FDB"
build_file_name="FDB_$timestamp.sql"

#db names spec
AUTH_DB_NAME="auth"
WORLD_DB_NAME="world"
CHARACTERS_DB_NAME="characters"

# Style spacing
echo ""

printf "*   Start building Firelands Database     ${purple}(FDB)${reset}\n"
printf "*   Using MYSQL Version                   ${purple}${mysql_version}\n${reset}"
printf "*   Creating ${cyan}build${reset} directory into         ${purple}${project_root}/data/sql${reset}\n\n"

mkdir -p "$build_directory"
cd "$build_directory"

printf ">   Creating ${cyan}$build_file_name${reset} Archive from data.\n"

touch $build_file_name
echo "$WATERMARK_TEXT" >> $build_file_name

create_folder="${project_root}/data/sql/create"
base_folder="${project_root}/data/sql/base"
custom_folder="${project_root}/data/sql/custom"

read -p "    Do you want to include drop databases module? (y/N) " include_drop_db
include_drop_db=${include_drop_db:-n}
include_drop_db=$(echo "$include_drop_db" | tr '[:upper:]' '[:lower:]')

if [ "$include_drop_db" = "y" ]; then
    if ! [[ -d $create_folder ]]; then
        printf "${red}>   ERROR: ${reset} Missing ${cyan}create${reset} data folder.\n"
        exit 0
    fi

    if [[ $mysql_version_number -ge 80000 ]]; then
        cat "$create_folder/drop_mysql_8.sql" >> $build_file_name
    else
        cat "$create_folder/drop_mysql.sql" >> $build_file_name
    fi
    printf ">   Added ${cyan}Drop database${reset} module.\n"
fi

cat "$create_folder/create_mysql.sql" >> $build_file_name
printf ">   Added ${cyan}create_mysql.sql${reset} module.\n"

# Auth Database Generator
echo "use $AUTH_DB_NAME;" >> $build_file_name
auth_db_files=$(ls "$base_folder/${AUTH_DB_NAME}_database/" | grep .sql)
printf ">   Generating ${cyan}${AUTH_DB_NAME}_database${reset} module.\n"
for _file in $auth_db_files
do
    printf ">   Processing ${cyan}${_file}${reset}.\n"
    cat "$base_folder/${AUTH_DB_NAME}_database/$_file" >> $build_file_name
done
printf ">   Processing ${cyan}${AUTH_DB_NAME}_database${reset} completed.\n"


# World Database Generator
echo "use $WORLD_DB_NAME;" >> $build_file_name
world_db_files=$(ls "$base_folder/${WORLD_DB_NAME}_database/" | grep .sql)
printf ">   Generating ${cyan}${WORLD_DB_NAME}_database${reset} module.\n"
for _file in $world_db_files
do
    printf ">   Processing ${cyan}${_file}${reset}.\n"
    cat "$base_folder/${WORLD_DB_NAME}_database/$_file" >> $build_file_name
done
printf ">   Processing ${cyan}${WORLD_DB_NAME}_database${reset} completed.\n"

# Characters Database Generator
echo "use $CHARACTERS_DB_NAME;" >> $build_file_name
characters_db_files=$(ls "$base_folder/${CHARACTERS_DB_NAME}_database/" | grep .sql)
printf ">   Generating ${cyan}${CHARACTERS_DB_NAME}_database${reset} module.\n"
for _file in $characters_db_files
do
    printf ">   Processing ${cyan}${_file}${reset}.\n"
    cat "$base_folder/${CHARACTERS_DB_NAME}_database/$_file" >> $build_file_name
done
printf ">   Processing ${cyan}${CHARACTERS_DB_NAME}_database${reset} completed.\n"

echo ">   Script Completed."
