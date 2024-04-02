#include "env.h"

// Declaration of a pointer to the Env class, used for representing an environment.
PTR(Env) Env::empty = NEW(EmptyEnv)();

// Implementation of the lookup method for the EmptyEnv class.
// This method throws an error indicating that a variable is free (unbound) when it is looked up in an empty environment.
PTR(Val) EmptyEnv::lookup(std::string find_name) {
    throw std::runtime_error("free variable: " + find_name);
};

// Constructor for the ExtendedEnv class, which represents a non-empty environment.
// It initializes an environment with a variable name, its value, and the rest of the environment.
ExtendedEnv::ExtendedEnv(std::string name_, PTR(Val) val_, PTR(Env) rest_) {
    name = name_;
    val = val_;
    rest = rest_;
}

// Implementation of the lookup method for the ExtendedEnv class.
// This method searches for a variable by name in the environment. If the name matches,
// it returns the associated value. If not, it recursively searches in the rest of the environment.
PTR(Val) ExtendedEnv::lookup(std::string find_name) {
    if (find_name == name) { // Check if the current environment binds the name.
        return val; // Return the value if found.
    } else {
        return rest->lookup(find_name); // Recursively search in the rest of the environment.
    }
};
