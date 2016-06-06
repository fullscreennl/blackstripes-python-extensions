//class FSArray//

#ifndef FSARRAY_H
#define FSARRAY_H

typedef struct FSArray{
    int retainCount;
    char *type;
    int cursor;
    int length;
    void** array;
}FSArray;

FSArray *FSArray_alloc(int length);
void FSArray_append(FSArray *p, void *elem);
void FSArray_release(FSArray *p);
void FSArray_retain(FSArray *p);
int FSArray_count(FSArray *p);
void *FSArray_objectAtIndex(FSArray *p,int index);

#endif


