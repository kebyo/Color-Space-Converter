#include "CImage.h"

void CImage::read1file(SInput console) {
    FILE *f = fopen(console.inputFile, "rb");
    if (!f) {
        throw CException("Input file didn't open\n");
    }
    data = new SMetaData[1];
    data[0].file = f;
    if (fscanf(f, "P%i%i%i%i\n", &this->data[0].version, &this->data[0].width, &this->data[0].height,
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
    char name1[n];
    for (int i = 0; i < filename1.size(); i++) {
        name1[i] = filename1[i];
    }
    char name2[n];
    for (int i = 0; i < n; i++) {
        name2[i] = filename2[i];
    }
    char name3[n];
    for (int i = 0; i < n; i++) {
        name3[i] = filename3[i];
    }
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
    if (fscanf(f1, "P%i%i%i%i\n", &this->data[0].version, &this->data[0].width, &this->data[0].height,
               &data[0].max_val) != 4) {
        throw CException("Wrong amount meta data in _1");
    }
    if (fscanf(f2, "P%i%i%i%i\n", &this->data[1].version, &this->data[1].width, &this->data[1].height,
               &data[1].max_val) != 4) {
        throw CException("Wrong amount meta data in _2");
    }
    if (fscanf(f3, "P%i%i%i%i\n", &this->data[2].version, &this->data[2].width, &this->data[2].height,
               &data[2].max_val) != 4) {
        throw CException("Wrong amount meta data in _3");
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
    if (data[1].version != mv || data[2].version != mv) {
        throw CException("Different max values", f1, f2, f3);
    }
    int len = data[0].size;
    unsigned char *buffer1 = new unsigned char[len];
    fread(buffer1, sizeof(unsigned char), len, f1);
    unsigned char *buffer2 = new unsigned char[len];
    fread(buffer2, sizeof(unsigned char), len, f1);
    unsigned char *buffer3 = new unsigned char[len];
    fread(buffer3, sizeof(unsigned char), len, f1);
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
    fprintf(new_f, "P%i\n%i %i\n%i\n", 6, data[0].width, data[0].height, data[0].max_val);
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
    string filename1 = console.inputFile;
    string filename2 = filename1;
    string filename3 = filename1;
    filename1.insert(filename1.size() - 4, "_1");
    filename2.insert(filename2.size() - 4, "_2");
    filename3.insert(filename3.size() - 4, "_3");
    int n = filename1.size();
    char name1[n];
    for (int i = 0; i < filename1.size(); i++) {
        name1[i] = filename1[i];
    }
    char name2[n];
    for (int i = 0; i < n; i++) {
        name2[i] = filename2[i];
    }
    char name3[n];
    for (int i = 0; i < n; i++) {
        name3[i] = filename3[i];
    }
    f1 = fopen(name1, "rb");
    if (!f1) {
        throw CException("Output file _1 didn't open");
    }
    f2 = fopen(name2, "rb");
    if (!f2) {
        throw CException("Output file _2 didn't open");
    }
    f3 = fopen(name3, "rb");
    if (!f3) {
        throw CException("Output file _3 didn't open");
    }
    fprintf(f1, "P%i\n%i %i\n%i\n", 5, data[0].width, data[0].height, data[0].max_val);
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
        double d = Cmax - Cmin;
        double l = (Cmax + Cmin) / 2.0;
        double h = 0;
        if (d == 0) {
            h = 0;
        } else {
            if (Cmax == r && g >= b) {
                h = 60.0 * (g - b) / d;
            }
            if (Cmax == r && g < b) {
                h = 60.0 * (g - b) / d + 360;
            }
            if (Cmax == g) {
                h = 60.0 * (b - r) / d + 120;
            }
            if (Cmax == b) {
                h = 60.0 * (r - g) / d + 240;
            }
        }
        double s = 0;
        if (l == 0 || Cmax == Cmin) {
            s = 0;
        } else if (l > 0 && l <= 0.5) {
            s = d / (2 * l);
        } else if (l < 1.0) {
            s = d / (2.0 - 2.0 * l);
        }
        pixRGB[i].red = h * 255.0 / 360.0;
        pixRGB[i].green = s * 255.0;
        pixRGB[i].blue = l * 255, 0;
    }
}

void CImage::HSLtoRGB() {
    for (int i = 0; i < data->size; i++) {
        double h = pixRGB[i].red / 255.0 * 360.0;
        double s = pixRGB[i].blue / 255.0;
        double l = pixRGB[i].green / 255.0;
        double c = (1 - abs(2 * l - 1)) * s;
        double x = c * (1 - abs(fmod(h / 60.0, 2) - 1));
        double m = l - c / 2.0;
        double r = 0, g = 0, b = 0;
        if (h >= 0 && h < 60) {
            r = c + m;
            g = x + m;
            b = 0 + m;
        }
        if (h >= 60 && h < 120) {
            r = x + m;
            g = c + m;
            b = 0 + m;
        }
        if (h >= 120 && h < 180) {
            r = 0 + m;
            g = c + m;
            b = x + m;
        }
        if (h >= 180 && h < 240) {
            r = 0 + m;
            g = x + m;
            b = c + m;
        }
        if (h >= 240 && h < 300) {
            r = x + m;
            g = 0 + m;
            b = c + m;
        }
        if (h >= 300 && h < 360) {
            r = c + m;
            g = 0 + m;
            b = x + m;
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
        double d = MAX - MIN;
        double h = 0, s = 0, v = 0;
        if (d == 0) {
            h = 0;
        } else {
            if (MAX == r && g >= b) {
                h = 60.0 * (g - b) / d;
            }
            if (MAX == r && g < b) {
                h = 60.0 * (g - b) / d + 360;
            }
            if (MAX == g) {
                h = 60.0 * (b - r) / d + 120;
            }
            if (MAX == b) {
                h = 60.0 * (r - g) / d + 240;
            }
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
        double s = pixRGB[i].blue / 255.0;
        double v = pixRGB[i].green / 255.0;
        double c = v * s;
        double x = c * (1 - abs(fmod(h / 60.0, 2) - 1));
        double m = v - c;
        double r = 0, g = 0, b = 0;
        if (h >= 0 && h < 60) {
            r = c + m;
            g = x + m;
            b = 0 + m;
        }
        if (h >= 60 && h < 120) {
            r = x + m;
            g = c + m;
            b = 0 + m;
        }
        if (h >= 120 && h < 180) {
            r = 0 + m;
            g = c + m;
            b = x + m;
        }
        if (h >= 180 && h < 240) {
            r = 0 + m;
            g = x + m;
            b = c + m;
        }
        if (h >= 240 && h < 300) {
            r = x + m;
            g = 0 + m;
            b = c + m;
        }
        if (h >= 300 && h < 360) {
            r = c + m;
            g = 0 + m;
            b = x + m;
        }
        pixRGB[i].red = r * 255.0;
        pixRGB[i].green = g * 255.0;
        pixRGB[i].blue = b * 255.0;
    }
}

void CImage::RGBtoYCbCr601() {
    double kr = 0.299, kg = 0.587, kb = 0.114;
    for (int i = 0; i < data->size; i++) {
        double r = pixRGB[i].red / 255.0;
        double g = pixRGB[i].green / 255.0;
        double b = pixRGB[i].blue / 255.0;
        double y = kr * r + kg * g + kb * b;
        double pb = 0.5 * (b - y) / (1.0 - kb);
        double pr = 0.5 * (r - y) / (1.0 - kr);
        pixRGB[i].red = y * 255.0;
        pixRGB[i].green = (pb + 0.5) * 255.0;
        pixRGB[i].blue = (pr + 0.5) * 255.0;
    }
}

void CImage::RGBtoYCbCr709() {
    double kr = 0.0722, kg = 0.2126, kb = 0.7152;
    for (int i = 0; i < data->size; i++) {
        double r = pixRGB[i].red / 255.0;
        double g = pixRGB[i].green / 255.0;
        double b = pixRGB[i].blue / 255.0;
        double y = kr * r + kg * g + kb * b;
        double pb = 0.5 * (b - y) / (1.0 - kb);
        double pr = 0.5 * (r - y) / (1.0 - kr);
        pixRGB[i].red = y * 255.0;
        pixRGB[i].green = (pb + 0.5) * 255.0;
        pixRGB[i].blue = (pr + 0.5) * 255.0;
    }
}

void CImage::YCbCr601toRGB() {
    double kr = 0.299, kg = 0.587, kb = 0.114;
    for (int i = 0; i < data->size; i++) {
        double y = pixRGB[i].red / 255.0;
        double pb = pixRGB[i].green / 255.0 - 0.5;
        double pr = pixRGB[i].blue / 255.0 - 0.5;
        double r = (y + pb * (2.0 - 2.0 * kr));
        double g = (y - (kb / kg) * (2.0 - 2.0 * kb) * pb - (kr / kg) * (2.0 - 2.0 * kr) * pr);
        double b = (y + (2.0 - 2.0 * kb) * pb);
        r = r < 0 ? 0 : r > 1 ? 1 : r;
        g = g < 0 ? 0 : g > 1 ? 1 : g;
        r = b < 0 ? 0 : b > 1 ? 1 : b;
        pixRGB[i].red = r * 255.0;
        pixRGB[i].green = r * 255.0;
        pixRGB[i].blue = r * 255.0;
    }
}

void CImage::YCbCr709toRGB() {
    double kr = 0.0722, kg = 0.2126, kb = 0.7152;
    for (int i = 0; i < data->size; i++) {
        double y = pixRGB[i].red / 255.0;
        double pb = pixRGB[i].green / 255.0 - 0.5;
        double pr = pixRGB[i].blue / 255.0 - 0.5;
        double r = (y + pb * (2.0 - 2.0 * kr));
        double g = (y - (kb / kg) * (2.0 - 2.0 * kb) * pb - (kr / kg) * (2.0 - 2.0 * kr) * pr);
        double b = (y + (2.0 - 2.0 * kb) * pb);
        r = r < 0 ? 0 : (r > 1 ? 1 : r);
        g = g < 0 ? 0 : (g > 1 ? 1 : g);
        b = b < 0 ? 0 : (b > 1 ? 1 : b);
        pixRGB[i].red = r * 255.0;
        pixRGB[i].green = r * 255.0;
        pixRGB[i].blue = r * 255.0;
    }
}

void CImage::RGBtoYCoCg() {
    for (int i = 0; i < data->size; i++) {
        double r = pixRGB[i].red / 255.0;
        double g = pixRGB[i].green / 255.0;
        double b = pixRGB[i].blue / 255.0;
        double y = r / 4.0 + g / 2.0 + b / 4.0;
        double cg = -r / 4.0 + g / 2.0 - b / 4.0;
        double co = r / 1.0 - b / 2.0;
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
        r = r < 0 ? 0 : (r > 1 ? 1 : r);
        g = g < 0 ? 0 : (g > 1 ? 1 : g);
        b = b < 0 ? 0 : (b > 1 ? 1 : b);
        pixRGB[i].red = r * 255.0;
        pixRGB[i].green = g * 255.0;
        pixRGB[i].blue = b * 255.0;
    }
}