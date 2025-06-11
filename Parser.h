#ifndef PARSER_H
#define PARSER_H
#include"Lexifier.h"
#include<string>
#include<map>
#include<set>
#include<string.h>
///////////////////////////////
struct StructInfo;
struct TypeSpecifier;
struct Declaration;
struct InitDeclarator;
struct Declarator;
struct Initializer;
struct Program;
struct Function;
struct DeclaratorType;
struct ComposedStatment;
struct AssignExp;
struct InitializerList;
struct LogicalOrExp;
struct LogicalAndExp;
struct InclusiveOrExp;
struct ExclusiveOrExp;
struct AndExp;
struct EqualityExp;
struct RelationalExp;
struct ShiftExp;
struct AdditiveExp;
struct MultiplicativeExp;
struct CastExp;
struct UnaryExp;
struct PostfixExp;
struct PrimaryExp;
struct TypeMark;
struct Expression;
struct ForStatement;
struct SwitchStatment;
struct WhileStatement;
struct DoWhileStatment;
struct IfElseStatment;
struct AsmStatement;
struct ReturnStatement;
struct Statement;
///////////////////////////////
struct TypeSpecifier{
    bool _signed;
    int baseType;//mark down if it is a base type
    string typeName;
    StructInfo*si;
    TypeSpecifier();
};

struct TypeMark{
    TypeSpecifier tp;
    int pointer;
    TypeMark();
};
struct Expression{
    vector<AssignExp>exp;
};
struct PrimaryExp{
    Expression*exp;
    int tp;
    string id_or_constant;
    PrimaryExp();
};
struct PostfixExp{
    PrimaryExp*primaryExp;
    //
    enum Type{Index,Funcall,Dot,Pointer,Add,Sub};
    vector<pair<Type,void*>>op;
    //
    PostfixExp();
};
struct UnaryExp{
    PostfixExp*postExp;
    //++ --
    int tp0;
    UnaryExp*unaryExp;
    //& * + - ! ~
    int tp1;
    CastExp*castExp;
    //
    UnaryExp();
};
struct CastExp{
    TypeMark type;
    CastExp*castExp;
    UnaryExp*exp;
    CastExp();
};
struct MultiplicativeExp{
    CastExp*exp0;
    CastExp*exp1;
    int tp;
    MultiplicativeExp();
};
struct AdditiveExp{
    MultiplicativeExp*exp0;
    MultiplicativeExp*exp1;
    int tp;
    AdditiveExp();
};
struct ShiftExp{
    AdditiveExp*exp0;
    AdditiveExp*exp1;
    int tp;
    ShiftExp();
};
struct RelationalExp
{
   ShiftExp*exp0;
   ShiftExp*exp1;
   int tp;
   RelationalExp();
};

struct EqualityExp{
    RelationalExp*exp0;
    RelationalExp*exp1;
    int tp;
    EqualityExp();
};
struct AndExp{
    EqualityExp*exp0;
    EqualityExp*exp1;
    AndExp();
};
struct ExclusiveOrExp{
    AndExp*exp0;
    AndExp*exp1;
    ExclusiveOrExp();
};
struct InclusiveOrExp{
    ExclusiveOrExp*exp0;
    ExclusiveOrExp*exp1;
    InclusiveOrExp();
};
struct LogicalAndExp{
    InclusiveOrExp*exp0;
    InclusiveOrExp*exp1;
    LogicalAndExp();
};
struct LogicalOrExp{
    LogicalAndExp*exp0;
    LogicalAndExp*exp1;
    LogicalOrExp();
};
struct AssignExp{
    LogicalOrExp*exp0;
    int op;
    AssignExp*exp1;
    AssignExp();
};

struct InitializerList{
    vector<Initializer*>init;
};
struct Initializer{
    AssignExp*exp;
    InitializerList*list;
    Initializer();
};
struct DeclaratorType{
    int pointer;
    vector<int>array;
    DeclaratorType*tp;
    DeclaratorType();
};

struct Declarator{
    //对于一个声明符,他要么是指针，要么是数组，要么什么也不是
    string id;
    DeclaratorType tp;
};
struct Declaration{
    TypeSpecifier ts;
    vector<InitDeclarator> dl;
};
struct InitDeclarator{
    Declarator dt;
    Initializer*init;
};

struct StructInfo
{
    string id;
    vector<Declaration>member;
};
struct Function{
    TypeMark retType;
    string id;
    vector<Declaration>argument;
    ComposedStatment*body;
};
struct Statement{
    enum{Expression,Composed,For,While,DoWhile,IfElse,Switch,Continue,Break,Return,Asm};
    int type;
    void*statement;
    Statement();
    Statement(int tp,void*stm);
};
struct ComposedStatment{
    //declaration为0,statement为1
    vector<pair<int,void*>>p;
};
struct ForStatement{
    Expression*exp0;
    Declaration*dec0;
    //
    Expression*exp1;
    //
    Expression*exp2;
    //
    Statement*stm;
    ForStatement();
};
struct SwitchStatment{
    Expression*exp;
    vector<tuple<bool,int,vector<Statement*>>>body;
    SwitchStatment();
};
struct WhileStatement{
    Expression*exp;
    Statement*stm;
    WhileStatement();
};
struct DoWhileStatment{
    Statement*body;
    Expression*exp;
    DoWhileStatment();
};
struct IfElseStatment{
    Expression*exp;
    Statement*stm0,*stm1;
    IfElseStatment();
};
struct AsmStatement{
    vector<string>ins;
};
struct ReturnStatement{
    Expression*exp;
    ReturnStatement();
};
struct Program{
    //declaration为0,function为1
    vector<pair<int,void*>>p;
};
///////////////////////////////
struct Parser{
    int idx;
    vector<Token>&token;
    Program program;
    Parser(vector<Token>&tk);
    Token& next();
    Token& nextn(int n);
    Token& cur();
    void parse();
    Function*Parse_function(TypeSpecifier*ts);
    Declaration*Parse_declaration(TypeSpecifier*ts);
    TypeSpecifier*Parse_type_specifier();
    pair<string,StructInfo*> Parse_Struct();
    StructInfo* Parse_struct_declaration_list(string name);
    Declarator* Parse_declarator();
    InitDeclarator*Parse_initDeclarator();
    Initializer*Parse_initializer();
    AssignExp* Parse_assignment_expression();
    InitializerList*Parse_initializerList();
    ComposedStatment* Parse_composed_statement();
    LogicalOrExp* Parse_logical_or_expression();
    LogicalAndExp*Parse_logical_and_expression();
    InclusiveOrExp*Parse_inclusive_or_expression();
    ExclusiveOrExp*Parse_exclusive_or_expression();
    AndExp*Parse_and_expression();
    EqualityExp*Parse_equality_expression();
    RelationalExp*Parse_relational_expression();
    ShiftExp*Parse_shift_expression();
    AdditiveExp* Parse_additive_expression();
    MultiplicativeExp*Parse_multiplicative_expression();
    CastExp*Parse_cast_expression();
    UnaryExp*Parse_unary_expression();
    PostfixExp*Parse_postfix_expression();
    PrimaryExp*Parse_primary_expression();
    Expression*Parse_expression();
    TypeMark ParseTypeMark();
    IfElseStatment*Parse_IfElseStatement();
    SwitchStatment*Parse_SwitchStatement();
    WhileStatement*Parse_WhileStatement();
    ForStatement*Parse_ForStatement();
    DoWhileStatment*Parse_DoWhileStatement();
    ReturnStatement*Parse_ReturnStatement();
    AsmStatement*Parse_AsmStatement();
    Statement*Parse_Statement();
    Expression*Parse_ExpressionStatement();
};
///////////////////////////////
bool CheckIsBaseType(const Token&tk);
int CharToInt(const string&s);
bool CheckIsTypeSpecifier(const Token&tk);
///////////////////////////////
#endif