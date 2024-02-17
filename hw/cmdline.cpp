#include "cmdline.h"
#include <iostream>
#include <cstdlib>
#include <string>

run_mode_t use_arguments(int argc, char* argv[]) {
    run_mode_t mode = do_nothing;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "--help") {
            std::cout << "Usage: ./msdscript [--help] [--test] [--interp] [--print] [--pretty-print]\n";
            exit(0);
        } else if (arg == "--test") {
            if (mode == do_nothing) {
                mode = do_test;
            } else {
                std::cerr << "Error: Multiple modes specified.\n";
                exit(1);
            }
        } else if (arg == "--interp") {
            if (mode == do_nothing) {
                mode = do_interp;
            } else {
                std::cerr << "Error: Multiple modes specified.\n";
                exit(1);
            }
        } else if (arg == "--print") {
            if (mode == do_nothing) {
                mode = do_print;
            } else {
                std::cerr << "Error: Multiple modes specified.\n";
                exit(1);
            }
        } else if (arg == "--pretty-print") {
            if (mode == do_nothing) {
                mode = do_pretty_print;
            } else {
                std::cerr << "Error: Multiple modes specified.\n";
                exit(1);
            }
        } else {
            std::cerr << "Error: Invalid argument '" << arg << "'.\n";
            exit(1);
        }
    }

    return mode;
}
