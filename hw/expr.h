#ifndef EXPR_H
#define EXPR_H

#include <string>
#include <stdexcept>
#include <sstream>

typedef enum {
    prec_none,    // 无优先级
    prec_add,     // 加法优先级
    prec_mult,    // 乘法优先级
} precedence_t;  // 优先级枚举类型

class Expr {
public:
    virtual ~Expr() {}

    virtual int interp() const = 0;  // 解释执行
    virtual bool has_variable() const = 0;  // 是否含有变量
    virtual Expr* subst(const std::string& varName, const Expr* replacement) const = 0;  // 替换变量
    std::string to_string();  // 转换为字符串
    virtual bool equals(const Expr* other) const = 0;  // 比较是否相等
    virtual void print(std::ostream& os) const = 0;  // 打印表达式
    void pretty_print(std::ostream& os) const {
        pretty_print_at(os, prec_none, 0, false);
    }
    virtual void pretty_print_at(std::ostream &os, precedence_t prec, int ini_pos, bool addParan) const { pretty_print(os); }

    std::string to_pretty_string() const;  // 转换为美观字符串
};

class Num : public Expr {
    int value;
public:
    Num(int value) : value(value) {}
    int interp() const override;  // 解释执行
    bool has_variable() const override;  // 是否含有变量
    Expr* subst(const std::string& varName, const Expr* replacement) const override;  // 替换变量
    bool equals(const Expr* other) const override;  // 比较是否相等
    void pretty_print_at(std::ostream &os, precedence_t prec, int ini_pos, bool outerParan) const override;  // 美观打印
    void print(std::ostream& os) const override;  // 打印表达式
};

class Add : public Expr {
    Expr* lhs;
    Expr* rhs;
public:
    Add(Expr* lhs, Expr* rhs) : lhs(lhs), rhs(rhs) {}
    ~Add();

    int interp() const override;  // 解释执行
    bool has_variable() const override;  // 是否含有变量
    Expr* subst(const std::string& varName, const Expr* replacement) const override;  // 替换变量
    bool equals(const Expr* other) const override;  // 比较是否相等
    void print(std::ostream& os) const override;  // 打印表达式
    void pretty_print_at(std::ostream &os, precedence_t prec, int ini_pos, bool outerParan) const override;  // 美观打印
};

class Mult : public Expr {
    Expr* lhs;
    Expr* rhs;
public:
    Mult(Expr* lhs, Expr* rhs) : lhs(lhs), rhs(rhs) {}
    ~Mult();

    int interp() const override;  // 解释执行
    bool has_variable() const override;  // 是否含有变量
    Expr* subst(const std::string& varName, const Expr* replacement) const override;  // 替换变量
    bool equals(const Expr* other) const override;  // 比较是否相等
    void print(std::ostream& os) const override;  // 打印表达式
    void pretty_print_at(std::ostream &os, precedence_t prec, int ini_pos, bool outerParan) const override;  // 美观打印
};

class VarExpr : public Expr {
    std::string name;
public:
    VarExpr(const std::string& name) : name(name) {}

    int interp() const override;  // 解释执行
    bool has_variable() const override;  // 是否含有变量
    Expr* subst(const std::string& varName, const Expr* replacement) const override;  // 替换变量
    bool equals(const Expr* other) const override;  // 比较是否相等
    void print(std::ostream& os) const override;  // 打印表达式
    void pretty_print_at(std::ostream &os, precedence_t prec, int ini_pos, bool outerParan) const override;  // 美观打印
};

class LetExpr : public Expr {
    std::string varName;
    Expr* bindingExpr;
    Expr* bodyExpr;

public:
    LetExpr(const std::string& varName, Expr* bindingExpr, Expr* bodyExpr);
    ~LetExpr();
    int interp() const override;  // 解释执行
    bool has_variable() const override;  // 是否含有变量
    Expr* subst(const std::string& vName, const Expr* replacement) const override;  // 替换变量
    bool equals(const Expr* other) const override;  // 比较是否相等
    void print(std::ostream& os) const override;  // 打印表达式
    void pretty_print_at(std::ostream &os, precedence_t prec, int ini_pos, bool outerParan) const override;  // 美观打印
};

#endif // EXPR_H
