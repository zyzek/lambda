#include <string>
#include <iostream>

#include "parser.h"
#include "lexer.h"

int main() {
    std::cout << "Lambda calculus interpreter. ^D to exit." << std::endl;

    std::string line;
    while (getline(std::cin, line)) {
        Lexer l(line);
        Parser p(l);
        /*enum token t;

        while ((t = l.next()) != t_eol) {
            std::cout << token_string(t);
            if (t == t_ident) std::cout << "(" << l.identifier() << ")";
            std::cout << " ";
        }
        std::cout << std::endl;*/
    }

    return 0;
}
