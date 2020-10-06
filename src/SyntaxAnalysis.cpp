#include"SyntaxAnalysis.h"

SyntaxAnalysis::SyntaxAnalysis(LexicalAnalysis& lexicalAnalysis):lexicalAnalysis(lexicalAnalysis)
{

}

void SyntaxAnalysis::doLexicalAnalysis()
{
	while (lexicalAnalysis.getNextSymbol());
}