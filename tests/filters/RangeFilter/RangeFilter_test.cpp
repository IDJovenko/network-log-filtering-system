#include "RangeFilter.hpp"

#include <gtest/gtest.h>

#include <optional>
#include <string_view>

#include "IPv4.hpp"

namespace em::ip_log_filter {
namespace {

std::optional<IPv4> Ip(std::string_view value) { return IPv4::parse(value); }

TEST(RangeFilterTest, MatchesIpInsideRange) {
  const auto start = Ip("192.168.1.10");
  const auto end = Ip("192.168.1.20");

  ASSERT_TRUE(start.has_value());
  ASSERT_TRUE(end.has_value());

  const auto filter = RangeFilter::create(*start, *end);
  const auto inside = Ip("192.168.1.15");

  ASSERT_TRUE(filter.has_value());
  ASSERT_TRUE(inside.has_value());
  EXPECT_TRUE(filter->match(*inside));
}

TEST(RangeFilterTest, MatchesRangeBoundariesInclusively) {
  const auto start = Ip("10.0.0.1");
  const auto end = Ip("10.0.0.10");

  ASSERT_TRUE(start.has_value());
  ASSERT_TRUE(end.has_value());

  const auto filter = RangeFilter::create(*start, *end);

  ASSERT_TRUE(filter.has_value());
  EXPECT_TRUE(filter->match(*start));
  EXPECT_TRUE(filter->match(*end));
}

TEST(RangeFilterTest, RejectsIpOutsideRange) {
  const auto start = Ip("172.16.0.10");
  const auto end = Ip("172.16.0.20");
  const auto below = Ip("172.16.0.9");
  const auto above = Ip("172.16.0.21");

  ASSERT_TRUE(start.has_value());
  ASSERT_TRUE(end.has_value());
  ASSERT_TRUE(below.has_value());
  ASSERT_TRUE(above.has_value());

  const auto filter = RangeFilter::create(*start, *end);

  ASSERT_TRUE(filter.has_value());
  EXPECT_FALSE(filter->match(*below));
  EXPECT_FALSE(filter->match(*above));
}

}  // namespace
}  // namespace em::ip_log_filter
