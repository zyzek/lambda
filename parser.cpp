#include <stdexcept>
#include "parser.h"

unique_ptr<Expression> Parser::parse_variable() {
    string ident = l.match_prev_ident();
    unique_ptr<Expression> var(new Variable(names->add_name(ident)));
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
        lhs = parse_expression();
    } catch (...) {
        l.set_index(start);
        return move(rhs);
    }

    unique_ptr<Expression> app(new Application(move(lhs), move(rhs)));
    return move(app);
}

unique_ptr<Expression> Parser::parse_abstraction() {
    // Consume the closing parenthesis
    l.match_prev(t_rparen);

    // parse a body
    unique_ptr<Expression> body = parse_expression();

    // Match the opening section of the abstraction: "(\ <binders>."
    l.match_prev(t_dot);

    // There must be at least one binder
    string ident = l.match_prev_ident();
    body = unique_ptr<Expression>(new Abstraction(names->add_name(ident), move(body)));

    // ...and further binders produce nested abstractions.
    try {
        while (l.match_prev(t_ident) == t_ident) {
            body = unique_ptr<Expression>(new Abstraction(names->add_name(l.identifier()), move(body)));
        }
    } catch (...) {}

    l.match_prev(t_backslash);
    l.match_prev(t_lparen);

    return move(body);
}

unique_ptr<Expression> Parser::parse_parenthetical() {
    l.match_prev(t_rparen);
    unique_ptr<Expression> e = parse_expression();
    l.match_prev(t_lparen);
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

    // Try an application
    try {
        return parse_application();
    } catch (...) {
        l.set_index(start);
    }

    // Try anything that's not an application
    try {
        return parse_non_application();
    } catch (...) {
        l.set_index(start);
    }

    throw runtime_error("Failed to parse expression.");
}

unique_ptr<Expression> Parser::parse_assignment() {
    // Parse the expression being assigned.
    auto val = parse_expression();

    // Parse an equals sign, and the variable name being assigned to.
    l.match_prev(t_equals);
    string ident = l.match_prev_ident();

    // Map the name to the expression in the environment, and return.
    names->add_name(ident);
    names->set_expression(ident, val);
    return move(val);
}

unique_ptr<Expression> Parser::parse(shared_ptr<Environment> env) {
    names = env;

    l.fastforward();
    unique_ptr<Expression> p;
    try {
        parse_assignment();
    } catch(...) {
        l.fastforward();
        p = parse_expression();
    }

    if (l.has_prev()) throw runtime_error("Unconsumed input.");
    return move(p);
}
