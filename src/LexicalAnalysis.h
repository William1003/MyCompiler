#ifndef _LEXICAL_ANALYSIS_H_
#define _LEXICAL_ANALYSIS_H_

#include<string>
#include"constValue.h"
#include"Output.h"

using namespace std;

class LexicalAnalysis
{
private:
	string fileContent;		//�ļ�����
	long fileLength;		//�ļ���С
	int index;				//�ļ�����
	char c;					//��ǰ������ַ�
	string token;			//��ǰ�����symbol
	enum SymbolCode symbolCode;			//��ǰ�����symbol�ı��
	int line;				//��¼��ǰ�к� 	
	bool finish;			//�Ƿ��ȡ����
	Output& output;
	SymbolCode reserver();			//�鱣����
	void getChar();			//����һ���ַ�
	void retract();			//����һ���ַ�

public:
	LexicalAnalysis(Output& output);
	void readFile(string filename);
	int getSymbolCode();
	string getToken();
	int getLine();
	bool getNextSymbol();
};

#endif