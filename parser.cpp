#include <stdexcept>
#include "parser.h"

unique_ptr<Expression> Parser::parse_variable() {
    if (t != t_ident) {
        throw runtime_error(string("Expected identifier; recieved ") + token_string(t));
    }
    unique_ptr<Expression> var(new Variable(names.add_name(l.identifier())));
    t = l.prev();
    return move(var);
}

unique_ptr<Expression> Parser::parse_application() {
    // Parse first the rightmost thing we can parse, that's not an application,
    // to enforce left-associativity.
    unique_ptr<Expression> rhs = parse_non_application();
    unique_ptr<Expression> lhs;
    
    // Parse another left hand side. If we can't then just return the rhs.
    size_t start = l.get_index();
    try {
        unique_ptr<Expression> lhs = parse_application();
    } catch (...) {
        l.set_index(start);
        return move(rhs);
    }

    unique_ptr<Expression> app(new Application(move(lhs), move(rhs)));
    return move(app);
}

unique_ptr<Expression> Parser::parse_abstraction() {
    size_t start = l.get_index();

    // Consume the closing parenthesis
    if (t != t_rparen) {
        throw runtime_error(string("Abstraction expects to close with a right paren; recieved ") + token_string(t));
    }
    t = l.prev();
    
    // parse a body
    unique_ptr<Expression> body = parse_expression();
    
    // consume a dot
    if (t != t_dot) {
        throw runtime_error(string("Abstraction expects a dot after binders; received ") + token_string(t));
    }
    t = l.prev();

    // Consume at least one binder
    if (t != t_ident) {
        throw runtime_error(string("Abstraction expects at least one binder; received ") + token_string(t));
    }
    body = unique_ptr<Expression>(new Abstraction(names.add_name(l.identifier()), move(body)));
    t = l.prev();
    while (t == t_ident) {
        body = unique_ptr<Expression>(new Abstraction(names.add_name(l.identifier()), move(body)));
        t = l.prev();
    }
    
    // parse a backslash
    if (t != t_backslash) {
        throw runtime_error(string("Abstraction expects backslash; recieved ") + token_string(t));
    }
    t = l.prev();
    
    // parse an opening bracket
    if (t != t_lparen) {
        throw runtime_error(string("Abstraction should open with left paren; received ") + token_string(t));
    }
    t = l.prev();

    return move(body);
}

unique_ptr<Expression> Parser::parse_parenthetical() {
    if (t != t_rparen) {
        throw runtime_error(string("Parenthesised expression should end with a right paren; received ") + token_string(t));
    }
    t = l.prev();
    unique_ptr<Expression> e = parse_expression();
    if (t != t_lparen) {
        throw runtime_error(string("Parenthesised expression should begin with a left paren; received ") + token_string(t));
    }
    t = l.prev();
    return e;
}

unique_ptr<Expression> Parser::parse_non_application() {
    size_t start = l.get_index();
    
    // Try a variable
    try {
        return parse_variable();
    } catch (...) {
        l.set_index(start);
    }

    // Try an abstraction
    try {
        return parse_abstraction();
    } catch (...) {
        l.set_index(start);
    }
    
    // Try an abstraction
    try {
        return parse_abstraction();
    } catch (...) {
        l.set_index(start);
    }

    // Try a parenthetical
    try {
        return parse_parenthetical();
    } catch (...) {
        l.set_index(start);
    }

    throw runtime_error("Failed to parse non-application expression.");
}

unique_ptr<Expression> Parser::parse_expression() {
    size_t start = l.get_index();
    
    // Try anything that's not an application
    try {
        return parse_non_application();
    } catch (...) {
        l.set_index(start);
    }

    // Try an application
    try {
        return parse_application();
    } catch (...) {
        l.set_index(start);
    }
    
    throw runtime_error("Failed to parse expression.");
}

unique_ptr<Expression> Parser::parse() {
    unique_ptr<Expression> p = parse_expression();
    if (l.has_prev()) throw runtime_error("Unconsumed input.");
    return move(p);
}
