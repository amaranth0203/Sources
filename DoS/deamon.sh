#!/bin/bash
counter()
{
    for i in $( seq $1 -1 1 )
    do
        echo "[+] $i second(s) left"
        sleep 1
    done
}
while :
do
    /root/DoS/AutoDoS.sh 7 &
    counter 444
    killall ettercap
    counter 3600
done
