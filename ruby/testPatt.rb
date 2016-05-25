#!/usr/bin/env ruby

line1 = "Cats are smater than dogs" 
line2 = "Dogs also like meat"

if ( line1 =~ /Cats(.*)/ )
    puts "Line1 contants Cats"
end
if ( line2 =~ /Cats(.*)/ )
    puts "Lines contants Cats"
end
puts "" ; puts ""

phone = "2004-959-559 #This is a Phone Number"
#phone = phone.sub!( /#.*$/ , "" )
phone.sub!( /#.*$/ , "" )
puts "Phone Num : #{ phone } "
#phone = phone.gsub!( /\D/ , "" )
phone.sub!( /#.*$/ , "" )
puts "Phone Num : #{ phone } "

