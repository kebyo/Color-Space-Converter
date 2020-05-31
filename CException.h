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

    string getError();

    FILE *getFile();

private:
    string error;
    FILE *file;
};


#endif //COLORSPACECONVERSION_CEXCEPTION_H
