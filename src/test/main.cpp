#include "main.h"

#include <base/argparser.h>

#include <cassert>
#include <stdexcept>
#include <variant>

#include "gtest/gtest.h"

using base::argparser;

argparse_result CMD_OPTIONS;

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);

    argparser parser{"test_chess"};
    parser.add_flag("debug").short_option('d');

    CMD_OPTIONS = parser.parse(argc, argv);

    return RUN_ALL_TESTS();
}
