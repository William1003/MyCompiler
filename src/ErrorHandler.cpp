#include"ErrorHandler.h"
#include<iostream>

using namespace std;

ErrorHandler::ErrorHandler()
{

}

void ErrorHandler::lexicalError()
{
	cout << "LEXICAL ERROR!" << endl;
}

void ErrorHandler::syntaxError(int line, string functionName)
{
	cout << "SYNTAX ERROR! @ line " << line << ", in " << functionName << endl;
}