#include <iostream>
#include "CException.h"
#include "CImage.h"
#include "string.h"

using namespace std;
SInput input;

int main(int argc, char *argv[]) {
    try {
        if (argc != 11) {
            CException("Wrong amount of arguments\n");
        }
        string tmp;
        for (int i = 1; i < argc;) {
            tmp = argv[i];
            if (tmp == "-f") {
                i++;
                tmp = argv[i];
                if (tmp == "RGB") {
                    input.from = 0;
                    i++;
                    continue;
                }
                if (tmp == "HSL") {
                    input.from = 1;
                    i++;
                    continue;
                }
                if (tmp == "HSV") {
                    input.from = 2;
                    i++;
                    continue;
                }
                if (tmp == "YCbCr.601") {
                    input.from = 3;
                    i++;
                    continue;
                }
                if (tmp == "YCbCr.709") {
                    input.from = 4;
                    i++;
                    continue;
                }
                if (tmp == "YCoCg") {
                    input.from = 5;
                    i++;
                    continue;
                }
                if (tmp == "CYM") {
                    input.from = 6;
                    i++;
                    continue;
                } else {
                    throw CException("Other color space\n");
                }
            }
            tmp = argv[i];
            if (tmp == "-t") {
                i++;
                tmp = argv[i];
                if (tmp == "RGB") {
                    input.to = 0;
                    i++;
                    continue;
                }
                if (tmp == "HSL") {
                    input.to = 1;
                    i++;
                    continue;
                }
                if (tmp == "HSV") {
                    input.to = 2;
                    i++;
                    continue;
                }
                if (tmp == "YCbCr.601") {
                    input.to = 3;
                    i++;
                    continue;
                }
                if (tmp == "YCbCr.709") {
                    input.to = 4;
                    i++;
                    continue;
                }
                if (tmp == "YCoCg") {
                    input.to = 5;
                    i++;
                    continue;
                }
                if (tmp == "CYM") {
                    input.to = 6;
                    i++;
                    continue;
                } else {
                    throw CException("Other color space\n");
                }
            }
            tmp = argv[i];
            if (tmp == "-i") {
                i++;
                input.countInput = atoi(argv[i]);
                if (input.countInput != 1 && input.countInput != 3) {
                    throw CException("Wrong amount of input files");
                }
                i++;
                input.inputFile = argv[i];
                i++;
                continue;
            }
            tmp = argv[i];
            if (tmp == "-o") {
                i++;
                input.countOutput = atoi(argv[i]);
                if (input.countOutput != 1 && input.countOutput != 3) {
                    throw CException("Wrong amount of output files");
                }
                i++;
                input.outputFile = argv[i];
                i++;
                continue;
            }
        }
        CImage img;
        if (input.countInput == 1) {
            img.read1file(input);
        } else {
            img.read3files(input);
        }
//        img.convert(input);
        if (input.countOutput == 1) {
            img.write1file(input);
        } else {
            img.write3files(input);
        }
        return 0;
    } catch (CException &exception) {
        cerr << exception.getError();
        if (exception.getFile()) {
            fclose(exception.getFile());
        }
        if (exception.getFile2()) {
            fclose(exception.getFile2());
        }
        if (exception.getFile3()) {
            fclose(exception.getFile3());
        }
        exit(1);
    }
}
