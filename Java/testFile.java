import java.io.* ;
public class testFile
{
    public static void main( String[] args )
    {
        FileWriter fw = null ;
        FileOutputStream fos = null ;
        int a = 97 ;
        try
        {
            fw = new FileWriter( "./fw.txt" ) ;
            fos = new FileOutputStream( "./fos.txt" ) ;
            for( int c = 0 ; c <= 10000 ; c ++ )
                fw.write( c ) ;
            fos.write( a ) ;
            fw.close( ) ;
            fos.close( ) ;
        }
        catch( Exception e )
        {
            System.out.println( "[-] exception " ) ;
        }
    }
}
