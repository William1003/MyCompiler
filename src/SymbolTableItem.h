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
	int intValue;
	char charValue;

public:
	IntOrChar() {}
	IntOrChar(int value) : intValue(value) {}
	IntOrChar(char value) : charValue(value) {}
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
	
public:
	SymbolTableItem(string name, string domain, SymbolTableItemKind kind, SymbolTableItemType type) : 
		name(name), domain(domain), kind(kind), type(type) {}
	bool nameEqual(string name);
	bool hasRet();
	bool itemDuplicated(const SymbolTableItem& item);
	bool isItemInDomain(string name, string domain);
	string getAttr();
	bool isFunction(string name);
	bool isParameter(string functionName);
	SymbolTableItemType getType()
	{
		return type;
	}
	SymbolTableItemKind getKind()
	{
		return kind;
	}
	virtual void doSomething();
};

class Const : public SymbolTableItem
{
private:
	IntOrChar value;

public:
	Const(string name, string domain, int value) :
		SymbolTableItem(name, domain, CONST, INT), value(IntOrChar(value)) {}
	Const(string name, string domain, char value) :
		SymbolTableItem(name, domain, CONST, CHAR), value(IntOrChar(value)) {}
};

class Var : public SymbolTableItem
{
private:
	IntOrChar value;

public:
	Var(string name, string domain, SymbolTableItemType type) :
		SymbolTableItem(name, domain, VAR, type) {}
	Var(string name, string domain, int value) :
		SymbolTableItem(name, domain, VAR, INT), value(IntOrChar(value)) {}
	Var(string name, string domain, char value) :
		SymbolTableItem(name, domain, VAR, CHAR), value(IntOrChar(value)) {}
	Var(string name, string domain, IntOrChar value, SymbolTableItemType type) :
		SymbolTableItem(name, domain, VAR, type), value(value) {}
};

class Array : public SymbolTableItem
{
private:
	int dimension;
	int row;
	int column;
	int currentRow = 0;
	int currentColumn = 0;
	vector<IntOrChar> values1D;
	vector<vector<IntOrChar>> values2D;

public:
	Array(string name, string domain, int column, SymbolTableItemType type) : 
		SymbolTableItem(name, domain, ARRAY, type), dimension(1), row(1), column(column) {}
	Array(string name, string domain, int row, int column, SymbolTableItemType type) :
		SymbolTableItem(name, domain, ARRAY, type), dimension(2), row(row), column(column) 
	{
		for (int i = 0; i < row; i++)
		{
			vector<IntOrChar> temp;
			values2D.push_back(temp);
		}
	}
	bool addValue(IntOrChar value);
	void nextRow() 
	{ 
		currentRow++; 
		currentColumn = 0;
	}
	bool isFullyAssign()
	{
		return currentRow == row && currentColumn == column;
	}
};

class Function : public SymbolTableItem
{
private:
	int parameterNumber = 0;
public:
	Function(string name, SymbolTableItemType type, int n) :
		SymbolTableItem(name, "0", FUNCTION, type), parameterNumber(n) {}
	int getParaCount() { return parameterNumber; }
};

class Parameter : public SymbolTableItem
{
private:
	IntOrChar value;

public:
	Parameter(string name, string domain, SymbolTableItemType type) :
		SymbolTableItem(name, domain, PARAMETER, type) {}
	void setValue(int value)
	{
		this->value.setValue(value);
	}
	void setValue(char value)
	{
		this->value.setValue(value);
	}
};


#endif // !_SYMBOL_TABLE_ITEM_H_