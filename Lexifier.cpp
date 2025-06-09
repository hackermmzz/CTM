#include "Lexifier.h"
///////////////////////////////////////////////
/*
key_for,Key_While,key_if,key_else,key_switch,key_case,key_default,key_continue,
key_break,key_return,key_asm,key_void,key_byte,key_hword,key_word,key_unsigned,key_struct,
*/
const  unordered_map<string,int>KeyWord={
    {"for",key_for},{"while",key_while},{"if",key_if},{"else",key_else},{"switch",key_switch},{"case",key_case},
    {"default",key_default},{"continue",key_continue},{"break",key_break},{"return",key_return},{"asm",key_asm},
    {"void",key_void},{"byte",key_byte},{"hword",key_hword},{"word",key_word},{"unsigned",key_unsigned},{"struct",key_struct}

};
///////////////////////////////////////////////
Lexifier::Lexifier(string file)
{
    //////////////////////////
    ifstream f(file,ios::binary);
    f.seekg(0,ios::end);
    buf.resize(f.tellg());
    f.seekg(0,ios::beg);
    f.read((char*)buf.data(),buf.size());
    string ans{buf.begin(),buf.end()};
    parse();
}

void Lexifier::parse()
{
    
}
