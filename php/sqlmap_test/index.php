<?php
mysql_connect('localhost','root','echo');
mysql_select_db('test');
?>

<form action="" method="get">
<table width="50%">
    <tr>
        <td>User</td>
        <td><input type="text" name="user"></td>
    </tr>
    <tr>
        <td></td>
        <td><input type="text" name="password"></td>
    </tr>
</table>
    <input type="submit" value="OK" name="s">
</form>

<?php
if($_GET['s']){
    $user = $_GET['user'];
    $pass = $_GET['password'];     
    $re = mysql_query("select * from test_user where user_name = '$user' and password = '$pass'");

    if(mysql_num_rows($re) == 0){       
        echo '0';
    }else{
        echo '1';
    }
}
?>