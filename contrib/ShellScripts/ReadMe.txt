1) copy update.sh, coreshell.sh and telnetAnnounce.sh to the bin folder
2) set all correct path's on update.sh and set account/password/port at telnetAnnounce.sh
3) enable telnet at worldserver.conf and install expect + telnet (apt-get install expect + apt-get install telnet)
4) set all files chmod 777
5) make sure that git is using a SSH key or login details

ingame commands:
.server shell update - pulls & compile the source and backups the old worldserver file
.server shell restore - restore the worldserver backup from the old compile