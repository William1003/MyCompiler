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
	string outputContent;
	string backupString;
	ofstream debugFile;

public:
	Output(string& ofname);
	void lexicalAnalysisOutput(SymbolCode symbolCode, string token);
	void syntaxAnalysisOutput(string syntaxName);
	void backup();
	void retract();
	void outputToFile();
};

#endif // !_OUTPUT_H_
