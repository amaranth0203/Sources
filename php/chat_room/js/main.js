$( "document" ).ready( function( ) {
            $( ".messages" ).val( 'message1\nmessage2\nmessage3\n' ) ;
            $( ".users" ).val( 'user1\nuser2\nuser3\n' ) ;
    test( )
} ) ;


function test( ) {
    function inc( ) {
        var data = {
            'action' : 'test' ,
        } ;
        $.ajax( {
            type: "POST" ,
            async: false ,
            dataType: "json" ,
            url: "handle_request.php" ,
            data: data ,
            success: function( data ) {
                alert( data["json"] ) ;
            }
        } ) ;
        // setTimeout( inc , 1000 ) ;
    } ;
    inc( )
}
