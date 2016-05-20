<?php
    require_once( $_SERVER['DOCUMENT_ROOT'].'/priv_dao/include_dao.php' ) ;
    $messages = array_reverse( DAOFactory::getMessagesDAO()->queryAllOrderBy('time') ) ;
?>