#pragma once
#include "StatementNode.h"
#include "ExpressionNode.h"
#include "BlockStatementNode.h"
#include <memory>
class IfStatement : public StatementNode {
	public:
		ExpressionNode condition;
		BlockStatementNode innerBody;
		std::unique_ptr<IfStatement> elseBlock;

		IfStatement(TokenModel token, ExpressionNode Condition, BlockStatementNode funcBody,
			std::unique_ptr<IfStatement> anotherEntry) : StatementNode(token), condition(Condition), innerBody(funcBody),
			elseBlock(std::move(anotherEntry)) {
		};
};
