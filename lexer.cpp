#include "lexer.h"

#define NA (-1)

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
                tokens.push_back(make_pair(t_lparen, NA));
                continue;
            case ')':
                tokens.push_back(make_pair(t_rparen, NA));
                continue;
            case '\\':
                tokens.push_back(make_pair(t_backslash, NA));
                continue;
            case '.':
                tokens.push_back(make_pair(t_dot, NA));
                continue;
            case '=':
                tokens.push_back(make_pair(t_equals, NA));
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
        tokens.push_back(make_pair(t_error, NA));
    }
}

enum token Lexer::peek(size_t i) {
    if (token_index >= tokens.size()) return t_eol;
    return tokens[i].first;
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

enum token Lexer::match_next(enum token expected) {
    enum token t = next();
    if (t != expected) {
        prev();
        throw runtime_error("Expected " + token_string(expected) + ", but received " + token_string(t) + ".");
    }
    return t;
}

enum token Lexer::match_prev(enum token expected) {
    enum token t = prev();
    if (t != expected) {
        next();
        throw runtime_error("Expected " + token_string(expected) + ", but received " + token_string(t) + ".");
    }
    return t;
}

string Lexer::match_next_ident() {
    match_next(t_ident);
    return identifier();
}

string Lexer::match_prev_ident() {
    match_prev(t_ident);
    return identifier();
}
