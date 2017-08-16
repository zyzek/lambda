#include "expression.h"

size_t Environment::add_name(string name) {
    auto index_p = indices.find(name);
    if (index_p != indices.end()) {
        return index_p->second;
    }

    size_t index = names.size();
    names.push_back(name);
    indices[name] = index;
    return index;
}

size_t Environment::unused_name() {
    return add_name("@" + to_string(names.size()));
}

void Abstraction::substitute(size_t target, shared_ptr<Expression> arg,
                             shared_ptr<Environment> names) {
    unordered_set<size_t> arg_free = arg->free_vars();

    // We must check if the free arguments would be bound by this binder
    // and change the binder if it would be.
    if (arg_free.count(binder)) {
        unique_ptr<Variable> new_var(new Variable(names->unused_name()));
        size_t new_name = new_var->get_name();
        body->substitute(binder, move(new_var), names);
        binder = new_name;
    }
    
    // If the body is a variable, and it matches the target, replace it.
    if (body->is_var() && body->free_vars().count(target)) {
        body = arg->copy();
    } else {
        // Recursively perform substitution.
        body->substitute(target, arg, names);
    }
}

void Application::substitute(size_t target, shared_ptr<Expression> arg,
                             shared_ptr<Environment> names) {
    if (left->is_var() && left->free_vars().count(target)) {
        left = arg->copy();
    } else {
        left->substitute(target, arg, names);
    }
    if (right->is_var() && right->free_vars().count(target)) {
        right = arg->copy();
    } else {
        right->substitute(target, arg, names);
    }
}

unique_ptr<Expression> Abstraction::copy() {
    unique_ptr<Expression> copy = unique_ptr<Expression>(new Abstraction(binder,
                                                                         body->copy()));
    return move(copy);
}

unique_ptr<Expression> Application::copy() {
    unique_ptr<Expression> left_copy = left->copy();
    unique_ptr<Expression> right_copy = right->copy();

    unique_ptr<Expression> copy = unique_ptr<Expression>(new Application(move(left_copy),
                                                                         move(right_copy)));
    return move(copy);
}

unique_ptr<Expression> Variable::copy() {
    unique_ptr<Expression> copy = unique_ptr<Expression>(new Variable(name));
    return move(copy);
}

unordered_set<size_t> Abstraction::free_vars() {
    unordered_set<size_t> body_free = body->free_vars();
    body_free.erase(binder);
    return body_free;
}

unordered_set<size_t> Application::free_vars() {
    unordered_set<size_t> left_free = left->free_vars();
    unordered_set<size_t> right_free = right->free_vars();
    right_free.insert(left_free.begin(), left_free.end());
    return right_free;
}

unordered_set<size_t> Variable::free_vars() {
    unordered_set<size_t> s;
    s.insert(name);
    return s;
}

/*
void Abstraction::apply(unique_ptr<Expression> arg) {
    unordered_set<size_t> arg_free = arg->free_vars();

    if (arg_free.count(binder)) {
        // if any free variable in the argument would be bound by
        // this abstraction, then we need to rename the binder and all
        // occurrences it binds.
        
        // Rename stuff: work out how to generate this
        string new_name = undefined;
        body.substitute(binder, new_name
    }


}*/
