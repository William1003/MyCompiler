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

void ErrorHandler::syntaxError()
{
	cout << "SYNTAX ERROR!" << endl;
}