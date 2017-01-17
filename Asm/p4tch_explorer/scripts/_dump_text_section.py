from pefile import PE

if __name__ == '__main__':
    import sys
    exe_file = sys.argv[1]
    final_bin_file = sys.argv[1] + ".bin"
    pe = PE(exe_file)
    pe_sections = pe.get_section_by_rva(pe.OPTIONAL_HEADER.AddressOfEntryPoint)
    buf = ''
    PointerToRawData = pe_sections.PointerToRawData
    length = pe_sections.Misc_VirtualSize - 1 # except 0xcc in function end_of_file
    with open( exe_file , 'rb' ) as f :
        buf = f.read( )[ PointerToRawData : PointerToRawData + length ]
    with open( final_bin_file , 'wb' ) as f :
        f.write(buf)
    exit( )
