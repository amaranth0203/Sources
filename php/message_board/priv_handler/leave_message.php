<?php
    require_once( $_SERVER['DOCUMENT_ROOT'].'/priv_dao/include_dao.php' ) ;
    $ip = '' ;
    if( !empty( $_SERVER['HTTP_CLIENT_IP'] ) ) {
        $ip = $_SERVER['HTTP_CLIENT_IP'] ;
    } else if ( !empty( $_SERVER['HTTP_X_FORWARDED_FOR'] ) ) {
        $ip = $_SERVER['HTTP_X_FORWARDED_FOR'] ;
    } else {
        $ip = $_SERVER['REMOTE_ADDR'] ;
    }
    date_default_timezone_set( 'ASIA/SHANGHAI' ) ;
    $time = date( 'Y-m-d H:i:s ' , time( ) ) ;

    $transaction = new Transaction();
    $message = new Message( ) ;
    $message->time = $time ;
    $message->ip = $ip ;
    $message->content = $_POST['message'] ;
    DAOFactory::getMessagesDAO()->insert($message);
    $transaction->commit();
    
?>