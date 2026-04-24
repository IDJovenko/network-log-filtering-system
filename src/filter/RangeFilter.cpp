#include "RangeFilter.hpp"

namespace em::ip_log_filter {
std::optional<RangeFilter> RangeFilter::create(std::string_view value) {
  auto hyphenPosition = value.find('-');
  if (hyphenPosition == std::string_view::npos) {
    return std::nullopt;
  }

  auto start = IPv4::parse(value.substr(0, hyphenPosition));
  if (!start.has_value()) {
    return std::nullopt;
  }
  auto end = IPv4::parse(value.substr(hyphenPosition + 1));
  if (!end.has_value()) {
    return std::nullopt;
  }

  return create(*start, *end);
}

std::optional<RangeFilter> RangeFilter::create(IPv4 start, IPv4 end) {
  if (start > end) {
    return std::nullopt;
  }
  return RangeFilter(start, end);
}

bool RangeFilter::match(const IPv4& ip) const {
  return ip >= start && ip <= end;
}
}  // namespace em::ip_log_filter
