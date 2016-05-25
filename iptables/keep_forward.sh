while :
do
    echo "1" > /proc/sys/net/ipv4/ip_forward
    sleep 100
done
