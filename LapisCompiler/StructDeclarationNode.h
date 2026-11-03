#pragma once
#include "StatementNode.h"
#include "FunctionDeclarationNode.h"
#include "VariableDeclarationNode.h"
#include <memory>
class StructDeclarationNode : public StatementNode {
	public:
		std::string structName;
		std::vector<std::unique_ptr<FunctionDeclarationNode>> innerFunctions;
		std::vector<std::unique_ptr<VariableDeclarationNode>> variables;

		StructDeclarationNode(TokenModel token, std::string name, std::vector<std::unique_ptr<FunctionDeclarationNode>> funcs,
			std::vector<std::unique_ptr<VariableDeclarationNode>> vars) :
			StatementNode(token), structName(name), innerFunctions(std::move(funcs)), variables(std::move(vars)) {
		};
};
