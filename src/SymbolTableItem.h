#ifndef _SYMBOL_TABLE_ITEM_H_
#define _SYMBOL_TABLE_ITEM_H_

#include<string>
#include<map>
#include<vector>
#include"constValue.h"

using namespace std;

class IntOrChar
{
private:
	int intValue = 0;
	char charValue = '\0';

public:
	IntOrChar() {}
	IntOrChar(int value) { intValue = value; charValue = '\0'; }
	IntOrChar(char value) { charValue = value; intValue = 0; }
	void setValue(int value)
	{
		this->intValue = value;
	}
	void setValue(char value)
	{
		this->charValue = value;
	}
	int getIntValue()
	{
		return intValue;
	}
	char getCharValue()
	{
		return charValue;
	}
};

class SymbolTableItem
{
private:
	string name;
	string domain;
	SymbolTableItemKind kind;
	SymbolTableItemType type;
	int intValue = 0;
	char charValue = '\0';
	int dimension = 0;
	int row = 1;
	int column = 1;
	int currentRow = 0;
	int currentColumn = 0;
	vector<IntOrChar> values1D;
	vector<vector<IntOrChar>> values2D;
	int parameterCount = 0;
	
public:
	SymbolTableItem(string name, string domain, SymbolTableItemKind kind, SymbolTableItemType type) : 
		name(name), domain(domain), kind(kind), type(type) {}
	SymbolTableItem(string name, string domain, SymbolTableItemKind kind, int value) :
		name(name), domain(domain), kind(kind), type(INT), intValue(value) {}
	SymbolTableItem(string name, string domain, SymbolTableItemKind kind, char value) :
		name(name), domain(domain), kind(kind), type(CHAR), charValue(value) {}
	SymbolTableItem(string name, string domain, SymbolTableItemType type, int dimension, int row, int column) :
		name(name), domain(domain), kind(ARRAY), type(type), dimension(dimension), row(row), column(column) 
	{
		for (int i = 0; i < row; i++)
		{
			vector<IntOrChar> temp;
			values2D.push_back(temp);
		}
	}
	SymbolTableItem(string name, SymbolTableItemType type, int paraNum) :
		name(name), domain("0"), kind(FUNCTION), type(type), parameterCount(paraNum) {}
	bool nameEqual(string name);
	bool hasRet();
	bool itemDuplicated(const SymbolTableItem& item);
	bool isItemInDomain(string name, string domain);
	string getAttr();
	bool isFunction(string name);
	SymbolTableItemType getType()
	{
		return type;
	}
	SymbolTableItemKind getKind()
	{
		return kind;
	}
	string getDomain() { return domain; }
	bool addValue(int value);
	bool addValue(char value);
	void nextRow();
	bool isFullyAssign();
	void setParaNum(int n);
	int getParaCount();
};

#endif // !_SYMBOL_TABLE_ITEM_H_