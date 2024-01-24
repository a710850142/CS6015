//
//  main.cpp
//  hw1
//
//  Created by 徐希尧 on 1/10/24.
//

#include "cmdline.h"   // 包含名为 "cmdline.h" 的头文件。Include the header file named "cmdline.h".

int main(int argc, char **argv) {   // 主函数入口，接收命令行参数。Main function entry, receives command line arguments.
    use_arguments(argc, argv);      // 调用一个函数来使用这些命令行参数。Call a function to use these command line arguments.
    return 0;                       // 返回 0，表示程序正常退出。Return 0, indicating normal program termination.
}
