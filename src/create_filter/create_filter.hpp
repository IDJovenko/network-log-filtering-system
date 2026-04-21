#pragma once
#include <memory>
#include <string>
#include <vector>

#include "IFilter.hpp"

namespace em::ip_log_filter {

/// @brief represents a key-value pair filter rule.
///        Example: {"type", "subnet", "value", "192.168.0.0/24"}
using RawFilterRule = std::vector<std::string>;
// TODO Perhaps it's better make FilterRule an array or tuple.
// Deside on it, when view of project become clear

/**
 * @brief Creates a filter object from a list of rules.
 *
 * The resulting filter will match only those elements
 * that satisfy **all** provided rules.
 *
 * Example of how the function works:
 * \code
 * // Creating a filter (AND-combination)
 * auto filter = create_filter({
 *   {"type", "subnet", "value", "192.168.0.0/24"},
 *   {"type", "range", "value", "192.168.0.1-192.168.0.255"}
 * });
 * \endcode
 */
std::unique_ptr<IFilter> create_filter(
    const std::vector<RawFilterRule>& params);

}  // namespace em::ip_log_filter
