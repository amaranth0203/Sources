#!/bin/bash

i=1
while [ $i -lt 5 ]
do
    echo "`date` : Partition $i"
    ls -ld /dev/sda$i
    sleep 1.5
    let i=$i+1
done > partitions.txt
