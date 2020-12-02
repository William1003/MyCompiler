#include"SymbolTableItem.h"
#include"constValue.h"
#include<algorithm>

extern const char* ItemType[TYPE_NUM];
extern const char* ItemKind[KIND_NUM];

bool SymbolTableItem::nameEqual(string name)
{
	if (this->name == name)
	{
		return true;
	}
	return false;
}

bool SymbolTableItem::itemDuplicated(const SymbolTableItem& item)
{
	string name1 = this->name;
	transform(name1.begin(), name1.end(), name1.begin(), ::tolower);
	string name2 = item.name;
	transform(name2.begin(), name2.end(), name2.begin(), ::tolower);
	if (name1 == name2 && this->domain == item.domain)
	{
		return true;
	}
	return false;
}

bool SymbolTableItem::hasRet()
{
	if (this->kind == FUNCTION)
	{
		return this->type > 0;
	}
	return false;
}

bool SymbolTableItem::addValue(int value)
{
	if (currentRow >= row || currentColumn >= column)
	{
		return false;
	}
	if (dimension == 1)
	{
		values1D.push_back(value);
		currentColumn++;
	}
	if (dimension == 2)
	{
		values2D[currentRow].push_back(value);
		currentColumn++;
	}
	return true;
}

bool SymbolTableItem::addValue(char value)
{
	if (currentRow >= row || currentColumn >= column)
	{
		return false;
	}
	if (dimension == 1)
	{
		values1D.push_back(value);
		currentColumn++;
	}
	if (dimension == 2)
	{
		values2D[currentRow].push_back(value);
		currentColumn++;
	}
	return true;
}

string SymbolTableItem::getAttr()
{
	return name + " " + domain + " " + ItemKind[kind] + " " + ItemType[type];
}

bool SymbolTableItem::isItemInDomain(string name, string domain)
{
	string lowcase = this->name;
	transform(lowcase.begin(), lowcase.end(), lowcase.begin(), ::tolower);
	transform(name.begin(), name.end(), name.begin(), ::tolower);
	return lowcase == name && this->domain == domain;
}

bool SymbolTableItem::isFunction(string name)
{
	string lowcase = this->name;
	transform(lowcase.begin(), lowcase.end(), lowcase.begin(), ::tolower);
	transform(name.begin(), name.end(), name.begin(), ::tolower);
	return lowcase == name && kind == FUNCTION;
}

int SymbolTableItem::getParaCount()
{
	return this->parameterCount;
}

void SymbolTableItem::setParaNum(int n)
{
	this->parameterCount = n;
}
void SymbolTableItem::nextRow()
{
	currentRow++;
	currentColumn = 0;
}

bool SymbolTableItem::isFullyAssign()
{
	// TODO:填满
	return currentRow + 1 == row && currentColumn == column;
}
