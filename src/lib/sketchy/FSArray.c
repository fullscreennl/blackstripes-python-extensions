#include <stdio.h>
#include <stdlib.h>
#include "FSObject.h"
#include "FSArray.h"

FSArray *FSArray_alloc(int length)
{
    FSArray *p = (FSArray *) malloc(sizeof(FSArray));
    p->array = (void**) malloc(length*sizeof(void*));
    p->cursor = 0;
    p->length = length;
    p->retainCount = 1;
    p->type = "Array";
    return p;
}

int FSArray_count(FSArray *p){
    return p->length;
}

void *FSArray_objectAtIndex(FSArray *p,int index){
    return p->array[index];
}

void FSArray_append(FSArray *p,void *elem){
    FSObject_retain(elem);
    p->array[p->cursor] = elem;
    p->cursor ++;
}

void FSArray_release(FSArray *p){
    int i = 0;
    for(i=0; i < p->length; i++){
        void *elem = (void*)p->array[i];
        FSObject_release(elem);
    }
    p->retainCount --;
    if(p->retainCount == 0){
        free(p->array);
        free(p);
    }
}

void FSArray_retain(FSArray *p){
    FSObject_retain(p);
}







