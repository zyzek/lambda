#include <string>
#include <iostream>

#include "parser.h"


/* An interpreter for the lambda calculus (enhanced with macros), which is decribed by the following grammar:
 *
 * statement := assignment | expression
 * assignment = variable = expression
 * expression := variable | application | abstraction | (expression)
 * application := expression expression
 * abstraction := (\ variable+ . expression)
 */

// TODO: Macros
// TODO: nullaries: (), (\x.)
// TODO: apply auto, make_unique, const
// TODO: command history
// TODO: remove pairs in lexer
// TODO: De Bruijn numbering.
// TODO: Efficiency (derecurse evaluation)
// TODO: canonical normal form and equivalence checking (De bruijn?)
// TODO: Smarter evaluation, so that any expression with a normal form will arrive at it, even if some evaluation orders do not terminate

int main() {
    std::cout << "Lambda calculus interpreter. ^D to exit." << std::endl;

    shared_ptr<Environment> env(new Environment());
    std::string line;
    while (getline(std::cin, line)) {
        try {
            Lexer l(line);
            Parser p(l);

            unique_ptr<Expression> e = p.parse(env);
            if (e) {
                bool recheck = true;
                while (recheck) {
                    recheck = false;
                    auto free_vars = e->free_vars();
                    for (auto i = free_vars.begin(); i != free_vars.end(); ++i) {
                        if (env->has_expression(*i)) {
                            auto s = env->get_expression(*i);
                            e = e->substitute(*i, s, *env);
                            recheck = true;
                        }
                    }
                }
                std::cout << e->reduce(*env)->str(*env) << std::endl;
            }
        } catch (...) {
            std::cout << "Parse failed." << std::endl;
        }
    }

    return 0;
}
