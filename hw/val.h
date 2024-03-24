#ifndef MSDSCRIPT_VAL_H
#define MSDSCRIPT_VAL_H

#include <string>

class Expr;

class Val {
public:
    virtual Expr* to_expr() = 0;
    virtual bool equals(Val* rhs) = 0;
    virtual Val* add_to(Val* rhs) = 0;
    virtual Val* mult_with(Val* rhs) = 0;
    virtual std::string to_string() = 0;
    virtual bool is_true() = 0;
    virtual Val* call(Val *actual_arg) = 0;
};

class NumVal : public Val {
private:
    int val;
public:
    NumVal(int v);
    Expr * to_expr() override;
    bool equals(Val* rhs) override;
    Val* add_to(Val* rhs) override;
    Val* mult_with(Val* rhs) override;
    std::string to_string() override;
    bool is_true() override;
    virtual Val* call(Val *actual_arg) override;
};

class BoolVal : public Val {
private:
    bool val;
public:
    BoolVal(bool v);
    Expr* to_expr() override;
    bool equals(Val* rhs) override;
    Val* add_to(Val* rhs) override;
    Val* mult_with(Val* rhs) override;
    std::string to_string() override;
    bool is_true() override;
    virtual Val* call(Val *actual_arg) override;
};

// FunVal类继承自Val类，用于表示函数值
class FunVal : public Val {
private:
    std::string formal_arg; // 函数的形式参数名称
    Expr* body; // 函数体，是一个表达式

public:
    // 构造函数，接受一个字符串和一个表达式指针作为参数
    FunVal(std::string arg, Expr* expr);

    // 将函数值转换为对应的表达式，重写了Val类的虚函数
    Expr* to_expr() override;

    // 检查当前函数值是否与另一个Val值相等，重写了Val类的虚函数
    bool equals(Val* rhs) override;

    // 尝试将当前函数值与另一个Val值进行加法操作，不支持此操作，重写了Val类的虚函数
    Val* add_to(Val* rhs) override;

    // 尝试将当前函数值与另一个Val值进行乘法操作，不支持此操作，重写了Val类的虚函数
    Val* mult_with(Val* rhs) override;

    // 将当前函数值转换为字符串形式，重写了Val类的虚函数
    std::string to_string() override;

    // 检查当前函数值在布尔上下文中的真值性，函数值本身不能直接用作布尔值，重写了Val类的虚函数
    bool is_true() override;

    // 调用当前函数值，接受一个实际参数并执行函数体，重写了Val类的虚函数
    virtual Val* call(Val *actual_arg) override;
};


#endif //MSDSCRIPT_VAL_H