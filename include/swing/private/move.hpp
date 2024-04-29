#ifndef SWING_MOVE_HPP
#define SWING_MOVE_HPP 1

#include <swing/type_traits.hpp>

namespace swing
{
  template <typename _Tp>
  [[nodiscard]]
  constexpr _Tp&& forward(remove_reference_t<_Tp>& __t) noexcept
  { return static_cast<_Tp&&>(__t); }

  // WARNING: usage like forward<int&>(1) is forbidden
  template <typename _Tp>
  [[nodiscard]]
  constexpr _Tp&& forward(remove_reference_t<_Tp>&& __t) noexcept
  {
    static_assert(!is_lvalue_reference<_Tp>::value,
                  "forward must not to be used to convert an rvalue to lvalue");
    return static_cast<_Tp&&>(__t);
  }

  template <typename _Tp>
  [[nodiscard]]
  constexpr remove_reference_t<_Tp>&&
  move(_Tp&& __t) noexcept
  { return static_cast<remove_reference_t<_Tp>&&>(__t); }


} // namespace swing


#endif
