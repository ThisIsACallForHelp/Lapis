#pragma once
#include "ASTNode.h"
#include "ExpressionNode.h"
#include <memory>
class EnumMemberNode : public ASTNode {
	public:
		TokenModel identifier;
		std::unique_ptr<ExpressionNode> value;

		EnumMemberNode(TokenModel token, TokenModel ID, std::unique_ptr<ExpressionNode> Value) :
			ASTNode(token), identifier(ID), value(std::move(Value)) {
		};
};
