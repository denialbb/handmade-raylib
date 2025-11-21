#include "catch2/catch_amalgamated.hpp"
#include "file_1.hpp"

// NOTE:
// https://github.com/catchorg/Catch2
// https://github.com/catchorg/Catch2/blob/devel/docs/Readme.md#top
TEST_CASE("Testing file 1")
{
    REQUIRE(func_file_1() == 1);        // should pass
}
