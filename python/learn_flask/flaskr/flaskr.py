import os
import sqlite3
from flask import Flask , request , session , g , redirect , url_for , abort , render_template , flash

app = Flask( __name__ )

def connect_db( ) :
    rv = sqlite3.connect(app.config['DATABASE'])
    rv.row_factory = sqlite3.Row
    return rv

def init_db():
    with app.app_context() :
        db = get_db( )
        with app.open_resource('schema.sql',mode='r') as f :
            db.cursor().executescript(f.read())
        db.commit( )

if __name__ == '__main__' :
    app.run( )
