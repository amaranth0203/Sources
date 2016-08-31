#include "funcs.h"
#include <stdio.h>
void test( ) {
    printf( "[+] wassup %s called\n" , __func__ ) ;
    return ;
}
void print( int* draft ) {
    int i , j ;
    for( i = 0 ; i < 9 ; i ++ ) {
        for( j = 0 ; j < 9 ; j ++ ) {
            printf( "%2d" , *(draft+9*i+j) ) ;
            if( j % 3 == 2 && j != 8 ) printf( " |" ) ;
        }
        if( i % 3 == 2 && i != 8 ) 
            printf ( "\n-------*-------*-------" ) ;
        puts( "" ) ;
    }
}
int check_idx( int* draft , int x ) {
    int i = x / 9 , j = x % 9 ;
    int m , n ;
    for( m = 0 ; m < 9 ; m ++ ) 
        if( ( 9*i + m ) != x && *( draft + 9*i + m ) == *( draft + x ) )
            return 0 ;
    for( m = 0 ; m < 9 ; m ++ )
        if( ( 9*m + j ) != x && *( draft + 9*m + j ) == *( draft + x ) )
            return 0 ;
    int boundary_i_start , boundary_i_end , boundary_j_start , boundary_j_end ;
    switch( i ) {
    case 0 : case 1 : case 2 :
        boundary_i_start = 0 ;
        boundary_i_end = 2 ;
    break ;
    case 3 : case 4 : case 5 :
        boundary_i_start = 3 ;
        boundary_i_end = 5 ;
    break ;
    case 6 : case 7 : case 8 :
        boundary_i_start = 6 ;
        boundary_i_end = 8 ;
    break ;
    }
    switch( j ) {
    case 0 : case 1 : case 2 :
        boundary_j_start = 0 ;
        boundary_j_end = 2 ;
    break ;
    case 3 : case 4 : case 5 :
        boundary_j_start = 3 ;
        boundary_j_end = 5 ;
    break ;
    case 6 : case 7 : case 8 :
        boundary_j_start = 6 ;
        boundary_j_end = 8 ;
    break ;
    }
    for( m = boundary_i_start ; m <= boundary_i_end ; m ++ )
        for( n = boundary_j_start ; n <= boundary_j_end ; n++ )
            if( ( m != i ) && ( n != j ) && ( 9*m + n ) != x && *( draft + 9*m + n ) == *( draft + x ) )
                return 0 ;
    return 1 ;
}


















