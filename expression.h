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
        //virtual unique_ptr<Expression> evaluate();
        virtual unordered_set<size_t> free_vars() = 0;
        virtual void substitute(size_t target, shared_ptr<Expression> arg,
                                shared_ptr<Environment> names) = 0;
        virtual unique_ptr<Expression> copy() = 0;
        virtual bool is_var() = 0;
        virtual string str(Environment *names) = 0;
};

class Abstraction: public Expression {
    private:
        size_t binder;
        unique_ptr<Expression> body;
    public:
        Abstraction(size_t binder, unique_ptr<Expression> body):
                    binder(binder), body(move(body)) {}
        bool is_var() { return false; }
        unique_ptr<Expression> copy();
        unordered_set<size_t> free_vars();
        void substitute(size_t target, shared_ptr<Expression> arg,
                        shared_ptr<Environment> names);
        string str(Environment *names);
};

class Application: public Expression {
    private:
        unique_ptr<Expression> left, right;
    public:
        Application(unique_ptr<Expression> left,
                    unique_ptr<Expression> right):
                   left(move(left)), right(move(right)) {}
        bool is_var() { return false; }
        unique_ptr<Expression> copy();
        unordered_set<size_t> free_vars();
        void substitute(size_t target, shared_ptr<Expression> arg,
                        shared_ptr<Environment> names);
        string str(Environment *names);
};

class Variable: public Expression {
    private:
       size_t name;
    public:
       Variable(size_t name): name(name) {}
       void substitute(size_t target, shared_ptr<Expression> arg,
                       shared_ptr<Environment> names) { return; }
       size_t get_name() { return name; };
       bool is_var() { return true; }
       unique_ptr<Expression> copy();
       unordered_set<size_t> free_vars();
       string str(Environment *names);
};
