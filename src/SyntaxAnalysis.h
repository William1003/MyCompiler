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
	bool declareHeader();		//声明头部
	bool parameterTable();		//参数表
	bool compoundStatement();	//复合语句
	bool statementQueue();		//语句列
	bool statement();			//语句
	bool loopStatement();		//循环语句
	bool ifStatement();			//条件语句
	bool useFunction();	//有返回值函数调用
	bool useFunctionWithoutRet();//无返回值函数调用
	bool assignStatement();		//赋值语句
	bool scanfStatement();		//读语句
	bool printfStatement();		//写语句
	bool switchStatement();		//情况语句
	bool caseTable();			//情况表
	bool caseStatement();		//情况子语句
	bool returnStatement();		//返回语句
	bool defaultStatement();	//缺省
	bool condition();			//条件
	bool expression();			//表达式
	bool relationOp();			//关系运算符
	bool term();				//项
	bool factor();				//因子
	bool character();			//字符
	bool step();				//步长
	bool myString();			//字符串
	bool mainFunction();		//主函数
	bool parameterValueTable();	//值参数表
};

#endif // !_SYNTAX_ANALYSIS_H_
