#pragma once
#include "IFilter.hpp"

namespace em::ip_log_filter {
class RangeFilter : public IFilter {
 public:
  static std::optional<RangeFilter> create(std::string_view value);
  static std::optional<RangeFilter> create(IPv4 start, IPv4 end);
  bool match(const IPv4& ip) const override;

 private:
  RangeFilter(IPv4 start, IPv4 end) : start(start), end(end) {};

  IPv4 start;
  IPv4 end;
};
}  // namespace em::ip_log_filter
