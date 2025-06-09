#ifndef LEXIFIER_H
#define LEXIFIER_H
#include"debug.cpp"
#include <fstream>
#include<vector>
#include<unordered_map>
///////////////////////////////////////////
enum{
    Inc=1,Dec,Add,Sub,Star,Div,Mod,Bneg,Not,Xor,Band,Lpb,Rpb,Assign,Lsb,Rsb,Lcb,Rcb,Bor,
    Sem,Dq,Sq,Lt,Comma,Gt,Dot,Le,Ge,Ne,Eq,Land,Lor,Adds,Subs,Muls,Divs,Mods,Point,
    key_for,key_while,key_if,key_else,key_switch,key_case,key_default,key_continue,
    key_break,key_return,key_asm,key_void,key_byte,key_hword,key_word,key_unsigned,key_struct,
    //
    INT_Literal,STR_Literal,CHR_Literal,ID
};
///////////////////////////////////////////
struct Token{
    int type;
    int line,pos;
    string data;
    bool operator==(const Token&tk)const{
        return type==tk.type;
    }
    bool operator==(int tp)const{
        return type==tp;
    }
    bool operator!=(const Token&tk)const{
        return type!=tk.type;
    }
    bool operator!=(int tp)const{
        return type!=tp;
    }
};
///////////////////////////////////////////
struct Lexifier{
    vector<char> buf;
    vector<Token>token;
    int idx;
    int line,pos;
    Lexifier(string file);
    void parse();
    void skip();//跳过空白 # 等那些我不需要解析的字符
    char next();
    char nextn(int n);
    char cur();
    void debug();//

};
///////////////////////////////////////////
#endif