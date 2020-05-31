//
// Created by ssiie on 31.05.2020.
//

#ifndef COLORSPACECONVERSION_CIMAGE_H
#define COLORSPACECONVERSION_CIMAGE_H


#include <iostream>
#include "CException.h"
#include <cmath>

using namespace std;

struct SInput {
    int from;
    int to;
    int countInput;
    int countOutput;
    char *inputFile;
    char *outputFile;
};

struct RGB {
    double red;
    double green;
    double blue;
};

struct SMetaData {
    FILE *file;
    int version;
    int width;
    int height;
    int max_val;
    int size;
};

class CImage {
public:

    void read1file(SInput console);

    void read3files(SInput console);

    void write1file(SInput console);

    void write3files(SInput console);

    void convert(SInput config);

    ~CImage();

private:
    SMetaData *data;
    RGB *pixRGB;


    void RGBtoHSL();

    void RGBtoHSV();

    void RGBtoYCbCr601();

    void RGBtoYCbCr709();

    void RGBtoYCoCg();

    void RGBtoCMY();

    void HSLtoRGB();

    void HSVtoRGB();

    void YCbCr601toRGB();

    void YCbCr709toRGB();

    void YCoCgtoRGB();

    void CMYtoRGB();

};


#endif //COLORSPACECONVERSION_CIMAGE_H
