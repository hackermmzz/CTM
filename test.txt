#include<iostream>
#include<map>
#include<vector>
#include"debug.cpp"
#include"Lexifier.h"
using namespace std;
///////////////////////解析参数
map<string,vector<string>>ParseArgument(int argc,char**argv);
///////////////////////设置默认参数
void SetDefaultArgument(map<string,vector<string>>&arg);
///////////////////////
int main(int argc,char**argv){
    //解析参数
    auto&&arguments=ParseArgument(argc,argv);
    SetDefaultArgument(arguments);
    //
    vector<string> file=arguments["-o"];
    //
    Lexifier lexifier(file.front());
    
    return 0;
}

map<string, vector<string>> ParseArgument(int argc, char **argv)
{
    map<string,vector<string>>ret;
    //对所有-x 类型获取其后面的参数，对于所有非此类型统一放到special里面
    auto&special= ret["Special"];
    int idx=0;
    while(idx<argc){
        string arg=argv[idx++];
        if(arg[0]!='-'){
            special.push_back(arg);
            continue;
        }
        //
        bool flag=0;
        auto&v=ret[arg.substr(1,arg.size())];
        while(idx<argc){
            string arg=argv[idx];
            if(arg[0]=='-')break;
            flag=1;
            v.push_back(arg);
            ++idx;
        }
        //
        if(!flag){
            special.push_back(arg);
        }
    }
    //
    return ret;
}

void SetDefaultArgument(map<string, vector<string>> &arg)
{
    //对输出文件进行设置
    if(arg.count("-o")==0)arg["-o"].push_back("a.out");
    //
}
