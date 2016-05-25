import java.io.* ;
import javax.servlet.* ;
import javax.servlet.http.* ;

public class HelloWorld extends HttpServlet
{
    private String message ;
    public void init( ) throws ServletException
    {
        message = "Hello world__" ;
    }
    public void doGet( HttpServletRequest request , HttpServletResponse response )
        throws ServletException , IOException
    {
        response.setContentType( "text/html" ) ;
        PrintWriter out = response.getWriter( ) ;
        out.println( "<h1>" + message + "</h1>" ) ;
    }
    public void destroy( )
    {
    }
}
