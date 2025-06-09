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
    static Token empty{0,token.size()?token.back().line:1,token.size()?token.back().pos+1:1};
    return t<token.size()?token[t]:empty;
}

Token& Parser::cur(){
    return nextn(0);
}

void Parser::parse(){
    //parse type_specifier
    Parse_type_specifier();
}

TypeSpecifier *Parser::Parse_type_specifier()
{
    TypeSpecifier*ret=new TypeSpecifier;
    //////////////////////////////////
    //check if is unsigned
    if(cur()==key_unsigned)ret->_signed=0,++idx;
    else ret->_signed=1;
    //get it's type 
    auto&tk=cur();
    if(CheckIsBaseType(tk))ret->baseType=tk.type,++idx;
    else{
        ret->baseType=0;
        if(tk.type==key_struct){
            auto r=Parse_Struct();
            ret->typeName=r.first;
            ret->si=r.second;
        }else{
            Error("I don't the type:"<<tk.data<<" in line:"<<tk.line<<" located at:"<<tk.pos);
        }
        ++idx;
    }
    ///////////////////////////////////
    return ret;
}

pair<string,StructInfo*> Parser::Parse_Struct()
{
    if(cur()!=key_struct){
        Error("parse struct failed in line:"<<cur().line<<" located at:"<<cur().pos);
    }
    ++idx;
    //
    string name=cur().data;
    if(cur()!=ID){
        Error("in line "<<cur().line<<" located at:"<<cur().pos<<" must to ID!");
    }
    ++idx;
    //
    StructInfo*info=0;
    if(cur()==Lcb){
        ++idx;
        info=Parse_struct_declaration_list(name);
        if(cur()!=Rcb){
            Error("in line:"<<cur().line<<" located at:"<<cur().pos<<" where is the \"}\" ?");
        }
        ++idx;
    }
    return {name,info};
}

StructInfo* Parser::Parse_struct_declaration_list(string name)
{
    StructInfo*ret=new StructInfo;
    ret->id=name;
    //
    while(cur()!=Rcb){
        //
        TypeSpecifier*tp=Parse_type_specifier();
        while(1){
            Declarator*dr=Parse_declarator();
            InitDeclarator init;
            init.init=0;
            init.dt=*dr;
            Declaration dt;dt.dl.push_back(init);
            ret->member.push_back(dt);//I generate one record for each member
            if(cur()==Comma){
                ++idx;
                continue;
            }else if(cur()==Sem){
                ++idx;
                break;
            }else{
                Error("declare error in line:"<<cur().line<<" locate at:"<<cur().pos);
            }
        }
    }
    //
    return ret;
}

Declarator *Parser::Parse_declarator()
{
    Declarator*ret=new Declarator;
    ret->pointer=0;
    //
    while(cur()==Star)++ret->pointer,++idx;
    //
    if(cur()==ID){
        ret->dd.id=cur().data;
        ret->dd.dt=0;
        ++idx;
    }
    else if(cur()==Lpb){
        ++idx;
        ret->dd.dt=Parse_declarator();
        if(cur()!=Rpb){
            Error("line:"<<cur().line<<" position:"<<cur().pos<<" must be \")\"");
        }
        ++idx;
    }
    //check if it is array
    if(cur()==Lsb){
        ++idx;
        if(cur()!=INT_Literal){
            Error("line:"<<cur().line<<" position:"<<cur().pos<<" must to be constant int");
        }
        int len=stoi(cur().data);
        if(len==0){
            Error("line:"<<cur().line<<" position:"<<cur().pos<<" the size of array must great than zero!");
        }
        ret->dd.arrayLen=len;
        ++idx;
        if(cur()!=Rsb){
            Error("line:"<<cur().line<<" position:"<<cur().pos<<" must be \"]\" !");
        }
        ++idx;
    }else{
        ret->dd.arrayLen=0;
    }
    return ret;
}

bool CheckIsBaseType(const Token &tk)
{
   if(tk==key_void||tk==key_byte||tk==key_hword||tk==key_word)return 1;
   return 0;
}
