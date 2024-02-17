#ifndef PARSE_H
#define PARSE_H

#include "expr.h"
#include <iostream>

// Parses an expression from the given input stream and returns a pointer to the expression object.
Expr* parse(std::istream &in);
Expr* parse_str(const std::string& str);

#endif // PARSE_H
