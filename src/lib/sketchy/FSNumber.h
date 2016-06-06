//class FSNumber//

#ifndef FSNUMBER_H
#define FSNUMBER_H

typedef struct FSNumber{
    int retainCount;
    char *type;
    float floatValue;
    float intValue;
}FSNumber;

FSNumber *FSNumber_allocWithInt(int intval);
FSNumber *FSNumber_allocWithFloat(float floatval);
void FSNumber_release(FSNumber *p);
void FSNumber_retain(FSNumber *p);

#endif


