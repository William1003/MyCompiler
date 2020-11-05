#include<string>
#include"Output.h"
#include"LexicalAnalysis.h"
#include"SyntaxAnalysis.h"
#include"iostream"
#include"SymbolTable.h"

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
	SyntaxAnalysis syntaxAnalysis(errorHandler, lexicalAnalysis, myOutput, table);
	syntaxAnalysis.startSyntaxAnalysis();
	// myOutput.outputToFile();
	table.output();
	return 0;
}