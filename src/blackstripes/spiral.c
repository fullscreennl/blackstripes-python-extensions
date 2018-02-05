#include <stdint.h>
#include <math.h>
#include "Python.h"
#include "../lib/sketchy/SketchyImage.h"
#include "../lib/sketchy/Point.h"
#include "blackstripes_signature.h"
//char *signature = "<!-- %d %s -->\n";

static char module_docstring[] = "This module provides a blackstripes spiral png to svg conversion";
static char draw_docstring[] = "Make a svg blackstripes spiral of the png";

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

static PyObject *spiral_draw(PyObject *self, PyObject *args);

static PyMethodDef module_methods[] = {
            {"draw", spiral_draw, METH_VARARGS, draw_docstring},
            {NULL, NULL, 0, NULL}
};

#if PY_MAJOR_VERSION >= 3
    static struct PyModuleDef moduledef = {
        PyModuleDef_HEAD_INIT,
        "spiral",     /* m_name */
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
PyInit_spiral(void)
#else
initspiral(void)
#endif
{
    PyObject *m;
    #if PY_MAJOR_VERSION >= 3
        m = PyModule_Create(&moduledef);
    #else
        m = Py_InitModule3("spiral", module_methods, module_docstring);
    #endif

    #if PY_MAJOR_VERSION >= 3
        return m;
    #endif

}

static void appendOpenSegment(FILE *svgFile, float fx, float fy){
    const char *svg_segment_format = "<path d=\"M%f,%f ";
    fprintf(svgFile, svg_segment_format, fx, fy);
}

static void appendCloseSegment(FILE *svgFile, float tx, float ty, float radius, const char *color, float nib_size_mm, int segment_iterations){
    const char *svg_segment_format = "A%f,%f 0 %i,%i %f,%f\" fill=\"none\" stroke=\"%s\" stroke-width=\"%f\" stroke-linecap=\"round\" />\n";
    int dir = 1;
    int long_way_home = 0;
    if (segment_iterations > 1800){
        long_way_home = 1;
    }
    fprintf(svgFile, svg_segment_format, radius, radius, long_way_home, dir, tx, ty, color, nib_size_mm);
}

static void decodePNG(const char* filename,const char* output_filename,float nibsize, const char* color, 
                      float scale, int level0, int level1, int level2, int level3, int linespacing, 
                      float sigTransX, float sigTransY, float sigScale, int roundShape){

    SketchyImage *im = SketchyImage_allocWithFileName(filename);
    SketchyImage_setNibSize(im, nibsize);
    int width = SketchyImage_getCanvasWidth(im);
    int height = SketchyImage_getCanvasHeight(im);

    int *levels = (int *)malloc(4*sizeof(int));
    levels[0] = level0;
    levels[1] = level1;
    levels[2] = level2;
    levels[3] = level3;

    int level_id = 0;

    float x = 0;
    float y = 0;
    float from_x = 0;
    float from_y = 0;
    float to_x = 0;
    float to_y = 0;

    FILE *svgFile = fopen(output_filename, "w");
    int extraHeight = 100;
    if(sigScale == 0.0){
        extraHeight = 0;
    }
    fprintf(svgFile,svg_formatstring,"100%","100%", width * scale, (height + extraHeight) * scale, width * scale, (height + extraHeight) * scale, scale);

    int i = 0;
    float radius = sqrt((width / 2.0) * (width / 2.0) + (height / 2.0) * (height / 2.0));
    if(roundShape){
        radius = width / 2.0;
    }

    int num_cycles = radius / linespacing;
    int numiterations = 3600 * num_cycles;
    float degree_to_radian_fact = 0.0174532925;

    float centerx = width / 2.0;
    float centery = height / 2.0;
    int pixelvalue = 0;

    int num_levels = 4;

    int penstate = 0;
    int newstate = 0;
    int segment_iterations = 0;

    while (numiterations > i){

        if (i % 3600 == 0){
            if(penstate == 1){
                to_x = x;
                to_y = y;
                penstate = 0;
                appendCloseSegment(svgFile, to_x, to_y, radius, color, nibsize, segment_iterations);
            }
            radius -= linespacing;
            level_id ++;
            if (level_id > num_levels - 1){
                level_id = 0;
            }
        }

        x = cos((i * degree_to_radian_fact) / 10.0) * radius + centerx;
        y = sin((i * degree_to_radian_fact) / 10.0) * radius + centery;

        if(x < 0 || x > width || y < 0 || y > height){
            pixelvalue = 999;
        }else{
            pixelvalue = SketchyImage_getPixel(im,(int)floor(x),(int)floor(y));
        }

        if (pixelvalue < levels[level_id]){
            newstate = 1;
            if (newstate != penstate){
                from_x = x;
                from_y = y;
                appendOpenSegment(svgFile , from_x, from_y);
                segment_iterations = 0;
            }
            penstate = newstate;
        }else{
            newstate = 0;
            if (newstate != penstate){
                to_x = x;
                to_y = y;
                appendCloseSegment(svgFile, to_x, to_y, radius, color, nibsize, segment_iterations);
            }
            penstate = newstate;
        }

        i ++;
        segment_iterations ++;
    }

    appendCloseSegment(svgFile, to_x, to_y, radius, color, nibsize, segment_iterations);

    char sig[80000];
    int success = 1;
    if(sigScale != 0.0){
        success = sprintf(sig, signature, sigTransX, sigTransY, sigScale, nibsize, color);
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

static PyObject *spiral_draw(PyObject *self, PyObject *args){
    const char *inputpng = "";
    const char *filename = "";
    const char *color = "";
    float nibsize = 1;
    float scale = 1.0;
    int level0 = 1;
    int level1 = 1;
    int level2 = 1;
    int level3 = 1;
    int linespacing = 1;
    float sigTransX = 0.0;
    float sigTransY = 0.0;
    float sigScale = 1.0;
    int roundShape = 0;
    if (!PyArg_ParseTuple(args, "ssfsfiiiiifff|i", &inputpng, &filename, &nibsize, &color,
                          &scale, &level0, &level1, &level2, &level3, &linespacing,
                          &sigTransX, &sigTransY, &sigScale, &roundShape)){
        return NULL;
    }

    decodePNG(inputpng, filename, nibsize, color, scale, level0, level1, level2, level3, 
              linespacing, sigTransX, sigTransY, sigScale, roundShape);

    PyObject *ret = Py_BuildValue("i", 0);
    return ret;
}
