# -*- coding: utf-8 -*-

from email.mime.text import MIMEText

from email import encoders
from email.header import Header
from email.utils import parseaddr , formataddr

import smtplib

def _format_addr( s ) :
    name , addr = parseaddr( s )
    return formataddr( ( \
        Header( name , 'utf-8' ).encode( ), \
        addr.encode( 'utf-8' ) if isinstance( addr , unicode ) else addr ) )

from_addr = '18775800048@163.com'
password = 'FOR1631212'
smtp_server = 'smtp.163.com'
#to_addr = '845687884@qq.com'
to_addr = u'陆济民@互联网.中国'.encode( 'utf8' )
#to_addr = u'陆济民@互联网.中国'.encode( 'gb2312' )

#msg = MIMEText( 'hello , send by Python...' , 'plain' , 'utf-8' )
msg = MIMEText( '<html><body><h1>hello</h1>' +
                '<p>send by <a href="http://www.python.org">Python</a>...</p>' +
                '</body></html>' , 'html' , 'utf-8' )
msg[ 'From' ] = _format_addr( u'Python爱好者 <%s>' % from_addr )
msg[ 'To' ] = _format_addr( u'Ending <%s>'.encode( 'utf8' ) % to_addr )
msg[ 'Subject' ] = Header( u'来自SMTP的问候……' , 'utf-8' ).encode( )

server = smtplib.SMTP( smtp_server , 25 )
server.set_debuglevel( 1 )
server.login( from_addr , password )
server.sendmail( from_addr , [ to_addr ] , msg.as_string( ) )
server.quit( )
