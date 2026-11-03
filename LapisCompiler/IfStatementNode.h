#pragma once
#include "StatementNode.h"
#include "ExpressionNode.h"
#include "BlockStatementNode.h"
#include <memory>
class IfStatementNode : public StatementNode {
	public:
		std::unique_ptr<ExpressionNode> condition;
		std::unique_ptr<BlockStatementNode> innerBody;
		std::unique_ptr<IfStatementNode> elseBlock;

		IfStatementNode(TokenModel token, std::unique_ptr<ExpressionNode> Condition, std::unique_ptr<BlockStatementNode> funcBody,
			std::unique_ptr<IfStatementNode> anotherEntry) : StatementNode(token), condition(std::move(Condition)), innerBody(std::move(funcBody)),
			elseBlock(std::move(anotherEntry)) {
		};
};
