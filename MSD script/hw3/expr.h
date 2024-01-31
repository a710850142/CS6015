#ifndef EXPR_H
#define EXPR_H

#include <string>
#include <stdexcept>

class Expr {
public:
    virtual ~Expr() {}

    virtual int interp() const = 0;
    virtual bool has_variable() const = 0;
    virtual Expr* subst(const std::string& varName, const Expr* replacement) const = 0;
    virtual bool equals(const Expr* other) const = 0;
};

class Num : public Expr {
    int value;
public:
    Num(int value) : value(value) {}

    int interp() const override;
    bool has_variable() const override;
    Expr* subst(const std::string& varName, const Expr* replacement) const override;
    bool equals(const Expr* other) const override;
};

class Add : public Expr {
    Expr* lhs;
    Expr* rhs;
public:
    Add(Expr* lhs, Expr* rhs) : lhs(lhs), rhs(rhs) {}
    ~Add();

    int interp() const override;
    bool has_variable() const override;
    Expr* subst(const std::string& varName, const Expr* replacement) const override;
    bool equals(const Expr* other) const override;
};

class Mult : public Expr {
    Expr* lhs;
    Expr* rhs;
public:
    Mult(Expr* lhs, Expr* rhs) : lhs(lhs), rhs(rhs) {}
    ~Mult();

    int interp() const override;
    bool has_variable() const override;
    Expr* subst(const std::string& varName, const Expr* replacement) const override;
    bool equals(const Expr* other) const override;
};

class VarExpr : public Expr {
    std::string name;
public:
    VarExpr(const std::string& name) : name(name) {}

    int interp() const override;
    bool has_variable() const override;
    Expr* subst(const std::string& varName, const Expr* replacement) const override;
    bool equals(const Expr* other) const override;
};

#endif // EXPR_H