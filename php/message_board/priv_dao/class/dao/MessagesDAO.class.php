<?php
/**
 * Intreface DAO
 *
 * @author: http://phpdao.com
 * @date: 2016-05-03 07:53
 */
interface MessagesDAO{

	/**
	 * Get Domain object by primry key
	 *
	 * @param String $id primary key
	 * @Return Messages 
	 */
	public function load($id);

	/**
	 * Get all records from table
	 */
	public function queryAll();
	
	/**
	 * Get all records from table ordered by field
	 * @Param $orderColumn column name
	 */
	public function queryAllOrderBy($orderColumn);
	
	/**
 	 * Delete record from table
 	 * @param message primary key
 	 */
	public function delete($id);
	
	/**
 	 * Insert record to table
 	 *
 	 * @param Messages message
 	 */
	public function insert($message);
	
	/**
 	 * Update record in table
 	 *
 	 * @param Messages message
 	 */
	public function update($message);	

	/**
	 * Delete all rows
	 */
	public function clean();

	public function queryByTime($value);

	public function queryByContent($value);

	public function queryByIp($value);


	public function deleteByTime($value);

	public function deleteByContent($value);

	public function deleteByIp($value);


}
?>