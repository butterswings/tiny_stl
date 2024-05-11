#ifndef SWING_PRIVATE_UTILITY_HPP
#define SWING_PRIVATE_UTILITY_HPP 1

#include <swing/type_traits.hpp>
#include <swing/private/move.hpp>

namespace swing
{

  template <typename _Integer, _Integer ..._Idx>
  struct integer_sequence
  {
    using type = integer_sequence;
    using value_type = _Integer;
    static constexpr std::size_t size() noexcept { return sizeof...(_Idx); }
  };

  template <std::size_t ..._Idx>
  using index_sequence = integer_sequence<std::size_t, _Idx...>;

  namespace detail
  {
    template <std::size_t, typename ...>
    struct _Nth_type;

    template <std::size_t _Idx, typename _Head, typename ..._Tail>
    struct _Nth_type<_Idx, _Head, _Tail...>
    { using type = typename _Nth_type<_Idx - 1, _Tail...>::type; };

    template <typename _Head, typename ..._Tail>
    struct _Nth_type<0, _Head, _Tail...>
    { using type = _Head; };

    template <class _Seq1, class _Seq2>
    struct __merge_and_renumber;

    template <typename _Integer, _Integer... I1, _Integer... I2>
    struct __merge_and_renumber<
      integer_sequence<_Integer, I1...>,
      integer_sequence<_Integer, I2...>>
    : integer_sequence<_Integer, I1..., (sizeof...(I1)+I2)...> { };

    template <typename _Integer, typename _Constant>
    struct __make_integer_sequence
    : __merge_and_renumber<
        typename __make_integer_sequence<
          _Integer, integral_constant<_Integer, _Constant::value / 2>>::type,
        typename __make_integer_sequence<
          _Integer, integral_constant<_Integer, _Constant::value - _Constant::value / 2>>::type> { };

    template<typename _Integer>
    struct __make_integer_sequence<
      _Integer, integral_constant<_Integer, 0>>
    : integer_sequence<_Integer> { };

    template<typename _Integer>
    struct __make_integer_sequence<
      _Integer, integral_constant<_Integer, 1>>
    : integer_sequence<_Integer, 0> { };
  }

  template <typename _Integer, _Integer _Num>
  using make_integer_sequence =
    typename detail::__make_integer_sequence<_Integer, integral_constant<_Integer, _Num>>::type;

  template <std::size_t _Num>
  using make_index_sequence = make_integer_sequence<std::size_t, _Num>;

} // namespace swing

#endif // SWING_PRIVATE_UTILITY_HPP
