#include "val.h"
#include "expr.h"

// 数值类的实现
NumVal::NumVal(int v) {
    val = v; // 存储数值
}

// 将数值转换为表达式
Expr* NumVal::to_expr() {
    return new NumExpr(val); // 创建一个数值表达式
}

// 判断数值是否等于另一个值
bool NumVal::equals(Val* rhs) {
    NumVal* numVal = dynamic_cast<NumVal*>(rhs); // 尝试将rhs转换为NumVal*
    if (numVal == nullptr) {
        return false; // 如果转换失败，则不相等
    }
    return val == numVal->val; // 比较数值是否相等
}

// 与另一个值相加
Val* NumVal::add_to(Val* rhs) {
    NumVal* numVal = dynamic_cast<NumVal*>(rhs); // 尝试将rhs转换为NumVal*
    if (numVal == nullptr) {
        throw std::runtime_error("add of non-number"); // 如果不是数值，抛出异常
    }
    return new NumVal((unsigned) val + (unsigned) numVal->val); // 返回相加后的结果
}

// 与另一个值相乘
Val* NumVal::mult_with(Val* rhs) {
    NumVal* numVal = dynamic_cast<NumVal*>(rhs); // 尝试将rhs转换为NumVal*
    if (numVal == nullptr) {
        throw std::runtime_error("multiply with non-number"); // 如果不是数值，抛出异常
    }
    return new NumVal((unsigned) val * (unsigned) numVal->val); // 返回相乘后的结果
}

// 将数值转换为字符串表示
std::string NumVal::to_string() {
    return std::to_string(val); // 使用标准库函数转换
}

// 判断数值是否为真（抛出异常，因为数值不能直接判断真假）
bool NumVal::is_true() {
    throw std::runtime_error("a number value can not be evaluated to a boolean");
}

// 调用函数（抛出异常，数值不支持调用）
Val* NumVal::call(Val *actual_arg) {
    throw std::runtime_error("no function to call!");
}

// 布尔值类的实现
BoolVal::BoolVal(bool v) {
    val = v; // 存储布尔值
}

// 将布尔值转换为表达式
Expr* BoolVal::to_expr() {
    return new BoolExpr(val); // 创建一个布尔表达式
}

// 判断布尔值是否等于另一个值
bool BoolVal::equals(Val *rhs) {
    BoolVal* other = dynamic_cast<BoolVal*>(rhs); // 尝试将rhs转换为BoolVal*
    if (other == nullptr) {
        return false; // 如果转换失败，则不相等
    }
    return val == other->val; // 比较布尔值是否相等
}

// 布尔值不支持加法，抛出异常
Val* BoolVal::add_to(Val *rhs) {
    throw std::runtime_error("add of non-number");
}

// 布尔值不支持乘法，抛出异常
Val* BoolVal::mult_with(Val *rhs) {
    throw std::runtime_error("multiply with non-number");
}

// 将布尔值转换为字符串表示
std::string BoolVal::to_string() {
    return val ? "_true" : "_false"; // 返回"_true"或"_false"
}

// 判断布尔值是否为真
bool BoolVal::is_true() {
    return val; // 直接返回布尔值
}

// 调用函数（抛出异常，布尔值不支持调用）
Val* BoolVal::call(Val *actual_arg) {
    throw std::runtime_error("no function to call!");
}

// FunVal构造函数
FunVal::FunVal(std::string arg, Expr* expr) {
    formal_arg = arg; // 形式参数名称
    body = expr; // 函数体表达式
}

// 将函数值转换为表达式
Expr* FunVal::to_expr() {
    return new FunExpr(formal_arg, body); // 创建一个表示函数的表达式
}

// 检查当前函数值是否等于另一个值
bool FunVal::equals(Val* rhs) {
    FunVal* other = dynamic_cast<FunVal*>(rhs); // 尝试将rhs转换为FunVal*
    if (other == nullptr) {
        return false; // 如果转换失败，则不相等
    }
    // 比较形式参数和函数体是否相同
    return formal_arg == other->formal_arg && body->equals(other->body);
}

// 函数值不支持加法，抛出异常
Val* FunVal::add_to(Val* rhs) {
    throw std::runtime_error("add of non-number");
}

// 函数值不支持乘法，抛出异常
Val* FunVal::mult_with(Val* rhs) {
    throw std::runtime_error("multiply with non-number");
}

// 将函数值转换为字符串
std::string FunVal::to_string() {
    // 使用表达式的pretty_string方法来生成字符串表示
    return to_expr()->to_pretty_string();
}

// 函数值不能直接评估为布尔值，抛出异常
bool FunVal::is_true() {
    throw std::runtime_error("a function value can not be evaluated to a boolean");
}

// 调用函数值
Val* FunVal::call(Val *actual_arg) {
    // 将实际参数替换进函数体，然后对替换后的表达式进行求值
    return body->subst(formal_arg, actual_arg->to_expr())->interp();
}






