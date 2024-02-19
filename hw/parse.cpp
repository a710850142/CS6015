#include <iostream>
#include "parse.h"

static void consume(std::istream &in, int expect);
void skip_whitespace(std::istream &in);
Expr *parse_num(std::istream &inn);

Expr* parse_expr(std::istream &in) {
    skip_whitespace(in);

    Expr* left = nullptr;

    // 假设首先尝试解析一个数字或括号内的表达式
    left = parse_primary(in);

    skip_whitespace(in);
    while (true) {
        char op = in.peek();
        if (op == '+' || op == '*') {
            in.get(); // 消耗运算符
            Expr* right = parse_primary(in);
            if (op == '+') {
                left = new Add(left, right);
            } else if (op == '*') {
                left = new Mult(left, right);
            }
        } else {
            break;
        }
        skip_whitespace(in);
    }

    return left;
}

Expr* parse_primary(std::istream &in) {
    skip_whitespace(in);

    int c = in.peek();
    if ((c == '-') || isdigit(c)) {
        return parse_num(in);
    } else if (c == '(') {
        consume(in, '(');
        skip_whitespace(in);
        if (in.peek() == ')') { // 空括号检查
            throw std::runtime_error("bad input");
        }
        Expr* e = parse_expr(in);
        skip_whitespace(in);
        if (in.peek() != ')') {
            throw std::runtime_error("bad input"); // 未找到预期的闭合括号
        }
        consume(in, ')'); // 正常消耗闭合的右括号
        return e;
    } else if (isalpha(c)) { // 处理变量名
        std::string varName;
        while (isalnum(in.peek()) || in.peek() == '_') {
            char nextChar = in.get();
            if (nextChar == '_') {
                throw std::runtime_error("invalid input"); // 如果变量名包含下划线，抛出异常
            }
            varName += nextChar;
        }
        if (varName.empty() || !isalpha(varName[0])) {
            throw std::runtime_error("invalid input"); // 确保变量名以字母开头
        }
        return new VarExpr(varName);
    } else {
        throw std::runtime_error("Unexpected character in input");
    }
}





Expr *parse_num(std::istream &inn) {
    int n = 0;
    bool negative = false;

    if (inn.peek() == '-') {
        negative = true;
        inn.get(); // 消费负号

        // 检查负号后是否紧跟数字
        if (!isdigit(inn.peek())) {
            throw std::runtime_error("invalid input"); // 没有数字跟随负号，抛出异常
        }
    }

    while (1) {
        int c = inn.peek();
        if (isdigit(c)) {
            inn.get(); // 消费数字字符
            n = n * 10 + (c - '0');
        } else {
            break;
        }
    }

    if (negative) {
        n = -n;
    }

    return new Num(n);
}


// 假设您已经在 parse.cpp 中包含了 parse.h
Expr* parse_str(const std::string &s) {
    std::istringstream iss(s);
    return parse_expr(iss);
}


static void consume(std::istream &in, int expect) {
    int c = in.get();
    if (c!=expect) {
        throw std::runtime_error("consume mismatch");
    }
}

void skip_whitespace(std::istream &in) {
    while (1) {
        int c = in.peek();
        if (!isspace(c))
            break;
        consume(in, c);
    }
}
