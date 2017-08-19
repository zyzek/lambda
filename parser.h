#pragma once
#include "lexer.h"
#include "expression.h"

using namespace std;

class Parser {
    private:
        shared_ptr<Lexer> l;
        shared_ptr<Environment> names;

        unique_ptr<Expression> parse_variable();
        unique_ptr<Expression> parse_application();
        unique_ptr<Expression> parse_abstraction();
        unique_ptr<Expression> parse_parenthetical();
        unique_ptr<Expression> parse_non_application();
        unique_ptr<Expression> parse_expression();
        unique_ptr<Expression> parse_assignment();
    public:
        Parser(shared_ptr<Lexer> lexer = shared_ptr<Lexer>(), shared_ptr<Environment> env = shared_ptr<Environment>()) {
            if (lexer != nullptr) {
                l = lexer;
            } else {
                l = make_shared<Lexer>();
            }

            if (env != nullptr) {
                names = env;
            } else {
                names = make_shared<Environment>();
            }
        }

        shared_ptr<Lexer> get_lexer() { return l; }
        shared_ptr<Environment> get_env() { return names; }
        unique_ptr<Expression> parse();
};
