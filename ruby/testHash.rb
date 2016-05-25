#!/usr/bin/env ruby

hsh = colors = { 'red' => 0xf00 , 'green' => 0x0f0 , 'blue' => 0x00f , }
#hsh.each do | key , value |
for key , value in hsh do
    print key, " is ", value , "\n"
end
