#pragma once
#include "StatementNode.h"
#include <memory>
class DeferStatementNode : public StatementNode {
	public:
		std::unique_ptr<StatementNode> statement;

		DeferStatementNode(TokenModel token, std::unique_ptr<StatementNode> Statement) :
			StatementNode(token), statement(std::move(Statement)) {
		};
};
