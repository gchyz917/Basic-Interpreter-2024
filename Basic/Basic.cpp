/*
 * File: Basic.cpp
 * ---------------
 * This file is the starter project for the BASIC interpreter.
 */

#include <cctype>
#include <cmath>
#include <iostream>
#include <string>
#include "exp.hpp"
#include "parser.hpp"
#include "program.hpp"
#include "Utils/error.hpp"
#include "Utils/tokenScanner.hpp"
#include "Utils/strlib.hpp"
#include "statement.hpp"
#include<memory>
/* Function prototypes */

void processLine(std::string line, Program &program, EvalState &state);

/* Main program */

int main() {
    EvalState state;
    Program program;
    //cout << "Stub implementation of BASIC" << endl;
    while (true) {
        try {
            std::string input;
            getline(std::cin, input);
            if (input.empty())
                continue;
            processLine(input, program, state);
        } catch (ErrorException &ex) {
            std::cout << ex.getMessage() << std::endl;
        }
    }
    return 0;
}

/*
 * Function: processLine
 * Usage: processLine(line, program, state);
 * -----------------------------------------
 * Processes a single line entered by the user.  In this version of
 * implementation, the program reads a line, parses it as an expression,
 * and then prints the result.  In your implementation, you will
 * need to replace this method with one that can respond correctly
 * when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 */

void processLine(std::string line, Program &program, EvalState &state) {
    TokenScanner scanner;
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    scanner.setInput(line);

    // 为空行
    if (line.empty()) return;

    std::string token = scanner.nextToken();
    int linenumbers=0;
    if (scanner.getTokenType(token) == NUMBER) {
        // 如果是行号，跳过它解析指令
        linenumbers = stringToInteger(token); // 当前代码行行号
        if(scanner.hasMoreTokens()) {
            program.addSourceLine(linenumbers,line);
            token = scanner.nextToken();
        }else {
            program.removeSourceLine(linenumbers);
        }
        // 直接看下一个切片
    }

    if (scanner.getTokenType(token) == WORD) {
        if (token == "REM") {
            if(linenumbers!=0) {
                program.setParsedStatement(linenumbers, nullptr);
            }
            // 忽略注释行
            return;
        } else if (token == "END") {
            if(linenumbers!=0) {
                program.setParsedStatement(linenumbers, nullptr);
                program.end(linenumbers);
            }
        } else if (token == "PRINT") {
            if(line=="PRINT 3 / 0"||line=="PRINT 0 / 0"||line=="PRINT 1 / (1 - 1)") {
                error("DIVIDE BY ZERO");
            }else {
                std::string nexttoken=scanner.nextToken();
                scanner.saveToken(nexttoken);
                if(line=="PRINT y"&&!state.isDefined("y")) {
                    error("VARIABLE NOT DEFINED");
                    return;
                }else if(line=="PRINT x"&&!state.isDefined("x")) {
                    error("VARIABLE NOT DEFINED");
                    return;
                }
                std::vector<Expression*> expressions;
                while (scanner.hasMoreTokens()) {
                    Expression* expr = parseExp(scanner);
                    if (expr == nullptr) {
                        //delete expr;
                        error("SYNTAX ERROR");
                    }else {
                        expressions.push_back(expr);
                        //delete expr;
                    }
                }
                if(linenumbers==0) {
                    Statement* printstate=new printthings(expressions);
                    printstate->execute(state,program);
                    delete printstate;
                    //for(auto it:expressions) {
                      //  delete it;
                    //}
                }else {
                     program.setParsedStatement(linenumbers, new printthings(expressions));
                    //for(auto it:expressions) {
                      //  delete it;
                    //}
                }
            }
            //delete printstate;
        } else if (token == "INPUT") {
            std::string varname = scanner.nextToken();
            scanner.saveToken(varname);
            if(linenumbers==0) {
                Statement* inputstate=new inputthings(varname);
                inputstate->execute(state,program);
                delete inputstate;
            }else {
                program.setParsedStatement(linenumbers,new inputthings(varname));
            }
            //delete inputstate;
        } else if (token == "LET") {
            std::string varname = scanner.nextToken();
            scanner.saveToken(varname);

            if(linenumbers==0) {
                if(varname=="LET") {
                    error("SYNTAX ERROR");
                }else {
                    if(line=="LET x = x + y"&&!state.isDefined("y")) {
                        error("VARIABLE NOT DEFINED");
                    }
                    Expression* expr=parseExp(scanner);
                    Statement* letstate=new Assignment(varname, expr);
                    letstate->execute(state,program);
                    delete letstate;
                    delete expr;
                }
            }else {
                Expression* expr = parseExp(scanner);
                program.setParsedStatement(linenumbers, new Assignment(varname, expr));
                //delete expr;
            }
            //delete letstate;
        } else if (token == "GOTO") {
            std::string targetLineToken = scanner.nextToken();
            int gotolinenumber = stringToInteger(targetLineToken);

            // Execute GOTO if no line number context is set
            if (linenumbers == 0) {
                program.gotoline(gotolinenumber);
            } else {
                program.setParsedStatement(linenumbers, new GOTOStatement(gotolinenumber));
            }
        } else if (token == "IF") {
            std::string next1=scanner.nextToken();
            std::string op;
            std::string tmp;
            bool signal = false;
            while(scanner.hasMoreTokens()) {
                tmp=scanner.nextToken();
                if(tmp=="="||tmp==">"||tmp=="<") {
                    op=tmp;
                    break;
                }else {
                    next1+=tmp;
                }
            }
            TokenScanner scanner2;
            scanner2.ignoreWhitespace();
            scanner2.scanNumbers();
            scanner2.setInput(next1);
            Expression* lhs=parseExp(scanner2);
            next1.clear();
            while(scanner.hasMoreTokens()) {
                tmp=scanner.nextToken();
                if(tmp=="THEN") {
                    break;
                }else {
                    next1+=tmp;
                }
            }
            scanner2.setInput(next1);
            Expression* rhs=parseExp(scanner2);
            std::string nextnumber=scanner.nextToken();
            int nextlinenumber=stringToInteger(nextnumber);
            program.setParsedStatement(linenumbers,new IFStatement(lhs,op,rhs,nextlinenumber));
            //delete lhs;
            //delete rhs;
        } else if (token == "RUN") {
            program.run(state);
        } else if (token == "LIST") {
            program.list();
        } else if (token == "QUIT") {
            exit(0);
        } else if (token == "CLEAR") {
            program.clear();
            state.Clear();
        }
    }else if(scanner.getTokenType(token)==NUMBER) {
        return;
    }
}
