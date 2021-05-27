//
// Created by Justin Siebenhaar on 2/23/21.
//

#ifndef MSDSCRIPT_VAL_H
#define MSDSCRIPT_VAL_H

#include <string>
#include "Pointers.h"
//#include "Cont.h"

class Env;
class Expr;
class Cont;

/**
 * give value to expressions
 */
CLASS(Val) {
public:
    virtual PTR(Val) add_to(PTR(Val) other_val) = 0;
    virtual PTR(Val) mult_to(PTR(Val) other_val) = 0;
    virtual bool equals(PTR(Val) e) =0;
    virtual bool is_true() = 0;
    virtual PTR(Val)call(PTR(Val)actual_arg) =0;
    virtual std::string get_string() = 0;
    virtual void call_step(PTR(Val) actual_arg_val, PTR(Cont) rest) = 0;
};

/**
 * //val can have a numerical value (2+1 = 3) this is the value of an expression
 */
class NumVal : public Val{
public:
    int rep;
    NumVal(int rep);
    PTR(Val) add_to(PTR(Val) other_val);
    PTR(Val) mult_to(PTR(Val) other_val);
    virtual bool equals(PTR(Val) e);
    bool is_true();
    PTR(Val)call(PTR(Val)actual_arg);
    std::string get_string();
    void call_step(PTR(Val) actual_arg_val, PTR(Cont) rest);
};

/**
 * //val can be a boolean value (2 == 1 = _false). this is the value of a comparison.
 */
class BoolVal: public Val{
public:
    bool rep;
    BoolVal(bool rep);
    PTR(Val) add_to(PTR(Val) other_val);
    PTR(Val) mult_to(PTR(Val) other_val);
    virtual bool equals(PTR(Val) e);
    bool is_true();
    PTR(Val)call(PTR(Val)actual_arg);
    std::string get_string();
    void call_step(PTR(Val) actual_arg_val, PTR(Cont) rest);
};

/**
 * function value
 */
class FunVal: public Val{
public:
    std::string formal_arg;
    PTR(Expr)body;
    PTR(Env) env;
    FunVal(std::string formal_arg, PTR(Expr)body, PTR(Env) env);
    PTR(Val) add_to(PTR(Val) other_val);
    PTR(Val) mult_to(PTR(Val) other_val);
    virtual bool equals(PTR(Val) e);
    bool is_true();
    PTR(Val)call(PTR(Val)actual_arg);
    std::string get_string();
    void call_step(PTR(Val) actual_arg_val, PTR(Cont) rest);
};


#endif //MSDSCRIPT_VAL_H
