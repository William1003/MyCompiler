#include"ErrorHandler.h"
#include<iostream>
#include"constValue.h"

using namespace std;

ErrorHandler::ErrorHandler()
{
	errorFile.open("error.txt");
	if (!errorFile)
	{
		cout << "Open ErrorFile Failed!" << endl;
		exit(EXIT_SUCCESS);
	}
	myErrorFile.open("myError.txt");
}

void ErrorHandler::lexicalError(int line)
{
	cout << "LEXICAL ERROR!" << endl;
	cout << line << " a" << endl;
	errorFile << line << " a" << endl;
}

void ErrorHandler::syntaxError(int line, string functionName)
{
	cout << "SYNTAX ERROR! @ line " << line << ", in " << functionName << endl;
}

void ErrorHandler::error(int line, ErrorCode errorCode)
{
	errorFile << line << " " << ErrorContent[errorCode] << endl;
}