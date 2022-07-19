#!/bin/bash
#For Debian based distributions only
#Change the path variable that matches your own
#Do not add a trailing slash
path="/home/firelands/server/bin"
while true; do
cd $path
./firelands
wait
done
