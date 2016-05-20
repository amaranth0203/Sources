<?php
    require_once( $_SERVER['DOCUMENT_ROOT'].'/priv_dao/include_dao.php' ) ;
    // $ip = '' ;
    // if( !empty( $_SERVER['HTTP_CLIENT_IP'] ) ) {
        // $ip = $_SERVER['HTTP_CLIENT_IP'] ;
    // } else if ( !empty( $_SERVER['HTTP_X_FORWARDED_FOR'] ) ) {
        // $ip = $_SERVER['HTTP_X_FORWARDED_FOR'] ;
    // } else {
        // $ip = $_SERVER['REMOTE_ADDR'] ;
    // }
    // date_default_timezone_set( 'ASIA/SHANGHAI' ) ;
    // $time = date( 'Y-m-d H:i:s ' , time( ) ) ;

    // print $ip.' -- ip<br/>' ;
    // print $time.' -- time<br/>' ;

    // $transaction = new Transaction();
    // $message = new Message( ) ;
    // $message->time = $time ;
    // $message->ip = $ip ;
    // $message->content = 'test' ;
    // DAOFactory::getMessagesDAO()->insert($message);
    // $transaction->commit();
    $messages = DAOFactory::getMessagesDAO()->queryAllOrderBy('time');
    // foreach( $messages as $message ) {
        // print $message->time." ".$message->ip." ".$message->content."<br/>" ;
    // }
    
?>