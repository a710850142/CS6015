#include "parse.h"
#include <vector>
#include <set>

// Parses an expression from an input stream
Expr* parse_expr(std::istream& in) {
    Expr* lhs = parse_comparg(in); // Parse the left-hand side component of the expression
    skip_whitespace(in); // Skip any whitespace
    int c = in.peek(); // Peek at the next character without consuming it
    if (c == '=') { // Check if it's an equality operator
        consume(in, "=="); // Consume the equality operator
        Expr* rhs = parse_expr(in); // Recursively parse the right-hand side of the equality expression
        return new EqExpr(lhs, rhs); // Return a new equality expression object
    }
    return lhs; // If not an equality expression, return the left-hand side expression
}

// Parses a component of an expression that could be involved in addition
Expr* parse_comparg(std::istream& in) {
    Expr* lhs = parse_addend(in); // Parse the left-hand side operand
    skip_whitespace(in); // Skip any whitespace
    int c = in.peek(); // Peek at the next character without consuming it
    if (c == '+') { // Check if it's an addition operator
        consume(in, '+'); // Consume the addition operator
        Expr* rhs = parse_comparg(in); // Recursively parse the right-hand side operand
        return new AddExpr(lhs, rhs); // Return a new addition expression object
    }
    return lhs; // If not an addition, return the left-hand side expression
}

// Parses an operand of an addition or multiplication
Expr* parse_addend(std::istream& in) {
    skip_whitespace(in); // Skip any whitespace
    Expr* lhs = parse_multicand(in); // Parse the left-hand side operand
    skip_whitespace(in); // Skip any whitespace
    int c = in.peek(); // Peek at the next character without consuming it
    if (c == '*') { // Check if it's a multiplication operator
        consume(in, '*'); // Consume the multiplication operator
        Expr* rhs = parse_addend(in); // Recursively parse the right-hand side operand
        return new MultExpr(lhs, rhs); // Return a new multiplication expression object
    }
    std::set<char> ops = {'+', '*', '=', ')', '_'}; // Set of valid operator characters
    if (c != -1 && ops.find(c) == ops.end()) { // Check if the character is invalid
        throw std::runtime_error("bad input"); // Throw an error for bad input
    }
    return lhs; // If not a multiplication, return the left-hand side expression
}

// Parses a multiplicand, which could be a number, variable, or parenthesized expression
Expr* parse_multicand(std::istream& in) {
    skip_whitespace(in); // Skip any whitespace
    int c = in.peek(); // Peek at the next character without consuming it
    if (c == '-' || isdigit(c)) { // If it's a negative sign or digit
        return parse_num(in); // Parse a number
    } else if (isalpha(c)) { // If it's an alphabetic character
        return parse_var(in); // Parse a variable
    } else if (c == '_') { // If it's an underscore (indicating a keyword)
        consume(in, c); // Consume the underscore
        std::string keyword = parse_keyword(in); // Parse the keyword
        // Parse specific keyword expressions
        if (keyword == "let") {
            return parse_let(in);
        } else if (keyword == "false") {
            return new BoolExpr(false);
        } else if (keyword == "true") {
            return new BoolExpr(true);
        } else if (keyword == "if") {
            return parse_if(in);
        }
        throw std::runtime_error("unknown keyword: " + keyword); // Throw an error for unknown keywords
    } else if (c == '(') { // If it's an opening parenthesis
        consume(in, '('); // Consume the parenthesis
        Expr* expr = parse_expr(in); // Parse the expression within the parentheses
        skip_whitespace(in); // Skip any whitespace
        consume(in, ')', "parentheses mismatch"); // Ensure the closing parenthesis is present
        return expr; // Return the parenthesized expression
    } else {
        consume(in, c); // Consume the character
        throw std::runtime_error("bad input"); // Throw an error for bad input
    }
}

// Parses a numeric value, handling negative numbers as well
Expr* parse_num(std::istream& in) {
    int num = 0;
    bool negative = false;
    if (in.peek() == '-') { // Check for a negative sign
        negative = true;
        consume(in, '-'); // Consume the negative sign
    }
    if (!isdigit(in.peek())) { // Ensure the next character is a digit
        throw std::runtime_error("invalid input");
    }
    int c;
    while (isdigit(c = in.peek())) { // Loop through digits
        consume(in, c); // Consume each digit
        num = num * 10 + c - '0'; // Update the number value
    }
    if (negative) {
        num = -num; // Apply negativity if necessary
    }
    return new NumExpr(num); // Return a new number expression object
}

// Parses a "let" expression, which allows for variable binding
Expr* parse_let(std::istream& in) {
    std::string errorMsg = "wrong format for let expression";

    VarExpr* var = dynamic_cast<VarExpr*>(parse_var(in)); // Parse the variable name
    skip_whitespace(in); // Skip any whitespace
    consume(in, '=', errorMsg); // Consume the equals sign
    Expr* rhs = parse_expr(in); // Parse the right-hand side expression
    skip_whitespace(in); // Skip any whitespace
    consume(in, "_in", errorMsg); // Consume the "_in" keyword
    Expr* body = parse_expr(in); // Parse the body of the let expression
    return new LetExpr(var->getVal(), rhs, body); // Return a new let expression object
}

// Parses an "if" expression, including its test, then, and else parts
Expr* parse_if(std::istream& in) {
    std::string errorMsg = "wrong format for if expression";
    Expr* test_part = parse_expr(in); // Parse the test expression
    consume(in, "_then", errorMsg); // Consume the "_then" keyword
    Expr* then_part = parse_expr(in); // Parse the then expression
    consume(in, "_else", errorMsg); // Consume the "_else" keyword
    Expr* else_part = parse_expr(in); // Parse the else expression
    return new IfExpr(test_part, then_part, else_part); // Return a new if expression object
}

// Parses a variable name
Expr* parse_var(std::istream& in) {
    skip_whitespace(in); // Skip any whitespace
    std::vector<char> s; // Vector to hold the variable name characters
    int c;
    while (isalpha(c = in.peek())) { // Loop through alphabetic characters
        consume(in, c); // Consume each character
        s.push_back(c); // Add each character to the vector
    }
    std::set<char> ops = {'+', '*', '=', ')'}; // Set of valid following characters
    if (c != -1 && !isspace(c) && ops.find(c) == ops.end()) { // Check for invalid characters
        throw std::runtime_error("invalid variable name");
    }
    return new VarExpr(std::string(s.begin(), s.end())); // Return a new variable expression object
}

// Parses a keyword following an underscore
std::string parse_keyword(std::istream& in) {
    std::stringstream ss; // Stringstream to build the keyword
    char c;
    while (isalpha(c = in.peek())) { // Loop through alphabetic characters
        consume(in, c); // Consume each character
        ss << c; // Add each character to the stringstream
    }
    return ss.str(); // Return the built keyword as a string
}

// Consumes a specified string from the input, throwing an error if not matched
void consume(std::istream& in, std::string expect, const std::string& message) {
    for (const char& c : expect) {
        consume(in, c, message); // Consume each character of the expected string
    }
}

// Consumes a specified character from the input, throwing an error if not matched
void consume(std::istream& in, int expect, const std::string& message) {
    int c = in.get(); // Get the next character
    if (c != expect) { // If it does not match the expected character
        throw std::runtime_error(message); // Throw the specified error message
    }
}

// Skips whitespace characters in the input
void skip_whitespace(std::istream& in) {
    int c;
    while (isspace(c = in.peek())) { // Loop through whitespace characters
        consume(in, c); // Consume each whitespace character
    }
}

// Parses an expression from a string (useful for testing)
Expr* parse_str(std::string s) {
    std::istringstream in(s); // Create an input stream from the string
    return parse_expr(in); // Parse the expression from the input stream
}
