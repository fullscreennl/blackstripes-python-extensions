//class Point//

#ifndef POINT_H
#define POINT_H

typedef struct FSPoint{
    int retainCount;
    char *type;
    float x;
    float y;
    float left_angle;
    float right_angle;
    int left_steps;
    int right_steps;
}Point;

Point *Point_alloc(float x, float y);

Point *Point_allocWithXY(float x, float y);
void Point_updateWithXY(Point *p,float x, float y);

Point *Point_allocWithSteps(int left_steps, int right_steps);
void Point_updateWithSteps(Point *p,int left_steps, int right_steps);

void Point_copy(Point *src,Point *dest);
void Point_release(Point *p);
void Point_retain(Point *p);
void Point_setNull(Point *p);
void Point_log(Point *p);

#endif


