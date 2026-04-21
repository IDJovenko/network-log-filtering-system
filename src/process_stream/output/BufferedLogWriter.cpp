#include "LogWriter.hpp"

namespace em::ip_log_filter {
BufferedLineWriter::BufferedLineWriter(std::ostream& output,
                                       std::size_t maxLines,
                                       std::size_t avgLineSize)
    : output(output), maxLines(maxLines) {
  buffer.reserve(maxLines * avgLineSize);
}

void BufferedLineWriter::writeLine(std::string_view line) {
  buffer.append(line);
  buffer.push_back('\n');
  ++bufferedLines;

  if (bufferedLines >= maxLines) {
    flush();
  }
}

void BufferedLineWriter::flush() {
  output << buffer;
  buffer.clear();
  bufferedLines = 0;
}

BufferedLineWriter::~BufferedLineWriter() { flush(); }
}  // namespace em::ip_log_filter
