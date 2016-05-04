$( "document" ).ready( function( ) {
    $(".leave_message").submit( function( ) {
        var data = {
            'action' : 'leave_message' ,
        } ;
        data = $(this).serialize() + "&" + $.param( data ) ;
        $.ajax({
            type: "POST",
            dataType: "json",
            url: "handle_request.php",
            data: data,
            success: function(data) {
                var message_html = [] ;
                message_html.push(
                    data["message"]["ip"]
                    + " At " + data["message"]["time"] 
                    + " leaves message : " + data["message"]["content"] ) ;
                message_html.push( "<br/>" ) ;
                $( ".message_area" ).html( message_html.join( "" ) + $( ".message_area" ).html( ) ) 
            }
        });
        return false ;
    } ) ;
} ) ;