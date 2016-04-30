
    function loadDoc( ) {
        var xhttp , xmlDoc , txt , x, i ;
        xhttp = new XMLHttpRequest( ) ;
        xhttp.onreadystatechange = function( ) {
            if( xhttp.readyState == 4 && xhttp.status == 200 ) {
                xmlDoc = xhttp.responseXML ;
<!-- alert( xmlDoc.getElementsByTagName( 'a' )[0].childNodes[0].nodeValue ) ; -->
                document.getElementById( "demo" ).innerHTML = xmlDoc.getElementsByTagName( "a" )[0].childNodes[0].nodeValue ;
            }
        } ;
        xhttp.open( "GET" , "sometxt.php?a=tttttt" , true ) ;
        xhttp.send( ) ;
    }