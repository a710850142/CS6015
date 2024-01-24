// 防止头文件重复包含的预处理指令
// Preprocessor directive to prevent the header file from being included multiple times
#ifndef EXPR_H
#define EXPR_H

#include <string>

// 基类 Expr，表示表达式
// Base class Expr, representing an expression
class Expr {
public:
    // 纯虚函数，用于比较两个表达式是否相等
    // Pure virtual function for comparing two expressions for equality
    virtual bool equals(const Expr* other) const = 0;

    // 虚析构函数，确保正确删除派生类对象
    // Virtual destructor to ensure derived class objects are deleted correctly
    virtual ~Expr() {}
};

// Num 类表示数字表达式
// Num class represents a numeric expression
class Num : public Expr {
    int value; // 存储数字表达式的值
public:
    // 构造函数，初始化数字表达式的值
    // Constructor to initialize the value of the numeric expression
    Num(int value) : value(value) {}

    // 重写 equals 函数，比较两个 Num 类型表达式是否相等
    // Override the equals function to compare two Num type expressions for equality
    bool equals(const Expr* other) const override;
};

// Add 类表示加法表达式
// Add class represents an addition expression
class Add : public Expr {
    Expr* lhs; // 左侧子表达式
    Expr* rhs; // 右侧子表达式
public:
    // 构造函数，初始化加法表达式的左右子表达式
    // Constructor to initialize the left and right subexpressions of the addition expression
    Add(Expr* lhs, Expr* rhs) : lhs(lhs), rhs(rhs) {}

    // 重写 equals 函数，比较两个 Add 类型表达式是否相等
    // Override the equals function to compare two Add type expressions for equality
    bool equals(const Expr* other) const override;
};

// Mult 类表示乘法表达式
// Mult class represents a multiplication expression
class Mult : public Expr {
    Expr* lhs; // 左侧子表达式
    Expr* rhs; // 右侧子表达式
public:
    // 构造函数，初始化乘法表达式的左右子表达式
    // Constructor to initialize the left and right subexpressions of the multiplication expression
    Mult(Expr* lhs, Expr* rhs) : lhs(lhs), rhs(rhs) {}

    // 重写 equals 函数，比较两个 Mult 类型表达式是否相等
    // Override the equals function to compare two Mult type expressions for equality
    bool equals(const Expr* other) const override;
};

// VarExpr 类表示变量表达式
// VarExpr class represents a variable expression
class VarExpr : public Expr {
    std::string name; // 存储变量名
public:
    // 构造函数，初始化变量表达式的名称
    // Constructor to initialize the name of the variable expression
    VarExpr(const std::string& name) : name(name) {}

    // 重写 equals 函数，比较两个 VarExpr 类型表达式是否相等
    // Override the equals function to compare two VarExpr type expressions for equality
    bool equals(const Expr* other) const override;
};

#endif // EXPR_H
