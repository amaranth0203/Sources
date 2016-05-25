#!/usr/bin/ruby

BEGIN { puts "Date and time: " + Time.now.to_s }

def bmi( weight , height )
    703.0*( weight.to_f / ( height.to_f**2 ) )
end

my_bmi = bmi( 196 , 73 )

puts "Your BMI is: " + x = sprintf( "%0.2f" , my_bmi )

puts "aaa" + bmi( 196 , 73 ).to_s + "aaa" 

END { puts "You've got some work ahead of you" }
