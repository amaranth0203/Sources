#include "funcs.h"
#include <stdio.h>
#include <stdlib.h>
void test( ) {
    printf( "[+] wassup %s called\n" , __func__ ) ;
    return ;
}
void print( int* draft ) {
    // system( "cls" ) ;
    int i , j ;
    printf ( "\n----------*----------*----------\n" ) ;
    for( i = 0 ; i < 9 ; i ++ ) {
        for( j = 0 ; j < 9 ; j ++ ) {
            printf( "%3d" , *(draft+9*i+j) ) ;
            if( j % 3 == 2 && j != 8 ) printf( " |" ) ;
        }
        if( i % 3 == 2 ) 
            printf ( "\n----------*----------*----------" ) ;
        puts( "" ) ;
    }
}
void print2( int* draft , int* field , int len ) {
    int i , j ;
    int i_f ;
    printf ( "\n----------*----------*----------\n" ) ;
    i_f = 0 ;
    for( i = 0 ; i < 9 ; i ++ ) {
        for( j = 0 ; j < 9 ; j ++ ) {
            if( ( 9*i + j ) == field[i_f] ) {
                printf( "%3d" , *(draft+9*i+j) ) ;
                i_f ++ ;
            }
            else {
                printf( " *%d" , *(draft+9*i+j) ) ;
            }
            if( j % 3 == 2 && j != 8 ) printf( " |" ) ;
        }
        if( i % 3 == 2 ) 
            printf ( "\n----------*----------*----------" ) ;
        puts( "" ) ;
    }
}
int check_idx( 
  int* draft , 
  int x /* offset in 81 */
  ) {
  int i = x / 9 , j = x % 9 ; /* coordinate */
  int m , n ;
  for( m = 1 ; m < 8 ; m ++ ) 
    if( 
      ( 9*i + m ) != x && // not itself
      *( draft + 9*i + m ) == *( draft + x ) // same value in row
      )
      return 0 ;
  
  for( m = 1 ; m < 8 ; m ++ )
    if( 
      ( 9*m + j ) != x &&  // not itself
      *( draft + 9*m + j ) == *( draft + x ) // same value in column
      )
      return 0 ;

  return 1 ;
}
int check_view( int* d ) {
  return 1 ;
}
void re_calc( int* d , int* f , int start , int end ) {
  /*
   * f[start] stands for current offset in matrix to fill in
   */
  print( d ) ;
  printf( "start end %d %d\n" , start , end ) ;
  int i , j ;
  if( start == end && check_view( d ) ) {
    printf( "\n%d elements given" , 9*9 - end ) ;
    print( d ) ;
    puts( "done" ) ;
  } else {
    for( j = 2 ; j < 9 ; j ++ ) {
      d[f[start]] = j ;
      /* d[start] = j ; */
      if( !check_idx( d , f[start] ) ) {
      /* if( !check_idx( d , start ) ) { */
        d[start] = 2 ;
        continue ;
      }
      re_calc( d , f , start + 1 , end ) ;
      d[start] = 2 ;
    }
  }
}

void trans_input( long long* quick_input , int* draft ) {
  int i , ii ;
  for( ii = 0 ; ii < 9 ; ii ++ ) {
    for( i = 8 ; i >= 0 ; i -- ) {
      draft[ii*9+i] = quick_input[ii]%10 ;
      quick_input[ii] /= 10 ;
    }
  }
}
















