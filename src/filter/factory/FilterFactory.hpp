#pragma once
#include <memory>
#include <vector>

#include "FilterRule.hpp"
#include "IFilter.hpp"

namespace em::ip_log_filter {
class FilterFactory {
 public:
  static std::unique_ptr<IFilter> create(const std::vector<FilterRule>& params);

 private:
  static std::unique_ptr<IFilter> createConcrete(const FilterRule& params);
};
}  // namespace em::ip_log_filter
