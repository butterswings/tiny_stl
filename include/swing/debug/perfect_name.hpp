#ifndef SWING_DEBUG_PERFECT_NAME_HPP
#define SWING_DEBUG_PERFECT_NAME_HPP

#if __cplusplus > 201703L

#include <swing/type_traits.hpp>
#include <swing/string_literal.hpp>

namespace swing
{
  namespace debug
  {
    template <typename _Tp>
    inline consteval auto perfect_name()
    { return make_string_literal(__PRETTY_FUNCTION__) { }; }

  } // namespace debug
} // namespace swing

#endif // C++20
#endif // SWING_DEBUG_PERFECT_NAME_HPP
