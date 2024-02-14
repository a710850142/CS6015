// expr.h
#ifndef EXPR_H
#define EXPR_H

#include <string>
#include <sstream>
#include <iostream>
#include <stdexcept>

typedef enum {
    prec_none,
    prec_add,
    prec_mult,
    prec_let
} precedence_t;

class Expr {
public:
    virtual ~Expr() {} // 确保有虚析构函数的实现
    virtual int interp() const = 0;
    virtual bool has_variable() const = 0;
    virtual Expr* subst(const std::string& varName, const Expr* replacement) const = 0;
    virtual bool equals(const Expr* other) const = 0;
    virtual void print(std::ostream &os) const = 0;
    virtual std::string to_string() const = 0; // 确保声明为纯虚函数
    virtual void pretty_print(std::ostream &os) const { print(os); }
    virtual void pretty_print_at(std::ostream &os, precedence_t prec, std::streampos& last_newline) const { pretty_print(os); }
};


class Num : public Expr {
    int value;
public:
    Num(int value) : value(value) {}
    int interp() const override;
    bool has_variable() const override;
    Expr* subst(const std::string& varName, const Expr* replacement) const override;
    bool equals(const Expr* other) const override;
    void print(std::ostream &os) const override;
    std::string to_string() const override; // 实现 to_string 方法
};

class Add : public Expr {
    Expr* lhs;
    Expr* rhs;
public:
    Add(Expr* lhs, Expr* rhs);
    ~Add();
    int interp() const override;
    bool has_variable() const override;
    Expr* subst(const std::string& varName, const Expr* replacement) const override;
    bool equals(const Expr* other) const override;
    void print(std::ostream &os) const override;
    std::string to_string() const override;
    void pretty_print_at(std::ostream &os, precedence_t prec, std::streampos& last_newline) const override;
};

class Mult : public Expr {
    Expr* lhs;
    Expr* rhs;
public:
    Mult(Expr* lhs, Expr* rhs);
    ~Mult();
    int interp() const override;
    bool has_variable() const override;
    Expr* subst(const std::string& varName, const Expr* replacement) const override;
    bool equals(const Expr* other) const override;
    void print(std::ostream &os) const override;
    std::string to_string() const override;
    void pretty_print_at(std::ostream &os, precedence_t prec, std::streampos& last_newline) const override;
};

class VarExpr : public Expr {
    std::string name;
public:
    VarExpr(const std::string& name);
    int interp() const override;
    bool has_variable() const override;
    Expr* subst(const std::string& varName, const Expr* replacement) const override;
    bool equals(const Expr* other) const override;
    void print(std::ostream &os) const override;
    std::string to_string() const override;
};

class LetExpr : public Expr {
    std::string var;
    Expr* rhs;
    Expr* body;
public:
    LetExpr(const std::string& var, Expr* rhs, Expr* body);
    ~LetExpr();
    int interp() const override;
    bool has_variable() const override;
    Expr* subst(const std::string& varName, const Expr* replacement) const override;
    bool equals(const Expr* other) const override;
    void print(std::ostream &os) const override;
    std::string to_string() const override;
    void pretty_print_at(std::ostream &os, precedence_t prec, std::streampos& last_newline) const override;
};

#endif // EXPR_H
