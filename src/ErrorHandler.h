#ifndef _ERROR_HANDLER_H_
#define _ERROR_HANDLER_H_

#include<string>
#include<fstream>
#include"constValue.h"

using namespace std;

class ErrorHandler
{
private:
	ofstream errorFile;
	ofstream myErrorFile;

public:
	ErrorHandler();
	void lexicalError(int line);
	void syntaxError(int line, string functionName);
	void error(int line, ErrorCode errorCode);
};

#endif // !_ERROR_HANDLER_H_
