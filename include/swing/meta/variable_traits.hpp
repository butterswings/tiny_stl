#ifndef SWING_VARIABLE_TRAITS_HPP
#define SWING_VARIABLE_TRAITS_HPP 1

#include <swing/type_traits.hpp>

namespace swing
{
  namespace detail
  {
    template <typename _Tp>
    struct __variable_traits
    {
      using variable_type = _Tp;
      using pointer_type  = add_pointer_t<_Tp>;
    };

    template <typename _Tp, typename _Cp>
    struct __variable_traits<_Tp _Cp::*>
    {
      using variable_type = _Tp;
      using pointer_type  = _Tp _Cp::*;
      using class_type    = _Cp;
    };

  } // namespace detail

  namespace meta
  {
    template <typename _Tp, typename _Up = remove_pointer_t<decay_t<_Tp>>,
              bool = disjunction<is_member_function_pointer<_Up>, is_function<_Up>>::value>
    struct variable_traits;

    template <typename _Tp, typename _Up>
    struct variable_traits<_Tp, _Up, false>
    : detail::__variable_traits<_Tp> { };

  } // namespace meta

} // namespace swing

#endif // SWING_VARIABLE_TRAITS_HPP
