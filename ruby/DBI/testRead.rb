#!/usr/bin/env ruby

require "dbi"

begin
    dbh = DBI.connect( "DBI:Mysql:TESTDB:localhost" , "root" , "" )
    sth = dbh.prepare( "SELECT * FROM EMPLOYEE WHERE INCOME > ? " )
    sth.execute( 1000 )
    sth.fetch do | row |
        printf "First Name : %s , Last Name : %s \n" , row[0] , row[1]
        printf "Age : %d , Sex : %c \n" , row[2] , row[3]
        printf "Salary : %d \n\n" , row[4]
    end
    sth.finish
rescue DB::DatabaseError => e
    puts "An error occurred"
    puts "Error code :      #{e.err}"
    puts "Error message :   #{e.errstr}"
ensure
    dbh.disconnect if dbh
end
