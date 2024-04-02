#include <sstream>
#include <iostream>
#include "val.h"
#include "expr.h"
#include "env.h"


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


bool NumExpr::equals(PTR(Expr) expr) {
    PTR(NumExpr) n = CAST(NumExpr) (expr);
    if (n == nullptr) {
        return false;
    }
    return val == n->val;
}


PTR(Val) NumExpr::interp(PTR(Env) env) {
    return NEW(NumVal)(val);
}


PTR(Expr) NumExpr::subst(std::string s, PTR(Expr) expr) {
    return THIS;
}

void NumExpr::print(std::ostream& out) {
    out << val;
}


void NumExpr::pretty_print_at(std::ostream& out, precedence_t precedence, std::streampos& newLinePrevPos, bool addParen) {
    out << val;
}


AddExpr::AddExpr(PTR(Expr) left, PTR(Expr) right) {
    lhs = left;
    rhs = right;
}


AddExpr::AddExpr(int left, int right) {
    lhs = NEW(NumExpr)(left);
    rhs = NEW(NumExpr)(right);
}


AddExpr::AddExpr(std::string left, int right) {
    lhs = NEW(VarExpr)(left);
    rhs = NEW(NumExpr)(right);
}

AddExpr::AddExpr(int left, std::string right) {
    lhs = NEW(NumExpr)(left);
    rhs = NEW(VarExpr)(right);
}


AddExpr::AddExpr(std::string left, std::string right) {
    lhs = NEW(VarExpr)(left);
    rhs = NEW(VarExpr)(right);
}

AddExpr::AddExpr(int left, PTR(Expr) right) {
    lhs = NEW(NumExpr)(left);
    rhs = right;
}

AddExpr::AddExpr(PTR(Expr) left, int right) {
    lhs = left;
    rhs = NEW(NumExpr)(right);
}

AddExpr::AddExpr(std::string left, PTR(Expr) right) {
    lhs = NEW(VarExpr)(left);
    rhs = right;
}

AddExpr::AddExpr(PTR(Expr) left, std::string right) {
    lhs = left;
    rhs = NEW(VarExpr)(right);
}


bool AddExpr::equals(PTR(Expr) expr) {
    PTR(AddExpr) a = CAST(AddExpr) (expr);
    if (a == NULL) {
        return false;
    }
    return lhs->equals(a->lhs) && rhs->equals(a->rhs);
}

PTR(Val) AddExpr::interp(PTR(Env) env) {
    return lhs->interp(env)->add_to(rhs->interp(env));
}


PTR(Expr) AddExpr::subst(std::string s, PTR(Expr) expr) {
    return NEW(AddExpr)(lhs->subst(s, expr), rhs->subst(s, expr));
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

    rhs->pretty_print_at(out, prec_equal, newLinePrevPos, addParen && !printParen);
    if (printParen) {
        out << ")";
    }
}


MultExpr::MultExpr(PTR(Expr) left, PTR(Expr) right) {
    lhs = left;
    rhs = right;
}


MultExpr::MultExpr(int left, int right) {
    lhs = NEW(NumExpr)(left);
    rhs = NEW(NumExpr)(right);
}


MultExpr::MultExpr(std::string left, int right) {
    lhs = NEW(VarExpr)(left);
    rhs = NEW(NumExpr)(right);
}


MultExpr::MultExpr(int left, std::string right) {
    lhs = NEW(NumExpr)(left);
    rhs = NEW(VarExpr)(right);
}


MultExpr::MultExpr(std::string left, std::string right) {
    lhs = NEW(VarExpr)(left);
    rhs = NEW(VarExpr)(right);
}

MultExpr::MultExpr(int left, PTR(Expr) right) {
    lhs = NEW(NumExpr)(left);
    rhs = right;
}

MultExpr::MultExpr(PTR(Expr) left, int right) {
    lhs = left;
    rhs = NEW(NumExpr)(right);
}

MultExpr::MultExpr(std::string left, PTR(Expr) right) {
    lhs = NEW(VarExpr)(left);
    rhs = right;
}

MultExpr::MultExpr(PTR(Expr) left, std::string right) {
    lhs = left;
    rhs = NEW(VarExpr)(right);
}


bool MultExpr::equals(PTR(Expr) expr) {
    PTR(MultExpr) m = CAST(MultExpr) (expr);
    if (m == NULL) {
        return false;
    }
    return lhs->equals(m->lhs) && rhs->equals(m->rhs);
}


PTR(Val) MultExpr::interp(PTR(Env) env) {
    return lhs->interp(env)->mult_with(rhs->interp(env));
}


PTR(Expr) MultExpr::subst(std::string s, PTR(Expr) expr) {
    return NEW(MultExpr)(lhs->subst(s, expr), rhs->subst(s, expr));
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

    rhs->pretty_print_at(out, prec_add, newLinePrevPos, addParen && !printParen);
    if (printParen) {
        out << ")";
    }
}


VarExpr::VarExpr(std::string s) {
    val = s;
}


bool VarExpr::equals(PTR(Expr) expr) {
    PTR(VarExpr) var = CAST(VarExpr) (expr);
    if (var == NULL) {
        return false;
    }
    return val == var->val;
}

PTR(Val) VarExpr::interp(PTR(Env) env) {
    return env->lookup(val);
}


PTR(Expr) VarExpr::subst(std::string s, PTR(Expr) expr) {
    if (val == s) {
        return expr;
    }
    return THIS;
}


void VarExpr::print(std::ostream& out) {
    out << val;
}


void VarExpr::pretty_print_at(std::ostream& out, precedence_t precedence, std::streampos& newLinePrevPos, bool addParen) {
    out << val;
}


LetExpr::LetExpr(std::string v, PTR(Expr) r, PTR(Expr) b) {
    variable = v;
    rhs = r;
    body = b;
}


bool LetExpr::equals(PTR(Expr) expr) {
    PTR(LetExpr) other = CAST(LetExpr) (expr);
    if (other == nullptr) {
        return false;
    }
    return variable == other->variable && rhs->equals(other->rhs) && body->equals(other->body);
}


PTR(Val) LetExpr::interp(PTR(Env) env) {
    PTR(Val) rhs_val = rhs->interp(env);
    PTR(Env) new_env = NEW(ExtendedEnv) (variable, rhs_val, env);
    return body->interp(new_env);
}


PTR(Expr) LetExpr::subst(std::string s, PTR(Expr) expr) {
    PTR(Expr) temp = rhs->subst(s, expr);

    if (variable == s) {
        return NEW(LetExpr)(variable, temp, body);
    }

    return NEW(LetExpr)(variable, temp, body->subst(s, expr));
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

bool BoolExpr::equals(PTR(Expr) rhs) {
    PTR(BoolExpr) other = CAST(BoolExpr) (rhs);
    if (other == nullptr) {
        return false;
    }
    return val == other->val;
}

PTR(Val) BoolExpr::interp(PTR(Env) env) {
    return NEW(BoolVal) (val);
}

PTR(Expr) BoolExpr::subst(std::string s, PTR(Expr) expr) {
    return THIS;
}

void BoolExpr::print(std::ostream& out) {
    val ? out << "_true" : out << "_false";
}

void BoolExpr::pretty_print_at(std::ostream& out, precedence_t precedence, std::streampos& newLinePrevPos, bool addParen) {
    val ? out << "_true" : out << "_false";
}


IfExpr::IfExpr(PTR(Expr) test, PTR(Expr) then, PTR(Expr) else_) {
    test_part = test;
    then_part = then;
    else_part = else_;
}

IfExpr::IfExpr(bool test, PTR(Expr) then, PTR(Expr) else_) {
    test_part = NEW(BoolExpr)(test);
    then_part = then;
    else_part = else_;
}

bool IfExpr::equals(PTR(Expr) rhs) {
    PTR(IfExpr) other = CAST(IfExpr) (rhs);
    if (other == nullptr) {
        return false;
    }
    return test_part->equals(other->test_part) && then_part->equals(other->then_part) && else_part->equals(other->else_part);
}

PTR(Val) IfExpr::interp(PTR(Env) env) {
    if (test_part->interp(env)->is_true()) {
        return then_part->interp(env);
    }
    return else_part->interp(env);
}

PTR(Expr) IfExpr::subst(std::string s, PTR(Expr) expr) {
    return NEW(IfExpr)(test_part->subst(s, expr), then_part->subst(s, expr), else_part->subst(s, expr));
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


EqExpr::EqExpr(PTR(Expr) left, PTR(Expr) right) {
    lhs = left;
    rhs = right;
}

EqExpr::EqExpr(int left, int right) {
    lhs = NEW(NumExpr)(left);
    rhs = NEW(NumExpr)(right);
}

EqExpr::EqExpr(std::string left, int right) {
    lhs = NEW(VarExpr)(left);
    rhs = NEW(NumExpr)(right);
}

bool EqExpr::equals(PTR(Expr) rhs_) {
    PTR(EqExpr) other = CAST(EqExpr) (rhs_);
    if (other == nullptr) {
        return false;
    }
    return lhs->equals(other->lhs) && rhs->equals(other->rhs);
}

PTR(Val) EqExpr::interp(PTR(Env) env) {
    return NEW(BoolVal) (lhs->interp(env)->equals(rhs->interp(env)));
}

PTR(Expr) EqExpr::subst(std::string s, PTR(Expr) expr) {
    return NEW(EqExpr)(lhs->subst(s, expr), rhs->subst(s, expr));
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


FunExpr::FunExpr(std::string arg, PTR(Expr) expr) {
    formal_arg = arg;
    body = expr;
}

bool FunExpr::equals(PTR(Expr) rhs) {
    PTR(FunExpr) other = CAST(FunExpr) (rhs);
    if (other == nullptr) {
        return false;
    }
    return formal_arg == other->formal_arg && body->equals(other->body);
}

PTR(Val) FunExpr::interp(PTR(Env) env) {
    return NEW(FunVal) (formal_arg, body, env);
}

PTR(Expr) FunExpr::subst(std::string s, PTR(Expr) expr) {
    if (formal_arg == s) {
        return THIS;
    }
    return NEW(FunExpr)(formal_arg, body->subst(s, expr));
}

void FunExpr::print(std::ostream& out) {
    out << "(_fun (" << formal_arg << ") ";
    body->print(out);
    out << ")";
}

void FunExpr::pretty_print_at(std::ostream& out, precedence_t precedence, std::streampos& newLinePrevPos, bool addParen) {
    if (addParen) {
        out << "(";
    }
    int indent = out.tellp() - newLinePrevPos;
    out << "_fun (" << formal_arg << ") \n";
    newLinePrevPos = out.tellp();

    out << std::string(indent + 2, ' ');
    body->pretty_print_at(out, prec_none, newLinePrevPos, false);
    if (addParen) {
        out << ")";
    }
}


CallExpr::CallExpr(PTR(Expr) func, PTR(Expr) arg) {
    to_be_called = func;
    actual_arg = arg;
}

CallExpr::CallExpr(PTR(Expr) func, int n) {
    to_be_called = func;
    actual_arg = NEW(NumExpr)(n);
}

CallExpr::CallExpr(std::string funcName, int n) {
    to_be_called = NEW(VarExpr)(funcName);
    actual_arg = NEW(NumExpr)(n);
}

CallExpr::CallExpr(std::string funcName, PTR(Expr) arg) {
    to_be_called = NEW(VarExpr)(funcName);
    actual_arg = arg;
}

CallExpr::CallExpr(std::string funcName1, std::string funcName2) {
    to_be_called = NEW(VarExpr)(funcName1);
    actual_arg = NEW(VarExpr)(funcName2);
}

bool CallExpr::equals(PTR(Expr) rhs) {
    PTR(CallExpr) other = CAST(CallExpr) (rhs);
    if (other == nullptr) {
        return false;
    }
    return to_be_called->equals(other->to_be_called) && actual_arg->equals(other->actual_arg);
}

PTR(Val) CallExpr::interp(PTR(Env) env) {
    return to_be_called->interp(env)->call(actual_arg->interp(env));
}

PTR(Expr) CallExpr::subst(std::string s, PTR(Expr) expr) {
    return NEW(CallExpr)(to_be_called->subst(s, expr), actual_arg->subst(s, expr));
}

void CallExpr::print(std::ostream& out) {
    to_be_called->print(out);
    out << "(";
    actual_arg->print(out);
    out << ")";
}

void CallExpr::pretty_print_at(std::ostream& out, precedence_t precedence, std::streampos& newLinePrevPos, bool addParen) {

    to_be_called->pretty_print_at(out, prec_none, newLinePrevPos, true);
    out << "(";
    actual_arg->pretty_print_at(out, prec_none, newLinePrevPos, false);
    out << ")";
}