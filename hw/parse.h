#ifndef MSDSCRIPT_PARSER_H
#define MSDSCRIPT_PARSER_H

#include "expr.h"

Expr* parse_expr(std::istream& in);
Expr* parse_comparg(std::istream& in);
Expr* parse_addend(std::istream& in);
Expr* parse_multicand(std::istream& in);
Expr* parse_num(std::istream& in);
Expr* parse_var(std::istream& in);
Expr* parse_let(std::istream& in);
Expr* parse_if(std::istream& in);
std::string parse_keyword(std::istream& in);
void consume(std::istream& in, int expect, const std::string& message = "consume mismatch");
void consume(std::istream& in, std::string expect, const std::string& message = "consume mismatch");
void skip_whitespace(std::istream& in);
Expr* parse_str(std::string s);


#endif