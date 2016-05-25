import java.io.* ;
public class testFile2
{
    public static void main( String[] args )
    {
        int c = 0 ;
        int count = 0 ;
        FileReader fr = null ;
        FileInputStream fis = null ;
        try
        {
            fr = new FileReader( "./f.txt" ) ;
            while( ( c=fr.read( ) ) != -1 )
            {
                count ++ ;
                System.out.print((char)c) ;
            }
            System.out.println( "\n[+] count : " + count ) ;
            fr.close( ) ;
            count = 0 ;
            fis = new FileInputStream( "./f.txt" ) ;
            while( ( c =fis.read( ) ) != -1 )
            {
                count ++ ;
                System.out.println((char)c) ;
            }
            System.out.println( "\n[+] count : " + count ) ;
            fis.close( ) ;
        }
        catch( Exception e )
        {
            System.out.println( "[-] exception" ) ;
        }
    }
}
