#include "cmdline.h"   // 包含自定义的 "cmdline.h" 头文件。Include the custom header file "cmdline.h".
#include <iostream>    // 包含标准输入输出流库。Include the standard input/output stream library.
#include <cstdlib>     // 包含 C 标准库，用于通用函数。Include the C standard library for general purpose functions.
#include <string>      // 包含字符串库。Include the string library.

// 定义 use_arguments 函数，用于处理命令行参数。
// Define the use_arguments function to handle command line arguments.
void use_arguments(int argc, char* argv[]) {
    bool test_seen = false;  // 声明并初始化一个布尔变量，用于跟踪是否已经看到 --test 参数。
    // Declare and initialize a boolean variable to track if --test argument has been seen.

    // 遍历命令行参数。
    // Loop through command line arguments.
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];  // 将当前参数转换为字符串。
        // Convert the current argument to a string.

        // 检查当前参数是否为 "--help"。
        // Check if the current argument is "--help".
        if (arg == "--help") {
            // 如果是，打印使用说明并退出程序。
            // If it is, print usage instructions and exit the program.
            std::cout << "Usage: ./msdscript [--help] [--test]\n";
            exit(0);
        } else if (arg == "--test") {
            // 检查当前参数是否为 "--test"。
            // Check if the current argument is "--test".
            if (!test_seen) {
                // 如果是第一次看到 "--test"，打印测试通过消息。
                // If it's the first time seeing "--test", print a message that tests passed.
                std::cout << "Tests passed\n";
                test_seen = true;  // 标记已经看到了 "--test"。
                // Mark that "--test" has been seen.
            } else {
                // 如果 "--test" 被重复看到，打印错误信息并退出程序。
                // If "--test" is seen more than once, print an error message and exit the program.
                std::cerr << "Error: --test argument seen multiple times.\n";
                exit(1);
            }
        } else {
            // 如果参数既不是 "--help" 也不是 "--test"，打印错误信息并退出程序。
            // If the argument is neither "--help" nor "--test", print an error message and exit the program.
            std::cerr << "Error: Invalid argument '" << arg << "'.\n";
            exit(1);
        }
    }
}
