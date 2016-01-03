//var array = [ 97 , 98 , 99 , 100 ] ;
var buffer = Buffer( '哈' ) ;
console.log( buffer )
console.log( buffer.toString( 'utf-8' ) ) ;
console.log( buffer.toString( 'ascii' ) ) ;
console.log( '\n\n' ) ;

//str = '\u00bd' ;
str = '嗯' ;
console.log( str + ": " + str.length + " characters, " + 
    Buffer.byteLength( str , 'utf8' ) + " bytes" ) ;
console.log( '\n\n' ) ;

var arr = [ Buffer( '1234' ) , Buffer( '0123' ) ] ;
console.log( arr ) ;
arr.sort( ) ;
console.log( arr ) ;
console.log( '\n\n' ) ;

buffer = new Buffer( 5 ) ;
console.log( buffer.length ) ;
buffer.write( 'abcd' , 1 , 'ascii' ) ;
console.log( buffer ) ;
console.log( buffer.slice( 1 , buffer.length ) ) ;
console.log( buffer.length ) ;
console.log( '\n\n' ) ;

buffer = Buffer( 6 ) ;
buffer.writeUInt16LE( 0x90ab , 0 ) ;
buffer.writeUInt32LE( 0x12345678 , 2 ) ;
console.log( buffer.readUIntLE( 0 , 6 ).toString( ) ) ;
console.log( '\n\n' ) ;

buffer = new Buffer( 'test' ) ;
var json = JSON.stringify( buffer ) ;
console.log( json ) ;
var copy = JSON.parse( json , function( key , value ) {
//    return value && value.type == 'Buffer'
//        ? new Buffer( value.data )
//        : value ;
    if( value.type == 'Buffer' )
        return new Buffer( value.data )
    else
        return value ;
}) ;
console.log( copy.toString( ) ) ;
console.log( '\n\n' ) ;


