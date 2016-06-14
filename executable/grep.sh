#!/bin/bash
#引用公用shell函数
funcs=`which functions.sh`
echo "funcs=$funcs"
. $funcs

#不搜索二进制文件，全字匹配，显示行号，不搜索～的临时文件
export GREP_OPTIONS='-R --exclude-dir=.svn --exclude-dir=.git  --exclude=*~  --binary-files=without-match  --color=always  -n -H '
export GREP_COLOR='1;31'
src_str=$2
search_dir=$1
if test x"$src_str" = x || test x"$search_dir" = x ; then
	echoColor "$0   dir strings [file type]" RED
	exit 
fi

if test x"$3" = x ; then
        if test -d "$search_dir"; then
  	    grep -R --exclude-dir=.svn --exclude-dir=.git "$src_str" $search_dir
	    echo;echoColor "Results by execute cmd: grep ${GREP_OPTIONS} $src_str $search_dir" GREEN
        else
            grep "$src_str" $search_dir -n -H
            echo;echoColor "Results by execute cmd: grep ${GREP_OPTIONS} $src_str $search_dir" GREEN
        fi
else	
	echo;echoColor "Results by execute cmd: grep ${GREP_OPTIONS} $src_str find $search_dir -name \"$3\"" GREEN  	
	fileslist=`find $search_dir -name "$3"`		
	if test x"$fileslist" = x ; then
	    echoColor " fies be searched is empty! not exec grep comand ,now exit!" RED
	    exit;
	else
	    echoColor "＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊ files below will be search!＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊" GREEN ;
	    echoColor "$fileslist" YELLOW
	    echo 
	    echoColor "＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊ begin to exec grep comand!＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊" GREEN ;
	    grep ${GREP_OPTIONS} "$src_str" `find $search_dir -name "$3"`
	fi
	echo
	echoColor "＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊ ＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊" GREEN ;
	echoColor "  search string \"$src_str\" in all \"$3\" files                                                          " RED
	echoColor "  Results by execute cmd: grep -R  --exclude-dir=.svn --exclude-dir=.git $src_str \`find $search_dir -name \"$3\"\`" RED
	echoColor "＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊" GREEN ;
	
fi
# find 和 grep 结合使用
# find -name "*.txt" -print -exec grep test {} \; 
# find -name "*.txt" -print -exec grep test {} \;
#参考文档：
#grep 匹配和 gdb的多文件设置断点调试
#http://hi.baidu.com/honinbou/blog/item/00ea9bb410ee667a8bd4b2f9.html
#grep 参数 详解
#http://tiandike.bokee.com/6346696.html