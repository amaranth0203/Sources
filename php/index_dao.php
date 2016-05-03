<?php

	//include all DAO files
	require_once('include_dao.php');
		
	//start new transaction
	$transaction = new Transaction();
	
	$group = new Group();
	$group->name = "administrator";
	DAOFactory::getGroupsDAO()->insert($group);
	echo 'generated id = '.$group->id.'<br/>';	

	//load row where primary key equal 3
	$module = DAOFactory::getModulesDAO()->load(3);
	echo $module->title.'<br/>';;
	
	$module->content = "Sample content";
	//update row
	DAOFactory::getModulesDAO()->update($module);
	
	//delete row where primary key equal 10
	$rowsDeleted = DAOFactory::getSessionDAO()->delete(10);
	echo 'rows deleted = '.$rowsDeleted.'<br/>';
		
	//commit transaction
	$transaction->commit();

	echo '<br/><br/>';
	
	//print all rows order by title
	$arr = DAOFactory::getCategoriesDAO()->queryAllOrderBy('title');
	for($i=0;$i<count($arr);$i++){
		$row = $arr[$i];
		echo $row->id.' '.$row->title.'<br/>';
	}
	
	echo '<br/><br/>';
	
	//print all rows where column 'showtitle'  is equal '0'
	$arr = DAOFactory::getModulesDAO()->queryByShowtitle('0');
	for($i=0;$i<count($arr);$i++){
		$row = $arr[$i];
		echo $row->id.' '.$row->title.'<br/>';
	}
	
	echo '<br/><br/>';

	//print all rows where content like 'Mambo' and creted_by is equal 62
	$arr = DAOFactory::getContentDAO()->queryByContentAndCreatedBy('Mambo', 62);
	for($i=0;$i<count($arr);$i++){
		$row = $arr[$i];
		echo $row->id.' '.$row->title.'<br/>';
	}
	
	echo '<br/><br/>';
	
	$tab = DAOFactory::getContentDAO()->getUserNameAndContentTitle();
	for($i=0;$i<count($tab);$i++){
		echo $tab[$i]->username.' : '.$tab[$i]->title.'<br/>';
	}
	
	echo '<br/><br/>';
	echo DAOFactory::getContentDAO()->getCountByCreatedBy(62);
	
	echo '<br/><br/>';
	
	//start new transaction
	$transaction = new Transaction();

	//delete rows where content equal 'cnt'
	$rowsDeleted = DAOFactory::getModulesDAO()->deleteByShowtitle('1');
	echo 'rows deleted = '.$rowsDeleted.'<br/>';
	

	
	//clean table
	DAOFactory::getContentDAO()->clean();
	$transaction->rollback();
	
	//Examples how use complex primary keys
	$ptt = DAOFactory::getPlayersToTeamsDAO()->load(8,2);
	echo $ptt->name.'<br/>';
	$ptt->name = "new name";
	DAOFactory::getPlayersToTeamsDAO()->update($ptt);
	
	$rowsDeleted = DAOFactory::getPlayersToTeamsDAO()->delete(8,3);
	echo 'rows deleted = '.$rowsDeleted.'<br/>';
	
	$p = new PlayersToTeam();
	$p->teamId=13;
	$p->playerId=12;
	DAOFactory::getPlayersToTeams2DAO()->insert($p);
?>