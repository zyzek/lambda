#include "expression.h"

// The environment stores a correspondence between names and indices.
// The actual expressions only hold indices, not the names themselves.

size_t Environment::add_name(string name) {
    // First check if the name is not already known: return its index if it is.
    auto index_p = indices.find(name);
    if (index_p != indices.end()) {
        return index_p->second;
    }

    // Assign new indices serially.
    size_t index = names.size();
    names.push_back(name);
    indices[name] = index;
    return index;
}

size_t Environment::unused_name() {
    // TODO: Make this a little more attractive, so that it assigns letters, if it can.
    // We ensure this assigned name is new, by ensuring its name is also its index (with "@" prepended).
    return add_name("@" + to_string(names.size()));
}

unique_ptr<Expression> Environment::get_expression(size_t i) { return expressions[i]->copy(); }

unique_ptr<Expression> Environment::get_expression(string name) { return get_expression(indices[name]); }

void Environment::set_expression(size_t i, unique_ptr<Expression>& e) { expressions[i] = e->copy(); }

void Environment::set_expression(string name, unique_ptr<Expression>& e) { set_expression(indices[name], e); }

bool Environment::has_expression(size_t i) { return (expressions.count(i) != 0); }

bool Environment::has_expression(string name) { return has_expression(indices[name]); }


// Define the steps that can reduce an expression to its unique (alpha-equivalent) normal form.

unique_ptr<Expression> Abstraction::reduce(Environment& names) {
    // TODO: Eta reduction
    return move(unique_ptr<Expression>(new Abstraction(binder, body->reduce(names))));
}

unique_ptr<Expression> Application::reduce(Environment& names) {
    // Reduce the components of this application to normal form
    auto l_red = left->reduce(names);
    auto r_red = right->reduce(names);

    // If left expression is an abstraction, apply beta-reduction.
    if (l_red->is_abs()) {
        // If the right hand side is an abstraction, we may need to further reduce the expression, post beta-reduction.
        if (r_red->is_abs()) {
            return l_red->apply(r_red, names)->reduce(names);
        }
        return l_red->apply(r_red, names);
    }

    return move(unique_ptr<Expression>(new Application(move(l_red), move(r_red))));
}

unique_ptr<Expression> Variable::reduce(Environment& names) {
    // Variables are already in normal form.
    return copy();
}


// Substitute one expression for a target variable in another expression, performing alpha-conversion
// where necessary in order to avoid variable capture.

unique_ptr<Expression> Abstraction::substitute(size_t target, unique_ptr<Expression>& arg, Environment& names) {
    // Before performing the actual substitution, we must check if the free arguments would be bound by this binder,
    // and alpha-convert if it would be. We then substitute recursively into the body.
    unordered_set<size_t> arg_free = arg->free_vars();
    if (arg_free.count(binder)) {
        size_t new_name = names.unused_name();
        unique_ptr<Expression> new_var(new Variable(new_name));
        unique_ptr<Expression> sub(new Abstraction(new_name, body->substitute(binder, new_var, names)));
        return sub->substitute(target, arg, names);
    } else {
        return move(unique_ptr<Expression>(new Abstraction(binder, body->substitute(target, arg, names))));
    }
}

unique_ptr<Expression> Application::substitute(size_t target, unique_ptr<Expression>& arg, Environment& names) {
    // Recursively substitute into this application's subexpressions.
    auto l = left->substitute(target, arg, names);
    auto r = right->substitute(target, arg, names);
    return move(unique_ptr<Expression>(new Application(move(l), move(r))));
}

unique_ptr<Expression> Variable::substitute(size_t target, unique_ptr<Expression>& arg, Environment& names) {
    // Replace this variable with the argument if it matches the target.
    if (target == name) {
        return arg->copy();
    }
    return this->copy();
}


// Apply a one expression to another. This will only have an effect if the left item is an abstraction.

unique_ptr<Expression> Abstraction::apply(unique_ptr<Expression>& arg, Environment& names) {
    // Beta-reduction: substitute the argument into the body, and drop the binder.
    return body->substitute(binder, arg, names);
}


// Return a copy of an expression.

unique_ptr<Expression> Abstraction::copy() {
    return move(unique_ptr<Expression>(new Abstraction(binder, body->copy())));
}

unique_ptr<Expression> Application::copy() {
    return move(unique_ptr<Expression>(new Application(left->copy(), right->copy())));
}

unique_ptr<Expression> Variable::copy() {
    return move(unique_ptr<Expression>(new Variable(name)));
}


// Return the set of variables which occur in a given expression which are not bound by any abstraction within it.

unordered_set<size_t> Abstraction::free_vars() {
    // An abstraction removes the bound variable from the set of free variables in the body.
    unordered_set<size_t> body_free = body->free_vars();
    body_free.erase(binder);
    return body_free;
}

unordered_set<size_t> Application::free_vars() {
    // The free variables in an application are the union of the free variables in its subexpressions.
    unordered_set<size_t> left_free = left->free_vars();
    unordered_set<size_t> right_free = right->free_vars();
    right_free.insert(left_free.begin(), left_free.end());
    return right_free;
}

unordered_set<size_t> Variable::free_vars() {
    // A variable is free in itself.
    unordered_set<size_t> s;
    s.insert(name);
    return s;
}


// Return a string representation of the given expression.

string Abstraction::str(Environment& names) {
    return "(\\" + names.get_name(binder) + ". " + body->str(names) + ")";
}

string Application::str(Environment& names) {
    // Parenthesise the RHS if it's an application, to ensure associativity is correct.
    if (right->is_app()) {
        return left->str(names) + " (" + right->str(names) + ")";
    } else {
        return left->str(names) + " " + right->str(names);
    }
}

string Variable::str(Environment& names) {
    return names.get_name(name);
}
