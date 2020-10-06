#ifndef _LEXICAL_ANALYSIS_H_
#define _LEXICAL_ANALYSIS_H_

#include<string>
#include"constValue.h"
#include"Output.h"

using namespace std;

class LexicalAnalysis
{
private:
	string fileContent;		//文件内容
	long fileLength;		//文件大小
	int index;				//文件索引
	char c;					//当前读入的字符
	string token;			//当前读入的symbol
	enum SymbolCode symbolCode;			//当前读入的symbol的标号
	int line;				//记录当前行号 	
	bool finish;			//是否读取结束
	Output& output;
	SymbolCode reserver();			//查保留字
	void getChar();			//读下一个字符
	void retract();			//回退一个字符

public:
	LexicalAnalysis(Output& output);
	void readFile(string filename);
	int getSymbolCode();
	string getToken();
	int getLine();
	bool getNextSymbol();
};

#endif