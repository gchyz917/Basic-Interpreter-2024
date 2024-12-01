/*
 * File: program.cpp
 * -----------------
 * 使用数据结构，如 map， set 等，请自行设计
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include "program.hpp"

#include <algorithm>


Program::Program() = default;

Program::~Program() = default;

void Program::clear() {
    // Replace this stub with your own code
    //todo
    sourcelines.clear();//清空存源代码的unorderedmap
    parsedStatements.clear();//清空存解释内容的unorderedmap
}

void Program::addSourceLine(int lineNumber, const std::string &line) {
    // Replace this stub with your own code
    //todo
    sourcelines[lineNumber]=line;
}

void Program::removeSourceLine(int lineNumber) {
    // Replace this stub with your own code
    //todo
    if(sourcelines.find(lineNumber)==sourcelines.end()) {
        return;
    }else{
        sourcelines.erase(lineNumber);//从源代码中移除
    }
    if(parsedStatements.find(lineNumber)!=parsedStatements.end()) {
        delete parsedStatements[lineNumber];//清除相应内存
        parsedStatements.erase(lineNumber);
    }
}

std::string Program::getSourceLine(int lineNumber) {
    // Replace this stub with your own code
    //todo
    auto it=sourcelines.find(lineNumber);
    if(it==sourcelines.end()) {
        return "";
    }else {
        return it->second;
    }
}

void Program::setParsedStatement(int lineNumber, Statement *stmt) {
    // Replace this stub with your own code
    //todo
    if(parsedStatements.find(lineNumber)!=parsedStatements.end()) {
        delete parsedStatements[lineNumber];//清空内存
        parsedStatements[lineNumber]=stmt;//待定
    }else {
        parsedStatements[lineNumber]=stmt;
    }
}

//void Program::removeSourceLine(int lineNumber) {

Statement *Program::getParsedStatement(int lineNumber) {
   // Replace this stub with your own code
   //todo
    auto it=parsedStatements.find(lineNumber);
    if(it!=parsedStatements.end()) {
        return it->second;
    }else {
        return NULL;
    }
}

int Program::getFirstLineNumber() {
    // Replace this stub with your own code
    //todo
    if(sourcelines.empty()) {
        return -1;
    }else {
        int minfirtline=1000;
        for(auto it:sourcelines) {
            if(it.first<minfirtline) {
                minfirtline=it.first;
            }
        }
        return minfirtline;
    }
}

int Program::getNextLineNumber(int lineNumber) {
    // Replace this stub with your own code
    //todo
    auto it=sourcelines.find(lineNumber);
    if(it==sourcelines.end()) {
        return -1;
    }else {
        std::vector<int> subsourceline;
        for(auto it:sourcelines) {
            subsourceline.push_back(it.first);
        }
        std::sort(subsourceline.begin(),subsourceline.end());
        if(lineNumber==subsourceline[subsourceline.size()-1]) {
            return -1;
        }else {
            auto it1=std::find(subsourceline.begin(),subsourceline.end(),lineNumber);
            return *(++it1);
        }
    }
}

//more func to add
//todo
void Program::gotoline(int linenumber) {
    currentLineNumber=linenumber;
}

void Program::run(EvalState &states) {
    currentLineNumber=getFirstLineNumber();
    while(currentLineNumber!=-1) {
        Statement* state=getParsedStatement(currentLineNumber);
        if(state!=nullptr) {
            state->execute(states,*this);
        }
        currentLineNumber=getNextLineNumber(currentLineNumber);//执行下一行的指令
    }
}

void Program::list() {
    std::vector<int>subsourcelines;
    for(auto it:sourcelines) {
        subsourcelines.push_back(it.first);
    }
    std::sort(subsourcelines.begin(),subsourcelines.end());
    for(int i=0;i<subsourcelines.size();i++){
        std::cout<<sourcelines[subsourcelines[i]]<<std::endl;
    }
}

bool Program::isvalidnumber(int linenumber) {
    return parsedStatements.find(linenumber)!=parsedStatements.end();
}

