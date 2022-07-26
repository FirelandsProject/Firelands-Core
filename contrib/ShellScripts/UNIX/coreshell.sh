#!/bin/bash

# Settings
homeDir=/home/server
binDir=$homeDir/bin

case "${1}" in
'update')
screen -dmS coreupdate ./update.sh
;;
'restore')
rm $binDir/worldserver
cp $binDir/worldserver.backup $binDir/worldserver
;;
'switchbranch')
screen -dmS coreswitchbranch ./switchbranch.sh "${2}"
;;
esac
