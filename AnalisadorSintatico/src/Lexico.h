#ifndef LEXICO_H
#define LEXICO_H

#include "Token.h"
#include "LexicalError.h"

class Lexico
{
public:

    Token *nextToken() throw (LexicalError);

};

#endif
