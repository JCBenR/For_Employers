//
// Created by Justin Siebenhaar on 1/21/21.
//

#ifndef MSDSCRIPT_EXPR_H
#define MSDSCRIPT_EXPR_H

#include <string>
#include "Pointers.h"
//#include "Val.h"


class Val;
class Env;

typedef enum {
    print_group_none,
    print_group_add,
    print_group_add_or_mult,
    nested_let
} print_mode_t;

typedef enum {
    mult_lhs,
    mult_rhs,
    add_lhs,
    add_rhs,
    let_lhs,
    let_rhs,
    eq_lhs,
    eq_rhs,
    not_applicable
} came_from;

/**
 * expressions
 */
CLASS(Expr) {
public:
    virtual bool equals(PTR(Expr) e) = 0;
    virtual PTR(Val) interp(PTR(Env) env) = 0; //updated from int to PTR(Val)
    virtual bool has_variable() = 0;
    virtual void print(std::ostream& out) = 0;
    void pretty_print(std::ostream& out);
    std::string to_string();
    virtual void pretty_print_at(std::ostream& out, print_mode_t mode, int newLine, came_from parent) = 0;
    std::string pretty_print_string();
    virtual void step_interp() = 0;
};

/**
 * functional expressions represent the expression as it is to be read in. Will include the variables that will represent the expression (the formal arg) and the expression itself where formal arg should be substituted with the actual arguments before being interpreted
 */
class FunExpr: public Expr{
public:
    std::string formal_arg;
    PTR(Expr) body;
    FunExpr(std::string formal_arg, PTR(Expr) body);
    virtual bool equals(PTR(Expr) e);
    PTR(Val) interp(PTR(Env) env);
    bool has_variable();
    void print(std::ostream& out);
    void pretty_print_at(std::ostream& out, print_mode_t mode, int newLine, came_from parent);
    void step_interp();
};

/**
 * the calling of a function after it's been declared in a FunExpr. will take the function to be called and the actual arguments that will be substituted for the formal_arg on the original FunExpr.
 */
class CallExpr: public Expr{
public:
    PTR(Expr) to_be_called;
    PTR(Expr) actual_arg;
    CallExpr(PTR(Expr) to_be_called, PTR(Expr) actual_arg);
    virtual bool equals(PTR(Expr)e);
    PTR(Val) interp(PTR(Env) env);
    bool has_variable();
    void print(std::ostream& out);
    void pretty_print_at(std::ostream& out, print_mode_t mode, int newLine, came_from parent);
    void step_interp();
};

/**
 * numerical expressions of numbers.
 */
class NumExpr: public Expr{
public:
    //PTR(Val) val; //what does this do?
    int rep;
    NumExpr(int rep);
    virtual bool equals(PTR(Expr)e);
    PTR(Val) interp(PTR(Env) env);
    bool has_variable();
    void print(std::ostream& out);
    void pretty_print_at(std::ostream& out, print_mode_t mode, int newLine, came_from parent);
    void step_interp();
};

/**
 * equations of addition
 */
class AddExpr: public Expr{
public:
    PTR(Expr) lhs;
    PTR(Expr) rhs;
    AddExpr(PTR(Expr) lhs, PTR(Expr) rhs);
    virtual bool equals(PTR(Expr) e);
    PTR(Val) interp(PTR(Env) env);
    bool has_variable();
    void print(std::ostream& out);
    bool isNum();
    void pretty_print_at(std::ostream& out, print_mode_t mode, int newLine, came_from parent);
    void step_interp();

};

/**
 * equations of multiplication
 */
class MultExpr: public Expr{
public:
    PTR(Expr) lhs;
    PTR(Expr) rhs;
    MultExpr(PTR(Expr) lhs, PTR(Expr) rhs);
    virtual bool equals(PTR(Expr) e);
    PTR(Val) interp(PTR(Env) env);
    bool has_variable();
    void print(std::ostream& out);
    void pretty_print_at(std::ostream& out,print_mode_t mode, int newLine, came_from parent);
    void step_interp();

};

/**
 * variables that are used to represent expressions
 */
class VariableExpr: public Expr{
public:
     std::string val;
     explicit VariableExpr(std::string val);
     virtual bool equals(PTR(Expr) e);
     PTR(Val) interp(PTR(Env) env);
     bool has_variable();
     void print(std::ostream& out);
     void pretty_print_at(std::ostream& out,print_mode_t mode, int newLine, came_from parent);
     void step_interp();
};

/**
 * let expression give a variable that would equal an actual expression (x = 4). Could be more complex then this, such as full functions or other expressions. in these expression the variable and actual value are first declared and then the expression where such a substitution would happen.
 */
class LetExpr: public Expr{
public:
    PTR(VariableExpr) lhs;
    PTR(Expr) rhs;
    PTR(Expr) body;
    LetExpr(PTR(VariableExpr) lhs, PTR(Expr) rhs, PTR(Expr) body);
    virtual bool equals(PTR(Expr) e);
    PTR(Val) interp(PTR(Env) env);
    bool has_variable();
    void print(std::ostream& out);
    void pretty_print_at(std::ostream& out,print_mode_t mode, int newLine, came_from parent);
    void step_interp();
};

/**
 * expression of true or false (Boolean)
 */
class BoolExpr: public Expr{
public:
    bool rep;
    BoolExpr(bool rep);
    bool equals(PTR(Expr) e);
    PTR(Val) interp(PTR(Env) env); //updated from int to PTR(Val)
    bool has_variable();
    void print(std::ostream& out);
    void pretty_print(std::ostream& out);
    void pretty_print_at(std::ostream& out, print_mode_t mode, int newLine, came_from parent);
    void step_interp();
};

/**
 * expressions of equality between expressions. this is NOT equality of value (interpreted or otherwise) but of the exact expressions. Example (5+5 is not equal to 4+6)
 */
class EqExpr : public Expr{
public:
    //bool rep;
    PTR(Expr)lhs;
    PTR(Expr)rhs;
    EqExpr(PTR(Expr) lhs, PTR(Expr) rhs);
    bool equals(PTR(Expr) e);
    PTR(Val) interp(PTR(Env) env); //updated from int to PTR(Val)
    bool has_variable();
    void print(std::ostream& out);
    void pretty_print(std::ostream& out);
    void pretty_print_at(std::ostream& out, print_mode_t mode, int newLine, came_from parent);
    void step_interp();
};

/**
 * represent if statements and the branches for either true of false. will take in an expression to be evaluated first. if true will follow the _then branch, otherwise will follow the _else branch
 */
class IfExpr: public Expr{
public:
    PTR(Expr) tOrF;
    PTR(Expr) lhs;
    PTR(Expr) rhs;
    IfExpr(PTR(Expr) tOrF, PTR(Expr) lhs, PTR(Expr) rhs);
    bool equals(PTR(Expr) e);
    PTR(Val) interp(PTR(Env) env); //updated from int to PTR(Val)
    bool has_variable();
    void print(std::ostream& out);
    void pretty_print(std::ostream& out);
    void pretty_print_at(std::ostream& out, print_mode_t mode, int newLine, came_from parent);
    void step_interp();
};
#endif //MSDSCRIPT_EXPR_H
