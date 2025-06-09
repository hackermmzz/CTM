#include "Parser.h"

Parser::Parser(vector<Token> &tk):token(tk)
{
    idx=0;
    parse();
}

Token& Parser::next(){
    return nextn(1);
}

Token& Parser::nextn(int n){
    int t=idx+n;
    static Token empty{0,0,0};
    return t<token.size()?token[t]:empty;
}

Token& Parser::cur(){
    return nextn(0);
}

void Parser::parse(){
    
}