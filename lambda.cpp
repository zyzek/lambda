#include <string>
#include <iostream>
#include <fstream>

#include "parser.h"

using namespace std;

/* An interpreter for the lambda calculus (enhanced with macros), which is decribed by the following grammar:
 *
 * statement := assignment | expression
 * assignment = variable = expression
 * expression := variable | application | abstraction | (expression)
 * application := expression expression
 * abstraction := (\ variable+ . expression)
 */

// TODO: nullaries: (), (\x.)
// TODO: apply auto, make_unique, const
// TODO: command history
// TODO: remove pairs in lexer
// TODO: De Bruijn numbering.
// TODO: Efficiency (derecurse evaluation)
// TODO: canonical normal form and equivalence checking (De bruijn?)
// TODO: Smarter evaluation, so that any expression with a normal form will arrive at it, even if some evaluation orders do not terminate

int main() {
    cout << "Lambda calculus interpreter. ^D to exit." << endl;

    // Initialise parser
    Parser p;
    shared_ptr<Environment> env = p.get_env();
    string line;

    // Read macros from the input file.
    ifstream macros("macros.lambda");
    if (macros.is_open()) {
        while (getline(macros, line)) {
            try {
                p.get_lexer()->tokenise(line);
                p.parse();
            } catch (...) { }
        }
        macros.close();
    } else {
        cout << "Failed to load macro definitions from 'macros.lambda'." << endl;
    }

    // Begin REPL
    size_t max = 1000;
    while (getline(std::cin, line)) {
        try {
            p.get_lexer()->tokenise(line);
            unique_ptr<Expression> e = p.parse();
            if (e) {
                // Substitute values in for defined free variables until we can't do so any longer.
                // Not guaranteed to halt; no cycle detection. So add a max iteration count.
                size_t i = 0;
                bool recheck = true;
                while (recheck && i++ < max) {
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

                // Reduce the actual expression, if it terminated
                if (i >= max) {
                    cout << "Expression failed to fix after " << max << " iterations during name substitution." << endl;
                } else {
                    cout << e->reduce(*env)->str(*env) << endl;
                }
            }
        } catch (...) {
            cout << "Parse failed." << endl;
        }
    }

    return 0;
}
