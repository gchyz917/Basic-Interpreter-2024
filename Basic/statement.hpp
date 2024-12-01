/*
 * File: statement.h
 * -----------------
 * Statement 一个抽象基类
 * This file defines the Statement abstract type.  In
 * the finished version, this file will also specify subclasses
 * for each of the statement types.  As you design your own
 * version of this class, you should pay careful attention to
 * the exp.h interface, which is an excellent model for
 * the Statement class hierarchy.
 * 注意：重点关注exp.h接口
 * 它是Statement类层次结构的一个很好的模型
 */

#ifndef _statement_h
#define _statement_h

#include <string>
#include <sstream>
#include "evalstate.hpp"
#include "exp.hpp"
#include "Utils/tokenScanner.hpp"
#include "program.hpp"
#include "parser.hpp"
#include "Utils/error.hpp"
#include "Utils/strlib.hpp"

class Program;

/*
 * Class: Statement
 * ----------------
 * This class is used to represent a statement in a program.
 * 这个类是用来表示程序中的一个语句
 * The model for this class is Expression in the exp.h interface.
 * Like Expression, Statement is an abstract class with subclasses
 * for each of the statement and command types required for the
 * BASIC interpreter.
 * 为Basic解释器所需的每种语句和命令提供了子类
 */

class Statement {

public:

/*
 * Constructor: Statement
 * ----------------------
 * The base class constructor is empty.  Each subclass must provide
 * its own constructor.
 * 基类构造函数为空，每个子类必须提供自己的构造函数
 */

    Statement();

/*
 * Destructor: ~Statement
 * Usage: delete stmt;
 * -------------------
 * The destructor deallocates the storage for this expression.
 * It must be declared virtual to ensure that the correct subclass
 * destructor is called when deleting a statement.
 * 析构函数用于释放这个表达式的存储空间
 * 被声明为虚函数，以确保在删除一个语句时调用正确的子类析构函数
 */

    virtual ~Statement();

/*
 * Method: execute
 * Usage: stmt->execute(state);
 * ----------------------------
 * This method executes a BASIC statement.  Each of the subclasses
 * defines its own execute method that implements the necessary
 * operations.  As was true for the expression evaluator, this
 * method takes an EvalState object for looking up variables or
 * controlling the operation of the interpreter.
 * 负责执行一个Basic语句
 * 每个子类都定义了自己的execute方法来实现必要的操作
 * 这个方法接受一个EvalState对象，用于查找变量或控制解释器的操作
 */

    virtual void execute(EvalState &state, Program &program) = 0;

};
class Assignment:public Statement {
private:
    std::string varname;//变量名
    Expression *expression;
public:
    Assignment(const std::string &variable,Expression *expr);
    virtual ~Assignment();
    virtual void execute(EvalState &state, Program &program);
};
//print子类
class printthings:public Statement {
private:
    std::vector<Expression*>expressions_;
public:
    printthings(const std::vector<Expression*>& exprs);
    ~printthings();
    virtual void execute(EvalState &state, Program &program);
};
//Input子类
class inputthings:public Statement {
private:
    std::string varname;
public:
    inputthings(const std::string variable);
    //似乎没有什么需要析构
    virtual void execute(EvalState &state, Program &program);
};
//If语句
class IFStatement:public Statement {
private:
    Expression* condition;//条件语句
    Statement* then;//分支语句
public:
    IFStatement(Expression* conditions,Statement* thenthings);
    ~IFStatement() ;
    virtual void execute(EvalState &state, Program &program);
};
//GOTO语句
class GOTOStatement:public Statement {
private:
    int linenumber;//跳转的行号
public:
    GOTOStatement(int number);
    //似乎不需要析构
    virtual void execute(EvalState &state, Program &program);
};
/*
 *此文件的其余部分必须由各个语句形式的子类定义组成
 *这些子类中的每一个都必须定义一个构造函数
 *该构造函数从scanner解析一个语句以及一个名为execute的方法
 *该方法执行语句
 *类实现还必须指定自己的析构方法来释放内存
 * The remainder of this file must consists of subclass
 * definitions for the individual statement forms.  Each of
 * those subclasses must define a constructor that parses a
 * statement from a scanner and a method called execute,
 * which executes that statement.  If the private data for
 * a subclass includes data allocated on the heap (such as
 * an Expression object), the class implementation must also
 * specify its own destructor method to free that memory.
 */

#endif
