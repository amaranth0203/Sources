#!/bin/bash

function the_function()
{
    echo "This is the_function. I was passed $# arguments : "\
         "\"$1\" , \"$2\" , \"$3\"."
    local myvar=100
    echo "  GLOBALVAR started as $GLOBALVAR;"
    GLOBALVAR=`expr $GLOBALVAR + 1`
    echo "  GLOBALVAR is now $GLOBALVAR"
    echo "  myvar started as $myvar;"
    myvar=`expr $myvar + 1`
    echo "uniquevar started as $uniquevar"
    uniquevar=`expr $uniquevar + 1`
    echo "uniquevar is $uniquevar"
    echo "  Leaving the_function."
}

GLOBALVAR=1
myvar=500
echo "This is the main script."
echo "GLOBALVAR is $GLOBALVAR"
echo "myvar is $myvar"
echo "I was passed $# arguments : "\
     "\"$1\" , \"$2\" , \"$3\"."
echo "*******************************"
echo

echo "Calling the_function with 1 2 3 ..."
the_function 1 2 3 
echo "GLOBALVAR is $GLOBALVAR"
echo "myvar is $myvar"
echo
echo "Calling the_function with 3 2 1 ..."
the_function 3 2 1 
echo "GLOBALVAR is $GLOBALVAR"
echo "myvar is $myvar"
echo
echo "Calling the_function with $1 $2 $3 ..."
the_function $1 $2 $3
echo "GLOBALVAR is $GLOBALVAR"
echo "myvar is $myvar"
echo 
echo "All Done."
echo "GLOBALVAR is $GLOBALVAR"
echo "myvar is $myvar"
echo "uniquevar is $uniquevar"
