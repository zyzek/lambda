#include <string>

enum token {
    t_lparen,    // '('
    t_rparen,    // ')'
    t_backslash, // '\'
    t_stop,      // '.'
    t_ident,     // strings of characters
    t_eol,       // '\n'
    t_error      // anything unrecognised
};

std::string token_string(enum token t);

class Lexer {
    private:
        std::string input;      // String currently being tokenised
        size_t i;               // Current index into the string
        std::string last_ident; // Last extracted identifier.
    public:
        Lexer(std::string input): input(input), i(0) {}
        enum token next();
        std::string &identifier() { return last_ident; }
};
