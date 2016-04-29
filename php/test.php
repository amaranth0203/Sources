<?php

    

exit( ) ;

    trait Counter {
        public function inc() {
            static $c = 0;
            $c = $c + 1;
            echo "$c\n";
        }
    }
    
    class C1 {
        use Counter;
    }
    
    class C2 {
        use Counter;
    }
    
    $o = new C1(); $o->inc(); // echo 1
    $o = new C1(); $o->inc(); // echo 2
    $p = new C2(); $p->inc(); // echo 1

    class Foo {
        public static $my_static = 'foo' ;
        public function staticValue( ) {
            return self::$my_static ;
        }
    }
    class Bar extends Foo {
        public function fooStatic( ) {
            return parent::$my_static ;
        }
    }

    print Foo::$my_static . "\n" ;

    $foo = new Foo ;
    print $foo->staticValue( )."\n" ;
    print $foo->my_static."\n" ;

    print $foo::my_static."\n" ;
    $classname = "Foo" ;
    print $classname::$my_static."\n" ;
    

    $message = "hello" ;

    $example = function( ) {
        var_dump( $message ) ;
    } ;

    $example( ) ;

    $example = function( ) use ( $message ) {
        var_dump( $message ) ;
    } ; 

    $example( ) ;

    $message = "world" ;

    $example( ) ;

    $message = "hello" ;

    $example = function( ) use ( &$message ) {
        var_dump( $message ) ; 
    } ;

    $example( ) ;

    $message = "world" ;

    $example( ) ;

    class C { } ;

    function getC( ) {
        return new C ;
    }

    var_dump( getC( ) ) ;

    list( $b , $a ) = [ 1 , 2 , 3 ] ;

    echo $a , $b ;

    function test_( $a , $b ) {

        echo $a , $b ;

        return ;
    }

    $a = [ 1 , 2 , 3 ] ;

    test_( ...$a ) ;

    echo 'before : '.$color.' '.$fruit."\n" ;

    // include 'vars.php' ;
    require 'vars.php' ;

    echo 'after  : '.$color.' '.$fruit."\n" ;

    for( $i = 0 ; $i < 10 ; $i ++ )
        for( $j = 0 ; $j < 10 ; $j ++ )
        {
            print $i.$j."\n" ;
            if( $j > 3 ) break 2 ;
        }

    $array = [
        [ 1 , 2 , 3 ] ,
        [ 4 ] ,
    ] ;

    foreach( $array as list( $a , $b ) ) {
        echo " a : $a , b : $b\n" ;
    }

    $arr = array( 1 , 2 , 3 , 4 ) ;

    while( list( , $value ) = each( $arr ) ) {
        echo $value."\n" ;
    }

    $arr = array( 1 , 2 , 3 , 4 ) ;

    print_r( $arr ) ;

    foreach( $arr as $value ) {
        $value *= 2 ;
    }

    print_r( $arr ) ;

    foreach( $arr as &$value ) {
        $value *= 2 ;
    }

    print_r( $arr ) ;

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