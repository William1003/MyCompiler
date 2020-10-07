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
	debugFile.open("debug.txt", ios::out);
}

void Output::lexicalAnalysisOutput(SymbolCode symbolCode, string token)
{
	outputContent += SymbolName[symbolCode];
	outputContent += " ";
	outputContent += token;
	outputContent += "\n";
	debugFile << SymbolName[symbolCode] << " " << token << endl;
}

void Output::syntaxAnalysisOutput(string syntaxName)
{
	outputContent += "<";
	outputContent += syntaxName;
	outputContent += ">";
	outputContent += "\n";
	debugFile << "<" << syntaxName << ">" << endl;
}

void Output::backup()
{
	backupString = outputContent;
	debugFile << "backup!" << endl;
}

void Output::retract()
{
	outputContent = backupString;
	backupString = "";
	debugFile << "retract!" << endl;
}

void Output::outputToFile()
{
	out << outputContent;
	out.close();
	debugFile.close();
}
