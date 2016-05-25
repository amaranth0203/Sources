<?php
    if (is_ajax()) {
        if (isset($_POST["action"]) && !empty($_POST["action"])) { 
            $action = $_POST["action"];
            switch($action) { 
            case "test" : test( ) ; break ;
            }
        }
    }
    function is_ajax() {
        return isset($_SERVER['HTTP_X_REQUESTED_WITH']) && strtolower($_SERVER['HTTP_X_REQUESTED_WITH']) == 'xmlhttprequest';
    }
    function test( ) {
        static $ids = array( ) ;
        if( !isset( $_COOKIE[ 'id' ] ) or !in_array( $_COOKIE[ 'id' ] , $ids ) ) {
            $id = rand( ) ;
            while( in_array( $id , $ids ) ) {
                $id = rand( ) ;
            }
            $ids[] = $id ;
            setcookie( 'id' , $id ) ;
            $_COOKIE[ 'id' ] = $id ;
        }
        $return = $_POST;
        $return[ "id" ] = $_COOKIE[ 'id' ] ;
        $return["json"] = json_encode($return);
            error_log( $_COOKIE[ 'id' ] ) ;
            error_log( $return[ 'id' ] ) ;
            error_log( print_R( $ids , TRUE ) ) ;
        echo json_encode($return);
    }
?>