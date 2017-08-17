#include "lexer.h"
#include <stdexcept>

std::string token_string(enum token t) {
    switch (t) {
        case t_lparen:
            return "(";
        case t_rparen:
            return ")";
        case t_backslash:
            return "\\";
        case t_dot:
            return ".";
        case t_ident:
            return "ID";
        case t_eol:
            return "EOL";
        default:
            return "ERROR";
    }
}

bool isident(char c) {
    return isalnum(c) || c == '_';
}

void Lexer::tokenise(string input) {
    tokens.clear();
    idents.clear();
    token_index = 0;

    size_t i = 0;
    enum token t;
    
    // Build tokens up while there is input to consume.
    while (i < input.length()) {
        // Skip spaces.
        while (isspace(input[i])) ++i;
        
        // Handle the special language characters
        switch (input[i++]) {
            case '(':
                tokens.push_back(make_pair(t_lparen, -1));
                continue;
            case ')':
                tokens.push_back(make_pair(t_rparen, -1));
                continue;
            case '\\':
                tokens.push_back(make_pair(t_backslash, -1));
                continue;
            case '.':
                tokens.push_back(make_pair(t_dot, -1));
                continue;
        }

        // Everything else, provided that it's alphanumeric or underscore,
        // composes part of an identifier.
        if (isident(input[i-1])) {
            string ident(1, input[i-1]);
            while (i < input.length() && isident(input[i])) {
                ident += input[i++];
            }
            tokens.push_back(make_pair(t_ident, idents.size()));
            idents.push_back(ident);
            continue;
        }
        tokens.push_back(make_pair(t_error, -1));
    }
}

enum token Lexer::next() {
    if (token_index >= tokens.size()) return t_eol;
    pair<enum token, ssize_t> p = tokens[token_index++];
    if (p.first == t_ident) last_ident = idents[p.second];
    return p.first;
}

enum token Lexer::prev() {
    if (token_index == 0) return t_eol;
    pair<enum token, ssize_t> p = tokens[--token_index];
    if (p.first == t_ident) last_ident = idents[p.second];
    return p.first;
}


