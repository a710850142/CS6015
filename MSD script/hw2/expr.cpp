#include "expr.h"

// Num 类的实现

// 计算并返回数字的值。
int Num::interp() const {
    return value;
}

// 检查数字中是否包含变量，这里始终返回 false。
bool Num::has_variable() const {
    return false;
}

// 替换操作，由于数字中不含变量，因此返回原数值。
Expr* Num::subst(const std::string& varName, const Expr* replacement) const {
    return new Num(value);
}

// 比较两个数字是否相等。
bool Num::equals(const Expr* other) const {
    const Num* num = dynamic_cast<const Num*>(other);
    return num && value == num->value;
}

// Add 类的实现

// 析构函数，释放左右操作数占用的内存。
Add::~Add() {
    delete lhs;
    delete rhs;
}

// 计算加法表达式的值。
int Add::interp() const {
    return lhs->interp() + rhs->interp();
}

// 检查加法表达式中是否包含变量。
bool Add::has_variable() const {
    return lhs->has_variable() || rhs->has_variable();
}

// 替换加法表达式中的变量。
Expr* Add::subst(const std::string& varName, const Expr* replacement) const {
    return new Add(lhs->subst(varName, replacement), rhs->subst(varName, replacement));
}

// 比较两个加法表达式是否相等。
bool Add::equals(const Expr* other) const {
    const Add* add = dynamic_cast<const Add*>(other);
    return add && lhs->equals(add->lhs) && rhs->equals(add->rhs);
}

// Mult 类的实现

// 析构函数，释放左右操作数占用的内存。
Mult::~Mult() {
    delete lhs;
    delete rhs;
}

// 计算乘法表达式的值。
int Mult::interp() const {
    return lhs->interp() * rhs->interp();
}

// 检查乘法表达式中是否包含变量。
bool Mult::has_variable() const {
    return lhs->has_variable() || rhs->has_variable();
}

// 替换乘法表达式中的变量。
Expr* Mult::subst(const std::string& varName, const Expr* replacement) const {
    return new Mult(lhs->subst(varName, replacement), rhs->subst(varName, replacement));
}

// 比较两个乘法表达式是否相等。
bool Mult::equals(const Expr* other) const {
    const Mult* mult = dynamic_cast<const Mult*>(other);
    return mult && lhs->equals(mult->lhs) && rhs->equals(mult->rhs);
}

// VarExpr 类的实现

// 计算变量表达式的值，由于变量无具体值，因此抛出异常。
int VarExpr::interp() const {
    throw std::runtime_error("No value for variable");
}

// 检查表达式中是否包含变量，这里始终返回 true。
bool VarExpr::has_variable() const {
    return true;
}

// 替换变量表达式中的变量。
Expr* VarExpr::subst(const std::string& varName, const Expr* replacement) const {
    if (name == varName) {
        return replacement->subst(varName, replacement);
    } else {
        return new VarExpr(name);
    }
}

// 比较两个变量表达式是否相等。
bool VarExpr::equals(const Expr* other) const {
    const VarExpr* var = dynamic_cast<const VarExpr*>(other);
    return var && name == var->name;
}
