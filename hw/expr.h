#ifndef MSDSCRIPT_EXPR_H
#define MSDSCRIPT_EXPR_H

#include <sstream>
#include <string>
#include "pointer.h"

class Val;
class Env;

enum precedence_t {
    prec_none,
    prec_equal,
    prec_add,
    prec_mult
};


CLASS(Expr) {
public:
    virtual bool equals(PTR(Expr) expr)=0;
    virtual PTR(Val) interp(PTR(Env) env) = 0;
    virtual PTR(Expr) subst(std::string s, PTR(Expr) expr) = 0;
    virtual void print(std::ostream& out) = 0;
    std::string to_string();
    void pretty_print(std::ostream& out);
    virtual void pretty_print_at(std::ostream& out, precedence_t precedence, std::streampos& newLinePrevPos, bool addParenthesesToLet) = 0;
    std::string to_pretty_string();
};

class NumExpr : public Expr {
private:
    int val;
public:
    NumExpr(int v);
    bool equals(PTR(Expr) expr) override;
    PTR(Val) interp(PTR(Env) env) override;
    PTR(Expr) subst(std::string s, PTR(Expr) expr) override;
    void print(std::ostream& out) override;
    void pretty_print_at(std::ostream& out, precedence_t precedence, std::streampos& newLinePrevPos, bool addParen) override;
};


class AddExpr : public Expr {
private:
    PTR(Expr) lhs;
    PTR(Expr) rhs;
public:
    AddExpr(PTR(Expr) left, PTR(Expr) right);
    AddExpr(int left, int right);
    AddExpr(std::string left, int right);
    AddExpr(int left, std::string right);
    AddExpr(std::string left, std::string right);
    AddExpr(int left, PTR(Expr) right);
    AddExpr(PTR(Expr) left, int right);
    AddExpr(std::string left, PTR(Expr) right);
    AddExpr(PTR(Expr) left, std::string right);
    bool equals(PTR(Expr) expr) override;
    PTR(Val) interp(PTR(Env) env) override;
    PTR(Expr) subst(std::string s, PTR(Expr) expr) override;
    void print(std::ostream& out) override;
    void pretty_print_at(std::ostream& out, precedence_t precedence, std::streampos& newLinePrevPos, bool addParen) override;
};


class MultExpr : public Expr {
public:
    PTR(Expr) lhs;
    PTR(Expr) rhs;
public:
    MultExpr(PTR(Expr) left, PTR(Expr) right);
    MultExpr(int left, int right);
    MultExpr(std::string left, int right);
    MultExpr(int left, std::string right);
    MultExpr(std::string left, std::string right);
    MultExpr(int left, PTR(Expr) right);
    MultExpr(PTR(Expr) left, int right);
    MultExpr(std::string left, PTR(Expr) right);
    MultExpr(PTR(Expr) left, std::string right);
    bool equals(PTR(Expr) expr) override;
    PTR(Val) interp(PTR(Env) env) override;
    PTR(Expr) subst(std::string s, PTR(Expr) expr) override;
    void print(std::ostream& out) override;
    void pretty_print_at(std::ostream& out, precedence_t precedence, std::streampos& newLinePrevPos, bool addParen) override;
};


class VarExpr : public Expr {
private:
    std::string val;
public:
    VarExpr(std::string s);
    bool equals(PTR(Expr) expr) override;
    PTR(Val) interp(PTR(Env) env) override;
    PTR(Expr) subst(std::string s, PTR(Expr) expr) override;
    void print(std::ostream& out) override;
    void pretty_print_at(std::ostream& out, precedence_t prec, std::streampos& newLinePrevPos, bool addParen) override;
};


class LetExpr : public Expr {
private:
    std::string variable;
    PTR(Expr) rhs;
    PTR(Expr) body;
public:
    LetExpr(std::string v, PTR(Expr) r, PTR(Expr) b);
    bool equals(PTR(Expr) expr) override;
    PTR(Val) interp(PTR(Env) env) override;
    PTR(Expr) subst(std::string s, PTR(Expr) expr) override;
    void print(std::ostream& out) override;
    void pretty_print_at(std::ostream& out, precedence_t precedence, std::streampos& newLinePrevPos, bool addParen) override;
};

class BoolExpr : public Expr {
private:
    bool val;
public:
    BoolExpr(bool v);
    bool equals(PTR(Expr) rhs) override;
    PTR(Val) interp(PTR(Env) env) override;
    PTR(Expr) subst(std::string s, PTR(Expr) expr) override;
    void print(std::ostream& out) override;
    void pretty_print_at(std::ostream& out, precedence_t precedence, std::streampos& newLinePrevPos, bool addParen) override;
};

class IfExpr : public Expr {
private:
    PTR(Expr) test_part;
    PTR(Expr) then_part;
    PTR(Expr) else_part;
public:
    IfExpr(PTR(Expr) test, PTR(Expr) then, PTR(Expr) else_);
    IfExpr(bool test, PTR(Expr) then, PTR(Expr) else_);
    bool equals(PTR(Expr) rhs) override;
    PTR(Val) interp(PTR(Env) env) override;
    PTR(Expr) subst(std::string s, PTR(Expr) expr) override;
    void print(std::ostream& out) override;
    void pretty_print_at(std::ostream& out, precedence_t precedence, std::streampos& newLinePrevPos, bool addParen) override;
};

class EqExpr : public Expr {
private:
    PTR(Expr) lhs;
    PTR(Expr) rhs;
public:
    EqExpr(PTR(Expr) left, PTR(Expr) right);
    EqExpr(int left, int right);
    EqExpr(std::string left, int right);
    bool equals(PTR(Expr) rhs_) override;
    PTR(Val) interp(PTR(Env) env) override;
    PTR(Expr) subst(std::string s, PTR(Expr) expr) override;
    void print(std::ostream& out) override;
    void pretty_print_at(std::ostream& out, precedence_t precedence, std::streampos& newLinePrevPos, bool addParen) override;
};

class FunExpr : public Expr {
private:
    std::string formal_arg;
    PTR(Expr) body;
public:
    FunExpr(std::string arg, PTR(Expr) expr);
    bool equals(PTR(Expr) rhs) override;
    PTR(Val) interp(PTR(Env) env) override;
    PTR(Expr) subst(std::string s, PTR(Expr) expr) override;
    void print(std::ostream& out) override;
    void pretty_print_at(std::ostream& out, precedence_t precedence, std::streampos& newLinePrevPos, bool addParen) override;
};

class CallExpr : public Expr {
private:
    PTR(Expr) to_be_called;
    PTR(Expr) actual_arg;
public:
    CallExpr(PTR(Expr) func, PTR(Expr) arg);
    CallExpr(PTR(Expr) func, int n);
    CallExpr(std::string funcName, int n);
    CallExpr(std::string funcName, PTR(Expr) arg);
    CallExpr(std::string funcName1, std::string funcName2);
    bool equals(PTR(Expr) rhs) override;
    PTR(Val) interp(PTR(Env) env) override;
    PTR(Expr) subst(std::string s, PTR(Expr) expr) override;
    void print(std::ostream& out) override;
    void pretty_print_at(std::ostream& out, precedence_t precedence, std::streampos& newLinePrevPos, bool addParen) override;
};

#endif //MSDSCRIPT_EXPR_H