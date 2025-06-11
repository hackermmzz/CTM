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
    ret->retType.tp=*ts;
    //解析指针
    int beg=idx;
    while(cur()==Star)++idx;
    ret->retType.pointer=idx-beg;
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
    //
    ++idx;
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
    AssignExp*ret=new AssignExp;
    auto*r=Parse_logical_or_expression();
    ret->exp0=r;
    auto&tk=cur();
    if(tk==Assign||tk==Adds||tk==Subs||tk==Muls||tk==Divs||tk==Mods){
        ret->op=tk.type;
        ++idx;
        ret->exp1=Parse_assignment_expression();
    }
    return ret;
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
    return ret;
}

ComposedStatment *Parser::Parse_composed_statement()
{
    ComposedStatment*ret=new ComposedStatment;
    if(cur()!=Lcb){
        Error("expect \"{\" in line "<<cur().line<<" pos "<<cur().pos);
    }
    ++idx;
    //
    while(cur()!=0&&cur()!=Rcb){
        auto tk=cur();
        //["unsigned"] ( "void" | "byte" | "hword" | "word")| "struct"
        if(CheckIsTypeSpecifier(tk)){
            ret->p.push_back({0,Parse_declaration(Parse_type_specifier())});
        }
        else ret->p.push_back({1,Parse_composed_statement()});
    }
    //
    if(cur()!=Rcb){
         Error("expect \"}\" in line "<<cur().line<<" pos "<<cur().pos);
    }
    return ret;
}

LogicalOrExp *Parser::Parse_logical_or_expression()
{
    LogicalOrExp*ret=new LogicalOrExp;
    ret->exp0=Parse_logical_and_expression();
    if(cur()==Lor){
        ++idx;
        ret->exp1=Parse_logical_and_expression();
    }
    return ret;
}

LogicalAndExp *Parser::Parse_logical_and_expression()
{
    LogicalAndExp*ret=new LogicalAndExp;
    ret->exp0=Parse_inclusive_or_expression();
    if(cur()==Land){
        ++idx;
        ret->exp1=Parse_inclusive_or_expression();
    }
    return ret;
}

InclusiveOrExp *Parser::Parse_inclusive_or_expression()
{
    InclusiveOrExp*ret=new InclusiveOrExp;
    ret->exp0=Parse_exclusive_or_expression();
    if(cur()==Bor){
        ++idx;
        ret->exp1=Parse_exclusive_or_expression();
    }
    return ret;
}

ExclusiveOrExp *Parser::Parse_exclusive_or_expression()
{
    ExclusiveOrExp*ret=new ExclusiveOrExp;
    ret->exp0=Parse_and_expression();
    if(cur()==Xor){
        ++idx;
        ret->exp1=Parse_and_expression();
    }
    return ret;
}

AndExp *Parser::Parse_and_expression()
{
    AndExp*ret=new AndExp;
    ret->exp0=Parse_equality_expression();
    if(cur()==Band){
        ++idx;
        ret->exp1=Parse_equality_expression();
    }
    return ret;
}

EqualityExp *Parser::Parse_equality_expression()
{
    EqualityExp*ret=new EqualityExp;
    ret->exp0=Parse_relational_expression();
    if(cur()==Eq||cur()==Ne){
        ret->tp=cur().type;
        ++idx;
        ret->exp1=Parse_relational_expression();
    }
    return ret;
}

RelationalExp *Parser::Parse_relational_expression()
{
    RelationalExp*ret=new RelationalExp;
    ret->exp0=Parse_shift_expression();
    if(cur()==Gt||cur()==Lt||cur()==Ge||cur()==Le){
        ret->tp=cur().type;
        ++idx;
        ret->exp1=Parse_shift_expression();
    }
    return ret;
}

ShiftExp *Parser::Parse_shift_expression()
{
    ShiftExp*ret=new ShiftExp;
    ret->exp0=Parse_additive_expression();
    if(cur()==Ls||cur()==Rs){
        ret->tp=cur().type;
        ++idx;
        ret->exp1=Parse_additive_expression();
    }
    return ret;
}

AdditiveExp *Parser::Parse_additive_expression()
{
    AdditiveExp*ret=new AdditiveExp;
    ret->exp0=Parse_multiplicative_expression();
    if(cur()==Add||cur()==Sub){
        ret->tp=cur().type;
        ++idx;
        ret->exp1=Parse_multiplicative_expression();
    }
    return ret;
}

MultiplicativeExp *Parser::Parse_multiplicative_expression()
{
    MultiplicativeExp*ret=new MultiplicativeExp;
    ret->exp0=Parse_cast_expression();
    if(cur()==Star||cur()==Div||cur()==Mod){
        ret->tp=cur().type;
        ++idx;
        ret->exp1=Parse_cast_expression();
    }
    return ret;
}

CastExp *Parser::Parse_cast_expression()
{
    CastExp*ret=new CastExp;
    if(cur()==Lpb){
        ++idx;
        ret->type=ParseTypeMark();
        if(cur()!=Rpb){
            Error("in line "<<cur().line<<" pos "<<cur().pos<<" missing \")\"");
        }
        ++idx;
        ret->castExp=Parse_cast_expression();
    }else{
        ret->exp=Parse_unary_expression();
    }
    return ret;
}

UnaryExp *Parser::Parse_unary_expression()
{
    UnaryExp*ret=new UnaryExp;
    if(cur()==Inc||cur()==Dec){
        ret->tp0=cur().type;
        ++idx;
        ret->unaryExp=Parse_unary_expression();
    }
    else if(cur()==Band||cur()==Star||cur()==Add||cur()==Sub||cur()==Bneg){
        ret->tp1=cur().type;
        ++idx;
        ret->castExp=Parse_cast_expression();
    }else{
        ret->postExp=Parse_postfix_expression();
    }
    return ret;
}

PostfixExp *Parser::Parse_postfix_expression()
{
    PostfixExp*ret=new PostfixExp;
    ret->primaryExp=Parse_primary_expression();
    //parse it's operations
    Token tk;
    while((tk=cur())!=0&&(tk==Lsb||tk==Lpb||tk==Dot||tk==Point||tk==Inc||tk==Dec)){
        ++idx;
        if(tk==Lsb){
            ret->op.push_back({PostfixExp::Index,Parse_expression()});
            if(tk!=Rsb){
                Error("in line "<<cur().line<<" pos "<<cur().pos<<" missing \"]\"");
            }
            ++idx;
        }else if(tk==Lpb){
            vector<AssignExp>*arg=new vector<AssignExp>;
            if(cur()!=Rpb){
                arg->push_back(*Parse_assignment_expression());
                while(cur()==Comma){
                    ++idx;
                    arg->push_back(*Parse_assignment_expression());
                }
            }
            ret->op.push_back({PostfixExp::Funcall,arg});
            if(tk!=Rpb){
                Error("in line "<<cur().line<<" pos "<<cur().pos<<" missing \")\"");
            }
            ++idx;
        }else if(tk==Dot||tk==Point){
            if(cur()!=ID){
                Error("expected a member name in line "<<cur().line<<" pos "<<cur().pos);
            }
            auto*r=Parse_primary_expression();
            ret->op.push_back({tk==Dot?PostfixExp::Dot:PostfixExp::Pointer,r});
            ++idx;
        }else if(tk==Inc||tk==Dec){
            ret->op.push_back({tk==Inc?PostfixExp::Add:PostfixExp::Sub,0});
        }
    }
    //
    return ret;
}

PrimaryExp *Parser::Parse_primary_expression()
{
    PrimaryExp*ret=new PrimaryExp;
    //
    if(cur()==ID){
        ret->id_or_constant=cur().data;
        ret->tp=ID;
        ++idx;
    }
    else if(cur()==INT_Literal||cur()==CHR_Literal||cur()==STR_Literal){
        ret->tp=cur().type;
        ret->id_or_constant=cur().data;
        ++idx;
    }
    else if(cur()==Lpb){
        ++idx;
        ret->exp=Parse_expression();
        if(cur()!=Rpb){
            Error("Expected \")\" in line "<<cur().line<<" pos "<<cur().pos);
        }
        ++idx;
    }
    else{
        Error("in line "<<cur().line<<" pos "<<cur().pos<<" occur error when parse expression");
    }
    //
    return ret;
}

Expression *Parser::Parse_expression()
{
    Expression*ret=new Expression;
    ret->exp.push_back(*Parse_assignment_expression());
    while(cur()==Comma){
        ++idx;
        ret->exp.push_back(*Parse_assignment_expression());
    }
    return ret;
}

TypeMark Parser::ParseTypeMark()
{
    TypeMark ret;
    ret.tp=*Parse_type_specifier();
    int beg=idx;
    while(cur()!=0&&cur()==Star)++idx;
    ret.pointer=idx-beg;
    return ret;
}

IfElseStatment *Parser::Parse_IfElseStatement()
{
    IfElseStatment*ret=new IfElseStatment;
    //
    if(cur()!=key_if){
        Error("expected key word if in line "<<cur().line<<" pos "<<cur().pos);
    }
    ++idx;
    if(cur()!=Lpb){
        Error("expected \"(\" in line "<<cur().line<<" pos "<<cur().pos);
    }
    ++idx;
    ret->exp0=Parse_expression();
    if(cur()!=Rpb){
         Error("expected \")\" in line "<<cur().line<<" pos "<<cur().pos);
    }
    ++idx;
    ret->stm0=Parse_Statement();
    if(cur()==key_else){
        ++idx;
        ret->stm1=Parse_Statement();
    }
    return ret;
}

SwitchStatment *Parser::Parse_SwitchStatement()
{
    SwitchStatment*ret=new SwitchStatment;
    if(cur()!=key_switch){
        Error("expected key word switch in line "<<cur().line<<" pos "<<cur().pos);
    }
    ++idx;
    
    if(cur()!=Lpb){
        Error("expected \"(\" in line "<<cur().line<<" pos "<<cur().pos);
    }
    ++idx;
    //
    ret->exp=Parse_expression();
    //
    if(cur()!=Rpb){
        Error("expected \")\" in line "<<cur().line<<" pos "<<cur().pos);
    }
    ++idx;
    //
    if(cur()!=Lcb){
         Error("expected \"{\" in line "<<cur().line<<" pos "<<cur().pos);
    }
    ++idx;
    //
    set<int>occur;
    bool defaultOccur=0;
    while(cur()!=0&&cur()!=Rcb){
        if(cur()==key_case){
            ++idx;
            if(cur()!=INT_Literal&&cur()!=CHR_Literal){
                Error("the expression after case must be constant in line "<<cur().line<<" pos "<<cur().pos);
            }
            int dat;
            if(cur()==INT_Literal)dat=stoi(cur().data);
            else dat=CharToInt(cur().data);
            if(occur.count(dat)){
                Error("the case branch \""<<(cur()==INT_Literal?dat:char(dat))<<"\" have occur before in line "<<cur().line<<" pos "<<cur().pos);
            }
            occur.insert(dat);
            ++idx;
            //
            
            if(cur()!=Colon){
                Error("in line "<<cur().line<<" pos "<<cur().pos<<" expected \":\"");
            }
            ++idx;
            ret->body.push_back({0,dat,{}});
        }
        else if(cur()==key_default){
            if(defaultOccur){
                Error("Only one default branch can occur in the switch statement in line "<<cur().line<<" pos "<<cur().pos);
            }
            defaultOccur=1;
            ++idx;
             if(cur()!=Colon){
                Error("in line "<<cur().line<<" pos "<<cur().pos<<" expected \":\"");
            }
            ++idx;
            ret->body.push_back({1,0,{}});
        }
        else{
            Statement*r=Parse_Statement();
            if(ret->body.size())get<2>(ret->body.back()).push_back(r);
        }
    }
    //
    if(cur()!=Rcb){
         Error("expected \"}\" in line "<<cur().line<<" pos "<<cur().pos);
    }
    return ret;
}

WhileStatement *Parser::Parse_WhileStatement()
{
    WhileStatement*ret=new WhileStatement;
    //
    if(cur()!=key_while){
        Error("in fact I am parsing while statement but why i am here in line "<<cur().line<<" pos "<<cur().pos);
    }
    ++idx;
    if(cur()!=Lpb){
        Error("expected \"(\" in line "<<cur().line<<" pos "<<cur().pos);
    }
    ++idx;
    ret->exp=Parse_expression();
    if(cur()!=Rpb){
        Error("expected \")\" in line "<<cur().line<<" pos "<<cur().pos);
    }
    ++idx;
    ret->stm=Parse_Statement();
    //
    return ret;
}

ForStatement *Parser::Parse_ForStatement()
{
    ForStatement*ret=new ForStatement;
    //
    if(cur()!=key_for){
         Error("in fact I am parsing for statement but why i am here in line "<<cur().line<<" pos "<<cur().pos);
    }
    ++idx;
    if(cur()!=Lpb){
        Error("expected \"(\" in line "<<cur().line<<" pos "<<cur().pos);
    }
    ++idx;
    //
    if(CheckIsTypeSpecifier(cur())){
        ret->dec0=Parse_declaration(Parse_type_specifier());
    }
    else{
        ret->exp0=Parse_ExpressionStatement();
    }
    //
    ret->exp1=Parse_ExpressionStatement();
    //
    if(cur()!=Rpb)ret->exp2=Parse_expression();
    //
    if(cur()!=Rpb){
        Error("expected \")\" in line "<<cur().line<<" pos "<<cur().pos);
    }
    ++idx;
    //
    ret->stm=Parse_Statement();
    return ret;
}

DoWhileStatment *Parser::Parse_DoWhileStatement()
{
    DoWhileStatment*ret=new DoWhileStatment;
    if(cur()!=key_do){
        Error("in fact I am parsing do while statement but why i am here in line "<<cur().line<<" pos "<<cur().pos);
    }
    ++idx;
    ret->body=Parse_Statement();
    if(cur()!=key_while){
        Error("expected key word while in line "<<cur().line<<" pos "<<cur().pos);
    }
    ++idx;
    if(cur()!=Lpb){
        Error("missing \"(\" in line "<<cur().line<<" pos "<<cur().pos);
    }
    ++idx;
    ret->exp=Parse_expression();
    if(cur()!=Rpb){
        Error("missing \")\" in line "<<cur().line<<" pos "<<cur().pos);
    }
    ++idx;
    if(cur()!=Sem){
        Error("missing \";\" in line "<<cur().line<<" pos "<<cur().pos);
    }
    ++idx;
    return ret;
}

ReturnStatement *Parser::Parse_ReturnStatement()
{
    ReturnStatement*ret=new ReturnStatement;
    //
    if(cur()!=key_return){
        Error("in fact I am parsing return statement but why i am here in line "<<cur().line<<" pos "<<cur().pos);
    }
    ++idx;
    //
    ret->exp=Parse_ExpressionStatement();
    //
    return ret;
}

AsmStatement *Parser::Parse_AsmStatement()
{
    AsmStatement*ret=new AsmStatement;
    //
    if(cur()!=key_asm){
         Error("in fact I am parsing asm statement but why i am here in line "<<cur().line<<" pos "<<cur().pos);
    }
    ++idx;
    //
    if(cur()!=Lpb){
        Error("expected \"(\" in line "<<cur().line<<" pos "<<cur().pos);
    }
    ++idx;
    if(cur()!=Rpb){
        while(1){
            if(cur()!=STR_Literal){
                Error("expected mips instruction in line "<<cur().line<<" pos "<<cur().pos);
            }
            ret->ins.
            push_back(cur().data);
            if(cur()==Comma){
                ++idx;
                continue;
            }
            else break;
        }
    }
    //
    if(cur()!=Rpb){
        Error("expected \")\" in line "<<cur().line<<" pos "<<cur().pos);
    }
    ++idx;
    if(cur()!=Sem){
        Error("expected \";\" in line "<<cur().line<<" pos "<<cur().pos);
    }
    ++idx;
    //
    return ret;
}

Statement *Parser::Parse_Statement()
{
    Statement*ret=new Statement;
    auto tk=cur();
    if(tk==Lcb){
       *ret={Statement::Composed,Parse_composed_statement()};
    }
    else if(tk==key_if){
        *ret={Statement::IfElse,Parse_IfElseStatement()};
    }
    else if(tk==key_switch){
        *ret={Statement::Switch,Parse_SwitchStatement()};
    }
    else if(tk==key_while){
        *ret={Statement::While,Parse_WhileStatement()};
    }
    else if(tk==key_for){
        *ret={Statement::For,Parse_ForStatement()};
    }
    else if(tk==key_do){
        *ret={Statement::DoWhile,Parse_DoWhileStatement()};
    }
    else if(tk==key_continue){
        *ret={Statement::Continue,0};
        if(next()!=Sem){
            Error("expected \";\" in line "<<next().line<<" pos "<<next().pos);
        }
        idx+=2;
    }
    else if(tk==key_break){
        *ret={Statement::Break,0};
        if(next()!=Sem){
            Error("expected \";\" in line "<<next().line<<" pos "<<next().pos);
        }
        idx+=2;
    }
    else if(tk==key_return){
        *ret={Statement::Return,Parse_ReturnStatement()};
    }
    else if(tk==key_asm){
        *ret={Statement::Asm,Parse_AsmStatement()};
    }
    else{
        *ret={Statement::Expression,Parse_ExpressionStatement()};
    }

     return ret;
}
Expression *Parser::Parse_ExpressionStatement()
{
    Expression*exp=new Expression;
    if(cur()==Sem){
        ++idx;
        return exp;
    }
    //
    exp=Parse_expression();
    //
    if(cur()!=Sem){
        Error("Expected \";\" in line "<<cur().line<<" pos "<<cur().pos);
    }
    ++idx;
    //
    return exp;
}
   
bool CheckIsBaseType(const Token &tk)
{
   if(tk==key_void||tk==key_byte||tk==key_hword||tk==key_word)return 1;
   return 0;
}

int CharToInt(const string &s)
{
    if(s.size()>2||(s.size()==2&&s[0]!='\\')){
        static_assert("wlh is da sha gou!!!");
    }
    if(s.size()==2){
        char nxt=s[1];
        switch (nxt)
        {
            
            case 'a':return '\a';
            case 'b':return '\b';
            case 'n':return '\n';
            case 'r':return '\r';
            case 't':return '\t';
            default:return nxt;
        }
    }
    return s[1];
}

bool CheckIsTypeSpecifier(const Token &tk)
{
    if(tk==key_unsigned||CheckIsBaseType(tk)||tk==key_struct)return 1;
    return 0;
}
