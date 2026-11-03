#pragma once
#include "ExpressionNode.h"
#include "StatementNode.h"
#include "BlockStatementNode.h"
#include <memory>
class UntilLoopNode : public StatementNode {
public:
	std::unique_ptr<ExpressionNode> condition;
	std::unique_ptr<BlockStatementNode> innerBody;

	UntilLoopNode(TokenModel token, std::unique_ptr<ExpressionNode> conditionExp, std::unique_ptr<BlockStatementNode> blockState) :
		StatementNode(token), condition(std::move(conditionExp)), innerBody(std::move(blockState)) {
	};
};
