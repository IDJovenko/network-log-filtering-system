#include "IPv4.hpp"

#include <charconv>
#include <exception>
#include <sstream>
#include <stdexcept>

namespace em::ip_log_filter {
std::optional<IPv4> IPv4::parse(std::string_view s) {
  unsigned int octets[4];

  const char* ptr = s.data();
  const char* end = s.data() + s.size();

  for (int i = 0; i < 4; ++i) {
    auto [next_ptr, ec] = std::from_chars(ptr, end, octets[i]);
    if (ec != std::errc() || octets[i] > 255) {
      return std::nullopt;
    }

    if (i < 3) {
      if (next_ptr == end || *next_ptr != '.') {
        return std::nullopt;
      }
      ptr = next_ptr + 1;  // move to next octet
    } else {
      // No characters after the last octet there must be
      if (next_ptr != end) {
        return std::nullopt;
      }
    }
  }

  uint32_t addr =
      (octets[0] << 24) | (octets[1] << 16) | (octets[2] << 8) | octets[3];
  return IPv4(addr);
}

std::optional<IPv4> IPv4::maskFromPrefix(int prefixLen) noexcept {
  if (prefixLen < 0 || prefixLen > 32) {
    return std::nullopt;
  }

  uint32_t mask = (prefixLen == 0) ? 0 : (0xFFFFFFFFu << (32 - prefixLen));
  return IPv4(mask);
}

IPv4& IPv4::operator&=(const IPv4& other) {
  addr &= other.addr;
  return *this;
}
IPv4& IPv4::operator|=(const IPv4& other) {
  addr |= other.addr;
  return *this;
}
IPv4& IPv4::operator^=(const IPv4& other) {
  addr ^= other.addr;
  return *this;
}
IPv4& IPv4::operator<<=(int shift) {
  if (shift < 0 || shift > 32) {
    throw std::invalid_argument(
        "IPv4::operator<<= shift must be between 0 and 32");
  }
  addr <<= shift;
  return *this;
}
IPv4& IPv4::operator>>=(int shift) {
  if (shift < 0 || shift > 32) {
    throw std::invalid_argument(
        "IPv4::operator<<= shift must be between 0 and 32");
  }
  addr >>= shift;
  return *this;
}
}  // namespace em::ip_log_filter
