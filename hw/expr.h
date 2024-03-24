#ifndef MSDSCRIPT_EXPR_H
#define MSDSCRIPT_EXPR_H

#include <sstream>
#include <string>

class Val;

enum precedence_t {
    prec_none,
    prec_equal,
    prec_add,
    prec_mult
};


class Expr {
public:
    virtual bool equals(Expr* expr)=0;
    virtual Val* interp() = 0;
    virtual Expr* subst(std::string s, Expr* expr) = 0;
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
    bool equals(Expr* expr) override;
    Val* interp() override;
    Expr* subst(std::string s, Expr* expr) override;
    void print(std::ostream& out) override;
    void pretty_print_at(std::ostream& out, precedence_t precedence, std::streampos& newLinePrevPos, bool addParen) override;
};


class AddExpr : public Expr {
private:
    Expr* lhs;
    Expr* rhs;
public:
    AddExpr(Expr* left, Expr* right);
    AddExpr(int left, int right);
    AddExpr(std::string left, int right);
    AddExpr(int left, std::string right);
    AddExpr(std::string left, std::string right);
    AddExpr(int left, Expr* right);
    AddExpr(Expr* left, int right);
    AddExpr(std::string left, Expr* right);
    AddExpr(Expr* left, std::string right);
    bool equals(Expr* expr) override;
    Val* interp() override;
    Expr* subst(std::string s, Expr* expr) override;
    void print(std::ostream& out) override;
    void pretty_print_at(std::ostream& out, precedence_t precedence, std::streampos& newLinePrevPos, bool addParen) override;
};


class MultExpr : public Expr {
public:
    Expr* lhs;
    Expr* rhs;
public:
    MultExpr(Expr *left, Expr *right);
    MultExpr(int left, int right);
    MultExpr(std::string left, int right);
    MultExpr(int left, std::string right);
    MultExpr(std::string left, std::string right);
    MultExpr(int left, Expr* right);
    MultExpr(Expr* left, int right);
    MultExpr(std::string left, Expr* right);
    MultExpr(Expr* left, std::string right);
    bool equals(Expr* expr) override;
    Val* interp() override;
    Expr* subst(std::string s, Expr* expr) override;
    void print(std::ostream& out) override;
    void pretty_print_at(std::ostream& out, precedence_t precedence, std::streampos& newLinePrevPos, bool addParen) override;
};


class VarExpr : public Expr {
private:
    std::string val;
public:
    VarExpr(std::string s);
    bool equals(Expr* expr) override;
    Val* interp() override;
    Expr* subst(std::string s, Expr* expr) override;
    void print(std::ostream& out) override;
    void pretty_print_at(std::ostream& out, precedence_t prec, std::streampos& newLinePrevPos, bool addParen) override;
    std::string getVal();
};


class LetExpr : public Expr {
private:
    std::string variable;
    Expr* rhs;
    Expr* body;
public:
    LetExpr(std::string v, Expr* r, Expr* b);
    bool equals(Expr* expr) override;
    Val* interp() override;
    Expr* subst(std::string s, Expr* expr) override;
    void print(std::ostream& out) override;
    void pretty_print_at(std::ostream& out, precedence_t precedence, std::streampos& newLinePrevPos, bool addParen) override;
};

class BoolExpr : public Expr {
private:
    bool val;
public:
    BoolExpr(bool v);
    bool equals(Expr* rhs) override;
    Val* interp() override;
    Expr* subst(std::string s, Expr* expr) override;
    void print(std::ostream& out) override;
    void pretty_print_at(std::ostream& out, precedence_t precedence, std::streampos& newLinePrevPos, bool addParen) override;
};

class IfExpr : public Expr {
private:
    Expr* test_part;
    Expr* then_part;
    Expr* else_part;
public:
    IfExpr(Expr* test, Expr* then, Expr* else_);
    IfExpr(bool test, Expr* then, Expr* else_);
    bool equals(Expr* rhs) override;
    Val* interp() override;
    Expr* subst(std::string s, Expr* expr) override;
    void print(std::ostream& out) override;
    void pretty_print_at(std::ostream& out, precedence_t precedence, std::streampos& newLinePrevPos, bool addParen) override;
};

class EqExpr : public Expr {
private:
    Expr* lhs;
    Expr* rhs;
public:
    EqExpr(Expr* left, Expr* right);
    EqExpr(int left, int right);
    EqExpr(std::string left, int right);
    bool equals(Expr* rhs_) override;
    Val* interp() override;
    Expr* subst(std::string s, Expr* expr) override;
    void print(std::ostream& out) override;
    void pretty_print_at(std::ostream& out, precedence_t precedence, std::streampos& newLinePrevPos, bool addParen) override;
};

class FunExpr : public Expr {
private:
    std::string formal_arg;
    Expr* body;
public:
    FunExpr(std::string arg, Expr* expr);
    bool equals(Expr* rhs) override;
    Val* interp() override;
    Expr* subst(std::string s, Expr* expr) override;
    void print(std::ostream& out) override;
    void pretty_print_at(std::ostream& out, precedence_t precedence, std::streampos& newLinePrevPos, bool addParen) override;
};

class CallExpr : public Expr {
private:
    Expr* to_be_called;
    Expr* actual_arg;
public:
    CallExpr(Expr* func, Expr* arg);
    CallExpr(Expr* func, int n);
    CallExpr(std::string funcName, int n);
    CallExpr(std::string funcName, Expr* arg);
    CallExpr(std::string funcName1, std::string funcName2);
    bool equals(Expr* rhs) override;
    Val* interp() override;
    Expr* subst(std::string s, Expr* expr) override;
    void print(std::ostream& out) override;
    void pretty_print_at(std::ostream& out, precedence_t precedence, std::streampos& newLinePrevPos, bool addParen) override;
};

#endif //MSDSCRIPT_EXPR_H