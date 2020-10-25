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

bool Array::addValue(IntOrChar value)
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
	return this->name == name && this->domain == domain;
}

bool SymbolTableItem::isFunction(string name)
{
	return this->name == name && kind == FUNCTION;
}

void SymbolTableItem::doSomething()
{
}

bool SymbolTableItem::isParameter(string functionName)
{
	return kind == PARAMETER && domain == functionName;
}