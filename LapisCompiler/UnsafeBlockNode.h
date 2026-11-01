#pragma once
#include "BlockStatementNode.h"
#include "StatementNode.h"
#include <memory>
class UnsafeBlockNode : public StatementNode {
	public:
		std::unique_ptr<BlockStatementNode> unsafeBody;

		UnsafeBlockNode(TokenModel token , std::unique_ptr<BlockStatementNode> innerBody) :
			StatementNode(token), unsafeBody(std::move(innerBody)) { };
};