#pragma once
#include "StatementNode.h"
#include "BlockStatementNode.h"
#include "VariableDeclarationNode.h"
#include <memory>
#include <vector>
#include <string>

class MacroDeclarationNode : public StatementNode {
    public:
        TokenModel name;
        bool isRadioactive;
        std::vector<std::unique_ptr<VariableDeclarationNode>> params;
        std::unique_ptr<BlockStatementNode> body;

        MacroDeclarationNode(TokenModel token, TokenModel macroName,
            std::vector<std::unique_ptr<VariableDeclarationNode>> parameters,
            std::unique_ptr<BlockStatementNode> macroBody, bool radioactive = false)
            : StatementNode(token), name(macroName), params(std::move(parameters)),
            body(std::move(macroBody)), isRadioactive(radioactive)
        {
        };
};
