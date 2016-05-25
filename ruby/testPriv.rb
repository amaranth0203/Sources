#!/usr/bin/env ruby

class Box
    def initialize( w , h )
        @width , @height = w , h
    end
    def getArea
        getWidth * getHeight
    end
    def getWidth
        @width
    end
    private :getWidth
    def getHeight
        @height
    end
    private :getHeight
    def printArea
        @area = getWidth( ) * getHeight
        puts "Big box area is : #{@area}"
    end
    protected :printArea
end

box = Box.new( 10 , 20 )

a = box.getArea( )
puts "Area of the box is : #{a}"
#box.printArea( )
