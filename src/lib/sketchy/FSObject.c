#include <stdio.h>
#include <stdlib.h>
#include "FSObject.h"

void FSObject_release(void *o){
    FSObject *obj = (FSObject*)o;
    obj->retainCount --;
    if(obj->retainCount == 0){
        free(obj);
    }
}

void FSObject_retain(void *o){
    FSObject *obj = (FSObject*)o;
    obj->retainCount ++;
}
