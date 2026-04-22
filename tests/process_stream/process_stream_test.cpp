#include <gtest/gtest.h>

#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

#include "IPv4.hpp"
#include "RangeFilter.hpp"
#include "ip_log_filter.hpp"  // process_stream declaration

namespace em::ip_log_filter {
namespace {

std::optional<IPv4> Ip(std::string_view value) { return IPv4::parse(value); }

std::unique_ptr<IFilter> MakeRangeFilter(std::string_view start,
                                         std::string_view end) {
  const auto startIp = Ip(start);
  const auto endIp = Ip(end);

  if (!startIp || !endIp) {
    return nullptr;
  }

  auto filter = RangeFilter::create(*startIp, *endIp);
  if (!filter) {
    return nullptr;
  }

  return std::make_unique<RangeFilter>(*filter);
}

std::vector<std::string> SplitLines(const std::string& text) {
  std::vector<std::string> lines;
  std::istringstream input(text);
  std::string line;

  while (std::getline(input, line)) {
    lines.push_back(line);
  }

  return lines;
}

TEST(ProcessStreamTest, EmptyStream) {
  std::istringstream input("");
  std::ostringstream output;

  auto filter = MakeRangeFilter("10.0.0.1", "10.0.0.255");
  ASSERT_TRUE(filter);

  process_stream(input, output, std::move(filter));

  EXPECT_TRUE(output.str().empty());
}

TEST(ProcessStreamTest, StreamWithEmptyLinesBetweenValidLines) {
  std::istringstream input(
      "10.0.0.1 - FIRST\n"
      "\n"
      "10.0.0.2 - SECOND\n"
      "   \n"
      "\t\n"
      "10.0.0.3 - THIRD\n"
      "\n"
      "10.0.0.4 - FOURTH\n");
  std::ostringstream output;

  auto filter = MakeRangeFilter("10.0.0.1", "10.0.0.255");
  ASSERT_TRUE(filter);

  process_stream(input, output, std::move(filter));

  EXPECT_EQ(output.str(),
            "10.0.0.1 - FIRST\n"
            "10.0.0.2 - SECOND\n"
            "10.0.0.3 - THIRD\n"
            "10.0.0.4 - FOURTH\n");
}

TEST(ProcessStreamTest, SmallStreamFiltersAndIgnoresInvalidLines) {
  std::istringstream input(
      "10.0.0.1 - KEEP_1\n"
      "10.0.1.1 - DROP_OUTSIDE\n"
      "invalid.ip - CORRUPTED\n"
      "10.0.0.2 - KEEP_2\n"
      " - EMPTY_LINE\n"
      "10.0.0.256 - INVALID_OCTET\n"
      "10.0.0.3 - KEEP_3\n");
  std::ostringstream output;

  auto filter = MakeRangeFilter("10.0.0.1", "10.0.0.255");
  ASSERT_TRUE(filter);

  process_stream(input, output, std::move(filter));

  EXPECT_EQ(output.str(),
            "10.0.0.1 - KEEP_1\n"
            "10.0.0.2 - KEEP_2\n"
            "10.0.0.3 - KEEP_3\n");
}

TEST(ProcessStreamTest, LargeStreamFlushesWithoutLosingLines) {
  std::ostringstream source;
  std::vector<std::string> expected;
  expected.reserve(1505);

  for (int i = 0; i < 1505; ++i) {
    const int octet = (i % 250) + 1;
    const std::string line =
        "10.0.0." + std::to_string(octet) + " - KEEP_" + std::to_string(i);

    source << line << '\n';
    expected.push_back(line);
  }

  std::istringstream input(source.str());
  std::ostringstream output;

  auto filter = MakeRangeFilter("10.0.0.1", "10.0.0.250");
  ASSERT_TRUE(filter);

  process_stream(input, output, std::move(filter));

  const auto actual = SplitLines(output.str());
  EXPECT_EQ(actual.size(), expected.size());
  EXPECT_EQ(actual, expected);
}

}  // namespace
}  // namespace em::ip_log_filter
