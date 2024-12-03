#include "program.hpp"
#include <algorithm>

Program::Program() = default;

Program::~Program() {
    for (auto& statement : parsedStatements) {
        delete statement.second;
    }
    parsedStatements.clear();
}

void Program::clear() {
    sourcelines.clear(); // 清空存源代码的 unordered_map
    parsedStatements.clear(); // 清空存解析内容的 unordered_map

}

void Program::addSourceLine(int lineNumber, const std::string &line) {
    sourcelines[lineNumber] = line;
}

void Program::removeSourceLine(int lineNumber) {
    if (sourcelines.find(lineNumber) == sourcelines.end()) {
        return;
    } else {
        sourcelines.erase(lineNumber); // 从源代码中移除
    }
    if (parsedStatements.find(lineNumber) != parsedStatements.end()) {
        delete parsedStatements[lineNumber]; // 清除相应内存
        parsedStatements.erase(lineNumber);
    }
}

std::string Program::getSourceLine(int lineNumber) {
    auto it = sourcelines.find(lineNumber);
    if (it == sourcelines.end()) {
        return "";
    } else {
        return it->second;
    }
}

void Program::setParsedStatement(int lineNumber, Statement *stmt) {
    if (parsedStatements.find(lineNumber) != parsedStatements.end()) {
        //delete parsedStatements[lineNumber]; // 清空内存
        parsedStatements[lineNumber] = stmt; // 存储新的语句
    } else {
        parsedStatements[lineNumber] = stmt;
    }
}

Statement *Program::getParsedStatement(int lineNumber) {
    auto it = parsedStatements.find(lineNumber);
    if (it != parsedStatements.end()) {
        return it->second;
    } else {
        return nullptr;
    }
}

int Program::getFirstLineNumber() {
    if (sourcelines.empty()) {
        return -1;
    } else {
        int minFirstLine = 1000;
        for (auto it : sourcelines) {
            if (it.first < minFirstLine) {
                minFirstLine = it.first;
            }
        }
        return minFirstLine;
    }
}

int Program::getNextLineNumber(int lineNumber) {
    auto it = sourcelines.find(lineNumber);
    if (it == sourcelines.end()) {
        return -1;
    } else {
        std::vector<int> sortedLineNumbers;
        for (auto& entry : sourcelines) {
            sortedLineNumbers.push_back(entry.first);
        }
        std::sort(sortedLineNumbers.begin(), sortedLineNumbers.end());

        auto it1 = std::find(sortedLineNumbers.begin(), sortedLineNumbers.end(), lineNumber);
        if (it1 == sortedLineNumbers.end() || ++it1 == sortedLineNumbers.end()) {
            return -1;
        }

        return *it1;
    }
}

void Program::gotoline(int linenumber) {
    currentLineNumber = linenumber; // 更新当前行号
}

void Program::run(EvalState &states) {
    currentLineNumber = getFirstLineNumber(); // 获取程序的第一行

    // 遍历每一行执行
    while (currentLineNumber != -1) {
        int original=currentLineNumber;
        bool isgoto=false;
        Statement* state = getParsedStatement(currentLineNumber);
        if (state != nullptr) {
            state->execute(states, *this); // 执行语句
            if(currentLineNumber!=original) {
                isgoto=true;
            }
            // 如果是 GOTO 或者 IF 跳转导致行号改变，不需要再次获取下一行
            if (currentLineNumber == -1) {
                // 如果 GOTO 更新了行号为 -1，表示跳转到无效行，结束循环
                break;
            }
        }else {
            if(endsignal[currentLineNumber]) {
                break;
            }
        }

        // 检查目标行号是否有效
        if (!isvalidnumber(currentLineNumber)) {
            throw ErrorException("LINE NUMBER ERROR");
        }

        // 获取下一行，避免跳转后继续从当前位置执行
        if(!isgoto){
            currentLineNumber=getNextLineNumber(currentLineNumber);
        }
    }
}

void Program::list() {
    std::vector<int> sortedLineNumbers;
    for (auto it : sourcelines) {
        sortedLineNumbers.push_back(it.first);
    }
    std::sort(sortedLineNumbers.begin(), sortedLineNumbers.end());

    for (int i = 0; i < sortedLineNumbers.size(); i++) {
        std::cout << sourcelines[sortedLineNumbers[i]] << std::endl;
    }
}

bool Program::isvalidnumber(int linenumber) {
    return parsedStatements.find(linenumber) != parsedStatements.end();
}

void Program::end(int linenumber) {
    endsignal[linenumber]=true;
}
