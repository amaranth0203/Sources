/* overflow :
 *
 * using debug in visual studio to see the overflow position in string s in function main
 * using function printf to see the position of function test 
 * replace the overflow position with the position of funtion test
 *
 * 2013-6-7 22:47:50 writen
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void jump ( const char * s )
{
    char buf[10] ;
    strcpy ( buf , s ) ;
    return ;
}

void test ( void ) 
{
    puts ( "\n\t\t\t\t\t\t Wass up ! " ) ;
    system ( "pause>nul" ) ;
    exit ( 0 ) ;
}
/*
 * aaa 0040116C aaa
 * 表示函数test的地址是 0x0040116C
 * 把这个地址作为溢出地址有两个方法
 * 一个是char s[50] 里边的 \x6C\x11\x40\x00 (注意顺序倒过来了)
 * 还有一个是下边的 s[16] = 一直到 s[19] = 四句
 * 其中的值是6c 11 40 00从16进制转换成十进制的值
 * 顺序也倒过来了
 *
 */
int main()
{
    char s[50] = "abcdefghijklmnop\x6C\x11\x40\x00" ;
    printf ( " \n\taaa %p aaa\n" , test ) ;
    /*
    s[16] = 108 ;
    s[17] = 17 ;
    s[18] = 64 ;
    s[19] = 00 ;
    */
    jump ( s ) ;
    system ( "pause>nul" ) ;
    return 0 ;
}