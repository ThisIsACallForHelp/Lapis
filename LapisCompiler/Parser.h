#pragma once
#include <vector>
#include "ExpressionNode.h"
#include "StatementNode.h"
#include "ProgramNode.h"
#include "AST.h"

class Parser
{
	std::vector<TokenModel> _tokens;
	size_t _position;
	std::string _fileName;
	TokenModel Current() const;
	TokenModel Advance();
	TokenModel Peek(int offset = 1) const;
	bool isOutOfBoundary(int offset = 1) const;
	TokenModel Match(TokenModel token) const;
	void MatchThisByTypeAndAdvance(Token::Tokens expected);
	//node parsers
	Token::Tokens MatchByType(Token::Tokens expected, Token::Tokens existing);
	std::unique_ptr<AddressOfNode> ParseAddressOfNode();
	std::unique_ptr<AssignmentNode> ParseAssignmentNode();
	std::unique_ptr<AtomExpressionNode> ParseAtomExpression();
	std::unique_ptr<BinaryExpressionNode> ParseBinaryExpression();
	std::unique_ptr<ExpressionNode> ParseLogicalOR();
	std::unique_ptr<ExpressionNode> ParseLogicalAND();
	std::unique_ptr<ExpressionNode> ParseBitwiseOR();
	std::unique_ptr<ExpressionNode> ParseBitwiseXOR();
	std::unique_ptr<ExpressionNode> ParseBitwiseAND();
	std::unique_ptr<ExpressionNode> ParseEquality();
	std::unique_ptr<ExpressionNode> ParseComparison();
	std::unique_ptr<ExpressionNode> ParseBitwiseShift();
	std::unique_ptr<ExpressionNode> ParseAdditive();
	std::unique_ptr<ExpressionNode> ParseMultiplicative();
	std::unique_ptr<ExpressionNode> ParseUnary();
	std::unique_ptr<ExpressionNode> ParsePrimary();
	std::unique_ptr<BlockStatementNode> ParseBlockStatement();
	std::unique_ptr<BreakStatementNode> ParseBreakStatement();
	std::unique_ptr<CastExpressionNode> ParseCastExpression();
	std::unique_ptr<ClassDeclarationNode> ParseClassDeclaration();
	std::unique_ptr<ContinueStatementNode> ParseContinueStatement();
	std::unique_ptr<DeferStatementNode> ParseDeferStatement();
	std::unique_ptr<EnumDeclarationNode> ParseEnumDeclaration();
	std::unique_ptr<EnumMemberNode> ParseEnumMember();
	std::unique_ptr<EraseStatementNode> ParseEraseStatement();
	std::unique_ptr<ExpressionNode> ParseExpressionNode();
	std::unique_ptr<ForLoopNode> ParseForLoop();
	std::unique_ptr<FunctionCallNode> ParseFunctionCall();
	std::unique_ptr<FunctionDeclarationNode> ParseFuncDeclaration();
	std::unique_ptr<FusionExpressionNode> ParseFusionExpression();
	std::unique_ptr<IfStatementNode> ParseIfStatement();
	std::unique_ptr<ImportNode> ParseImportNode();
	std::unique_ptr<IndexAccessNode> ParseIndexAccessNode();
	std::unique_ptr<InterfaceDeclarationNode> ParseInterfaceDeclaration();
	std::unique_ptr<LinkDirectiveNode> ParseLinkDirective();
	std::unique_ptr<LiteralNode> ParseLiteralNode();
	std::unique_ptr<MacroDeclarationNode> ParseMacroDeclaration();
	std::unique_ptr<MatchCaseNode> ParseMatchCaseNode();
	std::unique_ptr<MatchStatementNode> ParseMatchStateNode();
	std::unique_ptr<MemberAccessNode> ParseMemberAccNode();
	std::unique_ptr<NewExpressionNode> ParseNewExpressionNode();
	std::unique_ptr<ProgramNode> ParseProgramNode();
	std::unique_ptr<ReadFromNode> ParseReadFromNode();
	std::unique_ptr<ReplaceExpressionNode> ParseReplaceNode();
	std::unique_ptr<ReturnStatementNode> ParseReturnNode();
	std::unique_ptr<StatementNode> ParseStatementNode();
	std::unique_ptr<StructDeclarationNode> ParseStructDeclaration();
	std::unique_ptr<TryCatchNode> ParseTryCatchNode();
	std::unique_ptr<UnaryExpressionNode> ParseUnaryExpression();
	std::unique_ptr<UnsafeBlockNode> ParseUnsafeBlock();
	std::unique_ptr<UntilLoopNode> ParseUntilLoop();
	std::unique_ptr<VariableDeclarationNode> ParseVarDeclaration();
	std::unique_ptr<VariableAccessNode> ParseVarAccess();
	std::unique_ptr<WhileLoopNode> ParseWhileLoop();
	std::unique_ptr<WriteToNode> ParseWriteToNode();
	public:
		Parser(std::vector<TokenModel> tokens, std::string file = "main.lps") :
			_tokens(std::move(tokens)), _position(0), _fileName(file) {
		};
		
		std::unique_ptr<ProgramNode> Parse();
};

