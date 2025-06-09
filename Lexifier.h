#ifndef LEXIFIER_H
#define LEXIFIER_H
#include"debug.cpp"
#include <fstream>
#include<vector>
#include<unordered_map>
///////////////////////////////////////////
enum{
    Inc,Dec,Add,Sub,Star,Div,Mod,Neg,Not,Xor,Band,Lpb,Rpb,Ul,Assign,Lsb,Rsb,Lcb,Rcb,Bor,
    Bslash,Sem,Dq,Sq,Lt,Comma,Gt,Dot,Le,Ge,Ne,Eq,Land,Lor,Adds,Subs,Muls,Divs,Mods,
    key_for,key_while,key_if,key_else,key_switch,key_case,key_default,key_continue,
    key_break,key_return,key_asm,key_void,key_byte,key_hword,key_word,key_unsigned,key_struct,
};
extern const  unordered_map<string,int>KeyWord;
///////////////////////////////////////////
struct Lexifier{
    vector<char> buf;
    int idx;
    Lexifier(string file);
    void parse();
    void skip();//跳过空白 # 等那些我不需要解析的字符
    char next();
    char nextn(int n);
};
///////////////////////////////////////////
#endif