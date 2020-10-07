#include<string>
#include"Output.h"
#include"LexicalAnalysis.h"
#include"SyntaxAnalysis.h"

using namespace std;

int main()
{
	string ifname = "testfile.txt";
	string ofname = "output.txt";
	Output myOutput(ofname);
	ErrorHandler errorHandler;
	LexicalAnalysis lexicalAnalysis(myOutput);
	lexicalAnalysis.readFile(ifname);
	SyntaxAnalysis syntaxAnalysis(errorHandler, lexicalAnalysis, myOutput);
	syntaxAnalysis.startSyntaxAnalysis();
	myOutput.outputToFile();
	return 0;
}