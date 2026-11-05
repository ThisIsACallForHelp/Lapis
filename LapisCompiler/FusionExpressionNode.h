#pragma once
#include "ExpressionNode.h"
#include <memory>
class FusionExpressionNode : public ExpressionNode {
	public:
		std::string targetType;
		std::unique_ptr<ExpressionNode> bytes;

		FusionExpressionNode(TokenModel token, std::string type,
			std::unique_ptr<ExpressionNode> Bytes) : ExpressionNode(token),
			targetType(type), bytes(std::move(Bytes)) {
		};
};
