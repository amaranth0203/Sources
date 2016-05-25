#!/bin/bash

LOG=/tmp/factorial-log.txt
> $LOG

factorial()
{
    echo "`date` : Called with $1" >> $LOG
    sleep 1
    if [ "$1" -gt "1" ] ; then
        previous=`expr $1 - 1`
        factorial $previous
        parent=$?
        result=`expr $1 \* $parent`
        echo "`date` : Passed $1 - the factorial of $previous is ${parent}. "\
             "$1 * $parent is ${result}." >> $LOG
        echo "`date` : Sleeping for 2 seconds." >> $LOG
        sleep 2
        return $result
    else
        echo "`date` : Passed $1 - returning 1." >> $LOG
        echo "`date` : Sleeping for 5 seconds." >> $LOG
        sleep 5
        return 1
    fi
}

read -p "Enter a number : " x
echo "Started at `date`"
factorial $x
echo "Answer : $?"
echo "Here is my working :"
cat $LOG
rm -f $LOG
echo "Finished at `date`"
