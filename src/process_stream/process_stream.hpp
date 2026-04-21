#pragma once
#include <iosfwd>

#include "create_filter.hpp"

namespace em::ip_log_filter {
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
