#include <string>

#include "IPv4.hpp"

namespace em::ip_log_filter {
struct LogEntry {
  IPv4 ip;
  std::string message;
};
}  // namespace em::ip_log_filter
