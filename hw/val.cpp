#include "val.h"
#include "expr.h"

NumVal::NumVal(int v) {
    val = v;
}

PTR(Expr) NumVal::to_expr() {
    return NEW(NumExpr) (val);
}

bool NumVal::equals(PTR(Val) rhs) {
    PTR(NumVal) numVal = CAST(NumVal) (rhs);
    if (numVal == nullptr) {
        return false;
    }
    return val == numVal->val;
}

PTR(Val) NumVal::add_to(PTR(Val) rhs) {
PTR(NumVal) numVal = CAST(NumVal) (rhs);
if (numVal == nullptr) {
throw std::runtime_error("add of non-number");
}
return NEW(NumVal) ((unsigned) val + (unsigned) numVal->val);
}

PTR(Val) NumVal::mult_with(PTR(Val) rhs) {
PTR(NumVal) numVal = CAST(NumVal) (rhs);
if (numVal == nullptr) {
throw std::runtime_error("multiply with non-number");
}
return NEW(NumVal) ((unsigned) val * (unsigned) numVal->val);
}

std::string NumVal::to_string() {
    return std::to_string(val);
}

bool NumVal::is_true() {
    throw std::runtime_error("a number value can not be evaluated to a boolean");
}

PTR(Val) NumVal::call(PTR(Val) actual_arg) {
throw std::runtime_error("no function to call!");
}


BoolVal::BoolVal(bool v) {
    val = v;
}

PTR(Expr) BoolVal::to_expr() {
    return NEW(BoolExpr) (val);
}

bool BoolVal::equals(PTR(Val) rhs) {
PTR(BoolVal) other = CAST(BoolVal) (rhs);
if (other == nullptr) {
return false;
}
return val == other->val;
}

PTR(Val) BoolVal::add_to(PTR(Val) rhs) {
throw std::runtime_error("add of non-number");
}

PTR(Val) BoolVal::mult_with(PTR(Val) rhs) {
throw std::runtime_error("multiply with non-number");
}

std::string BoolVal::to_string() {
    return val ? "_true" : "_false";
}

bool BoolVal::is_true() {
    return val;
}

PTR(Val) BoolVal::call(PTR(Val) actual_arg) {
throw std::runtime_error("no function to call!");
}


FunVal::FunVal(std::string arg, PTR(Expr) expr) {
    formal_arg = arg;
    body = expr;
}

PTR(Expr) FunVal::to_expr() {
    return NEW(FunExpr)(formal_arg, body);
}

bool FunVal::equals(PTR(Val) rhs) {
PTR(FunVal) other = CAST(FunVal) (rhs);
if (other == nullptr) {
return false;
}
return formal_arg == other->formal_arg && body->equals(other->body);
}

PTR(Val) FunVal::add_to(PTR(Val) rhs) {
throw std::runtime_error("add of non-number");
}

PTR(Val) FunVal::mult_with(PTR(Val) rhs) {
throw std::runtime_error("multiply with non-number");
}

std::string FunVal::to_string() {
    return to_expr()->to_pretty_string();
}

bool FunVal::is_true() {
    throw std::runtime_error("a function value can not be evaluated to a boolean");
}

PTR(Val) FunVal::call(PTR(Val) actual_arg) {
return body->subst(formal_arg, actual_arg->to_expr())->interp();
}





