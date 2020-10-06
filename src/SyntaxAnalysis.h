#ifndef _SYNTAX_ANALYSIS_H_
#define _SYNTAX_ANALYSIS_H_

#include"LexicalAnalysis.h"

class SyntaxAnalysis
{
private:
	LexicalAnalysis& lexicalAnalysis;
public:
	SyntaxAnalysis(LexicalAnalysis& lexicalAnalysis);
	void doLexicalAnalysis();
};

#endif // !_SYNTAX_ANALYSIS_H_
