//
// Created by Justin Siebenhaar on 3/26/21.
//

#include "Env.h"


PTR(Env) Env::empty = NEW(EmptyEnv)();

EmptyEnv::EmptyEnv() = default;

/**
 * looks up variable in environment to return value
 * @param find_name string of variable to look up
 * @return Val of variable association
 */
PTR(Val)EmptyEnv::lookup(std::string find_name){
    throw std::runtime_error("free variable: " + find_name);
};

/**
 * extended environment hold variables/value association from pervious environments. useful if variable is not contained within environment of this, can look up to parents to find association.
 * @param name variable
 * @param val value
 * @param rest further extended environments
 */
ExtendedEnv::ExtendedEnv(std::string name, PTR(Val) val, PTR(Env) rest) {
    this->name = name;
    this->val = val;
    this->rest = rest;
}

/**
 * similar to lookup function (above) but on extended envornments
 * @param find_name variable to find
 * @return value associated with that variable
 */
PTR(Val)ExtendedEnv::lookup(std::string find_name){
    if (find_name == name)
        return val;
    else
        return rest->lookup(find_name);
};
