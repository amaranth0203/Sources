# !/usr/bin/env ruby -w

$:.unshift '.'
require 'launcher'

def help
    print "
  You must pass in the path to the file to launch.

  Usage: ${__FILE__} target_file
"
end

unless ARGV.size > 0
    help
    exit
end

app_map = {
    'rb' => 'notepad'
}

launcher = Launcher.new app_map
target = ARGV.join ' '
launcher.run target