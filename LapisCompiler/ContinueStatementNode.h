#pragma once
#include "StatementNode.h"
class ContinueStatementNode : public StatementNode {
	public:
		ContinueStatementNode(TokenModel token) : StatementNode(token) {};
};
