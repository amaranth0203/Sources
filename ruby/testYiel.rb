#!/usr/bin/env ruby

def test
    puts " You are in the method "
    yield
    puts " You are again back to the method "
    yield
end
test{
    puts " You are in the block "
} 

def test_( & aaa )
    aaa.call
end
test_ {
    puts "hello world"
}
