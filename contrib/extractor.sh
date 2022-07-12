#!/bin/bash

if ! which dialog > /dev/null; then
   echo "dialog package not found, please install before start"
   exit 0
fi

WIDTH=90
HEIGHT=15
CHOICE_HEIGHT=4
BACKTITLE="Firelands Core Extractor"
TITLE="Extractor Menu"
MENU="Please select one of the following options to start extracting"
OPTIONS=(
    1 "Extract dbc/db2 and maps"
    2 "Extract vmaps (needs maps to be extracted before you run this)"
    3 "Extract mmaps (needs vmaps to be extracted before you run this, may take hours)"
    4 "Extract all (may take hours)"
    0 "Exit"
)

SELECTION=$(dialog --clear --no-cancel \
    --backtitle "$BACKTITLE" \
    --title "$TITLE" \
    --menu "$MENU" \
    $HEIGHT $WIDTH $CHOICE_HEIGHT \
    "${OPTIONS[@]}" \
    2>&1 > /dev/tty \
)
clear

function extract_maps(){
    if [ -f "map_extractor" ]; then
        echo "Start Extracting dbc/db2 and maps"
        ./map_extractor
    else
        echo "Missing map_extractor binary file, please copy the binary file on the same level of this script"
    fi
}

function extract_vmaps(){
    if [[ -f "vmap4_extractor" && -f "vmap4_assembler" ]]; then
        echo "Start Extracting vmaps"
        ./vmap4_extractor
        mkdir vmaps
        ./vmap4_assembler Buildings vmaps
    else
        echo "Missing vmap4_extractor/vmap4_assembler binary files, please copy the binaries on the same level of this script"
    fi
}

function extract_mmaps(){
    if [ -f "mmaps_generator" ]; then
        echo "Start Extracting mmaps"
        mkdir mmaps
        ./mmaps_generator
    else
        echo "Missing mmaps_generator binary file, please copy the binary file on the same level of this script"
    fi
}

case $SELECTION in
    1)
        extract_maps
        exit 0
        ;;
    2)
        extract_vmaps
        exit 0
        ;;
    3)
        extract_mmaps
        exit 0
        ;;
    4)
        echo "Start Extracting ALL"
        extract_maps
        extract_vmaps
        extract_mmaps
        exit 0
        ;;
    0)
        echo "Bye..."
        exit 0
        ;;
esac

