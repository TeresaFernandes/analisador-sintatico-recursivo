#include<stdio.h>
class Token
{
public:
  Token(){}
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
