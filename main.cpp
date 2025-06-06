#include<iostream>
#include"debug.cpp"
#include"Lexifier.h"
using namespace std;
int main(int argc,char**argv){
    //current only need give me the file
    if(argc<=1){
        Error("Please input your file!");
        return 0;
    }
    //
    const char*file=argv[1];
    //
    Lexifier lexifier(file);
    
    return 0;
}