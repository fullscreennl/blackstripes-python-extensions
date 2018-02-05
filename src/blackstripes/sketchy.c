#include "Python.h"
#include "../lib/sketchy/SketchyImage.h"
#include "../lib/sketchy/Point.h"
#include "blackstripes_signature.h"

static char module_docstring[] = "This module provides a sketchy png to svg conversion";
static char sketch_docstring[] = "Make a svg sketch of the png";

static char svg_formatstring[] = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\
<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\"> \
<svg version=\"1.1\"\
 id=\"Layer_1\"\
 xmlns=\"http://www.w3.org/2000/svg\"\
 xmlns:xlink=\"http://www.w3.org/1999/xlink\"\
 x=\"0px\"\
 y=\"0px\"\
 width=\"%s\"\
 height=\"%s\"\
 viewBox=\"0 0 %f %f\"\
 enable-background=\"new 0 0 %f %f\"\
 xml:space=\"preserve\">\
 <g transform=\"scale(%f)\">\
<polyline points=\"";

static PyObject *sketchy_sketch(PyObject *self, PyObject *args);

static PyMethodDef module_methods[] = {
            {"draw", sketchy_sketch, METH_VARARGS, sketch_docstring},
            {NULL, NULL, 0, NULL}
};

#if PY_MAJOR_VERSION >= 3
    static struct PyModuleDef moduledef = {
        PyModuleDef_HEAD_INIT,
        "sketchy",     /* m_name */
        module_docstring,  /* m_doc */
        -1,                  /* m_size */
        module_methods,    /* m_methods */
        NULL,                /* m_reload */
        NULL,                /* m_traverse */
        NULL,                /* m_clear */
        NULL,                /* m_free */
    };
#endif

PyMODINIT_FUNC
#if PY_MAJOR_VERSION >= 3
PyInit_sketchy(void)
#else
initsketchy(void)
#endif
{
    PyObject *m;
    #if PY_MAJOR_VERSION >= 3
        m = PyModule_Create(&moduledef);
    #else
        m = Py_InitModule3("sketchy", module_methods, module_docstring);
    #endif

    #if PY_MAJOR_VERSION >= 3
        return m;
    #endif
        
}

static void decodePNG(const char* filename,const char* output_filename,int maxLineLength,float nibsize, const char* color, float scale, int linesize, float sigTransX, float sigTransY, float sigScale){

    SketchyImage *im = SketchyImage_allocWithFileName(filename);
    SketchyImage_setNibSize(im, linesize);
    float avg = SketchyImage_getAvgBrightness(im);
    long int threshold = SketchyImage_getBrightness(im);
    if(avg < 128){
       threshold = threshold * (128.0/avg);
    } 
    long int outputBrightness = SketchyImage_getOutputBrightness(im);
    int width = SketchyImage_getCanvasWidth(im);
    int height = SketchyImage_getCanvasHeight(im);

    Point *darkestPixel = SketchyImage_getDarkPixel(im);

    int x = darkestPixel->x;
    int y = darkestPixel->y;

    Point_release(darkestPixel);

    FILE *svgFile = fopen(output_filename, "w");
    int extraHeight = 100;
    if(sigScale == 0.0){
        extraHeight = 0;
    }
    fprintf(svgFile,svg_formatstring,"100%","100%", width * scale, (height + extraHeight) * scale, width * scale, (height + extraHeight) * scale, scale);

    srand(time(NULL));

    while(outputBrightness > threshold){
        int r = 10 + rand()%maxLineLength;
        Point *p = SketchyImage_bestPointOfNDestinationsFromXY2(im,r,x,y);
        fprintf(svgFile,"%i,%i ", x, y);
        outputBrightness = SketchyImage_getOutputBrightness(im);
        x = p->x;
        y = p->y;
    }

    char sig[80000];
    int success = 1;
    if(sigScale != 0.0){
        success = sprintf(sig, signature, sigTransX, sigTransY, sigScale, nibsize, color);
    }else{
        strcpy(sig, "");
    }
    if (success){
        fprintf(svgFile,"\" style=\"fill:none;stroke:%s;stroke-width:%f;stroke-linecap:round;stroke-linejoin:round;\" />%s</g></svg>\n", color, nibsize, sig);
    }
    fclose(svgFile);

    //SketchyImage_saveAsPNG(im,output_filename);
    //SketchyImage_saveStateAsPNG(im,"state.png");

    SketchyImage_release(im);

    char rsvgCommand[200];
    int rsvg_command_status = sprintf(rsvgCommand, "rsvg-convert -a -w 1000 %s -o %s.png --background-color '#ffffff'", output_filename, output_filename);
    if(rsvg_command_status){
        system(rsvgCommand);
    }

}

static PyObject *sketchy_sketch(PyObject *self, PyObject *args){
    const char *inputpng = "";
    const char *filename = "";
    const char *color = "";
    float nibsize = 1;
    int linesize = 1;
    int maxLineLength = 50;
    float scale = 1.0;
    float sigTransX = 0.0;
    float sigTransY = 0.0;
    float sigScale = 1.0;
    if (!PyArg_ParseTuple(args, "ssfisfifff", &inputpng, &filename, &nibsize, &maxLineLength, &color, &scale, &linesize, &sigTransX, &sigTransY, &sigScale)){
        return NULL;
    }

    decodePNG(inputpng, filename, maxLineLength, nibsize, color, scale, linesize, sigTransX, sigTransY, sigScale);

    PyObject *ret = Py_BuildValue("i", 0);
    return ret;
}
