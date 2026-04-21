#include "AndFilter.hpp"

#include <gtest/gtest.h>

#include <memory>
#include <optional>
#include <string_view>

#include "IPv4.hpp"
#include "RangeFilter.hpp"
#include "SubnetFilter.hpp"

namespace em::ip_log_filter {
namespace {

std::optional<IPv4> Ip(std::string_view value) { return IPv4::parse(value); }

// Helper: wrap an already-constructed filter into unique_ptr<IFilter>
template <typename F>
std::unique_ptr<IFilter> ToFilter(F filter) {
  return std::make_unique<F>(std::move(filter));
}

TEST(AndFilterTest, EmptyFilterSetMatchesAnyIp) {
  const auto ip = Ip("1.2.3.4");
  ASSERT_TRUE(ip.has_value());

  AndFilter filter;
  EXPECT_TRUE(filter.match(*ip));
}

TEST(AndFilterTest, AllFiltersMatchReturnTrue) {
  // both ranges overlap: the same IP satisfies both
  const auto start1 = Ip("10.0.0.1");
  const auto end1 = Ip("10.0.0.20");
  const auto start2 = Ip("10.0.0.5");
  const auto end2 = Ip("10.0.0.25");
  const auto inside = Ip("10.0.0.10");

  ASSERT_TRUE(start1 && end1 && start2 && end2 && inside);

  const auto r1 = RangeFilter::create(*start1, *end1);
  const auto r2 = RangeFilter::create(*start2, *end2);

  ASSERT_TRUE(r1.has_value());
  ASSERT_TRUE(r2.has_value());

  AndFilter andFilter;
  andFilter.addFilter(ToFilter(*r1)).addFilter(ToFilter(*r2));

  EXPECT_TRUE(andFilter.match(*inside));
}

TEST(AndFilterTest, OneFilterMismatchReturnsFalse) {
  // first range contains the IP, second does not
  const auto start1 = Ip("10.0.0.1");
  const auto end1 = Ip("10.0.0.20");
  const auto start2 = Ip("10.0.0.50");
  const auto end2 = Ip("10.0.0.100");
  const auto ip = Ip("10.0.0.10");

  ASSERT_TRUE(start1 && end1 && start2 && end2 && ip);

  const auto r1 = RangeFilter::create(*start1, *end1);
  const auto r2 = RangeFilter::create(*start2, *end2);

  ASSERT_TRUE(r1.has_value());
  ASSERT_TRUE(r2.has_value());

  AndFilter andFilter;
  andFilter.addFilter(ToFilter(*r1)).addFilter(ToFilter(*r2));

  EXPECT_FALSE(andFilter.match(*ip));
}

TEST(AndFilterTest, CombinesDifferentFilterTypes) {
  // SubnetFilter: 192.168.1.0/24
  // RangeFilter:  192.168.1.100 - 192.168.1.200
  const auto network = Ip("192.168.1.0");
  const auto rangeStart = Ip("192.168.1.100");
  const auto rangeEnd = Ip("192.168.1.200");
  const auto insideBoth = Ip("192.168.1.150");
  const auto inSubnetOnlyLow = Ip("192.168.1.50");
  const auto inSubnetOnlyHigh = Ip("192.168.1.250");

  ASSERT_TRUE(network && rangeStart && rangeEnd && insideBoth &&
              inSubnetOnlyLow && inSubnetOnlyHigh);

  const auto subnet = SubnetFilter::create(*network, 24);
  const auto range = RangeFilter::create(*rangeStart, *rangeEnd);

  ASSERT_TRUE(subnet.has_value());
  ASSERT_TRUE(range.has_value());

  AndFilter andFilter;
  andFilter.addFilter(ToFilter(*subnet)).addFilter(ToFilter(*range));

  EXPECT_TRUE(andFilter.match(*insideBoth));
  EXPECT_FALSE(andFilter.match(*inSubnetOnlyLow));
  EXPECT_FALSE(andFilter.match(*inSubnetOnlyHigh));
}

TEST(AndFilterTest, TwentyDifferentFiltersAllMatch) {
  // Mix of SubnetFilter and RangeFilter filters, all matching 192.168.1.100
  const auto targetIp = Ip("192.168.1.100");
  ASSERT_TRUE(targetIp.has_value());

  AndFilter andFilter;

  // Add 10 subnet filters that include 192.168.1.100
  std::vector<std::pair<std::string, int>> subnets = {
      {"192.168.1.0", 24},    // 192.168.1.0-192.168.1.255
      {"192.168.0.0", 23},    // 192.168.0.0-192.168.1.255
      {"192.168.1.64", 26},   // 192.168.1.64-192.168.1.127
      {"192.168.1.96", 27},   // 192.168.1.96-192.168.1.127
      {"192.168.1.96", 28},   // 192.168.1.96-192.168.1.111
      {"192.168.1.100", 32},  // 192.168.1.100 only
      {"192.168.0.0", 22},    // 192.168.0.0-192.168.3.255
      {"192.168.1.0", 25},    // 192.168.1.0-192.168.1.127
      {"192.168.1.96", 29},   // 192.168.1.96-192.168.1.103
      {"192.168.1.100", 31}   // 192.168.1.100-192.168.1.101
  };

  for (const auto& [networkStr, cidr] : subnets) {
    const auto network = Ip(networkStr);
    ASSERT_TRUE(network.has_value())
        << "Failed to parse network: " << networkStr;

    const auto subnet = SubnetFilter::create(*network, cidr);
    ASSERT_TRUE(subnet.has_value())
        << "Failed to create SubnetFilter for " << networkStr << "/" << cidr;

    andFilter.addFilter(ToFilter(*subnet));
  }

  // Add 10 range filters that include 192.168.1.100
  for (int i = 0; i < 10; ++i) {
    const int startOctet = 50 + i;
    const int endOctet = 150 - i;

    std::string startIpStr = "192.168.1." + std::to_string(startOctet);
    std::string endIpStr = "192.168.1." + std::to_string(endOctet);

    const auto start = Ip(startIpStr);
    const auto end = Ip(endIpStr);

    ASSERT_TRUE(start.has_value());
    ASSERT_TRUE(end.has_value());

    const auto range = RangeFilter::create(*start, *end);
    ASSERT_TRUE(range.has_value())
        << "Failed to create RangeFilter at iteration " << i;

    andFilter.addFilter(ToFilter(*range));
  }

  // All 20 mixed rules should match 192.168.1.100
  EXPECT_TRUE(andFilter.match(*targetIp));
}

TEST(AndFilterTest, TwentyRangeFiltersOneMismatch) {
  // Create 19 filters that match 10.0.0.100, and 1 filter that doesn't
  const auto targetIp = Ip("10.0.0.100");
  ASSERT_TRUE(targetIp.has_value());

  AndFilter andFilter;

  // Add 19 matching filters
  for (int i = 0; i < 19; ++i) {
    const int startOctet = 50 + i;
    const int endOctet = 150 - i;

    std::string startIpStr = "10.0.0." + std::to_string(startOctet);
    std::string endIpStr = "10.0.0." + std::to_string(endOctet);

    const auto start = Ip(startIpStr);
    const auto end = Ip(endIpStr);

    ASSERT_TRUE(start.has_value());
    ASSERT_TRUE(end.has_value());

    const auto range = RangeFilter::create(*start, *end);
    ASSERT_TRUE(range.has_value());

    andFilter.addFilter(ToFilter(*range));
  }

  // Add 1 mismatching filter (range that doesn't include 10.0.0.100)
  const auto mismatchStart = Ip("10.0.0.200");
  const auto mismatchEnd = Ip("10.0.0.250");
  ASSERT_TRUE(mismatchStart.has_value());
  ASSERT_TRUE(mismatchEnd.has_value());

  const auto mismatchRange = RangeFilter::create(*mismatchStart, *mismatchEnd);
  ASSERT_TRUE(mismatchRange.has_value());

  andFilter.addFilter(ToFilter(*mismatchRange));

  // Should return false because one filter doesn't match
  EXPECT_FALSE(andFilter.match(*targetIp));
}

}  // namespace
}  // namespace em::ip_log_filter
