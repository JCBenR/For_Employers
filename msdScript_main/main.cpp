#include "cmdline.h"
#include "expr.h"
#include "Parse.h"
#include "Env.h"
#include "Val.h"
#include <iostream>
#include <fstream>

int main(int argc, char* argv[]){
//    char *filename = nullptr;
//
//    filename = argv[1];
//
//    PTR(Expr) e;
//    if (filename != nullptr){
//        std::ifstream f_in(filename);
//        e = Parse::parse_expr(f_in);
//        std::cout << e->interp(Env::empty)->get_string() << std::endl;
//        return 0;
//    }
//    else {
//        e = Parse::parse_expr(std::cin);
//        std::cout << e->interp(Env::empty)->get_string() << std::endl;
//        return 0;
//    }

    try {
        use_arguments(argc, argv);
        return 0;
    } catch (std::runtime_error exn) {
        std::cerr << exn.what() << "\n";
        return 1;
    }
}

