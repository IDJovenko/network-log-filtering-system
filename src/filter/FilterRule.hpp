#pragma once
#include <string>

namespace em::ip_log_filter {

struct FilterRule {
  std::string type;
  std::string value;
};

}  // namespace em::ip_log_filter
