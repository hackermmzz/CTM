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
    while(cur()!=0){
    //先解析类型
    auto*ts=Parse_type_specifier();
    //跳过指针
    int beg=idx;
    int predict=0;
    while(nextn(predict)!=0&&nextn(predict)==Star)++predict;
    //满足 type {*} id ( 肯定是函数了
    if(nextn(predict)==ID&&nextn(predict+1)==Lpb){
        auto*ret=Parse_function(ts);
        program.p.push_back({1,ret});
    }
    else{
        auto*ret=Parse_declaration(ts);
        program.p.push_back({0,ret});
    }
    }
}
Function *Parser::Parse_function(TypeSpecifier *ts)
{
    Function*ret=new Function;
    ret->retType0=*ts;
    //解析指针
    int beg=idx;
    while(cur()==Star)++idx;
    ret->pointer=idx-beg;
    //
    if(cur()!=ID){
        Error("in line "<<cur().data<<" pos "<<cur().pos<<" function must have a name");
    }
    //
    ret->id=cur().data;
    ++idx;
    //
    if(cur()!=Lpb){
        Error("in line "<<cur().data<<" pos "<<cur().pos<<" missing \"(\"");
    }
    ++idx;
    //
    if(cur()!=Rpb){
        while(1){
            //解析函数的参数
            auto*r0=Parse_type_specifier();
            auto*r1=Parse_declarator();
            Declaration dt;
            InitDeclarator idt;
            idt.dt=*r1;
            dt.dl.push_back(idt);
            dt.ts=*r0;
            ret->argument.push_back(dt);
            if(cur()==Comma){
                ++idx;
                continue;
            }
            else break;
        }
    }
    //
    if(cur()!=Rpb){
        Error("in line "<<cur().data<<" pos "<<cur().pos<<" missing \")\"");
    }
    ++idx;
    ret->body=Parse_composed_statement();
    return ret;
}

Declaration *Parser::Parse_declaration(TypeSpecifier *ts)
{
    Declaration*ret=new Declaration;
    ret->ts=*ts;
    //
    while(1){
        auto*r=Parse_initDeclarator();
        ret->dl.push_back(*r);
        if(cur()==Comma){
            ++idx;
        }
        else if(cur()==Sem){
            ++idx;
            break;
        }
        else{
            Error("in line "<<cur().line<<" pos "<<cur().pos<<" missing \";\"");
        }
    }
    return ret;
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
    //先跳过所有的*
    int beg=idx;
    while(cur()!=0&&cur()==Star)++idx;
    int end=idx;
    if(cur()==Lpb){
        ++idx;
        Declarator*rr=Parse_declarator();
        *ret=*rr;
        if(cur()!=Rpb){
            Error("in line "<<cur().line<<" located at "<<cur().pos<<" miss \")\"");
        }
        ++idx;
    }else if(cur()==ID){
        ret->id=cur().data;
        ++idx;
    }else{
        Error("one declarator must have id to name it!"<<" in line "<<cur().line<<" located at "<<cur().pos);
    }
    //把类型放到最后一层
    auto*tp=&(ret->tp);
    while(tp->tp)tp=tp->tp;
    //处理数组
    vector<int>array;
    while(cur()==Lsb){
        ++idx;
        if(cur()!=INT_Literal){
            Error("array definition must give a sure integer constant!"<<" in line "<<cur().line<<" pos "<<cur().pos)
        }
        int siz=stoi(cur().data);
        if(siz<=0){
            Error("array size must great than 0"<<" in line "<<cur().line<<" pos "<<cur().pos)
        }
        ++idx;
        if(cur()!=Rsb){
            Error("in line "<<cur().line<<" pos "<<cur().pos<<" missing \')\"");
        }
        ++idx;
        array.push_back(siz);
    }
    if(array.size()){
        if(tp->array.size()||tp->pointer)tp->tp=new DeclaratorType,tp=tp->tp;
        tp->array=array;
    }
    //处理指针
    int cnt=end-beg;
    if(tp->array.size()||tp->pointer)tp->tp=new DeclaratorType,tp=tp->tp;
    if(cnt)tp->pointer=cnt;
    //
    return ret;
}

InitDeclarator *Parser::Parse_initDeclarator()
{
    InitDeclarator*ret=new InitDeclarator;
    //解析declarator
    auto*r=Parse_declarator();
    ret->dt=*r;
    if(cur()==Assign){
        ++idx;
        ret->init=Parse_initializer();
    }
    return ret;
}

Initializer *Parser::Parse_initializer()
{
    auto*ret=new Initializer;
    if(cur()==Lcb){
        auto*r=Parse_initializerList();
        if(cur()!=Rcb){
            Error("in line "<<cur().line<<" pos "<<cur().pos<<" missing \"}\"");
        }
        ++idx;
        ret->list=r;
    }
    else ret->exp=Parse_assignment_expression();
    return ret;
}

AssignExp *Parser::Parse_assignment_expression()
{
    
}

InitializerList *Parser::Parse_initializerList()
{
    auto*ret=new InitializerList;
    auto*r=Parse_initializer();
    ret->init.push_back(r);
    while(cur()==Comma){
        ++idx;
        ret->init.push_back(Parse_initializer());
    }
}

ComposedStatment *Parser::Parse_composed_statement()
{
    return nullptr;
}

bool CheckIsBaseType(const Token &tk)
{
   if(tk==key_void||tk==key_byte||tk==key_hword||tk==key_word)return 1;
   return 0;
}
