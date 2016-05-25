#!/usr/bin/env ruby

require "dbi"

begin
    dbh = DBI.connect( "DBI:Mysql:TESTDB:localhost" , "root" , "" )
    sth = dbh.prepare( "UPDATE EMPLOYEE SET AGE = AGE + 1 WHERE SEX = ?" )
    sth.execute( 'M' )
    sth.finish
    dbh.commit
rescue DB::DatabaseError => e
    puts "An error occurred"
    puts "Error code :      #{e.err}"
    puts "Error message :   #{e.errstr}"
    dbh.rollback
ensure
    dbh.disconnect if dbh
end
