var http = require( 'http' ) ;

var server = http.createServer( function( request , response ) {

    console.log( "\n\n--------begin--------\n" )

    console.log( request.method + ' ' + request.url ) ;
    // console.log( request.rawHeaders ) ;
    console.error( request.headers ) ;

    var body = [] ;
    request.on( 'error' ,function( err ) {
        console.error( err )
    }).on( 'data' , function( chunk ) {
        body.push( chunk ) ;
    }).on( 'end' , function() {
        body = Buffer.concat( body ).toString( ) ;
        console.log( body ) ;

        response.on( 'error' , function( err ) {
          console.error( err ) ;
        });

        response.writeHead( 200 , {
            'Content-Type' : 'application/json' ,
            'X-Powered-By' : 'bacon' ,
        }) ;

        response.end( JSON.stringify( {
            headers : request.headers ,
            method : request.method ,
            url : request.url ,
            body : body ,
        })) ;
    }) ;

    console.log( "\n---------end---------\n\n" )

}).listen( 80 ) ;


