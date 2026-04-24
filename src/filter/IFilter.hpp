#pragma once
#include "IPv4.hpp"

namespace em::ip_log_filter {
class IFilter {
 public:
  virtual bool match(const IPv4& ip) const = 0;
  virtual ~IFilter() noexcept = default;
};
}  // namespace em::ip_log_filter
