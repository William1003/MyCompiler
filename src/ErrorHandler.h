#ifndef _ERROR_HANDLER_H_
#define _ERROR_HANDLER_H_

#include<string>
#include<fstream>
#include<iostream>
#include"constValue.h"

using namespace std;

class ErrorHandler
{
private:
	ofstream errorFile;
	ofstream myErrorFile;
	int errorCount;

public:
	ErrorHandler();
	void lexicalError(int line);
	void syntaxError(int line, string functionName);
	void error(int line, ErrorCode errorCode);
	~ErrorHandler()
	{
		cout << "There is " << errorCount << " errors in the testfile!" << endl;
	}
};

#endif // !_ERROR_HANDLER_H_
