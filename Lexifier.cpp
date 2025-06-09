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
    while(idx<buf.size()){
        skip();

        //
        
        //
        //
        //
    }
}

void Lexifier::skip()
{
    bool flag=1;
    while(idx<buf.size()&&flag){
        flag=0;
        char ch0=buf[idx],ch1=idx+1<buf.size()?buf[idx+1]:0;
        //处理换行和空格
        if(ch0=='\n'||ch0=='\r'||ch0==' ')++idx,flag=1;
        //处理 // 注释
        else if(ch0=='/'&&ch1=='/'){
            while(idx<buf.size()&&buf[idx]!='\n')++idx;
            flag=1;
        }
        //处理/* */注释
        else if(ch0=='/'&&ch1=='*'){
            idx+=3;
            while(idx<buf.size()&&(buf[idx]!='/'||buf[idx-1]!='*'))++idx;
            flag=1;
        }
        //处理预处理#号
        else if(ch0=='#'){
            while(idx<buf.size()&&buf[idx]!='\n')++idx;
            flag=1;
        }
    }
}

void Lexifier::next(){
    return nextn(1);
}
void Lexifier::nextn(int n){
    int idx=min(idx+n,int(buf.size()));
    
}