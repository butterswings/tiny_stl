#ifndef SWING_PRIVATE_RANGE_ACCESS_H
#define SWING_PRIVATE_RANGE_ACCESS_H

#include <cstddef>

namespace swing
{
  template <typename _Tp, std::size_t _Num>
  [[nodiscard]]
  inline constexpr std::size_t
  size(const _Tp (&)[_Num]) noexcept
  { return _Num; }

} // namespace swing

#endif // SWING_PRIVATE_RANGE_ACCESS_H
