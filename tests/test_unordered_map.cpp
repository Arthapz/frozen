// include catch before import std to avoid STL issues
#include "bench.hpp"
#include "catch.hpp"

#ifdef FROZEN_USE_STD_MODULE
import std;
#else
#include <iostream>
#include <unordered_map>
#include <string>
#include <cctype>
#include <algorithm>
#endif

#ifdef FROZEN_USE_MODULE
import frozen;
#else 
#include <frozen/unordered_map.h>
#include <frozen/string.h>
#endif

#include <frozen/bits/elsa_std.h>

TEST_CASE("singleton frozen unordered map", "[unordered map]") {
  constexpr frozen::unordered_map<int, double, 1> ze_map{{1, 2.}};

  constexpr auto empty = ze_map.empty();
  REQUIRE(!empty);

  constexpr auto size = ze_map.size();
  REQUIRE(size == 1);

  constexpr auto max_size = ze_map.max_size();
  REQUIRE(max_size == 1);

  constexpr auto nocount = ze_map.count(3);
  REQUIRE(nocount == 0);

  constexpr auto count = ze_map.count(1);
  REQUIRE(count == 1);

  constexpr auto find = ze_map.at(1);
  REQUIRE(find == 2.);

  auto notfound = ze_map.find(3);
  REQUIRE(notfound == ze_map.end());

  auto found = ze_map.find(1);
  REQUIRE(found == ze_map.begin());

  auto range = ze_map.equal_range(1);
  REQUIRE(std::get<0>(range) == ze_map.begin());
  REQUIRE(std::get<1>(range) == ze_map.end());

  auto begin = ze_map.begin(), end = ze_map.end();
  REQUIRE(begin != end);

  // auto constexpr key_hash = ze_map.hash_function();
  // auto constexpr key_hashed = key_hash(1);
  // REQUIRE(key_hashed);

  auto constexpr key_eq = ze_map.key_eq();
  auto constexpr value_comparison = key_eq(11, 11);
  REQUIRE(value_comparison);

  auto cbegin = ze_map.cbegin(), cend = ze_map.cend();
  REQUIRE(cbegin < cend);

  std::for_each(ze_map.begin(), ze_map.end(), [](std::tuple<int, double>) {});

  static_assert(std::is_same<typename decltype(ze_map)::key_type, int>::value, "");
  static_assert(std::is_same<typename decltype(ze_map)::mapped_type, double>::value, "");
}

// This is mainly a test that construction does not time out
TEST_CASE("frozen::unordered_map powers of two", "[unordered_map]") {
  constexpr frozen::unordered_map<int, int, 14> frozen_map = {
    {1, 0}, {2, 1}, {4, 2}, {8, 3}, {16, 4}, {32, 5}, {64, 6}, {128, 7},
    {256, 8}, {512, 9}, {1024, 10}, {2048, 11}, {4096, 12}, {8192, 13}
  };

  for (const auto & pair : frozen_map) {
    REQUIRE(pair.first == (1 << pair.second));
  }
}

// This is also mainly a test that construction does not time out
#define M64(X) { X * 64, X }
TEST_CASE("frozen::unordered_map multiples of 64", "[unordered_map]") {
  constexpr frozen::unordered_map<int, int, 57> frozen_map = {
    M64(0), M64(1), M64(2), M64(3), M64(4), M64(5), M64(6), M64(7), M64(8),
    M64(9), M64(10), M64(11), M64(12), M64(13), M64(14), M64(15), M64(16),
    M64(17), M64(18), M64(19), M64(20), M64(21), M64(22), M64(23), M64(24),
    M64(25), M64(26), M64(27), M64(28), M64(29), M64(30), M64(31), M64(32),
    M64(33), M64(34), M64(35), M64(36), M64(37), M64(38), M64(39), M64(40),
    M64(41), M64(42), M64(43), M64(44), M64(45), M64(46), M64(47), M64(48),
    M64(49), M64(50), M64(51), M64(52), M64(53), M64(54), M64(55), M64(56),
  };

# undef M64

  for (const auto & pair : frozen_map) {
    REQUIRE(pair.first == 64 * pair.second);
  }
}

TEST_CASE("frozen::unordered_map <> std::unordered_map", "[unordered_map]") {
#define INIT_SEQ                                                               \
    {19, 12}, {1, 12}, {2, 12}, {4, 12}, {5, 12}, {6, 12}, {7, 12}, {8, 12},   \
        {9, 12}, {10, 12}, {11, 12}, {111, 12}, {1112, 12}, {1115, 12},        \
        {1118, 12}, {1110, 12}, {1977, 12}, {177, 12}, {277, 12}, {477, 12},   \
        {577, 12}, {677, 12}, {777, 12}, {877, 12}, {977, 12}, {1077, 12},     \
        {1177, 12}, {11177, 12}, {111277, 12}, {111577, 12}, {111877, 12},     \
        {111077, 12}, {1999, 12}, {199, 12}, {299, 12}, {499, 12}, {599, 12},  \
        {699, 12}, {799, 12}, {899, 12}, {999, 12}, {1099, 12}, {1199, 12},    \
        {11199, 12}, {111299, 12}, {111599, 12}, {111899, 12}, {111099, 12},   \
        {197799, 12}, {17799, 12}, {27799, 12}, {47799, 12}, {57799, 12},      \
        {67799, 12}, {77799, 12}, {87799, 12}, {97799, 12}, {107799, 12},      \
        {117799, 12}, {1117799, 12}, {11127799, 12}, {11157799, 12},           \
        {11187799, 12}, {11107799, 12}, {1988, 12}, {188, 12}, {288, 12},      \
        {488, 12}, {588, 12}, {688, 12}, {788, 12}, {888, 12}, {988, 12},      \
        {1088, 12}, {1188, 12}, {11188, 12}, {111288, 12}, {111588, 12},       \
        {111888, 12}, {111088, 12}, {197788, 12}, {17788, 12}, {27788, 12},    \
        {47788, 12}, {57788, 12}, {67788, 12}, {77788, 12}, {87788, 12},       \
        {97788, 12}, {107788, 12}, {117788, 12}, {1117788, 12},                \
        {11127788, 12}, {11157788, 12}, {11187788, 12}, {11107788, 12},        \
        {199988, 12}, {19988, 12}, {29988, 12}, {49988, 12}, {59988, 12},      \
        {69988, 12}, {79988, 12}, {89988, 12}, {99988, 12}, {109988, 12},      \
        {119988, 12}, {1119988, 12}, {11129988, 12}, {11159988, 12},           \
        {11189988, 12}, {11109988, 12}, {19779988, 12}, {1779988, 12},         \
        {2779988, 12}, {4779988, 12}, {5779988, 12}, {6779988, 12},            \
        {7779988, 12}, {8779988, 12}, {9779988, 12}, {10779988, 12},           \
        {11779988, 12}, {111779988, 12}, {1112779988, 12}, {1115779988, 12},   \
        {1118779988, 12}, {                                                    \
      1110779988, 13                                                           \
    }

  const std::unordered_map<int, int> std_map = { INIT_SEQ };
  constexpr frozen::unordered_map<int, int, 128> frozen_map = { INIT_SEQ };
  SECTION("checking size and content") {
    REQUIRE(std_map.size() == frozen_map.size());
    for (auto v : std_map)
      REQUIRE(frozen_map.count(std::get<0>(v)));
    for (auto v : frozen_map)
      REQUIRE(std_map.count(std::get<0>(v)));
  }

  static_assert(std::is_same<typename decltype(std_map)::key_type,
                             typename decltype(frozen_map)::key_type>::value, "");
  static_assert(std::is_same<typename decltype(std_map)::mapped_type,
                             typename decltype(frozen_map)::mapped_type>::value, "");
}

TEST_CASE("frozen::unordered_map <> frozen::make_unordered_map", "[unordered_map]") {
  constexpr frozen::unordered_map<int, int, 128> frozen_map = { INIT_SEQ };
  constexpr auto frozen_map2 = frozen::make_unordered_map<int, int>({INIT_SEQ});
  constexpr auto frozen_map3 = frozen::make_unordered_map(std::array<std::pair<int, int>, 128>{{INIT_SEQ}});
  REQUIRE(std::equal(frozen_map2.begin(), frozen_map2.end(), frozen_map3.begin()));

  SECTION("checking size and content") {
    REQUIRE(frozen_map.size() == frozen_map2.size());
    for (auto v : frozen_map2)
      REQUIRE(frozen_map.count(std::get<0>(v)));
    for (auto v : frozen_map)
      REQUIRE(frozen_map2.count(std::get<0>(v)));
  }
}

TEST_CASE("frozen::unordered_map <> std::unordered_map /small", "[unordered_map]") {
#define INIT_SEQ_SMALL \
{"0", 0},{"1", 1},{"2", 2},{"3", 3},{"5", 4},{"8", 5},{"9", 6},{"A", 7},{"W", 8},{"X", 9},{"r", 10},{"y", 11},{"BF", 12},{"AP", 13}

  const std::unordered_map<frozen::string, int> std_map = { INIT_SEQ_SMALL };
  constexpr frozen::unordered_map<frozen::string, int, 14> frozen_map = { INIT_SEQ_SMALL };
  SECTION("checking size and content") {
    REQUIRE(std_map.size() == frozen_map.size());
    for (auto v : std_map)
      REQUIRE(frozen_map.count(std::get<0>(v)));
    for (auto v : frozen_map)
      REQUIRE(std_map.count(std::get<0>(v)));
  }

  static_assert(std::is_same<typename decltype(std_map)::key_type,
                             typename decltype(frozen_map)::key_type>::value, "");
  static_assert(std::is_same<typename decltype(std_map)::mapped_type,
                             typename decltype(frozen_map)::mapped_type>::value, "");
}

TEST_CASE("frozen::unordered_map constexpr", "[unordered_map]") {
  constexpr frozen::unordered_map<unsigned, unsigned, 2> ce = {{3,4}, {11,12}};
  static_assert(ce.begin() +2 == ce.end(), "");
  static_assert(ce.size() == 2, "");
  static_assert(ce.count(3), "");
  static_assert(!ce.count(0), "");
  static_assert(ce.find(0) == ce.end(), "");
  static_assert(ce.contains(3), "");
  static_assert(!ce.contains(0), "");
}

TEST_CASE("access", "[unordered_map]") {
  constexpr frozen::unordered_map<unsigned, unsigned, 2> ce = {{3,4}, {11,12}};
  REQUIRE(4 == ce.at(3));
  REQUIRE_THROWS(ce.at(33));
}

TEST_CASE("Modifiable frozen::unordered_map", "[unordered_map]") {
  frozen::unordered_map<int, int, 3> frozen_map = {{0, 1}, {2, 3}, {4, 5}};

  SECTION("Lookup existing values") {
    REQUIRE(frozen_map.at(0) == 1);
    REQUIRE(frozen_map.find(0)->second == 1);
    REQUIRE(frozen_map.equal_range(0).first->second == 1);

    REQUIRE(frozen_map.at(2) == 3);
    REQUIRE(frozen_map.find(2)->second == 3);
    REQUIRE(frozen_map.equal_range(2).first->second == 3);

    REQUIRE(frozen_map.at(4) == 5);
    REQUIRE(frozen_map.find(4)->second == 5);
    REQUIRE(frozen_map.equal_range(4).first->second == 5);
  }

  SECTION("Lookup failure") {
    REQUIRE(frozen_map.find(5) == frozen_map.end());
    REQUIRE_THROWS_AS(frozen_map.at(5), std::out_of_range);
  }

  SECTION("Modify value") {
    frozen_map.at(0) = -1;
    REQUIRE(frozen_map.at(0) == -1);

    frozen_map.begin()->second = -2;
    REQUIRE(frozen_map.begin()->second == -2);

    (frozen_map.end() - 1)->second = -3;
    REQUIRE((frozen_map.end() - 1)->second == -3);

    frozen_map.equal_range(4).first->second = -5;
    REQUIRE(frozen_map.at(4) == -5);
  }
}

struct eq {
  template<class StrTy>
  constexpr auto operator()(const frozen::string &frozen, const StrTy &str) const {
    return frozen == frozen::string{str.data(), str.size()};
  }
};

TEST_CASE("frozen::unordered_map heterogeneous container", "[unordered_map]") {
  constexpr auto map = frozen::make_unordered_map<frozen::string, int>(
          {{"one", 1}, {"two", 2}, {"three", 3}},
          frozen::elsa<>{}, eq{});

  REQUIRE(map.find(std::string{"two"})->second == 2);
  REQUIRE(map.find(frozen::string{"two"})->second == 2);
}


// Dummy structure to check/showcase transparent comparison
struct case_insensitive {
  const char data[3];
  case_insensitive(const char (&s)[4]) : data{s[0], s[1], s[2]} {}

  friend bool operator==(frozen::string self, case_insensitive other) {
    return std::equal(self.begin(), self.end(), other.data, other.data + 3,
        [](char s, char o) { return std::tolower(s) == std::tolower(o);});
  }
};

template <>
struct frozen::elsa<case_insensitive> {
  constexpr std::size_t operator()(case_insensitive const &value, std::size_t seed) const {
    char tmp[3];
    for(size_t i = 0; i < 3; ++i)
      tmp[i] = tolower(value.data[i]);
    return frozen::elsa<>()(frozen::string(tmp, 3), seed);
  }
};

TEST_CASE("frozen::unordered_map transparent container", "[unordered_map]") {
  constexpr frozen::unordered_map<frozen::string, int, 1, frozen::anna<>, std::equal_to<>> map = {{"one", 1}};

  REQUIRE(map.find(case_insensitive("OnE"))->second == 1);
  REQUIRE(map.count(case_insensitive("One")) == 1);
  REQUIRE(map.count(case_insensitive("TwO")) == 0);
  REQUIRE(map.count(case_insensitive("333")) == 0);
}
