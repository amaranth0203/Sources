#!/bin/sh
#echo $SHELL
#while :
#do
#    echo `date` >> test.txt
#    sleep 2
#done
#
#cd /root
#touch abcdefg
#
#echo "[+] working directory : `pwd`"
#cd /root
#echo "[+] working directory : `pwd`"

function func_test()
{
    echo aaa | tee tmp.txt
}

func_test
