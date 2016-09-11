#include "funcs.h"
#include <stdio.h>
#include <string.h>

void test( ) {
    puts( "[+] wassup test" ) ;
}

float calculate( float operand1 , float operand2 , char operator ) {
    float ret = 0 ;
    if( operator == '+' ) {
        ret = operand1 + operand2 ;
    }
    else if( operator == '-' ) {
        ret = operand1 - operand2 ;
    }
    return ret ;
}

float evaluateExpression( char* str ) {
    float operand1 = 0 ;
    float operand2 = 0 ;
    char  operator = 0 ;
    int i ;
    for( i = 0 ; i < strlen( str ) ; i ++ ) {
        char ch = str[i] ;
        if( '0' <= ch && ch <= '9' ) {
            if( operator == 0 ) {
                operand1 = operand1 * 10 + ch - '0' ;
            }
            else {
                operand2 = operand2 * 10 + ch - '0' ;
            }
        }
        else if( ch == '+' || ch == '-' ) {
            if( operator == 0 ) {
                operator = ch ;
            }
            else {
                operand1 = calculate( operand1 , operand2 , operator ) ;
                operand2 = 0 ;
                operator = ch ;
            }
        }
    }
    operand1 = calculate( operand1 , operand2 , operator ) ;
    return operand1 ;
}
