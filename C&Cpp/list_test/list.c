#include <stdio.h>
#include <stdlib.h>
#include "list.h"

list* gene_list( ) {
    list* tmp = ( list* )malloc( sizeof( list ) ) ;
    tmp->next = tmp ;
    tmp->prev = tmp ;
    return tmp ;
}

list* list_append( list* l , void* data ) {
    list* tmp = gene_list( ) ;
    while( l->next != l )
        l = l->next ;
    l->next = tmp ;
    tmp->prev = l ;
    tmp->data = data ;
}

void* travel_list( void( tra )() , list* l , void* data ) {
    tra( l , data ) ;
    while( l->next != l ) {
        l = l->next ;
        tra( l , data ) ;
    }
}

void tra( list* l , void* data ) {
    printf( "%s " , l->data ) ;
}

int main( ) {
    list* l = gene_list( ) ;
    l->data = ( void* )"HEAD" ;
    int i ;
    char* buff ;
    for( i = 0 ; i < 100 ; i ++ ) {
        buff = ( char* )malloc( sizeof( char ) * 4 ) ;
        sprintf( buff , "%03d" , i ) ;
        list_append( l , buff ) ;
    }
    list_append( l , "TAIL" ) ;
    travel_list( tra , l , NULL ) ;
    return 0 ;
}








