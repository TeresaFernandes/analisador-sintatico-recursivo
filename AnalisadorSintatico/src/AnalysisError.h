#ifndef ANALYSIS_ERROR_H
#define ANALYSIS_ERROR_H

#include <string>

class AnalysisError{

	protected:
		string message;
		int position;


	public:
		AnalysisError(){}

		AnalysisError(string msg, int position = -1){
			this->message = msg;
			this->position = position;
		}

		string getMessage() const { return message; }
		int getPosition() const { return position; }

};
#endif
