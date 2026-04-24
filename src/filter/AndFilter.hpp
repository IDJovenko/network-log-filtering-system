#pragma once
#include <memory>
#include <vector>

#include "IFilter.hpp"

namespace em::ip_log_filter {
class AndFilter : public IFilter {
 public:
  AndFilter() = default;
  AndFilter& addFilter(std::unique_ptr<IFilter> filter);

  bool match(const IPv4& ip) const override;

 private:
  std::vector<std::unique_ptr<IFilter>> filters;
};
}  // namespace em::ip_log_filter
