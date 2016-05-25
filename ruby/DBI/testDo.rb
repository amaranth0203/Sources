#!/usr/bin/env ruby

class A
    @@count = 0
    def initialize( )
        @@count += 1
    end
    def say( )
        puts "[+] My count number : #{@@count} "
    end
end

A.new do | tmp |
    puts "inside do"
    tmp.say( )
end
