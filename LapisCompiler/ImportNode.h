#pragma once
#include "ASTNode.h"
class ImportNode : public ASTNode {
public:
	bool isLazy;
	std::string specifiedFile;
	std::string absoluteFilePath;
	ImportNode(TokenModel token, bool lazy, std::string linkedFile) :
		ASTNode(token), isLazy(lazy), specifiedFile(linkedFile) {

	};
};
