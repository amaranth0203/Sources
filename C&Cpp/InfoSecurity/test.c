#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//remove 'Z' by default , use 'X' instead.
char dict[5][5] = {
{ 'A' , 'B' , 'C' , 'D' , 'E' } , 
{ 'F' , 'G' , 'H' , 'I' , 'J' } ,
{ 'K' , 'L' , 'M' , 'N' , 'O' } ,
{ 'P' , 'Q' , 'R' , 'S' , 'T' } ,
{ 'U' , 'V' , 'W' , 'X' , 'Y' } } ;
char* encode( char* text ) ;
char* decode( char* text ) ;
void removeSpace( char* text ) ;
void toUpperCase( char* lowerCase ) ;
void replaceZwithX( char* text ) ;
void insertQintoDoubleLetter( char* text ) ;
void addQtoOddString( char* text ) ;
void toLowerCase( char* upperCase ) ;
void removeQ( char* text ) ;
int main( )
{
    char text[512] = "Playfair cipher was actually invented by wheatstone" ;
//    char text[512] = "xjjjxjjjxjjj" ;
    char* tmp = text ;
    char* tmp_ ;
    char* tmp__ ;
    puts( text ) ;
    tmp_ = encode( tmp ) ;
    puts( tmp_ ) ;
    tmp__ = decode( tmp_ ) ;
    puts( tmp__ ) ;
    removeQ( tmp__ ) ;
    toLowerCase( tmp__ ) ;
    puts( tmp__ ) ;
    return 0 ;
}
char* encode( char* text )
{
    int x1 , y1 , x2 , y2 ;
    char* ptr = text ;
    removeSpace( text ) ;
    toUpperCase( text ) ;
    replaceZwithX( text ) ;
    insertQintoDoubleLetter( text ) ;
    addQtoOddString( text ) ;
    for( ; *ptr != '\0' ; ptr += 2 )
    {
        for( x1 = 0 ; x1 < 5 ; x1 ++ )
        {
            for( y1 = 0 ; y1 < 5 ; y1 ++ )
            {
                if( dict[x1][y1] == *ptr ) break ;
            }
            if( y1 == 5 ) continue ;
            if( dict[x1][y1] == *ptr ) break ;
        }
        for( x2 = 0 ; x2 < 5 ; x2 ++ )
        {
            for( y2 = 0 ; y2 < 5 ; y2 ++ )
                if( dict[x2][y2] == *(ptr+1) ) break ;
            if( y2 == 5 ) continue ;
            if( dict[x2][y2] == *(ptr+1) ) break ;
        }
        if( ( x1 != x2 ) && ( y1 != y2 ) ) // not same column or row
        {
            *ptr = dict[x1][y2] ;
            *(ptr+1) = dict[x2][y1] ;
        }
        else if( ( x1 == x2 ) && ( y1 != y2 ) ) // same row
        {
            *ptr = dict[x1][(y1+1)%5] ;
            *(ptr+1) = dict[x2][(y2+1)%5] ;
        }
        else if( ( x1 != x2 ) && ( y1 == y2 ) ) // same column
        {
            *ptr = dict[(x1+1)%5][y1] ;
            *(ptr+1) = dict[(x2+1)%5][y2] ;
        }
    }
    return text ;
}
char* decode( char* text )
{
    int x1 , y1 , x2 , y2 ;
    char* ptr = text ;
    for( ; *ptr != '\0' ; ptr += 2 )
    {
        for( x1 = 0 ; x1 < 5 ; x1 ++ )
        {
            for( y1 = 0 ; y1 < 5 ; y1 ++ )
                if( dict[x1][y1] == *ptr ) break ;
            if( y1 == 5 ) continue ;
            if( dict[x1][y1] == *ptr ) break ;
        }
        for( x2 = 0 ; x2 < 5 ; x2 ++ )
        {
            for( y2 = 0 ; y2 < 5 ; y2 ++ )
                if( dict[x2][y2] == *(ptr+1) ) break ;
            if( y2 == 5 ) continue ;
            if( dict[x2][y2] == *(ptr+1) ) break ;
        }
        if( ( x1 != x2 ) && ( y1 != y2 ) ) // not same column or row
        {
            *ptr = dict[x1][y2] ;
            *(ptr+1) = dict[x2][y1] ;
        }
        else if( ( x1 == x2 ) && ( y1 != y2 ) ) // same row
        {
            *ptr = dict[x1][(y1+4)%5] ;
            *(ptr+1) = dict[x2][(y2+4)%5] ;
        }
        else if( ( x1 != x2 ) && ( y1 == y2 ) ) // same column
        {
            *ptr = dict[(x1+4)%5][y1] ;
            *(ptr+1) = dict[(x2+4)%5][y2] ;
        }
        else
        {
            puts( "?" ) ;
        }
    }
    return text ;
}
void toUpperCase( char* lowerCase )
{
    char* ptr = lowerCase ;
    while( *ptr != '\0' )
    {
        if( *ptr >= 'a' && * ptr <= 'z' )
        {
            *ptr = *ptr - ( 'a' - 'A' ) ;
        }
        ptr ++ ;
    }
}
void toLowerCase( char* upperCase )
{
    char* ptr = upperCase ;
    while( *ptr != '\0' )
    {
        if( *ptr >= 'A' && * ptr <= 'Z' )
        {
            *ptr = *ptr + ( 'a' - 'A' ) ;
        }
        ptr ++ ;
    }
}
void insertQintoDoubleLetter( char* text )
{
    int len = strlen( text ) ;
    int counter ; //how many times for double letter appearing
    int index , j ;
    for( index = 0 , counter = 0 ; index < len - 1 ; )
    {
        if( text[index] == text[index+1] )
        {
            counter ++ ;
            index += 2 ;
        }
        else
        {
            index += 2 ;
        }
    }
    for( index = 0 ; index < len + counter - 1 ; )
    {
        if( text[index] == text[index+1] )
        {
            for( j = len + counter - 1 ; j > index ; j -- )
                text[j] = text[j-1] ;
            text[index+1] = 'Q' ;
            index += 2 ;
        }
        else
        {
            index ++ ;
        }
    }
}
void addQtoOddString( char* text )
{
    if( strlen( text ) % 2 == 1 )
    {
        text[strlen(text)] = 'Q' ;
        text[strlen(text)+1] = '\0' ;
    }
}
void removeSpace( char* text )
{
    char* ptr ;
    char* ptrTmp ;
    ptr = text ;
    while( *ptr != '\0' )
    {
        if( *ptr == ' ' )
        {
            ptrTmp = ptr ;
            while( *(ptrTmp+1) != '\0' )
            {
                *ptrTmp = *(ptrTmp+1) ;
                ptrTmp++ ;
            }
            *ptrTmp = *(ptrTmp+1) ;
        }
        ptr ++ ;
    }
}
void removeQ( char* text )
{
    char* ptr ;
    char* ptrTmp ;
    ptr = text ;
    while( *ptr != '\0' )
    {
        if( *ptr == 'Q' )
        {
            ptrTmp = ptr ;
            while( *(ptrTmp+1) != '\0' )
            {
                *ptrTmp = *(ptrTmp+1) ;
                ptrTmp++ ;
            }
            *ptrTmp = *(ptrTmp+1) ;
        }
        ptr ++ ;
    }
}
void replaceZwithX( char* text )
{
    char* ptr = text ;
    while( *ptr != '\0' )
    {
        if( *ptr == 'Z' )
            *ptr = 'X' ;
        ptr ++ ;
    }
}
