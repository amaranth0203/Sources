#!/bin/bash

squarecube()
{
    echo "$2 * $2" | bc > $1
    echo "$2 * $2 * $2" | bc >> $1
}

output=`mktemp`
for i in 1 2 3 4 5
do
    squarecube $output $i
    square=`head -1 $output`
    cube=`tail -1 $output`
    echo "The square of $i is $square"
    echo "The cube of $i is $cube"
done
rm -f $output
