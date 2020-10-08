#ifndef _SYMBOL_TABLE_ITEM_H_
#define _SYMBOL_TABLE_ITEM_H_

#include<string>
using namespace std;

class SymbolTableItem
{
private:
	string name;
	int functionType;

public:
	SymbolTableItem(string name, int functionType);
	bool hasName(string name);
	bool hasRet();
};

#endif // !_SYMBOL_TABLE_ITEM_H_
