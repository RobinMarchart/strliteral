#define CATCH_CONFIG_MAIN
#include <big_random.test.hpp>
#include <catch2/catch.hpp>
#include <empty.test.hpp>
#include <istream>
#include <random.test.hpp>
#include <string>

TEST_CASE("string literal embed test", "[strliteral]") {
  SECTION("empty") {
    REQUIRE(1 == empty_test_size);
    REQUIRE(std::string("") ==
            std::string(reinterpret_cast<const char *>(empty_test)));
  }
  SECTION("random") {
    REQUIRE(65 == random_test_size);
    REQUIRE(
        std::string(reinterpret_cast<const char *>(random_test)) ==
        std::string(
            "\x8b\x71\xb5\xea\x84\xf2\x7a\x8a\x6f\xc1\xcd\x67\x18\xfa\xd6\x0f"
            "\x0d\xa7\x6a\x6f\xe4\x33\xa9\xfb\x0f\x45\xc2\x09\x5c\x4f\x47\x8f"
            "\x6a\xcd\x86\x74\xcc\xac\x6c\xc6\x01\xab\xd3\x74\xf5\x86\xfe\xb6"
            "\xdd\xbe\x3b\x72\x58\x79\xc9\x8e\xb9\x19\x14\x56\x5b\xa6\xda"
            "\x6a"));
  }
}
