#!/bin/bash

# Settings
homeDir=/home/server

binDir=$homeDir/bin
sourceDir=$homeDir/frostmourne-3_3_5

echo "########################"
echo "# Switching Branch:"
echo "########################"

cd $sourceDir

# ###############################
# # not tested yet
# ###############################
if git checkout ${1} then
    cd $binDir
    ./telnetCallback.sh branchSwitchSuccess
    exit 1
fi

./telnetCallback.sh branchSwitchError