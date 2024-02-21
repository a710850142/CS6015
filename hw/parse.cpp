#include <iostream>
#include "parse.h"

static void consume(std::istream &in, int expect);
void skip_whitespace(std::istream &in);
Expr *parse_num(std::istream &in);
Expr *parse_let(std::istream &in);

Expr* parse_expr(std::istream &in) {
    skip_whitespace(in);

    Expr* left = nullptr;

    // 尝试解析 let 表达式
    if (in.peek() == 'l') {
        return parse_let(in);
    }

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
        if (in.peek() == ')') { // 检查是否为空的括号表达式
            throw std::runtime_error("bad input");
        }
        Expr* e = parse_expr(in);
        skip_whitespace(in); // 在尝试消耗闭括号前，再次跳过任何空格
        if (in.peek() != ')') { // 确保下一个字符是闭括号
            throw std::runtime_error("bad input"); // 如果不是，则抛出 "bad input"
        }
        consume(in, ')');
        return e;
    } else if (isalpha(c)) { // 确保它开始于一个字母
        std::string varName;
        while (isalpha(in.peek()) || isdigit(in.peek()) || in.peek() == '_') { // 变量名可以包含字母和数字，检查是否含有下划线
            char nextChar = in.get();
            if (nextChar == '_') {
                throw std::runtime_error("invalid input"); // 如果变量名包含下划线，抛出异常
            }
            varName += nextChar;
        }
        return new VarExpr(varName);
    } else {
        throw std::runtime_error("Unexpected character in input");
    }
}



Expr *parse_num(std::istream &in) {
    int n = 0;
    bool negative = false;

    if (in.peek() == '-') {
        negative = true;
        in.get(); // 消费负号

        if (!isdigit(in.peek())) {
            throw std::runtime_error("invalid input"); // 没有数字跟随负号，抛出异常
        }
    }

    while (isdigit(in.peek())) {
        int c = in.get(); // 消费数字字符
        n = n * 10 + (c - '0');
    }

    if (negative) {
        n = -n;
    }

    return new Num(n);
}

Expr* parse_let(std::istream &in) {
    std::string token;
    in >> token;
    if (token != "let") {
        throw std::runtime_error("Expected 'let'");
    }
    skip_whitespace(in);

    std::string varName;
    in >> varName;
    skip_whitespace(in);

    consume(in, '=');
    Expr* bindingExpr = parse_expr(in);
    skip_whitespace(in);

    std::string inToken;
    in >> inToken;
    if (inToken != "_in") {
        throw std::runtime_error("Expected '_in' in let expression");
    }
    Expr* bodyExpr = parse_expr(in);
    return new LetExpr(varName, bindingExpr, bodyExpr);
}

static void consume(std::istream &in, int expect) {
    int c = in.get();
    if (c != expect) {
        throw std::runtime_error("consume mismatch");
    }
}

void skip_whitespace(std::istream &in) {
    while (isspace(in.peek())) {
        in.get();
    }
}

// 假设您已经在 parse.cpp 中包含了 parse.h
Expr* parse_str(const std::string &s) {
    std::istringstream iss(s);
    return parse_expr(iss);
}
