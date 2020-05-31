//
// Created by ssiie on 31.05.2020.
//

#ifndef COLORSPACECONVERSION_CEXCEPTION_H
#define COLORSPACECONVERSION_CEXCEPTION_H

#include <string>

using std::string;

class CException {
public:
    CException(string error);

    CException(string error, FILE *f);

    CException(string error, FILE *f1, FILE *f2, FILE *f3);

    string getError();

    FILE *getFile();

    FILE *getFile2();

    FILE *getFile3();

private:
    string error;
    FILE *file = nullptr;
    FILE *file2 = nullptr;
    FILE *file3 = nullptr;
};


#endif //COLORSPACECONVERSION_CEXCEPTION_H
