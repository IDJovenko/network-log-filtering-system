#pragma once
#include <memory>
#include <string>
#include <vector>

namespace em::ip_log_filter {

class IFilter;

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

/**
 * @brief The maximum number of log lines to buffer before writing to the output
 * stream.
 *
 * This constant defines the size of the internal buffer used to optimize I/O
 * operations. When the buffer reaches this capacity, its contents are flushed
 * to the output in a single batch.
 */
inline constexpr std::size_t OUTPUT_BUFFER_SIZE = 1000;
/**
 * @brief The average size of a log line in bytes.
 *
 * This constant is used to estimate the total memory required for buffering log
 * lines. It helps to reserve an appropriate amount of memory for the buffer,
 * improving performance by reducing reallocations.
 */
inline constexpr std::size_t AVG_LINE_SIZE = 100;
/**
 * @brief Filters a stream of log data by IPv4 addresses based on the provided
 * rules.
 *
 * Reads the input stream line by line, extracts valid IPv4 addresses, and
 * applies the filter. Lines with invalid IPs or that do not match the filter
 * criteria are discarded.
 *
 * Writes are performed in batches of up to  @p OUTPUT_BUFFER_SIZE records.
 */
void process_stream(std::istream& input, std::ostream& output,
                    std::unique_ptr<IFilter> filter);
}  // namespace em::ip_log_filter
