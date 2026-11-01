#pragma once
#include "ASTNode.h"
#include <vector>
#include <memory>
#include "StatementNode.h"
class ProgramNode : public ASTNode {
	public:
		std::string sourceFile;
		std::vector<std::unique_ptr<StatementNode>> statements;
		ProgramNode(TokenModel token, std::string fileName)
			: ASTNode(token), sourceFile(fileName) {
		};
};
