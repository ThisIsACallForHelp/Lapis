#pragma once
#include "ExpressionNode.h"
class VariableAccessNode : public ExpressionNode {
	public:
		TokenModel identifier;

		VariableAccessNode(TokenModel token, TokenModel ID) :
			ExpressionNode(token), identifier(ID) {
		};
};
