/**
* \mainpage MSDScript
* \author Xiyao Xu
* \author Second Author (if applicable)
* \date 02-06-2024
*/
#include <iostream>
#include <stdexcept>
#include "cmdline.h"
#include "expr.h"
#define CATCH_CONFIG_RUNNER
#include "catch.h"
#include <sstream>
#include "parse.h"
#include "val.h"

int main(int argc, char **argv) {
    try {
        run_mode_t mode = use_arguments(argc, argv);

        switch (mode) {
            case do_test:
                // 运行测试
                if (Catch::Session().run() != 0) {
                    return 1;
                }
                break;
            case do_interp:
                // 解释执行
            {
                std::string input;
                std::getline(std::cin, input); // 从标准输入读取表达式
                try {
                    PTR(Expr) expr = parse_str(input); // 解析表达式
                    std::string interp_str = expr->interp()->to_string();//让interp打印结果而不是地址
                    std::cout << interp_str << std::endl; // 执行并打印结果
                } catch (const std::exception& e) {
                    std::cerr << "Error: " << e.what() << std::endl;
                    return 1;
                }
            }
                break;
            case do_print:
                // 打印表达式
            {
                std::string input;
                std::getline(std::cin, input);
                PTR(Expr) expr = parse_str(input); // 解析表达式
                expr->print(std::cout); // 打印表达式
                std::cout << std::endl;
            }
                break;
            case do_pretty_print:
                // 美化打印表达式
            {
                std::string input;
                std::getline(std::cin, input);
                PTR(Expr) expr = parse_str(input); // 解析表达式
                expr->pretty_print(std::cout); // 美化打印表达式
                std::cout << std::endl;
            }
                break;
            default:
                // 如果没有指定模式，可能想打印帮助信息或直接退出
                std::cerr << "Error: Invalid mode or no mode specified.\n";
                return 1;
        }

        return 0;
    } catch (const std::runtime_error& exn) {
        std::cerr << "Unhandled exception: " << exn.what() << "\n";
        return 1;
    } catch (...) {
        std::cerr << "An unknown error occurred.\n";
        return 1;
    }
}
