#pragma once
#include "lexer.h"
#include "expression.h"


using namespace std;

/* expression := variable | application | abstraction | (expression)
 * variable := identifier
 * application := expression expression
 * abstraction := ( \ x y z . expression )
 *
 * application := expression (variable | abstraction) expression
 */

class Parser {
    private:
        Lexer l;
        token t;
        Environment names;

        unique_ptr<Expression> parse_variable();
        unique_ptr<Expression> parse_application();
        unique_ptr<Expression> parse_abstraction();
        unique_ptr<Expression> parse_parenthetical();
        unique_ptr<Expression> parse_non_application();
        unique_ptr<Expression> parse_expression();
    public:
        Parser(Lexer l): l(l) {}
        unique_ptr<Expression> parse();
};
