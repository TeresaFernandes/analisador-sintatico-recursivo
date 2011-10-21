#ifndef TOKEN_H
#define TOKEN_H

#include "Constants.h"

#include <string>

class Token {
	private:
		TokenId id;
		std::string lexeme;
		int position;

	public:

		Token(){}
		Token(TokenId id, const std::string &lexeme, int position){
			this->id = id;
			this->lexeme = lexeme;
			this->position = position;
		}

		TokenId getId() const { return id; }
		const std::string &getLexeme() const { return lexeme; }
		int getPosition() const { return position; }

};

#endif
