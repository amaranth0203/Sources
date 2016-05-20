<?php

/**
 * DAOFactory
 * @author: http://phpdao.com
 * @date: ${date}
 */
class DAOFactory{
	
	/**
	 * @return MessagesDAO
	 */
	public static function getMessagesDAO(){
		return new MessagesMySqlExtDAO();
	}


}
?>