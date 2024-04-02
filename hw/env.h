#ifndef MSDSCRIPT_ENV_H
#define MSDSCRIPT_ENV_H

#include "pointer.h"
#include <string>

// Forward declarations of the Val and Expr classes to resolve circular dependencies.
class Val;
class Expr;

// The Env (Environment) class is an abstract base class for environments in the interpreter.
// Environments map variable names to their values.
CLASS(Env) {
public:
    // A static pointer to an empty environment. Used as the base case for lookup operations.
    static PTR(Env) empty;

    // A pure virtual function that must be implemented by derived classes.
    // It takes a variable name and returns the associated value, if found.
    virtual PTR(Val) lookup(std::string find_name) = 0;
};

// The EmptyEnv class represents an empty environment. It derives from Env.
// Trying to lookup a variable in this environment will always fail.
class EmptyEnv : public Env {
public:
    // Implements the lookup function for an empty environment.
    // Since the environment is empty, it throws an error indicating a free variable.
    PTR(Val) lookup(std::string find_name);
};

// The ExtendedEnv class represents a non-empty environment, extending another environment.
// It contains a name-value binding and a pointer to the rest of the environment.
class ExtendedEnv : public Env {
private:
    // The name of the variable bound in this environment.
    std::string name;
    // The value associated with the name in this environment.
    PTR(Val) val;
    // A pointer to the rest of the environment, allowing for a chain of bindings.
    PTR(Env) rest;
public:
    // Constructor for ExtendedEnv. Initializes an environment with a given name-value binding
    // and the rest of the environment.
    ExtendedEnv(std::string name_, PTR(Val) val_, PTR(Env) rest_);

    // Implements the lookup function for a non-empty environment.
    // It searches for a variable name, returning its value if found, or recursing into the rest of the environment.
    PTR(Val) lookup(std::string find_name);
};

#endif // MSDSCRIPT_ENV_H
