#!/bin/bash
factorial()
{
    if [ "$1" -gt "1" ] ; then
        previous=`expr $1 - 1 `
        parent=`factorial $previous | tail -1`
        result=`expr $1 \* $parent`
        echo $result
    else
        echo 1
    fi
}

factorial $1
