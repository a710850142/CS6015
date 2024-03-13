#include "val.h"
#include "expr.h"

// Definition of the NumVal class for handling numeric values

// Constructor: Initializes a NumVal object with an integer value
NumVal::NumVal(int v) {
    val = v;
}

// Converts the numeric value to a NumExpr expression
Expr* NumVal::to_expr() {
    return new NumExpr(val);
}

// Checks if this NumVal object equals another Val object
bool NumVal::equals(Val* rhs) {
    NumVal* numVal = dynamic_cast<NumVal*>(rhs); // Attempt to cast the right-hand side to a NumVal
    if (numVal == nullptr) { // If the cast fails, the objects are not of the same type
        return false;
    }
    return val == numVal->val; // Return true if the values are equal
}

// Adds this NumVal object to another Val object and returns the result
Val* NumVal::add_to(Val* rhs) {
    NumVal* numVal = dynamic_cast<NumVal*>(rhs);
    if (numVal == nullptr) { // Check if the right-hand side is a NumVal
        throw std::runtime_error("add of non-number"); // Throw an error if it's not
    }
    return new NumVal(val + numVal->val); // Return the sum as a new NumVal
}

// Multiplies this NumVal object with another Val object and returns the result
Val* NumVal::mult_with(Val* rhs) {
    NumVal* numVal = dynamic_cast<NumVal*>(rhs);
    if (numVal == nullptr) { // Check if the right-hand side is a NumVal
        throw std::runtime_error("multiply with non-number"); // Throw an error if it's not
    }
    return new NumVal(val * numVal->val); // Return the product as a new NumVal
}

// Returns the string representation of the numeric value
std::string NumVal::to_string() {
    return std::to_string(val);
}

// Throws an error because a numeric value cannot be evaluated to a boolean
bool NumVal::is_true() {
    throw std::runtime_error("a number value can not be evaluated to a boolean");
}

// Definition of the BoolVal class for handling boolean values

// Constructor: Initializes a BoolVal object with a boolean value
BoolVal::BoolVal(bool v) {
    val = v;
}

// Converts the boolean value to a BoolExpr expression
Expr* BoolVal::to_expr() {
    return new BoolExpr(val);
}

// Checks if this BoolVal object equals another Val object
bool BoolVal::equals(Val *rhs) {
    BoolVal* other = dynamic_cast<BoolVal*>(rhs); // Attempt to cast the right-hand side to a BoolVal
    if (other == nullptr) { // If the cast fails, the objects are not of the same type
        return false;
    }
    return val == other->val; // Return true if the boolean values are equal
}

// Throws an error because addition is not defined for boolean values
Val* BoolVal::add_to(Val *rhs) {
    throw std::runtime_error("add of non-number");
}

// Throws an error because multiplication is not defined for boolean values
Val* BoolVal::mult_with(Val *rhs) {
    throw std::runtime_error("multiply with non-number");
}

// Returns the string representation of the boolean value, prefixed with an underscore
std::string BoolVal::to_string() {
    return val ? "_true" : "_false";
}

// Returns the boolean value itself
bool BoolVal::is_true() {
    return val;
}
