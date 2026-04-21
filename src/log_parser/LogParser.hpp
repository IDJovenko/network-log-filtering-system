#pragma once
#include <optional>
#include <string_view>

#include "LogEntry.hpp"

namespace em::ip_log_filter {
class LogParser {
 public:
  std::optional<LogEntry> parseLine(std::string_view line);
};
}  // namespace em::ip_log_filter
