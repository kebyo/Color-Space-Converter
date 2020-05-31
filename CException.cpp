//
// Created by ssiie on 31.05.2020.
//

#include "CException.h"

CException::CException(string error) {
    this->error = error;
}

CException::CException(string error, FILE *f) {
    this->error = error;
    this->file = f;
}

FILE *CException::getFile() {
    return this->file;
}

string CException::getError() {
    return this->error;
}

CException::CException(string error, FILE *f1, FILE *f2, FILE *f3) {
    this->error = error;
    this->file = f1;
    this->file2 = f2;
    this->file3 = f3;
}

FILE *CException::getFile2() {
    return this->file2;
}

FILE *CException::getFile3() {
    return this->file3;
}