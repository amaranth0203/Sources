var spawn = require( 'child_process' ).spawn ,
    grep = spawn( 'echo' , ['ssh'] ) ;

grep.on( 'close' , function( code , signal ) {
    console.log( 'child process terminated due to receipt of signal ' + signal ) ;
}) ;

grep.kill( 'SIGTERM' ) ;


var cp = require( 'child_process' ) ;
var n = cp.fork( __dirname + '/sub.js' ) ;
n.on( 'message' , function( m ) {
    console.log( 'PARENT got message : ' + m ) ;
} ) ;

n.send( { hello : 'world' } ) ;