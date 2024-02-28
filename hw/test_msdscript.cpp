#include "exec.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>

// 生成一个介于 min 和 max 之间的随机整数
int random_int(int min, int max) {
    return min + rand() % (max - min + 1);
}

// 随机生成一个变量名
std::string generate_random_variable() {
    std::string var = "x";
    var += std::to_string(random_int(0, 9)); // 为了简化，使用 x 后跟一个数字作为变量名
    return var;
}

// 随机生成一个 Num 表达式
std::string generate_random_num() {
    int num = random_int(0, 100); // 生成 0 到 100 之间的随机数
    return std::to_string(num);
}

// 递归生成随机表达式
std::string generate_random_expression(int depth = 0) {
    // 基准情况：减少递归深度，避免生成太复杂的表达式
    if (depth > 2) {
        return generate_random_num();
    }

    int expr_type = random_int(0, 2); // 随机选择表达式类型：0 for Num, 1 for Mult, 2 for Let
    switch (expr_type) {
        case 0: // Num
            return generate_random_num();
        case 1: { // Mult
            std::string left = generate_random_expression(depth + 1);
            std::string right = generate_random_expression(depth + 1);
            return "(" + left + " * " + right + ")";
        }
        case 2: { // Let
            std::string var = generate_random_variable();
            std::string binding_expr = generate_random_expression(depth + 1);
            std::string body_expr = generate_random_expression(depth + 1);
            return "(let " + var + "=" + binding_expr + " _in " + body_expr + ")";
        }
        default:
            return generate_random_num(); // 默认返回 Num，理论上不应该达到这里
    }
}

// 执行 msdscript 程序
ExecResult execute_msdscript(const std::string& msdscript_path, const std::string& mode, const std::string& input) {
    std::vector<std::string> command = {msdscript_path, mode};
    std::vector<const char*> argv;
    for (const auto& arg : command) argv.push_back(arg.c_str());
    argv.push_back(nullptr);

    return exec_program(argv.size() - 1, argv.data(), input);
}

// 主函数
int main(int argc, char* argv[]) {
    srand(time(nullptr)); // 初始化随机种子

    if (argc < 3 || argc > 4) {
        std::cerr << "Usage: " << argv[0] << " <msdscript1> [--print|--pretty-print|--interp] [msdscript2]" << std::endl;
        return 1;
    } //./test_msdscript msdscript --interp msdscript1

    std::string msdscript1_path = argv[1];
    std::string mode = argv[2]; // 获取模式参数
    std::string msdscript2_path = argc == 4 ? argv[3] : "";

    // 验证模式参数是否有效
    if (mode != "--print" && mode != "--pretty-print" && mode != "--interp") {
        std::cerr << "Invalid mode. Please use --print, --pretty-print, or --interp." << std::endl;
        return 1;
    }

    // 随机生成表达式
    std::string expression = generate_random_expression();

    // 测试单一实现
    if (msdscript2_path.empty()) {
        auto result = execute_msdscript(msdscript1_path, mode, expression);
        std::cout << "Expression: " << expression << "\n";
        std::cout << "Output: " << result.out << "\n";
        std::cout << "Error: " << result.err << "\n";
        std::cout << "Exit code: " << result.exit_code << "\n";
    }
        // 比较两个实现
    else {
        auto result1 = execute_msdscript(msdscript1_path, mode, expression);
        auto result2 = execute_msdscript(msdscript2_path, mode, expression);

        if (result1.out != result2.out || result1.err != result2.err) {
            std::cerr << "Mismatch found.\n";
            std::cerr << "Expression: " << expression << "\n";
            std::cerr << "Implementation 1 output: " << result1.out << ", error: " << result1.err << "\n";
            std::cerr << "Implementation 2 output: " << result2.out << ", error: " << result2.err << "\n";
            return 1;
        }
    }

    return 0;
}




