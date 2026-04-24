#include "FilterFactory.hpp"

#include <stdexcept>
#include <unordered_map>

#include "AndFilter.hpp"
#include "RangeFilter.hpp"
#include "SubnetFilter.hpp"

namespace em::ip_log_filter {

template <typename T>
concept ConcreteFilterT =
    std::derived_from<T, IFilter> && requires(std::string_view sv) {
      { T::create(sv) } -> std::same_as<std::optional<T>>;
    };

template <ConcreteFilterT FilterT>
std::unique_ptr<IFilter> makeConcreteFilterFromString(std::string_view value) {
  auto tmpFilter = FilterT::create(value);
  if (!tmpFilter) {
    return nullptr;
  }
  return std::make_unique<FilterT>(std::move(*tmpFilter));
}

std::unique_ptr<IFilter> FilterFactory::createConcrete(
    const FilterRule& params) {
  // params parsing: ["type", "subnet", "value", "192.168.0.0/24", ...]
  auto [type, value] = params;

  if (type == "subnet") {
    return makeConcreteFilterFromString<SubnetFilter>(value);
  };
  if (type == "range") {
    return makeConcreteFilterFromString<RangeFilter>(value);
  }

  throw std::runtime_error("Unknown filter type");
}

std::unique_ptr<IFilter> FilterFactory::create(
    const std::vector<FilterRule>& params) {
  if (params.empty()) {
    return nullptr;
  }

  AndFilter filters;

  for (const auto& rule : params) {
    auto filter = createConcrete(rule);
    if (!filter) {
      throw std::runtime_error("Failed to create \"" + rule.type +
                               "\" filter with value: \"" + rule.value + "\"");
    }
    filters.addFilter(std::move(filter));
  }

  return std::make_unique<AndFilter>(std::move(filters));
}
}  // namespace em::ip_log_filter
