#ifndef CMDLINE_H        // 如果宏 CMDLINE_H 尚未定义，则执行下面的代码。
// If the macro CMDLINE_H is not defined yet, proceed with the following code.
#define CMDLINE_H        // 定义宏 CMDLINE_H，以防止此头文件被多次包含。
// Define the macro CMDLINE_H to prevent this header file from being included multiple times.

// 声明 use_arguments 函数，它接受整数 argc 和字符指针数组 argv 作为参数。
// Declare the use_arguments function, which takes an integer argc and an array of character pointers argv as parameters.
void use_arguments(int argc, char* argv[]);

#endif                   // 结束预处理器指令，匹配 #ifndef 的开始。
// End of preprocessor directive, matching the start of #ifndef.

