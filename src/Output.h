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
	ofstream debug;

public:
	Output(string& ofname);
	~Output()
	{
		out.close();
		debugFile.close();
	}
	void lexicalAnalysisOutput(SymbolCode symbolCode, string token);
	void syntaxAnalysisOutput(string syntaxName);
	void backup();
	void backup(int line);

	void retract(int line);
	void outputToFile();
	void printChar(char c);
};

#endif // !_OUTPUT_H_
