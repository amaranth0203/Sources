import java.io.* ;
import javax.servlet.* ;
import javax.servlet.http.* ;

public class HelloForm extends HttpServlet
{
    public void doGet(  HttpServletRequest request ,
                        HttpServletResponse response )
        throws ServletException , IOException
    {
        response.setContentType( "text/html" ) ;
        PrintWriter out = response.getWriter( ) ;
            String title = "Try method GET to get info from form" ;
        String docType = "<!doctype html public \"-//w3c//dtd html 4.0 " + "transitional//en\">\n" ;
        out.println(    docType +
                        "<html>\n" +
                        "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">" +
                        "<head><title" + title + "</title></head>\n" +
                        "<bdy bgcolor=\"#f0f0f0\">\n" +
                        "<h1 align=\"center\">" + title + "</h1>\n" +
                        "<ul>\n" +
                        "  <li><b>first name</b>:"
                        + request.getParameter( "first_name" ) + "\n" +
                        "  <li><b>last name</b>:"
                        + request.getParameter( "last_name" ) + "\n" +
                        "</ul>\n" +
                        "</body></html>" ) ;
    }
}
