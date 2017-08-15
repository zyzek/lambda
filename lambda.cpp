#include <string>
#include <iostream>

#include "lexer.h"


int main() {
   
    std::string line;

    std::cout << "Lambda calculus interpreter. ^D to exit." << std::endl;

    while (getline(std::cin, line)) {
        Lexer l(line);
        enum token t;

        while ((t = l.next()) != t_eol) {
            std::cout << token_string(t);
            if (t == t_ident) std::cout << "(" << l.identifier() << ")";
            std::cout << " ";
        }

        std::cout << std::endl;
    }

    return 0;
}
