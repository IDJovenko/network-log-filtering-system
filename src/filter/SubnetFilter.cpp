#include "SubnetFilter.hpp"

#include <charconv>

namespace em::ip_log_filter {
static std::optional<int> getPrefixLenFrom(std::string_view prefixStr) {
  int prefixLen = 0;
  auto errorc = std::from_chars(prefixStr.data(),
                                prefixStr.data() + prefixStr.size(), prefixLen)
                    .ec;
  if (errorc != std::errc() || prefixLen < 0 || prefixLen > 32) {
    return std::nullopt;
  }
  return prefixLen;
}

std::optional<SubnetFilter> SubnetFilter::create(std::string_view value) {
  auto slashPosition = value.find('/');
  if (slashPosition == std::string_view::npos) {
    return std::nullopt;
  }

  auto network = IPv4::parse(value.substr(0, slashPosition));
  if (!network.has_value()) {
    return std::nullopt;
  }

  auto prefixLen = getPrefixLenFrom(value.substr(slashPosition + 1));
  if (!prefixLen.has_value()) {
    return std::nullopt;
  }

  return create(*network, *prefixLen);
}

std::optional<SubnetFilter> SubnetFilter::create(IPv4 network, int prefixLen) {
  auto mask = IPv4::maskFromPrefix(prefixLen);
  if (!mask) return std::nullopt;

  IPv4 subnet = network & *mask;
  return SubnetFilter(*mask, subnet);
}

bool SubnetFilter::match(const IPv4& ip) const {
  return (ip & mask) == network;
}
}  // namespace em::ip_log_filter
