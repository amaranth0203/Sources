#!/bin/bash
host=${1:-declan}

while :
do
    ping -c 3 -w 4 $host > /dev/null 2>&1
    if [ "$?" -eq "0" ] ; then
        echo
        echo "`date` : $host is up"
        echo
    else
        echo
        echo "`date` : $host is down"
        echo
    fi
    sleep 10
done
