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