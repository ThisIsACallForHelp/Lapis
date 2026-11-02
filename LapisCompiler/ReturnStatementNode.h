#pragma once
#include "StatementNode.h"
#include "ExpressionNode.h"
#include <memory>
class ReturnStatementNode : public StatementNode {
<<<<<<< HEAD
	public:
		std::vector<std::unique_ptr<ExpressionNode>> returnedVals;
		ReturnStatementNode(TokenModel token, std::vector<std::unique_ptr<ExpressionNode>> returned) : StatementNode(token), returnedVals(returned) 
		{};
};
=======

public:
	std::vector<std::unique_ptr<ExpressionNode>> returnedVals;
	ReturnStatementNode(TokenModel token, std::vector<std::unique_ptr<ExpressionNode>> returned) : StatementNode(token), returnedVals(std::move(returned))
	{
	};
};
>>>>>>> 6e7aa9d (almost finished all of the nodes in the parser)
