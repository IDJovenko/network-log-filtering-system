#pragma once
#include <cstdint>
#include <optional>
#include <string_view>

namespace em::ip_log_filter {
// NOTE If it is necessary to add IPv6,
// operators can be moved to the CRTP base class
class IPv4 {
 public:
  static std::optional<IPv4> parse(std::string_view ipStr);

  static std::optional<IPv4> maskFromPrefix(int prefixLen) noexcept;

  IPv4 operator&(const IPv4& other) const { return IPv4(addr & other.addr); }
  IPv4 operator|(const IPv4& other) const { return IPv4(addr | other.addr); }
  IPv4 operator^(const IPv4& other) const { return IPv4(addr ^ other.addr); }
  IPv4 operator~() const { return IPv4(~addr); }

  // Useful for subnet masks
  IPv4 operator<<(int shift) const { return IPv4(addr << shift); }
  IPv4 operator>>(int shift) const { return IPv4(addr >> shift); }

  IPv4& operator&=(const IPv4& other);
  IPv4& operator|=(const IPv4& other);
  IPv4& operator^=(const IPv4& other);
  IPv4& operator<<=(int shift);
  IPv4& operator>>=(int shift);

  auto operator<=>(const IPv4& other) const = default;

 private:
  explicit IPv4(uint32_t addr) : addr(addr) {}
  uint32_t addr;
};
}  // namespace em::ip_log_filter