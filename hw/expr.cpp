#include <sstream>
#include "expr.h"

// Num 类的 interp 方法实现
int Num::interp() const {
    return value;  // 返回该数值
}

// Num 类的 has_variable 方法实现
bool Num::has_variable() const {
    return false;  // 数值表达式不含变量，返回 false
}

// Num 类的 subst 方法实现
Expr* Num::subst(const std::string& varName, const Expr* replacement) const {
    return new Num(value);  // 返回新的 Num 类型表达式，保持不变
}

// Num 类的 equals 方法实现
bool Num::equals(const Expr* other) const {
    const Num* num = dynamic_cast<const Num*>(other);
    return num && value == num->value;  // 检查是否与另一个 Num 类型表达式相等
}

// Num 类的 print 方法实现
void Num::print(std::ostream& os) const {
    os << std::to_string(value);  // 将数值转换为字符串输出
}

// Num 类的 pretty_print_at 方法实现
void Num::pretty_print_at(std::ostream& ot, precedence_t prec, int newLinePrevPos, bool addParan) const {
    this->print(ot);  // 直接打印数值
}

// Add 类的析构函数实现
Add::~Add() {
    delete lhs;  // 释放左子表达式内存
    delete rhs;  // 释放右子表达式内存
}

// Add 类的 interp 方法实现
int Add::interp() const {
    return lhs->interp() + rhs->interp();  // 返回左右子表达式求值之和
}

// Add 类的 has_variable 方法实现
bool Add::has_variable() const {
    return lhs->has_variable() || rhs->has_variable();  // 左右子表达式任一含有变量即返回 true
}

// Add 类的 subst 方法实现
Expr* Add::subst(const std::string& varName, const Expr* replacement) const {
    return new Add(lhs->subst(varName, replacement), rhs->subst(varName, replacement));  // 对左右子表达式进行替换并返回新的 Add 表达式
}

// Add 类的 equals 方法实现
bool Add::equals(const Expr* other) const {
    const Add* add = dynamic_cast<const Add*>(other);
    return add && lhs->equals(add->lhs) && rhs->equals(add->rhs);  // 检查左右子表达式是否相等
}

// Add 类的 print 方法实现
void Add::print(std::ostream& os) const {
    os << '(';
    lhs->print(os);
    os << "+";
    rhs->print(os);
    os << ')';
}

// Add 类的 pretty_print_at 方法实现
void Add::pretty_print_at(std::ostream& os, precedence_t prec, int ini_pos, bool addParan) const {
    bool needParens = prec >= prec_add;
    if (needParens) os << "(";
    lhs->pretty_print_at(os, prec, ini_pos, true);
    os << " + ";
    rhs->pretty_print_at(os, prec, ini_pos, addParan && !addParan);
    if (needParens) os << ")";
}

// Mult 类的析构函数实现
Mult::~Mult() {
    delete lhs;  // 释放左子表达式内存
    delete rhs;  // 释放右子表达式内存
}

// Mult 类的 interp 方法实现
int Mult::interp() const {
    return lhs->interp() * rhs->interp();  // 返回左右子表达式求值之积
}

// Mult 类的 has_variable 方法实现
bool Mult::has_variable() const {
    return lhs->has_variable() || rhs->has_variable();  // 左右子表达式任一含有变量即返回 true
}

// Mult 类的 subst 方法实现
Expr* Mult::subst(const std::string& varName, const Expr* replacement) const {
    return new Mult(lhs->subst(varName, replacement), rhs->subst(varName, replacement));  // 对左右子表达式进行替换并返回新的 Mult 表达式
}

// Mult 类的 equals 方法实现
bool Mult::equals(const Expr* other) const {
    const Mult* mult = dynamic_cast<const Mult*>(other);
    return mult && lhs->equals(mult->lhs) && rhs->equals(mult->rhs);  // 检查左右子表达式是否相等
}

// Mult 类的 print 方法实现
void Mult::print(std::ostream& os) const {
    os << '(';
    lhs->print(os);
    os << "*";
    rhs->print(os);
    os << ')';
}

// Mult 类的 pretty_print_at 方法实现
void Mult::pretty_print_at(std::ostream& os, precedence_t prec, int ini_pos, bool addParan) const {
    bool needParens = prec >= prec_mult;
    if (needParens) os << "(";
    lhs->pretty_print_at(os, prec_mult, ini_pos, true);
    os << " * ";
    rhs->pretty_print_at(os, prec_add, ini_pos, addParan && !needParens);  // 对右子表达式采用右结合性
    if (needParens) os << ")";
}

// VarExpr 类的 interp 方法实现
int VarExpr::interp() const {
    throw std::runtime_error("No value for variable");  // 报错：变量没有对应值
}

// VarExpr 类的 has_variable 方法实现
bool VarExpr::has_variable() const {
    return true;  // 变量表达式总是含有变量，返回 true
}

// VarExpr 类的 subst 方法实现
Expr* VarExpr::subst(const std::string& varName, const Expr* replacement) const {
    if (name == varName) {
        return replacement->subst(varName, replacement);  // 返回替换后的表达式
    } else {
        return new VarExpr(name);  // 否则保持不变
    }
}

// VarExpr 类的 equals 方法实现
bool VarExpr::equals(const Expr* other) const {
    const VarExpr* var = dynamic_cast<const VarExpr*>(other);
    return var && name == var->name;  // 检查变量名是否相等
}

// VarExpr 类的 print 方法实现
void VarExpr::print(std::ostream& os) const {
    os << this->name;  // 输出变量名
}

// VarExpr 类的 pretty_print_at 方法实现
void VarExpr::pretty_print_at(std::ostream& os, precedence_t prec, int ini_pos, bool outerParan) const {
    this->print(os);  // 直接打印变量名
}

// Expr 类的 to_string 方法实现
std::string Expr::to_string() {
    std::stringstream st("");
    this->print(st);  // 将表达式打印到 stringstream 中
    return st.str();  // 返回字符串形式的表达式
}

// Expr 类的 to_pretty_string 方法实现
std::string Expr::to_pretty_string() const {
    std::stringstream ss;
    pretty_print(ss);  // 将表达式美观打印到 stringstream 中
    return ss.str();  // 返回美观字符串形式的表达式
}

// LetExpr 类的构造函数实现
LetExpr::LetExpr(const std::string& varName, Expr* bindingExpr, Expr* bodyExpr)
        : varName(varName), bindingExpr(bindingExpr), bodyExpr(bodyExpr) {}

// LetExpr 类的析构函数实现
LetExpr::~LetExpr() {
    delete bindingExpr;  // 释放绑定表达式内存
    delete bodyExpr;     // 释放主体表达式内存
}

// LetExpr 类的 interp 方法实现
int LetExpr::interp() const {
    int bind_n = this->bindingExpr->interp();  // 计算绑定表达式的值
    Num* bind_n_Expr = new Num(bind_n);        // 创建对应的数值表达式

    return this->bodyExpr->subst(varName, bind_n_Expr)->interp();  // 对主体表达式进行变量替换并求值
}

// LetExpr 类的 has_variable 方法实现
bool LetExpr::has_variable() const {
    return bindingExpr->has_variable() || bodyExpr->has_variable();  // 绑定表达式或主体表达式任一含有变量即返回 true
}

// LetExpr 类的 subst 方法实现
Expr* LetExpr::subst(const std::string& vName, const Expr* replacement) const {
    return new LetExpr(varName, bindingExpr->subst(vName, replacement), bodyExpr->subst(vName, replacement));  // 对绑定表达式和主体表达式进行变量替换并返回新的 LetExpr 表达式
}

// LetExpr 类的 equals 方法实现
bool LetExpr::equals(const Expr* other) const {
    const LetExpr* otherLet = dynamic_cast<const LetExpr*>(other);
    if (otherLet == NULL) {
        return false;  // 类型不匹配，返回 false
    }
    return otherLet && varName == otherLet->varName &&
           bindingExpr->equals(otherLet->bindingExpr) &&
           bodyExpr->equals(otherLet->bodyExpr);  // 检查变量名及绑定表达式、主体表达式是否相等
}

// LetExpr 类的 print 方法实现
void LetExpr::print(std::ostream& os) const {
    os << "(_let " << varName << "=";  // 输出 let 关键字和变量名
    bindingExpr->print(os);            // 输出绑定表达式
    os << " _in ";                     // 输出 in 关键字
    bodyExpr->print(os);               // 输出主体表达式
    os << ")";                         // 输出右括号
}

// LetExpr 类的 pretty_print_at 方法实现
void LetExpr::pretty_print_at(std::ostream& os, precedence_t prec, int ini_pos, bool addParan) const {
    if (addParan) os << "(";  // 添加左括号
    int pos_updated = os.tellp();  // 记录当前位置
    int indentation = pos_updated - ini_pos;  // 计算缩进量
    os << "_let " << varName << " = ";  // 输出 let 关键字和变量名
    bindingExpr->pretty_print_at(os, prec_none, ini_pos, false);  // 输出绑定表达式
    os << "\n";  // 换行
    int curPos = os.tellp();  // 记录当前位置
    while (indentation > 0) {  // 根据缩进量添加空格
        os << " ";
        indentation -= 1;
    }
    os << "_in  ";  // 输出 in 关键字
    bodyExpr->pretty_print_at(os, prec_none, curPos, false);  // 输出主体表达式
    if (addParan) {
        os << ")";  // 添加右括号
    }
}
