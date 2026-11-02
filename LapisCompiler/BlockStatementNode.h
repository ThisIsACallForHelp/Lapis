#pragma once
#include "ASTNode.h"
#include <memory>
#include <vector>
#include "StatementNode.h"
class BlockStatementNode : public StatementNode {
	public:
		std::vector<std::unique_ptr<StatementNode>> statements;
		BlockStatementNode(TokenModel token)
			: StatementNode(token){
		};
};
