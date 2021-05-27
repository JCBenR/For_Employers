//
// Created by Justin Siebenhaar on 3/10/21.
//

#include "expr.h"
#include "catch.h"
#include "Parse.h"
#include "Val.h"
#include "Pointers.h"
#include "Env.h"
#include "Step.h"
#include "Cont.h"
#include <stdexcept>
#include <iostream>
#include <string>
#include <sstream>

/**
 * function expressions.
 * @param formal_arg the variable to represent the function
 * @param body the expression to be called by that value
 */
FunExpr::FunExpr(std::string formal_arg, PTR(Expr)body){
    this->formal_arg = formal_arg;
    this->body = body;
}

/**
 * checks to see if expression is equal to literal expression passed.
 * @param e expression to compare against
 * @return boolean value of whether expressions are teh same or not.
 */
bool FunExpr::equals(PTR(Expr)e) {
    PTR(FunExpr)i = CAST(FunExpr)(e);
    if(i == nullptr)
        return false;
    else
        return(this->formal_arg == i->formal_arg && this->body->equals(i->body));
}

/**
 * interprets the FunExpr to return the Val of that expression.
 * @param env environment to be passed, for use with more complex expressions.
 * @return Val value of expression when interpreted. could be NumVal, BoolVal or FunVal.
 */
PTR(Val) FunExpr::interp(PTR(Env) env) {
    return NEW(FunVal)(formal_arg, body, env);
}

/**
 * a stepped version of interp using stack memory, instead of heap to avoid overflow memory errors.
 */
void FunExpr::step_interp() {
    Step::mode = Step::continue_mode;
    Step::val = NEW(FunVal)(formal_arg, body, Step::env);
    Step::cont = Step::cont;
}

/**
 * DEPRECIATED: for use to see if the expression has a variable. In cases of FunExpr, would check to see if the expr contained a variable. for use in calling subst (also Depreciated)
 * @return boolean if the expr contained a variable
 */
bool FunExpr::has_variable() {
    return true;
}

/**
 * method for printing out the AddExpr literal representation of the expr.
 * @param out output stream to use.
 */
void FunExpr::print(std::ostream &out) {
    out << "(_fun ";
    out << "(" << formal_arg << ") ";
    out << body->to_string();
    out << ")";
}

/**
 * DEPRECIATED cleaned up version of the print method that uses excess parenthesis and spaces.
 * @param out output stream to use
 * @param mode which continuation mode to use
 * @param newLine interger value of where newLine starts, for determining pretty_print spacing.
 * @param parent expression this has come from, for determining pretty_print spacing.
 */
void FunExpr::pretty_print_at(std::ostream &out, print_mode_t mode, int newLine, came_from parent) {}

TEST_CASE("FUNEXPR"){
    PTR(Expr)add1 = NEW(AddExpr)(NEW(VariableExpr)("x"), NEW(NumExpr)(10));
    PTR(Expr)add2 = NEW(AddExpr)(NEW(VariableExpr)("x"), NEW(NumExpr)(5));

    PTR(Expr)fun1 = NEW(FunExpr)("x", add1);
    PTR(Expr)fun2 = NEW(FunExpr)("x", add1);
    PTR(Expr)fun3 = NEW(FunExpr)("x", add2);
    PTR(Expr)let1 = NEW(LetExpr)(NEW(VariableExpr)("f"), fun1, NEW(CallExpr)(NEW(VariableExpr)("f"), NEW(NumExpr)(5)));

    CHECK(fun1->equals(fun2));
    CHECK_FALSE(fun1->equals(fun3));
    CHECK(fun1->has_variable());
    CHECK(let1->interp(Env::empty)->equals(NEW(NumVal)(15)));
}

TEST_CASE("FunExpr has_variable"){
    PTR(Expr)add1 = NEW(AddExpr)(NEW(VariableExpr)("x"), NEW(NumExpr)(10));
    PTR(Expr)add2 = NEW(AddExpr)(NEW(VariableExpr)("x"), NEW(NumExpr)(5));

    PTR(Expr)fun1 = NEW(FunExpr)("x", add1);
    PTR(Expr)fun2 = NEW(FunExpr)("x", add2);
    PTR(Val)fun1val = NEW(FunVal)("x", add1,Env::empty);
    CHECK(fun1->has_variable());
}

TEST_CASE("FunExpr interp"){
    PTR(Expr)add1 = NEW(AddExpr)(NEW(VariableExpr)("x"), NEW(NumExpr)(10));
    PTR(Expr)add2 = NEW(AddExpr)(NEW(VariableExpr)("x"), NEW(NumExpr)(5));

    PTR(Expr)fun1 = NEW(FunExpr)("x", add1);
    PTR(Expr)fun2 = NEW(FunExpr)("x", add2);
    PTR(Val)fun1val = NEW(FunVal)("x", add1, Env::empty);
    CHECK(fun1->interp(Env::empty)->equals(fun1val));
    CHECK_FALSE(fun2->interp(Env::empty)->equals(fun1val));
}

TEST_CASE("print & pretty print"){
    PTR(Expr)add1 = NEW(AddExpr)(NEW(VariableExpr)("x"), NEW(NumExpr)(10));
    PTR(Expr)fun1 = NEW(FunExpr)("x", add1);
    PTR(Expr)call1 = NEW(CallExpr)(fun1, NEW(NumExpr)(10));

    std::string fun1print = "(_fun (x) (x+10))";

    std::string letFunPrint = "(_let f = _fun(x) x + 10 _in f(10))";
    PTR(Expr)letFun1 = NEW(LetExpr)(NEW(VariableExpr)("f"), fun1, call1);



//    std::cout << "**************"<< std::endl;
//    std::cout << fun1->to_string() << std::endl;
    CHECK(fun1->to_string() == fun1print);
//    CHECK(letFun1->to_string() == letFunPrint);
}

/*====================================CALLEXPR====================================*/

/**
 * calls a function that has already been created.
 * @param to_be_called function to be called
 * @param actual_arg function variables to be passed
 */
CallExpr::CallExpr(PTR(Expr)to_be_called, PTR(Expr)actual_arg) {
    this->to_be_called = to_be_called;
    this->actual_arg = actual_arg;
}

/**
 * checks for equality of expr
 * @param e expr for comparison
 * @return boolean of equality
 */
bool CallExpr::equals(PTR(Expr)e) {
    PTR(CallExpr)i = CAST(CallExpr)(e);
    if(i == nullptr) {
        return false;
    }
    else
        return(this->to_be_called->equals(i->to_be_called) && this->actual_arg->equals(i->actual_arg)); //using ->equals() because these are both Expr.
}

/**
 * interprets the function passed and return a value of the interpreted and evaluated expression
 * @param env environment for larger expressions
 * @return Val of evaluated expression
 */
PTR(Val) CallExpr::interp(PTR(Env) env) {
    return to_be_called->interp(env)->call(actual_arg->interp(env));
}

/**
 * a stepped version of interp using stack memory, instead of heap to avoid overflow memory errors.
 */
void CallExpr::step_interp() {
    Step::mode = Step::interp_mode;
    Step::expr = to_be_called;
    Step::cont = NEW(ArgThenCallCont)(actual_arg, Step::env, Step::cont);
}

/**
 * DEPRECIATED: for use to see if the expression has a variable. In cases of CallExpr, would check to see if the expr contained a variable. for use in calling subst (also Depreciated)
 * @return boolean if the expr contained a variable
 */
bool CallExpr::has_variable() {
    return true;
}

/**
 * method for printing out the AddExpr literal representation of the expr.
 * @param out output stream to use.
 */
void CallExpr::print(std::ostream &out) {
    to_be_called->print(out);
    out << "(";
    actual_arg->print(out);
    out << ")";
}

/**
 * DEPRECIATED cleaned up version of the print method that uses excess parenthesis and spaces.
 * @param out output stream to use
 * @param mode which continuation mode to use
 * @param newLine interger value of where newLine starts, for determining pretty_print spacing.
 * @param parent expression this has come from, for determining pretty_print spacing.
 */
void CallExpr::pretty_print_at(std::ostream &out, print_mode_t mode, int newLine, came_from parent) {}

TEST_CASE("Call Expr equals"){
    PTR(Expr)add1 = NEW(AddExpr)(NEW(VariableExpr)("x"), NEW(NumExpr)(10));
    PTR(Expr)add2 = NEW(AddExpr)(NEW(VariableExpr)("x"), NEW(NumExpr)(5));

    PTR(Expr)fun1 = NEW(FunExpr)("x", add1);
    PTR(Expr)fun3 = NEW(FunExpr)("x", add2);
    PTR(Expr)call1 = NEW(CallExpr)(fun1, NEW(NumExpr)(10));
    PTR(Expr)call2 = NEW(CallExpr)(fun1, NEW(NumExpr)(10));
    PTR(Expr)call3 = NEW(CallExpr)(fun3, NEW(NumExpr)(10));

    CHECK(call1->equals(call2));
    CHECK_FALSE(call1->equals(call3));
}

TEST_CASE("CallExpr interp"){
    PTR(Expr)add1 = NEW(AddExpr)(NEW(VariableExpr)("x"), NEW(NumExpr)(10));
    PTR(Expr)add2 = NEW(AddExpr)(NEW(VariableExpr)("x"), NEW(NumExpr)(5));
    PTR(Expr)mult1 = NEW(MultExpr)(NEW(VariableExpr)("z"), NEW(NumExpr)(2));
    PTR(Expr)mult2 = NEW(MultExpr)(NEW(VariableExpr)("z"), NEW(NumExpr)(5));

    PTR(Expr)fun1 = NEW(FunExpr)("x", add1);
    PTR(Expr)fun3 = NEW(FunExpr)("x", add2);
    PTR(Expr)fun4Mult = NEW(FunExpr)("z", mult1);
    PTR(Expr)fun5Mult = NEW(FunExpr)("z", mult2);
    PTR(Expr)call1 = NEW(CallExpr)(fun1, NEW(NumExpr)(10));
    PTR(Expr)call2 = NEW(CallExpr)(fun1, NEW(NumExpr)(10));
    PTR(Expr)call3 = NEW(CallExpr)(fun3, NEW(NumExpr)(10));
    PTR(Expr)call4 = NEW(CallExpr)(fun4Mult, NEW(NumExpr)(100));
    PTR(Expr)call5 = NEW(CallExpr)(fun5Mult, NEW(NumExpr)(250));

    PTR(Val)call1Val = call1->interp(Env::empty);
    PTR(Val)call3Val = call3->interp(Env::empty);
    PTR(Val)call4Val = call4->interp(Env::empty);
    PTR(Val)call5Val = call5->interp(Env::empty);

    CHECK(call1Val->equals(NEW(NumVal)(20)));
    CHECK(call3Val->equals(NEW(NumVal)(15)));
    CHECK(call4Val->equals(NEW(NumVal)(200)));
    CHECK(call5Val->equals(NEW(NumVal)(1250)));

//    std::cout << (NEW(LetExpr)(NEW(VariableExpr)("factrl"), NEW(FunExpr)("factrl", NEW(FunExpr)("x", NEW(IfExpr)(NEW(EqExpr)(NEW(VariableExpr)("x"), NEW(NumExpr)(1)), NEW(NumExpr)(1), NEW(MultExpr)(NEW(VariableExpr)("x"), NEW(CallExpr)( NEW(CallExpr)( NEW(VariableExpr)("factrl"), NEW(VariableExpr)("factrl")),NEW(AddExpr)( NEW(VariableExpr)("x"), NEW(NumExpr)(-1))))))), NEW(CallExpr)( NEW(CallExpr)( NEW(VariableExpr)("factrl"), NEW(VariableExpr)("factrl")), NEW(NumExpr)(10))))->interp() << std::endl;

//    CHECK((NEW(LetExpr)(NEW(VariableExpr)("factrl"), NEW(FunExpr)("factrl", NEW(FunExpr)("x", NEW(IfExpr)(NEW(EqExpr)(NEW(VariableExpr)("x"), NEW(NumExpr)(1)), NEW(NumExpr)(1), NEW(MultExpr)(NEW(VariableExpr)("x"), NEW(CallExpr)( NEW(CallExpr)( NEW(VariableExpr)("factrl"), NEW(VariableExpr)("factrl")),NEW(AddExpr)( NEW(VariableExpr)("x"), NEW(NumExpr)(-1))))))), NEW(CallExpr)( NEW(CallExpr)( NEW(VariableExpr)("factrl"), NEW(VariableExpr)("factrl")), NEW(NumExpr)(10))))->interp()->equals(NEW(NumVal)(3628800)));



//    PTR(Expr)factBool = NEW(BoolExpr)();
//    PTR(Expr)factEq = NEW(EqExpr)(NEW(VariableExpr)("x"), NEW(NumExpr)(1));
//    PTR(Expr)funcElseNest = NEW(CallExpr)()
//    PTR(Expr)factElse = NEW(MultExpr)(NEW(VariableExpr)("x"), )
//    PTR(Expr)factIf = NEW(IfExpr)(factEq, NEW(NumExpr)(1), )
//    PTR(Expr)factFun = NEW(FunExpr)()
//    PTR(Expr)mattsTest = NEW(LetExpr)(NEW(VariableExpr)("factrl"), )
}

TEST_CASE("callExpr hasvar"){
    PTR(Expr)add1 = NEW(AddExpr)(NEW(VariableExpr)("x"), NEW(NumExpr)(10));

    PTR(Expr)fun1 = NEW(FunExpr)("x", add1);
    PTR(Expr)call1 = NEW(CallExpr)(fun1, NEW(NumExpr)(10));

    CHECK(call1->has_variable());
}