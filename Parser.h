#ifndef PARSER_H
#define PARSER_H
#include"Lexifier.h"
#include<string>
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
///////////////////////////////
struct InitializerList{
    vector<Initializer*>init;
};
struct Initializer{
    AssignExp*exp;
    InitializerList*list;
};
struct DeclaratorType{
    int pointer;
    vector<int>array;
    DeclaratorType*tp;
};
struct TypeSpecifier{
    bool _signed;
    int baseType;//mark down if it is a base type
    string typeName;
    StructInfo*si;
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
    TypeSpecifier retType0;
    int pointer;
    string id;
    vector<Declaration>argument;
    ComposedStatment*body;
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
};
///////////////////////////////
bool CheckIsBaseType(const Token&tk);
///////////////////////////////
#endif