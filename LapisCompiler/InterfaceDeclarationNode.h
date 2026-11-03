#pragma once
#include "StatementNode.h"
#include "FunctionDeclarationNode.h"
#include <memory>
#include "TokenModel.h"
#include <vector>
class InterfaceDeclarationNode : public StatementNode {
	public:
		std::vector<std::unique_ptr<FunctionDeclarationNode>> functions;

		InterfaceDeclarationNode(TokenModel token);
};
