#ifndef TOKEN_H
#define TOKEN_H


#include "Lexico/lex.yy.c"

class Token
{

	public:

		Token(){}

		Token(int id_, char * lexeme_, int position_){
		   this->id=id_;
		   this->lexeme=lexeme_;
		   this->position=position_;
		}

		int getId() const { return id; }
		char * getLexeme() const{ return lexeme; }
		int getPosition() const{ return position; }

		//enum TokenId {ZERO_PARA_A_EXECUCAO, STRING_=0, CHAR_, REAL_NUMBER_, INTEGER_NUMBER_, BOOLEAN_, IDENTIFIER_, SET_, OF_, RECORD_, END_, LIST_, ARRAY_, IN_, NOT_, NIL_, CASE_, ELSE_, THEN_, IF_, DO_, DOWNTO_, TO_, FOR_, WHILE_, BEGIN_, ASSIGNMENT_, VAR_, DIV_, MOD_, AND_, OR_, WRITE_, WRITELN_, READ_, READLN_, FUNCTION_, PROCEDURE_, TYPE_, CONST_, PROGRAM_, USES_, COMMA_, DOT_, MINUS_, PLUS_, UPARROW_, COLON_, SEMI_COLON_, DOTDOT_, LBRAC_, RBRAC_, LPAREN_, RPAREN_, ARROBA_, LBRAC_BAR_, RBRAC_BAR_, CONS_, OP_MULT_, OP_DIV_, EQUAL_, NOTEQUAL_, LT_, LE_, GT_, GE_, TYPE_INTEGER_, TYPE_REAL_, TYPE_CHAR_, TYPE_STRING_, TYPE_BOOLEAN_, TRUE_, FALSE_, SIGN_};

	private:
		int id;
		char * lexeme;
		int position;
};

#endif
