#ifndef SKETCHYIMAGE_H
#define SKETCHYIMAGE_H

#include "bool.h"
#include "Point.h"

typedef struct SketchyImage{
    int retainCount;
    char *type;
    unsigned char *imageData;
    unsigned char *outputImageData;
    float scaleFactor;
    float xCorrection;
    float yCorrection;
    float xOffset;
    float yOffset;
    long int brightness;
    long int outputBrightness;
    float avgBrightness;
    unsigned width;
    unsigned height;
    int nibsize;
}SketchyImage;

SketchyImage *SketchyImage_allocWithFileName(const char *filename);
void SketchyImage_setNibSize(SketchyImage *obj,int nibsize);
void SketchyImage_release(SketchyImage *obj);
void SketchyImage_retain(SketchyImage *obj);
void SketchyImage_saveAsPNG(SketchyImage *obj,const char *name);
void SketchyImage_saveStateAsPNG(SketchyImage *obj,const char *name);
float SketchyImage_avgDarknessForLine(SketchyImage *obj, float x1, float y1, float x2, float y2);
Point *SketchyImage_bestPointOfNDestinationsFromXY2(SketchyImage *obj, int radius, int x, int y);
long int SketchyImage_getBrightness(SketchyImage *obj);
long int SketchyImage_getOutputBrightness(SketchyImage *obj);
int SketchyImage_getCanvasWidth(SketchyImage *obj);
int SketchyImage_getCanvasHeight(SketchyImage *obj);
int SketchyImage_getPixel(SketchyImage *obj,int x, int y);
Point *SketchyImage_getDarkPixel(SketchyImage *obj);
float SketchyImage_getAvgBrightness(SketchyImage *obj);

#endif
