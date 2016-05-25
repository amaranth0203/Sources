#!/bin/bash

[ -z "$_definitions" ] && . definitions
[ -f /etc/redhat-release ] && . ./redhat-network
[ -f /etc/debian_version ] && . ./debian-network
[ `uname` == "SunOS" ] && . ./solaris-network

for thisnic in $*
do
    thisip=`getipaddr $thisnic`
    case $? in
        $SUCCESS) echo "The IP Address configured for $thisnic is $thisip" ;;
        $_WRONG_PLATFORM) echo "This does not seem to be running on the expected platfrom" ;;
        $_NO_IP) echo "NO IP Address defined for $thisnic" ;;
        $_NO_CONFIG) echo "No configuration found for $thisnic" ;;
    esac
done
