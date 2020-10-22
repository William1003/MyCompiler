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
	Var(string name, string domain, int value) :
		SymbolTableItem(name, domain, VAR, INT), value(IntOrChar(value)) {}
	Var(string name, string domain, char value) :
		SymbolTableItem(name, domain, VAR, CHAR), value(IntOrChar(value)) {}
};

class Array : public SymbolTableItem
{
private:
	int dimension;
	int row;
	int column;
	vector<vector<IntOrChar>> values;

public:
	Array(string name, string domain, int row, SymbolTableItemType type) : 
		SymbolTableItem(name, domain, ARRAY, type), dimension(1), row(row), column(1) {}
	void setColumn(int column)
	{
		this->column = column;
	}
};

class Function : public SymbolTableItem
{
public:
	Function(string name, SymbolTableItemType type) :
		SymbolTableItem(name, "0", FUNCTION, type) {}
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