#include"ErrorHandler.h"
#include<iostream>

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

void ErrorHandler::lexicalError()
{
	cout << "LEXICAL ERROR!" << endl;
}

void ErrorHandler::syntaxError(int line, string functionName)
{
	cout << "SYNTAX ERROR! @ line " << line << ", in " << functionName << endl;
}