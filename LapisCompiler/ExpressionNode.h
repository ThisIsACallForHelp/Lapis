#pragma once
#include "ASTNode.h"
class ExpressionNode : public ASTNode
{
	public:
		ExpressionNode(TokenModel token) : ASTNode(token) {};
};

