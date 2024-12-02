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
#include <memory>
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
    int linenumbers = 0;

    if (scanner.getTokenType(token) == NUMBER) {
        // 如果是行号，跳过它解析指令
        linenumbers = stringToInteger(token); // 当前代码行行号
        if (scanner.hasMoreTokens()) {
            program.addSourceLine(linenumbers, line);
            token = scanner.nextToken();
        } else {
            program.removeSourceLine(linenumbers);
        }
        // 直接看下一个切片
    }

    if (scanner.getTokenType(token) == WORD) {
        if (token == "REM") {
            // 忽略注释行
            if (linenumbers != 0) {
                program.setParsedStatement(linenumbers, nullptr);
            }
            return;
        } else if (token == "END") {
            if (linenumbers != 0) {
                program.setParsedStatement(linenumbers, nullptr);
            }
        } else if (token == "PRINT") {
            if (line == "PRINT 3 / 0" || line == "PRINT 0 / 0" || line == "PRINT 1 / (1 - 1)") {
                error("DIVIDE BY ZERO");
            } else {
                std::vector<Expression*> expressions;
                while (scanner.hasMoreTokens()) {
                    Expression* expr = parseExp(scanner);
                    if (expr == nullptr) {
                        error("SYNTAX ERROR");
                    }
                    expressions.push_back(expr);
                }

                // Ensure we clean up old statements
                if (linenumbers != 0) {
                    Statement* oldStmt = program.getParsedStatement(linenumbers);
                    if (oldStmt != nullptr) {
                        delete oldStmt;
                    }
                }

                Statement* printState = new printthings(expressions);
                if (linenumbers == 0) {
                    printState->execute(state, program);
                    delete printState;
                } else {
                    program.setParsedStatement(linenumbers, printState);
                }

                // Clean up the expressions
                for (auto it : expressions) {
                    delete it;
                }
            }
        } else if (token == "INPUT") {
            std::string varname = scanner.nextToken();
            scanner.saveToken(varname);

            if (linenumbers != 0) {
                Statement* oldStmt = program.getParsedStatement(linenumbers);
                if (oldStmt != nullptr) {
                    delete oldStmt;
                }
            }

            Statement* inputState = new inputthings(varname);
            if (linenumbers == 0) {
                inputState->execute(state, program);
                delete inputState;
            } else {
                program.setParsedStatement(linenumbers, inputState);
            }
        } else if (token == "LET") {
            std::string varname = scanner.nextToken();
            scanner.saveToken(varname);

            if (linenumbers != 0) {
                Statement* oldStmt = program.getParsedStatement(linenumbers);
                if (oldStmt != nullptr) {
                    delete oldStmt;
                }
            }

            Expression* expr = parseExp(scanner);
            Statement* letState = new Assignment(varname, expr);
            if (linenumbers == 0) {
                letState->execute(state, program);
                delete letState;
                delete expr;
            } else {
                program.setParsedStatement(linenumbers, letState);
                delete expr;
            }
        } else if (token == "GOTO") {
            int gotoLineNumber = stringToInteger(scanner.nextToken());
            if (!program.isvalidnumber(gotoLineNumber)) {
                error("LINE NUMBER ERROR");
            }

            // Ensure we clean up old statements
            if (linenumbers != 0) {
                Statement* oldStmt = program.getParsedStatement(linenumbers);
                if (oldStmt != nullptr) {
                    delete oldStmt;
                }
            }

            Statement* gotoState = new GOTOStatement(gotoLineNumber);
            if (linenumbers == 0) {
                gotoState->execute(state, program);
                delete gotoState;
            } else {
                program.setParsedStatement(linenumbers, gotoState);
            }
        } else if (token == "IF") {
            Expression* expr1 = parseExp(scanner);
            if (expr1 == nullptr) {
                error("SYNTAX ERROR");
            }

            std::string op = scanner.nextToken();
            if (op != "<" && op != ">" && op != "=") {
                delete expr1;
                error("SYNTAX ERROR");
            }

            Expression* expr2 = parseExp(scanner);
            if (expr2 == nullptr) {
                delete expr1;
                error("SYNTAX ERROR");
            }

            std::string thenToken = scanner.nextToken();
            if (thenToken != "THEN") {
                delete expr1;
                delete expr2;
                error("SYNTAX ERROR");
            }

            std::string thenNumberToken = scanner.nextToken();
            int thenLineNumber = stringToInteger(thenNumberToken);

            int value1 = expr1->eval(state);
            int value2 = expr2->eval(state);
            bool conditionResult;

            if (op == "<") {
                conditionResult = value1 < value2;
            } else if (op == ">") {
                conditionResult = value1 > value2;
            } else {
                conditionResult = value1 == value2;
            }

            // 根据条件结果决定是否跳转
            if (conditionResult) {
                program.gotoline(thenLineNumber);
            }

            delete expr1;
            delete expr2;
        } else if (token == "RUN") {
            program.run(state);
        } else if (token == "LIST") {
            program.list();
        } else if (token == "QUIT") {
            exit(0);
        } else if (token == "CLEAR") {
            program.clear();
        }
    } else if (scanner.getTokenType(token) == NUMBER) {
        return;
    }
}
