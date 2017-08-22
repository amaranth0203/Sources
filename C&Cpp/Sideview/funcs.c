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
    for( i = 0 ; i < (MATRIX_LONG+2) ; i ++ ) {
      for( j = 0 ; j < (MATRIX_LONG+2) ; j ++ ) {
            /* printf( "%3d" , *(draft+9*i+j) ) ; */
            printf( "%2d" , *(draft+9*i+j) ) ;
            /* if( j % 3 == 2 && j != 8 ) printf( " |" ) ; */
        }
        /* if( i % 3 == 2 )  */
            /* printf ( "\n----------*----------*----------" ) ; */
        puts( "" ) ;
    }
    /* printf( "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n" ) ; */
    
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
int check_duplicate( 
  int* draft , 
  int x /* offset in 81 */
  ) {
  int i = x / 9 , j = x % 9 ; /* coordinate */
  int m , n ;
  for( m = 1 ; m < 8 ; m ++ ) 
    if( 
      ( 9*i + m ) != x && // not itself
      *( draft + x ) != 2 && // skip blank
      *( draft + 9*i + m ) == *( draft + x ) // same value in row
      )
      return 0 ;
  
  for( m = 1 ; m < 8 ; m ++ )
    if( 
      ( 9*m + j ) != x &&  // not itself
      *( draft + x ) != 2 && // skip blank
      *( draft + 9*m + j ) == *( draft + x ) // same value in column
      )
      return 0 ;

  return 1 ;
}

int check_blank_count( int* draft ) {
  /* TO BE DONE */
  ( void* )draft ;

  int m , n ;
  int max = 3 ;
  int count_2_max ;
  int count_2 ;
  for( m = 1 ; m < 8 ; m ++ ) {
    if( *( draft + 9*0 + m ) > max ) max = *( draft + 9*0 + m ) ;
  }
  for( m = 1 ; m < 8 ; m ++ ) {
    if( *( draft + 9*8 + m ) > max ) max = *( draft + 9*8 + m ) ;
  }
  for( n = 1 ; n < 8 ; n ++ ) {
    if( *( draft + 9*n + 0 ) > max ) max = *( draft + 9*n + 0 ) ;
  }
  for( n = 1 ; n < 8 ; n ++ ) {
    if( *( draft + 9*n + 8 ) > max ) max = *( draft + 9*n + 8 ) ;
  }
  count_2_max = 9 - max ;

  for( m = 1 ; m < 8 ; m ++ ) {
    count_2 = 0 ;
    for( n = 1 ; n < 8 ; n ++ ) {
      if( *( draft + 9*n + m ) == 2 ) count_2 ++ ;
    }
    if( count_2 > count_2_max ) return 0 ;
  }

  for( m = 1 ; m < 8 ; m ++ ) {
    count_2 = 0 ;
    for( n = 1 ; n < 8 ; n ++ ) {
      if( *( draft + 9*m + n ) == 2 ) count_2 ++ ;
    }
    if( count_2 > count_2_max ) return 0 ;
  }

  return 1 ;
}

int check_view( 
  int* draft , 
  int x /* offset in 81 */
  ) {
  int i = x / 9 , j = x % 9 ; /* coordinate */
  int m , n ;

  // top view
  for( m = 1 ; m < 8 ; m ++ ) {
    if( *( draft + 9*0 + m ) == 0 ) { // needn't check view
      continue ;
    }
    if( *( draft + 9*1 + m ) == 1 ) { // un_filled
      continue ;
    }
    else if( *( draft + 9*1 + m ) == 2 ){ // fill blank , search down
      for( n = 2 ; n < 8 ; n ++ ) {
        if( *( draft + 9*n + m ) == 1 ) { // this line unfinished
          break ;
        }
        else if( *( draft + 9*n + m ) == 2 ) { // continue search
          continue ;
        }
        else { // check view
          if( *( draft + 9*n + m ) != *( draft + 9*0 + m ) ) {
            return 0 ;
          }
          break ;
        }
      }
    }
    else { // fill other, need to check view
      if( *( draft + 9*1 + m ) != *( draft + 9*0 + m ) ) {
        return 0 ;
      }
    }
  }

  // left view
  for( m = 1 ; m < 8 ; m ++ ) {
    if( *( draft + 9*0 + m ) == 0 ) { // needn't check view
      continue ;
    }
    if( *( draft + 9*m + 1 ) == 1 ) { // un_filled
      continue ;
    }
    else if( *( draft + 9*m + 1 ) == 2 ){ // fill blank , search down
      for( n = 2 ; n < 8 ; n ++ ) {
        if( *( draft + 9*m + n ) == 1 ) { // this line unfinished
          /* printf( "[+] search %d %d %d line unfinished\n" , m , n , *(draft+9*m+n) ) ; */
          break ;
        }
        else if( *( draft + 9*m + n ) == 2 ) { // continue search
          /* printf( "[+] search %d %d %d continue search\n" , m , n , *(draft+9*m+n) ) ; */
          continue ;
        }
        else { // check view
          if( *( draft + 9*m + n ) != *( draft + 9*m + 0 ) ) {
            return 0 ;
          }
          /* printf( "[+] search %d %d %d pass\n" , m , n , *(draft+9*m+n) ) ; */
          break ;
        }
      }
    }
    else { // fill other, need to check view
      if( *( draft + 9*m + 1 ) != *( draft + 9*m + 0 ) ) {
        return 0 ;
      }
    }
  }

  // right view
  for( m = 1 ; m < 8 ; m ++ ) {
    if( *( draft + 9*0 + m ) == 0 ) { // needn't check view
      continue ;
    }
    if( *( draft + 9*m + 7 ) == 1 ) { // un_filled
      continue ;
    }
    else if( *( draft + 9*m + 7 ) == 2 ){ // fill blank , search down
      for( n = 6 ; n > 0 ; n -- ) {
        if( *( draft + 9*m + n ) == 1 ) { // this line unfinished
          /* printf( "[+] search %d %d %d line unfinished\n" , m , n , *(draft+9*m+n) ) ; */
          break ;
        }
        else if( *( draft + 9*m + n ) == 2 ) { // continue search
          /* printf( "[+] search %d %d %d continue search\n" , m , n , *(draft+9*m+n) ) ; */
          continue ;
        }
        else { // check view
          if( *( draft + 9*m + n ) != *( draft + 9*m + 8 ) ) {
            return 0 ;
          }
          /* printf( "[+] search %d %d %d pass\n" , m , n , *(draft+9*m+n) ) ; */
          break ;
        }
      }
    }
    else { // fill other, need to check view
      if( *( draft + 9*m + 7 ) != *( draft + 9*m + 8 ) ) {
        return 0 ;
      }
    }
  }

  // bottom view
  for( m = 1 ; m < 8 ; m ++ ) {
    if( *( draft + 9*0 + m ) == 0 ) { // needn't check view
      continue ;
    }
    if( *( draft + 9*7 + m ) == 1 ) { // un_filled
      continue ;
    }
    else if( *( draft + 9*7 + m ) == 2 ){ // fill blank , search down
      for( n = 6 ; n > 0 ; n -- ) {
        if( *( draft + 9*n + m ) == 1 ) { // this line unfinished
          /* printf( "[+] search %d %d %d line unfinished\n" , m , n , *(draft+9*n+m) ) ; */
          break ;
        }
        else if( *( draft + 9*n + m ) == 2 ) { // continue search
          /* printf( "[+] search %d %d %d continue search\n" , m , n , *(draft+9*n+m) ) ; */
          continue ;
        }
        else { // check view
          if( *( draft + 9*n + m ) != *( draft + 9*8 + m ) ) {
            return 0 ;
          }
          /* printf( "[+] search %d %d %d pass\n" , m , n , *(draft+9*n+m) ) ; */
          break ;
        }
      }
    }
    else { // fill other, need to check view
      if( *( draft + 9*7 + m ) != *( draft + 9*8 + m ) ) {
        return 0 ;
      }
    }
  }

  return 1 ;
}
void re_calc( int* d , int* f , int start , int end ) {
  /*
   * f[start] stands for current offset in matrix to fill in
   */
  /* print( d ) ; */
  /* printf( "start end %d %d\n" , start , end ) ; */
  int i , j ;
  if( start == end && check_view( d , f[start] ) ) {
    printf( "\n%d elements given" , 9*9 - end ) ;
    print( d ) ;
    puts( "done" ) ;
  } else {
    for( j = 2 ; j < 8 ; j ++ ) {
      d[f[start]] = j ;
      /* d[start] = j ; */
      if( 
        !check_duplicate( d , f[start] ) || 
        !check_view( d , f[start] ) ||
        !check_blank_count( d )
        ) {
      /* if( !check_idx( d , start ) ) { */
        d[f[start]] = 1 ;
        continue ;
      }
      re_calc( d , f , start + 1 , end ) ;
      d[f[start]] = 1 ;
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
















