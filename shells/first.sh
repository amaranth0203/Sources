#!/bin/bash

#echo -n "Enter your name: " 
#read myvar
#echo "myvar is $MYVAR"
#echo "Hello $myvar"

#echo -n "Please enter your first name and last name : "
#read firstname lastname
#echo "Hello, $firstname. How is the $lastname family? "

#echo "Relevant environment variables : "
#set | grep "name="

while read message
do
    echo $message
done < /etc/motd
