#!/bin/bash
#For Debian based distributions only
#Change the path variable that matches your own
path="/home/firelands/server/bin"
while true; do
cd $path
./authserver
wait
done
