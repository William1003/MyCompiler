#include<string>
#include"Output.h"
#include"LexicalAnalysis.h"
#include"SyntaxAnalysis.h"
#include"iostream"
#include"SymbolTable.h"
#include"TempCode.h"
#include"Mips.h"
using namespace std;

int main()
{
	cout << "欢迎光临" << endl;
	string ifname = "testfile.txt";
	string ofname = "output.txt";
	Output myOutput(ofname);
	ErrorHandler errorHandler;
	LexicalAnalysis lexicalAnalysis(myOutput, errorHandler);
	lexicalAnalysis.readFile(ifname);
	SymbolTable table;
	TempCode tempCode(table);
	Mips mips(tempCode, table);
	SyntaxAnalysis syntaxAnalysis(errorHandler, lexicalAnalysis, myOutput, table, tempCode);
	syntaxAnalysis.startSyntaxAnalysis();
	// myOutput.outputToFile();
	tempCode.outputToFile();
	table.output();
	mips.generate();
	return 0;
}