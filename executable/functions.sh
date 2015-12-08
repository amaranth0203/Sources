#!/bin/bash
functions_base_included=true
#>>>>>>>>>>文本颜色显示相关
#>>>>>>>>>>linux系统相关
#>>>>>>>>>>字符串处理相关 
#function sh_sed_file_d_row_include_xxx() 删除包含特定内容的行
#function sh_sed_file_r_row_include_xxx() 替换包含特定内容的行
#function sh_sed_file_a_row_above_xxx()   在特定内容行上添加内容
#function sh_sed_file_a_row_below_xxx()   在特定内容行下添加内容

#>>>>>>>>>>scm配置管理相关
#>>>>>>>>>>分布式编译相关


#>>>>>>>>>>文本颜色显示相关 start
#function unset_color()
#function echoColor()
#function echoText()        同 echoColor 功能 eg：echoText   "需要显示的内容"  Red | Green | Blue | Purple | Cyan
#function echoTextRed()     显示红色字体   eg: echoTextRed    "需要显示的内容"
#function echoTextGreen()   显示绿色字体   eg: echoTextGreen  "需要显示的内容"
#function echoTextBlue()    显示蓝色字体   eg: echoTextBlue   "需要显示的内容"
#function echoTextPurple()  显示紫色字体   eg: echoTextPurple "需要显示的内容"
#function echoTextCyan()    显示灰色字体   eg: echoTextCyan   "需要显示的内容"
#***************************************************************
#unset 颜色环境变量
#***************************************************************
function unset_color()
{
    unset GREP_OPTIONS 
    unset GREP_COLOR
}

#******************************************************************************
#在shell中使用echo命令输出带颜色的文本
#参考文档：http://hi.baidu.com/lonelycorn/blog/item/5f6a09d8c184fa3332fa1c45.html
#0 透明（使用终端颜色）, 40 黑, 41 红, 42 绿, 43 黄, 44 蓝 45 紫, 46 青绿, 47白（灰）
#前景色（也就是文本的颜色）可以被以下数字替换
#30 黑 31 红, 32 绿, 33 黄, 34 蓝, 35 紫, 36 青绿, 37 白（灰）
#高亮是1，不高亮是0
#注意m后面紧跟字符串。
#*******************************************************************************
function echoColor()
{
        if [ $# != 2 ]
        then
                return 1
        fi
 
        szString=$1
        case "$2" in
                BLACK)
                        szColor=31
                        ;;
                RED)
                        szColor=31
                        ;;
                GREEN)
                        szColor=32
                        ;;
                YELLOW)
                        szColor=33
                        ;;
                BLUE)
                        szColor=34
                        ;;
                PURPLE)
                        szColor=35
                        ;;
                CYAN)
                        szColor=36
                        ;;
                WHITE)
                        szColor=36
                        ;;
                reset)
                        szColor=0
                        ;;

                *)
                        #szColor=39
                        szColor=50
        esac
 
        echo -e "\\033["$szColor"m$szString\\033[39m"
        return 0;
}


#*********************************************************************************
#同 echoColor 功能
#语法 echoText   "需要显示的内容"  Red | Green | Blue | Purple | Cyan
#eg： 用红色显示: "display text in red"
#  echoText "display text in red" Red
#*********************************************************************************
function echoText()
{
        if [ $# != 2 ]
        then
                return 1
        fi
 
        szString=$1
        case "$2" in
                BLACK)
                        szColor=31
                        ;;
                RED)
                        szColor=31
                        ;;
                GREEN)
                        szColor=32
                        ;;
                YELLOW)
                        szColor=33
                        ;;
                BLUE)
                        szColor=34
                        ;;
                PURPLE)
                        szColor=35
                        ;;
                CYAN)
                        szColor=36
                        ;;
                WHITE)
                        szColor=36
                        ;;
                reset)
                        szColor=0
                        ;;

                *)
                        #szColor=39
                        szColor=50
        esac
 
        echo  -e "\\033["$szColor"m$szString\\033[39m"
        return 0;
}

function echoTextRed()
{
    echoText "$1" RED
}

function echoTextGreen()
{
    echoText "$1" GREEN
}

function echoTextBlue()
{
    echoText "$1" BLUE
}


function echoTextPurple()
{
    echoText "$1" PURPLE
}

function echoTextCyan()
{
    echoText "$1" CYAN
}

function echoBegin()
{
for BG in  44m;
do
	echo -en " \033$FG\033[$BG           $1         \033[0m";echo
done
}

function echoEnd()
{

for BG in 45m;
do
	echo -en " \033$FG\033[$BG           $1         \033[0m"
done
}

function echoColorTest()
{
T='gYw' # The test text
echo
echo "        default 40m     41m     42m     43m     44m     45m     46m
47m"
## FGs 为前景(foreground)色, BG 为背景(background)色
for FGs in '    m' '   1m' ' 30m' '1;30m' ' 31m' '1;31m' ' 32m' '1;32m' '
33m' '1;33m' ' 34m' '1;34m' ' 35m' '1;35m' ' 36m' '1;36m' ' 37m' '1;37m'
        do
        FG=$(echo $FGs|tr -d ' ')
        echo -en " $FGs \033[$FG $T "
        for BG in 40m 41m 42m 43m 44m 45m 46m 47m;
                do
                echo -en " \033[$FG\033[$BG $T \033[0m"
        done
        echo
done
echo
}
#>>>>>>>>>>文本颜色显示相关 end


#>>>>>>>>>>交换函数 start
#function question ()
#function sh_menu_select()
#function sh_menu_select2()
#function sh_ls()
#function sh_get_menu_list()


#*********************************************************************************
#获取ubuntu系统版本
# qmode : 1 = yes/no
#         2 = string
#result=`question "创建压缩包，用于软件发布?" 1 no`
#ac_additional_info=`question  "输入补充信息:"  2 `
#*********************************************************************************
function question () {
        unset GREP_OPTIONS 
        unset GREP_COLOR
	local q=$1
	local qmode=$2
	local def="$3"

	if [ "$qmode" = "1" ] ; then
		while [ 1 = 1 ] ; do
			#echo -n "$q [yes/no]" >&2
			echoText "$q [yes/no]" RED >&2
			if [ x"$def" != x ]; then
				echo -n " ($def)" >&2
			fi
			echo -n ": " >&2
			read answer
			if [ "x$answer" = "x" ] ; then
				echo "$def"
				return
			fi
			if [ "$answer" != "yes" ] && [ "$answer" != "no" ] ; then
				echo "Answer 'yes' or 'no' !!" >&2
			else
				echo "$answer"
				return
			fi
		done
	elif [ "$qmode" = "2" ] ; then
		#echo -n "$q" >&2
		echoText "$q" RED>&2
		if [ x"$def" != x ]; then
			echoText "default:($def)" GREEN >&2
			#echo ":($def)" >&2
		fi
		echo -n ": " >&2
		read answer
		if [ "x$answer" = "x" ] ; then
			answer="$def"
		fi
		echo $answer
		return
	fi

	echo "ERROR: With question mode (script internal error) !?!!" >&2
}


function sh_menu_select()
{
    local default_serial_number=1
    local default_value=
    local index=0

    if [ "$1" = "" ]; then
        return
    else
        ac_menu_list=`echo  $1| tr  '\n' ' '`
        if [ "$2" != "" ]; then
            ac_value_list=`echo  $2| tr  '\n' ' '`
        fi
    fi

    #默认值
    default_serial_number=1   
    if [ "$ac_menu_list" != "" ]; then        
        menu_list=($ac_menu_list)
        default_value=${menu_list[0]} 
    else
        echoText "Menu item is empty" RED >&2
        return
    fi

    #value_list=

    if [ "$ac_value_list" != "" ]; then       
        value_list=($ac_value_list)
        default_value=${value_list[0]} 
    fi
    

    echoText "====  Menu items below:   ====" GREEN   >&2
    for i in ${menu_list[@]} 
    do
	((serial_number=${index}+1)) 
	echoText "     ${serial_number}. ${menu_list[${index}]}" GREEN  >&2
        ((index=${index}+1)) 
    done
    echoText "====Select from the list above====" GREEN >&2
    echoText "exmple:  index $index for ${menu_list[${index}-1]}" GREEN >&2
    export ac_choose=
    local ANSWER
    while [ -z $ac_choose ]
    do
        echo -n "Which would you like? [1]" >&2
        if [ -z "$3" ] ; then
            read ANSWER
        else
            ANSWER=$3
        fi

        case $ANSWER in
        "")
            export ac_choose=true
            ANSWER=1
            return_value=${default_value}
	    ;;
        *)
            if IsNumeric $ANSWER;then
            if [ "$ANSWER" -gt "$index" ]; then
                echoText "I didn't understand your response.  Please try again." RED >&2
                echo >&2
            else
                if [ "$value_list" != "" ]; then
                    return_value=${value_list[${ANSWER}-1]}
                else 
                    return_value=${menu_list[${ANSWER}-1]}
                fi	
                export ac_choose=true
            fi
            else
                return_value=$ANSWER
                export ac_choose=true
            fi
            ;;
        esac
        if [ -n "$3" ] ; then
            break
        fi
    done
    if IsNumeric $ANSWER;then
        echoTextGreen "==>  your answer is: ${ANSWER}. ${return_value}" >&2
    else
        echoTextGreen "==> your answer is:  ${return_value}" >&2
    fi 
    result=`echo $return_value| grep ".sh"`
    if [ -z "$result" ]; then
        return_value=`echo  $return_value| tr  '.' ' '`
    fi
    echo "$return_value"
    export return_value
}

function sh_menu_select2()
{
    local default_serial_number=1
    local default_value=
    local index=0

    if [ "$1" = "" ]; then
        return
    else
        ac_menu_list=`echo  $1| tr  '\n' ' '`
        if [ "$2" != "" ]; then
            ac_value_list=`echo  $2| tr  '\n' ' '`
        fi
    fi

    #默认值
    default_serial_number=1   
    if [ "$ac_menu_list" != "" ]; then        
        menu_list=($ac_menu_list)
        default_value=${menu_list[0]} 
    else
        echoText "Menu item is empty" RED >&2
        return
    fi

    #value_list=

    if [ "$ac_value_list" != "" ]; then       
        value_list=($ac_value_list)
        default_value=${value_list[0]} 
    fi
    

    echoText "====  Menu items below:   ====" GREEN   >&2
    for i in ${menu_list[@]} 
    do
	((serial_number=${index}+1)) 
	echoText "     ${serial_number}. ${menu_list[${index}]}" GREEN  >&2
        ((index=${index}+1)) 
    done
    echoText "====Select from the list above====" GREEN >&2
    echoText "exmple:  index $index for ${menu_list[${index}-1]}" GREEN >&2
    export ac_choose=
    local ANSWER
    while [ -z $ac_choose ]
    do
        echo -n "Which would you like? [1]" >&2
        if [ -z "$3" ] ; then
            read ANSWER
        else
            ANSWER=$3
        fi

        case $ANSWER in
        "")
            export ac_choose=true
            ANSWER=1
            return_value=${default_value}
	    ;;
        *)
            if IsNumeric $ANSWER;then
            if [ "$ANSWER" -gt "$index" ]; then
                echoText "I didn't understand your response.  Please try again." RED >&2
                echo >&2
            else
                if [ "$value_list" != "" ]; then
                    return_value=${value_list[${ANSWER}-1]}
                else 
                    return_value=${menu_list[${ANSWER}-1]}
                fi	
                export ac_choose=true
            fi
            else
                return_value=$ANSWER
                export ac_choose=true
            fi
            ;;
        esac
        if [ -n "$3" ] ; then
            break
        fi
    done
    if IsNumeric $ANSWER;then
        echoTextGreen "==>  your answer is: ${ANSWER}. ${return_value}" >&2
    else
        echoTextGreen "==> your answer is:  ${return_value}" >&2
    fi 
    echo "$return_value"
    export return_value
}


#****************************************************************************************
#菜单选择方式列出目录下的项
#sh_ls  arg1 arg2 arg3
#arg1 指向需要操作的目录
#arg2 列出的对象类型: svn repo files
#arg3 当 arg2 类型为svn的情况下有效
#sh_ls ~/workspace/gphone/mtk6573/         #列出 ~/workspace/gphone/mtk6573/ 下的所有目录
#sh_ls ~/workspace/gphone/mtk6573/ files   #列出 ~/workspace/gphone/mtk6573/ 下的所有目录和文件
#sh_ls ~/repos/ repo                       #列出 ~/repos/ 下的svn仓库
#sh_ls http://smartphone/repositories/MT6573/trunk/alps-w1140-20111007 svn [svn_options] 
                                           #列出目录(svn工作拷贝或者svn url)下的svn控制项目
#*****************************************************************************************
function sh_ls()
{
    local default_serial_number=1
    local default_value=
    local index=0
    local ac_list=
    local dir_list=
    local repos_list=

    if [ "$1" = "" ]; then
        return
    else

        if [ "$2" = "svn" ]; then
           ac_list=`svn list  $1  $3| tr  '\n' ' '`
        else
           ac_list=`ls  $1 2>/dev/null | tr  '\n' ' '`
        fi
    fi

    #判断目录  
    for file in $ac_list
    do
        if [ "$2" = "files" ]; then
            break
        fi
        if test -d $1/$file ; then  
            dir_list="$dir_list $file"
            if test -f $1/$file/conf/svnserve.conf ; then
                repos_list="$repos_list $file"
            fi
        fi
    done

    if [ "$2" = "repo" ]; then
        ac_list=$repos_list
    elif [ "$2" = "svn" ]; then
        ac_list=$ac_list
    elif [ "$2" = "files" ]; then
        ac_list=$ac_list
    else
        ac_list=$dir_list
    fi


    if [ "$ac_list" != "" ]; then
        default_serial_number=1   
        targets_list=($ac_list)
        default_value=${targets_list[0]}    
    else
        echoText "Menu item is empty" RED >&2
        return
    fi
     

    echoText "====  Items below $1   ====" GREEN   >&2
    for i in ${targets_list[@]} 
    do
	((serial_number=${index}+1)) 
        echoText "     ${serial_number}. ${targets_list[${index}]}" GREEN  >&2
        ((index=${index}+1)) 
    done
    echoText "====Select from the list above====" GREEN >&2
    echoText "exmple:  index $index for ${targets_list[${index}-1]}" GREEN >&2
    echo >&2

    export ac_choose=
    local ANSWER
    while [ -z $ac_choose ]
    do
        echo -n "Which would you like? [1]" >&2
        if [ -z "$3" ] ; then
            read ANSWER
        else
            ANSWER=$3
        fi

        case $ANSWER in
        "")
            export ac_choose=true
            ANSWER=1
            if test -d $1; then
	        return_value=$1/${default_value}
            else
                return_value=${default_value}
            fi
	    ;;
        *)
            if [ "$ANSWER" -gt "$index" ]; then
                echoText "I didn't understand your response.  Please try again." RED >&2
                echo >&2
            else
                if [ "$2" = "files" ]; then
                    return_value=${targets_list[${ANSWER}-1]}
                elif test -d $1; then
	            return_value=$1/${targets_list[${ANSWER}-1]}
                else
                    return_value=${targets_list[${ANSWER}-1]}
                fi		
                export ac_choose=true
            fi
            ;;
        esac
        if [ -n "$3" ] ; then
            break
        fi
    done
    echoTextGreen "==> your answer is: ${ANSWER}. ${targets_list[${ANSWER}-1]}" >&2
    echo $return_value
    export return_value
}

function sh_get_menu_list()
{
  local keywork1=$1
  local keyword2=$2
  local menu_list=
  if [ -n "$keyword2" ] && [ -n "$keyword2" ]; then
    menu_list=`cat $0|grep $keyword1 |grep $keyword2|awk -F'=' '{print $1}'`
  fi
  echo "$menu_list"
}
function sh_get_funcs_list()
{
  local keywork1=$1
  local keyword2=$2
  local menu_list=
  if [ -n "$keyword2" ] && [ -n "$keyword2" ]; then
    menu_list=`cat $0|grep $keyword1 |grep $keyword2| awk  '{print $2}' |tr -d '()'`
  fi
  echo "$menu_list"
}
#>>>>>>>>>>交换函数 end



#>>>>>>>>>>linux系统相关 start
#function sh_system_is_i386()     #派断是否为 32 位操作系统
#function sh_cpu_support_64()     #判断 CPU 是否支持64位
#function sh_get_ubuntu_version() #获取ubuntu系统版本
#function sh_print_system_info()  #显示 ubuntu 的系统信息
#function sh_dpkg_status()        #判断软件包状态
#function sh_dpkg_install()       #安装软件包
#function sh_dpkg_remove()        #卸载软件包
#function sh_get_date()           #格式化输出系统日期
#function sh_get_time()           #格式化输出系统时间
#function sh_get_date_and_time()  #格式化输出系统日期和时间
#function sh_get_ip_address()     #获取(活动网卡)ip地址
#function sh_gethostip()          #同上
#function sh_get_netmask()        #获取(活动网卡)子网掩码
#function sh_get_gateway()        #获取(活动网卡)网关
#function sh_get_dns()            #获取(活动网卡)dns
#*********************************************************************************
#派断是否为 32 位系统
#*********************************************************************************
function sh_system_is_i386()
{
    local cmd="uname -a"
    local result=
    #cmd=`sh_get_arch_cmd`
    echo "$cmd | grep 64" >&2
    result=`$cmd | grep 64`
if [ "$result" = "" ]
then
echo "0" >&2
return 0
else
echo "1" >&2
return 1
fi 
}


#*********************************************************************************
#判断 CPU 是否支持64位
#*********************************************************************************
function sh_cpu_support_64()
{
    result=`cat /proc/cpuinfo | grep flags | grep lm`
if [ "$result" = "" ]
then
return 1
else
return 0
fi 
}

#*********************************************************************************
#获取ubuntu系统版本
#*********************************************************************************
function sh_get_ubuntu_version()
{
    local ac_version=
    ac_version=`cat /etc/issue`
    ac_version=`echo $ac_version|awk '{print $2}'`
    echo $ac_version
}


#*********************************************************************************
#显示 ubuntu 的系统信息
#*********************************************************************************
function sh_print_system_info()
{
os_ver=`cat /etc/issue`
os_arch=`uname -m`
meminfo=`free -m`
cpu_kernel=`cat /proc/cpuinfo | grep processor | wc -l`
cpu_info=`cat /proc/cpuinfo |grep "model name"`
result=`cat /proc/cpuinfo | grep flags | grep lm`
if [ -n "$result" ]; then
cpu_bit=64
else
cpu_bit=32
fi
echo "
=========================================
ubuntu 版本： $os_ver
操作系统位数:  $os_arch
内存信息:     
$meminfo
cpu信息:     
`cat /proc/cpuinfo |grep -E 'model name|vendor_id' | sort |uniq`
$cpu_info
cpu核数:     $cpu_kernel
cpu位数:     $cpu_bit
==========================================
"
}


#*********************************************************************************
#判断软件包状态
#*********************************************************************************
function sh_dpkg_status() 
{
    local pkg=
    if [ "$1" = "" ]; then 
        return 0
    fi
    pkg=`echo "$1" |awk -F "=" '{print $1}'`
    status=`dpkg -s $pkg |grep "install ok installed"`
    if [ "$status" != "" ]
    then
        return 0
    else
        return 1
    fi 
}


#*********************************************************************************
#安装软件包
#*********************************************************************************
function sh_dpkg_install()
{
    local old_install=
    local new_install=
    local not_exist=
    for pkg in $1
    do
        if  ! sh_dpkg_status $pkg;then
            sudo apt-get install $pkg
            if ! sh_dpkg_status $pkg;then
                new_install="$new_install $pkg"
            else
                not_exist="$not_exist $pkg"
            fi
        else
             old_install="$old_install $pkg"
        fi
    done
    echo "pkg exist:     $old_install" >&2
    echo "pkg installed: $new_install" >&2
    echo "pkg not exist: $not_exist" >&2
}


#*********************************************************************************
#卸载软件包
#*********************************************************************************
function sh_dpkg_remove()
{
    local ok_removed=
    local no_removed=
    for pkg in $1
    do
        if  sh_dpkg_status $pkg; then
            sudo apt-get remove $pkg
            ok_removed="$ok_removed $pkg"
        else
            no_removed="$no_removed $pkg"
        fi
    done
    echo "pkg removed:     $ok_removed" >&2
    echo "pkg not exist:   $no_removed" >&2
}


#*********************************************************************************
#格式化输出系统日期
#*********************************************************************************
function sh_get_date()
{
    local ac_date="$(date +%Y%m%d)"
    echo $ac_date
}

#*********************************************************************************
#格式化输出系统时间
#*********************************************************************************
function sh_get_time()
{
    local ac_time="$(date +%T)"
    echo $ac_time
}
function sh_get_time2()
{
    local ac_time="$(date +%T)"
    ac_time=`echo $ac_time|tr -d ':'`
    echo "$ac_time"
}

#*********************************************************************************
#格式化输出系统日期和时间
#*********************************************************************************
function sh_get_date_and_time()
{
    local ac_date="$(date +%Y%m%d)"
    local ac_time="$(date +%T)"
    echo $ac_date-$ac_time
}

#*********************************************************************************
#格式化输出系统日期和时间
#*********************************************************************************
function sh_get_date_and_time2()
{
#时间信息相关
#ac_date=`date | sed 's/ //g'|sed 's/CST//g'`
#ac_time=`date +%X | sed 's/ //g'|sed 's/CST//g'`
ac_time="$(date +%T)"
ac_date="$(date +%Y%m%d)"
ac_time=`echo $ac_time|sed "s/://g"`
if [ "$1" = "date" ]; then
echo $ac_date
elif [ "$1" = "time" ]; then
echo $ac_time
else
echo $ac_date$ac_time
fi
}
#时间信息相关
ac_time=`sh_get_date_and_time2 time`
ac_date=`sh_get_date_and_time2 "date"`
ac_time_date=`sh_get_date_and_time2`


#*********************************************************************************
#获取(活动网卡)ip地址
#*********************************************************************************
function sh_get_host_ip()
{
  local ipaddr=
  ipaddr=`gethostinfo.shc wlan0 |awk '{printf $2}'`
  if [ -z "$ipaddr" ]; then
    ipaddr=`gethostinfo.shc |awk '{printf $2}'`
  fi
  echo $ipaddr
}
#同上
function sh_get_ip_address()
{
  echo `sh_get_host_ip`
}
function sh_get_host_address()
{
  echo `sh_get_host_ip`
}
function sh_gethostip()
{
  echo `sh_get_host_ip`
}

#*********************************************************************************
#获取(活动网卡)子网掩码
#*********************************************************************************
function sh_get_host_netmask()
{
  unset_color  
  local result=
  result=`ifconfig  wlan |grep inet -w |tr ':' ' ' |awk '{print $7}'`
  if [ -z "$result" ]; then
    result=`ifconfig  eth |grep inet -w |tr ':' ' ' |awk '{print $7}'`
  fi
  echo $result
}
function sh_get_netmask()
{
  echo `sh_get_host_netmask`
}
#*********************************************************************************
#获取(活动网卡)网关
#*********************************************************************************
function sh_get_host_gateway()
{
  unset_color 
  local route_info=(`route |grep "default"`)
  local gateway=`echo ${route_info[1]}`
  echo $gateway
}
function sh_get_gateway()
{
  echo `sh_get_host_gateway`
}

#*********************************************************************************
#获取(活动网卡)dns
#*********************************************************************************
function sh_get_dns()
{
  unset_color 
  local route_info=(`cat /etc/resolv.conf |grep "nameserver"`)
  local nameserver=`echo ${route_info[1]}`
  echo $nameserver
}


#*********************************************************************************
#wget下载
#*********************************************************************************
function sh_wget()
{
    local result=0
    local ac_dir=./
    if [ "$1" = "" ]; then
        return 0
    fi
    if [ "$2" != "" ]; then
        ac_dir=$2
    fi
    wget --no-check-certificate  $1 --directory-prefix=$ac_dir --no-clobber
    result=1
    return ${result}
}



function sh_at_home_bake()
{
  unset_color 
  local ipaddr=`sh_get_host_ip`
  local at_home=`echo $ipaddr|grep "192.168"`
  if [ "$at_home" = "" ] 
  then
    return 1
  else
    return 0
  fi 
}


function sh_at_home()
{
ipaddr=`gethostinfo.shc wlan0 |awk '{printf $2}'`
if [ -n "$ipaddr" ]; then
  ipaddr=`echo $ipaddr|grep 192.168`
  if [ -n "$ipaddr" ]; then
    sudo ifdown eth0
    return 0
  fi
  echo "aaaa"
  return 1
else
  ipaddr=`gethostinfo.shc |awk '{printf $2}'`
fi
if [ -z "$ipaddr" ]; then
  echoTextRed "为获取有效ip"
  return 1
else
  ipaddr=`echo $ipaddr|grep 192.168`
  if [ -n "$ipaddr" ]; then
    return 0
  fi
  return 1
fi
return 1
}



function sh_is_svn_server()
{
    unset GREP_OPTIONS 
    unset GREP_COLOR
    local ipaddr=
    if [ -n "$1" ]; then 
       ipaddr=$1
    else
       ipaddr=`sh_get_host_ip`
    fi
    if [ "$ipaddr" = "smartphone" ] 
    then
        return 0
    else
        return 1
    fi
}



function delete_min_string_form_start()
{
  local string=$1
  local substring=$2
  local result=
  echo "string=$string substring=$substring"
  result=${string:0-1}
  while [ "$result" = "$substring" ]
  do
    result=${string:0}
    string=${string#$substring} 
  done
  echo "string=$string substring=$substring"
}

function delete_min_string_form_end()
{
  local string=$1
  local substring=$2
  local result=
  echo "string=$string substring=$substring">&2
  result=${string:0-1}
  while [ "$result" = "$substring" ]
  do
    result=${string:0-1}
    string=${string%$substring} 
  done
  echo "string=$string substring=$substring">&2
  echo "$string"
}

function delete_min_string_form_end()
{
  local string=$1
  local substring=$2
  local result=
  #echo "string=$string substring=$substring">&2
  result=${string:0-1}
  while [ "$result" = "$substring" ]
  do
    result=${string:0-1}
    string=${string%$substring} 
  done
  #echo "string=$string substring=$substring">&2
  echo "$string"
}

function replace_all_substring()
{
  local string=$1
  local substring=$2
  local replacement=$3
  echo "string=$string substring=$substring">&2
  string=${string//$substring/$replacement} 
  echo "string=$string substring=$substring">&2
  echo "$string"
}

function sh_strstr()
{
   local string=$1
   local substring=$2
   if [ -z "$string" ] || [ -z "$substring" ]; then
     return 1
   fi
   local result=
   result=`echo $string|grep $substring`
   if [ -n "$result" ]; then
     return 0
   else
     return 1
   fi
}



function mount_point_is_busy() 
{
  local mount_point=$1
  local mount_point_dir=
  local mount_point_name=
  mount_point=`delete_min_string_form_end $mount_point /`
  if [ -z "$mount_point" ]; then
    return 0
  else
    result=`mount |grep $mount_point -w`
    if [ -n "$result" ]; then
      return 0
    else
      return 1
    fi  
  fi
}


function sh_mount_ext4_img()
{
  local system_img=$1
  local mount_point=$2
  local SIM2IMG=`which simg2img`
  local result=`hostname |grep compiler`
  local cmd=
  if [ -n "$result" ]; then
    cmd="sudo.shc mount"
  else
    cmd="sudo mount"
  fi
  if [ -z "$system_img" ] || [ -z "$mount_point" ]; then
    echo "sh_mount_ext4_img /path/to/xxx.img /path/to/mount_point" >&2
    return 1
  else
    if test ! -f $system_img; then
      echo "$system_img 不存在!" >&2
      return 1
    fi
    mount_point=`sh_delete_last_slash $mount_point`
    result=`file $system_img |grep ext4 |grep filesystem`
    if [ -z "$result" ]; then
      if test -f $SIM2IMG; then
        result=`$SIM2IMG $system_img ${system_img}_format  2>&1`
        if [ -n "$result" ]; then
	  echo "`basename $system_img` $result" >&2
	  return 1
        fi
      else
        echo "simg2img 命令不存在!!" >&2
        return 1
      fi
      mkdir -p $mount_point >&2
      #echo "$cmd mount -o loop ${system_img}_format $mount_point">&2
      #echo "mount `basename ${system_img}` $mount_point" >&2
      $cmd -o loop ${system_img}_format $mount_point
    else
      mkdir -p $mount_point >&2
      #echo "mount `basename ${system_img}` $mount_point" >&2
      #echo "$cmd -o loop $system_img $mount_point">&2
      $cmd -o loop $system_img $mount_point
    fi
  fi
  mount_point=`mount|grep $mount_point`
  if [ -n "$mount_point" ]; then
    return 0
  else
    return 1
  fi
}

function sh_umount_all_ext4_img()
{
  local files_rm=
  local result=`hostname |grep compiler`
  local cmd=
  if [ -n "$result" ]; then
    cmd="sudo.shc umount"
  else
    cmd="sudo umount"
  fi
  #脚步入口：解析参数
  while test $# != 0
  do
    case $1 in
        *)
         files_rm="$1 $files_rm"
	 ;;
         esac
         shift
  done
  result="`mount |grep $def_mount_point |awk '{print $3}'|tr '\n' ' '`"
  echo "result=$result"
  if [ -n "$result" ]; then
    $cmd $result
  fi
  if [ -n "$files_rm" ]; then
    rm $files_rm -rf
  fi
}

function sh_chown_ext4_img_parttion()
{
  local mount_point=$1
  local result=`hostname |grep compiler`
  local cmd=
  if [ -n "$result" ]; then
    cmd="sudo.shc chown"
  else
    cmd="sudo chown"
  fi
  if [ -n "$mount_point" ]; then
    $cmd -R $USER.$USER $mount_point
  fi
}


function sh_umount_ext4_img()
{
  local system_img=
  local mountpoint=

}

#十六进制到十进制
function sh_HexToDec()
{
  local string=
  local result=
  while test $# != 0
  do
    case $1 in
        *)
	string="$string $1"
	;;
        esac
        shift
  done
  string="`echo $string|tr -d ' '`"
  #((str=0xff))
  ((result=16#$string))
  echo $result
}

#十六进制到十进制
function sh_HexToDecSend()
{
  local string=
  local result=
  while test $# != 0
  do
    case $1 in
        *)
	string="$1 $string"
	;;
        esac
        shift
  done
  string=`echo $string|tr -d ' '`
  #((str=0xff))
  ((result=16#$string))
  echo $result
}

#十六进制数倒序
function sh_HexRevert()
{
  local string=
  local result=
  while test $# != 0
  do
    case $1 in
        *)
	string="$1 $string"
	;;
        esac
        shift
  done
  echo $string
}


function sh_HexToCh()
{
  local hexs=
  local dec=
  local char=
  local chars=
  while test $# != 0
  do
    case $1 in
        *)
	hexs="$hexs $1"
	;;
        esac
        shift
  done
  for hex in $hexs
  do
    dec=`sh_HexToDec $hex`
    char=`echo $dec | awk '{printf "%c\n",$1}'`
    chars="$chars$char"
  done
  echo "$chars"
}

function sh_DecToCh()
{
  local decs=
  local dec=
  local char=
  local chars=
  while test $# != 0
  do
    case $1 in
        *)
	decs="$decs $1"
	;;
        esac
        shift
  done
  for dec in $decs
  do
    char=`echo $dec | awk '{printf "%c\n",$1}'`
    chars="$chars $char"
  done
  echo "$chars"
}

function sh_ChToDec()
{
  local chars=
  while test $# != 0
  do
    case $1 in
        *)
	chars="$chars $1"
	;;
        esac
        shift
  done
  echo $chars | tr -d "\n" | od -An -t dC
}

function sh_ChToHex()
{
  local hexs=
  local decs="`sh_ChToDec $1`"
  for dec in $decs
  do
    hexs="$hexs `sh_DecToHex $dec`"
  done
  echo "$hexs"
}


#8进制到十进制
function sh_OtcToDec()
{
  local string=$1
  local result=
  ((result=8#$string))
  echo $result
}


#二进制到十进制
function sh_BinToDec()
{
  local string=$1
  local result=
  ((result=2#$string))
  echo $result
}

#十进制到十六进制
function sh_DecToHex()
{
  local string=$1
  local result=
  result=`echo "obase=16;$string"|bc`
  echo $result
}



#十进制到八进制
function sh_DecToOct()
{
  local string=$1
  local result=
  result=`echo "obase=8;$string"|bc`
  echo $result
}

#十进制到二进制
function sh_DecToBin()
{
  local string=$1
  local result=
  result=`echo "obase=2;$string"|bc`
  echo $result
}

#十六进制到二进制
function sh_HexToBin()
{
  local string=
  local result=
  while test $# != 0
  do
    case $1 in
        *)
	string="$string $1"
	;;
        esac
        shift
  done
  string="`sh_HexToDec $string`"
  echo "string=$string">&2
  result=`sh_DecToBin "$string"`
  echo $result
}


#二进制到十六进制
function sh_BinToHex()
{
  local string=$1
  local result=
  string=`sh_BinToDec $string`
  result=`sh_DecToHex $string`
  echo $result
}

function sh_HexToOct()
{
  local string=$1
  local result=
  string=`sh_HexToDec $string`
  result=`sh_DecToOct $string`
  echo $result
}

function sh_OctToHex()
{
  local string=$1
  local result=
  string=`sh_OtcToDec $string`
  result=`sh_DecToHex $string`
  echo $result
}


function sh_merge_shc()
{
  local cmd=$1
  local data=$2
  cp $cmd ${cmd}c -rfv
  if test -d $data; then
    cd $data
    tar -cvzf /tmp/`basename $data`.tar.gz . 
    cd -
    echo "cat /tmp/`basename $data`.tar.gz >>${cmd}c"
    cat /tmp/`basename $data`.tar.gz >>${cmd}c
    rm /tmp/`basename $data`.tar.gz -rf  
  else
    cat $data >> ${cmd}c
  fi
}

function sh_split_shc()
{
  local sh_name=$1
  local result=$2
  if [ -z "$sh_name" ]; then
    echo "$result"
    return
  fi
  if [ -z "$result" ]; then
    result=/tmp/`basename $sh_name`.tar.gz
  fi
  #echo "sed '1,/^exit 0/d' $sh_name  > $result 2>/dev/null ">&2
  sed '1,/^exit 0/d' $sh_name  > $result 2>/dev/null #删除第一到包含'exit 0'开头的行
  if test ! -s $result; then
    result=
  fi
  echo "$result"
}

#>>>>>>>>>>linux系统相关 end


#>>>>>>>>>>字符串处理相关 start
#*********************************************************************************
#sed命令相关
#*********************************************************************************

function sh_get_lineno_of_xxx()
{
  local File=$1
  local keyword=$2
  local only=true
  local lineno=
  if [ -n "$3" ]; then
    only=$3
  fi
  if [ "$only" = "true" ]; then
    lineno=`cat $File |grep $keyword -nw | awk -F ':' '{printf $1" " }'`
  else
    lineno=`cat $File |grep $keyword -n | awk -F ':' '{printf $1" " }'`
  fi
  echo $lineno
}

#*********************************************************************************
#删除包含特定内容的行,如果包含路径注意转意
#*********************************************************************************
function sh_sed_file_d_row_include_xxx2()
{
  local File=$1
  local keyword=$2
  local only=$3
  local linenos=
  local cmds=
  linenos=`sh_get_lineno_of_xxx  $File $keyword $only`
  echo "linenos=$linenos"
  for lineno in $linenos
  do
    if [ -n "$cmds" ]; then
      cmds="${cmds};${lineno}d"
    else
      cmds="${lineno}d"
    fi
  done
  echo "sed -i '$cmds' $File"
  sed -i "$cmds" $File
}

#*********************************************************************************
#替换包含特定内容的行,如果包含路径注意转意
# sh_sed_file_d_row_include_xxx /tmp/new smartphone
#*********************************************************************************
function sh_sed_file_d_row_include_xxx()
{
  local File=$1
  local keyword=$2
  local only=true
  local lineno=
  if [ -n "$3" ]; then
    only=$3
  fi
  if [ "$only" = "true" ]; then
    sed -i "/\<$keyword\>/"d $File
  else
    sed -i "/$keyword/"d $File   
  fi
}

function sh_sed_replace_xxx()
{
  files_list=`find.sh`
  for f in $files_list
  do
    echo "sed -i s#$1/#$2#g  $f"
    sed -i "s#$1#$2#g"  $f
  done
}


function test_aaaaaaaaa()
{
cp /tmp/old /tmp/new
. /usr/local/bin/functions-base.sh
cat /tmp/new
}

# sh_sed_file_r_row_include_xxx /tmp/new <VirtualHost "<VirtualHost 192.168.0.123:80>"
function sh_sed_file_r_row_include_xxx()
{
  local File=$1
  local old=$2
  local new=$3
  local only=true
  if [ -n "$4" ]; then
    only=$4
  fi
  if [ "$only" = "true" ]; then
    sed -i "/\<$old\>/c\\$new" $File
  else
    sed -i "/$old/c\\$new" $File
  fi
}

#sh_sed_file_a_row_below_xxx /tmp/new svn "111111111\n2222222222\n333333333"  [false|true]
function sh_sed_file_a_row_below_xxx()
{
  local File=$1
  local old=$2
  local new=$3
  local only=true
  if [ -n "$4" ]; then
    only=$4
  fi
  if [ "$only" = "true" ]; then
    sed -i "/\<$old\>/a\\$new" $File
  else
    sed -i "/$old/a\\$new" $File
  fi
}









#*********************************************************************************
#如果路径信息中最后一个字符是'/'那么就删除
#*********************************************************************************
function sh_delete_last_slash()
{
  local last_char=
  local result=
  if [ -n "$1" ]; then
    last_char=`echo $1 | awk '{print   substr($1,length($1),1)}'`
    #echo "last_char=$last_char" >&2
    if [ "$last_char" = "/" ]; then
      result=`echo ${1%/*}`
      #echo "old=$1" >&2
      #echo "new=$result" >&2
    else
      result=$1
    fi
  fi
  echo $result
}

#*********************************************************************************
#获取(活动网卡)dns
#*********************************************************************************
function sh_string_to_number()
{
  local result=
  result=`echo $1 |  awk 'BEGIN { FS = "."; } { printf "%d", ($1 * 1000 + $2) * 1000 + $3;}'`
  echo $result
}


#*********************************************************************************
#把版本串转换成数字串
#*********************************************************************************
function sh_versin_char_to_number()
{
    ac_var=`echo $1 |  awk 'BEGIN { FS = "."; } { printf "%d", ($1 * 10000 + $2) * 10000 + $3;}'`
    echo $ac_var
}


function sh_cmp_version()
{
    #echo `sh_versin_char_to_number $1`>&2
    #echo `sh_versin_char_to_number $2`>&2
    if test `sh_versin_char_to_number $1` -gt `sh_versin_char_to_number $2`
    then
        echo "1" >&2
        return 1
    else
        echo "0" >&2
        return 0
    fi 
}

function sh_version_equel()
{
    if test `sh_versin_char_to_number $1` -eq `sh_versin_char_to_number $2`
    then
        echo "1" >&2
        return 1
    else
        echo "0" >&2
        return 0
    fi
}

#同上
function VERSION_TO_NUMBER()
{
  local result=
  result=`echo $1 |  awk 'BEGIN { FS = "."; } { printf "%d", ($1 * 1000 + $2) * 1000 + $3;}'`
  echo $result
}

function VERSION_COMPARE() 
{
    ac_cmd=`which $1`
    if [ "$ac_cmd" = "" ]; then
        soft_required=true
        files_should_install="$1 $files_should_install"
    fi
    if test `VERSION_TO_NUMBER $2` -ge `VERSION_TO_NUMBER $3`
    then
        echoText "Version $ac_version found" GREEN
    else
        echoText "Version $ac_version found. You need at least bash $3" RED
        soft_required=true
        files_should_install="$1 $files_should_install"
    fi
}


function sh_number_to_string()
{
  local result=
  result=`echo $1 |  awk 'BEGIN { FS = "."; } { printf "%d", ($1 * 1000 + $2) * 1000 + $3;}'`
  echo $result
}

#http://hlee.javaeye.com/blog/577628
#http://linux.chinaunix.net/techdoc/develop/2008/12/31/1055895.shtml
function IsNumeric()
{
_NUM=$1
_NUM=`echo $1|awk '{if($0~/[^0-9]/) print "1"}'`
if [ "$_NUM" != "" ]
then
return 1
else
return 0
fi
} 

# characters()
function IsCharacters()
{
_LETTERS_ONLY=$1
_LETTERS_ONLY=`echo $1|awk '{if($0~/[^a-zA-Z]/) print "1"}'`
if [ "$_LETTERS_ONLY" != "" ]
then
return 0
else
return 1
fi 
}


function sh_filter()
{
local result=
local args=
local strings1=
local strings2=
local strings3=
local ac_filter=
local v_mode=
local w_mode=
while test $# != 0
do
    case $1 in
	-v)
	    v_mode=$1
	    ;; 
	-w)
            w_mode=$1
            ;;
        *)
	   args="$args $1"
	   ;;
       esac
       shift
done
  strings1=`echo $args | awk -F ',' '{print $1}'`
  strings2=`echo $args | awk -F ',' '{print $2}'`
  for str2 in $strings2
  do
    ac_str=`echo $str2|tr -d '[|]'|tr -d ' '`  
    if [ -n "$ac_str" ]; then 
      if [ -z "$ac_filter" ]; then
        ac_filter=$ac_str
      else
        ac_filter="$ac_filter|$ac_str"
      fi
    fi
  done
  #显示过滤串
  #echo "${ac_filter}">&2
  for str1 in $strings1
  do
    if [ -n "$ac_filter" ]; then
      #echo ""$str1"|grep  -E  "$ac_filter" ${v_mode} ${w_mode}"
      result=`echo "$str1"|grep  -E  "$ac_filter" ${v_mode} ${w_mode}`
      if [ -n "$result" ]; then
         strings3="$strings3 $str1"
      fi
    fi
  done
echo "$strings3"
}

function sh_filter_out()
{
local result=
local args=
local strings1=
local strings2=
local strings3=
while test $# != 0
do
    case $1 in
	-m)
	    shift
	    mode=$1
	    ;; 
        *)
	   args="$args $1"
	   ;;
       esac
       shift
done
strings1=`echo "$args" | awk -F',' '{print $1}'`
strings2=`echo "$args" | awk -F',' '{print $2}'`
for str in $strings1
do
  ac_str=`echo $str|tr -d '[|]'`
  #str=`echo $str|tr -d '[|]'`
  result=`echo "$strings2"|grep -E "$ac_str"`
  if [ -z "$result" ]; then
    strings3="$strings3 $str"
  fi
done
echo "$strings3"
}

function sh_format_chs()
{
  local c=
  local h=
  local s=
  local string=
  local bstring="000000000000000000000000"
  local result=
  local len=0
  while test $# != 0
  do
    case $1 in
        *)
	string="$string $1"
	;;
        esac
        shift
  done

  string=`echo $string |tr -d ' '`
  len=${#string}
  if [ "$len" != "6" ]; then
    echo "格式错误" >&2
    return
  fi
  string="`sh_HexToBin \"$string\"`"
  len=${#string}
  ((len=24-$len))
  result=`echo ${bstring:0:$len}`
  result=${result}${string}
  #echo "result=$result"
  h=`echo ${result:0:8}`
  s=`echo ${result:8:6}`
  c=`echo ${result:14:10}`
  echo "h=$h s=$s c=$c" >&2
  h=`sh_BinToDec $h`
  s=`sh_BinToDec $s`
  c=`sh_BinToDec $c`
  echo "$c $h $s"
}

function sh_display_hdisk_info() 
{
  local mbr=/tmp/mbr
  local ebr=/tmp/ebr
  local xbr_info=
  local disk=`ls /dev/[h-s]d[a-z]`
  local diskc=`echo $disk |wc -w`
  local addr=
  local add=
  local fs_type=
  local sec_count=
  local index=1
  if [ "$diskc" -gt "1" ]; then
    echoTextGreen "请选择设备..."
    disk=`sh_menu_select "$disk"`
  fi
  sudo dd if=$disk bs=512 count=1 of=$mbr
  #sh_display_xbr_info $mbr
  for c in `seq 0 3`
  do
    ((add=16*c))
    ((addr=450+add))
    fs_type="`hexmodify.shc -i $mbr -s $addr -n 1 | tr -d ' '`"
    ((add=16*c))
    ((addr=454+add))
    sec_count="`hexmodify.shc -i $mbr -s $addr -n 4`"
    sec_count=`sh_HexToDecSend $sec_count`
    ((index=c+1))
    if [ "$fs_type" != "00" ]; then
      if [ "$fs_type" != "05" ]; then
        echo "${disk}${index} type=$fs_type count=$sec_count"
      else
        echo "扩展分区： ${disk}${index} type=$fs_type count=$sec_count"
        sh_display_ebr_info  $disk $sec_count
      fi
    fi
  done
}
ebr_th=5
function sh_display_ebr_info() 
{
  local ebr=/tmp/ebr
  local disk=$1
  local sec_index=$2
  echo "sudo dd if=$disk bs=512 count=1 skip=$sec_index of=${ebr}${ebr_th}"
  sudo dd if=$disk bs=512 count=1 skip=$sec_index of=${ebr}${ebr_th} 
  for c in `seq 0 1`
  do
    ((add=16*c))
    ((addr=450+add))
    fs_type="`hexmodify.shc -i ${ebr}${ebr_th} -s $addr -n 1 | tr -d ' '`"
    ((add=16*c))
    ((addr=454+add))
    sec_index="`hexmodify.shc -i ${ebr}${ebr_th} -s $addr -n 4`"
    sec_index=`sh_HexToDecSend $sec_index`
    echo "fs_type=$fs_type sec_count=$sec_index"
    if [ "$fs_type" != "00" ]; then
      if [ "$fs_type" != "05" ]; then
        echo "逻辑磁盘 ${disk} type=$fs_type count=$sec_index"
      else
        echo "扩展分区： ${disk}${index} type=$fs_type count=$sec_index"
        ((ebr_th=ebr_th+1))
        sh_display_ebr_info ${disk} ${sec_index}
      fi
    fi
  done
}



function sh_chs_to_lba()
{
#C表示当前柱面号
#H表示当前磁头号
#Cs表示起始柱面号
#Hs表示起始磁头号
#Ss表示起始扇区号
#PS表示每磁道有多少个扇区
#PH表示每柱面有多少个磁道
#一般情况下，CS=0、HS=0、SS=1；PS=63、PH=255
  local C=$1
  local H=$2
  local S=$3
  local CS=0
  local HS=0
  local SS=1
  local PS=63
  local PH=255
  local _CS=`echo $C-$CS|bc`
  local _HS=`echo $H-$HS|bc`
  local _SS=`echo $S-$SS|bc`
  local lba=
  lba=`echo $_CS*$PH*$PS + $_HS*$PS + $_SS|bc`
  echo $lba
}

function sh_display_part_table()
{
  local act_flag=$1
  local hsc_start=$2
  local fs_type=$3
  local hsc_end=$4
  local sec_num=$5
  local sec_count=$6
  local part_name=$7
local xbr_info2="
${act_flag}:1:活动标识
${hsc_start}:3:起磁扇柱
${fs_type}:1:文件类型
${hsc_end}:3:束磁扇柱
${sec_num}:4:始扇区号
${sec_count}:4:总扇区数
"
local PartType_list="
0b:Fat32
07:NTFS
05:Extended
83:Linux
82:Linux-swap-Solaris 
"
  if [ -z "$part_name" ]; then
    part_name=
  fi
  for c in `seq 0 3`
  do
    echoTextBlue "分区表 $c"
    for i in $xbr_info2  
    do
      addr_dec=`echo $i|awk -F":" '{print $1}'`
      ((addr_add=16*c))
      ((addr_dec=addr_dec+addr_add))
      size=`echo $i|awk -F":" '{print $2}'`
      name=`echo $i|awk -F":" '{print $3}'`
      ((addr_del=addr_dec+size))
      addr_del="00000`sh_DecToHex $addr_del | tr A-Z  a-z`" 
      addr_hex="00000`sh_DecToHex $addr_dec | tr A-Z  a-z`"
      #info="`hexdump  -s $addr_dec $parttion -C  -n $size | sed \"s/$addr_del//g\"`"
      info="`hexmodify.shc -i $parttion -s $addr_dec  -n $size`"
      cmp=`echo $info |tr -d ' '`
      if [ "`echo $addr_dec-$addr_add | bc`" = "${act_flag}" ]; then
        if [ "$cmp" = "80" ]; then
          info2="活动分区"
        else
          info2="非"
        fi
      elif [ "`echo $addr_dec-$addr_add | bc`" = "${fs_type}" ]; then
        for t in $PartType_list
        do
          pt=`echo $t | awk -F":" '{print $1}'`
          info2=`echo $t | awk -F":" '{print $2}'`
          if [ "$cmp" = "$pt" ]; then 	    
	    break
	  else
	    info2="未知类型"
          fi
        done        
      elif [ "`echo $addr_dec-$addr_add | bc`" = "${sec_count}" ]; then
        info2="`sh_HexToDecSend $info`"
        part_len_k=`echo ${info2}*512/1024|bc`
	part_len_m=`echo "scale=8;$part_len_k/1024"|bc`
        part_len_g=`echo "scale=8;$part_len_m/1024"|bc`
      else
        info2="`sh_HexToDecSend $info`"
      fi
      printf "地址:$addr_dec   字节数:%3s     %s:%15s(%s)\n" "$size"  "$name" "$info" "$info2"
    done    
    echoTextGreen "分区大小: ${part_len_k}K   ${part_len_m}M  ${part_len_g}G\n"
  done

}


function sh_display_part_table2()
{
  local act_flag=$1
  local hsc_start=$2
  local fs_type=$3
  local hsc_end=$4
  local sec_num=$5
  local sec_count=$6
  local part_name=$7
  local bin=$1
  local count=`hexmodify.shc -i $bin  -v | grep 55 -w |grep aa -w |wc -l`
  echo "count=$count"
  ((count=count-1))
  for i in `seq 0 $count`
  do
    ((address=512*i))
    echo "address=$address"
    ((address_skip=address+446))
    dd if=$bin of=/tmp/partiton.bin bs=1 count=512 skip=$address
    sh_display_xbr_info /tmp/partiton.bin
  done

}

function sh_display_xbr_info()
{
  local result=
  parttion=$1
  part_name=$2
  #sh_dpkg_install "hexdump"
  if [ -z "$parttion" ]; then
    echoTextRed "请指定引导记录!!"
    return
  else
    if test ! -f $parttion; then
      echoTextRed "$parttion 不存在!!"
      return
    fi
  fi
  if [ -z "$part_name" ]; then
    result=`basename $parttion | grep -i br`
    if [ -z "$result" ]; then
      part_name=MBR
    else
      part_name=$result
    fi
  fi
  echoTextBlue "显示 $part_name 信息:"
xbr_info2="
446:1:活动标识
447:3:s磁扇柱
450:1:文件类型
451:3:e磁扇柱
454:4:始扇区号
458:4:总扇区数
"

PartType_list="
0b:Fat32
07:NTFS
05:Extended
83:Linux
82:Linux-swap-Solaris 
"
  sh_display_part_table 446 447 450 451 454 458 MBR
  return
  for c in `seq 0 3`
  do
    echoTextBlue "$part_name 分区表 $c"
    for i in $xbr_info2  
    do
      addr_dec=`echo $i|awk -F":" '{print $1}'`
      ((addr_add=16*c))
      ((addr_dec=addr_dec+addr_add))
      size=`echo $i|awk -F":" '{print $2}'`
      name=`echo $i|awk -F":" '{print $3}'`
      ((addr_del=addr_dec+size))
      addr_del="00000`sh_DecToHex $addr_del | tr A-Z  a-z`" 
      addr_hex="00000`sh_DecToHex $addr_dec | tr A-Z  a-z`"
      #info="`hexdump  -s $addr_dec $parttion -C  -n $size | sed \"s/$addr_del//g\"`"
      info="`hexmodify.shc -i $parttion -s $addr_dec  -n $size`"
      cmp=`echo $info |tr -d ' '`
      if [ "`echo $addr_dec-$addr_add | bc`" = "446" ]; then
        if [ "$cmp" = "80" ]; then
          info2="活动分区"
        else
          info2="非"
        fi
      elif [ "`echo $addr_dec-$addr_add | bc`" = "450" ]; then
        for t in $PartType_list
        do
          pt=`echo $t | awk -F":" '{print $1}'`
          info2=`echo $t | awk -F":" '{print $2}'`
          if [ "$cmp" = "$pt" ]; then 	    
	    break
	  else
	    info2="未知类型"
          fi
        done        
      elif [ "`echo $addr_dec-$addr_add | bc`" = "458" ]; then
        info2="`sh_HexToDecSend $info`"
        part_len_k=`echo ${info2}*512/1024|bc`
	part_len_m=`echo "scale=8;$part_len_k/1024"|bc`
        part_len_g=`echo "scale=8;$part_len_m/1024"|bc`
      else
        info2="`sh_HexToDecSend $info`"
      fi
      printf "地址:$addr_dec   字节数:%3s     %s:%15s(%s)\n" "$size"  "$name" "$info" "$info2"
    done    
    echoTextGreen "分区大小: ${part_len_k}K   ${part_len_m}M  ${part_len_g}G\n"
  done
}


function sh_display_bootimg_info()
{
  echoTextBlue "displaying bootimg info......"
  local img=$1
  local magic=
  local base=
  local kernel_size=
  local kernel_addr=
  local ramdisk_size=
  local ramdisk_addr=
  local board=
  local cmdline=
  local MKBOOTIMG=out/host/linux-x86/bin/mkbootimg
  if test -f $img; then 
    base=`hexdump  -s 12  -n 4   $img -C | awk '{print $5$4$3$2}'`
    base=`echo "$base-00008000"|bc`
    magic=`hexdump  -s 0   -n 8   $img -C | awk '{print $NF}' | grep "|" | tr -d '|'`
    #大小,起始地址
    kernel_size="`hexdump  -s 8   -n 4   $img -C | awk '{print $5$4$3$2}'`"
    kernel_size=`sh_HexToDec $kernel_size`
    kernel_addr=`hexdump  -s 12  -n 4   $img -C | awk '{print $5$4$3$2}'`
    ramdisk_size=`hexdump  -s 16  -n 4   $img -C | awk '{print $5$4$3$2}'`
    ramdisk_size=`sh_HexToDec $ramdisk_size`
    ramdisk_addr=`hexdump  -s 20  -n 4   $img -C | awk '{print $5$4$3$2}'`
    second_size=`hexdump  -s 24  -n 4   $img -C | awk '{print $5$4$3$2}'`
    second_size=`sh_HexToDec $second_size`
    second_addr=`hexdump  -s 28  -n 4   $img -C | awk '{print $5$4$3$2}'`
    tags_addr=`hexdump  -s 32  -n 4   $img -C | awk '{print $5$4$3$2}'`
    page_size=`hexdump  -s 36  -n 4   $img -C | awk '{print $5$4$3$2}'`
    board="`hexdump  -s 48  -n 16  $img -C | awk '{print $NF}' | grep "|" | tr -d '|'`"
    board="`echo $board| tr -d '.'`"
    echoTextBlue "获取命令行参数,请稍后......"
    for i in `seq 64 512`
    do
      str="`hexdump -s $i -n 1 $img  -v -C | awk '{print $2}' |tr '\n' ' '|tr -d ' '`"
      if [ "$str" = "00" ]; then 
	 continue;
      fi
      str=`sh_HexToDec $str`
      #printf \\110 也可以输出字符
      str=`echo $str | awk '{printf "%c\n",$1}'`
      cmdline="${cmdline}${str}"
    done
  fi
echoTextBlue "
        magic:  $magic 
         base:  0x$base
  kernel size:  $kernel_size     `echo \"scale=8;$kernel_size/1024/1024\"|bc`M
  kernel addr:  $kernel_addr
 ramdisk_size:  $ramdisk_size    `echo \"scale=8;$ramdisk_size/1024/1024\"|bc`M
 ramdisk_addr:  $ramdisk_addr
  second_size:  $second_size  
  second_addr:  $second_addr
    tags_addr:  $tags_addr
    page_size:  $page_size   `echo \`sh_HexToDec $page_size\``(byte)
        board:  $board
      cmdline:  $cmdline"
if [ -n "$cmdline" ]; then
  cmdline="--cmdline '$cmdline'"
fi
if [ -n "$base" ]; then
  base="--base $base"
fi
if [ -n "$page_size" ]; then
  page_size="--pagesize `echo \`sh_HexToDec $page_size\``"
fi
if [ -n "$board" ]; then
  board="`echo "--board $board" | tr -d '.'`"
fi
#ro.custom.build.version=1340631902
#build/tools/buildinfo.sh:8:echo "ro.custom.build.version=$CUSTOM_BUILD_VERNO"
echoTextGreen "$MKBOOTIMG --kernel kernel --ramdisk  ramdisk.img $base  $page_size $cmdline $board --output boot.img"
}

function sh_uppertolower()
{
  echo "$1"|tr 'A-Z' 'a-z' 
}
function sh_lowertoupper()
{
  echo "$1"|tr 'a-z' 'A-Z'
}




#>>>>>>>>>>字符串处理相关 end


#>>>>>>>>>>scm配置管理相关
function sh_check_repo()
{
if test -f ~/bin/repo; then
echoText "exist file ~/bin/repo,skip!" RED
else
mkdir -pv ~/bin/
curl http://android.git.kernel.org/repo >~/bin/repo
chmod a+x ~/bin/repo
fi
export PATH=~/bin/:$PATH
}

#同步函数
function sh_repo_sync()
{
nohup repo sync
while [ $? = 1 ]; do
echoText "======sync failed, re-sync again======" RED
sleep 3
nohup repo sync
done
}

function sh_opt_is_mounted()
{
  local result=
  result=`mount |grep \/opt -w`
  if [ -n "$result" ]; then
    return 0
  else
    return 1
  fi
}

function sh_mysql_backup() 
{
  local db_name=$1
  local username=$2
  local password=$3
  local backup_dir=$4
  if ! sh_opt_is_mounted; then
    return
  fi
  if [ -z "$backup_dir" ]; then
    backup_dir=/opt/backup/mysql
  fi 
  mkdir -pv $backup_dir
  if [ -z "$db_name" ] || [ -z "$db_name" ] || [ -z "$db_name" ]; then
    return
  fi
  echo "mysqldump -u $username --password=$password  $db_name --skip-lock-tables >$backup_dir/${db_name}.sql"
  mysqldump -u $username --password=$password  $db_name --skip-lock-tables >$backup_dir/${db_name}.sql
}







#*********************************************************************************
#获取ini文件的所有标签
#*********************************************************************************
function sh_get_ini_tags()
{
  local ac_tags=
  local ac_tag=
  local ac_ini=$1
  if test -f $ac_ini ; then
    ac_tags=`cat $ac_ini |grep "\[*\]" |tr -d '[|]'|tr '\n' ' '`
    ac_tags=`echo $ac_tags | sed 's/global//g'`
  fi
  echo "$ac_tags"
}

function sh_get_ini_tag()
{
  local ac_tags=
  local ac_tag=
  local ac_ini=$1
  if test -f $ac_ini ; then
    ac_tags=`sh_get_ini_tags $ac_ini`
    if [ -n "$ac_tags" ]; then
      ac_tag=`sh_menu_select "$ac_tags"`
      echo "tag=$ac_tag">&2
    fi
  fi
  echo $ac_tag
}

#>>>>>>>>>>邓伟成添加的函数 end
#$1信息，$2默认路径，功能：获得手动输入目录:BY dwc
#用法:curr_dir=`inputDirectory "请输入路径," "~/workspace"`
function inputDirectory()
{
	local message=$1
	local defaultDir=$2
	dirc=
	while [ 1 ]
	do
		echoText "$message 默认路径为:$defaultDir" RED >&2
		echo -n "input directory:" >&2
		read dirc
		if [ "x$dirc" = "x" ]; then
			dirc="$defaultDir/"
			break;
		elif [ -d  "$dirc" ]; then
			dirc="$dirc/"
			break;
		else
			result=`question "$dirc is not a directory! Create it ?" 1  yes`
			if [ "$result" = "yes" ] ; then
				mkdir -pv $dirc #创建目录
				break;
			fi
		fi
	done
	
	echo $dirc
}


#$1,信息，$2,最小值(包含)，$3最大值(包含)，$4默认值:BY dwc
#功能：获得手动输入指定范围内的数值
function inputNumber()
{
	local message=$1
	local numBegin=$2
	local numEnd=$3
	local numDefault=$4
	user_num=
	while [ 1 ]
	do
		echoText "$1数值范围为[$numBegin,$numEnd],默认值为: $numDefault" GREEN >&2
		echo  -n "input number:"	>&2
		read	user_num
	if [ "x$user_num" = "x" ]  ; then
		  user_num=$numDefault
		break;
		elif $(IsNumeric $user_num) ; then
			if [ $user_num -ge $numBegin -a $user_num -le  $numEnd  ] ; then
				break;
			fi
	fi
	done 
	echo  $user_num
}

#$1 提示信息 $2数组  $3 默认选项:BY dwc
#默认选项从“1”开始计数,
#调用示例	:	list=(a b c d e f g)
#调用示例	:	getChoice "该文件夹下的版本库有:" list  2
#调用示例	:	echo "you choose $?"
#获得返回值使用aa=`getChoice "该文件夹下的版本库有:" list  2`
#直接回车得到答案真实答案 b,而不是2
function getChoice()
{
    
    local	index=1
    local	message=$1
    eval	parameter_list=(\${$2[@]}) 
    local	default_value=$3
    local	ANSWER=

    local v

	echoText "$message  默认选项为[ $default_value ]" RED >&2
    for v in "${parameter_list[@]}"
    do
        echo "     $index. $v"	>&2
        index=$(($index+1))
    done
	while [ 1 ]
	do
	echoText  "请选择！ 默认选项为： [ $default_value ] " GREEN 	>&2
	echo -n "enter number:" 	>&2
       read ANSWER
        if [ -z "$ANSWER" ] ; then
             ANSWER=$default_value
            break;
        elif (echo -n $ANSWER | grep -q -e "^[0-9][0-9]*$") ; then
            if [ $ANSWER -le ${#parameter_list[@]} -a $ANSWER -gt 0  ] ; then
# 				MATCH=$ANSWER
				break;
            fi
		fi
    done
#     echo "$ANSWER" >&2
    echo ${parameter_list[$(( $ANSWER - 1 ))]}
}

#用于查找字符串是否存在于另一个字符串的函数
function strIsSubstring()
{
	local x=0
	case "$2" in
	*$1*) x=1;;
	esac
	echo $x
	return $x
}

#用于查找字符串是否等于另一个字符串的函数
function strEqSubstring()
{
        local x=0
        case "$2" in
        $1) x=1;;
        esac
        echo $x
        return $x
}
#在设置环境变量时获取项目的modem信息，提供给项目代码中的functions.sh使用
function sh_get_version_of_modem()
{
  local version=
  local ac_modem_ver=
  local work_copy=$1
  local product_solution=$2
  local config_file=$3
  local modem_folder_name=
  if [ -z "$work_copy" ]; then
    work_copy=`pwd`
  fi
  if [ -z "$product_solution" ] || [ -z "$config_file" ]; then
    echo "$version" 
    return
  fi
  if [ "$product_solution" = "mtk" ]; then
    modem_folder_name=`cat $work_copy/$config_file | grep ^CUSTOM_MODEM | awk -F '=' '{print $2}'`
    ac_modem_ver=`strings $work_copy/mediatek/custom/common/modem/${modem_folder_name}/modem.info | grep MTK_BUILD_VERNO |  awk -F '=' '{print $2}' |tr -d ' '`
    version=${ac_modem_ver}
  else
    echo "qcom_mode_1.0"
  fi
  echo "$version"
}

function sh_get_version_of_android()
{
  local version=
  local work_copy=$1
  local product_solution=$2
  local config_file=$3
  if [ -z "$work_copy" ]; then
    work_copy=`pwd`
  fi
  if [ -z "$product_solution" ] || [ -z "$config_file" ]; then
    echo "$version" 
    return
  fi
  if [ "$product_solution" = "mtk" ]; then
    ac_version=`cat mediatek/config/common/ProjectConfig.mk | grep MTK_BUILD_VERNO  |  awk -F '=' '{print $2}' |tr -d ' '`
    version=${ac_version}
  else
    echo "qcom_mode_1.0"
  fi
  echo "$version"
}

#*********************************************************************************
#从列表中选择产品名称
#*********************************************************************************
function sh_get_board_config_file()
{
  local product_solution=
  local product_name=
while test $# != 0
do
    case $1 in	
	-d)
	    shift
	    product_name=$1
	    ;; 
	-s)
	    shift
	    product_solution=$1
	    ;; 
	esac
	shift
done
  #echo "product_name=$product_name product_solution=$product_solution" >&2
  local vivo_top=vendor/vivo
  local vivo_out=$vivo_top/out
  local project_out=$vivo_out/$project_name
  mkdir -p $project_out
  local BoardConfig_file=$project_out/BoardConfig.txt
  local BoardConfig_file2=$project_out/BoardConfig2.txt
  local ac_filtered="generic|asus|common|google|lge|sample|samsung|ti|x86|android|banyan_addon|win"
  rm  $BoardConfig_file -rf
  echoTextBlue "==> 获取目标板配置文件...." >&2
  ls device/*/*/BoardConfig.mk 2>/dev/null | grep -v -E "$ac_filtered">>$BoardConfig_file
  ls vendor/*/*/BoardConfig.mk 2>/dev/null |grep -v -E "$ac_filtered">>$BoardConfig_file
  ls mediatek/*/*/BoardConfig.mk 2>/dev/null |grep -v -E "$ac_filtered">>$BoardConfig_file
  if [ "$product_solution" = "mtk" ]; then
    product_solution=mediatek
  fi
  if [ -n "$product_name" ]; then
    if [ -n "$product_solution" ]; then
      cat $BoardConfig_file |grep $product_solution | grep $product_name>$BoardConfig_file2
    else
      cat $BoardConfig_file |grep $product_name>$BoardConfig_file2
    fi
    cat $BoardConfig_file2>$BoardConfig_file
  fi
  local ac_count=`cat $BoardConfig_file |wc -l`
  if [ "$ac_count" -eq  "0" ]; then
    board_config_file=
  elif [ "$ac_count" -eq  "1" ]; then
    board_config_file=`cat $BoardConfig_file`
  else
    menu_list="`cat $BoardConfig_file`"
    board_config_file=`sh_menu_select2 "$menu_list"`
  fi
  echo "$board_config_file"
}

function sh_get_product_info()
{
  local product_file=
  local product_solution=
  local product_name=
while test $# != 0
do
    case $1 in	
	-d)
	    shift
	    product_name=$1
	    ;; 
	-s)
	    shift
	    product_solution=$1
	    ;; 
	esac
	shift
done
  if [ -n "$product_solution" ]; then
    ac_product_solution="-s $product_solution"
  fi
  if [ -n "$product_name" ]; then
    ac_product_name="-d $product_name"
  fi
  local board_config_file=`sh_get_board_config_file $ac_product_solution $ac_product_name`
  echoTextBlue "==> 获取平台产品信息...." >&2
  if [ -z "$board_config_file" ]; then
    product_name=
    product_file=
  else
    product_name=`dirname $board_config_file`
    product_name=`basename $product_name`
    product_file=build/target/product/${product_name}.mk
  fi
  if test  ! -f  $product_file && [ -n "$product_file" ]; then
    product_file=`dirname $board_config_file`/${product_name}.mk
  fi
  echo "$product_name  $product_file"
}

function   sh_get_product_name_bake()
{
  local product_name=
  local work_copy=$1
  if [ -n "$work_copy" ]; then
     if test -d $work_copy/mediatek/; then
        products_name=`ls $work_copy/mediatek/config/`
     else
        products_name=`ls $work_copy/device/qcom/`
     fi
  fi
  product_name=`sh_menu_select2 "$products_name"`
  echo "$product_name"
}

function sh_copy_config_files_from_vendor()
{
  local project_name=$1
  local hardware_list=
  local project_list=
  local dirs_list=
  local dirs_list2=
  local files_list=
  local files_list2=
  local files_list3=
  local files_list4=
  local tmp_dirs_f=/tmp/ac_dirs
  local vendor_root=vendor/vivo
  rm $tmp_dirs_f -rf; touch $tmp_dirs_f
  hardware_list=`cat project_info | grep "\[" |grep "\]" |tr -d '[]' | grep -v default`
  for h in $hardware_list
  do
    project_list="$project_list `parseini.shc project_info get $h project_name`"
  done
  #echo "project_list=$project_list"
  if [ -n "$project_name" ]; then
    dirs_list=`ls ${vendor_root}/source/PD* -d 2>/dev/null | sort -r `
    files_list=`find ${vendor_root}/source/PD* -path '*/.*'   -prune -o  -type f  -print  2>/dev/null | sort |uniq`
    dirs_list=`sh_filter ${dirs_list},${project_list}`
    files_list=`sh_filter ${files_list},${project_list}`
    #echo "dirs_list=$dirs_list"
    #echo "files_list=$files_list"
    if [ -n "$files_list" ]; then
      for d in $dirs_list
      do
        files_list=`echo "$files_list" | sed s#$d/##g`
      done   
    fi
    dirs_list2=`find ${vendor_root}/source/PD* -path '*/.*'   -prune -o  -type d  -print 2>/dev/null |sort |uniq`
    if [ -n "$dirs_list2" ]; then
      for d in $dirs_list
      do
        dirs_list2=`echo "$dirs_list2" |sed s#$d##g | sort |uniq |tee $tmp_dirs_f`
      done   
    fi
    if [ -n "$dirs_list2" ]; then
      sed -i 's/.//' $tmp_dirs_f
    fi
    dirs_list2=
    project_saved=`echo $out_link_saved |awk -F "-" '{print $2}'|awk -F "M" '{print $1}'`
    for d in `cat $tmp_dirs_f`
    do
      count=`cat $tmp_dirs_f |grep $d |wc -l`
      if [ "$count" = "1" ]; then
        dirs_list2="$dirs_list2 $d"
      else
        if [ "$project_saved" != "" ] && [ -d "${vendor_root}/source/$project_saved/$d" ];then
          dir_have_file=`ls ${vendor_root}/source/$project_saved/$d -p|grep [^/]$`
          if [ "$dir_have_file" != "" ];then
             dirs_list2="$dirs_list2 $d"
          fi
        fi
      fi
    done
    rm $tmp_dirs_f -rf
    echoTextGreen "请稍侯......"
    if [ -n "$dirs_list2" ]; then
      files_list3=`svn st $dirs_list2 |grep ? -w | awk '{print $2}'`
      files_list4=`svn st $dirs_list2 |grep M -w | awk '{print $2}'`
    fi
    if [ "$out_link_saved" != "$out_link_cur" ] && [ -n "$out_link_saved" ]; then
      if [ -n "$files_list3" ]; then
        echo $files_list3 | xargs rm -rf
      fi
      if [ -n "$files_list4" ]; then
        echo $files_list4 | xargs svn revert -q
      fi
    fi
    if test -d ${vendor_root}/source/${project_name}; then
      cd ${vendor_root}/source/${project_name}
      files_list2=`find  -path '*/.*'   -prune -o  -type f  -print`
      if [ -n "$files_list2" ]; then
        cp --parents -rfvL $files_list2 ../../../../
      fi
      cd -
    else
      echoTextRed "${vendor_root}/source/${project_name} not exist!!"
    fi
  fi
}

function sh_get_list_of_project_info()
{
  local hardware_list=
  local project_list=
  if test -f  project_info; then
    hardware_list=`cat project_info | grep "\[" |grep "\]" |tr -d '[]' | grep -v default`
    for h in $hardware_list
    do
      project_list="$project_list `parseini.shc project_info get $h project_name`"
    done
  fi
  echo "$project_list"
} 

function sh_config_project_files()
{
  local project_name=$1
  local android_branch=$2
  local dirs_list=
  local files_list=
  local project_list=
  local files_list_f=/tmp/files_list
  local files_list_f2=/tmp/files_list2
  local saved_dir=`pwd`
  local vendor_root=vendor/vivo
  local vendor_source=${vendor_root}/source

  rm -rf $files_list_f $files_list_f2
  touch $files_list_f $files_list_f2
  dirs_list=`ls $vendor_source/project/PD* -d 2>/dev/null | sort -r `
  project_list=`sh_get_list_of_project_info`
  dirs_list=`sh_filter ${dirs_list},${project_list}`
  for d in $dirs_list
  do
    cd $d
    find  -path '*/.*'   -prune -o  -type f  -print  2>/dev/null | sort |uniq >> $files_list_f
    cd $saved_dir
  done
  files_list=`cat $files_list_f |sort|uniq`
  echoTextGreen "请稍侯......"
  if [ -n "$files_list" ]; then
    files_list_revert=`svn st $files_list |grep M -w | awk '{print $2}'`
    files_list_delete=`svn st $files_list |grep ? -w | awk '{print $2}'`
  fi
  if [ -n "$files_list_delete" ]; then
    echo $files_list_delete | xargs rm -rfv
  fi
  if [ -n "$files_list_revert" ]; then
    echo $files_list_revert | xargs svn revert 
  fi
  ##拷贝项目相关文件
  if [ -z "${project_name}" ]; then
    echoTextRed "获取项目名失败!!"
    return
  fi
  if test -d $vendor_source/project/${project_name}; then
    echoTextRed "cd $vendor_source/project/${project_name}"
    cd $vendor_source/project/${project_name}
    find  -path '*/.*'   -prune -o  -type f  -print >> $files_list_f2
    files_list=`cat $files_list_f2 |sort|uniq`
    if [ -n "$files_list" ]; then
      cp --parents -rfvL $files_list $saved_dir
    fi
    cd $saved_dir
  else
    echoTextRed "$vendor_source/project/${project_name} not exist!!"
  fi

  #拷贝 ${vendor_root}/source/common 下的所有代码到 ${vendor_root}/out/PDXXXX
  if test -d $vendor_source/common; then
    cd $vendor_source/common
    files_list=`find  -path '*/.*'   -prune -o  -type f  -print  2>/dev/null`
    if [ -n "$files_list" ]; then
      mkdir -pv ../../out/${project_name}
      #cp --parents -rfvL $files_list  ../../out/${project_name}
    fi
    cd $saved_dir
  fi

  #如果 android 分支下有代码,那么优先使用
  if [ -n "$android_branch" ]; then
    if test -d $vendor_source/$android_branch; then
     cd $vendor_source/$android_branch
      files_list=`find  -path '*/.*'   -prune -o  -type f  -print  2>/dev/null`
      if [ -n "$files_list" ]; then
        mkdir -pv ../../out/${project_name}
        cp --parents -rfvL $files_list  ../../out/${project_name}
      fi
      cd $saved_dir
    fi
    cp $vendor_source/${project_name}
  fi
}

function sh_convert_file_suffixs_to_grep_e_filter()
{
  local strings="$1"
  local result=
  for str in $strings
  do
    if [ -z "$result" ]; then
      result="\.$str$"
    else
      result="\.${str}$|$result"
    fi
  done
  echo "$result"
}

function sh_convert_strs_to_grep_e_filter()
{
  local strings="$1"
  local result=
  for str in $strings
  do
    if [ -z "$result" ]; then
      result="$str"
    else
      result="${str}|$result"
    fi
  done
  echo "$result"
}

function sh_convert_strs_to_grep_e_filter_start()
{
  local strings="$1"
  local result=
  for str in $strings
  do
    if [ -z "$result" ]; then
      result="^$str"
    else
      result="^${str}|$result"
    fi
  done
  echo "$result"
}

function sh_convert_strs_to_grep_e_filter_end()
{
  local strings="$1"
  local result=
  for str in $strings
  do
    if [ -z "$result" ]; then
      result="$str$"
    else
      result="${str}$|$result"
    fi
  done
  echo "$result"
}

function sh_get_android_PLATFORM_VERSION()
{
  local version=common
  version=`cat build/core/version_defaults.mk |grep "PLATFORM_VERSION :=" |awk '{print $3}'`
  version=`echo ${version:0:3}`
  if [ -n "$version" ]; then
    echo "$version"
  else
    echo "common"
  fi
}

function sh_get_projects_list()
{
  local ac_list=
  local project_list=
  ac_list=`cat project_info | grep "\[" |grep "\]" |tr -d '[]' | grep -v default`
  for ac in $ac_list
  do
    project_list="`parseini.shc project_info get $ac project_name` $project_list"
  done
  echo "$project_list"
}

function sh_copy_vendor_vivo_res()
{
  local project_name=
  local vivo_rom=
while test $# != 0
do
    case $1 in
	-v)
	    shift
	    vivo_rom=$1
	    ;; 
	-p)
	    shift
	    project_name=$1
	    ;; 
	-m)
	    shift
	    mode=$1
	    ;; 
        *)
	   mode=$1
	   ;;
       esac
       shift
done
  local saved_dir=`pwd`
  local files_list_f=`mktemp`
  local files_list_tmp=`mktemp`
  local vivo_top=vendor/vivo
  local overlay_top=$vivo_top/overlay/resources
  local vivo_out=$vivo_top/out
  local project_out=$vivo_out/$project_name
  local files_list_fb=$saved_dir/$project_out/.files-overlay-res
  local files_list_fb2=$saved_dir/$project_out/.files-overlay-res2
  local files_list_add=$saved_dir/$project_out/.files-overlay-res-add
  local file_suffixs="png jpg bmp xml"
  local file_suffixs2="Makefile Kconfig config"
  local git_svn_st="svn st"
  local git_svn_revert="svn revert -q"
  local project_overlay=$project_out/overlay/resources
  local project_adds=vendor/vivo/out/$project_name/adds
  local dirs_list=$overlay_top/common
  local files_list=
  local files_list_revert=
  local files_list_delete=
  local ac_files="$files_list_fb $files_list_fb2 $files_list_add"
  local project_out=$vivo_out/$project_name
  if [ -z "$project_name" ]; then
    return
  fi
  if [ -n "$vivo_rom" ]; then
    dirs_list="$dirs_list  $overlay_top/${vivo_rom}"
  fi
  start_time=`sh_get_date_and_time2`
  echoTextGreen "====> 在目录 $dirs_list 中搜索文件......" >&2
  rm $ac_files -rf; touch $ac_files
  for d in $dirs_list
  do
    if test -d $d; then
      find.sh  -d $d -t "$file_suffixs" -e "$file_suffixs2">> $files_list_fb2
      cd $d
      find.sh  -t "$file_suffixs"  -e "$file_suffixs2">> $files_list_f
      cd $saved_dir
    fi
  done
   files_list=`cat $files_list_f |sort|uniq`
  #恢复操作
  if [ "$mode" != "print" ] && [ -n "$files_list" ]; then
    echoTextGreen "====> 文件恢复......"
    st_rm=no
    if [ "$st_rm" = "yes" ]; then
      $git_svn_st frameworks/base/core/res | grep -v Android.mk | awk '{print $2}' | xargs rm -rf
    else
      files_list_revert=`$git_svn_st  $files_list 2>$files_list_tmp |grep  -w M |awk '{print $2}'`
      files_list_delete=`$git_svn_st  $files_list 2>$files_list_tmp |grep   ? |awk '{print $2}'`
      files_list_delete="`cat $files_list_tmp  |tr '\“\”' ' '|awk '{print $3}'` $files_list_delete"
      if [ -n "$files_list_delete" ]; then 
        rm $files_list_delete -rf
      fi
      if [ -n "$files_list_revert" ]; then
        $git_svn_revert  $files_list_revert
      fi
      $git_svn_st frameworks/base/core/res |grep ?|awk '{print $2}' | xargs rm -rf
    fi
  fi
  #文件拷贝
  echoTextGreen "====> 文件拷贝......"
  for f in $files_list
  do
    ac_file=
    ac_dir=
    for d in $dirs_list
    do
      if test ! -f $f; then
        ac_dir=$d
        ac_file=$d/$f
      fi
      if [ -n "$ac_file" ] && test -f $ac_dir/$f; then
        cd $ac_dir
        if [ "$mode" = "print" ] || [ "$mode" = "recovery" ]; then
          cp --parents -rfpL  $f   $saved_dir/$project_adds
        else
          cp --parents -rfpL $f   $saved_dir
        fi
        echo $f >> $files_list_add
        cd $saved_dir      
      fi
    done
  done
  ls $ac_files
  rm $files_list_f -rf
}

function sh_copy_vendor_vivo_overlay()
{
  local mode=
  local android_version=
  local product_solution=
  local project_name=
while test $# != 0
do
    case $1 in
	-a)
	    shift
	    android_version=$1
	    ;; 
	-s)
	    shift
	    product_solution=$1
            product_solution=`sh_uppertolower $product_solution`
	    ;; 
	-p)
	    shift
	    project_name=$1
	    ;; 
	-sp)
	    shift
	    platform_overlay=$1
	    ;; 
	-m)
	    shift
	    mode=$1
	    ;; 
	-donly)
	    shift
	    driver_only=$1
	    ;; 
        *)
	   mode=$1
	   ;;
       esac
       shift
done
  #4.2 qcom $project_name
  local vivo_top=vendor/vivo
  local overlay_top=vendor/vivo/overlay
  local adds_top=vendor/vivo/adds
  local vivo_out=vendor/vivo/out
  local project_out=$vivo_out/$project_name
  local project_overlay=$project_out/overlay
  local project_adds=$project_out/adds
  local files_list=
  local dirs_list=
  local files_list_revert=
  local files_list_revert2=
  local files_list_delete=
  local saved_dir=`pwd`
  local file_prefixs_cp_donly="bootable/bootloader  kernel mediatek" #driver only 版本拷贝
  local file_prefixs_cp="bootable kernel hardware mediatek" #文件路径bootable等开头,不存在也拷贝
  local file_suffixs_cp="xxx" #文件路径aidl等结尾,不存在也拷贝
  local file_suffixs="c cpp cxx h java aidl mk prop rc sh pl py bmp idl i dws pm pk8 pem fstab so" #搜索后缀为c cpp结尾的文件
  local file_suffixs2="Makefile Kconfig config files" #搜索后缀为config等无后缀的文件
  local files_list_f=`mktemp`
  local files_list_tmp=`mktemp`
  local files_list_fb=$saved_dir/$project_out/.files-overlay-list
  local files_list_fb2=$saved_dir/$project_out/.files-overlay-list2
  local files_list_frm=$saved_dir/$project_out/.files-overlay-in-framework
  local files_list_vendor=$saved_dir/$project_out/.files-overlay-in-vendor
  local adds_list_fb=$saved_dir/$project_out/.files-adds-list
  local adds_list_fb2=$saved_dir/$project_out/.files-adds-list2
  local adds_list_frm=$saved_dir/$project_out/.files-adds-in-framework
  local adds_list_vendor=$saved_dir/$project_out/.files-adds-in-vendor
  local dirs_list_adds=$saved_dir/$project_out/.dirs_list_should_delete
 
  local ac_files="$files_list_fb $files_list_fb2 $files_list_frm $files_list_vendor $adds_list_fb $adds_list_fb2 $adds_list_frm $adds_list_vendor $dirs_list_adds"
  local ac_dirs="$project_overlay $project_adds"
  local dirs_list_common=$overlay_top/common
  local dirs_list_android=$overlay_top/android/`sh_get_android_PLATFORM_VERSION`
  local dirs_list_androids=`ls $overlay_top/android/* -d 2>/dev/null | grep -v common | sort -n` 
  local dirs_list_platfrom="$overlay_top/$product_solution/common $overlay_top/$product_solution/`sh_get_android_PLATFORM_VERSION`"
  local dirs_list_platfroms=`ls $overlay_top/{mtk,qcom}/* -d 2>/dev/null |sort -n`
  local dirs_list_projects=`ls $overlay_top/project/PD* -d 2>/dev/null`
  local dirs_list_project=$overlay_top/project/$project_name
  local st_file=$vivo_top/.flag_copy_vendor_vivo_files
  #local git_svn_st="git status -s"
  local git_svn_st="svn st"
  local git_svn_revert="svn revert -q"
  if test ! -f $st_file; then
    return
  fi
  if [ -n "$file_prefixs_cp" ] && [ -n "$file_suffixs_cp" ]; then
    local ac_file_prefixs=`sh_convert_strs_to_grep_e_filter_start "$file_prefixs_cp"`
    local ac_file_suffixs=`sh_convert_strs_to_grep_e_filter_start "$file_suffixs_cp"`
    local ac_file_prefixs_donly=`sh_convert_strs_to_grep_e_filter_start "$file_prefixs_cp_donly"`
  fi

  #ac_pid="`pidof make`"
  #ac_ppid=`cat /proc/$ac_pid/status | grep PPid: |awk '{print $2}'`
  #ac_name=`ps  |grep  $ac_ppid -w  | grep bash | awk '{print $NF}' ` >&2
  local ac_count=
  local is_make=`ps  |tr -d '.' | grep make -w`
  #ac_pids=`ps  | grep bash |awk '{print $1}'`
  ac_count=`ps  | grep bash |wc -l`
  if [ -n "$is_make" ] && [ "$ac_count" -ne "1" ]; then
    return
  else
    echo "====>拷贝文件......"
  fi
  echo "$android_version $product_solution $project_name"
  if [ -z "$android_version" ] || [ -z "$product_solution" ] || [ -z "$project_name" ]; then 
    echoTextRed "sh_copy_vendor_vivo_overlay 4.2 qcom PD1225"
    return; 
  fi
  rm $ac_dirs $ac_files -rf; mkdir -p $ac_dirs; touch $ac_files
  #项目多了,如果搜索性能下降,那么可以考虑只搜索特定项目和特定android版本
  #projects_list=`sh_get_projects_list`
  #for p in $projects_list ; do dirs_list_projects="$overlay_top/project/$p $dirs_list_projects" ; done
  echoTextGreen "====> start `date`"
  start_time=`sh_get_date_and_time2`
  echoTextGreen "====> 开始文件搜索......" >&2
  dirs_list="$dirs_list_common $dirs_list_androids  $dirs_list_platfroms $dirs_list_projects"
  #echo "dirs_list=$dirs_list"
  for d in $dirs_list
  do
    if test -d $d; then
      #此处可以改善性能
      find.sh  -d $d -t "$file_suffixs" -e "$file_suffixs2">> $files_list_fb2
      cd $d
      find.sh  -t "$file_suffixs"  -e "$file_suffixs2">> $files_list_f
      cd $saved_dir
    fi
  done
  cat $files_list_f |sort|uniq>$files_list_fb
  sed -i "s#\.\/##g"  $files_list_fb #去掉开头的./
  files_list=`cat $files_list_fb |sort|uniq`
  
  force_copy=yes
  force_rm=no
  #如果需要支持文件不存在也拷贝,那么force_copy=yes 
  if [ "$mode" != "print" ] && [ -n "$files_list" ]; then
    echoTextGreen "====> 拷贝之前删除旧文件......" >&2
    files_list_revert=`$git_svn_st  $files_list 2>$files_list_tmp |grep  -w M |awk '{print $2}'`
    files_list_delete=`$git_svn_st  $files_list 2>$files_list_tmp |grep   ? |awk '{print $2}'`
    files_list_delete="`cat $files_list_tmp  |tr '\“\”' ' '|awk '{print $3}'` $files_list_delete"
    if [ -n "$files_list_delete" ]; then 
      rm $files_list_delete -rf
    fi
  fi
  echoTextGreen "====> 开始拷贝......" >&2
  #echo "platform_overlay=$platform_overlay" >&2
  if [ -n "$platform_overlay" ]; then
    dirs_list_platfrom="$dirs_list_platfrom $overlay_top/$product_solution/$platform_overlay"
  fi
  dirs_list="$dirs_list_common $dirs_list_android  $dirs_list_platfrom $dirs_list_project"
  #echo "dirs_list=$dirs_list" >&2
  for f in $files_list
  do
    f=`echo $f|sed "s#\.\/##g"`
    if [ "$driver_only" = "yes" ]; then 
      f=`echo $f |grep -E "$ac_file_prefixs_donly"` #只拷贝kernel boot 等开头的文件
      if [ -z "$f" ]; then 
        continue
      fi
    fi
    ac_file=
    ac_dir=
    for d in $dirs_list
    do
      if test -f $d/$f; then
        ac_dir=$d
        ac_file=$d/$f
      fi
    done

    if [ -n "$ac_file" ] && test -f $saved_dir/$f; then
      cd $ac_dir
      if [ "$mode" = "print" ] || [ "$mode" = "recovery" ]; then
        cp --parents -rfpL $f   $saved_dir/$project_overlay
      else
        cp --parents -rfpL $f   $saved_dir
      fi
      echo $f >> $files_list_frm
      echo "$ac_file">>$files_list_vendor
      cd $saved_dir      
    fi
    #不存在也拷贝,此功能暂时屏蔽(通过设置 force_copy=no)
    #只拷贝特定文件,针对特定平台(mtk)
    if [ -n "$ac_file" ] && test ! -f $saved_dir/$f; then     
      echo $f >> $adds_list_fb
      echo "$ac_file">>$adds_list_fb2
      if [ "$force_copy" = "yes" ]; then   
        f=`echo $f |grep -E "$ac_file_prefixs|$ac_file_suffixs"` #只拷贝特殊文件
        if [ -z "$f" ]; then continue; fi
        cd $ac_dir
        if [ "$mode" = "print" ] || [ "$mode" = "recovery" ]; then
          cp --parents -rfpL  $f   $saved_dir/$project_overlay
        else
          cp --parents -rfpL $f   $saved_dir
        fi
        echo $f >> $adds_list_frm
        echo "$ac_file">>$adds_list_vendor
        cd $saved_dir       
      fi
    fi
  done

  #拷贝列表之外,处于M状态的文件进行恢复
  #files_list_revert_ex=frameworks/base/packages/SystemUI/Android.mk
  #files_list_revert="$files_list_revert $files_list_revert_ex"
  if [ "$mode" = "recovery" ]; then
    if [ -n "$files_list_revert" ]; then
      echoTextGreen "====> 文件恢复中......" >&2
      $git_svn_revert  $files_list_revert
    fi
  else
    if [ -n "$files_list_revert" ]; then
      for f in $files_list_revert
      do
        result=`cat $files_list_frm |grep  $f -w`
        if [ -z "$result" ]; then
          files_list_revert2="$files_list_revert2 $f"
        fi
      done    
      if [ -n "$files_list_revert2" ]; then
        $git_svn_revert  $files_list_revert2
      fi
    fi
  fi

  rm $files_list_f $files_list_tmp -rf
  if [ "$force_copy" = "yes" ] && [ "$force_rm" = "yes" ]; then
    echoTextGreen "====> 处理拷贝过程创建的目录......"
    rm $files_list_f $files_list_tmp -rf
    cat $files_list_fb >> $files_list_f
    cat $adds_list_fb >> $files_list_f 
    for a in `cat  $files_list_f |sort|uniq`
    do
      dirname $a >>$files_list_tmp
    done
    cat $files_list_tmp
    $git_svn_st `cat $files_list_tmp | sort |uniq ` 2>/dev/null |grep ?  |awk '{print $2}'>> $dirs_list_adds
  fi
  echoTextGreen "====> 拷贝完成......" >&2
  #显示结果
  for f in $ac_files
  do
    ac_f=`echo $f |sed "s#${saved_dir}/##g"`
    echoTextBlue "对文件 $ac_f 进行排序......"
    cat $f |sort|uniq>$files_list_tmp
    cp $files_list_tmp $f -rf
  done
  if [ "$mode" = "print" ] && test -f  $files_list_vendor; then
echoTextGreen "
$project_overlay 存放覆盖的文件
$project_adds 存放添加的文件
=======文件路径信息 vendor/vivo 下======
cat $files_list_vendor
cat $adds_list_vendor
=======文件路径信息 frameworks 下======
cat $files_list_frm
cat $adds_list_frm"
  fi
  rm $files_list_f $files_list_tmp -rf
  rm $st_file -rf
  ## 
  end_time=`sh_get_date_and_time2`
  ((sun_time=${end_time}-${start_time}))
  echoTextGreen "====> end `date`"
  echoTextGreen "====> 耗时: $sun_time 秒" >&2
  echo "====> calling sh_copy_vendor_vivo_overlay ">>/tmp/counts
}

function sh_copy_vendor_vivo_source()
{
  local project_name=PD1225
  local vivo_rom=
while test $# != 0
do
    case $1 in
	-v)
	    shift
	    vivo_rom=$1
	    ;; 
	-p)
	    shift
	    project_name=$1
	    ;; 
	-m)
	    shift
	    mode=$1
	    ;; 
        *)
	   mode=$1
	   ;;
       esac
       shift
done
  local saved_dir=`pwd`
  local files_list_f=`mktemp`
  local files_list_tmp=`mktemp`
  local vivo_top=vendor/vivo
  local source_top=$vivo_top/source
  local vivo_out=$vivo_top/out
  local project_out=$vivo_out/$project_name
  local files_list_fb=$saved_dir/$project_out/.files-list
  local files_list_fb2=$saved_dir/$project_out/.files-list2
  local files_list_frm=$saved_dir/$project_out/.files-source-in-framework
  local files_list_vendor=$saved_dir/$project_out/.files-source-in-vendor
  local file_suffixs="aidl"
  local git_svn_st="svn st"
  local git_svn_revert="svn revert -q"
  local frm_aidl_src=$source_top/common/frameworks-ext
  local frm_aidl_dst=$saved_dir/frameworks
  local files_list=
  local ac_files="$files_list_fb $files_list_fb2"
  local project_out=$vivo_out/$project_name	
  echoTextGreen "====> 在目录 $frm_aidl_src 中搜索aidl......" >&2
  rm $ac_files -rf; touch $ac_files
  find.sh  -d  $source_top/common/frameworks-ext -t "$file_suffixs" >> $files_list_fb2
  cd $source_top/common/frameworks-ext
  find.sh  -t "$file_suffixs" >> $files_list_f
  cd $saved_dir
  files_list=`cat $files_list_f|sort|uniq`
  cd $frm_aidl_src
  for f in $files_list
  do
    echo $f>>$files_list_fb
    cp --parents -rfpL $f   $frm_aidl_dst
  done
  cd $saved_dir
}


function sh_ccache_config()
{
	local root_dir=`pwd`
	export USE_CCACHE=1
	export CCACHE_DIR=$root_dir/.ccache
	if test ! -d $CCACHE_DIR;then
		echo "creating ccache........."
		$root_dir/prebuilts/misc/linux-x86/ccache/ccache -M 20G
	else
		echo "ccache already exist"
	fi
}

function  sh_get_product_solution()
{
  local  product_solution=mtk
  if test -d mediatek; then
    product_solution=mtk
  elif test -d vendor/qcom;then
    product_solution=qcom
  fi
  echo $product_solution
}

function is_svn_working_copy()
{  
    if test -d $1/.svn ;then
        return 0
    else 
        return 1
    fi   
}


function sh_get_svn_url()
{
    local ac_url=
    echo "is_svn_working_copy $1" >&2
    if is_svn_working_copy $1; then
        ac_url=`svn info  $1 |grep "URL:" |awk '{print $2}'`
    else
    ac_url=`svn info  $1 |grep "URL:" |awk '{print $2}'`
    fi 
    echo $ac_url  
}
function sh_get_make_ext4_flags()
{
  local ac_dir=$1
  local system_size_file=
  local file_contexts=
  local ac_make_ext4fs=
  local system_size=
  local make_ext4_flags=
  if [ "$ac_dir" = "" ];then
    ac_dir=`pwd`
  fi
  if [ -d $ac_dir/sdk_files/make_system ];then
   system_size_file=$ac_dir/sdk_files/make_system/system_image_info.txt
   file_contexts=$ac_dir/sdk_files/make_system/file_contexts
   ac_make_ext4fs=$ac_dir/sdk_files/make_system/make_ext4fs
   if [ -f $system_size_file ];then
     system_size=`cat $system_size_file  |grep system_size |awk -F= '{print $2}'`
   fi
   if [ "$system_size" = "" ];then
     system_size="1024M"
   fi
   if [ -f $file_contexts ];then
     make_ext4_flags="-s -S $file_contexts -l $system_size -a"
   else
     make_ext4_flags="-s -l $system_size -a"
   fi
   if [ -f $ac_make_ext4fs ];then
     MAKE_EXT4FS=$ac_make_ext4fs
     export  MAKE_EXT4FS=$ac_make_ext4fs
   fi
  fi
  echo $make_ext4_flags
}
function sh_get_make_ext4fs()
{
  local ac_make_ext4fs=
  local ac_dir=$1
  if [ "$ac_dir" != "" ];then
    if [ -d $ac_dir ];then
      ac_make_ext4fs=$ac_dir/sdk_files/make_system/make_ext4fs
      if [ -f $ac_make_ext4fs ];then
        echo $ac_make_ext4fs
      fi
    fi
  fi
}

