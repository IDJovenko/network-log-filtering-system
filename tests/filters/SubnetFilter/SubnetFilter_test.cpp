#include "SubnetFilter.hpp"

#include <gtest/gtest.h>

#include <optional>
#include <string_view>

#include "IPv4.hpp"

namespace em::ip_log_filter {
namespace {

std::optional<IPv4> Ip(std::string_view value) { return IPv4::parse(value); }

TEST(SubnetFilterTest, MatchesIpInsideSubnet) {
  const auto network = Ip("192.168.1.0");
  const auto inside = Ip("192.168.1.42");

  ASSERT_TRUE(network.has_value());
  ASSERT_TRUE(inside.has_value());

  const auto filter = SubnetFilter::create(*network, 24);

  ASSERT_TRUE(filter.has_value());
  EXPECT_TRUE(filter->match(*inside));
}

TEST(SubnetFilterTest, RejectsIpOutsideSubnet) {
  const auto network = Ip("192.168.1.0");
  const auto outside = Ip("192.168.2.1");

  ASSERT_TRUE(network.has_value());
  ASSERT_TRUE(outside.has_value());

  const auto filter = SubnetFilter::create(*network, 24);

  ASSERT_TRUE(filter.has_value());
  EXPECT_FALSE(filter->match(*outside));
}

TEST(SubnetFilterTest, MatchesSubnetBoundaryAddresses) {
  const auto network = Ip("10.0.0.0");
  const auto networkAddress = Ip("10.0.0.0");
  const auto broadcastAddress = Ip("10.0.0.255");

  ASSERT_TRUE(network.has_value());
  ASSERT_TRUE(networkAddress.has_value());
  ASSERT_TRUE(broadcastAddress.has_value());

  const auto filter = SubnetFilter::create(*network, 24);

  ASSERT_TRUE(filter.has_value());
  EXPECT_TRUE(filter->match(*networkAddress));
  EXPECT_TRUE(filter->match(*broadcastAddress));
}

}  // namespace
}  // namespace em::ip_log_filter
