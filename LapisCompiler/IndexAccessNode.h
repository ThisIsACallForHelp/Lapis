#pragma once
#include <memory>
#include "ExpressionNode.h"
class IndexAccessNode : public ExpressionNode {
	public:
		std::unique_ptr<ExpressionNode> value;
		std::unique_ptr<ExpressionNode> index;
		std::unique_ptr<ExpressionNode> slice;
		IndexAccessNode(TokenModel token, std::unique_ptr<ExpressionNode> val,
			std::unique_ptr<ExpressionNode> Index, std::unique_ptr<ExpressionNode> Slice = nullptr) : ExpressionNode(token), value(std::move(val)),
			index(std::move(Index)), slice(std::move(Slice)){ }

};
