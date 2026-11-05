#pragma once
#include <vector>
#include "ExpressionNode.h"
#include "StatementNode.h"
#include "ProgramNode.h"

class Parser
{
	std::vector<TokenModel> _tokens;
	size_t _position;
	std::string _fileName;

	TokenModel Current() const;
	TokenModel Advance() const;
	TokenModel Peek(int offset = 1) const;
	bool isOutOfBoundary() const;
	bool Match(TokenModel token) const;
	TokenModel Expect(Token::Tokens token, std::string& errorMessage);
	public:
		Parser(std::vector<TokenModel> tokens, std::string file = "main.lps") :
			_tokens(std::move(tokens)), _position(0), _fileName(file) {
		};
		std::unique_ptr<ProgramNode> Parse();	
};

