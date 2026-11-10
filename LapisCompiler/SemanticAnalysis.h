#pragma once
#include "SymbolTable.h"
#include "Type.h"
#include "SymbolError.h"
#include <vector>
#include "AST.h"
class SemanticAnalysis
{
	SymbolTable* _currScope;
	std::vector<SymbolError> _errors;
	Type _lastEvaluatedType;

	public:
		SemanticAnalysis(SymbolTable* scope) :
			_currScope(scope), _errors({}), _lastEvaluatedType(Type()) {

		};
		void Analyze(ProgramNode* root);
		void ReportError(std::string message, TokenModel token, std::string hint);
		void VisitVariableDeclare(VariableDeclarationNode* node);
		void VisitFuncDeclare(FunctionDeclarationNode* node);
		void VisitClassNode(ClassDeclarationNode* node);
		void VisitStructNode(StructDeclarationNode* node);
		void VisitEnumDeclaration(EnumDeclarationNode* node);
		void VisitInterfaceNode(InterfaceDeclarationNode* node);
		void VisitBlockStatement(BlockStatementNode* node);
		void VisitIfNode(IfStatementNode* node);
		void VisitWhileLoop(WhileLoopNode* node);
		void VisitForLoop(ForLoopNode* node);
		void VisitMatchStatement(MatchStatementNode* node);
		void VisitReturnStatement(ReturnStatementNode* node);
		void VisitEraseStatement(EraseStatementNode* node);
		void VisitTryCatchStatement(TryCatchNode* node);
		void VisitImportNode(ImportNode* node);
		void VisitLinkDirNode(LinkDirectiveNode* node);
		void VisitBreakStatement(BreakStatementNode* node);
		void VisitContinueStatement(ContinueStatementNode* node);
		void VisitDeferStatement(DeferStatementNode* node);
		void VisitUnsafeBlock(UnsafeBlockNode* node);
		void VisitLiteral(LiteralNode* node);
		void VisitVariableAccess(VariableAccessNode* node);
		void VisitBinaryExpression(BinaryExpressionNode* node);
		void VisitUnaryExpression(UnaryExpressionNode* node);
		void VisitAssignment(AssignmentNode* node);
		void VisitFuncCall(FunctionCallNode* node);
		void VisitMemberAccess(MemberAccessNode* node);
		void VisitIndexAccess(IndexAccessNode* node);
		void VisitCastExpression(CastExpressionNode* node);
		void VisitNewExpression(NewExpressionNode* node);
		void VisitAddressOf(AddressOfNode* node);
		void VisitAtomExpression(AtomExpressionNode* node);
		void VisitFusionExpression(FusionExpressionNode* node);
		void VisitReplaceExpression(ReplaceExpressionNode* node);
};

