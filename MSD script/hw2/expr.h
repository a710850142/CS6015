// expr.h 文件

// 防止重复包含的预处理指令
#ifndef EXPR_H
#define EXPR_H

#include <string>
#include <stdexcept>

// Expr 基类，用于所有表达式的通用接口。
class Expr {
public:
    // 虚析构函数，确保正确的资源回收。
    virtual ~Expr() {}

    // interp 方法用于计算并返回表达式的整数值。
    virtual int interp() const = 0;

    // has_variable 方法用于检查表达式中是否包含变量。
    virtual bool has_variable() const = 0;

    // subst 方法用于替换表达式中的变量。
    virtual Expr* subst(const std::string& varName, const Expr* replacement) const = 0;

    // equals 方法用于比较两个表达式是否相等。
    virtual bool equals(const Expr* other) const = 0;
};

// Num 类表示一个数字。
class Num : public Expr {
    int value; // 表达式中的数值。
public:
    // 构造函数，初始化数字的值。
    Num(int value) : value(value) {}

    // 重写基类的虚函数。
    int interp() const override; // 计算并返回数值。
    bool has_variable() const override; // 检查表达式中是否有变量，这里总是返回 false。
    Expr* subst(const std::string& varName, const Expr* replacement) const override; // 替换操作，对数字无效。
    bool equals(const Expr* other) const override; // 比较两个数字是否相等。
};

// Add 类表示加法表达式。
class Add : public Expr {
    Expr* lhs; // 左侧表达式
    Expr* rhs; // 右侧表达式
public:
    // 构造函数，接受两个表达式作为加法的左右操作数。
    Add(Expr* lhs, Expr* rhs) : lhs(lhs), rhs(rhs) {}
    ~Add(); // 析构函数，负责删除左右表达式。

    // 重写基类的虚函数。
    int interp() const override; // 计算加法表达式的值。
    bool has_variable() const override; // 检查表达式中是否有变量。
    Expr* subst(const std::string& varName, const Expr* replacement) const override; // 替换操作。
    bool equals(const Expr* other) const override; // 比较两个加法表达式是否相等。
};

// Mult 类表示乘法表达式。
class Mult : public Expr {
    Expr* lhs; // 左侧表达式
    Expr* rhs; // 右侧表达式
public:
    // 构造函数，接受两个表达式作为乘法的左右操作数。
    Mult(Expr* lhs, Expr* rhs) : lhs(lhs), rhs(rhs) {}
    ~Mult(); // 析构函数，负责删除左右表达式。

    // 重写基类的虚函数。
    int interp() const override; // 计算乘法表达式的值。
    bool has_variable() const override; // 检查表达式中是否有变量。
    Expr* subst(const std::string& varName, const Expr* replacement) const override; // 替换操作。
    bool equals(const Expr* other) const override; // 比较两个乘法表达式是否相等。
};

// VarExpr 类表示一个变量表达式。
class VarExpr : public Expr {
    std::string name; // 变量的名称
public:
    // 构造函数，初始化变量名称。
    VarExpr(const std::string& name) : name(name) {}

    // 重写基类的虚函数。
    int interp() const override; // 抛出异常，变量无法直接计算值。
    bool has_variable() const override; // 总是返回 true，因为它是一个变量。
    Expr* subst(const std::string& varName, const Expr* replacement) const override; // 替换操作，如果变量名匹配则替换。
    bool equals(const Expr* other) const override; // 比较两个变量表达式是否相等。
};

#endif // EXPR_H
