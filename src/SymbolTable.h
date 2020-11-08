#ifndef _SYMBOL_TABLE_H_
#define _SYMBOL_TABLE_H_

#include<vector>
#include"SymbolTableItem.h"
#include<map>
#include<fstream>

class SymbolTable
{
private:
	int count = 0;
	ofstream symbolTableFile;
	int index = 0;
	map<string, int> addrCount;

public:
	SymbolTable();
	bool push(const SymbolTableItem& item);
	bool hasRet(string functionName);
	bool hasItem(string name, string domain);
	bool hasFunction(string functionName);
	void output();
	SymbolTableItem& getCurrent();
	SymbolTableItemType getParaType(string functionName);
	SymbolTableItemType getCurrentType();
	SymbolTableItemType getFunctionType(string functionName);
	SymbolTableItemKind getKind(string name);
	SymbolTableItemKind getCurrentKind();
	bool findParameter(string functionName);
	bool nextParameter(string functionName);
	vector<SymbolTableItem> table;
	SymbolTableItem getItem(string name, string domain);
};

#endif // !_SYMBOL_TABLE_H_
