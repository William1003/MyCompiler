#ifndef _CHAR_JUDGE_H_
#define _CHAR_JUDGE_H_

#define isSpace(c) (c == ' ')
#define isNewline(c) (c == '\n')
#define isTab(c) (c == '\t')
#define isLetter(c) ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_') 
#define isDigit(c) (c >= '0' && c <= '9')
#define isColon(c) (c == ':')
#define isComma(c) (c == ',')
#define isSemi(c) (c == ';')
#define isEqu(c) (c == '=')
#define isPlus(c) (c == '+')
#define isMinus(c) (c == '-')
#define isDivi(c) (c == '/')
#define isStar(c) (c == '*')
#define isLpar(c) (c == '(')
#define isRpar(c) (c == ')')
#define isLbrack(c) (c == '[')
#define isRbrack(c) (c == ']')
#define isLbrace(c) (c == '{')
#define isRbrace(c) (c == '}')
#define isExclamation(c) (c == '!')
#define isLt(c) (c == '<')
#define isGt(c) (c == '>')
#define isSingleq(c) (c == '\'')
#define isDoubleq(c) (c == '"')

#endif // !CHARJUDGE_H
