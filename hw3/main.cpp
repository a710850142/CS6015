#include "cmdline.h"
#define CATCH_CONFIG_RUNNER
#include "catch.h"
#include <cstring>

int main(int argc, char **argv) {

    use_arguments(argc, argv);

    if (Catch::Session().run() != 0) {
        exit(1);
    };


    return 0;
}