#!/bin/bash

#getipaddr()
#{
#    cd /etc/sysconfig/network-scripts || return 1
#    if [ -f ifcfg-$1 ] ; then
#        unset IPADDR
#        . ifcfg-$1
#        if
#
#    else
#        return 3
#    fi
#    # Not strictly needed
#    return 0
#}

test()
{
    echo aaa
    return 4
}

bbb=`test`
echo "aaa $? aaa"

echo "bbb $bbb bbb"
