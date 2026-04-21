#include "AndFilter.hpp"

namespace em::ip_log_filter {
AndFilter& AndFilter::addFilter(std::unique_ptr<IFilter> filter) {
  if (filter) {
    filters.push_back(std::move(filter));
  }

  return *this;
}

bool AndFilter::match(const IPv4& ip) const {
  for (const auto& filter : filters) {
    if (!filter->match(ip)) {
      return false;
    }
  }

  return true;  // if filters are empty or all filters matches
}
}  // namespace em::ip_log_filter
