#ifndef ANALYSIS_ERROR_H
#define ANALYSIS_ERROR_H

#include <string>

class AnalysisError{

	protected:
		char * message;
		int position;


	public:
		AnalysisError(){}

		AnalysisError(char* msg, int position = -1){
			this->message = msg;
			this->position = position;
		}

		char* getMessage() const { return message; }
		int getPosition() const { return position; }

};
#endif
