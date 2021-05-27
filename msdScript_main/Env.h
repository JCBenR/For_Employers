//
// Created by Justin Siebenhaar on 3/26/21.
//

#ifndef MSDSCRIPT_ENV_H
#define MSDSCRIPT_ENV_H

#include "Parse.h"
#include "expr.h"
#include "catch.h"
#include "Pointers.h"

/**
 * environments holding the variable/value associations.
 */
CLASS(Env) {
public:
    virtual PTR(Val) lookup(std::string find_name) =0;
    static PTR(Env) empty;
};

/**
 * empty environments with no variables or values
 */
class EmptyEnv : public Env {
public:
    EmptyEnv();
    PTR(Val) lookup(std::string find_name);
};

/**
 * environments being passed down to children with associations.
 */
class ExtendedEnv : public Env {
public:
    std::string name;
    PTR(Val) val;
    PTR(Env) rest;
    ExtendedEnv(std::string name, PTR(Val) val, PTR(Env) rest);
    PTR(Val) lookup(std::string find_name);
};


#endif //MSDSCRIPT_ENV_H
