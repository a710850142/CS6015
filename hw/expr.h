#ifndef EXPR_H
#define EXPR_H

#include <string>
#include <sstream>
#include <iostream>
#include <stdexcept>

// 声明优先级类型
// Declare precedence type
typedef enum {
    prec_none,      // = 0, no precedence
    prec_add,       // = 1, precedence of addition
    prec_mult       // = 2, precedence of multiplication
} precedence_t;

class Expr {
public:
    virtual ~Expr() {}

    // 解释表达式的值
    // Interpret the value of the expression
    virtual int interp() const = 0;
    // 检查表达式中是否包含变量
    // Check if the expression contains a variable
    virtual bool has_variable() const = 0;
    // 替换表达式中的变量
    // Substitute a variable in the expression
    virtual Expr* subst(const std::string& varName, const Expr* replacement) const = 0;
    // 检查两个表达式是否相等
    // Check if two expressions are equal
    virtual bool equals(const Expr* other) const = 0;

    // 打印表达式
    // Print the expression
    virtual void print(std::ostream &os) const = 0;
    // 将表达式转换为字符串
    // Convert the expression to a string
    std::string to_string() const;

    // 默认的漂亮打印实现，可在子类中重写
    // Default pretty print implementation, can be overridden in subclasses
    virtual void pretty_print(std::ostream &os) const { print(os); }
    // 为 pretty_print 提供一个辅助方法
    // Provide an auxiliary method for pretty_print
    virtual void pretty_print_at(std::ostream &os, precedence_t prec) const { pretty_print(os); }
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
    void pretty_print(std::ostream &os) const override;
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

    void print(std::ostream &os) const override;
    void pretty_print(std::ostream &os) const override;
    void pretty_print_at(std::ostream &os, precedence_t prec) const override;
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

    void print(std::ostream &os) const override;
    void pretty_print(std::ostream &os) const override;
    void pretty_print_at(std::ostream &os, precedence_t prec) const override;
};

class VarExpr : public Expr {
    std::string name;
public:
    VarExpr(const std::string& name) : name(name) {}

    int interp() const override;
    bool has_variable() const override;
    Expr* subst(const std::string& varName, const Expr* replacement) const override;
    bool equals(const Expr* other) const override;

    // VarExpr 不需要实现 pretty_print 方法
    // VarExpr does not need to implement the pretty_print method
    void print(std::ostream &os) const override;
};

#endif // EXPR_H
