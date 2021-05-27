//
// Created by Justin Siebenhaar on 4/1/21.
//

#ifndef MSDSCRIPT_STEP_H
#define MSDSCRIPT_STEP_H

#include "Pointers.h"
//#include "expr.h"
//#include "Cont.h"
//#include "Env.h"

class Expr;
class Cont;
class Val;
class Env;

/**
 * Step class allows for stepping through equations in  manner that manages memory and prevent memory overflows. each step records the expression, the environment, value and what is left after evaluating a step of the expression. it then passes evaluated steps on through until it can perform the final equation.
 */
CLASS(Step) {
public:
    typedef enum{
        interp_mode,
        continue_mode
    } mode_t;

    static mode_t mode;

    static PTR(Expr) expr;
    static PTR(Env) env;
    static PTR(Val) val;
    static PTR(Cont) cont;

    static PTR(Val) interp_by_steps(PTR(Expr) e);
};


#endif //MSDSCRIPT_STEP_H
