#include "Token.h"
class ErroSintatico{

	protected:
		char* message;
		int position;

	public:
		ErroSintatico(){}

		ErroSintatico(char* msg, int position = -1){
			this->message = msg;
			this->position = position;
			printf("LINE %d: %s", position, message);
		}

		char* getMessage() const { return message; }
		int getPosition() const { return position; }

};