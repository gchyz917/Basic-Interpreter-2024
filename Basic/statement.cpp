/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include "statement.hpp"


/* Implementation of the Statement class */

int stringToInt(std::string str);

Statement::Statement() = default;

Statement::~Statement() = default;

//todo
void Statement::execute(EvalState &state, Program &program) {
    //TODO
}
//Let子类
Assignment::Assignment(const std::string &variable,Expression *expr) {
    this->varname=variable;
    this->expression=expr;
}
Assignment::~Assignment()=default;
void Assignment::execute(EvalState &state, Program &program) {
    int value=expression->eval(state);
    state.setValue(varname,value);//对新定义的变量进行赋值
}

//print子类
printthings::printthings(const std::vector<Expression*>& exprs):expressions_(exprs) {
    this->expressions_=exprs;
}
printthings::~printthings()=default;
void printthings::execute(EvalState &state, Program &program) {
    //std::cout << "Executing print statement" << std::endl;
    for(auto expr:expressions_) {
        if(expr!=nullptr) {
            int value=expr->eval(state);
            std::cout<<value;
            if(expr!=expressions_.back()) { //如果不是最后一个表达式
                std::cout<<" ";
            }
        }
    }
    std::cout<<std::endl;
}

//Input子类
inputthings::inputthings(const std::string variable) {
    this->varname=variable;
}
inputthings::~inputthings()=default;
void inputthings::execute(EvalState &state, Program &program) {
    std::string input;
    while(true) {
        std::cout<<" ? ";
        std::getline(std::cin,input,'\n');
        try {
            int value=stringToInteger(input);
            state.setValue(varname,value);
            break;
        }catch (const std::exception& e) {
            std::cout<<"INVALID NUMBER"<<std::endl;
        }
    }
}

//If语句
IFStatement::IFStatement(Expression* lhs, std::string op,Expression* rhs, int thenLinenumber)
    : expr1(lhs), op(op), expr2(rhs), thenLinenumber(thenLinenumber) {}

IFStatement::~IFStatement()=default;

void IFStatement::execute(EvalState &state, Program &program) {
    bool result = false;
    int value1=expr1->eval(state);
    int value2=expr2->eval(state);
    if (op == "<") result = value1 < value2;
    else if (op == ">") result = value1 > value2;
    else if (op == "=") result = value1 == value2;
    if (result) {
        program.gotoline(thenLinenumber);
    }
}
//GOTO语句
GOTOStatement::GOTOStatement(int number) {
    this->linenumber=number;
}
GOTOStatement::~GOTOStatement()=default;
void GOTOStatement::execute(EvalState &state, Program &program) {
    program.gotoline(linenumber);//跳转到linenumber那行并执行
}
