#pragma once
#include "IFilter.hpp"

namespace em::ip_log_filter {
class SubnetFilter : public IFilter {
 public:
  static std::optional<SubnetFilter> create(std::string_view value);
  static std::optional<SubnetFilter> create(IPv4 network, int prefixLen);
  bool match(const IPv4& ip) const override;

 private:
  SubnetFilter(IPv4 mask, IPv4 network) : mask(mask), network(network) {};

  IPv4 mask;
  IPv4 network;
};
}  // namespace em::ip_log_filter
