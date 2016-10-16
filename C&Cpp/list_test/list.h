#ifndef _LITS_H_
#define _LIST_H_

typedef struct _list {
    struct _list* prev ;
    struct _list* next ;
    void* data ;
} list ;
    
#endif

