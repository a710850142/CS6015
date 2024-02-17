/**
* \mainpage MSDScript
* \author Xiyao Xu
* \author Second Author (if applicable)
* \date 02-06-2024
*/
#include "cmdline.h"
#include "expr.h"
#define CATCH_CONFIG_RUNNER
#include "catch.h"
#include <iostream>
#include <sstream>
#include "parse.h"

int main(int argc, char **argv) {
    run_mode_t mode = use_arguments(argc, argv);

    switch (mode) {
        case do_test:
            // 运行测试
            if (Catch::Session().run() != 0) {
                exit(1);
            };
            break;
        case do_interp:
            // 解释执行
        {
            std::string input;
            std::getline(std::cin, input); // 从标准输入读取表达式
            try {
                Expr* expr = parse_str(input); // 解析表达式
                std::cout << expr->interp() << std::endl; // 执行并打印结果
                delete expr;
            } catch (const std::exception& e) {
                std::cerr << "Error: " << e.what() << std::endl;
                exit(1);
            }
        }
            break;
        case do_print:
            // 打印表达式
        {
            std::string input;
            std::getline(std::cin, input);
            Expr* expr = parse_str(input); // 解析表达式
            expr->print(std::cout); // 打印表达式
            std::cout << std::endl;
            delete expr;
        }
            break;
        case do_pretty_print:
            // 美化打印表达式
        {
            std::string input;
            std::getline(std::cin, input);
            Expr* expr = parse_str(input); // 解析表达式
            expr->pretty_print(std::cout); // 美化打印表达式
            std::cout << std::endl;
            delete expr;
        }
            break;
        default:
            // 如果没有指定模式，可能想打印帮助信息或直接退出
            break;
    }

    return 0;
}
