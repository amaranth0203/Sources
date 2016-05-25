#!/bin/bash

echo Dollar Star is $*
echo "Dollar Star in double quotes is $*"
echo Dollar At is $@
echo "Dollar At in double quotes is $@"
echo
echo "Looping through Dollar Star"
for i in $*
do
    echo "Parameter is $i"
done
echo
echo "Looping through Dollar Star with double quotes"
for i in "$*"
do
    echo "Parameter is $i"
done
echo
echo "Looping through Dollar At"
for i in $@
do
    echo "Parameter is $i"
done
echo
echo "Looping through Dollar At with double quotes"
for i in "$@"
do
    echo "Parameter is $i"
done
