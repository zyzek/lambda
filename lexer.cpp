#include "lexer.h"

std::string token_string(enum token t) {
    switch (t) {
        case t_lparen:
            return "(";
        case t_rparen:
            return ")";
        case t_backslash:
            return "\\";
        case t_stop:
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

enum token Lexer::next() {
    // End when there is no more input to consume.
    if (i >= input.length()) return t_eol;
    
    // Skip spaces
    while (isspace(input[i])) ++i;
    
    // Handle the special language characters
    switch (input[i++]) {
        case '(':
            return t_lparen;
        case ')':
            return t_rparen;
        case '\\':
            return t_backslash;
        case '.':
            return t_stop;
    }
    
    // Everything else, provided that it's alphanumeric or underscore,
    // composes part of an identifier.
    if (isident(input[i-1])) {
        last_ident = input[i-1];
        //std::string ident(1, input[i-1]);
        while (i < input.length() && isident(input[i])) {
            last_ident += input[i];
            ++i;
        }
        return t_ident;
    }
    
    return t_error;
}
