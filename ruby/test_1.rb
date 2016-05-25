#!/usr/bin/env ruby

(1..5).each do |i|
    ( 1..5 ).each do | j |
        ( 1..5 ).each do | k |
            if ( i != j )&&( j != k )&&( k != i )
                puts i.to_s + j.to_s + k.to_s
end ; end ; end ; end

#for i in 1...5
#    for j in 1...5
#        for k in 1...5
#            if ( i != j )&&( j != k )&&( i != k )
#                puts i.to_s + j.to_s + k.to_s
#            end
#        end
#    end
#end
