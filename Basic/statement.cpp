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
Assignment::~Assignment() {
    delete expression;
}
void Assignment::execute(EvalState &state, Program &program) {
    int value=expression->eval(state);
    state.setValue(varname,value);//对新定义的变量进行赋值
}

//print子类
printthings::printthings(const std::vector<Expression*>& exprs):expressions_(exprs) {
    this->expressions_=exprs;
}
printthings::~printthings() {
    for(auto expr:expressions_) {
        delete expr;
    }
    expressions_.clear();
}
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
void inputthings::execute(EvalState &state, Program &program) {
    std::string input;
    std::cin>>input;
    try {
        int value = stringToInteger(input); // 尝试将输入转换为整数
        state.setValue(varname, value); // 将值存储在 EvalState 中
    } catch (const std::exception& e) {
        // 如果转换失败，抛出一个 ErrorException
        error("INVALID NUMBER");
    }
}

//If语句
IFStatement::IFStatement(Expression *conditions, Statement *thenthings) {
    this->condition=conditions;
    this->then=thenthings;
}
IFStatement::~IFStatement() {
    delete condition;
    delete then;
}
void IFStatement::execute(EvalState &state, Program &program) {
    if(condition->eval(state)!=0) {
        then->execute(state,program);
    }
}

//GOTO语句
GOTOStatement::GOTOStatement(int number) {
    this->linenumber=number;
}
void GOTOStatement::execute(EvalState &state, Program &program) {
    program.gotoline(linenumber);//跳转到linenumber那行并执行
}
