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
    int linenumbers;
    if (scanner.getTokenType(token) == NUMBER) {
        // 如果是行号，跳过它解析指令
        linenumbers = stringToInteger(token); // 当前代码行行号
        program.addSourceLine(linenumbers,line);
        token = scanner.nextToken();
        // 直接看下一个切片
    }

    if (scanner.getTokenType(token) == WORD) {
        if (token == "REM") {
            // 忽略注释行
            return;
        } else if (token == "END") {
            program.setParsedStatement(linenumbers, nullptr);
        } else if (token == "PRINT") {
            std::vector<Expression*> expressions;
            while (scanner.hasMoreTokens()) {
                Expression* expr = parseExp(scanner);
                if (expr == nullptr) {
                    error("SYNTAX ERROR");
                }
                expressions.push_back(expr);
            }
            program.setParsedStatement(linenumbers, new printthings(expressions));
            Statement* printstate=new printthings(expressions);
            printstate->execute(state,program);
            delete printstate;
        } else if (token == "INPUT") {
            std::string varname = scanner.nextToken();
            if (!state.isDefined(varname)) {
                error("VARIABLE NOT DEFINED");
            }
            program.setParsedStatement(linenumbers, new inputthings(varname));
            Statement* inputstate=new inputthings(varname);
            inputstate->execute(state,program);
            delete inputstate;
        } else if (token == "LET") {
            std::string varname = scanner.nextToken();
            Expression* expr = parseExp(scanner);
            program.setParsedStatement(linenumbers, new Assignment(varname, expr));
            Statement* letstate=new Assignment(varname, expr);
            letstate->execute(state,program);
            delete letstate;
        } else if (token == "GOTO") {
            int gotolinenumber = stringToInteger(scanner.nextToken());
            if (!program.isvalidnumber(gotolinenumber)) {
                error("LINE NUMBER ERROR");
            }
            program.setParsedStatement(linenumbers, new GOTOStatement(gotolinenumber));
            Statement* gotostate=new GOTOStatement(gotolinenumber);
            gotostate->execute(state,program);
            delete gotostate;
        } else if (token == "IF") {
            Expression* condition = parseExp(scanner);
            if (!scanner.hasMoreTokens() || scanner.nextToken()!= "THEN") {
                error("SYNTAX ERROR");
            }
            if (!scanner.hasMoreTokens()) {
                error("SYNTAX ERROR");
            }
            std::string nextone = scanner.nextToken();
            scanner.saveToken(nextone);
            if (scanner.getTokenType(nextone)!= NUMBER) {
                error("SYNTAX ERROR");
            }
            int thenlinenumber = stringToInteger(nextone);
            if (!program.isvalidnumber(thenlinenumber)) {
                error("LINE NUMBER ERROR");
            }
            Statement* thenstatement = program.getParsedStatement(thenlinenumber);
            program.setParsedStatement(linenumbers, new IFStatement(condition, thenstatement));
            Statement* ifstate=new IFStatement(condition, thenstatement);
            ifstate->execute(state,program);
            delete ifstate;
        } else if (token == "RUN") {
            program.run(state);
        } else if (token == "LIST") {
            program.list();
        } else if (token == "QUIT") {
            exit(0);
        } else if (token == "CLEAR") {
            program.clear();
        }
    }
}
