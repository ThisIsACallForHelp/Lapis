#pragma once
#include "StatementNode.h"
#include "ExpressionNode.h"
#include "FunctionDeclarationNode.h"
#include "VariableDeclarationNode.h"
#include <memory>
#include <vector>
class ClassDeclarationNode : public StatementNode {
	public:
        std::string className;
		Token::Tokens classType;
		Token::Tokens accessModifier;
		std::vector<std::unique_ptr<StatementNode>> inheritsFrom;
		std::vector<std::unique_ptr<VariableDeclarationNode>> fields;
		std::vector<std::unique_ptr<FunctionDeclarationNode>> innerFunctions;

        ClassDeclarationNode(TokenModel token, std::string name,
            std::vector<std::unique_ptr<StatementNode>> parents,
            std::vector<std::unique_ptr<VariableDeclarationNode>> properties,
            std::vector<std::unique_ptr<FunctionDeclarationNode>> internalFuncs,
            Token::Tokens Type = Token::noneToken,
            Token::Tokens accModifier = Token::noneToken) :
            StatementNode(token), className(name) , classType(Type), accessModifier(accModifier), inheritsFrom(std::move(parents)),
            fields(std::move(properties)), innerFunctions(std::move(internalFuncs)) {
        };
};
