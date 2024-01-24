#include "expr.h"

// Implementations of equals method for each class
bool Num::equals(const Expr* other) const {
    const Num* num = dynamic_cast<const Num*>(other);
    return num && value == num->value;
}

bool Add::equals(const Expr* other) const {
    const Add* add = dynamic_cast<const Add*>(other);
    return add && lhs->equals(add->lhs) && rhs->equals(add->rhs);
}

bool Mult::equals(const Expr* other) const {
    const Mult* mult = dynamic_cast<const Mult*>(other);
    return mult && lhs->equals(mult->lhs) && rhs->equals(mult->rhs);
}

bool VarExpr::equals(const Expr* other) const {
    const VarExpr* var = dynamic_cast<const VarExpr*>(other);
    return var && name == var->name;
}
