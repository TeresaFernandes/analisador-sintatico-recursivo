#ifndef SYNTATIC_ERROR_H
#define SYNTATIC_ERROR_H

#include "AnalysisError.h"

#include <string>

class SyntaticError : public AnalysisError
{
public:

    SyntaticError(string msg, int position = -1){
    	this->message = msg;
    	this->position = position;
    }
};

#endif
