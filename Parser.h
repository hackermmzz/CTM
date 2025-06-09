#ifndef PARSER_H
#define PARSER_H
#include"Lexifier.h"
///////////////////////////////
struct StructInfo;
struct TypeSpecifier;
struct Declaration;
struct InitDeclarator;
struct Declarator;
struct Initializer;
///////////////////////////////
struct TypeSpecifier{
    bool _signed;
    int baseType;//mark down if it is a base type
    string typeName;
    StructInfo*si;
};
struct Declarator{
    int pointer;
    struct DirectDeclarator{
        string id;
        Declarator*dt;
        int arrayLen;
    }dd;
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

///////////////////////////////
struct Parser{
    int idx;
    vector<Token>&token;
    Parser(vector<Token>&tk);
    Token& next();
    Token& nextn(int n);
    Token& cur();
    void parse();
    TypeSpecifier*Parse_type_specifier();
    pair<string,StructInfo*> Parse_Struct();
    StructInfo* Parse_struct_declaration_list(string name);
    Declarator* Parse_declarator();
};
///////////////////////////////
bool CheckIsBaseType(const Token&tk);
///////////////////////////////
#endif