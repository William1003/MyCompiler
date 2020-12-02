#ifndef _MIPS_H_
#define _MIPS_H_

#include<fstream>
#include<vector>
#include"TempCode.h"
#include"SymbolTable.h"
#include"Quaternion.h"
#define REG_NUM 10
#include<stack>
using namespace std;

class Mips
{
private:
	ifstream tempCodeFile;
	ofstream mips;
	TempCode& tempCode;
	SymbolTable& symbolTable;
	int tregs[10] = { 0 };
	stack<Quaternion> pushParaStack;

public:
	Mips(TempCode& tempCode, SymbolTable& symbolTable);
	void generate();
	int findFreeTReg();
	void freeReg(int i);
	void loadValue(string regName, string name, string domain, bool loadConst, string& value, bool& get);
	void saveValue(string name, string domain, string regName);
	void pushPara(Quaternion q, string currentDomain);
	string subreplace(string resource_str, string sub_str, string new_str)
	{
		string::size_type pos = 0;
		int begin = 0;
		while ((pos = resource_str.find(sub_str, begin)) != string::npos)   //替换所有指定子串
		{
			resource_str.replace(pos, sub_str.length(), new_str);
			begin += new_str.length();
		}
		return resource_str;
	}
};
#endif // !_MIPS_H_
