#ifndef _LEXICAL_ANALYSIS_H_
#define _LEXICAL_ANALYSIS_H_

#include<string>
#include"constValue.h"
#include"Output.h"
#include"ErrorHandler.h"

using namespace std;

class LexicalAnalysis
{
private:
	string fileContent;		//文件内容
	long fileLength;		//文件大小
	int index;				//文件索引
	int backupIndex;
	char c;					//当前读入的字符
	string token;			//当前读入的symbol
	string backupToken;
	enum SymbolCode symbolCode;			//当前读入的symbol的标号
	enum SymbolCode backupSymbolCode;
	int line;				//记录当前行号 	
	bool finish;			//是否读取结束
	bool autoComplete;
	int value;
	Output& output;
	enum SymbolCode reserver(string& token);			//查保留字
	void getChar();			//读下一个字符
	void retract();			//回退一个字符
	ErrorHandler& errorHandler;
	void skipRead(char target);

public:
	LexicalAnalysis(Output& output, ErrorHandler& errorHandler);
	void readFile(string filename);
	SymbolCode getSymbolCode();
	string getToken();
	int getLineCount();
	bool getNextSymbol();
	int getValue();
	bool isFinish();
	void setAutoComplete();
	int getIndex();
	void setIndex(int index);
	void backup();
	void restore();
	bool isCharacter();
	SymbolCode preRead();
};

#endif