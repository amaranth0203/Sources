#!/usr/bin/env ruby
# encoding : utf-8

def dump_jpeg

    cmd = 'adb devices'
    puts "[!] " + cmd
    ret = `#{cmd}`
    puts ret

    if not ret[24..-1].include? 'device' 
        abort "[-] device not attached\n"
    else
        puts "[+] devices found\n"
    end
    puts "\n\n"

    cmd = 'adb shell ls /sdcard/'
    cmd += 37929.chr('utf-8')
    cmd += 21578.chr('utf-8')
    cmd += 28288.chr('utf-8')
    puts "[!] " + cmd
    ret = `#{cmd}`
    puts ret

    jpegs = []

    ret.split().each do |filename|
        jpegs.push '/sdcard/' + 37929.chr('utf-8') + 21578.chr('utf-8') + 28288.chr('utf-8') + '/' + filename
    end

    puts "------------------ file(s) to process start ------------------" 
    jpegs.each do |filename|
        puts filename
    end
    puts "------------------ file(s) to process end --------------------\n\n" 

    jpegs.each do |filename|
        cmd = 'adb pull '
        cmd += filename
        cmd += ' .'
        puts "[!] " + cmd
        ret = `#{cmd}`
        puts ret
    end

    jpegs.each do |filename|
        cmd = 'adb shell rm '
        cmd += filename
        puts "[!] " + cmd
        ret = `#{cmd}`
        puts ret
    end

end





dump_jpeg
