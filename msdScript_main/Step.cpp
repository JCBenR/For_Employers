//
// Created by Justin Siebenhaar on 4/1/21.
//

#include "Step.h"
#include "Env.h"
#include "Cont.h"

Step::mode_t Step::mode;
PTR(Expr) Step::expr;
PTR(Env) Step::env;
PTR(Val) Step::val;
PTR(Cont) Step::cont;

/**
 * calls step mode on an equation
 * @param e expression to be passed
 * @return value of the evaluated expression.
 */
PTR(Val) Step::interp_by_steps(PTR(Expr) e) {
    Step::mode = Step::interp_mode;
    Step::expr = e;
    Step::env = Env::empty;
    Step::val = nullptr;
    Step::cont = Cont::done;

    while(1){
        if (Step::mode == Step::interp_mode)
            Step::expr->step_interp();
        else{
            if (Step::cont == Cont::done)
                return Step::val;
            else
                Step::cont->step_continue();
        }
    }
}

