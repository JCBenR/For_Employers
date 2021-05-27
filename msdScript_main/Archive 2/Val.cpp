//
// Created by Justin Siebenhaar on 2/23/21.
//

#include "Val.h"
//#include "expr.h"
#include "catch.h"
#include "Env.h"
//#include "Pointers.h"
//#include "Cont.h"
#include "Step.h"

//expression is something we can parse and pass to interp
//value is what interp gives us. expressions have values

/**
 * numerical value
 * @param rep the integer of that value
 */
NumVal::NumVal(int rep) {
    this->rep = rep;
}

/**
 * will provide a literal number of addition of two numbers or expressions
 * @param other_val other value to add with
 * @return NumVal of the two numbers added.
 */
PTR(Val)NumVal::add_to(PTR(Val)other_val) {
    PTR(NumVal)other_num = CAST(NumVal)(other_val);
    if (other_num == nullptr) throw std::runtime_error("add of non-number");
    double finalAnswer = (double)rep + (double)other_num->rep;
    if (finalAnswer > UINT32_MAX)
        throw std::runtime_error("number is larger than max value");
    PTR(NumVal)checkedFinalAnswer = NEW(NumVal)(rep + other_num->rep);
    return checkedFinalAnswer;
}

/**
 * will provide a literal number of multiplication of two numbers or expressions
 * @param other_val other value to multiply with
 * @return NumVal of the two numbers multiplied.
 */
PTR(Val)NumVal::mult_to(PTR(Val)other_val) {
    PTR(NumVal)other_num = CAST(NumVal)(other_val);
    if (other_num == nullptr) throw std::runtime_error("add of non-number");
    double finalAnswer = (double)rep * (double)other_num->rep;
    if (finalAnswer > UINT32_MAX)
        throw std::runtime_error("number is larger than max value");
    PTR(NumVal)checkedFinalAnswer = NEW(NumVal)(rep * other_num->rep);
    return checkedFinalAnswer;
}

/**
 * the boolean representation of a equality between two values
 * @param e value to compare with
 * @return boolean of comparison
 */
bool NumVal::equals(PTR(Val) e){
    PTR(NumVal)other_num = CAST(NumVal)(e);
    if(other_num == nullptr)
        return false;
    else
        return(this->rep == other_num->rep);
}

/**
 * method to verify if the boolean value is true. NumVal cannot have boolean value so will always return an error
 * @return runtime error
 */
bool NumVal::is_true() {
    throw std::runtime_error("numbers can't be entered for this field");
}

/**
 * used to call a function. not possible on NumVal; will throw error
 * @param actual_arg argument to call
 * @return runtime error
 */
PTR(Val)NumVal::call(PTR(Val)actual_arg){
    throw std::runtime_error("cannot use call on NumVal");
};

/**
 * takes val and return string version (used for console printing)
 * @return string of Val
 */
std::string NumVal::get_string(){
    return std::to_string(rep);
}

/**
 * similar to call (above). not valid on NumVal; will throw error
 * @param actual_arg_val argument to call next
 * @param rest rest of equation to evaluate
 */
void NumVal::call_step(Val *actual_arg_val, Cont *rest) {
    throw std::runtime_error("cannot use call_step on NumVal");
}

/**
 * boolean values True or False
 * @param rep expr value to interpret
 */
BoolVal::BoolVal(bool rep) {
    this->rep = rep;
}

/**
 * not possible for BoolVal, will throw error
 * @param other_val value to add with
 * @return runtime error
 */
PTR(Val)BoolVal::add_to(PTR(Val)other_val) {
    throw std::runtime_error("add of non-number");
}

/**
 * not possible for BoolVal, will throw error
 * @param other_val value to multiply with
 * @return runtime error
 */
PTR(Val)BoolVal::mult_to(PTR(Val)other_val) {
    throw std::runtime_error("mult of non-number");
};

/**
 * comparison of two values.
 * @param e other value to compare with
 * @return will return true if both match, otherwise false.
 */
bool BoolVal::equals(PTR(Val) e){
    PTR(BoolVal)other_bool = CAST(BoolVal)(e);
    if (other_bool == nullptr)
        return false;
    else
        return this->rep == other_bool->rep; //not sure on this
}

/**
 *helper method to check whether value is true or not.
 * @return true or false. if is true, will return true, else false.
 */
bool BoolVal::is_true(){
    return rep;
}

/**
 * cannot be used for BoolVal; will throw error
 * @param actual_arg argument to call
 * @return runtime error
 */
PTR(Val)BoolVal::call(PTR(Val)actual_arg){
    throw std::runtime_error("cannot use call on BoolVal");
};

/**
 * returns literal string representation of boolean value
 * @return string
 */
std::string BoolVal::get_string(){
    if (rep){
        return "_true";
    }
    return "_false";
}

/**
 * not possible on BoolVal, will throw error
 * @param actual_arg_val arugment to call
 * @param rest rest of equation
 */
void BoolVal::call_step(Val *actual_arg_val, Cont *rest) {
    throw std::runtime_error("cannot use call_step on BoolVal");
}

/*FUNCTION VALS*/
/**
 *
 * @param formal_arg arguments to be passed
 * @param body on which to make substitutions with arguments
 * @param env rest of environment for larger equations
 */
FunVal::FunVal(std::string formal_arg, PTR(Expr)body, PTR(Env) env) {
    this->formal_arg = formal_arg;
    this->body = body;
    this->env = env;
}

/**
 * not possible on FunVal; will throw error
 * @param other_val to add to
 * @return runtime error
 */
PTR(Val) FunVal::add_to(PTR(Val)other_val) {
    throw std::runtime_error("add of non-number");
}

/**
 * not possible on FunVal; will throw error
 * @param other_val to multiply to
 * @return runtime error
 */
PTR(Val) FunVal::mult_to(PTR(Val)other_val) {
    throw std::runtime_error("mult of non-number");
}

/**
 * checks for equality of two values.
 * @param e other value to compare
 * @return boolean
 */
bool FunVal::equals(PTR(Val)e) {
    PTR(FunVal)other_fun = CAST(FunVal)(e);
    if (other_fun == nullptr)
        return false;
    else
        return this->formal_arg == other_fun->formal_arg && this->body == other_fun->body;
}

/**
 * not possible on FunVal; will throw error
 * @return runtime error
 */
bool FunVal::is_true() {
    throw std::runtime_error("numbers can't be entered for this field");
}

/**
 * used to call functions
 * @param actual_arg arguments which formal args will be substituted with
 * @return value of evaluated function
 */
PTR(Val)FunVal::call(PTR(Val)actual_arg){
    return body->interp(NEW(ExtendedEnv)(formal_arg, actual_arg, env));
};

/**
 * for use with printing
 * @return string version of function
 */
std::string FunVal::get_string(){
    return "[function]";
}

/**
 * similar to call (above).
 * @param actual_arg_val arguments which formal args will be substituted with
 * @param rest rest of equation for evaluation
 */
void FunVal::call_step(PTR(Val) actual_arg_val, PTR(Cont) rest){
    Step::mode = Step::interp_mode;
    Step::expr = body;
    Step::env = NEW(ExtendedEnv)(formal_arg, actual_arg_val, env);
    Step::cont = rest;
}

TEST_CASE("funVal Call"){
    PTR(Env) emp = Env::empty;
    PTR(AddExpr)add1 = NEW(AddExpr)(NEW(VariableExpr)("x"), NEW(NumExpr)(1));
    PTR(FunVal)fun1 = NEW(FunVal)("x", add1, Env::empty);
    PTR(FunVal)fun2 = NEW(FunVal)("x", add1, Env::empty);
    PTR(FunVal)fun3 = NEW(FunVal)("z", add1, Env::empty);
    PTR(FunExpr)funExpr1 = NEW(FunExpr)("x", add1);

    PTR(Val)ten = NEW(NumVal)(10);
    CHECK(fun1->call(ten)->equals(NEW(NumVal)(11)));
}



TEST_CASE("FunVal is_true, mult, add_to"){
    PTR(AddExpr)add1 = NEW(AddExpr)(NEW(VariableExpr)("x"), NEW(NumExpr)(1));
    PTR(FunVal)fun1 = NEW(FunVal)("x", add1, Env::empty);
    PTR(FunVal)fun2 = NEW(FunVal)("x", add1, Env::empty);

    CHECK_THROWS(fun1->is_true());
    CHECK_THROWS(fun1->add_to(fun2));
    CHECK_THROWS(fun1->mult_to(fun2));

}

TEST_CASE("FunVal equals"){

    std::string x = "x";
    PTR(AddExpr)add1 = NEW(AddExpr)(NEW(VariableExpr)("x"), NEW(NumExpr)(1));
    PTR(FunVal)fun1 = NEW(FunVal)("x", add1, Env::empty);
    PTR(FunVal)fun2 = NEW(FunVal)("x", add1, Env::empty);
    PTR(FunVal)fun3 = NEW(FunVal)("z", add1, Env::empty);

    CHECK(fun1->equals(fun2));
    CHECK_FALSE(fun1->equals(fun3));

}


TEST_CASE("bool equals"){
    PTR(Val) boolOne = NEW(BoolVal)(true);
    PTR(Val) boolTwo = NEW(BoolVal)(true);
    PTR(Val) boolThree = NEW(BoolVal)(false);
    PTR(Val) boolFour = NEW(BoolVal)(false);


    CHECK(boolOne->equals(boolTwo));
    CHECK(boolThree->equals(boolFour));
    CHECK_FALSE(boolTwo->equals(boolThree));
}

TEST_CASE("bool Add"){
    PTR(Val) boolOne = NEW(BoolVal)(true);
    PTR(Val) boolTwo = NEW(BoolVal)(true);
    PTR(Val) boolThree = NEW(BoolVal)(false);

    CHECK_THROWS(boolThree->add_to(boolTwo));
}

TEST_CASE("bool mult"){
    PTR(Val) boolOne = NEW(BoolVal)(true);
    PTR(Val) boolTwo = NEW(BoolVal)(true);
    PTR(Val) boolThree = NEW(BoolVal)(false);

    CHECK_THROWS(boolThree->mult_to(boolTwo));
}

TEST_CASE("NumVal add_to"){
    PTR(Val) numberOne = NEW(NumVal)(1);
    PTR(Val) numberTwo = NEW(NumVal)(2);
    PTR(Val) numberThree = NEW(NumVal)(3);
    PTR(Val) numberFour = NEW(NumVal)(4);
    PTR(Val) numberFive = NEW(NumVal)(5);
    PTR(Val) negativeThree = NEW(NumVal)(-3);

    CHECK(numberOne->add_to(numberTwo)->equals(numberThree));
    CHECK(numberTwo->add_to(numberThree)->equals(numberFive));
    CHECK(numberOne->add_to(numberThree)->equals(numberFour));
    CHECK(numberFive->add_to(negativeThree)->equals(numberTwo));
}


TEST_CASE("mult_to"){
    PTR(Val) numberOne = NEW(NumVal)(1);
    PTR(Val) numberTwo = NEW(NumVal)(2);
    PTR(Val) numberThree = NEW(NumVal)(3);
    PTR(Val) numberFive = NEW(NumVal)(5);
    PTR(Val) numberSix = NEW(NumVal)(6);
    PTR(Val) negativeFifteen = NEW(NumVal)(-15);
    PTR(Val) negativeThree = NEW(NumVal)(-3);

    CHECK(numberOne->mult_to(numberTwo)->equals(numberTwo));
    CHECK(numberTwo->mult_to(numberThree)->equals(numberSix));
    CHECK(numberOne->mult_to(numberThree)->equals(numberThree));
    CHECK(numberFive->mult_to(negativeThree)->equals(negativeFifteen));

}

TEST_CASE("num and bool call"){
    PTR(Val)bool1 = NEW(BoolVal)(true);
    PTR(Val)num1 = NEW(NumVal)(10);
    CHECK_THROWS(bool1->call(num1));
    CHECK_THROWS(num1->call(bool1));
    CHECK_THROWS(num1->is_true());
}


