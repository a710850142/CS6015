#include "parse.h"
#include <vector>
#include <set>

// 解析表达式
Expr* parse_expr(std::istream& in) {
    Expr* lhs = parse_comparg(in);
    skip_whitespace(in);
    int c = in.peek();
    if (c == '=') {
        consume(in, "==");
        Expr* rhs = parse_expr(in);
        return new EqExpr(lhs, rhs); // 等于表达式
    }
    return lhs; // 如果不是等于表达式，返回左侧表达式
}

// 解析加法和减法的组成部分
Expr* parse_comparg(std::istream& in) {
    Expr* lhs = parse_addend(in);
    skip_whitespace(in);
    int c = in.peek();
    if (c == '+') {
        consume(in, '+');
        Expr* rhs = parse_comparg(in);
        return new AddExpr(lhs, rhs); // 加法表达式
    }
    return lhs; // 如果不是加法，返回左侧表达式
}

// 解析乘法和除法的组成部分
Expr* parse_addend(std::istream& in) {
    skip_whitespace(in);
    Expr* lhs = parse_multicand(in);
    skip_whitespace(in);
    int c = in.peek();
    if (c == '*') {
        consume(in, '*');
        Expr* rhs = parse_addend(in);
        return new MultExpr(lhs, rhs); // 乘法表达式
    }
    std::set<char> ops = {'+', '*', '=', ')', '_'}; // 处理 "<expr> <expr>" 错误
    if (c != -1 && ops.find(c) == ops.end()) {
        throw std::runtime_error("bad input");
    }
    return lhs;
}

// 解析乘法的因子
Expr* parse_multicand(std::istream& in) {
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
            return parse_let(in); // let表达式
        } else if (keyword == "false") {
            return new BoolExpr(false); // 布尔表达式 false
        } else if (keyword == "true") {
            return new BoolExpr(true); // 布尔表达式 true
        } else if (keyword == "if") {
            return parse_if(in); // if表达式
        }
        throw std::runtime_error("unknown keyword: " + keyword);
    } else if (c == '(') {
        consume(in, '(');
        Expr* expr = parse_expr(in);
        skip_whitespace(in);
        consume(in, ')', "parentheses mismatch"); // 消费闭合的括号
        return expr;
    } else {
        consume(in, c);
        throw std::runtime_error("bad input");
    }
}

// 解析数字
Expr* parse_num(std::istream& in) {
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
    return new NumExpr((int) num);
}

// 解析let表达式
Expr* parse_let(std::istream& in) {
    std::string errorMsg = "wrong format for let expression";
    VarExpr* var = dynamic_cast<VarExpr*>(parse_var(in));
    skip_whitespace(in);
    consume(in, '=', errorMsg);
    Expr* rhs = parse_expr(in);
    skip_whitespace(in);
    consume(in, "_in", errorMsg);
    Expr* body = parse_expr(in);
    return new LetExpr(var->getVal(), rhs, body);
}

// 解析if表达式
Expr* parse_if(std::istream& in) {
    std::string errorMsg = "wrong format for if expression";
    Expr* test_part = parse_expr(in); // 解析条件部分
    consume(in, "_then", errorMsg); // 消费_then关键字
    Expr* then_part = parse_expr(in); // 解析then分支
    consume(in, "_else", errorMsg); // 消费_else关键字
    Expr* else_part = parse_expr(in); // 解析else分支
    return new IfExpr(test_part, then_part, else_part); // 创建if表达式
}

// 解析变量名
Expr* parse_var(std::istream& in) {
    skip_whitespace(in); // 跳过空白
    std::vector<char> s; // 存放变量名字符
    int c;
    while (isalpha(c = in.peek())) { // 检查是否为字母
        consume(in, c); // 消费字符
        s.push_back(c); // 添加到变量名中
    }
    std::set<char> ops = {'+', '*', '=', ')'}; // 定义操作符集合
    if (c != -1 && !isspace(c) && ops.find(c) == ops.end()) {
        throw std::runtime_error("invalid variable name"); // 如果遇到非法字符，抛出异常
    }
    return new VarExpr(std::string(s.begin(), s.end())); // 创建变量表达式
}

// 解析关键字
std::string parse_keyword(std::istream& in) {
    std::stringstream ss; // 使用字符串流收集字符
    char c;
    while (isalpha(c = in.peek())) { // 检查是否为字母
        consume(in, c); // 消费字符
        ss << c; // 添加到字符串流中
    }
    return ss.str(); // 返回构建的关键字字符串
}

// 消费期望的字符串
void consume(std::istream& in, std::string expect, const std::string& message) {
    for (const char& c : expect) { // 遍历期望的字符串
        consume(in, c, message); // 逐字符消费
    }
}

// 消费单个期望的字符
void consume(std::istream& in, int expect, const std::string& message) {
    int c = in.get(); // 读取一个字符
    if (c != expect) { // 如果不是期望的字符
        throw std::runtime_error(message); // 抛出异常
    }
}

// 跳过输入流中的空白字符
void skip_whitespace(std::istream& in) {
    int c;
    while (isspace(c = in.peek())) { // 检查下一个字符是否为空白
        consume(in, c); // 消费空白字符
    }
}

// 用于测试的函数，通过字符串来解析表达式
Expr* parse_str(std::string s) {
    std::istringstream in(s); // 将字符串转换为输入流
    return parse_expr(in); // 解析表达式
}
