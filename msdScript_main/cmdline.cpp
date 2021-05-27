//
// Created by Justin Siebenhaar on 1/19/21.
//
#define CATCH_CONFIG_RUNNER
#include "cmdline.h"
#include "catch.h"
#include "expr.h"
#include "Parse.h"
#include "Val.h"
#include "Env.h"
#include "Step.h"
#include <iostream>

void use_arguments(int argc, char* argv[]){

    bool seen = false;

    for (int i = 1; i < argc; i++) {
        std::string currentArg =((std::string) argv[i]);

        if(currentArg == "--help"){
            std::cout << "available arguments are:" << std::endl;
            std::cout << "_______________________________________________" << std::endl;
            std::cout << "--help            |   get help" << std::endl;
            std::cout << "--interp          |   interpret equations" << std::endl;
            std::cout << "--step            |   interprets equations using steps" << std::endl;
            std::cout << "--print           |   print functions" << std::endl;
            std::cout << "_______________________________________________" << std::endl;

            exit(0);
        }
        else if(currentArg == "--test" && !seen){
            std::cout << "catch is: \n";
            int catchSession = Catch::Session().run(1, argv);
            std::cout << catchSession;
            if(catchSession != 0){
                exit(1);
            }
            std::cout << "catch is: \n";
            std::cout << catchSession;
            seen = true;
        }
        else if(seen){
            std::cerr << "argument already seen" << std::endl;
            exit(1);
        }
        else if (currentArg == "--step"){
            while (1){

                PTR(Expr)e = Parse::parse_expr(std::cin);
                PTR(Val)val2 = Step::interp_by_steps(e);

                std::cout << val2->get_string() << std::endl;

                Parse::skip_whitespace(std::cin);
                if (std::cin.eof())
                    break;
            }
        }
        else if (currentArg == "--interp"){
            while (1){

                PTR(Expr)e = Parse::parse_expr(std::cin);
                PTR(Val)val2 = e->interp(Env::empty);

                std::cout << e->interp(Env::empty)->get_string() << std::endl;

                Parse::skip_whitespace(std::cin);
                if (std::cin.eof())
                    break;
            }
        }
        else if (currentArg == "--print"){
            while (1){

                PTR(Expr)e = Parse::parse_expr(std::cin);

                e->print(std::cout);
                std::cout << "\n";

                Parse::skip_whitespace(std::cin);
                if (std::cin.eof())
                    break;
            }
        }
        else if (currentArg == "--pretty-print"){
            while (1){
                std::stringstream ss;
                PTR(Expr)e = Parse::parse_expr(std::cin);

                e->pretty_print(ss);
                std::cout << ss.str() << std::endl;

                Parse::skip_whitespace(std::cin);
                if (std::cin.eof())
                    break;
            }
        }
        else {
            std::cerr << "argument not recognized" << std::endl;
            exit(1);
        }
    }
}