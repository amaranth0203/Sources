#!/usr/bin/env ruby

class Example
    VAR1 = 100
    VAR2 = 200
    def show
        puts " Value of first Constant is #{VAR1}"
        puts " Value of second Constant is #VAR2"
    end
end

object = Example.new( )
object.show( )
