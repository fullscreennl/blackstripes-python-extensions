#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "FSObject.h"
#include "Point.h"
#include "bool.h"
#include "SketchyImage.h"

#include "../lodepng/lodepng.h"

typedef enum{
    lineDarknessModeAvg = 1,
    lineDarknessModeClear = 2
}LineDarknessMode;

int gCounter = 0;

int SketchyImage_kernelValueByXY(SketchyImage *obj,int x, int y, bool clear,int kernelSize);

SketchyImage *SketchyImage_allocWithFileName(const char *filename){

    SketchyImage *obj = (SketchyImage *) malloc(sizeof(SketchyImage));
    obj->retainCount = 1;
    obj->type = "SketchyImage";
    obj->scaleFactor = 1.0;
    obj->xCorrection = 0.0;
    obj->yCorrection = 0.0;
    obj->xOffset = 0.0;
    obj->yOffset = 0.0;
    obj->brightness = 0;
    obj->outputBrightness = 0;
    obj->nibsize = 1;
    obj->avgBrightness = 255.0;

    unsigned error;
    unsigned char* image;
    unsigned width, height;

    error = lodepng_decode32_file(&image, &width, &height, filename);
    if(error){
        printf("error %u: %s\n", error, lodepng_error_text(error));
    }

    obj->width = width;
    obj->height = height;

    int numPixels = obj->width * obj->height;

    obj->imageData = (unsigned char*)calloc(numPixels , 1);
    obj->outputImageData = (unsigned char*)calloc(numPixels , 1);

    if(obj->imageData == NULL || obj->outputImageData == NULL){
        printf("mem error\n");
    }

    int i;
    int j;
    int numpix = width*height*4;
    for(i=0,j=0; i<numpix; i+=4,j++){
        int pixelValue = image[i];
        obj->brightness += pixelValue;
        obj->outputImageData[j] = 255;
        obj->imageData[j] = pixelValue;
    }
    obj->outputBrightness = width*height * 255;
    obj->avgBrightness = obj->brightness / (width*height);

    free(image);

    return obj;

}

int SketchyImage_getCanvasWidth(SketchyImage *obj){
    return obj->width;
}

int SketchyImage_getCanvasHeight(SketchyImage *obj){
    return obj->height;
}

void SketchyImage_setNibSize(SketchyImage *obj,int nibsize){
    obj->nibsize = nibsize;
}

long int SketchyImage_getOutputBrightness(SketchyImage *obj){
    return obj->outputBrightness;
}

long int SketchyImage_getBrightness(SketchyImage *obj){
    return obj->brightness;
}

float SketchyImage_getAvgBrightness(SketchyImage *obj){
    return obj->avgBrightness;
}

Point *SketchyImage_getDarkPixel(SketchyImage *obj){
    int darkest = 255;
    int i = 0;
    int x = 0;
    int y = 0;
    int numPixels = obj->width * obj->height;
    for (i=0; i<numPixels; i++){
        int pixelValue = obj->imageData[i];
        if(pixelValue < darkest){
            darkest = pixelValue;
            x = i%obj->width;
            y = floor(i/obj->width);
        }
    }
    return Point_alloc((float)x,(float)y);
}

void SketchyImage_release(SketchyImage *obj){
    obj->retainCount --;
    if(obj->retainCount == 0){
        free(obj->imageData);
        free(obj);
    }
}

void SketchyImage_retain(SketchyImage *obj){
    FSObject_retain(obj);
}

//
// helper method called by :
// 1) SketchyImage_avgDarknessForLine returns the average darkness for pixels under a line
// 2) SketchyImage_clearDarknessForLine clears the darkness for pixels under the line, returns 0
//
float SketchyImage_darknessHelperForLine(SketchyImage *obj, float x1, float y1, float x2, float y2,LineDarknessMode mode){

    int kernelSize = obj->nibsize;
    if(kernelSize%2 == 0){
        kernelSize = kernelSize - 1;
    }

    int xpol = (x1-x2) < 0;
    int ypol = (y1-y2) < 0;
    if(xpol == 0){
        xpol = -1;
    }
    if(ypol == 0){
        ypol = -1;
    }

    float xd = fabs(x1-x2);
    float yd = fabs(y1-y2);
    float slope = (x1-x2)/(y1-y2);
    float totaldarkness = 0;
    float avg;
    if(xd > yd){
        int i;
        int cx = (int)x1;
        int cy = (int)y1;
        for(i=0;i<xd;i++){
            if(mode == lineDarknessModeAvg){
                int darkness = SketchyImage_kernelValueByXY(obj,(int)floor(cx),(int)floor(cy),false,kernelSize);
                totaldarkness = totaldarkness + darkness;
            }else{
                SketchyImage_kernelValueByXY(obj,cx,cy,true,kernelSize);
            }
            cx += xpol;
            cy = y1 + i/slope * xpol;
        }
        avg = totaldarkness/xd; 
    }else{
        int i;
        int cx = (int)x1;
        int cy = (int)y1;
        for(i=0;i<yd;i++){
            if(mode == lineDarknessModeAvg){
                int darkness = SketchyImage_kernelValueByXY(obj,cx,cy,false,kernelSize);
                totaldarkness = totaldarkness + darkness;
            }else{
                SketchyImage_kernelValueByXY(obj,cx,cy,true,kernelSize);
            }
            cy += ypol;
            cx = x1 + i*slope * ypol;
        }
        avg = totaldarkness/yd; 
    }
    return avg;
}

float SketchyImage_avgDarknessForLine(SketchyImage *obj, float x1, float y1, float x2, float y2){
    float avg = SketchyImage_darknessHelperForLine(obj,x1,y1,x2,y2,lineDarknessModeAvg);
    return avg;
}

void SketchyImage_clearDarknessForLine(SketchyImage *obj, float x1, float y1, float x2, float y2){
    SketchyImage_darknessHelperForLine(obj,x1,y1,x2,y2,lineDarknessModeClear);
}

Point *SketchyImage_bestPointOfNDestinationsFromXY2(SketchyImage *obj, int radius, int x, int y){

    x = x * obj->scaleFactor;
    y = y * obj->scaleFactor;

    float degree_to_radian_fact = 0.0174532925;
    int n = 360; //full circle scan
    int i;
    float best = 9999.0;
    int bestX = 0;
    int bestY = 0;
    gCounter ++;
    for(i=gCounter; i<n+gCounter; i+=3){

        int rx = cos(i*degree_to_radian_fact) * radius + x;
        int ry = sin(i*degree_to_radian_fact) * radius + y;

        if(rx < obj->width-1 && ry < obj->height-1 && rx > 0 && ry > 0){
            float avg = SketchyImage_avgDarknessForLine(obj,x,y,rx,ry);
            if(avg < best){
                best = avg;
                bestX = rx;
                bestY = ry;
                // if(avg < 100){
                //     break;
                // }
            }
        }

    }
    SketchyImage_clearDarknessForLine(obj,x,y,bestX,bestY);
    bestX = bestX/obj->scaleFactor;
    bestY = bestY/obj->scaleFactor;
    return Point_alloc((float)bestX,(float)bestY);

}

int pix(SketchyImage *obj,int pixelindex){
    if(pixelindex < (obj->width * obj->height)){
        return pixelindex;
    }
    return 0;
}

int SketchyImage_getPixel(SketchyImage *obj,int x, int y){
    if (y < 0 || y > obj->height-1 || x < 0 || x > obj->width-1){
        return 255;
    }
    int index = y * obj->width + x;
    return obj->imageData[pix(obj,index)];
}

int SketchyImage_kernelValueByXY(SketchyImage *obj,int x, int y, bool clear,int kernelSize){
    
    //the image data contains a 6 byte header
    //these are the threshold levels
    //this used to be fixed (36 spacing)
    //int levels[6] = {217, 180, 144, 108, 72, 36};
    if (x > obj->width-1 || y > obj->height-1 || x < 0 || y < 0){
        //out of bounds
        return -1;
    }
    int w = obj->width;
    int pixelindex = y*w + x;

    int pixelValue = obj->imageData[pixelindex];
    if(kernelSize > 1){
        int i;
        int limit = (kernelSize - 1) / 2.0;
        for(i=0;i<limit;i++){
            pixelValue += obj->imageData[pix(obj,pixelindex+1+i)];
            pixelValue += obj->imageData[pix(obj,pixelindex+obj->width+(i*obj->width))];
            pixelValue += obj->imageData[pix(obj,pixelindex-obj->width-(i*obj->width))];
            pixelValue += obj->imageData[pix(obj,pixelindex-1-i)];
        }
    }

    if(clear){
        int index;
        if(kernelSize > 1){
            int i;
            int limit = (kernelSize - 1) / 2.0;
            for(i=0;i<limit;i++){

                index = pix(obj,pixelindex+1+i);
                obj->brightness += (255-obj->imageData[index]);
                obj->imageData[index] = 255;
                obj->outputBrightness -= obj->outputImageData[index];
                obj->outputImageData[index] = 0;

                index = pix(obj,pixelindex+obj->width+(i*obj->width));
                obj->brightness += (255-obj->imageData[index]);
                obj->imageData[index] = 255;
                obj->outputBrightness -= obj->outputImageData[index];
                obj->outputImageData[index] = 0;

                index = pix(obj,pixelindex-obj->width-(i*obj->width));
                obj->brightness += (255-obj->imageData[index]);
                obj->imageData[index] = 255;
                obj->outputBrightness -= obj->outputImageData[index];
                obj->outputImageData[index] = 0;

                index = pix(obj,pixelindex-1-i);
                obj->brightness += (255-obj->imageData[index]);
                obj->imageData[index] = 255;
                obj->outputBrightness -= obj->outputImageData[index];
                obj->outputImageData[index] = 0;

            }
        }
        index = pixelindex;
        obj->brightness += (255-obj->imageData[index]);
        obj->imageData[index] = 255;
        obj->outputBrightness -= obj->outputImageData[index];
        obj->outputImageData[index] = 0;
    }

    return pixelValue;
}

void SketchyImage_saveStateAsPNG(SketchyImage *obj,const char *name){
    unsigned char *imd = obj->imageData;
    unsigned error = lodepng_encode_file(name, imd,obj->width, obj->height,LCT_GREY,8);
    if(error) printf("error %u: %s\n", error, lodepng_error_text(error));
}

void SketchyImage_saveAsPNG(SketchyImage *obj,const char *name){
    unsigned char *imdo = obj->outputImageData;
    unsigned erroro = lodepng_encode_file(name, imdo,obj->width, obj->height,LCT_GREY,8);
    if(erroro) printf("error %u: %s\n", erroro, lodepng_error_text(erroro)); 
}




