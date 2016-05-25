#!/bin/bash
while :
do
for i in $( seq 1 1 4 )
do
    if [ $i == 1 ] ; then continue ; fi
    echo $i
    sleep 1
done
done
