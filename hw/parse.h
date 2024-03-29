#ifndef MSDSCRIPT_PARSER_H
#define MSDSCRIPT_PARSER_H

#include "expr.h"

PTR(Expr) parse_expr(std::istream& in);
PTR(Expr) parse_comparg(std::istream& in);
PTR(Expr) parse_addend(std::istream& in);
PTR(Expr) parse_multicand(std::istream& in);
PTR(Expr) parse_inner(std::istream& in);
PTR(Expr) parse_num(std::istream& in);
PTR(Expr) parse_var(std::istream& in);
PTR(Expr) parse_let(std::istream& in);
PTR(Expr) parse_if(std::istream& in);
PTR(Expr) parse_fun(std::istream& in);
std::string parse_keyword(std::istream& in);
void consume(std::istream& in, int expect, const std::string& message = "consume mismatch");
void consume(std::istream& in, std::string expect, const std::string& message = "consume mismatch");
void skip_whitespace(std::istream& in);
PTR(Expr) parse_str(std::string s);


#endif //MSDSCRIPT_PARSER_H