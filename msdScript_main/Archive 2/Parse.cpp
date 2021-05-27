//
// Created by Justin Siebenhaar on 2/9/21.
//

#include "Parse.h"
#include "expr.h"
#include "catch.h"
#include "Pointers.h"
#include "Cont.h"
#include "Step.h"
#include "Val.h"
#include <stdexcept>
#include <iostream>
#include <string>
#include <istream>
#include <sstream>

/**
 * parse will read in literal strings of equations entered by the user, extract the relavant parts and assign to the appropriate classes for interpretation.
 */

/**
 *
 * @param in input stream
 * @param expect
 */
void Parse::consume(std::istream &in, int expect) {
    int c = in.get();
    if (c != expect)
        throw std::runtime_error("consume mismatch"); //consume inside a let
}

/**
 * skips any surrounding whitespace
 * @param in input stream
 */
void Parse::skip_whitespace(std::istream &in) {
    while(1){
        int c = in.peek();
        if (!isspace(c))
            break;
        consume(in, c);
    }
}

/**
 * reads out numbers. if finds a leading '-' means a negative number
 * @param in input stream
 * @return NumExpr
 */
PTR(Expr) Parse::parse_num(std::istream &in) {
    int n=0;
    bool negative = false;

    if (in.peek() == '-'){
        negative = true;
        consume(in, '-');
    }
    while(1){
        int c = in.peek();
        if (isdigit(c)) {
            //checkSizeOfNumber(c);
            consume(in, c);
            n = (unsigned) n * 10 + (c - '0');
            if (n < 0)
                std::cerr << c << " is too large a number!" << std::endl;
        }
        else
            break;
    }
    if (negative)
        n = -n;

    return NEW(NumExpr)(n);
}

/**
 * looks for variables either as char or strings
 * @param in input stream
 * @return VariableExpr
 */
PTR(VariableExpr) Parse::parse_variable(std::istream &in) {
    std::string v;
    while (true) {
    char c = in.peek();
        if (isalpha(c)) {
            consume(in, c);
            v.push_back(c);
        } else
            break;
    }
    return NEW(VariableExpr)(v);
}

/**
 * specific for parsing let statements
 * @param in input stream
 * @return LetExpr
 */
PTR(Expr) Parse::parse_let(std::istream &in){
    skip_whitespace(in);
    PTR(VariableExpr)lhs = parse_variable(in);
    skip_whitespace(in);
    char c = in.peek();
    if (c == '='){
        consume(in, c);
    }
    skip_whitespace(in);
    PTR(Expr)rhs = parse_expr(in);
    skip_whitespace(in);
    PTR(Expr)body = parse_expr(in);

    return NEW(LetExpr)(lhs, rhs, body);
}

/**
 * parses If statements
 * @param in input stream
 * @return IfExpr
 */
PTR(Expr) Parse::parse_if(std::istream &in){
    skip_whitespace(in);
    PTR(Expr) ifStatement = parse_expr(in);
    skip_whitespace(in);
    PTR(Expr) thenStatement = parse_keyword(in);
    skip_whitespace(in);
    PTR(Expr) elseStatement = parse_keyword(in);
    return NEW(IfExpr)(ifStatement, thenStatement, elseStatement);
}

/**
 * specific for parsing Function statements
 * @param in input stream
 * @return FunExpr
 */
PTR(Expr) Parse::parse_fun(std::istream &in){
    skip_whitespace(in);
    std::string formal_arg;
    consume(in, '(');
    skip_whitespace(in);

    while (true) {
        char c = in.peek();
        if (isalpha(c)) {
            consume(in, c);
            formal_arg.push_back(c);
        } else
            break;
    }
    skip_whitespace(in);

    consume(in, ')');

    skip_whitespace(in);
        PTR(Expr)e = parse_expr(in);
        return NEW(FunExpr)(formal_arg, e);
    }

/**
 * parses out keywords of various expr
 * @param in input stream
 * @return full expression
 */
PTR(Expr) Parse::parse_keyword(std::istream &in){
    std::string keyword;
    while (true) {
        char c = in.peek();
        if (isalpha(c) || c == '_') {
            consume(in, c);
            keyword.push_back(c);
        } else break;
    }
    if (keyword == "_let")
        return parse_let(in);
    else if (keyword ==  "_in"|| keyword == "_then" || keyword == "_else")
        return parse_expr(in);
    else if (keyword == "_if")
        return parse_if(in);
    else if (keyword == "_true")
        return NEW(BoolExpr)(true);
    else if (keyword == "_false")
        return NEW(BoolExpr)(false);
    else if (keyword == "_fun")
        return parse_fun(in);
    else
        throw std::runtime_error(keyword + " is not a valid input");
}

/**
 * parses out AddExpr
 * @param in input stream
 * @return Expr
 */
PTR(Expr) Parse::parse_addend(std::istream &in) {
    PTR(Expr)e;

    e = parse_multicand(in);
    skip_whitespace(in);

    int c = in.peek();
    if (c == '*' ) {
        consume(in, '*');
        PTR(Expr)rhs = parse_addend(in);
        return NEW(MultExpr)(e, rhs);
    }
    else
        return e;
}

/**
 * parses out MultExpr
 * @param in input stream
 * @return Expr
 */
PTR(Expr) Parse::parse_multicand(std::istream &in) {
    PTR(Expr)e = parse_inner(in);
    while (in.peek() == '('){
        consume(in, '(');
        PTR(Expr)actual_arg = parse_expr(in);
        consume(in, ')');
        e = NEW(CallExpr)(e, actual_arg);
    }
    return e;
}

/**
 * specifically used for parsing out nested expressions
 * @param in input stream
 * @return Expr
 */
PTR(Expr) Parse::parse_inner(std::istream &in){
    skip_whitespace(in);

    int c = in.peek();
    if ((c == '-' || isdigit(c)))
        return parse_num(in);
    else if (isalpha(c))
        return parse_variable(in);
    else if (c == '_') {
        return parse_keyword(in);
    }
    else if (c == '('){
        consume(in, '(');
        PTR(Expr)e = parse_expr(in);
        skip_whitespace(in);
        c = in.get();
        if (c != ')')
            throw std::runtime_error("Missing close parenthesis");
        return e;
    }
    else{
        consume(in, c);
        throw std::runtime_error("invalid input");
    }
}

/**
 * parses out comparison arguments for EqExpr
 * @param in input stream
 * @return AddExpr
 */
PTR(Expr) Parse::parse_comparg(std::istream &in){ //change to parse_comparg

    PTR(Expr)e;

    e = parse_addend(in);
    skip_whitespace(in);

    int c = in.peek();
    if (c == '+' ) {
        consume(in, '+');
        PTR(Expr)rhs = parse_comparg(in);
        return NEW(AddExpr)(e, rhs);
    }
    else
        return e;
}

/**
 * used to determine the '=' sign, whether a single or double.
 * @param in input stream
 * @return Expr
 */
PTR(Expr) Parse::parse_expr(std::istream &in){
    PTR(Expr)e;

    e = parse_comparg(in);
    skip_whitespace(in);

    int c = in.peek();
    if (c == '=' ) { //the only time it should encounter the '=' is when it's a comparg, and would then be a '=='.
        consume(in, '=');
        consume(in, '=');
        PTR(Expr)rhs = parse_expr(in);
        return NEW(EqExpr)(e, rhs);
    }
    else
        return e;
}

/**
 * starts process of parsing
 * @param s string to be parsed
 * @return Expr
 */
PTR(Expr) Parse::parse_str(std::string s){
    std::istringstream ss(s);
//    skip_whitespace(ss);
//    if (!ss.eof()){
//        throw std::runtime_error("invalid entry");
//    }
    return parse_expr(ss);
}

TEST_CASE("parse"){

//    std::string mattsTest = "_let factrl = _fun (factrl)\n"
//                            "                _fun (x)\n"
//                            "                  _if x == 1\n"
//                            "                  _then 1\n"
//                            "                  _else x * factrl(factrl)(x + -1)\n"
//                            "_in  factrl(factrl)(10)";

//    auto *mattsTestRun = Parse::parse_str(mattsTest);

//    mattsTestRun->print(std::cout);

    std::string parseSimpleFun = "(_fun (x) (x+5))";

    PTR(Expr)simpleFun = NEW(FunExpr)("x", NEW(AddExpr)(NEW(VariableExpr)("x"), NEW(NumExpr)(5)));

    CHECK(Parse::parse_str(parseSimpleFun)->equals(simpleFun));

    std::string parseSimpleFunMult = "(_fun (x) (x*5))";

    PTR(Expr)simpleFunMult = NEW(FunExpr)("x", NEW(MultExpr)(NEW(VariableExpr)("x"), NEW(NumExpr)(5)));

    CHECK(Parse::parse_str(parseSimpleFunMult)->equals(simpleFunMult));

    std::string simpleCall = "f(2)";

    CHECK(Parse::parse_str(simpleCall)->equals(NEW(CallExpr)(NEW(VariableExpr)("f"), NEW(NumExpr)(2))));

    std::string parseCallExpr = "_let f = (_fun (x) (x*x))"
                                "_in f(2)";

    PTR(Expr)parseLetCall = NEW(LetExpr)(NEW(VariableExpr)("f"), NEW(FunExpr)("x", NEW(MultExpr)(NEW(VariableExpr)("x"), NEW(VariableExpr)("x"))), NEW(CallExpr)(NEW(VariableExpr)("f"), NEW(NumExpr)(2)));

//    std::cout << parseLetCall->to_string() << std::endl;

    CHECK(Parse::parse_str(parseCallExpr)->equals(parseLetCall));

    std::string parseTrue = "_if _true\n"
                            "_then x\n"
                            "_else y";

    std::string parseTrue2 = "_if _true _then x _else y";
    PTR(Expr)parseTrueTest1 = NEW(IfExpr)(NEW(BoolExpr)(true), NEW(VariableExpr)("x"), NEW(VariableExpr)("y"));
//    std::cout << parseTrue << std::endl;
//    std::cout << parseTrueTest1->to_string() << std::endl;

    CHECK(Parse::parse_str(parseTrue)->equals(parseTrueTest1));
    CHECK(Parse::parse_str(parseTrue2)->equals(parseTrueTest1));

    std::string parseSubst = "_if 5 == 5 _then 6 _else 7";
    PTR(Expr)parseSubstTest1 = NEW(IfExpr)(NEW(EqExpr)(NEW(NumExpr)(5), NEW(NumExpr)(5)), NEW(NumExpr)(6), NEW(NumExpr)(7));

    CHECK(Parse::parse_str(parseSubst)->equals(parseSubstTest1));

    std::string parseSubstfalse = "_if 5 == 6 _then 6 _else 7";
    PTR(Expr)parseSubstTest1false = NEW(IfExpr)(NEW(EqExpr)(NEW(NumExpr)(5), NEW(NumExpr)(6)), NEW(NumExpr)(6), NEW(NumExpr)(7));

    CHECK(Parse::parse_str(parseSubstfalse)->equals(parseSubstTest1false));
    CHECK_FALSE(Parse::parse_str(parseSubst)->equals(parseSubstTest1false));
    CHECK_FALSE(Parse::parse_str(parseSubstfalse)->equals(parseSubstTest1));


//    std::string boolExprString = "_true";
//    PTR(Expr)boolExprTest = NEW(BoolExpr)(true);
    CHECK(Parse::parse_str("_true")->equals(NEW(BoolExpr)(true)));
    CHECK(Parse::parse_str("_false")->equals(NEW(BoolExpr)(false)));



    std::string parseTest1 = "5 * 2";
    PTR(Expr)parseTest1B = NEW(MultExpr)(NEW(NumExpr)(5), NEW(NumExpr)(2));
    CHECK(Parse::parse_str(parseTest1)->equals(parseTest1B));


    std::string test1A = "1 * (2 + 3)";
    PTR(Expr)test1 =(NEW(MultExpr)(NEW(NumExpr)(1), NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3))));
    CHECK(Parse::parse_str(test1A)->equals(test1));


    std::string test2A ="(2 + 4) + 1";
    PTR(Expr)test2 =(NEW(AddExpr)(NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr)(4)), NEW(NumExpr)(1)));
    CHECK(Parse::parse_str(test2A)->equals(test2));


    std::string test3A ="2 + 4 + 1";
    PTR(Expr)test3 = (NEW(AddExpr)(NEW(NumExpr)(2), NEW(AddExpr)(NEW(NumExpr)(4), NEW(NumExpr)(1))));
    CHECK(Parse::parse_str(test3A)->equals(test3));

    std::string test4A = "2 + 4 * 1";
    PTR(Expr)test4 = (NEW(AddExpr)(NEW(NumExpr)(2), NEW(MultExpr)(NEW(NumExpr)(4), NEW(NumExpr)(1))));
    CHECK(Parse::parse_str(test4A)->equals(test4));

    std::string test5A ="(5 * 4) * 3 * 2";
    PTR(Expr)test5 = (NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(5), NEW(NumExpr)(4)), NEW(MultExpr)(NEW(NumExpr)(3), NEW(NumExpr)(2))));
    CHECK(Parse::parse_str(test5A)->equals(test5));

    std::string test6A = "1 * (-2 + 3)";
    PTR(Expr)test6 =(NEW(MultExpr)(NEW(NumExpr)(1), NEW(AddExpr)(NEW(NumExpr)(-2), NEW(NumExpr)(3))));
    CHECK(Parse::parse_str(test6A)->equals(test6));


    std::string letString1 = "(_let x=5 _in (x+1))";
    PTR(LetExpr)testLet1 = NEW(LetExpr)(NEW(VariableExpr)("x"), NEW(NumExpr)(5), NEW(AddExpr)(NEW(VariableExpr)("x"), NEW(NumExpr)(1)));
    CHECK(Parse::parse_str(letString1)->equals(testLet1));

    std::string letString2 = "(_let x=5 _in ((_let y=3 _in (y+2))+x))";
    PTR(LetExpr)testLet2 = NEW(LetExpr)(NEW(VariableExpr)("x"), NEW(NumExpr)(5), NEW(AddExpr)(NEW(LetExpr)(NEW(VariableExpr)("y"), NEW(NumExpr)(3), NEW(AddExpr)(NEW(VariableExpr)("y"), NEW(NumExpr)(2))), NEW(VariableExpr)("x")));
    CHECK(Parse::parse_str(letString2)->equals(testLet2));


    std::string letString3 = "_let x = 5\n"
                          "_in  (_let y = 3\n"
                          "      _in  (_let z = 1\n"
                          "            _in  z + 2) + 5) + 8";
    PTR(LetExpr)testLet3 = NEW(LetExpr)(NEW(VariableExpr)("x"), NEW(NumExpr)(5),
                                    NEW(AddExpr)(NEW(LetExpr)(NEW(VariableExpr)("y"), NEW(NumExpr)(3),
                                                            NEW(AddExpr)(NEW(LetExpr)(NEW(VariableExpr)("z"), NEW(NumExpr)(1),
                                                                                    NEW(AddExpr)(NEW(VariableExpr)("z"), NEW(NumExpr)(2))), NEW(NumExpr)(5)
                                         )), NEW(NumExpr)(8)));
    CHECK(Parse::parse_str(letString3)->equals(testLet3));


    std::string letString4 = "(_let x = 5\n"
                          " _in  x) + 1";
    PTR(AddExpr)testLet4 = NEW(AddExpr)(NEW(LetExpr)(NEW(VariableExpr)("x"), NEW(NumExpr)(5), NEW(VariableExpr)("x")), NEW(NumExpr)(1));
    CHECK(Parse::parse_str(letString4)->equals(testLet4));


    std::string letString5 = "_let z = 1\n"
                          "_in  z + 2";
    PTR(LetExpr)testLet5 = NEW(LetExpr)(NEW(VariableExpr)("z"), NEW(NumExpr)(1), NEW(AddExpr)(NEW(VariableExpr)("z"), NEW(NumExpr)(2)));
    CHECK(Parse::parse_str(letString5)->equals(testLet5));


    std::string missingParen = "(_let x = 5\n"
                             " _in  x + 1";
    PTR(AddExpr)testLet6 = NEW(AddExpr)(NEW(LetExpr)(NEW(VariableExpr)("x"), NEW(NumExpr)(5), NEW(VariableExpr)("x")), NEW(NumExpr)(1));
    CHECK_THROWS_WITH(Parse::parse_str(missingParen), "Missing close parenthesis");

    std::string badChar = "... + 6 % 7";
    CHECK_THROWS_WITH(Parse::parse_str(badChar), "invalid input");

    std::string badKeyword = "(_let x = 5\n"
                             " _test  x + 1";
    CHECK_THROWS_WITH(Parse::parse_str(badKeyword), "_test is not a valid input");

}