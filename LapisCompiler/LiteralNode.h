#pragma once
#include "ExpressionNode.h"
class LiteralNode : public ExpressionNode {
	public:
		TokenModel value;
		std::string type;

		LiteralNode(TokenModel token, TokenModel Val, std::string Type) :
			ExpressionNode(token), value(Val), type(Type) {
		};
};
