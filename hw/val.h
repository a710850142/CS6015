#ifndef MSDSCRIPT_VAL_H
#define MSDSCRIPT_VAL_H

#include <string>
#include "pointer.h"

class Expr;

CLASS(Val) {
        public:
        virtual PTR(Expr) to_expr() = 0;
        virtual bool equals(PTR(Val) rhs) = 0;
        virtual PTR(Val) add_to(PTR(Val) rhs) = 0;
        virtual PTR(Val) mult_with(PTR(Val) rhs) = 0;
        virtual std::string to_string() = 0;
        virtual bool is_true() = 0;
        virtual PTR(Val) call(PTR(Val) actual_arg) = 0;
};

class NumVal : public Val {
private:
    int val;
public:
    NumVal(int v);
    PTR(Expr)  to_expr() override;
    bool equals(PTR(Val) rhs) override;
    PTR(Val) add_to(PTR(Val) rhs) override;
    PTR(Val) mult_with(PTR(Val) rhs) override;
    std::string to_string() override;
    bool is_true() override;
    virtual PTR(Val) call(PTR(Val) actual_arg) override;
};

class BoolVal : public Val {
private:
    bool val;
public:
    BoolVal(bool v);
    PTR(Expr) to_expr() override;
    bool equals(PTR(Val) rhs) override;
    PTR(Val) add_to(PTR(Val) rhs) override;
    PTR(Val) mult_with(PTR(Val) rhs) override;
    std::string to_string() override;
    bool is_true() override;
    virtual PTR(Val) call(PTR(Val) actual_arg) override;
};

class FunVal : public Val {
private:
    std::string formal_arg;
    PTR(Expr) body;
public:
    FunVal(std::string arg, PTR(Expr) expr);
    PTR(Expr) to_expr() override;
    bool equals(PTR(Val) rhs) override;
    PTR(Val) add_to(PTR(Val) rhs) override;
    PTR(Val) mult_with(PTR(Val) rhs) override;
    std::string to_string() override;
    bool is_true() override;
    virtual PTR(Val) call(PTR(Val) actual_arg) override;
};

#endif //MSDSCRIPT_VAL_H