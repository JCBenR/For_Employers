//
// Created by Justin Siebenhaar on 2/9/21.
//

#ifndef MSDSCRIPT_PARSE_H
#define MSDSCRIPT_PARSE_H
#include "expr.h"
#include "Pointers.h"
#include <iostream>
CLASS(Parse){
public:
    static PTR(Expr)parse_num(std::istream &in);
    static void consume(std::istream &in, int expect);
    static void skip_whitespace(std::istream &in);
    static PTR(Expr)parse_addend(std::istream &in);
    static PTR(Expr)parse_expr(std::istream &in);
    static PTR(Expr)parse_multicand(std::istream &in);
    static PTR(VariableExpr)parse_variable(std::istream &in);
    static PTR(Expr)parse_let(std::istream &in);
    static PTR(Expr)parse_if(std::istream &in);
    static PTR(Expr)parse_keyword(std::istream &in);
    static PTR(Expr)parse_str(std::string s);
    static PTR(Expr)parse_comparg(std::istream &in);
    static PTR(Expr)parse_inner(std::istream &in);
    static PTR(Expr)parse_fun(std::istream &in);
};


#endif //MSDSCRIPT_PARSE_H
