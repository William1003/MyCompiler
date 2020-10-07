#ifndef _SYNTAX_ANALYSIS_H_
#define _SYNTAX_ANALYSIS_H_

#include"LexicalAnalysis.h"
#include"ErrorHandler.h"

class SyntaxAnalysis
{
private:
	ErrorHandler& errorHandler;
	LexicalAnalysis& lexicalAnalysis;
	int value;
	bool hasNext;
	void getNext();
	SymbolCode symbolCode;
	SymbolCode symbolCodeBackup;
	SymbolCode getSymbolCode();
	Output& output;
	void backup();
	void retract();

public:
	SyntaxAnalysis(ErrorHandler& errorHandler, LexicalAnalysis& lexicalAnalysis, Output& output);
	void doLexicalAnalysis();
	void startSyntaxAnalysis();

	bool program();				//程序
	bool constDeclaration();	//常量说明
	bool constDefinition();		//常量定义
	bool varDeclaration(bool isGlobal);//变量说明
	bool varDefinition();		//变量定义
	bool varWithoutInit();		//变量定义无初始化
	bool varWithInit();			//变量定义及初始化
	bool functionWithRet();		//有返回值函数定义
	bool functionWithoutRet();	//无返回值函数定义
	bool integer();				//整数
	bool uinteger();			//无符号整数
	bool constant();			//常量
};

#endif // !_SYNTAX_ANALYSIS_H_
