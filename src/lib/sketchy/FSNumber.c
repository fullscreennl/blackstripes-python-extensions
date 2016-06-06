#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "FSObject.h"
#include "FSNumber.h"

FSNumber *FSNumber_allocWithInt(int intval){
    FSNumber *n = (FSNumber *) malloc(sizeof(FSNumber));
    n->intValue = intval;
    n->floatValue = (float)intval;
    n->retainCount = 1;
    n->type = "Number";
    return n;
}

FSNumber *FSNumber_allocWithFloat(float floatval){
    FSNumber *n = (FSNumber *) malloc(sizeof(FSNumber));
    n->intValue = (int)round(floatval);
    n->floatValue = floatval;
    n->retainCount = 1;
    n->type = "Number";
    return n;
}

void FSNumber_release(FSNumber *n){
    FSObject_release(n);
}

void FSNumber_retain(FSNumber *n){
    FSObject_retain(n);
}






