#include "expr.h"

// Num 类实现
// Implementation of the Num class

// 解释此数字表达式
// Interprets this numeric expression
int Num::interp() const {
    return value;
}

// 检查表达式中是否包含变量
// Checks if the expression contains a variable
bool Num::has_variable() const {
    return false;
}

// 替换表达式中的变量
// Substitutes a variable in the expression
Expr* Num::subst(const std::string& varName, const Expr* replacement) const {
    return new Num(value);
}

// 检查两个表达式是否相等
// Checks if two expressions are equal
bool Num::equals(const Expr* other) const {
    const Num* num = dynamic_cast<const Num*>(other);
    return num && value == num->value;
}

// 打印此数字表达式
// Prints this numeric expression
void Num::print(std::ostream &os) const {
    os << value;
}

// 漂亮打印此数字表达式，与 print 相同
// Pretty prints this numeric expression, same as print
void Num::pretty_print(std::ostream &os) const {
    print(os); // For Num, pretty_print and print are the same
}

// Add 类实现
// Implementation of the Add class

// 析构函数，释放左右子表达式
// Destructor, frees left and right subexpressions
Add::~Add() {
    delete lhs;
    delete rhs;
}

// 解释此加法表达式
// Interprets this addition expression
int Add::interp() const {
    return lhs->interp() + rhs->interp();
}

// 检查表达式中是否包含变量
// Checks if the expression contains a variable
bool Add::has_variable() const {
    return lhs->has_variable() || rhs->has_variable();
}

// 替换表达式中的变量
// Substitutes a variable in the expression
Expr* Add::subst(const std::string& varName, const Expr* replacement) const {
    return new Add(lhs->subst(varName, replacement), rhs->subst(varName, replacement));
}

// 检查两个表达式是否相等
// Checks if two expressions are equal
bool Add::equals(const Expr* other) const {
    const Add* add = dynamic_cast<const Add*>(other);
    return add && lhs->equals(add->lhs) && rhs->equals(add->rhs);
}

// 打印此加法表达式
// Prints this addition expression
void Add::print(std::ostream &os) const {
    os << "(";
    lhs->print(os);
    os << " + ";
    rhs->print(os);
    os << ")";
}

// 漂亮打印此加法表达式
// Pretty prints this addition expression
void Add::pretty_print(std::ostream &os) const {
    pretty_print_at(os, prec_none);
}

// 根据优先级漂亮打印此加法表达式
// Pretty prints this addition expression based on precedence
void Add::pretty_print_at(std::ostream &os, precedence_t prec) const {
    bool needParens = prec > prec_add;
    if (needParens) {
        os << "(";
    }
    lhs->pretty_print_at(os, prec_add);
    os << " + ";
    rhs->pretty_print_at(os, prec_add);
    if (needParens) {
        os << ")";
    }
}

// Mult 类实现
// Implementation of the Mult class

// 析构函数，释放左右子表达式
// Destructor, frees left and right subexpressions
Mult::~Mult() {
    delete lhs;
    delete rhs;
}

// 解释此乘法表达式
// Interprets this multiplication expression
int Mult::interp() const {
    return lhs->interp() * rhs->interp();
}

// 检查表达式中是否包含变量
// Checks if the expression contains a variable
bool Mult::has_variable() const {
    return lhs->has_variable() || rhs->has_variable();
}

// 替换表达式中的变量
// Substitutes a variable in the expression
Expr* Mult::subst(const std::string& varName, const Expr* replacement) const {
    return new Mult(lhs->subst(varName, replacement), rhs->subst(varName, replacement));
}

// 检查两个表达式是否相等
// Checks if two expressions are equal
bool Mult::equals(const Expr* other) const {
    const Mult* mult = dynamic_cast<const Mult*>(other);
    return mult && lhs->equals(mult->lhs) && rhs->equals(mult->rhs);
}

// 打印此乘法表达式
// Prints this multiplication expression
void Mult::print(std::ostream &os) const {
    os << "(";
    lhs->print(os);
    os << " * ";
    rhs->print(os);
    os << ")";
}

// 漂亮打印此乘法表达式
// Pretty prints this multiplication expression
void Mult::pretty_print(std::ostream &os) const {
    pretty_print_at(os, prec_none);
}

// 根据优先级漂亮打印此乘法表达式
// Pretty prints this multiplication expression based on precedence
void Mult::pretty_print_at(std::ostream &os, precedence_t prec) const {
    bool needParens = prec >= prec_mult;
    if (needParens) os << "(";

    lhs->pretty_print_at(os, prec_mult);
    os << " * ";
    rhs->pretty_print_at(os, prec_none);

    if (needParens) os << ")";
}

// VarExpr 类实现
// Implementation of the VarExpr class

// 解释此变量表达式，抛出异常
// Interprets this variable expression, throws an exception
int VarExpr::interp() const {
    throw std::runtime_error("No value for variable");
}

// 检查表达式中是否包含变量
// Checks if the expression contains a variable
bool VarExpr::has_variable() const {
    return true;
}

// 替换表达式中的变量
// Substitutes a variable in the expression
Expr* VarExpr::subst(const std::string& varName, const Expr* replacement) const {
    if (name == varName) {
        return replacement->subst(varName, replacement);
    } else {
        return new VarExpr(name);
    }
}

// 检查两个表达式是否相等
// Checks if two expressions are equal
bool VarExpr::equals(const Expr* other) const {
    const VarExpr* var = dynamic_cast<const VarExpr*>(other);
    return var && name == var->name;
}

// 打印此变量表达式
// Prints this variable expression
void VarExpr::print(std::ostream &os) const {
    os << name;
}

// 将表达式转换为字符串
// Converts the expression to a string
std::string Expr::to_string() const {
    std::stringstream ss;
    print(ss);
    return ss.str();
}
