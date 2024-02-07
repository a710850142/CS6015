#ifndef EXPR_H
#define EXPR_H

#include <string>
#include <sstream>
#include <iostream>
#include <stdexcept>

// Define an enumeration for precedence levels of expressions. This is useful for pretty printing expressions with correct parentheses.
typedef enum {
    prec_none,      // No precedence, default level
    prec_add,       // Precedence of addition, lower than multiplication
    prec_mult       // Precedence of multiplication, higher than addition
} precedence_t;

// Base class for all expression types. It declares the interface that all expressions must implement.
class Expr {
public:
    virtual ~Expr() {} // Virtual destructor to ensure proper cleanup of derived class objects through base class pointers.

    // Evaluate the expression and return its value as an integer.
    virtual int interp() const = 0;
    // Return true if the expression contains a variable, false otherwise.
    virtual bool has_variable() const = 0;
    // Replace occurrences of a variable with another expression and return the new expression.
    virtual Expr* subst(const std::string& varName, const Expr* replacement) const = 0;
    // Check if this expression is equal to another expression.
    virtual bool equals(const Expr* other) const = 0;

    // Print the expression to an output stream.
    virtual void print(std::ostream &os) const = 0;
    // Convert the expression to a string and return it.
    std::string to_string() const;

    // Default implementation of pretty printing. Can be overridden in derived classes for custom behavior.
    virtual void pretty_print(std::ostream &os) const { print(os); }
    // Auxiliary method for pretty_print, allowing for precedence-aware printing.
    virtual void pretty_print_at(std::ostream &os, precedence_t prec) const { pretty_print(os); }
};

// Represents numeric literals in expressions.
class Num : public Expr {
    int value; // The numeric value of this literal.
public:
    Num(int value) : value(value) {}

    // Implementations of virtual methods from Expr.
    int interp() const override;
    bool has_variable() const override;
    Expr* subst(const std::string& varName, const Expr* replacement) const override;
    bool equals(const Expr* other) const override;

    // Methods to print and pretty print the numeric literal.
    void print(std::ostream &os) const override;
    void pretty_print(std::ostream &os) const override;
};

// Represents addition operations between two expressions.
class Add : public Expr {
    Expr* lhs; // Left-hand side expression
    Expr* rhs; // Right-hand side expression
public:
    Add(Expr* lhs, Expr* rhs) : lhs(lhs), rhs(rhs) {}
    ~Add(); // Destructor to clean up dynamically allocated expressions.

    // Implementations of virtual methods from Expr.
    int interp() const override;
    bool has_variable() const override;
    Expr* subst(const std::string& varName, const Expr* replacement) const override;
    bool equals(const Expr* other) const override;

    // Methods to print and pretty print the addition expression, with precedence handling.
    void print(std::ostream &os) const override;
    void pretty_print(std::ostream &os) const override;
    void pretty_print_at(std::ostream &os, precedence_t prec) const override;
};

// Represents multiplication operations between two expressions.
class Mult : public Expr {
    Expr* lhs; // Left-hand side expression
    Expr* rhs; // Right-hand side expression
public:
    Mult(Expr* lhs, Expr* rhs) : lhs(lhs), rhs(rhs) {}
    ~Mult(); // Destructor to clean up dynamically allocated expressions.

    // Implementations of virtual methods from Expr.
    int interp() const override;
    bool has_variable() const override;
    Expr* subst(const std::string& varName, const Expr* replacement) const override;
    bool equals(const Expr* other) const override;

    // Methods to print and pretty print the multiplication expression, with precedence handling.
    void print(std::ostream &os) const override;
    void pretty_print(std::ostream &os) const override;
    void pretty_print_at(std::ostream &os, precedence_t prec) const override;
};

// Represents variable expressions in the expression tree.
class VarExpr : public Expr {
    std::string name; // The name of the variable.
public:
    VarExpr(const std::string& name) : name(name) {}

    // Implementations of virtual methods from Expr.
    int interp() const override; // Throws an exception because variables cannot be interpreted without a value.
    bool has_variable() const override;
    Expr* subst(const std::string& varName, const Expr* replacement) const override;
    bool equals(const Expr* other) const override;

    // Print the variable's name.
    void print(std::ostream &os) const override;
};

#endif // EXPR_H
