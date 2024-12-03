/*
 * File: program.h
 * ---------------
 * This interface exports a Program class for storing a BASIC
 * program.
 * 用于存储一个Basic程序
 */

#ifndef _program_h
#define _program_h

#include <string>
#include <vector>
#include <set>
#include <unordered_map>
#include "statement.hpp"
//可能会用set,vector , 或者unordered_map

class Statement;

/*
 * This class stores the lines in a BASIC program.  Each line
 * in the program is stored in order according to its line number.
 * Moreover, each line in the program is associated with two
 * components:
 *
 * 1. The source line, which is the complete line (including the
 *    line number) that was entered by the user.
 *
 * 2. The parsed representation of that statement, which is a
 *    pointer to a Statement.
 */

class Program {

public:

/*
 * Constructor: Program
 * Usage: Program program;
 * -----------------------
 * Constructs an empty BASIC program.
 */

    Program();

/*
 * Destructor: ~Program
 * Usage: usually implicit
 * -----------------------
 * Frees any heap storage associated with the program.
 */

    ~Program();

/*
 * Method: clear
 * Usage: program.clear();
 * -----------------------
 * Removes all lines from the program.
 * 清除程序中的所有指令
 */

    void clear();

/*
 * Method: addSourceLine
 * Usage: program.addSourceLine(lineNumber, line);
 * -----------------------------------------------
 * 添加一个带有指定行号的源代码行
 * Adds a source line to the program with the specified line number.
 * 如果这个行号原先存在，新添加的代码行将替代掉原有行的内容
 * 如果之前有解析过也将被删除
 * If that line already exists, the text of the line replaces
 * the text of any existing line and the parsed representation
 * (if any) is deleted.  If the line is new, it is added to the
 * program in the correct sequence.
 * 如果这个代码行是新的，这个新的代码行会被添加到程序中，保持正确的顺序
 */

    void addSourceLine(int lineNumber, const std::string& line);

/*
 * Method: removeSourceLine
 * Usage: program.removeSourceLine(lineNumber);
 * 从程序中移除指定编号的代码行，并释放任何解析过的内存
 * --------------------------------------------
 * Removes the line with the specified number from the program,
 * freeing the memory associated with any parsed representation.
 * If no such line exists, this method simply returns without
 * performing any action.
 * 如果这个代码行原本就不存在，直接返回，不做任何事情
 */

    void removeSourceLine(int lineNumber);

/*
 * Method: getSourceLine
 * Usage: string line = program.getSourceLine(lineNumber);
 * -------------------------------------------------------
 * 返回指定行号的代码行
 * 如果行号不存在，这个方法将返回一个空字符串
 * Returns the program line with the specified line number.
 * If no such line exists, this method returns the empty string.
 */

    std::string getSourceLine(int lineNumber);

/*
 * Method: setParsedStatement
 * Usage: program.setParsedStatement(lineNumber, stmt);
 * ----------------------------------------------------
 * 将语句的解析添加到指定行号的语句中（似乎意思是要解释一个语句吧 待定）
 * Adds the parsed representation of the statement to the statement
 * at the specified line number.  If no such line exists, this
 * method raises an error.  If a previous parsed representation
 * exists, the memory for that statement is reclaimed.
 * 如果没有这样的行存在，将引发一个错误Error
 * 如果这样的行以前已经被解析过，将之前那个语句的内存将被收回
 */

    void setParsedStatement(int lineNumber, Statement *stmt);

/*
 * Method: getParsedStatement
 * Usage: Statement *stmt = program.getParsedStatement(lineNumber);
 * ----------------------------------------------------------------
 * Retrieves the parsed representation of the statement at the
 * specified line number.  If no value has been set, this method
 * returns NULL.
 * 检索指定行号的语句的解析表示。（没看懂，待定）
 * 如果没有设置任何值，这个方法会返回NULL。
 */

    Statement *getParsedStatement(int lineNumber);

/*
 * Method: getFirstLineNumber
 * Usage: int lineNumber = program.getFirstLineNumber();
 * 返回程序中第一行的行号
 * 如果程序中没有任何行，这个方法返回-1。
 * -----------------------------------------------------
 * Returns the line number of the first line in the program.
 * If the program has no lines, this method returns -1.
 */

    int getFirstLineNumber();

/*
 * Method: getNextLineNumber
 * Usage: int nextLine = program.getNextLineNumber(lineNumber);
 * ------------------------------------------------------------
 * Returns the line number of the first line in the program whose
 * number is larger than the specified one, which must already exist
 * in the program.  If no more lines remain, this method returns -1.
 * 返回程序中行号大于指定行号的第一行的行号，指定的行号必须已经存在于程序中。
 * 省流：upperbound
 * 如果没有更多的行，这个方法返回-1。
 */

    int getNextLineNumber(int lineNumber);

    //more func to add
    //todo
    void gotoline(int linenumebr);
    void run(EvalState &states);
    void list();
    bool isvalidnumber(int linenumber);


private:

    // Fill this in with whatever types and instance variables you need
    //todo
    int currentLineNumber;//当前代码行号
    EvalState evalstate;
    bool endsignal=false;
    //比如存一些数据结构，如unorderedmap
    std::unordered_map<int,std::string> sourcelines;//存储源代码行
    std::unordered_map<int,Statement*> parsedStatements;//存储解释过的语句
};

#endif
