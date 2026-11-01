#pragma once
#include "StatementNode.h"
#include "ExpressionNode.h"
#include "BlockStatementNode.h"
class WhileLoopNode : public StatementNode {
	public:
		std::unique_ptr<ExpressionNode> condition;
		std::unique_ptr<BlockStatementNode> innerBody;

		WhileLoopNode(TokenModel token, std::unique_ptr<ExpressionNode> conditionExp, std::unique_ptr<BlockStatementNode> blockState) :
			StatementNode(token), condition(std::move(conditionExp)), innerBody(std::move(blockState)) {
		};
};
