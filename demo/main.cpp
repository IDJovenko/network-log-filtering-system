#include <fstream>
#include <iostream>

#include "create_filter.hpp"
#include "process_stream.hpp"

int main() {
  // Creating a filter (AND combination)
  auto filter = em::ip_log_filter::create_filter(
      {{"type", "subnet", "value", "192.168.0.0/24"},
       {"type", "range", "value", "192.168.0.240-192.168.0.255"}});

  // Processing the log file
  auto fin = std::ifstream("test.log");
  if (!fin.is_open()) {
    std::cerr << "Failed to open test.log" << std::endl;
    return 1;
  }
  em::ip_log_filter::process_stream(fin, std::cout, std::move(filter));
}
