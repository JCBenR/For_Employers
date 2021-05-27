//
// Created by Justin Siebenhaar on 4/1/21.
//

#include "Cont.h"
#include "Step.h"
#include "Env.h"
#include "Val.h"
#include "catch.h"
#include "Parse.h"

PTR(Cont) Cont::done = NEW(Done)();

/**
 * will throw error if step continue is called on done (meaning there are no more steps).
 */
void Done::step_continue() {
    throw std::runtime_error("can't continue done");
}

/**
 * evaluated right side before passing values to AddCont for addition
 */
void RightThenAddCont::step_continue() {
    PTR(Val) lhs_val = Step::val;
    Step::mode = Step::interp_mode;
    Step::expr = rhs;
    Step::env = env;
    Step::cont = NEW(AddCont)(lhs_val, rest);
}

/**
 * adds lhs and rhs then continues
 */
void AddCont::step_continue() {
    PTR(Val) rhs_val = Step::val;
    Step::mode = Step::continue_mode;
    Step::val = lhs_val->add_to(rhs_val);
    Step::cont = rest;
}

/**
 * evaluated right side before passing values to MultCont for multiplication
 */
void RightThenMultCont::step_continue() {
    PTR(Val) lhs_val = Step::val;
    Step::mode = Step::interp_mode;
    Step::expr = rhs;
    Step::env = env;
    Step::cont = NEW(MultCont)(lhs_val, rest);
}

/**
 * multiplies lhs and rhs then continues
 */
void MultCont::step_continue() {
    PTR(Val) rhs_val = Step::val;
    Step::mode = Step::continue_mode;
    Step::val = lhs_val->mult_to(rhs_val);
    Step::cont = rest;
}

/**
 * evaluates rhs before passing both EqCont for comparison
 */
void RightThenEqCont::step_continue(){
    PTR(Val) lhs_val = Step::val;
    Step::mode = Step::interp_mode;
    Step::expr = rhs;
    Step::env = env;
    Step::cont = NEW(EqCont)(lhs_val, rest);
}

/**
 * evaluates the equality of both sizes values before continuing
 */
void EqCont::step_continue() {
    PTR(Val) rhs_val = Step::val;
    Step::mode = Step::continue_mode;
    Step::val = NEW(BoolVal)(lhs_val->equals(rhs_val));
    Step::cont = rest;
}

/**
 * evaluates which branch of If statement to follow, then continues
 */
void IfBranchCont::step_continue() {
    PTR(Val) test_val = Step::val;
    Step::mode = Step::interp_mode;
    if (test_val->is_true())
        Step::expr = then_part;
    else
        Step::expr = else_part;
    Step::env = env;
    Step::cont = rest;
}

/**
 * evaluates let statement then continues
 */
void LetBodyCont::step_continue() {
    Step::mode = Step::interp_mode;
    Step::expr = body;
    Step::env = NEW(ExtendedEnv)(var, Step::val, env);
    Step::cont = rest;
}

/**
 * evaluates argument of function before continuing with call
 */
void ArgThenCallCont::step_continue() {
    Step::mode = Step::interp_mode;
    Step::expr = actual_arg;
    Step::env = env;
    Step::cont = NEW(CallCont)(Step::val, rest);
}

/**
 * calls function with arguments
 */
void CallCont::step_continue() {
    to_be_called_val->call_step(Step::val, rest);
}

TEST_CASE("step_continue"){
    CHECK( Step::interp_by_steps(Parse::parse_str("1"))->equals(NEW(NumVal)(1)) );
    CHECK(Step::interp_by_steps(NEW(AddExpr)(NEW(NumExpr)(5), NEW(NumExpr)(5)))->equals(NEW(NumVal)(10)));
    CHECK(Step::interp_by_steps(NEW(MultExpr)(NEW(NumExpr)(5), NEW(NumExpr)(5)))->equals(NEW(NumVal)(25)));

    PTR(Expr)addFirst = NEW(AddExpr)(NEW(NumExpr)(5), NEW(NumExpr)(1));
    PTR(Expr)addSecond = NEW(AddExpr)(NEW(NumExpr)(5), NEW(NumExpr)(1));

    PTR(Expr)ansFirst = NEW(NumExpr)(6);
    PTR(Expr)addEqExpr = NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2));
    PTR(Expr)addEqExpr2 = NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2));
    PTR(Expr)addEqExpr3 = NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(3));

    CHECK(Step::interp_by_steps(Parse::parse_str("2 == 2"))->equals(NEW(BoolVal)(true)));
    CHECK_FALSE(Step::interp_by_steps(Parse::parse_str("2 == 3"))->equals(NEW(BoolVal)(true)));

    CHECK(Step::interp_by_steps(Parse::parse_str("_if _true _then 1 _else 2"))->equals(NEW(NumVal)(1)));
    CHECK(Step::interp_by_steps(Parse::parse_str("_if _false _then 1 _else 2"))->equals(NEW(NumVal)(2)));

    CHECK(Step::interp_by_steps(Parse::parse_str("_let f = (_fun (x) (x*x))"
                                                 "_in f(2)"))->equals(NEW(NumVal)(4)));
//TODO check DONE throws error
    CHECK_THROWS(Cont::done->step_continue());

    //TODO: rightThenEqCont
    std::string string1 = "(_let x=5 _in (x+1))";

    CHECK_FALSE(addEqExpr2->equals(addEqExpr3));
    CHECK(Step::interp_by_steps(Parse::parse_str("2 == 2"))->equals(NEW(BoolVal)(true)));



}