#!/bin/bash

select item in one two three four five
do
    if [ ! -z "$item" ] ; then 
        echo "you chose option number $REPLY which is \"$item\""
    else
        echo "$REPLY is not valid."
    if
done
