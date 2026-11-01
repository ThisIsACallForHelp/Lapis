#pragma once
#include "ASTNode.h"
class StatementNode : public ASTNode {
	public:
		StatementNode(TokenModel _token) : ASTNode(_token) {};
};
