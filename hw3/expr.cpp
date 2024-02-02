#include "expr.h"

// Num
int Num::interp() const {
    return value;
}

bool Num::has_variable() const {
    return false;
}

Expr* Num::subst(const std::string& varName, const Expr* replacement) const {
    return new Num(value);
}

bool Num::equals(const Expr* other) const {
    const Num* num = dynamic_cast<const Num*>(other);
    return num && value == num->value;
}

// Add
Add::~Add() {
    delete lhs;
    delete rhs;
}

int Add::interp() const {
    return lhs->interp() + rhs->interp();
}

bool Add::has_variable() const {
    return lhs->has_variable() || rhs->has_variable();
}

Expr* Add::subst(const std::string& varName, const Expr* replacement) const {
    return new Add(lhs->subst(varName, replacement), rhs->subst(varName, replacement));
}

bool Add::equals(const Expr* other) const {
    const Add* add = dynamic_cast<const Add*>(other);
    return add && lhs->equals(add->lhs) && rhs->equals(add->rhs);
}

// Mult
Mult::~Mult() {
    delete lhs;
    delete rhs;
}

int Mult::interp() const {
    return lhs->interp() * rhs->interp();
}

bool Mult::has_variable() const {
    return lhs->has_variable() || rhs->has_variable();
}

Expr* Mult::subst(const std::string& varName, const Expr* replacement) const {
    return new Mult(lhs->subst(varName, replacement), rhs->subst(varName, replacement));
}

bool Mult::equals(const Expr* other) const {
    const Mult* mult = dynamic_cast<const Mult*>(other);
    return mult && lhs->equals(mult->lhs) && rhs->equals(mult->rhs);
}

// VarExpr
int VarExpr::interp() const {
    throw std::runtime_error("No value for variable");
}

bool VarExpr::has_variable() const {
    return true;
}

Expr* VarExpr::subst(const std::string& varName, const Expr* replacement) const {
    if (name == varName) {
        return replacement->subst(varName, replacement);
    } else {
        return new VarExpr(name);
    }
}

bool VarExpr::equals(const Expr* other) const {
    const VarExpr* var = dynamic_cast<const VarExpr*>(other);
    return var && name == var->name;
}