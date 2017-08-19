#pragma once
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <string>

using namespace std;

class Environment {
    private:
        vector<string> names;
        unordered_map<string, size_t> indices;
    public:
        size_t add_name(string name);
        size_t unused_name();
        string get_name(size_t i) { return names[i]; }
};

class Expression {
    public:
        virtual ~Expression() {}

        virtual unique_ptr<Expression> reduce(Environment& names) = 0;
        virtual unique_ptr<Expression> substitute(size_t target, unique_ptr<Expression>& arg, Environment& names) = 0;
        virtual unique_ptr<Expression> apply(unique_ptr<Expression>& arg, Environment& names) = 0;
        virtual unique_ptr<Expression> copy() = 0;
        virtual unordered_set<size_t> free_vars() = 0;
        virtual string str(Environment& names) = 0;

        virtual bool is_var() = 0;
        virtual bool is_app() = 0;
        virtual bool is_abs() = 0;
};

class Abstraction: public Expression {
    private:
        size_t binder;
        unique_ptr<Expression> body;
    public:
        Abstraction(size_t binder, unique_ptr<Expression> body):
                    binder(binder), body(move(body)) {}

        unique_ptr<Expression> reduce(Environment& names);
        unique_ptr<Expression> substitute(size_t target, unique_ptr<Expression>& arg, Environment& names);
        unique_ptr<Expression> apply(unique_ptr<Expression>& arg, Environment& names);
        unique_ptr<Expression> copy();
        unordered_set<size_t> free_vars();
        string str(Environment& names);

        bool is_var() { return false; }
        bool is_app() { return false; }
        bool is_abs() { return true; }

        size_t get_binder() { return binder; }
};

class Application: public Expression {
    private:
        unique_ptr<Expression> left, right;
    public:
        Application(unique_ptr<Expression> left,
                    unique_ptr<Expression> right):
                   left(move(left)), right(move(right)) {}

        unique_ptr<Expression> reduce(Environment& names);
        unique_ptr<Expression> substitute(size_t target, unique_ptr<Expression>& arg, Environment& names);
        unique_ptr<Expression> apply(unique_ptr<Expression>& arg, Environment& names) { return copy(); }
        unique_ptr<Expression> copy();
        unordered_set<size_t> free_vars();
        string str(Environment& names);

        bool is_var() { return false; }
        bool is_app() { return true; }
        bool is_abs() { return false; }
};

class Variable: public Expression {
    private:
       size_t name;
    public:
        Variable(size_t name): name(name) {}

        unique_ptr<Expression> reduce(Environment& names);
        unique_ptr<Expression> substitute(size_t target, unique_ptr<Expression>& arg, Environment& names);
        unique_ptr<Expression> apply(unique_ptr<Expression>& arg, Environment& names) { return copy(); }
        unique_ptr<Expression> copy();
        unordered_set<size_t> free_vars();
        string str(Environment& names);

        bool is_var() { return true; }
        bool is_app() { return false; }
        bool is_abs() { return false; }

        size_t get_name() { return name; };
};
