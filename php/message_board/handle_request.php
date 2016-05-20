<?php
    if (is_ajax()) {
        if (isset($_POST["action"]) && !empty($_POST["action"])) { 
            $action = $_POST["action"];
            switch($action) { 
            case "get_messages" : get_messages( ) ; break ;
            case "leave_message" : leave_message( ) ; break ;
            }
        }
    }
    function is_ajax() {
        return isset($_SERVER['HTTP_X_REQUESTED_WITH']) && strtolower($_SERVER['HTTP_X_REQUESTED_WITH']) == 'xmlhttprequest';
    }
    function get_messages( ) {
        $return = $_POST;
        require_once( $_SERVER['DOCUMENT_ROOT'].'/priv_handler/get_messages.php' ) ;
        $return["messages"] = $messages ;
        echo json_encode($return);
    }
    function leave_message( ) {
        $return = $_POST;
        require_once( $_SERVER['DOCUMENT_ROOT'].'/priv_handler/leave_message.php' ) ;
        $return["message"] = $message ;
        $return["json"] = json_encode($return);
        echo json_encode($return);
    }
?>