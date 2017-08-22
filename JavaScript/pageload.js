var page = require( 'webpage' ).create( ) ;
console.log( '[+] wassup' ) ;
page.open( 'http://www.baidu.com' , function( status ) {
    console.log( 'Status:' + status ) ;
    if( status === 'success' ) {
        page.render( 'example.png' ) ;
    }
    console.log( '[+] wassup after if' ) ;
    phantom.exit( ) ;
} ) ;
