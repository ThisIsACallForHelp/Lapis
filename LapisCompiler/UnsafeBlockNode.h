#pragma once
#include "BlockStatementNode.h"
#include "StatementNode.h"
class UnsafeBlockNode : public StatementNode {
	public:
		BlockStatementNode unsafeBody;

		UnsafeBlockNode(TokenModel token ,BlockStatementNode innerBody) :
			StatementNode(token), unsafeBody(innerBody) { };
};