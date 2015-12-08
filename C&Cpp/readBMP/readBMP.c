#include <stdio.h>
int main(int argc , char ** argv )
{
    FILE* f = fopen(argv[1], "rwb+");
    unsigned char info[54];
    fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header

    // extract image height and width from header
    int width = *(int*)&info[18];
    int height = *(int*)&info[22];
    printf( "\n %d %d\n" , width , height ) ;
    //printf( "\n %x %x %x %x %x %x %x %x\n" , info[18] , info[19] , info[20] , info[21] , info[22] , info[23] , info[24] , info[25] ) ;
    fclose(f) ;
    return 0 ;
}
