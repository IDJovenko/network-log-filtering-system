#include "LogParser.hpp"

#include <optional>

namespace em::ip_log_filter {
std::optional<LogEntry> LogParser::parseLine(std::string_view line) {
  static constexpr std::string_view delimetr = " - ";
  const auto delimPosition = line.find(delimetr);
  if (delimPosition == std::string_view::npos) {
    return std::nullopt;
  }

  auto ip = IPv4::parse(line.substr(0, delimPosition));
  if (!ip) {
    return std::nullopt;
  }
  auto msg = line.substr(delimPosition + delimetr.size());

  return LogEntry{.ip = *ip, .message = std::string(msg)};
}
}  // namespace em::ip_log_filter