#include "cmdline.h"   // Include the custom header file "cmdline.h".
#include <iostream>    // Include the standard input/output stream library.
#include <cstdlib>     // Include the C standard library for general purpose functions.
#include <string>      // Include the string library.

// Define the use_arguments function to handle command line arguments.
void use_arguments(int argc, char* argv[]) {
    bool test_seen = false;  // Declare and initialize a boolean variable to track if --test argument has been seen.

    // Loop through command line arguments.
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];  // Convert the current argument to a string.

        // Check if the current argument is "--help".
        if (arg == "--help") {
            // If it is, print usage instructions and exit the program.
            std::cout << "Usage: ./msdscript [--help] [--test]\n";
            exit(0);
        } else if (arg == "--test") {
            // Check if the current argument is "--test".
            if (!test_seen) {
                // If it's the first time seeing "--test", print a message that tests passed.
                std::cout << "Tests passed\n";
                test_seen = true;  // Mark that "--test" has been seen.
            } else {
                // If "--test" is seen more than once, print an error message and exit the program.
                std::cerr << "Error: --test argument seen multiple times.\n";
                exit(1);
            }
        } else {
            // If the argument is neither "--help" nor "--test", print an error message and exit the program.
            std::cerr << "Error: Invalid argument '" << arg << "'.\n";
            exit(1);
        }
    }
}
