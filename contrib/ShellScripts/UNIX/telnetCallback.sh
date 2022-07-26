#!/bin/bash

case "${1}" in
'updateError')
/usr/bin/expect <<EOF
spawn telnet localhost 3333
expect "Username:"
send "USER\r"
expect "Password:"
send "PASSWORD\r"
expect "TC>"
send "gmn COMPILE ERROR OCCURED CHECK FOR ERRORS!\r"
expect "TC>"
send "server state 3"
expect "TC>"
send "quit\r"
EOF
;;
'updateSuccess')
/usr/bin/expect <<EOF
spawn telnet localhost 3333
expect "Username:"
send "USER\r"
expect "Password:"
send "PASSWORD\r"
expect "TC>"
send "gmn CORE COMPILE IS DONE! SERVER RESTART IN 20 MINUTES! PLEASE APPLY ALL SQL UPDATES!\r"
expect "TC>"
send "server restart 20\r"
expect "TC>"
send "server state 0"
expect "TC>"
send "quit\r"
EOF
;;
'branchSwitchError')
/usr/bin/expect <<EOF
spawn telnet localhost 3333
expect "Username:"
send "USER\r"
expect "Password:"
send "PASSWORD\r"
expect "TC>"
send "gmn BRANCH SWITCH TO ${2} FAILED!\r"
expect "TC>"
send "server restart 20\r"
expect "TC>"
send "server state 0"
expect "TC>"
send "quit\r"
EOF
;;
'branchSwitchSuccess')
/usr/bin/expect <<EOF
spawn telnet localhost 3333
expect "Username:"
send "USER\r"
expect "Password:"
send "PASSWORD\r"
expect "TC>"
send "gmn BRANCH SWITCH TO ${2} WAS SUCCESSFULLY!\r"
expect "TC>"
send "server restart 20\r"
expect "TC>"
send "server state 0"
expect "TC>"
send "quit\r"
EOF
;;
esac
