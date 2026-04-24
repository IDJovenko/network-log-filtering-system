#include "LogParser.hpp"

#include <gtest/gtest.h>

#include <optional>
#include <string_view>

#include "IPv4.hpp"

namespace em::ip_log_filter {
namespace {

std::optional<IPv4> ParseIp(std::string_view ip) { return IPv4::parse(ip); }

TEST(LogParserValidLineTest, ExtractsIpAndMessageFromStandardFormat) {
  LogParser parser;

  const auto entry = parser.parseLine("192.168.1.10 - LOGIN_OK");
  const auto expectedIp = ParseIp("192.168.1.10");

  ASSERT_TRUE(entry.has_value());
  ASSERT_TRUE(expectedIp.has_value());

  EXPECT_EQ(entry->ip, *expectedIp);
  EXPECT_EQ(entry->message, "LOGIN_OK");
}

TEST(LogParserValidLineTest, PreservesMessageContent) {
  LogParser parser;

  const auto entry = parser.parseLine("10.0.0.1 - user-login-failed");

  ASSERT_TRUE(entry.has_value());
  EXPECT_EQ(entry->message, "user-login-failed");
}

TEST(LogParserInvalidLineTest, RejectsEmptyLineAndLineWithoutIp) {
  LogParser parser;

  EXPECT_EQ(parser.parseLine(""), std::nullopt);
  EXPECT_EQ(parser.parseLine("ONLY_MESSAGE"), std::nullopt);
  EXPECT_EQ(parser.parseLine(" - EMPTY_LINE"), std::nullopt);
}

TEST(LogParserInvalidLineTest, RejectsBrokenFormat) {
  LogParser parser;

  EXPECT_EQ(parser.parseLine("192.168.1.1 LOGIN_OK"), std::nullopt);
  EXPECT_EQ(parser.parseLine("192.168.1.1-LOGIN_OK"), std::nullopt);
}

TEST(LogParserInvalidLineTest, RejectsInvalidDataCasesFromSpec) {
  LogParser parser;

  EXPECT_EQ(parser.parseLine("192.168.1.256 - INVALID_OCTET"), std::nullopt);
  EXPECT_EQ(parser.parseLine("192.168.1 - INCOMPLETE"), std::nullopt);
  EXPECT_EQ(parser.parseLine("invalid.ip - CORRUPTED"), std::nullopt);
  EXPECT_EQ(parser.parseLine(" - EMPTY_LINE"), std::nullopt);
}

}  // namespace
}  // namespace em::ip_log_filter
