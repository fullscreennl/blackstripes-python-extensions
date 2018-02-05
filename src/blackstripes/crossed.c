#include <stdint.h>
#include "Python.h"
#include "../lib/sketchy/SketchyImage.h"
#include "../lib/sketchy/Point.h"
#include "crossed-large.h"
#include "crossed-xlarge.h"

#include "blackstripes_signature.h"
//char *signature = "<!-- %d %s -->\n";

static char module_docstring[] = "This module provides a blackstripes crossed png to svg conversion";
static char draw_docstring[] = "Make a svg blackstripes classic of the png";

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
 <g transform=\"scale(%f)\">";

static PyObject *crossed_draw(PyObject *self, PyObject *args);

static PyMethodDef module_methods[] = {
            {"draw", crossed_draw, METH_VARARGS, draw_docstring},
            {NULL, NULL, 0, NULL}
};

#if PY_MAJOR_VERSION >= 3
    static struct PyModuleDef moduledef = {
        PyModuleDef_HEAD_INIT,
        "crossed",     /* m_name */
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
PyInit_crossed(void)
#else
initcrossed(void)
#endif
{
    PyObject *m;
    #if PY_MAJOR_VERSION >= 3
        m = PyModule_Create(&moduledef);
    #else
        m = Py_InitModule3("crossed", module_methods, module_docstring);
    #endif

    #if PY_MAJOR_VERSION >= 3
        return m;
    #endif

}

static void appendSegment(FILE *svgFile, float fx, float fy,  float tx, float ty, float radius, const char *color, float nib_size_mm, int dir){
    const char *svg_segment_format = "<path d=\"M%f,%f  A%f,%f 0 0,%d %f,%f\" fill=\"none\" stroke=\"%s\" stroke-width=\"%f\" stroke-linecap=\"round\" />\n";
    fprintf(svgFile, svg_segment_format,
            fx, fy,
            radius, radius,
            dir,
            tx, ty,
            color,
            nib_size_mm
            );
}

static void decodePNG(const char* filename,const char* output_filename,float nibsize, const char* color, float scale, int level0, int level1, int level2, int level3, int type, float sigTransX, float sigTransY, float sigScale){

    SketchyImage *im = SketchyImage_allocWithFileName(filename);
    SketchyImage_setNibSize(im, nibsize);
    int width = SketchyImage_getCanvasWidth(im);
    int height = SketchyImage_getCanvasHeight(im);

    float x = 0;
    float y = 0;
    float from_x = 0;
    float from_y = 0;
    float to_x = 0;
    float to_y = 0;

    int threshold = level0;
    int newstate = 1;
    int penstate = -1;
    int pixelvalue = 0;
    int layerIndex = 0;

    int *levels = (int *)malloc(4*sizeof(int));
    levels[0] = level0;
    levels[1] = level1;
    levels[2] = level2;
    levels[3] = level3;

    FILE *svgFile = fopen(output_filename, "w");
    int extraHeight = 100;
    if(sigScale == 0.0){
        extraHeight = 0;
    }
    fprintf(svgFile,svg_formatstring,"100%","100%", width * scale, (height + extraHeight) * scale, width * scale, (height + extraHeight) * scale, scale);

    int i = 0;
    float radius = 0;

    int16_t *coords;
    int size;
    if(type == 1){
        coords = coords_l;
        size = sizeof(coords_l);
    }else{
        coords = coords_xl;
        size = sizeof(coords_xl);
    }

    for(i = 0; i < size / sizeof(int16_t); i += 2)
    {
        if(coords[i] == -20){
            layerIndex ++;
            threshold = levels[layerIndex];
        }else if(coords[i] == -10){
            radius = coords[i+1] / 10.0;
            newstate = 0;
            if (newstate != penstate){
                to_x = x;
                to_y = y;
                appendSegment(svgFile, from_x, from_y, to_x, to_y, radius, color, nibsize, 1);
            }
            penstate = newstate;
        }else{
            x = coords[i] / 10.0;
            y = coords[i+1] / 10.0;
            pixelvalue = SketchyImage_getPixel(im,(int)round(x),(int)round(y));
            if (pixelvalue < threshold){
                newstate = 1;
                if (newstate != penstate){
                    from_x = x;
                    from_y = y;
                }
                penstate = newstate;
            }else{
                newstate = 0;
                if (newstate != penstate){
                    to_x = x;
                    to_y = y;
                    appendSegment(svgFile, from_x, from_y, to_x, to_y, radius, color, nibsize, 1);
                }
                penstate = newstate;
            }
        }
    }
    char sig[80000];
    int success = 1;
    if(sigScale != 0.0){
         success = sprintf(sig, signature, sigTransX, sigTransY, sigScale, nibsize / sigScale, color);
    }else{
        strcpy(sig, "");
    }

    if (success){
        fprintf(svgFile,"%s</g></svg>\n", sig);
    }
    fclose(svgFile);
    SketchyImage_release(im);

    char rsvgCommand[200];
    int rsvg_command_status = sprintf(rsvgCommand, "rsvg-convert -a -w 1000 %s -o %s.png --background-color '#ffffff'", output_filename, output_filename);
    if(rsvg_command_status){
        system(rsvgCommand);
    }

}

static PyObject *crossed_draw(PyObject *self, PyObject *args){
    const char *inputpng = "";
    const char *filename = "";
    const char *color = "";
    float nibsize = 1;
    float scale = 1.0;
    int level0 = 1;
    int level1 = 1;
    int level2 = 1;
    int level3 = 1;
    int type = 1;
    float sigTransX = 0.0;
    float sigTransY = 0.0;
    float sigScale = 1.0;
    if (!PyArg_ParseTuple(args, "ssfsfiiiiifff", &inputpng, &filename, &nibsize , &color, &scale, &level0, &level1, &level2, &level3, &type, &sigTransX, &sigTransY, &sigScale)){
        return NULL;
    }

    decodePNG(inputpng, filename, nibsize, color, scale, level0, level1, level2, level3, type, sigTransX, sigTransY, sigScale);

    PyObject *ret = Py_BuildValue("i", 0);
    return ret;
}
