#include "IPv4.hpp"

#include <gtest/gtest.h>

#include <optional>
#include <string_view>

namespace em::ip_log_filter {
namespace {

std::optional<IPv4> Parse(std::string_view ip) { return IPv4::parse(ip); }

TEST(IPv4ParsingValidTest, ParsesCommonAddresses) {
  EXPECT_TRUE(Parse("192.168.1.1").has_value());
  EXPECT_TRUE(Parse("10.0.0.42").has_value());
  EXPECT_TRUE(Parse("172.16.255.254").has_value());
}

TEST(IPv4ParsingValidTest, ParsesBoundaryAddresses) {
  EXPECT_TRUE(Parse("0.0.0.0").has_value());
  EXPECT_TRUE(Parse("255.255.255.255").has_value());
}

TEST(IPv4ParsingInvalidTest, RejectsOctetOverflow) {
  EXPECT_EQ(Parse("256.0.0.1"), std::nullopt);
  EXPECT_EQ(Parse("1.256.0.1"), std::nullopt);
  EXPECT_EQ(Parse("1.0.256.1"), std::nullopt);
  EXPECT_EQ(Parse("1.0.0.256"), std::nullopt);
}

TEST(IPv4ParsingInvalidTest, RejectsInvalidFormat) {
  EXPECT_EQ(Parse("1.2.3"), std::nullopt);
  EXPECT_EQ(Parse("1.2.3.4.5"), std::nullopt);
  EXPECT_EQ(Parse("1..3.4"), std::nullopt);
  EXPECT_EQ(Parse("-1.2.3.4"), std::nullopt);
}

TEST(IPv4ParsingInvalidTest, RejectsGarbage) {
  EXPECT_EQ(Parse("abc.def.ghi.jkl"), std::nullopt);
  EXPECT_EQ(Parse("1.2.3.4abc"), std::nullopt);
  EXPECT_EQ(Parse("hello"), std::nullopt);
}

TEST(IPv4ValueObjectTest, SupportsComparisonOperators) {
  const auto low = Parse("10.0.0.1");
  const auto high = Parse("10.0.0.2");
  const auto same = Parse("10.0.0.1");

  ASSERT_TRUE(low.has_value());
  ASSERT_TRUE(high.has_value());
  ASSERT_TRUE(same.has_value());

  EXPECT_EQ(*low, *same);
  EXPECT_LT(*low, *high);
  EXPECT_GT(*high, *low);
}

TEST(IPv4ValueObjectTest, SupportsBitwiseOperations) {
  const auto ip = Parse("192.168.1.129");
  const auto mask = Parse("255.255.255.0");
  const auto network = Parse("192.168.1.0");
  const auto broadcast = Parse("192.168.1.255");
  const auto hostPart = Parse("0.0.0.129");

  ASSERT_TRUE(ip.has_value());
  ASSERT_TRUE(mask.has_value());
  ASSERT_TRUE(network.has_value());
  ASSERT_TRUE(broadcast.has_value());
  ASSERT_TRUE(hostPart.has_value());

  EXPECT_EQ((*ip & *mask), *network);
  EXPECT_EQ((*ip | ~(*mask)), *broadcast);
  EXPECT_EQ((*ip ^ *network), *hostPart);
}

TEST(IPv4ValueObjectTest, PrefixMaskFactoryCreatesExpectedMasks) {
  const auto mask0 = IPv4::maskFromPrefix(0);
  const auto mask24 = IPv4::maskFromPrefix(24);
  const auto mask32 = IPv4::maskFromPrefix(32);
  const auto expected0 = Parse("0.0.0.0");
  const auto expected24 = Parse("255.255.255.0");
  const auto expected32 = Parse("255.255.255.255");

  ASSERT_TRUE(mask0.has_value());
  ASSERT_TRUE(mask24.has_value());
  ASSERT_TRUE(mask32.has_value());
  ASSERT_TRUE(expected0.has_value());
  ASSERT_TRUE(expected24.has_value());
  ASSERT_TRUE(expected32.has_value());

  EXPECT_EQ(*mask0, *expected0);
  EXPECT_EQ(*mask24, *expected24);
  EXPECT_EQ(*mask32, *expected32);
}

TEST(IPv4ValueObjectTest, PrefixMaskFactoryRejectsInvalidPrefix) {
  EXPECT_EQ(IPv4::maskFromPrefix(-1), std::nullopt);
  EXPECT_EQ(IPv4::maskFromPrefix(33), std::nullopt);
}

}  // namespace
}  // namespace em::ip_log_filter
