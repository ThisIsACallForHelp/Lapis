#pragma once
#include "StatementNode.h"
#include <memory>
class EraseStatementNode : public StatementNode {
	public:
		std::unique_ptr<StatementNode> statement;

		EraseStatementNode(TokenModel token, std::unique_ptr<StatementNode> Statement) :
			StatementNode(token), statement(std::move(Statement)) {
		};
};
