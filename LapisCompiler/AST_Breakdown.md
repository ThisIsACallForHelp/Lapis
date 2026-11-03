our abstract syntax tree will be broken down to 4 categories:

- Root & Structure
- Declarations
- Statements
- Expressions

now, lets determine what will be included:

Roots & Structures:
- ProgramNode => the list of all the top-level statements/declarations
- BlockStatementNode => any code inside of { ... }
- LinkDirectiveNode => for linking libraries
- ImportNode => for importing libraries

Declarations:
- VaraibleDeclarationNode => for declaring variables with modifiers 
- FunctionDeclarationNode => for declaring functions with modifiers
- SkeletonFunctionNode => for skeleton functions
- MacroDeclarationNode => for macros
- ClassDeclarationNode => for declaring macros
- StructDeclarationNode => for declaring structs
- EnumDeclarationNode => for declaring Enums
- InterfaceDeclarationNode => for declaring interfaces

Statements:
- IfStatement => condition, then block and optional else
- WhileLoopNode => while(condition) { ... }
- UntilLoopNode => do { ... } until(condition)
- ForLoopNode => for(let i = value; i < EndValue; i++)
- MatchStatementNode => hold the target expression and a list of MatchCaseNodes
- MatchCaseNode => a pattern to match
- ReturnStatementNode => for returned values
- BreakStatementNode => loop control 
- ContinueStatementNode => loop control
- DeferStatementNode => guarantees cleanup on scope exit
- EraseStatementNode => for deallocating 
- TryCatchNode => safer code
- UnsafeBlockNode => unsafe code 

Expressions:
- BinaryExpressionNode => Two operands and an operator 
- UnaryExpressionNode => single operand 
- BorrowExpressionNode => for reference borrowing
- AssignmentExpressionNode => assignments 
- FunctionCallNode => calling a function 
- ReplaceExpressionNode => non-destructive mutation 
- PartialExpressionNode => for compile time slicing
- AtomExpressionNode => for extracting bytes and opcodes
- FusionExpressionNode => for assembling the bytes together
- AddressOfNode => fetching the address of the variable
- ReadFromNode => to read from a specific address
- WriteToNode => to write something into a specific address
- BitIndexExpression => bit array access and slicing
- CastExpressionNode => for casting 
- CrashExpressionNode => for tier 2 program termination 
- NuclearExpressionNode => for tier 3 program termination 
