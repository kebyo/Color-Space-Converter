#include "CImage.h"
#include <string.h>
#include <algorithm>

void CImage::read1file(SInput console) {
    FILE *f = fopen(console.inputFile, "rb");
    if (!f) {
        throw CException("Input file didn't open\n");
    }
    data = new SMetaData[1];
    data[0].file = f;
    if (fscanf(f, "P%d%d%d%d\n", &this->data[0].version, &this->data[0].width, &this->data[0].height,
               &data[0].max_val) != 4) {
        throw CException("Wrong amount data in file", f);
    }
    if (data[0].version != 6) {
        throw CException("Expected version 6");
    }
    data[0].size = data[0].width * data[0].height;
    unsigned char *buffer;
    buffer = new unsigned char[data[0].size * 3];
    pixRGB = new RGB[data[0].size];
    fread(buffer, sizeof(unsigned char), data[0].size * 3, f);
    for (int i = 0, j = 0; i < data[0].size * 3; i += 3, j++) {
        pixRGB[j] = {(double) buffer[i], (double) buffer[i + 1], (double) buffer[i + 2]};
    }
    delete[] buffer;
    fclose(f);
}

void CImage::read3files(SInput console) {
    FILE *f1, *f2, *f3;
    string filename1 = console.inputFile;
    string filename2 = filename1;
    string filename3 = filename1;
    filename1.insert(filename1.size() - 4, "_1");
    filename2.insert(filename2.size() - 4, "_2");
    filename3.insert(filename3.size() - 4, "_3");
    int n = filename1.size();
    char *name1 = new char[filename1.size() + 1];
    char *name2 = new char[filename2.size() + 1];
    char *name3 = new char[filename3.size() + 1];
    strcpy(name1, filename1.c_str());
    strcpy(name2, filename2.c_str());
    strcpy(name3, filename3.c_str());
    f1 = fopen(name1, "rb");
    if (!f1) {
        throw CException("File _1 didn't open");
    }
    f2 = fopen(name2, "rb");
    if (!f2) {
        throw CException("File _2 didn't open");
    }
    f3 = fopen(name3, "rb");
    if (!f3) {
        throw CException("File _3 didn't open");
    }
    data = new SMetaData[3];
    if (fscanf(f1, "P%d%d%d%d\n", &this->data[0].version, &this->data[0].width, &this->data[0].height,
               &data[0].max_val) != 4) {
        throw CException("Wrong amount of meta data in _1");
    }
    if (fscanf(f2, "P%d%d%d%d\n", &this->data[1].version, &this->data[1].width, &this->data[1].height,
               &data[1].max_val) != 4) {
        throw CException("Wrong amount of meta data in _2");
    }
    if (fscanf(f3, "P%d%d%d%d\n", &this->data[2].version, &this->data[2].width, &this->data[2].height,
               &data[2].max_val) != 4) {
        throw CException("Wrong amount of meta data in _3");
    }
    if (data[0].version != 5) {
        throw CException("Expected version 5 in _1", f1, f2, f3);
    }
    if (data[1].version != 5) {
        throw CException("Expected version 5 in _2", f1, f2, f3);
    }
    if (data[2].version != 5) {
        throw CException("Expected version 5 in _3", f1, f2, f3);
    }
    int width = data[0].width;
    if (data[1].width != width || data[2].width != width) {
        throw CException("Different widths", f1, f2, f3);
    }
    int height = data[0].height;
    if (data[1].height != height || data[2].height != height) {
        throw CException("Different heights", f1, f2, f3);
    }
    data[0].size = width * height;
    data[1].size = width * height;
    data[2].size = width * height;
    int mv = data[0].max_val;
    if (data[1].max_val != mv || data[2].max_val != mv) {
        throw CException("Different max values", f1, f2, f3);
    }
    int len = data[0].size;
    unsigned char *buffer1 = new unsigned char[len];
    fread(buffer1, sizeof(unsigned char), len, f1);
    unsigned char *buffer2 = new unsigned char[len];
    fread(buffer2, sizeof(unsigned char), len, f2);
    unsigned char *buffer3 = new unsigned char[len];
    fread(buffer3, sizeof(unsigned char), len, f3);
    pixRGB = new RGB[len];
    for (int i = 0; i < len; i++) {
        pixRGB[i] = {(double) buffer1[i], (double) buffer2[i], (double) buffer3[i]};
    }
    delete[] buffer1;
    delete[] buffer2;
    delete[] buffer3;
    fclose(f1);
    fclose(f2);
    fclose(f3);
}

void CImage::write1file(SInput console) {
    FILE *new_f = fopen(console.outputFile, "wb");
    if (!new_f) {
        throw CException("Output file didn't open", new_f);
    }
    fprintf(new_f, "P%d\n%d %d\n%d\n", 6, data[0].width, data[0].height, data[0].max_val);
    int len = data[0].size * 3;
    unsigned char *buffer = new unsigned char[len];
    for (int i = 0, j = 0; i < len; i += 3, j++) {
        buffer[i] = (unsigned char) pixRGB[j].red;
        buffer[i + 1] = (unsigned char) pixRGB[j].green;
        buffer[i + 2] = (unsigned char) pixRGB[j].blue;
    }
    fwrite(buffer, sizeof(unsigned char), len, new_f);
    delete[] buffer;
    fclose(new_f);
}

void CImage::write3files(SInput console) {
    FILE *f1, *f2, *f3;
    string filename1 = console.outputFile;
    string filename2 = filename1;
    string filename3 = filename1;
    filename1.insert(filename1.size() - 4, "_1");
    filename2.insert(filename2.size() - 4, "_2");
    filename3.insert(filename3.size() - 4, "_3");
    int n = filename1.size();
    char *name1 = new char[filename1.size() + 1];
    char *name2 = new char[filename2.size() + 1];
    char *name3 = new char[filename3.size() + 1];
    strcpy(name1, filename1.c_str());
    strcpy(name2, filename2.c_str());
    strcpy(name3, filename3.c_str());
    f1 = fopen(name1, "wb");
    if (!f1) {
        throw CException("Output file _1 didn't open");
    }
    f2 = fopen(name2, "wb");
    if (!f2) {
        throw CException("Output file _2 didn't open");
    }
    f3 = fopen(name3, "wb");
    if (!f3) {
        throw CException("Output file _3 didn't open");
    }
    fprintf(f1, "P%i\n%d %d\n%d\n", 5, data[0].width, data[0].height, data[0].max_val);
    fprintf(f2, "P%i\n%d %d\n%d\n", 5, data[0].width, data[0].height, data[0].max_val);
    fprintf(f3, "P%i\n%d %d\n%d\n", 5, data[0].width, data[0].height, data[0].max_val);
    unsigned char *buffer1 = new unsigned char[data[0].size];
    unsigned char *buffer2 = new unsigned char[data[0].size];
    unsigned char *buffer3 = new unsigned char[data[0].size];
    for (int i = 0; i < data[0].size; i++) {
        buffer1[i] = (unsigned char) pixRGB[i].red;
        buffer2[i] = (unsigned char) pixRGB[i].green;
        buffer3[i] = (unsigned char) pixRGB[i].blue;
    }
    fwrite(buffer1, sizeof(unsigned char), data[0].size, f1);
    fwrite(buffer2, sizeof(unsigned char), data[0].size, f2);
    fwrite(buffer3, sizeof(unsigned char), data[0].size, f3);
    delete[] buffer1;
    delete[] buffer2;
    delete[] buffer3;
    fclose(f1);
    fclose(f2);
    fclose(f3);
}

CImage::~CImage() {
    delete[] pixRGB;
}

void CImage::convert(SInput config) {
    switch (config.from) {
        case 0:
            break;
        case 1:
            HSLtoRGB();
            break;
        case 2:
            HSVtoRGB();
            break;
        case 3:
            YCbCr601toRGB();
            break;
        case 4:
            YCbCr709toRGB();
            break;
        case 5:
            YCoCgtoRGB();
            break;
        case 6:
            CMYtoRGB();
            break;
    }
    switch (config.to) {
        case 0:
            break;
        case 1:
            RGBtoHSL();
            break;
        case 2:
            RGBtoHSV();
            break;
        case 3:
            RGBtoYCbCr601();
            break;
        case 4:
            RGBtoYCbCr709();
            break;
        case 5:
            RGBtoYCoCg();
            break;
        case 6:
            RGBtoCMY();
            break;
    }
}

void CImage::RGBtoCMY() {
    for (int i = 0; i < data->size; i++) {
        pixRGB[i].red = (1 - pixRGB[i].red / 255.0) * 255.0;
        pixRGB[i].green = (1 - pixRGB[i].green / 255.0) * 255.0;
        pixRGB[i].blue = (1 - pixRGB[i].blue / 255.0) * 255.0;
    }
}

void CImage::CMYtoRGB() {
    for (int i = 0; i < data->size; i++) {
        pixRGB[i].red = (1 - pixRGB[i].red / 255.0) * 255.0;
        pixRGB[i].green = (1 - pixRGB[i].green / 255.0) * 255.0;
        pixRGB[i].blue = (1 - pixRGB[i].blue / 255.0) * 255.0;
    }
}

void CImage::RGBtoHSL() {
    for (int i = 0; i < data->size; i++) {
        double r = pixRGB[i].red / 255.0;
        double g = pixRGB[i].green / 255.0;
        double b = pixRGB[i].blue / 255.0;
        double Cmax = max(r, max(g, b));
        double Cmin = min(r, min(g, b));
        double l = (Cmax + Cmin) / 2.0;
        double h = 0, s = 0;
        if (Cmax == Cmin) {
            h = 0;
        } else {
            if (Cmax == r && g >= b) {
                h = 60.0 * (g - b) / (Cmax - Cmin);
            } else if (Cmax == r && g < b) {
                h = 60.0 * (g - b) / (Cmax - Cmin) + 360.0;
            } else if (Cmax == g) {
                h = 60.0 * (b - r) / (Cmax - Cmin) + 120.0;
            } else if (Cmax == b) {
                h = 60.0 * (r - g) / (Cmax - Cmin) + 240.0;
            }
        }
        if (l == 0 || Cmax == Cmin) {
            s = 0;
        } else if (l <= 0.5) {
            s = (Cmax - Cmin) / (2.0 * l);
        } else {
            s = (Cmax - Cmin) / (2.0 - 2.0 * l);
        }
        pixRGB[i].red = h * 255.0 / 360.0;
        pixRGB[i].green = s * 255.0;
        pixRGB[i].blue = l * 255.0;
    }
}

void CImage::HSLtoRGB() {
    for (int i = 0; i < data->size; i++) {
        double h = pixRGB[i].red / 255.0 * 360.0;
        double s = pixRGB[i].green / 250.0;
        double l = pixRGB[i].blue / 255.0;
        double r, g, b;
        double q = l < 0.5 ? l * (1.0 + s) : l + s - l * s;
        double p = 2 * l - q;
        double hk = h / 360.0;
        double Tr = hk + 1.0 / 3.0;
        double Tg = hk;
        double Tb = hk - 1.0 / 3.0;
        Tr = Tr < 0 ? Tr + 1.0 : Tr;
        Tg = Tg < 0 ? Tg + 1.0 : Tg;
        Tb = Tb < 0 ? Tb + 1.0 : Tb;
        Tr = Tr > 1.0 ? Tr - 1.0 : Tr;
        Tg = Tg > 1.0 ? Tg - 1.0 : Tg;
        Tb = Tb > 1.0 ? Tb - 1.0 : Tb;
        if (Tr < 1.0 / 6.0) {
            r = p + (q - p) * 6.0 * Tr;
        } else if (Tr < 1.0 / 2.0) {
            r = q;
        } else if (Tr < 2.0 / 3.0) {
            r = p + (q - p) * (2.0 / 3.0 - Tr) * 6.0;
        } else {
            r = p;
        }
        if (Tg < 1.0 / 6.0) {
            g = p + (q - p) * 6.0 * Tg;
        } else if (Tg < 1.0 / 2.0) {
            g = q;
        } else if (Tg < 2.0 / 3.0) {
            g = p + (q - p) * (2.0 / 3.0 - Tg) * 6.0;
        } else {
            g = p;
        }
        if (Tb < 1.0 / 6.0) {
            b = p + (q - p) * 6.0 * Tb;
        } else if (Tb < 1.0 / 2.0) {
            b = q;
        } else if (Tb < 2.0 / 3.0) {
            b = p + (q - p) * (2.0 / 3.0 - Tb) * 6.0;
        } else {
            b = p;
        }
        if (s == 0) {
            r = l;
            g = l;
            b = l;
        }
        pixRGB[i].red = r * 255.0;
        pixRGB[i].green = g * 255.0;
        pixRGB[i].blue = b * 255.0;
    }
}

void CImage::RGBtoHSV() {
    for (int i = 0; i < data->size; i++) {
        double r = pixRGB[i].red / 255.0;
        double g = pixRGB[i].green / 255.0;
        double b = pixRGB[i].blue / 255.0;
        double MAX = max(r, max(g, b));
        double MIN = min(r, min(g, b));
        double h, s, v;
        if (MAX == MIN) {
            h = 0;
        } else if (MAX == r && g >= b) {
            h = 60.0 * (g - b) / (MAX - MIN);
        } else if (MAX == r && g < b) {
            h = 60.0 * (g - b) / (MAX - MIN) + 360.0;
        } else if (MAX == g) {
            h = 60.0 * (b - r) / (MAX - MIN) + 120.0;
        } else if (MAX == b) {
            h = 60.0 * (r - g) / (MAX - MIN) + 240.0;
        }
        if (MAX == 0) {
            s = 0;
        } else {
            s = 1 - MIN / MAX;
        }
        v = MAX;
        pixRGB[i].red = h / 360.0 * 255.0;
        pixRGB[i].green = s * 255.0;
        pixRGB[i].blue = v * 255.0;
    }
}

void CImage::HSVtoRGB() {
    for (int i = 0; i < data->size; i++) {
        double h = pixRGB[i].red / 255.0 * 360.0;
        double s = pixRGB[i].green / 255.0;
        double V = pixRGB[i].blue / 255.0;
        double r, g, b;
        double Vmin = (1 - s) * V;
        double Hi = abs(fmod(h / 60.0, 6));
        pixRGB[i].red = r * 255.0;
        pixRGB[i].green = g * 255.0;
        pixRGB[i].blue = b * 255.0;
        double a = (V - Vmin) * (fmod(h, 60)) / 60.0;
        double Vinc = Vmin + a;
        double Vdec = V - a;
        switch ((int) Hi) {
            case 0:
                r = V;
                g = Vinc;
                b = Vmin;
                break;
            case 1:
                r = Vdec;
                g = V;
                b = Vmin;
                break;
            case 2:
                r = Vmin;
                g = V;
                b = Vinc;
                break;
            case 3:
                r = Vmin;
                g = Vdec;
                b = V;
                break;
            case 4:
                r = Vinc;
                g = Vmin;
                b = V;
                break;
            case 5:
                r = V;
                g = Vmin;
                b = Vdec;
                break;
        }
        pixRGB[i].red = r * 255.0;
        pixRGB[i].green = g * 255.0;
        pixRGB[i].blue = b * 255.0;
    }
}

void CImage::RGBtoYCbCr601() {
    double kr = 0.299, kb = 0.114;
    for (int i = 0; i < data->size; i++) {
        double r = pixRGB[i].red / 255.0;
        double g = pixRGB[i].green / 255.0;
        double b = pixRGB[i].blue / 255.0;
        double y = kr * r + (1 - kr - kb) * g + kb * b;
        double pb = 0.5 * (b - y) / (1.0 - kb);
        double pr = 0.5 * (r - y) / (1.0 - kr);
        pixRGB[i].red = y * 255.0;
        pixRGB[i].green = (pb + 0.5) * 255.0;
        pixRGB[i].blue = (pr + 0.5) * 255.0;
    }
}

void CImage::RGBtoYCbCr709() {
    double kr = 0.0722, kb = 0.7152;
    for (int i = 0; i < data->size; i++) {
        double r = pixRGB[i].red / 255.0;
        double g = pixRGB[i].green / 255.0;
        double b = pixRGB[i].blue / 255.0;
        double y = kr * r + (1 - kr - kb) * g + kb * b;
        double pb = 0.5 * (b - y) / (1.0 - kb);
        double pr = 0.5 * (r - y) / (1.0 - kr);
        pixRGB[i].red = y * 255.0;
        pixRGB[i].green = (pb + 0.5) * 255.0;
        pixRGB[i].blue = (pr + 0.5) * 255.0;
    }
}

void CImage::YCbCr601toRGB() {
    double kr = 0.299, kb = 0.114;
    for (int i = 0; i < data->size; i++) {
        double y = pixRGB[i].red / 255.0;
        double pb = pixRGB[i].green / 255.0 - 0.5;
        double pr = pixRGB[i].blue / 255.0 - 0.5;
        double r, g, b;
        b = 2 * pb * (1 - kb) + y;
        r = 2 * pr * (1 - kr) + y;
        g = (y - kr * r - kb * b) / (1 - kr - kb);
        r = r < 0 ? 0 : r;
        r = r > 1 ? 1 : r;
        b = b < 0 ? 0 : b;
        b = b > 1 ? 1 : b;
        g = g < 0 ? 0 : g;
        g = g > 1 ? 1 : g;
        pixRGB[i].red = r * 255.0;
        pixRGB[i].green = g * 255.0;
        pixRGB[i].blue = b * 255.0;
    }
}

void CImage::YCbCr709toRGB() {
    double kr = 0.0722, kg = 0.2126, kb = 0.7152;
    for (int i = 0; i < data->size; i++) {
        double y = pixRGB[i].red / 255.0;
        double pb = pixRGB[i].green / 255.0 - 0.5;
        double pr = pixRGB[i].blue / 255.0 - 0.5;
        double r, g, b;
        b = 2 * pb * (1 - kb) + y;
        r = 2 * pr * (1 - kr) + y;
        g = (y - kr * r - kb * b) / (1 - kr - kb);
        r = r < 0 ? 0 : r;
        r = r > 1 ? 1 : r;
        b = b < 0 ? 0 : b;
        b = b > 1 ? 1 : b;
        g = g < 0 ? 0 : g;
        g = g > 1 ? 1 : g;
        pixRGB[i].red = r * 255.0;
        pixRGB[i].green = g * 255.0;
        pixRGB[i].blue = b * 255.0;
    }
}

void CImage::RGBtoYCoCg() {
    for (int i = 0; i < data->size; i++) {
        double r = pixRGB[i].red / 255.0;
        double g = pixRGB[i].green / 255.0;
        double b = pixRGB[i].blue / 255.0;
        double y = r / 4.0 + g / 2.0 + b / 4.0;
        double cg = -r / 4.0 + g / 2.0 - b / 4.0;
        double co = r / 2.0 - b / 2.0;
        pixRGB[i].red = y * 255.0;
        pixRGB[i].green = (co + 0.5) * 255.0;
        pixRGB[i].blue = (cg + 0.5) * 255.0;

    }
}

void CImage::YCoCgtoRGB() {
    for (int i = 0; i < data->size; i++) {
        double y = pixRGB[i].red / 255.0;
        double co = pixRGB[i].green / 255.0 - 0.5;
        double cg = pixRGB[i].blue / 255.0 - 0.5;
        double r = y + co - cg;
        double g = y + cg;
        double b = y - co - cg;
        r = r < 0 ? 0 : r;
        r = r > 1 ? 1 : r;
        b = b < 0 ? 0 : b;
        b = b > 1 ? 1 : b;
        g = g < 0 ? 0 : g;
        g = g > 1 ? 1 : g;
        pixRGB[i].red = r * 255.0;
        pixRGB[i].green = g * 255.0;
        pixRGB[i].blue = b * 255.0;
    }
}