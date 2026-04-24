// Here is the implementation of the process_stream function declared in
// ip_log_filter.hpp.

#include <iostream>
#include <stdexcept>
#include <string>

#include "IFilter.hpp"
#include "LogParser.hpp"
#include "LogWriter.hpp"
#include "ip_log_filter.hpp"  // process_stream declaration

namespace em::ip_log_filter {

void process_stream(std::istream& input, std::ostream& output,
                    std::unique_ptr<IFilter> filter) {
  BufferedLineWriter writer(output, OUTPUT_BUFFER_SIZE, AVG_LINE_SIZE);
  LogParser parser;

  std::string line;
  while (std::getline(input, line)) {
    auto logEntry = parser.parseLine(line);
    if (!logEntry) {
      continue;
    };

    if (!filter->match(logEntry->ip)) {
      continue;
    }

    writer.writeLine(line);
  }
}
}  // namespace em::ip_log_filter
