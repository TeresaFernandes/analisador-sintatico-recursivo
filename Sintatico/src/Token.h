#include<stdio.h>

class Token
{
public:
  Token(){
    this->id=-1;
    this->lexeme=0;
    this->line=0;
  }
  Token(int t, char * l, int ln){
    this->id=t;
    this->lexeme=l;
    this->line=ln;
  }
  ~Token(){}

  char* lexeme;
  int id;
  int line;
  
  bool isNull(){
    return (this->id==0 && this->lexeme==NULL && this->line==0);
  }
};
