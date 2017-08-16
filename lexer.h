#include <string>
#include <utility>
#include <vector>

using namespace std;

enum token {
    t_lparen,    // '('
    t_rparen,    // ')'
    t_backslash, // '\'
    t_stop,      // '.'
    t_ident,     // strings of characters
    t_eol,       // '\n'
    t_error      // anything unrecognised
};

string token_string(enum token t);

class Lexer {
    private:
        // Pairs of tokens and indexes into the idents vector.
        vector<pair<enum token, ssize_t> > tokens;
        vector<string> idents;

        // Current index into the token sequence.
        size_t token_index;

        // Last identifier string
        string last_ident;
    public:
        Lexer(string input) { tokenise(input); }
        void tokenise(string in);
        enum token next();
        string identifier() { return last_ident; }
        void set_index(size_t i);
};
