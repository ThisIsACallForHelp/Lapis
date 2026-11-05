#pragma once
#include "StatementNode.h"
#include "FunctionDeclarationNode.h"
#include <memory>
#include "TokenModel.h"
#include <vector>
class InterfaceDeclarationNode : public StatementNode {
	public:
		std::vector<std::unique_ptr<FunctionDeclarationNode>> functions;
		std::string interfaceName;

		InterfaceDeclarationNode(TokenModel token,
			std::vector<std::unique_ptr<FunctionDeclarationNode>> funcs, 
			std::string name) :
			StatementNode(token), functions(std::move(funcs)), 
			interfaceName(name){
		};
};
