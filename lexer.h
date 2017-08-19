#pragma once
#include <string>
#include <utility>
#include <vector>

using namespace std;

enum token {
    t_lparen,    // '('
    t_rparen,    // ')'
    t_backslash, // '\'
    t_dot,      // '.'
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
        enum token prev();
        bool has_next() { return token_index < tokens.size(); }
        bool has_prev() { return token_index > 0; }
        enum token match_next(enum token expected);
        enum token match_prev(enum token expected);
        void rewind() { token_index = 0; }
        void fastforward() { token_index = tokens.size(); }
        string identifier() { return last_ident; }
        void set_index(size_t i) { token_index = i; }
        size_t get_index() { return token_index; }
};
