#pragma once
#include "lexer.h"
#include "expression.h"


using namespace std;

class Parser {
    private:
        Lexer l;
        token t;
        shared_ptr<Environment> names;

        unique_ptr<Expression> parse_variable();
        unique_ptr<Expression> parse_application();
        unique_ptr<Expression> parse_abstraction();
        unique_ptr<Expression> parse_parenthetical();
        unique_ptr<Expression> parse_non_application();
        unique_ptr<Expression> parse_expression();
        unique_ptr<Expression> parse_assignment();
    public:
        Parser(Lexer l): l(l) { l.fastforward(); }
        unique_ptr<Expression> parse(shared_ptr<Environment> env);
};
