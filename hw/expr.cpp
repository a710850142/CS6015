#include <sstream>
#include "val.h"
#include "expr.h"

// expr
std::string Expr::to_string() {
    std::stringstream st("");
    print(st);  // 利用多态性调用对应子类的print方法
    return st.str();
}

std::string Expr::to_pretty_string() {
    std::stringstream st("");
    pretty_print(st);  // 利用多态性调用对应子类的pretty_print方法以获取格式化字符串
    return st.str();
}

void Expr::pretty_print(std::ostream &out) {
    std::streampos pos = out.tellp();
    pretty_print_at(out, prec_none, pos, false); // 调用pretty_print_at实现具体的格式化打印
}

// 数字表达式类
NumExpr::NumExpr(int v) {
    val = v; // 初始化时存储整数值
}

bool NumExpr::equals(Expr* expr) {
    NumExpr *n = dynamic_cast<NumExpr*>(expr); // 动态类型转换，检查是否为NumExpr类型
    if (n == nullptr) {
        return false; // 类型不匹配，返回false
    }
    return val == n->val; // 比较数值是否相等
}

Val* NumExpr::interp() {
    return new NumVal(val); // 解释执行，返回对应的值对象
}

bool NumExpr::has_variable() {
    return false; // 数字表达式不包含变量
}

Expr* NumExpr::subst(std::string s, Expr *expr) {
    return this; // 数字表达式不做替换，直接返回自身
}

void NumExpr::print(std::ostream& out) {
    out << val; // 打印存储的整数值
}

void NumExpr::pretty_print_at(std::ostream& out, precedence_t precedence, std::streampos& newLinePrevPos, bool addParenthesesToLet) {
    out << val; // 美观打印也是直接输出整数值
}

// addexpr
AddExpr::AddExpr(Expr *left, Expr *right) {
    lhs = left;
    rhs = right;
}

AddExpr::AddExpr(int left, int right) {
    lhs = new NumExpr(left);
    rhs = new NumExpr(right);
}

AddExpr::AddExpr(std::string left, int right) {
    lhs = new VarExpr(left);
    rhs = new NumExpr(right);
}

AddExpr::AddExpr(int left, std::string right) {
    lhs = new NumExpr(left);
    rhs = new VarExpr(right);
}

AddExpr::AddExpr(std::string left, std::string right) {
    lhs = new VarExpr(left);
    rhs = new VarExpr(right);
}

AddExpr::AddExpr(int left, Expr* right) {
    lhs = new NumExpr(left);
    rhs = right;
}

AddExpr::AddExpr(Expr* left, int right) {
    lhs = left;
    rhs = new NumExpr(right);
}

bool AddExpr::equals(Expr* expr) {
    AddExpr *a = dynamic_cast<AddExpr*>(expr);
    if (a == NULL) {
        return false;
    }
    return lhs->equals(a->lhs) && rhs->equals(a->rhs);
}

Val* AddExpr::interp() {
    return lhs->interp()->add_to(rhs->interp());
}

bool AddExpr::has_variable() {
    return lhs->has_variable() || rhs->has_variable();
}

Expr* AddExpr::subst(std::string s, Expr *expr) {
    return new AddExpr(lhs->subst(s, expr), rhs->subst(s, expr));
}

void AddExpr::print(std::ostream& out) {
    out << "(";
    lhs->print(out);
    out << "+";
    rhs->print(out);
    out << ")";
}

void AddExpr::pretty_print_at(std::ostream &out, precedence_t precedence, std::streampos& newLinePrevPos, bool addParenthesesToLet) {
    bool addParentheses = prec_add <= precedence;
    if (addParentheses) {
        out << "(";
    }
    lhs->pretty_print_at(out, prec_add, newLinePrevPos, true);
    out << " + ";
    // let as right arg in AddExpr never need parentheses
    rhs->pretty_print_at(out, prec_equal, newLinePrevPos, false);
    if (addParentheses) {
        out << ")";
    }
}
// 乘法表达式类，表示两个表达式的乘积
MultExpr::MultExpr(Expr *left, Expr *right) {
    lhs = left;  // 左子表达式
    rhs = right; // 右子表达式
}

// 构造函数重载，允许直接使用整数创建乘法表达式
MultExpr::MultExpr(int left, int right) {
    lhs = new NumExpr(left); // 左侧创建数字表达式
    rhs = new NumExpr(right); // 右侧创建数字表达式
}

// 构造函数重载，左侧为变量名，右侧为整数
MultExpr::MultExpr(std::string left, int right) {
    lhs = new VarExpr(left); // 左侧创建变量表达式
    rhs = new NumExpr(right); // 右侧创建数字表达式
}

// 构造函数重载，左侧为整数，右侧为变量名
MultExpr::MultExpr(int left, std::string right) {
    lhs = new NumExpr(left); // 左侧创建数字表达式
    rhs = new VarExpr(right); // 右侧创建变量表达式
}

// 构造函数重载，两侧均为变量名
MultExpr::MultExpr(std::string left, std::string right) {
    lhs = new VarExpr(left); // 左侧创建变量表达式
    rhs = new VarExpr(right); // 右侧创建变量表达式
}

// 构造函数重载，左侧为整数，右侧为表达式
MultExpr::MultExpr(int left, Expr* right) {
    lhs = new NumExpr(left); // 左侧创建数字表达式
    rhs = right; // 右侧直接使用传入的表达式
}

// 构造函数重载，左侧为表达式，右侧为整数
MultExpr::MultExpr(Expr* left, int right) {
    lhs = left; // 左侧直接使用传入的表达式
    rhs = new NumExpr(right); // 右侧创建数字表达式
}

// 检查两个乘法表达式是否相等
bool MultExpr::equals(Expr* expr) {
    MultExpr *m = dynamic_cast<MultExpr*>(expr);
    if (m == NULL) {
        return false;
    }
    return lhs->equals(m->lhs) && rhs->equals(m->rhs);
}

// 解释执行乘法表达式，返回乘积的值
Val* MultExpr::interp() {
    return lhs->interp()->mult_with(rhs->interp());
}

// 检查乘法表达式中是否含有变量
bool MultExpr::has_variable() {
    return lhs->has_variable() || rhs->has_variable();
}

// 替换乘法表达式中的变量
Expr* MultExpr::subst(std::string s, Expr *expr) {
    return new MultExpr(lhs->subst(s, expr), rhs->subst(s, expr));
}

// 将乘法表达式转换为字符串形式
void MultExpr::print(std::ostream& out) {
    out << "(";
    lhs -> print(out);
    out << "*";
    rhs -> print(out);
    out << ")";
}

// 根据优先级在乘法表达式周围添加括号，进行美观打印
void MultExpr::pretty_print_at(std::ostream &out, precedence_t precedence, std::streampos& newLinePrevPos, bool addParenthesesToLet) {
    bool addParentheses = prec_mult <= precedence;
    if (addParentheses) {
        out << "(";
    }
    lhs->pretty_print_at(out, prec_mult, newLinePrevPos, true);
    out << " * ";
    rhs->pretty_print_at(out, prec_add, newLinePrevPos, addParenthesesToLet && !addParentheses);
    if (addParentheses) {
        out << ")";
    }
}

// 变量表达式类，表示一个变量
VarExpr::VarExpr(std::string s) {
    val = s; // 存储变量名
}

// 检查两个变量表达式是否相等
bool VarExpr::equals(Expr* expr) {
    VarExpr *var = dynamic_cast<VarExpr*>(expr);
    if (var == NULL) {
        return false;
    }
    return val == var->val;
}

// 尝试解释执行变量表达式时抛出异常，因为变量无法直接求值
Val * VarExpr::interp() {
    throw std::runtime_error("A variable has no value!");
}

// 变量表达式总是含有变量
bool VarExpr::has_variable() {
    return true;
}

// 替换变量表达式中的变量
Expr* VarExpr::subst(std::string s, Expr* expr) {
    if (val == s) {
        return expr;
    }
    return this;
}

// 将变量表达式转换为字符串形式
void VarExpr::print(std::ostream& out) {
    out << val;
}

// 美观打印变量表达式，直接输出变量名
void VarExpr::pretty_print_at(std::ostream& out, precedence_t precedence, std::streampos& newLinePrevPos, bool addParenthesesToLet) {
    out << val;
}

// 获取变量名
std::string VarExpr::getVal() {
    return val;
}

// Let表达式类，表示let绑定表达式
LetExpr::LetExpr(std::string v, Expr* r, Expr* b) {
    variable = v;  // 被绑定的变量名
    rhs = r;       // 右侧表达式，其值将绑定到变量
    body = b;      // let表达式绑定后执行的主体表达式
}

// 检查两个Let表达式是否相等
bool LetExpr::equals(Expr* expr) {
    LetExpr* other = dynamic_cast<LetExpr*>(expr);
    if (other == nullptr) {
        return false;
    }
    return variable == other->variable && rhs->equals(other->rhs) && body->equals(other->body);
}

// 解释执行Let表达式
Val * LetExpr::interp() {
    Val* rhs_val = rhs->interp();  // 计算右侧表达式的值
    // 替换主体表达式中的变量为右侧表达式的值，然后解释执行主体表达式
    return body->subst(variable, rhs_val->to_expr())->interp();
}

// 检查Let表达式是否包含变量
bool LetExpr::has_variable() {
    return rhs->has_variable() || body->has_variable();
}

// 在Let表达式中替换变量
Expr* LetExpr::subst(std::string s, Expr* expr) {
    Expr* temp = rhs->subst(s, expr);  // 替换右侧表达式中的变量
    // 如果当前Let绑定的变量名与要替换的变量名相同，则不替换主体表达式中的该变量
    if (variable == s) {
        return new LetExpr(variable, temp, body);
    }
    // 否则，替换主体表达式中的变量
    return new LetExpr(variable, temp, body->subst(s, expr));
}

// 打印Let表达式
void LetExpr::print(std::ostream& out) {
    out << "(_let " << variable << "=";
    rhs->print(out);
    out << " _in ";
    body->print(out);
    out << ")";
}

// 根据上下文在Let表达式外添加括号进行美观打印
void LetExpr::pretty_print_at(std::ostream& out, precedence_t precedence, std::streampos& newLinePrevPos, bool addParenthesesToLet) {
    if (addParenthesesToLet) {
        out << "(";
    }
    int indentation = out.tellp() - newLinePrevPos;
    out << "_let " << variable << " = ";
    rhs->pretty_print_at(out, prec_none,newLinePrevPos, false);
    out << "\n";
    newLinePrevPos = out.tellp();

    out << std::string(indentation, ' ');
    out << "_in  ";
    body->pretty_print_at(out, prec_none, newLinePrevPos, false);

    if (addParenthesesToLet) {
        out << ")";
    }
}

// Bool表达式类，表示布尔值
BoolExpr::BoolExpr(bool v) {
    val = v;  // 存储布尔值
}

// 检查两个Bool表达式是否相等
bool BoolExpr::equals(Expr* rhs) {
    BoolExpr* other = dynamic_cast<BoolExpr*>(rhs);
    if (other == nullptr) {
        return false;
    }
    return val == other->val;
}

// 解释执行Bool表达式，返回布尔值
Val* BoolExpr::interp() {
    return new BoolVal(val);
}

// Bool表达式不包含变量
bool BoolExpr::has_variable() {
    return false;
}

// 在Bool表达式中替换变量，实际上不进行任何操作，直接返回自身
Expr* BoolExpr::subst(std::string s, Expr* expr) {
    return this;
}

// 打印Bool表达式
void BoolExpr::print(std::ostream& out) {
    val ? out << "_true" : out << "_false";
}

// 美观打印Bool表达式
void BoolExpr::pretty_print_at(std::ostream& out, precedence_t precedence, std::streampos& newLinePrevPos, bool addParenthesesToLet) {
    val ? out << "_true" : out << "_false";
}

// If表达式类，表示条件表达式
IfExpr::IfExpr(Expr* test, Expr* then, Expr* else_) {
    test_part = test;  // 条件部分
    then_part = then;  // 条件为真时执行的表达式
    else_part = else_; // 条件为假时执行的表达式
}

// 检查两个If表达式是否相等
bool IfExpr::equals(Expr* rhs) {
    IfExpr* other = dynamic_cast<IfExpr*>(rhs);
    if (other == nullptr) {
        return false;
    }
    return test_part->equals(other->test_part) && then_part->equals(other->then_part) && else_part->equals(other->else_part);
}

// 解释执行If表达式
Val* IfExpr::interp() {
    // 根据条件部分的值决定执行then部分还是else部分
    if (test_part->interp()->is_true()) {
        return then_part->interp();
    }
    return else_part->interp();
}

// 检查If表达式是否包含变量
bool IfExpr::has_variable() {
    return test_part->has_variable() || then_part->has_variable() || else_part->has_variable();
}

// 在If表达式中替换变量
Expr* IfExpr::subst(std::string s, Expr* expr) {
    return new IfExpr(test_part->subst(s, expr), then_part->subst(s, expr), else_part->subst(s, expr));
}

// 打印If表达式
void IfExpr::print(std::ostream& out) {
    out << "(_if ";
    test_part->print(out);
    out << " _then ";
    then_part->print(out);
    out << " _else ";
    else_part->print(out);
    out << ")";
}

// 根据上下文在If表达式外添加括号进行美观打印
void IfExpr::pretty_print_at(std::ostream& out, precedence_t precedence, std::streampos& newLinePrevPos, bool addParenthesesToLet) {
    if (addParenthesesToLet) {
        out << "(";
    }
    int indentation = out.tellp() - newLinePrevPos;
    out << "_if ";
    test_part->pretty_print_at(out, prec_none, newLinePrevPos, false);
    out << "\n";
    newLinePrevPos = out.tellp();

    out << std::string(indentation, ' ') << "_then ";
    then_part->pretty_print_at(out, prec_none, newLinePrevPos, false);
    out << "\n";
    newLinePrevPos = out.tellp();

    out << std::string(indentation, ' ') << "_else ";
    else_part->pretty_print_at(out, prec_none, newLinePrevPos, false);

    if (addParenthesesToLet) {
        out << ")";
    }
}

//eqexpr
// 等于表达式类，用于表示两个表达式的等值比较
EqExpr::EqExpr(Expr* left, Expr* right) {
    lhs = left;  // 左侧子表达式
    rhs = right; // 右侧子表达式
}

// 构造函数重载，允许直接用整数进行等于比较的创建
EqExpr::EqExpr(int left, int right) {
    lhs = new NumExpr(left); // 将左侧整数转换为数字表达式
    rhs = new NumExpr(right); // 将右侧整数转换为数字表达式
}

// 检查两个等于表达式是否相等
bool EqExpr::equals(Expr* rhs_) {
    EqExpr* other = dynamic_cast<EqExpr*>(rhs_);
    if (other == nullptr) {
        return false; // 类型转换失败表示类型不匹配
    }
    // 递归地比较两边的子表达式是否相等
    return lhs->equals(other->lhs) && rhs->equals(other->rhs);
}

// 解释执行等于表达式，返回一个表示比较结果的布尔值
Val* EqExpr::interp() {
    // 分别解释执行左右子表达式，并比较它们的值是否相等
    return new BoolVal(lhs->interp()->equals(rhs->interp()));
}

// 检查等于表达式是否包含变量
bool EqExpr::has_variable() {
    // 如果左侧或右侧子表达式包含变量，则返回true
    return lhs->has_variable() || rhs->has_variable();
}

// 在等于表达式中替换变量
Expr *EqExpr::subst(std::string s, Expr* expr) {
    // 分别在左右子表达式中替换变量，返回一个新的等于表达式
    return new EqExpr(lhs->subst(s, expr), rhs->subst(s, expr));
}

// 将等于表达式转换为字符串形式
void EqExpr::print(std::ostream& out) {
    out << "(";
    lhs->print(out); // 打印左侧子表达式
    out << "==";     // 打印等于符号
    rhs->print(out); // 打印右侧子表达式
    out << ")";
}

// 根据优先级在等于表达式周围添加括号进行美观打印
void EqExpr::pretty_print_at(std::ostream& out, precedence_t precedence, std::streampos& newLinePrevPos,
                             bool addParenthesesToLet) {
    bool addParentheses = prec_equal <= precedence; // 判断当前上下文是否需要添加括号
    if (addParentheses) {
        out << "(";
    }

    // 美观打印左侧子表达式
    lhs->pretty_print_at(out, prec_equal, newLinePrevPos, true);
    out << " == ";
    // 美观打印右侧子表达式，注意这里传递的优先级信息确保正确地添加括号
    rhs->pretty_print_at(out, prec_none, newLinePrevPos, !addParentheses && addParenthesesToLet);
    if (addParentheses) {
        out << ")";
    }
}



