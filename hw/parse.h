#ifndef PARSE_H
#define PARSE_H

#include "expr.h"
#include <istream>

// 解析表达式
Expr* parse_expr(std::istream &in);
Expr* parse_primary(std::istream &in);

// 解析数字
Expr* parse_num(std::istream &in);

// 解析字符串表达式为 Expr 对象
Expr* parse_str(const std::string &s);

// 跳过空白字符
void skip_whitespace(std::istream &in);

#endif // PARSE_H
