#include "expr.h"

// Num 类成员函数实现
int Num::interp() const {
    return value;
}

bool Num::has_variable() const {
    return false;
}

Expr* Num::subst(const std::string& varName, const Expr* replacement) const {
    return new Num(value); // 数字表达式不包含变量，直接返回自身的副本
}

bool Num::equals(const Expr* other) const {
    const Num* asNum = dynamic_cast<const Num*>(other);
    return asNum && this->value == asNum->value;
}

void Num::print(std::ostream &os) const {
    os << value;
}

std::string Num::to_string() const {
    std::ostringstream oss;
    oss << value; // 假设 value 是 Num 类中存储数字的成员变量
    return oss.str();
}

// Add 类成员函数实现
Add::Add(Expr* lhs, Expr* rhs) : lhs(lhs), rhs(rhs) {}

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
    const Add* asAdd = dynamic_cast<const Add*>(other);
    return asAdd && lhs->equals(asAdd->lhs) && rhs->equals(asAdd->rhs);
}

void Add::print(std::ostream &os) const {
    os << "(";
    lhs->print(os);
    os << " + ";
    rhs->print(os);
    os << ")";
}

std::string Add::to_string() const {
    std::ostringstream oss;
    print(oss);
    return oss.str();
}

void Add::pretty_print_at(std::ostream &os, precedence_t prec, std::streampos& last_newline) const {
    // 检查当前表达式是否需要在外部添加括号
    bool needOuterParens = prec > prec_add; // 如果当前上下文优先级高于加法，需要括号

    if (needOuterParens) os << "(";

    lhs->pretty_print_at(os, prec_add, last_newline);
    os << " + ";
    rhs->pretty_print_at(os, prec_add, last_newline);

    if (needOuterParens) os << ")";
}




// Mult 类成员函数实现
Mult::Mult(Expr* lhs, Expr* rhs) : lhs(lhs), rhs(rhs) {}

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
    const Mult* asMult = dynamic_cast<const Mult*>(other);
    return asMult && lhs->equals(asMult->lhs) && rhs->equals(asMult->rhs);
}

void Mult::print(std::ostream &os) const {
    os << "(";
    lhs->print(os);
    os << " * ";
    rhs->print(os);
    os << ")";
}

std::string Mult::to_string() const {
    std::ostringstream oss;
    print(oss);
    return oss.str();
}

void Mult::pretty_print_at(std::ostream &os, precedence_t prec, std::streampos& last_newline) const {
    // 判断左侧表达式是否需要括号
    bool leftNeedsParens = dynamic_cast<const Add*>(lhs) != nullptr;
    if (leftNeedsParens) os << "(";
    lhs->pretty_print_at(os, prec_mult, last_newline); // 使用乘法的优先级进行打印
    if (leftNeedsParens) os << ")";

    os << " * ";

    // 判断右侧表达式是否需要括号
    bool rightNeedsParens = dynamic_cast<const Add*>(rhs) != nullptr || dynamic_cast<const Mult*>(rhs) != nullptr;
    if (rightNeedsParens) os << "(";
    rhs->pretty_print_at(os, prec_none, last_newline); // 修改为prec_none以确保括号在需要时被添加
    if (rightNeedsParens) os << ")";
}











// VarExpr 类成员函数实现
VarExpr::VarExpr(const std::string& name) : name(name) {}

int VarExpr::interp() const {
    throw std::runtime_error("Variable has no value");
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
    const VarExpr* asVar = dynamic_cast<const VarExpr*>(other);
    return asVar && this->name == asVar->name;
}

void VarExpr::print(std::ostream &os) const {
    os << name;
}

std::string VarExpr::to_string() const {
    std::ostringstream oss;
    print(oss);
    return oss.str();
}

// LetExpr 类成员函数实现
LetExpr::LetExpr(const std::string& var, Expr* rhs, Expr* body) : var(var), rhs(rhs), body(body) {}

LetExpr::~LetExpr() {
    delete rhs;
    delete body;
}

int LetExpr::interp() const {
    throw std::runtime_error("LetExpr interp not implemented");
}

bool LetExpr::has_variable() const {
    return rhs->has_variable() || body->has_variable();
}

Expr* LetExpr::subst(const std::string& varName, const Expr* replacement) const {

    Expr* newRhs = rhs->subst(varName, replacement);
    Expr* newBody = body;
    if (var != varName) {
        newBody = body->subst(varName, replacement);
    } else {

    }

    return new LetExpr(var, newRhs, newBody);
}






bool LetExpr::equals(const Expr* other) const {
    const LetExpr* otherLet = dynamic_cast<const LetExpr*>(other);
    if (!otherLet) return false;
    return var == otherLet->var && rhs->equals(otherLet->rhs) && body->equals(otherLet->body);
}



void LetExpr::print(std::ostream &os) const {
    os << "(_let " << var << "=";
    rhs->print(os);
    os << " _in ";
    body->print(os);
    os << ")";
}


std::string LetExpr::to_string() const {
    std::ostringstream oss;
    print(oss);
    return oss.str();
}

std::string generateIndent(int level) {
    return std::string(level, ' ');
}

// 假设我们有一个全局变量或者通过其他方式传递缩进级别
int indentLevel = 0;

void LetExpr::pretty_print_at(std::ostream &os, precedence_t prec, std::streampos& last_newline) const {
    os << "_let " << var << " = ";
    rhs->pretty_print_at(os, prec_let, last_newline);
    os << "\n" << generateIndent(indentLevel) << "_in  ";
    // 增加缩进级别以反映嵌套
    indentLevel += 4;
    body->pretty_print_at(os, prec_let, last_newline);
    // 恢复之前的缩进级别
    indentLevel -= 4;
}


