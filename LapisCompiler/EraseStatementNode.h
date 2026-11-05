#pragma once
#include "StatementNode.h"
#include "ExpressionNode.h"
#include <memory>
class EraseStatementNode : public StatementNode {
	public:
		std::unique_ptr<ExpressionNode> statement;

		EraseStatementNode(TokenModel token, std::unique_ptr<ExpressionNode> Statement) :
			StatementNode(token), statement(std::move(Statement)) {
		};
};
