#include <algorithm>
#include <stdexcept>

#include "FilterFactory.hpp"
#include "ip_log_filter.hpp"

namespace em::ip_log_filter {

static std::vector<FilterRule> parseRawFilterRules(
    const std::vector<RawFilterRule>& rawRules) {
  std::vector<FilterRule> parsedRules;
  parsedRules.reserve(rawRules.size());

  std::transform(rawRules.begin(), rawRules.end(),
                 std::back_inserter(parsedRules),
                 [](const RawFilterRule& rawRule) {
                   if (rawRule.size() != 4) {
                     throw std::runtime_error("Invalid filter rule format");
                   }
                   return FilterRule{rawRule[1], rawRule[3]};
                 });
  return parsedRules;
}

std::unique_ptr<IFilter> create_filter(
    const std::vector<RawFilterRule>& params) {
  auto parsed_params = parseRawFilterRules(params);
  return FilterFactory::create(parsed_params);
}

}  // namespace em::ip_log_filter
