#!/usr/bin/env ruby

$global_variable = 10

class Class1
    def print_global
        puts "Global variable in Class1 is #{$global_variable}"
end ; end

class Class2
    def print_global
        puts "Global variable in Class2 is #$global_variable"
end ; end

class1obj = Class1.new
class1obj.print_global
class2obj = Class2.new
class2obj.print_global
