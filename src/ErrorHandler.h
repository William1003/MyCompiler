#ifndef _ERROR_HANDLER_H_
#define _ERROR_HANDLER_H_

#include<string>
#include<fstream>

using namespace std;

class ErrorHandler
{
private:
	ofstream errorFile;
	ofstream myErrorFile;

public:
	ErrorHandler();
	void lexicalError();
	void syntaxError(int line, string functionName);
};

#endif // !_ERROR_HANDLER_H_
