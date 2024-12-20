
// Generated from Expr.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"
#include "ExprParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by ExprParser.
 */
class  ExprVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by ExprParser.
   */
    virtual std::any visitCalc(ExprParser::CalcContext *context) = 0;

    virtual std::any visitMulDiv(ExprParser::MulDivContext *context) = 0;

    virtual std::any visitAddSub(ExprParser::AddSubContext *context) = 0;

    virtual std::any visitParens(ExprParser::ParensContext *context) = 0;

    virtual std::any visitInt(ExprParser::IntContext *context) = 0;


};

