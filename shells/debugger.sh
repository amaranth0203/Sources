#!/bin/bash

LOGFILE=/tmp/myscript.log
# The higher the value of VERBOSE, the more talkative the log file is.
# Low values of VERBOSE mean a shorter log file;
# High values of VERBOSE mean a longer log file.
VERBOSE=10
APPNAME=`basename $0`


function logmsg()
{
    echo "${APPNAME} : `date` : $@" >> $LOGFILE
    echo "[+] begin"
    echo "$@"
    echo "[+] end"
}

function debug()
{
    verbosity=$1
    shift
    if [ "$VERBOSE" -gt "$verbosity" ] ; then
        echo "${APPNAME} : `date` : DEBUG Level ${verbosity} : $@" >> $LOGFILE
    fi
}

function die()
{
    echo "${APPNAME} : `date` : FATAL ERROR : $@" >> $LOGFILE
    exit 1
}

logmsg Starting script $0
uname -a || die uname command not found
logmsg `uname -a`
cat /etc/redhat-release || debug 8 Not a RedHat-based system
cat /etc/debian_version || debug 8 Not a Debian-based system
cd /proc || debug 5 /proc filesystem not found
grep -q "pysical id" /proc/cpuinfo || debug 8 /proc/cpuinfo virtual file not found.
logmsg Found `grep "physical id" /proc/cpuinfo | sort -u | wc -l` physical CPUs.
unset IPADDR
. /etc/sysconfig/network-scriptes/ifcfg-eth0 || debug 1 ifcfg-eth0 not readable
logmsg eth0 IP address defined as $IPADDR
logmsg Script $0 finished.
