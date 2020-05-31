#include "CImage.h"

void CImage::read1file(SInput console) {
    FILE *f = fopen(console.inputFile, "rb");
    if (!f) {
        throw CException("Input file didn't open\n");
    }
    file = f;
    if (fscanf(f, "P%i%i%i%i\n", &this->version, &this->width, &this->height, &max_val) != 4) {
        throw CException("Wrong amount data in file", f);
    }
    if (version != 6) {
        throw CException("Expected version 6");
    }
    size = width * height;
    unsigned char *buffer;
    buffer = new unsigned char[size * 3];
    pixRGB = new RGB[size];
    fread(buffer, sizeof(unsigned char) * 3, size, f);
    for (int i = 0; i < size; i += 3) {
        pixRGB[i] = {(double) buffer[i], (double) buffer[i + 1], (double) buffer[i + 3]};
    }
    delete[] buffer;
    fclose(f);
    return;
}

void CImage::read3files(SInput console) {

}

CImage::~CImage() {
    delete[] pix;
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
    for (int i = 0; i < size; i++) {
        if (version == 5) {
            pix[i] = (1 - pix[i] / 255.0) * 255.0;
        } else {
            pixRGB[i].red = (1 - pixRGB[i].red / 255.0) * 255.0;
            pixRGB[i].green = (1 - pixRGB[i].green / 255.0) * 255.0;
            pixRGB[i].blue = (1 - pixRGB[i].blue / 255.0) * 255.0;
        }
    }
}

void CImage::CMYtoRGB() {
    for (int i = 0; i < size; i++) {
        if (version == 5) {
            pix[i] = (1 - pix[i] / 255.0) * 255.0;
        } else {
            pixRGB[i].red = (1 - pixRGB[i].red / 255.0) * 255.0;
            pixRGB[i].green = (1 - pixRGB[i].green / 255.0) * 255.0;
            pixRGB[i].blue = (1 - pixRGB[i].blue / 255.0) * 255.0;
        }
    }
}

void CImage::RGBtoHSL() {
    for (int i = 0; i < size; i++) {
        if (version == 5) {

        } else {
            double r = pixRGB[i].red / 255.0;
            double g = pixRGB[i].green / 255.0;
            double b = pixRGB[i].blue / 255.0;
            double Cmax = max(r, max(g, b));
            double Cmin = min(r, min(g, b));
            double d = Cmax - Cmin;
            double l = (Cmax + Cmin) / 2.0;
            double h;
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
            double s = (d) / (1.0 - abs(1 - (Cmax + Cmin)));
            pixRGB[i].red = h * 255.0 / 360.0;
            pixRGB[i].green = s * 255.0;
            pixRGB[i].blue = l * 255, 0;
        }
    }
}

void CImage::HSLtoRGB() {
    for (int i = 0; i < size; i++) {
        double h = pixRGB[i].red / 255.0 * 360.0;
        double s = pixRGB[i].blue / 255.0;
        double l = pixRGB[i].green / 255.0;
        double c = (1 - abs(2 * l - 1)) * s;
        double x = c * (1 - abs(fmod(h / 60.0, 2) - 1));
        double m = l - c / 2.0;
        double r, g, b;
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
    for (int i = 0; i < size; i++) {
        double r = pixRGB[i].red / 255.0;
        double g = pixRGB[i].green / 255.0;
        double b = pixRGB[i].blue / 255.0;
        double MAX = max(r, max(g, b));
        double MIN = min(r, min(g, b));
        double d = MAX - MIN;
        double h, s, v;
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
    for (int i = 0; i < size; i++) {
        double h = pixRGB[i].red / 255.0 * 360.0;
        double s = pixRGB[i].blue / 255.0;
        double v = pixRGB[i].green / 255.0;
        double c = v * s;
        double x = c * (1 - abs(fmod(h / 60.0, 2) - 1));
        double m = v - c;
        double r, g, b;
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
    for (int i = 0; i < size; i++) {
        double r = pixRGB[i].red / 255.0;
        double g = pixRGB[i].green / 255.0;
        double b = pixRGB[i].blue / 255.0;
        double y = kr * r + kg * g + kb * b;
        double pb = (1 / 2) * (b - y) / (1 - kb);
        double pr = (1 / 2) * (r - y) / (1 - kr);
        pixRGB[i].red = y * 255.0;
        pixRGB[i].green = (pb + 0.5) * 255.0;
        pixRGB[i].blue = (pr + 0.5) * 255.0;
    }
}

void CImage::RGBtoYCbCr709() {
    double kr = 0.0722, kg = 0.2126, kb = 0.7152;
    for (int i = 0; i < size; i++) {
        double r = pixRGB[i].red / 255.0;
        double g = pixRGB[i].green / 255.0;
        double b = pixRGB[i].blue / 255.0;
        double y = kr * r + kg * g + kb * b;
        double pb = (1 / 2) * (b - y) / (1 - kb);
        double pr = (1 / 2) * (r - y) / (1 - kr);
        pixRGB[i].red = y * 255.0;
        pixRGB[i].green = (pb + 0.5) * 255.0;
        pixRGB[i].blue = (pr + 0.5) * 255.0;
    }
}

void CImage::YCbCr601toRGB() {
    double kr = 0.299, kg = 0.587, kb = 0.114;
    for (int i = 0; i < size; i++) {
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
    for (int i = 0; i < size; i++) {
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
    for (int i = 0; i < size; i++) {
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
    for (int i = 0; i < size; i++) {
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