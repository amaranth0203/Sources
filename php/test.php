<?php

    function my_callback_function( ) {
        echo 'hello world' ;
    }

    class MyClass {
        static function myCallbackMethod( ) {
            echo 'Hello World' ;
        }
    }

    call_user_func( 'my_callback_function' ) ;

    call_user_func( [ "MyClass" , "myCallbackMethod" ] ) ;

    $obj = new MyClass( ) ;
    call_user_func( [ $obj , "myCallbackMethod" ] ) ;

    call_user_func( "MyClass::myCallbackMethod" ) ;

    $var = "\naaa\n" ;

    echo $var ;

    var_dump( $var ) ;

    function test( ) {
        static $count = 0 ;
        $count ++ ;
        echo $count ;
        if( $count < 10 ) {
            test( ) ;
        }
        // $count -- ;
        echo $count ;
    }

    test( ) ;

    $obj = new stdclass ;

    $a = array( "a" => "apple" , "b" => "banana" ) ;
    $b = array( "a" => "pear" , "b" => "strawberry" , "c" => "cherry" ) ;
    $c = $a + $b ;
    var_dump( $c ) ;
    
?>