#!/bin/bash

echo "My name is `basename $0` - I was called as $0"
echo "I was called with $# parameters."
count=1
while [ "$#" -ge "1" ]
do
    echo "Parameter number $count is: $0"
    let count=$count+1
    shift
done
