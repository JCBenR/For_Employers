//
// Created by Justin Siebenhaar on 4/1/21.
//

#ifndef MSDSCRIPT_CONT_H
#define MSDSCRIPT_CONT_H

#include "Pointers.h"
#include <string>
//#include "expr.h"
//#include "Step.h"
//#include "Env.h"
//#include "Val.h"

class Val;
class Step;
class Expr;
class Env;

/**
 * continuations represent parts of a full expression still to be evaluated. used for memory management. individual parts of the expression are evaluated before mathematical or functional operations are performed and then passed on through.
 */
CLASS(Cont) {
public:
    static PTR(Cont) done;
    virtual void step_continue() = 0;
};

/**
 * will evaluate the right side before continuing.
 */
class RightThenAddCont : public Cont {
public:
    PTR(Expr) rhs;
    PTR(Env) env;
    PTR(Cont) rest;
    RightThenAddCont(PTR(Expr) rhs, PTR(Env )env, PTR(Cont) rest){
        this->rhs = rhs;
        this->env = env;
        this->rest = rest;
    };
    void step_continue() override;
};

/**
 * will add two values together.
 */
class AddCont : public Cont{
public:
    PTR(Val) lhs_val;
    PTR(Cont) rest;
    AddCont(PTR(Val) lhs_val, PTR(Cont) rest){
        this->lhs_val = lhs_val;
        this->rest = rest;
    };
    void step_continue() override;
};

/**
 * will evaluate the right side before continuing.
 */
class RightThenMultCont : public Cont {
public:
    PTR(Expr) rhs;
    PTR(Env) env;
    PTR(Cont) rest;
    RightThenMultCont(PTR(Expr) rhs, PTR(Env) env, PTR(Cont) rest){
        this->rhs = rhs;
        this->env = env;
        this->rest = rest;
    };
    void step_continue();
};

/**
 * multiplies to values together
 */
class MultCont : public Cont{
public:
    PTR(Val) lhs_val;
    PTR(Cont) rest;
    MultCont(PTR(Val) lhs_val, PTR(Cont) rest){
        this->lhs_val = lhs_val;
        this->rest = rest;
    };
    void step_continue();
};

/**
 * will evaluate the right side before continuing.
 */
class RightThenEqCont : public Cont{
public:
    PTR(Expr) rhs;
    PTR(Env) env;
    PTR(Cont) rest;
    RightThenEqCont(PTR(Expr) rhs, PTR(Env) env, PTR(Cont) rest){
        this->rhs = rhs;
        this->env = env;
        this->rest = rest;
    };
    void step_continue();
};

/**
 * evaluates equality of expressions
 */
class EqCont : public Cont{
public:
    PTR(Val) lhs_val;
    PTR(Cont) rest;
    EqCont(PTR(Val) lhs_val, PTR(Cont) rest){
        this->lhs_val = lhs_val;
        this->rest = rest;
    };
    void step_continue();
};

/**
 * continues on proper branch of if statement
 */
class IfBranchCont : public Cont{
public:
    PTR(Expr) then_part;
    PTR(Expr) else_part;
    PTR(Env) env;
    PTR(Cont) rest;
    IfBranchCont(PTR(Expr) then_part, PTR(Expr) else_part, PTR(Env) env, PTR(Cont) rest){
        this->then_part  = then_part;
        this->else_part = else_part;
        this->env = env;
        this->rest = rest;
    }
    void step_continue();
};

/**
 * continues with evaluation of a let statement. passed are the variable and the value of that variable (Expr), the environment and the rest of the continuation
 */
class LetBodyCont : public Cont{
public:
    std::string var;
    PTR(Expr) body;
    PTR(Env) env;
    PTR(Cont) rest;
    LetBodyCont(std::string var, PTR(Expr) body, PTR(Env) env, PTR(Cont) rest){
        this->var = var;
        this->body = body;
        this->env = env;
        this->rest = rest;
    }
    void step_continue();
};

/**
 * evaluates the arguments before continuing
 */
class ArgThenCallCont : public Cont {
public:
    PTR(Expr) actual_arg;
    PTR(Env) env;
    PTR(Cont) rest;
    ArgThenCallCont(PTR(Expr) actual_arg, PTR(Env) env, PTR(Cont) rest){
        this->actual_arg = actual_arg;
        this->env = env;
        this->rest = rest;
    }
    void step_continue();
};

/**
 * calls function with arguments substituted
 */
class CallCont : public Cont {
public:
    PTR(Val) to_be_called_val;
    PTR(Cont) rest;
    CallCont(PTR(Val) to_be_called_val, PTR(Cont) rest){
        this->to_be_called_val = to_be_called_val;
        this->rest = rest;
    }
    void step_continue();
};

/**
 * indicates finish of continuation
 */
class Done : public Cont {
public:
    void step_continue();
    Done(){};
};

#endif //MSDSCRIPT_CONT_H
