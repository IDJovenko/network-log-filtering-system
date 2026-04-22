#include <exception>
#include <iostream>

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

BufferedLineWriter::~BufferedLineWriter() {
  try {
    flush();
  } catch (const std::exception& e) {
    // TODO: Replace fprintf(stderr, ...) with proper logging system.
    fprintf(stderr,
            "BufferedLineWriter::~BufferedLineWriter():"
            "Error while flushing buffer: %s",
            e.what());
    // TODO: write to fallback file unflushed logs
  } catch (...) {
    // TODO: Replace fprintf(stderr, ...) with proper logging system.
    fprintf(stderr,
            "BufferedLineWriter::~BufferedLineWriter(): Unknown error while "
            "flushing buffer.");
    // TODO: write to fallback file unflushed logs
  }
}
}  // namespace em::ip_log_filter
