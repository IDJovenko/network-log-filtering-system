#include "IPv4.hpp"

#include <exception>
#include <sstream>

namespace em::ip_log_filter {
std::optional<IPv4> IPv4::parse(std::string_view s) {
  std::istringstream iss(std::string{s});
  unsigned int a, b, c, d;
  char dot1, dot2, dot3;

  if (!(iss >> a >> dot1 >> b >> dot2 >> c >> dot3 >> d) || dot1 != '.' ||
      dot2 != '.' || dot3 != '.' || a > 255 || b > 255 || c > 255 || d > 255) {
    return std::nullopt;
  }
  if (iss.get() != EOF) {
    return std::nullopt;  // Extra characters after valid IP
  }

  uint32_t addr = (a << 24) | (b << 16) | (c << 8) | d;
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
  addr <<= shift;
  return *this;
}
IPv4& IPv4::operator>>=(int shift) {
  addr >>= shift;
  return *this;
}
}  // namespace em::ip_log_filter
