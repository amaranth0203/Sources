'''
当commit()时出现
AttributeError: 'MySQLConnection' object has no attribute 'get_characterset_info'
的时候，
先执行下边的代码即可。。。
估计是MySql 5.5.x 和MySql 5.6.x的差异
'''
from mysql.connector.connection import MySQLConnection
MySQLConnection.get_characterset_info = MySQLConnection.get_charset


'''
另一种解决方法
换个驱动
'''

engine = create_engine( 'mysql+mysqldb://root:passwd@localhost:3306/test' )
