#pragma once
#include "ExpressionNode.h"
#include "StatementNode.h"
#include "BlockStatementNode.h"
class UntilLoopNode : public StatementNode {
public:
	ExpressionNode condition;
	BlockStatementNode innerBody;

	UntilLoopNode(TokenModel token, ExpressionNode conditionExp, BlockStatementNode blockState) :
		StatementNode(token), condition(conditionExp), innerBody(blockState) {
	};
};
