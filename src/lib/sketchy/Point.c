#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "Point.h"
#include "FSObject.h"
#include "FSArray.h"
#include "FSNumber.h"

Point *Point_alloc(float x, float y){
    Point *p = (Point *) malloc(sizeof(Point));
    p->x = x;
    p->y = y;
    p->retainCount = 1;
    p->type = "Point";
    return p;
}

void Point_copy(Point *src,Point *dest){
    dest->x = src->x;
    dest->y = src->y;
    dest->left_angle = src->left_angle;
    dest->right_angle = src->right_angle;
    dest->left_steps = src->left_steps;
    dest->right_steps = src->right_steps;
}

void Point_log(Point *p){
    printf("Point <%p left:%i l-angle:%f right:%i r-angle:%f - x:%f  y:%f >\n",
            p,p->left_steps,p->left_angle,p->right_steps,p->right_angle,p->x,p->y);
}

void Point_release(Point *p){
    FSObject_release(p);
}

void Point_retain(Point *p){
    FSObject_retain(p);
}

void Point_setNull(Point *p){
    p->x = 0.0;
    p->y = 0.0;
}



