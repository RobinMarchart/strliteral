#define CATCH_CONFIG_MAIN
#include <big_random.test.hpp>
#include <catch2/catch.hpp>
#include <empty.test.hpp>
#include <random.test.hpp>
#include <string>

TEST_CASE("string literal embed test", "[strliteral]") {
  SECTION("empty") {
    REQUIRE(1 == empty_test_size);
    REQUIRE(std::string("") ==
            std::string(reinterpret_cast<const char *>(empty_test)));
  }
}
