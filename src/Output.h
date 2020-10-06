#ifndef _OUTPUT_H_
#define _OUTPUT_H_

#include<fstream>
#include<string>
#include"constValue.h"

using namespace std;

class Output
{
private:
	ofstream out;

public:
	Output(string& ofname);
	void lexicalAnalysisOutput(SymbolCode symbolCode, string token);
	void syntaxAnalysisOutput();
};

#endif // !_OUTPUT_H_
