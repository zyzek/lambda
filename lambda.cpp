#include <string>
#include <iostream>

#include "parser.h"

// TODO: De Bruijn numbering.
// TODO: Smarter evaluation order.
// TODO: Macros
// TODO: canonical normal form and equivalence checking (De bruijn?)

int main() {
    std::cout << "Lambda calculus interpreter. ^D to exit." << std::endl;

    std::string line;
    while (getline(std::cin, line)) {
        try {
            Lexer l(line);
            Parser p(l);
            
            unique_ptr<Expression> e = p.parse();
            Environment names = p.get_names();
            std::cout << e->reduce(names)->str(names) << std::endl;
        } catch (...) {
            std::cout << "Parse failed." << std::endl;
        }
    }

    return 0;
}
