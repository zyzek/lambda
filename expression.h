#include <unordered_set>
#include <vector>

using namespace std;

class NameList {
    private:
        vector<string> names;
    public:
        size_t add_name(string name);
        size_t unused_name();
};

class Expression {
    public:
        virtual ~Expression() {}
        //virtual unique_ptr<Expression> evaluate();
        virtual unordered_set<size_t> free_vars() = 0;
        virtual void substitute(size_t target, shared_ptr<Expression> arg,
                                shared_ptr<NameList> names) = 0;
        virtual unique_ptr<Expression> copy() = 0;
        virtual bool is_var() = 0;
};

class Abstraction: public Expression {
    private:
        size_t binder;
        unique_ptr<Expression> body;
    public:
        //unique_ptr<Expression> apply(unique_ptr<Expression> arg);
        Abstraction(size_t binder, unique_ptr<Expression> body):
                    binder(binder), body(move(body)) {}
        bool is_var() { return false; }
        unique_ptr<Expression> copy();
        unordered_set<size_t> free_vars();
        void substitute(size_t target, shared_ptr<Expression> arg,
                        shared_ptr<NameList> names);
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
                        shared_ptr<NameList> names);
};

class Variable: public Expression {
    private:
       size_t name;
    public:
       Variable(size_t name): name(name) {}
       void substitute(size_t target, shared_ptr<Expression> arg,
                       shared_ptr<NameList> names) { return; }
       size_t get_name() { return name; };
       bool is_var() { return true; }
       unique_ptr<Expression> copy();
       unordered_set<size_t> free_vars();
};
