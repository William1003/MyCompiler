#include"Output.h"
#include<cstdlib>
#include<iostream>

using namespace std;

extern const char* SymbolName[SYMBOL_NUM];

Output::Output(string& ofname)
{
	out.open(ofname, ios::out);
	if (!out)
	{
		cout << "Open OutputFile Failed!" << endl;
		exit(EXIT_SUCCESS);
	}
}

void Output::lexicalAnalysisOutput(SymbolCode symbolCode, string token)
{
	out << SymbolName[symbolCode] << " " << token << endl;
}