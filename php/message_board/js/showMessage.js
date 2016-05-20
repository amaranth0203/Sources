$( "document" ).ready( function( ) {
    var data = {
        'action' : 'get_messages' ,
    } ;
    data = $(this).serialize() + "&" + $.param( data )
    $.ajax({
        type: "POST",
        dataType: "json",
        url: "handle_request.php",
        data: data,
        success: function(data) {
            var message_html = [] ;
            $.each( data["messages"] , function( index , message ) {
                message_html.push(
                    message["ip"]
                    + " At " + message["time"] 
                    + " leaves message : " + message["content"] ) ;
                message_html.push( "<br/>" ) ;
            } ) ;
            $( ".message_area" ).html( message_html.join( "" ) ) ;
        }
    });
} ) ;