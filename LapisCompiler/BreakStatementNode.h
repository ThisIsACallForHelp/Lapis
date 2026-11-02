#pragma once
#include "StatementNode.h"
class BreakStatementNode : public StatementNode {
	public:
		BreakStatementNode(TokenModel token) : StatementNode(token) {};
};
