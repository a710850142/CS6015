#include "parse.h"
#include <sstream>
#include <cctype>
#include <stdexcept>

// Helper function to consume whitespace
void skip_whitespace(std::istream &in) {
    while (std::isspace(in.peek())) in.get();
}

Expr* parse_str(const std::string& str) {
    std::istringstream in(str);  // 将字符串转换为输入流
    return parse(in);  // 使用 parse 函数解析输入流中的表达式
}

// Parse a number
Expr* parse_num(std::istream &in) {
    int num;
    in >> num;
    return new Num(num);
}

// Parse a variable name
Expr* parse_var(std::istream &in) {
    std::string varName;
    while (std::isalpha(in.peek())) {
        varName += in.get();
    }
    return new VarExpr(varName);
}

// Forward declaration to allow recursive calls
Expr* parse_expr(std::istream &in);

// Parse a parenthesized expression
Expr* parse_paren(std::istream &in) {
    in.get(); // Consume '('
    Expr* expr = parse_expr(in);
    skip_whitespace(in);
    if (in.get() != ')') throw std::runtime_error("Expected ')'");
    return expr;
}

// Parse let expressions
Expr* parse_let(std::istream &in) {
    std::string keyword;
    in >> keyword; // Should be "_let"
    std::string var;
    in >> var;
    skip_whitespace(in);
    if (in.get() != '=') throw std::runtime_error("Expected '=' after variable name in let expression");
    Expr* rhs = parse_expr(in);
    skip_whitespace(in);
    std::string inKeyword;
    in >> inKeyword; // Should be "_in"
    Expr* body = parse_expr(in);
    return new LetExpr(var, rhs, body);
}

Expr* parse_expr(std::istream &in) {
    skip_whitespace(in);
    char nextChar = in.peek();
    if (std::isdigit(nextChar)) {
        return parse_num(in);
    } else if (std::isalpha(nextChar)) {
        // Peek next to distinguish between variable and let
        std::streampos startPos = in.tellg();
        std::string potentialKeyword;
        in >> potentialKeyword;
        if (potentialKeyword == "_let") {
            in.seekg(startPos);
            return parse_let(in);
        } else {
            in.seekg(startPos);
            return parse_var(in);
        }
    } else if (nextChar == '(') {
        return parse_paren(in);
    } else {
        throw std::runtime_error("Unexpected character in input");
    }
}

// Entry point for parsing
Expr* parse(std::istream &in) {
    Expr* expr = parse_expr(in);
    skip_whitespace(in);
    if (in.peek() != EOF) throw std::runtime_error("Extra input after expression");
    return expr;
}
