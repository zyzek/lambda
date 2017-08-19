#include <string>
#include <iostream>

#include "parser.h"

int main() {
    std::cout << "Lambda calculus interpreter. ^D to exit." << std::endl;

    std::string line;
    while (getline(std::cin, line)) {
        
        try { 
            Lexer l(line);
            Parser p(l);
            
            unique_ptr<Expression> e = p.parse();
            Environment names = p.get_names();
            std::cout << e->str(&names) << std::endl;
        } catch (...) {
            std::cout << "Parse failed." << std::endl;
        }
    }

    return 0;
}
