#pragma once
#include <ostream>
#include <string>
#include <string_view>

namespace em::ip_log_filter {

/**
 * @brief Interface for writing log lines.
 */
class ILineWriter {
 public:
  virtual ~ILineWriter() = default;

  virtual void writeLine(std::string_view line) = 0;
  virtual void flush() = 0;
};

/**
 * @brief A class that buffers log lines and writes them to an output stream in
 * batches.
 *
 * This class is designed to optimize I/O performance by reducing the number of
 * write operations. It accumulates log lines in an internal buffer and flushes
 * them to the output stream when the buffer reaches a specified capacity or
 * when the object is destroyed.
 */
class BufferedLineWriter : public ILineWriter {
 public:
  BufferedLineWriter(std::ostream& output, std::size_t maxLines,
                     std::size_t avgLineSize = 64);

  void writeLine(std::string_view line) override;
  void flush() override;

  ~BufferedLineWriter() override;

 private:
  std::ostream& output;
  std::string buffer;
  std::size_t maxLines;
  std::size_t bufferedLines = 0;
};

}  // namespace em::ip_log_filter
