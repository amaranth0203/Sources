#!/usr/bin/env ruby

require 'dbi'
begin
    dbh = DBI.connect( "DBI:Mysql:TESTDB:localhost" , "root" , "" )
    puts dbh.func( :client_info )
    puts dbh.func( :client_version )
    puts dbh.func( :host_info )
    puts dbh.func( :proto_info )
    puts dbh.func( :server_info )
    puts dbh.func( :thread_id )
    puts dbh.func( :stat )
rescue DBI::DatabaseError => e
    puts "An error occurred"
    puts "Error code :      #{e.err}"
    puts "Error message :   #{e.errstr}"
ensure
    dbh.disconnect if dbh
end
