#pragma once
#include "Token.h"
#include <string>
class TokenModel
{
	int _lineNumber;
	int _charPosition;
	Token::Tokens _token;
	std::string _tokenValue;
	public:
		TokenModel(Token::Tokens token, std::string value,int  line, int position) :
		_token(token), _tokenValue(value), _lineNumber(line), _charPosition(position) 
		{}
		inline int GetLineNumber() const { return _lineNumber; }
		inline int GetCharPosition() const { return _charPosition; }
		inline Token::Tokens GetTokenType() const { return _token; }
		inline std::string GetTokenValue() const { return _tokenValue; }
};

