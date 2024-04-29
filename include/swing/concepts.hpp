#ifndef SWING_CONCEPTS_HPP
#define SWING_CONCEPTS_HPP 1

#include <swing/type_traits.hpp>

namespace swing
{
#if __cplusplus > 201703L

template <typename _Tp, typename _Up>
concept same_as = is_same<_Tp, _Up>::value && is_same<_Up, _Tp>::value;

// public and unambiguous
template <typename _Derived, typename _Base>
concept derived_from = is_base_of<_Base, _Derived>::value
  && is_convertible<const volatile _Derived*, const volatile _Base*>::value;

template <typename _From, typename _To>
concept convertible_to = is_convertible<_From, _To>::value
  && requires { static_cast<_To>(declval<_From>()); };

#endif // C++20
} // namespace swing

#endif // SWING_CONCEPTS_HPP
