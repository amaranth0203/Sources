#!/bin/bash
host=${1:-declarn}

while grep -qw $host /tmp/hosts-to-ping.txt
do
    ping -c 3 -w 4 $host > /dev/null 2>&1
    if [ "$?" -eq "0" ] ; then 
        echo "`date` : $host is up "
    else
        echo "`date` : $host is down "
    fi
    sleep 4
done
