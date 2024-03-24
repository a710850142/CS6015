#include <sstream>
#include "val.h"
#include "expr.h"


std::string Expr::to_string() {
    std::stringstream st("");
    print(st);
    return st.str();
}


std::string Expr::to_pretty_string() {
    std::stringstream st("");
    pretty_print(st);
    return st.str();
}


void Expr::pretty_print(std::ostream &out) {
    std::streampos pos = out.tellp();
    pretty_print_at(out, prec_none, pos, false);
}


NumExpr::NumExpr(int v) {
    val = v;
}


bool NumExpr::equals(Expr* expr) {
    NumExpr *n = dynamic_cast<NumExpr*>(expr);
    if (n == nullptr) {
        return false;
    }
    return val == n->val;
}


Val* NumExpr::interp() {
    return new NumVal(val);
}


Expr* NumExpr::subst(std::string s, Expr *expr) {
    return this;
}


void NumExpr::print(std::ostream& out) {
    out << val;
}


void NumExpr::pretty_print_at(std::ostream& out, precedence_t precedence, std::streampos& newLinePrevPos, bool addParen) {
    out << val;
}


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

AddExpr::AddExpr(std::string left, Expr* right) {
    lhs = new VarExpr(left);
    rhs = right;
}

AddExpr::AddExpr(Expr* left, std::string right) {
    lhs = left;
    rhs = new VarExpr(right);
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


void AddExpr::pretty_print_at(std::ostream &out, precedence_t precedence, std::streampos& newLinePrevPos, bool addParen) {
    bool printParen = prec_add <= precedence;
    if (printParen) {
        out << "(";
    }
    lhs->pretty_print_at(out, prec_add, newLinePrevPos, true);
    out << " + ";
    // let as right arg in AddExpr never need parentheses
    rhs->pretty_print_at(out, prec_equal, newLinePrevPos, false);
    if (printParen) {
        out << ")";
    }
}


MultExpr::MultExpr(Expr *left, Expr *right) {
    lhs = left;
    rhs = right;
}


MultExpr::MultExpr(int left, int right) {
    lhs = new NumExpr(left);
    rhs = new NumExpr(right);
}


MultExpr::MultExpr(std::string left, int right) {
    lhs = new VarExpr(left);
    rhs = new NumExpr(right);
}


MultExpr::MultExpr(int left, std::string right) {
    lhs = new NumExpr(left);
    rhs = new VarExpr(right);
}


MultExpr::MultExpr(std::string left, std::string right) {
    lhs = new VarExpr(left);
    rhs = new VarExpr(right);
}

MultExpr::MultExpr(int left, Expr* right) {
    lhs = new NumExpr(left);
    rhs = right;
}

MultExpr::MultExpr(Expr* left, int right) {
    lhs = left;
    rhs = new NumExpr(right);
}

MultExpr::MultExpr(std::string left, Expr* right) {
    lhs = new VarExpr(left);
    rhs = right;
}

MultExpr::MultExpr(Expr* left, std::string right) {
    lhs = left;
    rhs = new VarExpr(right);
}


bool MultExpr::equals(Expr* expr) {
    MultExpr *m = dynamic_cast<MultExpr*>(expr);
    if (m == NULL) {
        return false;
    }
    return lhs->equals(m->lhs) && rhs->equals(m->rhs);
}


Val* MultExpr::interp() {
    return lhs->interp()->mult_with(rhs->interp());
}


Expr* MultExpr::subst(std::string s, Expr *expr) {
    return new MultExpr(lhs->subst(s, expr), rhs->subst(s, expr));
}


void MultExpr::print(std::ostream& out) {
    out << "(";
    lhs -> print(out);
    out << "*";
    rhs -> print(out);
    out << ")";
}


void MultExpr::pretty_print_at(std::ostream &out, precedence_t precedence, std::streampos& newLinePrevPos, bool addParen) {
    bool printParen = prec_mult <= precedence;
    if (printParen) {
        out << "(";
    }
    lhs->pretty_print_at(out, prec_mult, newLinePrevPos, true);
    out << " * ";
    // add parentheses for rhs when : 1. rhs is let 2. the outermost mult expression is followed with an add expression
    rhs->pretty_print_at(out, prec_add, newLinePrevPos, addParen && !printParen);
    if (printParen) {
        out << ")";
    }
}


VarExpr::VarExpr(std::string s) {
    val = s;
}

bool VarExpr::equals(Expr* expr) {
    VarExpr *var = dynamic_cast<VarExpr*>(expr);
    if (var == NULL) {
        return false;
    }
    return val == var->val;
}


Val * VarExpr::interp() {
    throw std::runtime_error("A variable has no value!");
}


Expr* VarExpr::subst(std::string s, Expr* expr) {
    if (val == s) {
        return expr;
    }
    return this;
}


void VarExpr::print(std::ostream& out) {
    out << val;
}


void VarExpr::pretty_print_at(std::ostream& out, precedence_t precedence, std::streampos& newLinePrevPos, bool addParen) {
    out << val;
}

std::string VarExpr::getVal() {
    return val;
}


LetExpr::LetExpr(std::string v, Expr* r, Expr* b) {
    variable = v;
    rhs = r;
    body = b;
}


bool LetExpr::equals(Expr* expr) {
    LetExpr* other = dynamic_cast<LetExpr*>(expr);
    if (other == nullptr) {
        return false;
    }
    return variable == other->variable && rhs->equals(other->rhs) && body->equals(other->body);
}


Val * LetExpr::interp() {
    Val* rhs_val = rhs->interp();
    return body->subst(variable, rhs_val->to_expr())->interp();
}


Expr* LetExpr::subst(std::string s, Expr* expr) {
    Expr* temp = rhs->subst(s, expr);
    if (variable == s) {
        return new LetExpr(variable, temp, body);
    }
    return new LetExpr(variable, temp, body->subst(s, expr));
}


void LetExpr::print(std::ostream& out) {
    out << "(_let " << variable << "=";
    rhs->print(out);
    out << " _in ";
    body->print(out);
    out << ")";
}


void LetExpr::pretty_print_at(std::ostream& out, precedence_t precedence, std::streampos& newLinePrevPos, bool addParen) {
    if (addParen) {
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

    if (addParen) {
        out << ")";
    }
}

BoolExpr::BoolExpr(bool v) {
    val = v;
}

bool BoolExpr::equals(Expr* rhs) {
    BoolExpr* other = dynamic_cast<BoolExpr*>(rhs);
    if (other == nullptr) {
        return false;
    }
    return val == other->val;
}

Val* BoolExpr::interp() {
    return new BoolVal(val);
}

Expr* BoolExpr::subst(std::string s, Expr* expr) {
    return this;
}

void BoolExpr::print(std::ostream& out) {
    val ? out << "_true" : out << "_false";
}

void BoolExpr::pretty_print_at(std::ostream& out, precedence_t precedence, std::streampos& newLinePrevPos, bool addParen) {
    val ? out << "_true" : out << "_false";
}


IfExpr::IfExpr(Expr* test, Expr* then, Expr* else_) {
    test_part = test;
    then_part = then;
    else_part = else_;
}

IfExpr::IfExpr(bool test, Expr* then, Expr* else_) {
    test_part = new BoolExpr(test);
    then_part = then;
    else_part = else_;
}

bool IfExpr::equals(Expr* rhs) {
    IfExpr* other = dynamic_cast<IfExpr*>(rhs);
    if (other == nullptr) {
        return false;
    }
    return test_part->equals(other->test_part) && then_part->equals(other->then_part) && else_part->equals(other->else_part);
}

Val* IfExpr::interp() {
    if (test_part->interp()->is_true()) {
        return then_part->interp();
    }
    return else_part->interp();
}

Expr* IfExpr::subst(std::string s, Expr* expr) {
    return new IfExpr(test_part->subst(s, expr), then_part->subst(s, expr), else_part->subst(s, expr));
}

void IfExpr::print(std::ostream& out) {
    out << "(_if ";
    test_part->print(out);
    out << " _then ";
    then_part->print(out);
    out << " _else ";
    else_part->print(out);
    out << ")";
}

void IfExpr::pretty_print_at(std::ostream& out, precedence_t precedence, std::streampos& newLinePrevPos, bool addParen) {
    if (addParen) {
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

    if (addParen) {
        out << ")";
    }
}


EqExpr::EqExpr(Expr* left, Expr* right) {
    lhs = left;
    rhs = right;
}

EqExpr::EqExpr(int left, int right) {
    lhs = new NumExpr(left);
    rhs = new NumExpr(right);
}

EqExpr::EqExpr(std::string left, int right) {
    lhs = new VarExpr(left);
    rhs = new NumExpr(right);
}

bool EqExpr::equals(Expr* rhs_) {
    EqExpr* other = dynamic_cast<EqExpr*>(rhs_);
    if (other == nullptr) {
        return false;
    }
    return lhs->equals(other->lhs) && rhs->equals(other->rhs);
}

Val* EqExpr::interp() {
    return new BoolVal(lhs->interp()->equals(rhs->interp()));
}

Expr *EqExpr::subst(std::string s, Expr* expr) {
    return new EqExpr(lhs->subst(s, expr), rhs->subst(s, expr));
}

void EqExpr::print(std::ostream& out) {
    out << "(";
    lhs->print(out);
    out << "==";
    rhs->print(out);
    out << ")";
}

void EqExpr::pretty_print_at(std::ostream& out, precedence_t precedence, std::streampos& newLinePrevPos,
                             bool addParen) {
    bool printParen = prec_equal <= precedence;
    if (printParen) {
        out << "(";
    }
    lhs->pretty_print_at(out, prec_equal, newLinePrevPos, true);
    out << " == ";
    rhs->pretty_print_at(out, prec_none, newLinePrevPos, !printParen && addParen);
    if (printParen) {
        out << ")";
    }
}
// Constructor for a function expression with a single formal argument and a body expression.
FunExpr::FunExpr(std::string arg, Expr* expr) {
    formal_arg = arg; // Set the formal argument of the function.
    body = expr;      // Set the body of the function.
}

// Method to compare this function expression with another expression for equality.
bool FunExpr::equals(Expr *rhs) {
    FunExpr* other = dynamic_cast<FunExpr*>(rhs); // Attempt to cast the right-hand side to a FunExpr.
    if (other == nullptr) {
        return false; // If casting fails, they cannot be equal (not both FunExpr).
    }
    // Check if both the formal argument and body are equal.
    return formal_arg == other->formal_arg && body->equals(other->body);
}

// Interpret the function expression by creating a new function value.
Val* FunExpr::interp() {
    return new FunVal(formal_arg, body); // Return a new function value with the same formal argument and body.
}

// Substitute occurrences of a variable with an expression in the function body, but not if the variable is the formal argument.
Expr* FunExpr::subst(std::string s, Expr* expr) {
    if (formal_arg == s) {
        return this; // If the variable is the formal argument, return the function unchanged.
    }
    // Otherwise, return a new FunExpr with the body substituted.
    return new FunExpr(formal_arg, body->subst(s, expr));
}

// Print the function expression in a simple form to an output stream.
void FunExpr::print(std::ostream& out) {
    out << "(_fun (" << formal_arg << ") "; // Start with the function syntax.
    body->print(out); // Print the body of the function.
    out << ")"; // Close the function expression.
}

// Pretty-print the function expression with proper indentation and line breaks.
void FunExpr::pretty_print_at(std::ostream& out, precedence_t precedence, std::streampos& newLinePrevPos, bool addParen) {
    if (addParen) {
        out << "("; // Optionally add opening parenthesis.
    }
    int indent = out.tellp() - newLinePrevPos; // Calculate current indentation.
    out << "_fun (" << formal_arg << ") \n"; // Print the function signature with a line break.
    newLinePrevPos = out.tellp(); // Update the position for new line indentation.

    out << std::string(indent + 2, ' '); // Add indentation for the body.
    body->pretty_print_at(out, prec_none, newLinePrevPos, false); // Print the body with updated indentation.
    if (addParen) {
        out << ")"; // Optionally add closing parenthesis.
    }
}

// 构造函数，接受一个表示函数和一个表示实际参数的表达式
CallExpr::CallExpr(Expr* func, Expr* arg) {
    to_be_called = func; // 被调用的函数表达式
    actual_arg = arg; // 实际参数表达式
}

// 构造函数，接受一个表示函数的表达式和一个整数作为实际参数
CallExpr::CallExpr(Expr* func, int n) {
    to_be_called = func; // 被调用的函数表达式
    actual_arg = new NumExpr(n); // 使用整数创建一个数值表达式作为实际参数
}

// 构造函数，接受一个表示函数名的字符串和一个整数作为实际参数
CallExpr::CallExpr(std::string funcName, int n) {
    to_be_called = new VarExpr(funcName); // 使用函数名创建一个变量表达式作为被调用的函数
    actual_arg = new NumExpr(n); // 使用整数创建一个数值表达式作为实际参数
}

// 构造函数，接受一个表示函数名的字符串和一个表示实际参数的表达式
CallExpr::CallExpr(std::string funcName, Expr* arg) {
    to_be_called = new VarExpr(funcName); // 使用函数名创建一个变量表达式作为被调用的函数
    actual_arg = arg; // 实际参数表达式
}

// 构造函数，接受两个表示函数名的字符串，分别作为被调用的函数和实际参数
CallExpr::CallExpr(std::string funcName1, std::string funcName2) {
    to_be_called = new VarExpr(funcName1); // 使用第一个函数名创建一个变量表达式作为被调用的函数
    actual_arg = new VarExpr(funcName2); // 使用第二个函数名创建一个变量表达式作为实际参数
}

// 检查当前调用表达式是否等于另一个表达式
bool CallExpr::equals(Expr* rhs) {
    CallExpr* other = dynamic_cast<CallExpr*>(rhs); // 尝试将rhs转换为CallExpr*
    if (other == nullptr) {
        return false; // 如果转换失败，则不相等
    }
    // 比较被调用的函数和实际参数是否相等
    return to_be_called->equals(other->to_be_called) && actual_arg->equals(other->actual_arg);
}

// 对调用表达式进行求值
Val* CallExpr::interp() {
    // 首先对被调用的函数进行求值，然后调用求值结果上的call方法，传入实际参数的求值结果
    return to_be_called->interp()->call(actual_arg->interp());
}

// 替换表达式中的变量
Expr* CallExpr::subst(std::string s, Expr* expr) {
    // 分别对被调用的函数和实际参数进行替换，然后创建一个新的CallExpr作为结果
    return new CallExpr(to_be_called->subst(s, expr), actual_arg->subst(s, expr));
}

// 将调用表达式打印到输出流
void CallExpr::print(std::ostream& out) {
    to_be_called->print(out); // 打印被调用的函数
    out << "(";
    actual_arg->print(out); // 打印实际参数
    out << ")";
}

void CallExpr::pretty_print_at(std::ostream& out, precedence_t precedence, std::streampos& newLinePrevPos, bool addParen) {
    VarExpr* tmp1 = dynamic_cast<VarExpr*>(to_be_called);
    CallExpr* tmp2 = dynamic_cast<CallExpr*>(to_be_called);
    bool printParen = tmp1 == nullptr && tmp2 == nullptr;
    if (printParen) out << "(";
    to_be_called->pretty_print_at(out, prec_none, newLinePrevPos, false);
    if (printParen) out << ")";
    out << "(";
    actual_arg->pretty_print_at(out, prec_none, newLinePrevPos, false);
    out << ")";
}