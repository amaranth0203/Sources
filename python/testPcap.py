import pcap
import dpkt
pc = pcap.pcap( 'vboxnet0' ) 
for ptime , pdata in pc :
    print ptime , dpkt.ethernet.Ethernet( pdata ) 
