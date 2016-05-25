#!/bin/bash

process_exists()
{
    pidof "$1" && echo "These $1 process(es) were found." || \
        echo "No $1 processes were found."
}

echo "Checking system processes..."
process_exists apache2 | tee apache2.log
process_exists mysqld | tee mysql.log
process_exists squid | tee squid.log
