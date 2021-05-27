//
// Created by Justin Siebenhaar on 1/21/21.
//

#include "expr.h"
#include "catch.h"
#include "Env.h"
#include "Val.h"
#include "Pointers.h"
#include "Step.h"
#include "Cont.h"
#include <stdexcept>
#include <iostream>
#include <string>
#include <sstream>

std::stringstream ss;

/**
 * this method can be used on an expr to translate it into a printable string, for use on std::cout. It creates a string stream and will call the print() method on this.
 * @return string version of Expr
 */
std::string Expr::to_string() {
    std::stringstream ss;
    (this)->print(ss);
    return ss.str();
}

/**
 * DEPRECIATED: creates a cleaned up version of the expr, with excess whitespace and parenthesis removed. Calls teh pretty_print method on the subclass. To be used in conjunction with pretty_print_string for output to the console, if necessary.
 * @param out the output stream in which to print.
 */
void Expr::pretty_print(std::ostream& out) {
    pretty_print_at(out, print_group_none, 0, not_applicable);
}

/**
 * DEPRECIATED: returns a cleaned up version. can be used in conjunction with other methods to return values of the expression or method.
 * @return string version of the Expr.
 */
std::string Expr::pretty_print_string() {
    std::stringstream ss;
    this->pretty_print(ss);
    return ss.str();
}

/*NUM*/
/**
 *
 * @param rep Numerical value.
 */
NumExpr::NumExpr(int rep) {
    this->rep = rep;
}

/**
 *
 * @param e expression for comparison
 * @return boolean of whether they are equal or not.
 */
bool NumExpr::equals(PTR(Expr)e){
    PTR(NumExpr) i = CAST(NumExpr)(e);
    if(i == nullptr)
        return false;
    else
        return(this->rep == i->rep);
}

/**
 * interprets the NumExpr to return the NumVal of that number.
 * @param env environment to be passed, for use with more complex expressions.
 * @return NumVal (rep) value.
 */
PTR(Val) NumExpr::interp(PTR(Env) env){
    return NEW(NumVal)(rep);
}

/**
 * a stepped version of interp using stack memory, instead of heap to avoid overflow memory errors.
 */
void NumExpr::step_interp() {
    Step::mode = Step::continue_mode;
    Step::val = NEW(NumVal)(rep);
    Step::cont = Step::cont;
}

/**
 * DEPRECIATED: for use to see if the expression has a variable. In cases of NumExpr, no variables are allow so will always return false.
 * @return always returns false because numbers cannot be Variables.
 */
bool NumExpr::has_variable(){
    return false;
}

/**
 * method for printing out the NumExpr rep.
 * @param out output stream to use.
 */
void NumExpr::print(std::ostream &out) {
    out << this->rep;
}

/**
 * DEPRECIATED cleaned up version of the print method that uses excess parenthesis and spaces.
 * @param out output stream to use
 * @param mode which continuation mode to use
 * @param newLine interger value of where newLine starts, for determining pretty_print spacing.
 * @param parent expression this has come from, for determining pretty_print spacing.
 */
void NumExpr::pretty_print_at(std::ostream &out, print_mode_t mode, int newLine, came_from parent) {
//    std::stringstream ss;
        out << this->rep;
}

/* ****************** */
/*ADD*/
/**
 * takes two Expr and adds them together
 * @param lhs Left hand expression
 * @param rhs Right hand expression
 */
AddExpr::AddExpr(PTR(Expr)lhs, PTR(Expr)rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

/**
 * determines if expression `e` equals this. NOTE: this is interpreting the equality of the literal expressions, not their values. Both expression must be the same to return true.
 * @param e other expression to be compared.
 * @return boolean true or false based on whether expressions are the same or not.
 */
bool AddExpr::equals(PTR(Expr)e){
    PTR(AddExpr)i = CAST(AddExpr)(e);
    if(i == nullptr)
        return false;
    else
        return(this->lhs->equals(i->lhs) && this->rhs->equals(i->rhs));
};

/**
 * interprets the AddExpr to return the NumVal of the addition of lhs + rhs.
 * @param env environment to be passed, for use with more complex expressions.
 * @return NumVal (rep) value.
 */
PTR(Val) AddExpr::interp(PTR(Env) env){
    return (this->lhs->interp(env)->add_to(this->rhs->interp(env)));
}

/**
 * a stepped version of interp using stack memory, instead of heap to avoid overflow memory errors.
 */
void AddExpr::step_interp() {
    Step::mode = Step::interp_mode;
    Step::expr = lhs;
    Step::env = Step::env;
    Step::cont = NEW(RightThenAddCont)(rhs, Step::env, Step::cont);
}

/**
 * DEPRECIATED: for use to see if the expression has a variable. In cases of AddExpr, would check to see if the expr contained a variable. for use in calling subst (also Depreciated)
 * @return boolean if the expr contained a variable
 */
bool AddExpr::has_variable(){
    return(this->lhs->has_variable() || this->rhs->has_variable());
}

/**
 * method for printing out the AddExpr literal representation of the expr.
 * @param out output stream to use.
 */
void AddExpr::print(std::ostream &out) {
    out << "(";
    this->lhs->print(out);
    out << "+";
    this->rhs->print(out);
    out << ")";
}

/**
 * DEPRECIATED cleaned up version of the print method that uses excess parenthesis and spaces.
 * @param out output stream to use
 * @param mode which continuation mode to use
 * @param newLine interger value of where newLine starts, for determining pretty_print spacing.
 * @param parent expression this has come from, for determining pretty_print spacing.
 */
void AddExpr::pretty_print_at(std::ostream& out, print_mode_t mode, int newLine, came_from parent) {
    PTR(Expr)newLhs = this->lhs;
    PTR(Expr)newRhs = this->rhs;

    bool needsParens = false;
//
    if (mode == print_group_add || mode == print_group_add_or_mult) {
        needsParens = true;
    }

//    if (mode == add_rhs || mode == mult_rhs)
//        needsParens = true;

    if (needsParens){
        out << "(";
        this->lhs->pretty_print_at(out, print_group_add, newLine, add_lhs);
        out << " + ";
        this->rhs->pretty_print_at(out, print_group_none, newLine, add_rhs);
        out << ")";
    }
    else{
        this->lhs->pretty_print_at(out, print_group_add, newLine, add_lhs);
        out << " + ";
        this->rhs->pretty_print_at(out, print_group_none, newLine, add_rhs);
    }
}


/*MULTIPLY*/
/**
 * creates a new MultExpr object
 * @param lhs Left hand expression
 * @param rhs Right hand expression
 */
MultExpr::MultExpr(PTR(Expr)lhs, PTR(Expr)rhs) {
        this->lhs = lhs;
        this->rhs = rhs;
}

/**
 * determines if expression `e` equals this. NOTE: this is interpreting the equality of the literal expressions, not their values. Both expression must be the same to return true.
 * @param e other expression to be compared.
 * @return boolean true or false based on whether expressions are the same or not.
 */
bool MultExpr::equals(PTR(Expr)e){
    PTR(MultExpr)i = CAST(MultExpr)(e);
    if(i == nullptr)
        return false;
    else
        return(this->lhs->equals(i->lhs) && this->rhs->equals(i->rhs));
};

/**
 *  interprets the MultExpr to return the NumVal of the addition of lhs * rhs.
 * @param env environment to be passed, for use with more complex expressions.
 * @return NumVal (rep) value.
 */
PTR(Val) MultExpr::interp(PTR(Env) env){
    return(this->lhs->interp(env)->mult_to(this->rhs->interp(env)));
}

/**
 * a stepped version of interp using stack memory, instead of heap to avoid overflow memory errors.
 */
void MultExpr::step_interp() {
    Step::mode = Step::interp_mode;
    Step::expr = lhs;
    Step::env = Step::env;
    Step::cont = NEW(RightThenMultCont)(rhs, Step::env, Step::cont);
}

/**
 * DEPRECIATED: for use to see if the expression has a variable. In cases of MultExpr, would check to see if the expr contained a variable. for use in calling subst (also Depreciated)
 * @return boolean if the expr contained a variable
 */
bool MultExpr::has_variable(){
    return(this->lhs->has_variable() || this->rhs->has_variable());
}

/**
 * method for printing out the MultExpr literal representation of the expr.
 * @param out output stream to use.
 */
void MultExpr::print(std::ostream &out) {
    out << "(";
    this->lhs->print(out);
    out << "*";
    this->rhs->print(out);
    out << ")";
}

/**
 * DEPRECIATED cleaned up version of the print method that uses excess parenthesis and spaces.
 * @param out output stream to use
 * @param mode which continuation mode to use
 * @param newLine interger value of where newLine starts, for determining pretty_print spacing.
 * @param parent expression this has come from, for determining pretty_print spacing.
 */
void MultExpr::pretty_print_at(std::ostream &out, print_mode_t mode, int newLine, came_from parent) {
    PTR(Expr)newLhs = this->lhs;
    PTR(Expr)newRhs = this->rhs;

    bool needsParens = false;

    if (mode == print_group_add_or_mult) {
        needsParens = true;
    }

    if (needsParens){
        out << "(";
        this->lhs->pretty_print_at(out, print_group_add_or_mult, newLine, mult_lhs);
        out << " * ";
        this->rhs->pretty_print_at(out, print_group_add, newLine, mult_rhs);
        out << ")";
    }
    else{
        this->lhs->pretty_print_at(out, print_group_add_or_mult, newLine, mult_lhs);
        out << " * ";
        this->rhs->pretty_print_at(out, print_group_add, newLine, mult_rhs);
    }
}

/*VARIABLE*/
/**
 * Variable string, used to represent an expression
 * @param val the string value to use for the variable.
 */
VariableExpr::VariableExpr(std::string val) {
    this->val = val;
}

/**
 * determines if expression `e` equals this. NOTE: this is interpreting the equality of the literal expressions, not their values. Both expression must be the same to return true.
 * @param e other expression to be compared.
 * @return boolean true or false based on whether expressions are the same or not.
 */
bool VariableExpr::equals(PTR(Expr)e) {
    PTR(VariableExpr)i = CAST(VariableExpr)(e);
    if (i == nullptr)
        return false;
    else
        return(this->val == i->val);
};

/**
 * looks up the NumVal of the variable in library.
 * @param env environment to be passed, for use with more complex expressions.
 * @return NumVal (rep) value.
 */
PTR(Val) VariableExpr::interp(PTR(Env) env){
    return env->lookup(val);
}

/**
 * a stepped version of interp using stack memory, instead of heap to avoid overflow memory errors.
 */
void VariableExpr::step_interp() {
    Step::mode = Step::continue_mode;
    Step::val = Step::env->lookup(val);
    Step::cont = Step::cont;
}

/**
 * DEPRECIATED: for use to see if the expression has a variable. In cases of VariableExpr, would check to see if the expr contained a variable (which it always will). for use in calling subst (also Depreciated)
 * @return boolean True in all cases.
 */
bool VariableExpr::has_variable(){
    return true;
}

/**
 * method for printing out the AddExpr literal representation of the expr.
 * @param out output stream to use.
 */
void VariableExpr::print(std::ostream &out) {
    out << this->val;
}

/**
 * DEPRECIATED cleaned up version of the print method that uses excess parenthesis and spaces.
 * @param out output stream to use
 * @param mode which continuation mode to use
 * @param newLine interger value of where newLine starts, for determining pretty_print spacing.
 * @param parent expression this has come from, for determining pretty_print spacing.
 */
void VariableExpr::pretty_print_at(std::ostream &out, print_mode_t mode, int newLine, came_from parent) {
//    std::stringstream ss;
    out << this->val;
}


/*LET*/
/**
 * provides an expression with variable and it's expr representation (could be a NumExpr or a full Expr). a way of assigning a variable to an expr.
 * @param lhs variable to represent the expr
 * @param rhs the expr itself
 * @param body expression where the variable will be substituted with the rhs expr
 */
LetExpr::LetExpr(PTR(VariableExpr)lhs, PTR(Expr)rhs, PTR(Expr)body){
    this->lhs = lhs;
    this->rhs = rhs;
    this->body = body;
}

/**
 * determines if expression `e` equals this. NOTE: this is interpreting the equality of the literal expressions, not their values. Both expression must be the same to return true.
 * @param e other expression to be compared.
 * @return boolean true or false based on whether expressions are the same or not.
 */
bool LetExpr::equals(PTR(Expr)e) {
    PTR(LetExpr)i = CAST(LetExpr)(e);
    if(i == nullptr)
        return false;
    else
        return(this->lhs->equals(i->lhs) && this->rhs->equals(i->rhs) && this->body->equals(i->body));
}

/**
 * interprets the LetExpr to return the NumVal of the variable and make the appropriate substitution in the expression body.
 * @param env environment to be passed, for use with more complex expressions.
 * @return NumVal (rep) value.
 */
PTR(Val) LetExpr::interp(PTR(Env) env){
    PTR(Val) rhs_val = rhs->interp(env);
    PTR(Env) new_env = NEW(ExtendedEnv)(lhs->to_string(), rhs_val, env);
    return body->interp(new_env);
}

/**
 * a stepped version of interp using stack memory, instead of heap to avoid overflow memory errors.
 */
void LetExpr::step_interp() {
    Step::mode = Step::interp_mode;
    Step::expr = rhs;
    Step::env = Step::env;
    Step::cont = NEW(LetBodyCont) (lhs->to_string(), body, Step::env, Step::cont);
}

/**
 * DEPRECIATED: for use to see if the expression has a variable. In cases of LetExpr, would check to see if the expr contained a variable. for use in calling subst (also Depreciated)
 * @return boolean if the expr contained a variable
 */
bool LetExpr::has_variable(){
    return(this->rhs->has_variable() || this->body->has_variable());
}

//subst job is not to interpret a let
//never replace lhs
//always produces a LetExpr
//always occurs with the subs on the rhs
//always replace on the rhs
//don't go in body if lhs of subst matches lhs of let

/**
 * method for printing out the AddExpr literal representation of the expr.
 * @param out output stream to use.
 */
void LetExpr::print(std::ostream& out){
    out << "(_let " << lhs->to_string()<<"="<<rhs->to_string()<< " _in " <<body->to_string()<<")";
};

/**
 * DEPRECIATED cleaned up version of the print method that uses excess parenthesis and spaces.
 * @param out output stream to use
 * @param mode which continuation mode to use
 * @param newLine interger value of where newLine starts, for determining pretty_print spacing.
 * @param parent expression this has come from, for determining pretty_print spacing.
 */
void LetExpr::pretty_print_at(std::ostream& out, print_mode_t mode, int newLine, came_from parent){
int whereNewLineIs1 = newLine;//assign newLine to local variable for use within function

    //if add or add/mult it needs parens
    //if its a let, it needs to be shifted over correctly
    bool needsParens = false;

    if ((mode == print_group_add_or_mult || mode == print_group_add) && (parent == add_lhs || parent == mult_lhs)) {
        needsParens = true;
    }

    /**
     * using accumulator, is able to tell how many spaces to move the cursor to line up _let and _in.
     * whereLineIs1 is passed through (the accumulator)
     * whereLetIs, marks where the let is
     * whereNewLineIs2 marks the new line between _let and _in. it's needed to capture this before calculating spaces to move because it will correspond to the let that may be nested inside the _in statement. If no new _let is nested, it is still passed in case a _let is inside a AddExpr/MultExpr.
     * then calculate number of spaces to move
     * loop through spacesToMove providing a space each time
     * assign whereNewLineIs2 to whereNewLineIs1 and pass that down to the body.
     */
    int whereLetIs, whereNewLineIs2, spacesToMove; //new local variables

    if (needsParens) {
        out << "(";
    }

        whereLetIs = out.tellp(); //mark where let is
        out << "_let ";
        lhs->pretty_print_at(out, nested_let, whereNewLineIs1, let_lhs);
        out << " = ";
        rhs->pretty_print_at(out, nested_let, whereNewLineIs1, let_rhs);
        out << std::endl;
        whereNewLineIs2 = out.tellp();//mark where second new line is
        spacesToMove = whereLetIs - whereNewLineIs1; //subtract to get number of spaces to move
        for (int i = 0; i < spacesToMove; ++i) { //move spaces
            out << " ";
        }
        whereNewLineIs1 = whereNewLineIs2; //assign second line to first line
        out << "_in  ";
        body->pretty_print_at(out, nested_let, whereNewLineIs1, not_applicable);

    if (needsParens) { out << ")"; }
};


/*BOOLEXPR*/
/**
 * an expression of true or false
 * @param rep true or false.
 */
BoolExpr::BoolExpr(bool rep){
    this->rep = rep;
}

/**
 * determines if expression `e` equals this. NOTE: this is interpreting the equality of the literal expressions, not their values. Both expression must be the same to return true.
 * @param e other expression to be compared.
 * @return boolean true or false based on whether expressions are the same or not.
 */
bool BoolExpr::equals(PTR(Expr)e) {
    PTR(BoolExpr)i = CAST(BoolExpr)(e);
    if(i == nullptr)
        return false;
    else
        return((this->rep) == (i->rep));
}

/**
 * interprets the BoolExpr to return the BoolVal of the equality of two values.
 * @param env environment to be passed, for use with more complex expressions.
 * @return BoolVal of equality.
 */
PTR(Val) BoolExpr::interp(PTR(Env) env) {
    return NEW(BoolVal)(this->rep);
}

/**
 * a stepped version of interp using stack memory, instead of heap to avoid overflow memory errors.
 */
void BoolExpr::step_interp() {
    Step::mode = Step::continue_mode;
    Step::val = NEW(BoolVal)(rep);
    Step::cont = Step::cont;
}

/**
 * DEPRECIATED: for use to see if the expression has a variable. In cases of BoolExpr, would check to see if the expr contained a variable (which it never will). for use in calling subst (also Depreciated)
 * @return boolean False in all cases.
 */
bool BoolExpr::has_variable() {
    return false;
}

/**
 * method for printing out the AddExpr literal representation of the expr.
 * @param out output stream to use.
 */
void BoolExpr::print(std::ostream &out) {
    std::string answer;
    answer = this->rep ? "_true" : "_false";
    out << answer;
}

/**
 * DEPRECIATED cleaned up version of the print method that uses excess parenthesis and spaces.
 * @param out output stream to use
 * @param mode which continuation mode to use
 * @param newLine interger value of where newLine starts, for determining pretty_print spacing.
 * @param parent expression this has come from, for determining pretty_print spacing.
 */
void BoolExpr::pretty_print_at(std::ostream &out, print_mode_t mode, int newLine, came_from parent) {
    std::string answer;
    answer = this->rep ? "_true" : "_false";
    out << answer;
};


/*EQEXPR*/
/**
 * compares two expressions for equality. note this is NOT comparing equality of results (Val), it is comparing equality of the expressions themselves.
 * @param lhs expr 1 to compare
 * @param rhs expr 2 to compare
 */
EqExpr::EqExpr(PTR(Expr)lhs, PTR(Expr)rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

//so an equality expression has an expression (like an Add) and it's job is to compare that to another expression (like a simple number, which is also an expression) and see if, when both are interpreted, they come out to the same thing. so the constructor of the equality takes an expression. the equals method, is comparing that expressions interpreted value to another expressions interpreted value and then returning true/false.

/**
 * determines if expression `e` equals this. NOTE: this is interpreting the equality of the literal expressions, not their values. Both expression must be the same to return true.
 * @param e other expression to be compared.
 * @return boolean true or false based on whether expressions are the same or not.
 */
bool EqExpr::equals(PTR(Expr)e) {
    PTR(EqExpr)other = CAST(EqExpr)(e);
    if (other == nullptr){
        std::cout << "types not comparable" << std::endl;
        return false;
    } else
        return (this->lhs->equals(other->lhs) && this->rhs->equals(other->rhs));
}

/**
 * interprets the BoolExpr to return the BoolVal of the equality of two expressions.
 * @param env for use with more complex expressions
 * @return BoolVal of equality of expressions
 */
PTR(Val) EqExpr::interp(PTR(Env) env) {
    if (this->lhs->interp(env)->equals(this->rhs->interp(Env::empty)))
        return NEW(BoolVal)(true);
    else
        return NEW(BoolVal)(false);
}

/**
 * a stepped version of interp using stack memory, instead of heap to avoid overflow memory errors.
 */
void EqExpr::step_interp() {
    Step::mode = Step::interp_mode;
    Step::expr = lhs;
    Step::env = Step::env;
    Step::cont = NEW(RightThenEqCont)(rhs, Step::env,  Step::cont);
}

/**
 * DEPRECIATED: for use to see if the expression has a variable. In cases of EqExpr, would check to see if the expr contained a variable (will always be false). for use in calling subst (also Depreciated)
 * @return boolean False in all cases.
 */
bool EqExpr::has_variable() {
    return false;
}

/**
 * method for printing out the AddExpr literal representation of the expr.
 * @param out output stream to use.
 */
void EqExpr::print(std::ostream &out) {
    std::string answer;
    out << "("<< lhs->to_string() << "==" << rhs->to_string() << ")";
}

/**
 * DEPRECIATED cleaned up version of the print method that uses excess parenthesis and spaces.
 * @param out output stream to use
 * @param mode which continuation mode to use
 * @param newLine interger value of where newLine starts, for determining pretty_print spacing.
 * @param parent expression this has come from, for determining pretty_print spacing.
 */
void EqExpr::pretty_print_at(std::ostream &out, print_mode_t mode, int newLine, came_from parent) {
    std::string answer;
    if (parent == let_lhs ||  parent == add_lhs ||  parent == mult_lhs || parent == eq_lhs || parent == mult_rhs || parent == add_rhs || parent == eq_rhs){
        out << "(";
    }
    lhs->pretty_print_at(out, mode, newLine, eq_lhs);
    out << " == ";
    rhs->pretty_print_at(out, mode, newLine, eq_rhs);

    if (parent == let_lhs ||  parent == add_lhs ||  parent == mult_lhs || parent == eq_lhs || parent == mult_rhs || parent == add_rhs || parent == eq_rhs){
        out << ")";
    }
}

/*IF EXPRESSION*/

/**
 * creates a comparison between two expressions and provides a branch for either T/F.
 * @param tOrF expr to compare. will result in a boolean.
 * @param lhs path to follow upon True return from tOrF
 * @param rhs path to follow upon False return from tOrF
 */
IfExpr::IfExpr(PTR(Expr)tOrF, PTR(Expr)lhs, PTR(Expr)rhs){
    this->tOrF = tOrF;
    this->lhs = lhs;
    this->rhs = rhs;
}

/**
 * determines if expression `e` equals this. NOTE: this is interpreting the equality of the literal expressions, not their values. Both expression must be the same to return true.
 * @param env for use with more complex expressions
 * @return boolean true or false based on whether expressions are the same or not.
 */
bool IfExpr::equals(PTR(Expr)e){
    PTR(IfExpr)other = CAST(IfExpr)(e);
    if(other == nullptr)
        return false;
    else
        return(this->lhs->equals(other->lhs) && this->rhs->equals(other->rhs) && this->tOrF->equals(other->tOrF));
};

/**
 * interprets the IfExpr to return the BoolVal of the equality of two values. If true, follows the '_else' path, otherwise the '_then' path.
 * @param env
 * @return value of correct path based on comparison
 */
PTR(Val) IfExpr::interp(PTR(Env) env){
    if (tOrF->interp(env)->is_true())
        return lhs->interp(env);
    else
        return rhs->interp(env);
}; //updated from int to PTR(Val)

/**
 * a stepped version of interp using stack memory, instead of heap to avoid overflow memory errors.
 */
void IfExpr::step_interp() {
    Step::mode = Step::interp_mode;
    Step::expr = tOrF;
    Step::env = Step::env;
    Step::cont = NEW(IfBranchCont)(lhs, rhs, Step::env, Step::cont);
}

/**
 * DEPRECIATED: for use to see if the expression has a variable. In cases of IfExpr, would check to see if the expr contained a variable. for use in calling subst (also Depreciated)
 * @return boolean False in all cases.
 */
bool IfExpr::has_variable(){
    return(this->rhs->has_variable() || this->lhs->has_variable());
};

/**
 * method for printing out the AddExpr literal representation of the expr.
 * @param out output stream to use.
 */
void IfExpr::print(std::ostream& out){
    out << "("
        <<"_if " << tOrF->to_string()<< std::endl
        << "_then " << lhs->to_string()<< std::endl
        << "_else " <<rhs->to_string()
        << ")";
};

/**
 * DEPRECIATED cleaned up version of the print method that uses excess parenthesis and spaces.
 * @param out output stream to use
 * @param mode which continuation mode to use
 * @param newLine interger value of where newLine starts, for determining pretty_print spacing.
 * @param parent expression this has come from, for determining pretty_print spacing.
 */
void IfExpr::pretty_print_at(std::ostream& out, print_mode_t mode, int newLine, came_from parent){
    int whereNewLineIs1 = newLine;
    int whereIfIs, whereNewLineIs2, whereNewLineIs3, spacesToMove; //new local variables

    //steps
    //1.find where _if starts on it's line
    //2. note where new line (_then line) starts
    //3. subtract where if starts from it's line start
    //4. indent _then that many spaces
    //5. print then and result
    //6. new line
    //7. indent _else same amount of spaces
    //8. print else and result

//    out << "(";
        whereIfIs = out.tellp();
    out <<"_if " << tOrF->to_string()<< std::endl;
        whereNewLineIs2 = out.tellp();
        spacesToMove = whereIfIs -whereNewLineIs1;
    for (int i = 0; i < spacesToMove; ++i) {
        out << " ";
    }
    whereNewLineIs1 = whereNewLineIs2;
    out << "_then " << lhs->to_string();
    out << std::endl;
    for (int i = 0; i < spacesToMove; ++i) {
        out << " ";
    }
    out << "_else " << rhs->to_string();

//    out << ")";
};

//TEST_CASE("if print"){
//    PTR(Expr)boolTrue = NEW(BoolExpr)(true);
//    PTR(Expr)boolFalse = NEW(BoolExpr)(false);
//    PTR(Expr)numOne = NEW(NumExpr)(1);
//    PTR(Expr)numTwo = NEW(NumExpr)(2);
//    PTR(Expr)numThree = NEW(NumExpr)(3);
//
//    PTR(Expr)varOne = NEW(VariableExpr)("x");
//    PTR(Expr)varZ = NEW(VariableExpr)("z");
//
//    PTR(Expr)ifFirst = NEW(IfExpr)(boolTrue, varOne, numThree);
//    std::string ifTestOne = "(_if _true\n"
//                            "_then x\n"
//                            "_else 3)";
//
//    CHECK(ifFirst->to_string() == ifTestOne);
//}

//TEST_CASE("If Pretty Print"){
//    PTR(Expr)boolTrue = NEW(BoolExpr)(true);
//    PTR(Expr)boolFalse = NEW(BoolExpr)(false);
//    PTR(Expr)numOne = NEW(NumExpr)(1);
//    PTR(Expr)numTwo = NEW(NumExpr)(2);
//    PTR(Expr)numThree = NEW(NumExpr)(3);
//
//    PTR(Expr)varOne = NEW(VariableExpr)("x");
//    PTR(Expr)varZ = NEW(VariableExpr)("z");
//
//    PTR(Expr)ifFirst = NEW(IfExpr)(boolTrue, varOne, numThree);
//    PTR(Expr)ifSecond = NEW(IfExpr)(boolFalse, numOne, varZ);
//    PTR(Expr)ifFirstB = NEW(IfExpr)(boolTrue, numOne, numTwo);
//    PTR(Expr)ifSecondB = NEW(IfExpr)(boolFalse, numOne, numTwo);
//
//    PTR(Expr)ifSubst = ifFirst->subst("x", numOne);
//    PTR(Expr)ifSubst2 = ifSecond->subst("z", numTwo);
//
//    //ifFirst->pretty_print(std::cout);
//    std::string ifTestOne = "_if _true\n"
//                            "_then x\n"
//                            "_else 3";
//    CHECK(ifFirst->pretty_print_string() == ifTestOne);
//}

//TEST_CASE("If Substs") {
//    PTR(Expr)boolTrue = NEW(BoolExpr)(true);
//    PTR(Expr)boolFalse = NEW(BoolExpr)(false);
//    PTR(Expr)numOne = NEW(NumExpr)(1);
//    PTR(Expr)numTwo = NEW(NumExpr)(2);
//    PTR(Expr)numThree = NEW(NumExpr)(3);
//
//    PTR(Expr)varOne = NEW(VariableExpr)("x");
//    PTR(Expr)varZ = NEW(VariableExpr)("z");
//
//    PTR(Expr)ifFirst = NEW(IfExpr)(boolTrue, varOne, numThree);
//    PTR(Expr)ifSecond = NEW(IfExpr)(boolFalse, numOne, varZ);
//    PTR(Expr)ifFirstB = NEW(IfExpr)(boolTrue, numOne, numTwo);
//    PTR(Expr)ifSecondB = NEW(IfExpr)(boolFalse, numOne, numTwo);
//
//    PTR(Expr)ifSubst = ifFirst->subst("x", numOne);
//    PTR(Expr)ifSubst2 = ifSecond->subst("z", numTwo);
//
//    CHECK(ifSubst->interp()->equals(NEW(NumVal)(1)));
//    CHECK(ifSubst2->interp()->equals(NEW(NumVal)(2)));
//}

TEST_CASE("If has variable"){
    PTR(Expr)boolTrue = NEW(BoolExpr)(true);
    PTR(Expr)boolFalse = NEW(BoolExpr)(false);
    PTR(Expr)numOne = NEW(NumExpr)(1);
    PTR(Expr)numTwo = NEW(NumExpr)(2);
    PTR(Expr)varOne = NEW(VariableExpr)("x");

    PTR(Expr)ifFirst = NEW(IfExpr)(boolTrue, numOne, varOne);
    PTR(Expr)ifSecond = NEW(IfExpr)(boolFalse, numOne, numTwo);
    PTR(Expr)ifSecondB = NEW(IfExpr)(boolFalse, numOne, numTwo);

    CHECK(ifFirst->has_variable());
    CHECK_FALSE(ifSecondB->has_variable());
    CHECK_FALSE(ifSecond->has_variable());
}

TEST_CASE("if interp"){
    PTR(Expr)boolTrue = NEW(BoolExpr)(true);
    PTR(Expr)boolFalse = NEW(BoolExpr)(false);
    PTR(Expr)numOne = NEW(NumExpr)(1);
    PTR(Expr)numTwo = NEW(NumExpr)(2);

    PTR(Expr)ifFirst = NEW(IfExpr)(boolTrue, numOne, numTwo);
    PTR(Expr)ifSecond = NEW(IfExpr)(boolFalse, numOne, numTwo);
    PTR(Expr)ifFirstB = NEW(IfExpr)(boolTrue, numOne, numTwo);
    PTR(Expr)ifSecondB = NEW(IfExpr)(boolFalse, numOne, numTwo);

    CHECK(ifFirst->interp(Env::empty)->equals(NEW(NumVal)(1)));
    CHECK(ifSecondB->interp(Env::empty)->equals(NEW(NumVal)(2)));
    CHECK_FALSE(ifFirst->interp(Env::empty)->equals(NEW(NumVal)(2)));
    CHECK_FALSE(ifSecondB->interp(Env::empty)->equals(NEW(NumVal)(1)));

}

TEST_CASE("if equals"){
    PTR(Expr)boolTrue = NEW(BoolExpr)(true);
    PTR(Expr)boolFalse = NEW(BoolExpr)(false);
    PTR(Expr)numOne = NEW(NumExpr)(1);
    PTR(Expr)numTwo = NEW(NumExpr)(2);

    PTR(Expr)ifFirst = NEW(IfExpr)(boolTrue, numOne, numTwo);
    PTR(Expr)ifSecond = NEW(IfExpr)(boolFalse, numOne, numTwo);
    PTR(Expr)ifFirstB = NEW(IfExpr)(boolTrue, numOne, numTwo);
    PTR(Expr)ifSecondB = NEW(IfExpr)(boolFalse, numOne, numTwo);

    CHECK(ifFirst->equals(ifFirstB));
    CHECK(ifSecond->equals(ifSecondB));
    CHECK_FALSE(ifFirstB->equals(ifSecond));
}

TEST_CASE("EqExpr Pretty print"){
    PTR(Expr)addFirst = NEW(AddExpr)(NEW(NumExpr)(5), NEW(NumExpr)(1));
    PTR(Expr)addSecond = NEW(AddExpr)(NEW(NumExpr)(5), NEW(NumExpr)(1));
    PTR(Expr)multFirst = NEW(MultExpr)(NEW(NumExpr)(5), NEW(NumExpr)(5));

    PTR(Expr)ansFirst = NEW(NumExpr)(6);
    PTR(Expr)ansMult = NEW(NumExpr)(25);
    PTR(Expr)ansMultWrong = NEW(NumExpr)(24);
    PTR(Expr)addEqExpr = NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2));
    PTR(Expr)addEqExpr2 = NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2));
    PTR(Expr)addEqExpr3 = NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(3));
    PTR(Expr)addEqExpr4 = NEW(EqExpr)(addFirst, ansFirst);
    PTR(Expr)multEqExpr = NEW(EqExpr)(multFirst, ansMult);
    PTR(Expr)multEqExprWrong = NEW(EqExpr)(multFirst, ansMultWrong);
    CHECK(addEqExpr->pretty_print_string() == "1 == 2");

    PTR(Expr)addWithEq = NEW(AddExpr)(addEqExpr, ansFirst); // (1 == 2) + 6
//    addWithEq->pretty_print(std::cout);
    CHECK(addWithEq->pretty_print_string() == "(1 == 2) + 6");

    PTR(Expr)multWithEq = NEW(MultExpr)(addEqExpr, ansFirst);//(1 == 2) * 6
    CHECK(multWithEq->pretty_print_string() == "(1 == 2) * 6");

    //PTR(Expr)letExpr = NEW(LetExpr)("x",);
    std::string string1 = "(_let x=5 _in (x+1))";
    std::string string2 = "(_let x=5 _in ((_let y=3 _in (y+2))+x))";
    std::string string3 = "_let x = 5\n"
                          "_in  (_let y = 3\n"
                          "      _in  (_let z = 1\n"
                          "            _in  z + 2) + 5) + 8";
    std::string string4 = "(_let x = 5\n"
                          " _in  x) + 1";


    std::string string5 = "_let z = 1\n"
                          "_in  z + 2";

    std::string stringLet = "_let same = 1 == 2\n"
                            "_in  _if 1 == 2\n"
                            "     _then _false + 5\n"
                            "     _else 88";

    PTR(Expr)ifFalseStatement = NEW(IfExpr)(NEW(BoolExpr)(false), NEW(AddExpr)(NEW(BoolExpr)(false), NEW(NumExpr)(5)), NEW(NumExpr)(88));
    PTR(Expr)letFalseStatement = NEW(LetExpr)(NEW(VariableExpr)("same"), NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)), ifFalseStatement);

//    CHECK(letFalseStatement->pretty_print_string() == stringLet);


    PTR(Expr)eqEq = NEW(EqExpr)(addEqExpr, ansFirst);
    CHECK(eqEq->pretty_print_string() == "(1 == 2) == 6");

    PTR(Expr)eqEqDoubleAdd = NEW(EqExpr)(addEqExpr, addEqExpr);
    CHECK(eqEqDoubleAdd->pretty_print_string() == "(1 == 2) == (1 == 2)");

    //    std::cout << eqEqDoubleAdd->pretty_print_string()<< std::endl;
}

TEST_CASE("EqExpr Subst"){
    PTR(Expr)addFirst = NEW(AddExpr)(NEW(NumExpr)(5), NEW(NumExpr)(1));
    PTR(Expr)multFirst = NEW(MultExpr)(NEW(NumExpr)(5), NEW(NumExpr)(5));

    PTR(Expr)ansFirst = NEW(NumExpr)(6);
    PTR(Expr)ansMult = NEW(VariableExpr)("x");
    PTR(Expr)ansFirstX = NEW(VariableExpr)("x");
    PTR(Expr)addEqExpr4 = NEW(EqExpr)(addFirst, ansFirst);

    PTR(Expr)addEqExpr5 = NEW(EqExpr)(addFirst, ansFirstX);
}

TEST_CASE("EqExpr has variable"){
    PTR(Expr)addFirst = NEW(AddExpr)(NEW(NumExpr)(5), NEW(NumExpr)(1));
    PTR(Expr)multFirst = NEW(MultExpr)(NEW(NumExpr)(5), NEW(NumExpr)(5));

    PTR(Expr)ansFirst = NEW(NumExpr)(6);
    PTR(Expr)ansMult = NEW(NumExpr)(25);
    PTR(Expr)addEqExpr4 = NEW(EqExpr)(addFirst, ansFirst);
    PTR(Expr)multEqExpr = NEW(EqExpr)(multFirst, ansMult);

    CHECK_FALSE(addEqExpr4->has_variable());
    CHECK_FALSE(multEqExpr->has_variable());
}

TEST_CASE("EqExpr Print"){
    PTR(Expr)addFirst = NEW(AddExpr)(NEW(NumExpr)(5), NEW(NumExpr)(1));
    PTR(Expr)addSecond = NEW(AddExpr)(NEW(NumExpr)(5), NEW(NumExpr)(1));
    PTR(Expr)multFirst = NEW(MultExpr)(NEW(NumExpr)(5), NEW(NumExpr)(5));

    PTR(Expr)ansFirst = NEW(NumExpr)(6);
    PTR(Expr)ansMult = NEW(NumExpr)(25);
    PTR(Expr)ansMultWrong = NEW(NumExpr)(24);
    PTR(Expr)addEqExpr = NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2));
    PTR(Expr)addEqExpr2 = NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2));
    PTR(Expr)addEqExpr3 = NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(3));
    PTR(Expr)addEqExpr4 = NEW(EqExpr)(addFirst, ansFirst);
    PTR(Expr)multEqExpr = NEW(EqExpr)(multFirst, ansMult);
    PTR(Expr)multEqExprWrong = NEW(EqExpr)(multFirst, ansMultWrong);

    CHECK(addEqExpr4->to_string() == "((5+1)==6)");
    CHECK(multEqExpr->to_string() == "((5*5)==25)");
    CHECK((NEW(EqExpr)(NEW(NumExpr)(3), NEW(NumExpr)(3)))->to_string() == "(3==3)");
}

TEST_CASE("EqExpr interp"){
    PTR(Expr)addFirst = NEW(AddExpr)(NEW(NumExpr)(5), NEW(NumExpr)(1));
    PTR(Expr)addSecond = NEW(AddExpr)(NEW(NumExpr)(5), NEW(NumExpr)(1));
    PTR(Expr)multFirst = NEW(MultExpr)(NEW(NumExpr)(5), NEW(NumExpr)(5));


    PTR(Expr)ansFirst = NEW(NumExpr)(6);
    PTR(Expr)ansMult = NEW(NumExpr)(25);
    PTR(Expr)ansMultWrong = NEW(NumExpr)(24);
    PTR(Expr)addEqExpr = NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2));
    PTR(Expr)addEqExpr2 = NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2));
    PTR(Expr)addEqExpr3 = NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(3));
    PTR(Expr)addEqExpr4 = NEW(EqExpr)(addFirst, ansFirst);
    PTR(Expr)multEqExpr = NEW(EqExpr)(multFirst, ansMult);
    PTR(Expr)multEqExprWrong = NEW(EqExpr)(multFirst, ansMultWrong);

    CHECK(addEqExpr4->interp(Env::empty)->equals(NEW(BoolVal)(true)));
    CHECK(multEqExpr->interp(Env::empty)->equals(NEW(BoolVal)(true)));
    CHECK_FALSE(multEqExpr->interp(Env::empty)->equals(NEW(BoolVal)(false)));
    CHECK_FALSE(multEqExprWrong->interp(Env::empty)->equals((NEW(BoolVal)(true))));
    CHECK(multEqExprWrong->interp(Env::empty)->equals((NEW(BoolVal)(false))));
}

TEST_CASE("EqExpr equals"){
    PTR(Expr)addFirst = NEW(AddExpr)(NEW(NumExpr)(5), NEW(NumExpr)(1));
    PTR(Expr)addSecond = NEW(AddExpr)(NEW(NumExpr)(5), NEW(NumExpr)(1));

    PTR(Expr)ansFirst = NEW(NumExpr)(6);
    PTR(Expr)addEqExpr = NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2));
    PTR(Expr)addEqExpr2 = NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2));
    PTR(Expr)addEqExpr3 = NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(3));

    CHECK(addEqExpr->equals(addEqExpr2));
    CHECK_FALSE(addEqExpr2->equals(addEqExpr3));
}

TEST_CASE("boolExpr Subst"){
    PTR(Expr)boolOne = NEW(BoolExpr)(true);
    PTR(Expr)boolTwo = NEW(BoolExpr)(true);
    PTR(Expr)boolThree = NEW(BoolExpr)(false);
    PTR(Expr)boolFour = NEW(BoolExpr)(false);

    PTR(Val) valBoolOne = NEW(BoolVal)(true);

}

TEST_CASE("BoolExpr Print"){
    PTR(Expr)boolOne = NEW(BoolExpr)(true);
    PTR(Expr)boolTwo = NEW(BoolExpr)(true);
    PTR(Expr)boolThree = NEW(BoolExpr)(false);
    PTR(Expr)boolFour = NEW(BoolExpr)(false);

    PTR(Val) valBoolOne = NEW(BoolVal)(true);


    //need to figure out how to get this printing. look at others.
    CHECK(boolOne->to_string() == "_true");
    CHECK(boolFour->to_string() == "_false");

}

TEST_CASE("boolExpr has_variable"){
    PTR(Expr)boolOne = NEW(BoolExpr)(true);
    PTR(Expr)boolTwo = NEW(BoolExpr)(true);
    PTR(Expr)boolThree = NEW(BoolExpr)(false);
    PTR(Expr)boolFour = NEW(BoolExpr)(false);

    PTR(Val) valBoolOne = NEW(BoolVal)(true);

    CHECK(boolOne->has_variable() == false);
    CHECK(boolThree->has_variable() == false);
}

TEST_CASE("BoolExpr pretty Print"){
    PTR(Expr)boolOne = NEW(BoolExpr)(true);
    PTR(Expr)boolTwo = NEW(BoolExpr)(true);
    PTR(Expr)boolThree = NEW(BoolExpr)(false);
    PTR(Expr)boolFour = NEW(BoolExpr)(false);

    PTR(Val) valBoolOne = NEW(BoolVal)(true);

    //need to figure out how to get this printing. look at others.
    CHECK(boolOne->pretty_print_string() == "_true");
    CHECK(boolFour->pretty_print_string() == "_false");
}

TEST_CASE("boolExpr interp"){
    PTR(Expr)boolOne = NEW(BoolExpr)(true);
    PTR(Expr)boolTwo = NEW(BoolExpr)(true);
    PTR(Expr)boolThree = NEW(BoolExpr)(false);
    PTR(Expr)boolFour = NEW(BoolExpr)(false);

    PTR(Val) valBoolOne = NEW(BoolVal)(true);
    PTR(Val) valBoolTwo = NEW(BoolVal)(true);
    PTR(Val) valBoolThree = NEW(BoolVal)(false);
    PTR(Val) valBoolFour = NEW(BoolVal)(false);

    CHECK((boolOne->interp(Env::empty))->equals(valBoolOne));
    CHECK((boolTwo->interp(Env::empty))->equals(valBoolTwo));
    CHECK((boolThree->interp(Env::empty))->equals(valBoolThree));
    CHECK((boolFour->interp(Env::empty))->equals(valBoolThree));
    CHECK_FALSE((boolOne->interp(Env::empty))->equals(valBoolThree));
    CHECK_FALSE((boolThree->interp(Env::empty))->equals(valBoolTwo));
}

TEST_CASE("boolExpr equals"){
    PTR(Expr)boolOne = NEW(BoolExpr)(true);
    PTR(Expr)boolTwo = NEW(BoolExpr)(true);
    PTR(Expr)boolThree = NEW(BoolExpr)(false);
    PTR(Expr)boolFour = NEW(BoolExpr)(false);

    CHECK(boolOne->equals(boolTwo));
    CHECK(boolThree->equals(boolFour));
    CHECK_FALSE(boolFour->equals(boolOne));
    CHECK_FALSE(boolOne->equals(NEW(NumExpr)(5)));
}

TEST_CASE( "let" ){
    std::ostream & objOstream = std::cout;

    PTR(LetExpr)test1 = NEW(LetExpr)(NEW(VariableExpr)("x"), NEW(NumExpr)(5), NEW(AddExpr)(NEW(VariableExpr)("x"), NEW(NumExpr)(1)));
    PTR(LetExpr)test2 = NEW(LetExpr)(NEW(VariableExpr)("x"), NEW(NumExpr)(5), NEW(AddExpr)(NEW(VariableExpr)("x"), NEW(NumExpr)(1)));
    PTR(LetExpr)test4 = NEW(LetExpr)(NEW(VariableExpr)("x"), NEW(NumExpr)(5), NEW(AddExpr)(NEW(LetExpr)(NEW(VariableExpr)("y"), NEW(NumExpr)(3), NEW(AddExpr)(NEW(VariableExpr)("y"), NEW(NumExpr)(2))), NEW(VariableExpr)("x")));
    PTR(LetExpr)test5 = NEW(LetExpr)(NEW(VariableExpr)("x"), NEW(NumExpr)(5),
                                 NEW(AddExpr)(NEW(LetExpr)(NEW(VariableExpr)("y"), NEW(NumExpr)(3),
                                                         NEW(AddExpr)(NEW(LetExpr)(NEW(VariableExpr)("z"), NEW(NumExpr)(1),
                                                                                 NEW(AddExpr)(NEW(VariableExpr)("z"), NEW(NumExpr)(2))), NEW(NumExpr)(5)
                )), NEW(NumExpr)(8)));
    PTR(LetExpr)test6 = NEW(LetExpr)(NEW(VariableExpr)("z"), NEW(NumExpr)(1), NEW(AddExpr)(NEW(VariableExpr)("z"), NEW(NumExpr)(2)));
    PTR(AddExpr)test9 = NEW(AddExpr)(NEW(LetExpr)(NEW(VariableExpr)("x"), NEW(NumExpr)(5), NEW(VariableExpr)("x")), NEW(NumExpr)(1));
    //does not pass (not any example)--comes up with 2 sets of parens
    PTR(MultExpr)test7 = NEW(MultExpr)(
            NEW(NumExpr)(5),
                    NEW(AddExpr)(
                        NEW(LetExpr)(NEW(VariableExpr)("x"), NEW(NumExpr)(5), NEW(VariableExpr)("x")),
                        NEW(NumExpr)(1))
            );
    //does not pass (this is example 4)--should have no parens
    PTR(MultExpr)test7B = NEW(MultExpr)(
            NEW(NumExpr)(5),
            NEW(LetExpr)(NEW(VariableExpr)("x"), NEW(NumExpr)(5), NEW(AddExpr)(NEW(VariableExpr)("x"), NEW(NumExpr)(1)))
            );
//    PTR(LetExpr)test7C = NEW(LetExpr)(NEW(VariableExpr)("x"), NEW(NumExpr)(5), NEW(MultExpr)(NEW(NumExpr)(5), NEW(AddExpr)(NEW(VariableExpr)("x"), NEW(NumExpr)(1))));
//passes (example 1) = 26
PTR(AddExpr)test8 = NEW(AddExpr)(NEW(MultExpr)(NEW(NumExpr)(5), NEW(LetExpr)(NEW(VariableExpr)("x"), NEW(NumExpr)(5), NEW(VariableExpr)("x"))), NEW(NumExpr)(1));

//    std::cout << test7->pretty_print_string() << std::endl;

    std::string string1 = "(_let x=5 _in (x+1))";
    std::string string2 = "(_let x=5 _in ((_let y=3 _in (y+2))+x))";
    std::string string3 = "_let x = 5\n"
                          "_in  (_let y = 3\n"
                          "      _in  (_let z = 1\n"
                          "            _in  z + 2) + 5) + 8";
    std::string string4 = "(_let x = 5\n"
                          " _in  x) + 1";
    std::string string5 = "_let z = 1\n"
                          "_in  z + 2";
    //test3->subst("x", NEW(NumExpr)(6))->print(objOstream);
//    std::cout << test5->pretty_print_string() << std::endl;

//    std::cout << test8->pretty_print_string() << std::endl;

    CHECK(test1->interp(Env::empty)->equals(NEW(NumVal)(6)));
    CHECK(test1->equals(test2));
    CHECK(test1->equals(NEW(AddExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2))) == false);
    CHECK(test1->has_variable());
    //these shouldn't pass anymore
//    PTR(LetExpr)test3 = NEW(LetExpr)(NEW(VariableExpr)("x"), NEW(NumExpr)(5), NEW(AddExpr)(NEW(VariableExpr)("x"), NEW(NumExpr)(1)));
//    CHECK(test3->subst("x", NEW(NumExpr)(6))->interp() == 7);
//    CHECK(test3->to_string() == string1);
    CHECK(test4->to_string() == string2);
    CHECK(test5->pretty_print_string() == string3);
    CHECK(test6->pretty_print_string() == string5);
    CHECK(test9->pretty_print_string() == string4);

    // _let x = y
    // _in  x + 2 -> subst("y", 8)
    // =
    // _let x = 8
    // _in  x + 2
    PTR(LetExpr)subst1 = NEW(LetExpr)(NEW(VariableExpr)("x"), NEW(VariableExpr)("y"), NEW(AddExpr)(NEW(VariableExpr)("x"), NEW(NumExpr)(2)));
    PTR(LetExpr)subst1A = NEW(LetExpr)(NEW(VariableExpr)("x"), NEW(NumExpr)(8), NEW(AddExpr)(NEW(VariableExpr)("x"), NEW(NumExpr)(2)));

//     _let x = 8
//     _in  x + 2 + y -> subst("y", 9)
//     =
//     _let x = 8
//     _in  x + 2 + 9

    PTR(LetExpr)subst2 = NEW(LetExpr)(NEW(VariableExpr)("x"), NEW(NumExpr)(8), NEW(AddExpr)(NEW(VariableExpr)("x"), NEW(AddExpr)(NEW(NumExpr)(2), NEW(VariableExpr)("y"))));
    PTR(LetExpr)subst2A = NEW(LetExpr)(NEW(VariableExpr)("x"), NEW(NumExpr)(8), NEW(AddExpr)(NEW(VariableExpr)("x"), NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr)(9))));
    // y + y -> subst("y", 8)
    // =
    // 8 + 8

    // _let x = y
    // _in  x + y -> subst("y", 8)
    // =
    // _let x = 8
    // _in  x + 8
    PTR(LetExpr)subst3 = NEW(LetExpr)(NEW(VariableExpr)("x"), NEW(VariableExpr)("y"), NEW(AddExpr)(NEW(VariableExpr)("x"), NEW(VariableExpr)("y")));
    PTR(LetExpr)subst3A = NEW(LetExpr)(NEW(VariableExpr)("x"), NEW(NumExpr)(8), NEW(AddExpr)(NEW(VariableExpr)("x"), NEW(NumExpr)(8)));

    // _let z = x
    // _in  z + 32 -> subst("z", 0)
    // =
    // _let z = x
    // _in  z + 32

    PTR(LetExpr)subst4 = NEW(LetExpr)(NEW(VariableExpr)("z"), NEW(VariableExpr)("x"), NEW(AddExpr)(NEW(VariableExpr)("z"), NEW(NumExpr)(32)));
    PTR(LetExpr)subst4A = NEW(LetExpr)(NEW(VariableExpr)("z"), NEW(VariableExpr)("x"), NEW(AddExpr)(NEW(VariableExpr)("z"), NEW(NumExpr)(32)));
    // _let z = z
    // _in  z + 32 -> subst("z", 0)
    // =
    // _let z = 0
    // _in  z + 32
    PTR(LetExpr)subst5 = NEW(LetExpr)(NEW(VariableExpr)("z"), NEW(VariableExpr)("z"), NEW(AddExpr)(NEW(VariableExpr)("z"), NEW(NumExpr)(32)));
    PTR(LetExpr)subst5A = NEW(LetExpr)(NEW(VariableExpr)("z"), NEW(NumExpr)(0), NEW(AddExpr)(NEW(VariableExpr)("z"), NEW(NumExpr)(32)));

    // _let z = z + 2
    // _in  z + 32 -> subst("z", 0)
    // =
    // _let z = 0 + 2
    // _in  z + 32

    PTR(LetExpr)subst6 = NEW(LetExpr)(NEW(VariableExpr)("z"), NEW(AddExpr)(NEW(VariableExpr)("z"), NEW(NumExpr)(2)), NEW(AddExpr)(NEW(VariableExpr)("z"), NEW(NumExpr)(32)));
    PTR(LetExpr)subst6A = NEW(LetExpr)(NEW(VariableExpr)("z"), NEW(AddExpr)(NEW(NumExpr)(0), NEW(NumExpr)(2)), NEW(AddExpr)(NEW(VariableExpr)("z"), NEW(NumExpr)(32)));

}

TEST_CASE( "expr" ){
    PTR(NumExpr)test1 = NEW(NumExpr)(1);
    PTR(NumExpr)test2 = NEW(NumExpr)(2);
    PTR(NumExpr)test3 = NEW(NumExpr)(3);
    PTR(NumExpr)test4 = NEW(NumExpr)(4);
    PTR(NumExpr)test5 = NEW(NumExpr)(2);
    std::string dog1 = "dog";
    std::string dog2 = "dog";
    std::string cat1 = "cat";

    CHECK((NEW(NumExpr)(1))->equals(NEW(NumExpr)(1)) == true);
    CHECK((NEW(NumExpr)(1))->equals(NEW(NumExpr)(2)) == false);
    CHECK((NEW(NumExpr)(1))->equals(NEW(AddExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2))) == false);

    CHECK((NEW(AddExpr)(test1, test2))->equals(NEW(AddExpr)(test1, test5)) == true);
    CHECK((NEW(AddExpr)(test1, test2))->equals(NEW(AddExpr)(test3, test4)) == false);
    CHECK((NEW(AddExpr)(test1, test2))->equals(NEW(MultExpr)(test1, test2)) == false);

    CHECK((NEW(MultExpr)(test1, test2))->equals(NEW(MultExpr)(test1, test5)) == true);
    CHECK((NEW(MultExpr)(test1, test2))->equals(NEW(MultExpr)(test3, test4)) == false);
    CHECK((NEW(AddExpr)(test1, test2))->equals(NEW(MultExpr)(test1, test2)) == false);


    CHECK((NEW(VariableExpr)("dog"))->equals(NEW(VariableExpr)("dog")) == true);
    CHECK((NEW(VariableExpr)(dog1))->equals(NEW(VariableExpr)(dog2)) == true);
    CHECK((NEW(VariableExpr)("dog"))->equals(NEW(VariableExpr)("cat")) == false); //change back to false
    CHECK((NEW(VariableExpr)(dog1))->equals(NEW(VariableExpr)(cat1)) == false);
    CHECK((NEW(VariableExpr)("dog"))->equals(NEW(MultExpr)(test1, test2)) == false);
}

TEST_CASE( "interp" ){

    PTR(NumExpr)test1 = NEW(NumExpr)(1);
    PTR(NumExpr)test2 = NEW(NumExpr)(2);
    PTR(NumExpr)test3 = NEW(NumExpr)(3);
    PTR(NumExpr)test4 = NEW(NumExpr)(4);
    PTR(NumExpr)test5 = NEW(NumExpr)(2);
    std::string dog1 = "dog";
    std::string dog2 = "dog";
    std::string cat1 = "cat";

    CHECK(test1->interp(Env::empty)->equals(NEW(NumVal)(1)));
    CHECK(test2->interp(Env::empty)->equals(NEW(NumVal)(2)));

    CHECK((NEW(AddExpr)(test1, test2))->interp(Env::empty)->equals(NEW(NumVal)(3)));
    CHECK((NEW(AddExpr)(test1, test2))->interp(Env::empty) != NEW(NumVal)(4));
    CHECK((NEW(AddExpr)(test4, test4))->interp(Env::empty)->equals(NEW(NumVal)(8)));

    CHECK((NEW(MultExpr)(test1, test2))->interp(Env::empty)->equals(NEW(NumVal)(2)));
    CHECK((NEW(MultExpr)(test1, test2))->interp(Env::empty) != NEW(NumVal)(4));
    CHECK((NEW(MultExpr)(test4, test4))->interp(Env::empty)->equals(NEW(NumVal)(16)));

    CHECK_THROWS_WITH((NEW(VariableExpr)("x"))->interp(Env::empty), "free variable: x" );
}

TEST_CASE( "has_variable" ){

    PTR(NumExpr)test1 = NEW(NumExpr)(1);
    PTR(NumExpr)test2 = NEW(NumExpr)(2);
    PTR(NumExpr)test3 = NEW(NumExpr)(3);
    PTR(NumExpr)test4 = NEW(NumExpr)(4);
    PTR(NumExpr)test5 = NEW(NumExpr)(2);
    std::string dog1 = "dog";
    std::string dog2 = "dog";
    std::string cat1 = "cat";
    PTR(VariableExpr)var1 = NEW(VariableExpr)("dog");

    PTR(AddExpr)add1 = NEW(AddExpr)(test1, test2); //3
    PTR(AddExpr)add2 = NEW(AddExpr)(test2, test3); //5
    PTR(AddExpr)add3 = NEW(AddExpr)(test2, var1); //contains variable

    PTR(MultExpr)mult1 = NEW(MultExpr)(test1, test2); //2
    PTR(MultExpr)mult2 = NEW(MultExpr)(test2, test4); //8
    PTR(MultExpr)mult3 = NEW(MultExpr)(test1, var1); //contains variable

    CHECK(test1->has_variable() == false);

    CHECK(add1->has_variable()==false);
    CHECK(add3->has_variable()==true);

    CHECK(mult1->has_variable() == false);
    CHECK(mult3->has_variable() == true);

    CHECK(var1->has_variable() == true);
}

TEST_CASE( "subst" ){

    PTR(NumExpr)test1 = NEW(NumExpr)(1);
    PTR(NumExpr)test2 = NEW(NumExpr)(2);
    PTR(NumExpr)test3 = NEW(NumExpr)(3);
    PTR(NumExpr)test4 = NEW(NumExpr)(4);
    PTR(NumExpr)test5 = NEW(NumExpr)(2);
    std::string dog1 = "dog";
    std::string dog2 = "dog";
    std::string cat1 = "cat";
    PTR(VariableExpr)var1 = NEW(VariableExpr)("dog");
    PTR(VariableExpr)var2 = NEW(VariableExpr)("cat");

    PTR(AddExpr)add1 = NEW(AddExpr)(test1, test2); //3
    PTR(AddExpr)add2 = NEW(AddExpr)(test2, test3); //5
    PTR(AddExpr)add3 = NEW(AddExpr)(test2, var1); //contains variable

    PTR(MultExpr)mult1 = NEW(MultExpr)(test1, test2); //2
    PTR(MultExpr)mult2 = NEW(MultExpr)(test2, test4); //8
    PTR(MultExpr)mult3 = NEW(MultExpr)(var1, test1);
    PTR(MultExpr)mult4 = NEW(MultExpr)(var2, test1); //contains variable
}

TEST_CASE( " print ") {

    std::string addString = "(1+(2+3))";
    std::string multString = "(1*(2+3))";
    std::string one = "1";
    std::string dog = "dog";
    std::stringstream ss;

    PTR(Expr)test1 = (NEW(AddExpr)(NEW(NumExpr)(1), NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3))));

    PTR(Expr)test2 = (NEW(MultExpr)(NEW(NumExpr)(1), NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3))));

    PTR(Expr)testNum = NEW(NumExpr)(1);
    PTR(Expr)testVariable = NEW(VariableExpr)("dog");

    CHECK( test1->to_string()   == addString);
    CHECK( test2->to_string() == multString );

    CHECK( testNum->to_string() == one);
    CHECK( testVariable->to_string() == dog);

}

TEST_CASE( " pretty_ print"){
    //"1 * (2 + 3)";
    //"(2 + 4) + 1";
    //"2 + 4 + 1";
    //"2 + 4 * 1";
    //"(5 * 4) * 3 * 2";
    PTR(Expr)test1 =(NEW(MultExpr)(NEW(NumExpr)(1), NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3))));
    PTR(Expr)test2 =(NEW(AddExpr)(NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr)(4)), NEW(NumExpr)(1)));
    PTR(Expr)test3 = (NEW(AddExpr)(NEW(NumExpr)(2), NEW(AddExpr)(NEW(NumExpr)(4), NEW(NumExpr)(1))));
    PTR(Expr)test4 = (NEW(AddExpr)(NEW(NumExpr)(2), NEW(MultExpr)(NEW(NumExpr)(4), NEW(NumExpr)(1))));
    PTR(Expr)test5 = (NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(5), NEW(NumExpr)(4)), NEW(MultExpr)(NEW(NumExpr)(3), NEW(NumExpr)(2))));
    PTR(Expr)test6 = (NEW(AddExpr)(NEW(VariableExpr)("cat"), NEW(MultExpr)(NEW(NumExpr)(4), NEW(NumExpr)(1))));
    //


    CHECK(test1->pretty_print_string() == "1 * (2 + 3)");
    CHECK(test2->pretty_print_string() == "(2 + 4) + 1");
    CHECK(test3->pretty_print_string() == "2 + 4 + 1");
    CHECK(test4->pretty_print_string() == "2 + 4 * 1");
    CHECK(test5->pretty_print_string() == "(5 * 4) * 3 * 2");
    CHECK(test6->pretty_print_string() == "cat + 4 * 1");
}

//TEST_CASE("parse"){
//    std::string parseTest1 = "5 * 2";
//
//    PTR(Expr)parseTest1B = NEW(MultExpr)(NEW(NumExpr)(5), NEW(NumExpr)(2));
//
//    CHECK(Parse::parse_str(parseTest1)->equals(parseTest1B));
//}

/*
 * fast fibonicci test
 *
_let pair = _fun (a) _fun (b)
              _fun(sel)
              _if sel _then a _else b
_in _let fst = _fun (p) p(_true)
_in _let snd = _fun (p) p(_false)
_in _let fib = _fun (fib)
                _fun (x)
                  _if x == 0
                  _then pair(1)(1)
                  _else _if x == 1
                        _then pair(1)(1)
                        _else _let p = fib(fib)(x + -1)
                              _in pair(fst(p) + snd(p))(fst(p))
_in  fst(fib(fib)(30))
 * */

/*
 * regular fiboniacci test
 *
 * _let fib = _fun (fib)
             _fun (x)
               _if x == 0
               _then 1
               _else _if x == 1
                     _then 1
                     _else fib(fib)(x + -2) + fib(fib)(x + -1)
_in  fib(fib)(30)
 */