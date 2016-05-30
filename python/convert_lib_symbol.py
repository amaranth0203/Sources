'''
    with vs dev env run : 
    [ dumpbin -headers user32.lib | findstr /c:" Symbol name" > user32.txt ]
    [ dumpbin -headers kernel32.lib | findstr /c:" Symbol name" > kernel32.txt ]
    [ dumpbin -headers gdi32.lib | findstr /c:" Symbol name" > gdi32.txt ]
    then generate a file named qyh_keyword.c for ctags
    then while coding with win32asm
    can auto complete function with this ctags by C-p

                                            2016.05.30 17:40
'''



fnames = ( "user32.txt" , "kernel32.txt" , "gdi32.txt" )
buff = []

for fname in fnames :
    buff.append( "// "+fname.split('.')[0]+".lib start" )
    with open( fname , "r" ) as f :
        for line in f :
            line = line.strip( )
            start = line.find( ': _' ) + 3
            if 'A@' in line :
                end = line.rfind( 'A@' )
            elif 'W@' in line :
                end = line.rfind( 'W@' )
            else :
                end = line.rfind( '@' )
            buff.append( '#define ' + line[start:end] )
    buff.append( "// "+fname.split('.')[0]+".lib end\n\n" )
print "\n".join( buff )

with open( "qyh_keywords.c" , "w" ) as f :
    f.write( "\n".join( buff ) )