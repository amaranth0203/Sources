from Tkinter import *
import ImageTk
import tkMessageBox

class Application( Frame ) :
    def __init__( self , master = None ) :
        Frame.__init__( self , master )
        self.pack( )
        self.createWidgets( )
        self.images = glob( "/root/psu.jpg" ) 
        self.image = PhotoImage( )
        self.testLabel = Label( self , image=self.image ) ;
        self.testLabel.grid( column = 1 , row = 1 )

    def createWidgets( self ) :
        self.nameInput = Entry( self )
        self.nameInput.pack( )
        self.quitButton = Button( self , text = 'Hello' , command = self.hello )
        self.quitButton.pack( )

    def hello( self ) :
        name = self.nameInput.get( ) or 'World'
        tkMessageBox.showinfo( 'Message' , 'Hello, %s' % name )

 

app = Application( )
app.master.title( 'Hello World____' )
app.mainloop( )
