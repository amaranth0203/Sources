#!/usr/bin/env ruby

require "dbi"

begin
    dbh = DBI.connect( "DBI:Mysql:TESTDB:localhost" , "root" , "" )
    dbh.do( "INSERT INTO EMPLOYEE(
                FIRST_NAME ,
                LAST_NAME ,
                AGE ,
                SEX ,
                INCOME )
             VALUES( 'Mac' , 'Mohan' , 20 , 'M' , 2000 )" )
    puts "Record has been created"
    dbh.commit
rescue DB::DatabaseError => e
    puts "An error occurred"
    puts "Error code :      #{e.err}"
    puts "Error message :   #{e.errstr}"
    dbh.rollback
ensure
    dbh.disconnect if dbh
end
