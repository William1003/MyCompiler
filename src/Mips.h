#ifndef _MIPS_H_
#define _MIPS_H_

#include<fstream>
#include<vector>
#include"TempCode.h"
#include"SymbolTable.h"
#include"Quaternion.h"
#define REG_NUM 10

using namespace std;

class Mips
{
private:
	ifstream tempCodeFile;
	ofstream mips;
	TempCode& tempCode;
	SymbolTable& symbolTable;
	int tregs[10] = { 0 };

public:
	Mips(TempCode& tempCode, SymbolTable& symbolTable);
	void generate();
	int findFreeTReg();
	void freeReg(int i);
	void loadValue(string regName, string name, string domain, bool loadConst, string& value, bool& get);
	void saveValue(string name, string domain, string regName);
};
#endif // !_MIPS_H_
