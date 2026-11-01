#pragma once
#include "TokenModel.h"
class ASTNode
{
	TokenModel _token;
	public:
		ASTNode(TokenModel input) : _token(input) {}
		inline TokenModel GetToken() const{ return _token;  }
		inline int GetLine() const { return _token.GetLineNumber(); }
		inline int GetTokenType() const { return _token.GetTokenType(); }
		inline int GetCharPosition() const { return _token.GetCharPosition(); }
		inline std::string GetTokenValue() const { return _token.GetTokenValue(); }
};

