#ifndef PARSER_H
#define PARSER_H
#include"Lexifier.h"
///////////////////////////////
struct Parser{
    int idx;
    vector<Token>&token;
    Parser(vector<Token>&tk);
    Token& next();
    Token& nextn(int n);
    Token& cur();
    void parse();
};
///////////////////////////////
#endif