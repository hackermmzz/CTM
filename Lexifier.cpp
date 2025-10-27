#include "Lexifier.h"
///////////////////////////////////////////////
/*
key_for,Key_While,key_if,key_else,key_switch,key_case,key_default,key_continue,
key_break,key_return,key_asm,key_void,key_byte,key_hword,key_word,key_unsigned,key_struct,
*/
unordered_map<string,int>KeyWord={
    {"for",key_for},{"while",key_while},{"if",key_if},{"else",key_else},{"switch",key_switch},{"case",key_case},{"do",key_do},
    {"default",key_default},{"continue",key_continue},{"break",key_break},{"return",key_return},{"asm",key_asm},
    {"void",key_void},{"byte",key_byte},{"hword",key_hword},{"word",key_word},{"unsigned",key_unsigned},{"struct",key_struct},

};
///////////////////////////////////////////////
Lexifier::Lexifier(string file)
{
    idx=0;
    pos=1,line=1;
    //////////////////////////
    ifstream f(file,ios::binary);
    f.seekg(0,ios::end);
    buf.resize(f.tellg());
    f.seekg(0,ios::beg);
    f.read((char*)buf.data(),buf.size());
    string ans{buf.begin(),buf.end()};
    parse();
    //调试
    debug();

}

void Lexifier::parse()
{
    
    /*
    key_for,key_do,key_while,key_if,key_else,key_switch,key_case,key_default,key_continue,
    key_break,key_return,key_asm,key_void,key_byte,key_hword,key_word,key_unsigned,key_struct
    */
    while(1){
        skip();
        if(idx>=buf.size())break;
        char ch=buf[idx];
        //+ - * / % ++ -- += -= *= /= %=
        if(ch=='+'){
            switch(next()){
                case '+':
                    token.push_back({Inc,line,pos,"++"});
                    pos+=2,idx+=2;
                    break;
                case '=':
                    token.push_back({Adds,line,pos,"+="});
                    pos+=2,idx+=2;
                    break;
                default:
                    token.push_back({Add,line,pos,"+"});
                    ++pos;++idx;
            }
        }
        else if(ch=='-'){
            switch(next()){
                case '-':
                    token.push_back({Dec,line,pos,"--"});
                    pos+=2;idx+=2;
                    break;
                case '=':
                    token.push_back({Subs,line,pos,"-="});
                    pos+=2;idx+=2;
                    break;
                case '>':
                    token.push_back({Point,line,pos,"->"});
                    pos+=2;idx+=2;
                    break;
                default:
                    token.push_back({Sub,line,pos,"-"});
                    ++pos;++idx;
            }
        }
        else if(ch=='*'){
            switch(next()){
                case '=':
                    token.push_back({Muls,line,pos,"*="});
                    pos+=2;idx+=2;
                    break;
                default:
                    token.push_back({Star,line,pos,"*"});
                    ++pos;++idx;
            }
        }
        else if(ch=='/'){
            switch(next()){
                case '=':
                    token.push_back({Divs,line,pos,"/="});
                    pos+=2;idx+=2;
                    break;
                default:
                    token.push_back({Div,line,pos,"/"});
                    ++pos;++idx;
            }
        }
        else if(ch=='%'){
            switch(next()){
                case '=':
                    token.push_back({Mods,line,pos,"%="});
                    pos+=2;idx+=2;
                    break;
                default:
                    token.push_back({Mod,line,pos,"%"});
                    ++pos;++idx;
            }
        }
        //~ ^ & |
        else if(ch=='~')token.push_back({Bneg,line,pos,"~"}),++pos,++idx;
        else if(ch=='^')token.push_back({Xor,line,pos,"^"}),++pos,++idx;
        else if(ch=='&'){
            if(next()=='&')token.push_back({Land,line,pos,"&&"}),pos+=2,idx+=2;
            else token.push_back({Band,line,pos,"&"}),++pos,++idx;
        }
        else if(ch=='|'){
            if(next()=='|')token.push_back({Lor,line,pos,"||"}),pos+=2,idx+=2;
            else token.push_back({Bor,line,pos,"|"}),++pos,++idx;
        }
        //! != == >= <= > < <<  >>
        else if(ch=='!'){
            if(next()=='=')token.push_back({Ne,line,pos,"!="}),pos+=2,idx+=2;
            else token.push_back({Not,line,pos,"!"}),++pos,++idx;
        }
        else if(ch=='='){
            if(next()=='=')token.push_back({Eq,line,pos,"=="}),pos+=2,idx+=2;
            else token.push_back({Assign,line,pos,"="}),++pos,++idx;
        }
        else if(ch=='>'){
            if(next()=='=')token.push_back({Ge,line,pos,">="}),pos+=2,idx+=2;
            else if(next()=='>')token.push_back({Rs,line,pos,">>"}),pos+=2,idx+=2;
            else token.push_back({Gt,line,pos,">"}),++pos,++idx;
        }
        else if(ch=='<'){
            if(next()=='=')token.push_back({Le,line,pos,"<="}),pos+=2,idx+=2;
            else if(next()=='<')token.push_back({Ls,line,pos,"<<"}),pos+=2,idx+=2;
            else token.push_back({Lt,line,pos,"<"}),++pos,++idx;
        }
        //() [] {}  "" '' ; , .  :
        else if(ch=='(')token.push_back({Lpb,line,pos,"("}),++pos,++idx;
        else if(ch==')')token.push_back({Rpb,line,pos,")"}),++pos,++idx;
        else if(ch=='[')token.push_back({Lsb,line,pos,"["}),++pos,++idx;
        else if(ch==']')token.push_back({Rsb,line,pos,"]"}),++pos,++idx;
        else if(ch=='{')token.push_back({Lcb,line,pos,"{"}),++pos,++idx;
        else if(ch=='}')token.push_back({Rcb,line,pos,"}"}),++pos,++idx;
        else if(ch==';')token.push_back({Sem,line,pos,";"}),++pos,++idx;
        else if(ch==',')token.push_back({Comma,line,pos,","}),++pos,++idx;
        else if(ch=='.')token.push_back({Dot,line,pos,"."}),++pos,++idx;
        else if(ch==':')token.push_back({Colon,line,pos,":"}),++pos,++idx;
        else if(ch=='\"'){
            Token tk;tk.line=line,tk.pos=idx;tk.type=STR_Literal;
            bool flag=1;
            while(ch=next(),ch){
                if(ch=='\n')break;
                if(ch=='\"'&&buf[idx-1]!='\\'){
                    tk.data=string{buf.begin()+tk.pos+1,buf.begin()+idx+1};
                    flag=0;
                    break;
                }
                ++idx;
                ++pos;
            }
            if(flag){
                cerr<<"Error:where is another \" in line "<<line<<" position "<<pos<<endl;
                exit(0);
            }
            token.push_back(tk);
            idx+=2;pos+=2;
        }
        else if(ch=='\''){
            bool flag=1;
            Token tk;tk.line=line;tk.pos=idx;tk.type=CHR_Literal;
            if(next()=='\\'){
                if(nextn(3)=='\''){
                    tk.data.push_back('\\');
                    tk.data.push_back(nextn(2));
                    pos+=4;idx+=4;
                    flag=0;
                }
            }
            else if(nextn(2)=='\''){
                tk.data.push_back(next());
                pos+=3;idx+=3;
                flag=0;
            }
            if(flag){
                cerr<<"Error:where is another \' in line "<<line<<" position "<<pos<<endl;
                exit(0);
            }
            token.push_back(tk);
        }
        //解析数字(不支持浮点数)
        else if(isdigit(ch)){
            Token tk;tk.type=INT_Literal;tk.line=line;tk.pos=pos;
            while(isdigit(cur())){
                tk.data.push_back(cur());
                ++idx;++pos;
            }
            token.push_back(tk);
        }
        //解析ID
        else if(ch=='_'||(ch>='A'&&ch<='Z')||(ch>='a'&&ch<='z')){
            Token tk;tk.type=ID;tk.line=line;tk.pos=pos;
            tk.data.push_back(cur());
            ++idx;++pos;
            while((ch=cur())&&(ch=='_'||(ch>='A'&&ch<='Z')||(ch>='a'&&ch<='z')||(ch>='0'&&ch<='9'))){
                tk.data.push_back(ch);
                ++idx;++pos;
            }
            //判断tk是否为关键字
            if(KeyWord.count(tk.data)){
                tk.type=KeyWord[tk.data];
            }
            //
            token.push_back(tk);
        }
        //报错
        else{
            cerr<<"Error: unknown the character:\'"<<ch<<"\' what do you mean?"<<endl;
            exit(0);
        }
    }
}

void Lexifier::skip()
{
    bool flag=1;
    int preidx=idx;
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
            ++idx;
        }
        //处理预处理#号
        else if(ch0=='#'){
            while(idx<buf.size()&&buf[idx]!='\n')++idx;
            flag=1;
        }
    }
    //更新位置信息
    for(int i=preidx;i<idx;++i){
        if(buf[i]!='\n')++pos;
        else ++line,pos=1;
    }
}

char Lexifier::next(){
    return nextn(1);
}
char Lexifier::nextn(int n){
    int t=n+idx;
    return t<buf.size()?buf[t]:0;    
}

char Lexifier::cur()
{
    return nextn(0);
}

void Lexifier::debug()
{
    for(auto&tk:token){
        cout<<"line:"<<tk.line<<" pos:"<<tk.pos<<" "<<tk.type<<" "<<tk.data<<endl;
    }
}

