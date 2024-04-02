#include "parse.h"
#include <vector>
#include <set>


PTR(Expr) parse_expr(std::istream& in) {
PTR(Expr) lhs = parse_comparg(in);
skip_whitespace(in);
int c = in.peek();
if (c == '=') {
consume(in, "==");
PTR(Expr) rhs = parse_expr(in);
return NEW(EqExpr) (lhs, rhs);
}
return lhs;
}


PTR(Expr) parse_comparg(std::istream& in) {
PTR(Expr) lhs = parse_addend(in);
skip_whitespace(in);
int c = in.peek();
if (c == '+') {
consume(in, '+');
PTR(Expr) rhs = parse_comparg(in);
return NEW(AddExpr) (lhs, rhs);
}
return lhs;
}


PTR(Expr) parse_addend(std::istream& in) {
skip_whitespace(in);
PTR(Expr) lhs = parse_multicand(in);
skip_whitespace(in);
int c = in.peek();
if (c == '*') {
consume(in, '*');
PTR(Expr) rhs = parse_addend(in);
return NEW(MultExpr) (lhs, rhs);
}
std::set<char> ops = {'+', '*', '=', ')', '_'};
if (c != -1 && ops.find(c) == ops.end()) {
throw std::runtime_error("bad input");
}
return lhs;
}


PTR(Expr) parse_multicand(std::istream& in) {
PTR(Expr) expr = parse_inner(in);
while (in.peek() == '(') {
consume(in, '(');
PTR(Expr) actual_arg = parse_expr(in);
consume(in, ')');
expr = NEW(CallExpr) (expr, actual_arg);
}
return expr;
}


PTR(Expr) parse_inner(std::istream& in) {
skip_whitespace(in);
int c = in.peek();
if (c == '-' || isdigit(c)) {
return parse_num(in);
} else if (isalpha(c)) {
return parse_var(in);
} else if (c == '_') {
consume(in, c);
std::string keyword = parse_keyword(in);
if (keyword == "let") {
return parse_let(in);
} else if (keyword == "false") {
return NEW(BoolExpr) (false);
} else if (keyword == "true") {
return NEW(BoolExpr) (true);
} else if (keyword == "if") {
return parse_if(in);
} else if (keyword == "fun") {
return parse_fun(in);
}
throw std::runtime_error("unknown keyword: " + keyword);
} else if (c == '(') {
consume(in, '(');
PTR(Expr) expr = parse_expr(in);
skip_whitespace(in);
consume(in, ')', "parentheses mismatch");
return expr;
} else {
consume(in, c);
throw std::runtime_error("bad input");
}
}

PTR(Expr) parse_fun(std::istream& in) {
skip_whitespace(in);
consume(in, '(');
PTR(Expr) varExpr = parse_var(in);
consume(in, ')');
PTR(Expr) body = parse_expr(in);
return NEW(FunExpr) (varExpr->to_string(), body);
}

PTR(Expr) parse_num(std::istream& in) {
long num = 0;
bool negative = false;
if (in.peek() == '-') {
negative = true;
consume(in, '-');
}
if (!isdigit(in.peek())) {
throw std::runtime_error("invalid input");
}
int c;
while (isdigit(c = in.peek())) {
consume(in, c);
num = num * 10 + c - '0';
}
if (num > INT_MAX || num < INT_MIN) {
throw std::runtime_error("the number is out of the range of integer!");
}
if (negative) {
num = - num;
}
return NEW(NumExpr) ((int) num);
}

PTR(Expr) parse_let(std::istream& in) {
std::string errorMsg = "wrong format for let expression";

PTR(VarExpr) var = CAST(VarExpr) (parse_var(in));
skip_whitespace(in);
consume(in, '=', errorMsg);
PTR(Expr) rhs = parse_expr(in);
skip_whitespace(in);
consume(in, "_in", errorMsg);
PTR(Expr) body = parse_expr(in);
return NEW(LetExpr) (var->to_string(), rhs, body);
}

PTR(Expr) parse_if(std::istream& in) {
std::string errorMsg = "wrong format for if expression";
PTR(Expr) test_part = parse_expr(in);
consume(in, "_then", errorMsg);
PTR(Expr) then_part = parse_expr(in);
consume(in, "_else", errorMsg);
PTR(Expr) else_part = parse_expr(in);
return NEW(IfExpr) (test_part, then_part, else_part);
}

PTR(Expr) parse_var(std::istream& in) {
skip_whitespace(in);
std::vector<char> s;
int c;
while (isalpha(c = in.peek())) {
consume(in, c);
s.push_back(c);
}
std::set<char> ops = {'+', '*', '=', '(', ')'};
if (c != -1 && !isspace(c) && ops.find(c) == ops.end()) {
throw std::runtime_error("invalid variable name");
}
return NEW(VarExpr) (std::string(s.begin(), s.end()));
}

std::string parse_keyword(std::istream& in) {
    std::stringstream ss;
    char c;
    while (isalpha(c = in.peek())) {
        consume(in, c);
        ss << c;
    }
    return ss.str();
}

void consume(std::istream& in, std::string expect, const std::string& message) {
    for (const char& c : expect) {
        consume(in, c, message);
    }
}

void consume(std::istream& in, int expect, const std::string& message) {
    int c = in.get();
    if (c != expect) {
        throw std::runtime_error(message);
    }
}

void skip_whitespace(std::istream& in) {
    int c;
    while (isspace(c = in.peek())) {
        consume(in, c);
    }
}


PTR(Expr) parse_str(std::string s) {
std::istringstream in(s);
return parse_expr(in);
}