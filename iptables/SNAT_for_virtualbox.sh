#
# Share school-net with virtual machine
# VBOX_NET use host-only Adapter
# OUTPUT_INTERFACE is the interface to school-net
#
#                           2014-12-18 14:22:37

VBOX_NET="192.168.44.0/24"
OUTPUT_INTERFACE="eth0"

# enable ip forward
echo "1" > /proc/sys/net/ipv4/ip_forward

# flush nat rules
iptables -t nat -F
iptables -t nat -X
iptables -t nat -Z

# share with virtualbox
iptables -t nat -A POSTROUTING -s $VBOX_NET -o $OUTPUT_INTERFACE -j MASQUERADE
iptables -t nat -A POSTROUTING -s $VBOX_NET -o "ppp0" -j MASQUERADE
