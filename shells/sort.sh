#!/bin/bash

if [ "$1" > "$2" ] ; then
    echo "$2 $1"
else
    echo "$1 $2"
fi
