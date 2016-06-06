//class FSObject//

#ifndef FSOBJECT_H
#define FSOBJECT_H

typedef struct FSObject{
    int retainCount;
    char *type;
}FSObject;

void FSObject_release(void *o);
void FSObject_retain(void *o);

#endif


