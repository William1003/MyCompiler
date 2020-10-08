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
	void syntaxAnalysisOutput(string syntaxName, int line);
	void backup();
	void backup(int line);

	void retract(int line);
	void outputToFile();
};

#endif // !_OUTPUT_H_
