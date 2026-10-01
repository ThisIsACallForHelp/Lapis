#pragma once
#include "Token.h"
#include <vector>
#include <string>

class Lexer
{
	std::string _codeString;
	int _position;
public:
	Lexer(std::string code) : _codeString(code), _position(0) {}
	char PeekNextChar();
	Token MoveIndex(int offset);
	Token TokenizeText();
};

