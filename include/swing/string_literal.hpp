#ifndef SWING_STRING_LITERAL_HPP
#define SWING_STRING_LITERAL_HPP 1

#include <swing/utility.hpp>

namespace swing
{
// WARNING: using magic_string<> can lead to runtime potential undefined behavior
// like std::string(magic_string<>::value), use macro make_string_literal("") instead
template <char ..._Chars>
struct magic_string
{
  using type = magic_string;
  static const char value[];
  static constexpr std::size_t size() { return sizeof...(_Chars); }
};

template <char ..._Chars>
constexpr char magic_string<_Chars...>::value[] { _Chars... };

namespace string_literal
{
// forward declaration for string_literal::reverse
template <typename _Literal, char _Char>
struct push_front;
} // namespace string_literal

namespace detail
{
template <typename _Literal>
struct __string_literal_pop_front;

template <char _Front, char ..._Chars>
struct __string_literal_pop_front<magic_string<_Front, _Chars...>>
: magic_string<_Chars...> { };

template <typename _Literal, char _Char>
struct __string_literal_push_front;

template <char ..._Chars, char _Char>
struct __string_literal_push_front<magic_string<_Chars...>, _Char>
: magic_string<_Char, _Chars...> { };

template <typename _Literal, char _Char>
struct __string_literal_push_back;

template <char ..._Chars, char _Char>
struct __string_literal_push_back<magic_string<_Chars...>, _Char>
: magic_string<_Chars..., _Char> { };

template <typename _Literal, typename _Reversed>
struct __string_literal_reverse;

template <char _Char, char ..._Chars1, char ..._Chars2>
struct __string_literal_reverse<
         magic_string<_Char, _Chars1...>,
         magic_string<_Chars2...>>
: __string_literal_reverse<
    magic_string<_Chars1...>,
    typename swing::string_literal::push_front<magic_string<_Chars2...>, _Char>::type> { };

template <char ..._Chars>
struct __string_literal_reverse<magic_string<>, magic_string<_Chars...>>
: magic_string<_Chars...> { };

template <typename _Literal1, typename _Literal2>
struct __string_literal_concat;

template <char ..._Chars1, char ..._Chars2>
struct __string_literal_concat<
         magic_string<_Chars1...>,
         magic_string<_Chars2...>>
: magic_string<_Chars1..., _Chars2...> { };

} // namespace detail

namespace string_literal
{
template <typename _Literal>
struct pop_front
: detail::__string_literal_pop_front<typename _Literal::type> { };

template <typename _Literal, char _Char>
struct push_front
: detail::__string_literal_push_front<typename _Literal::type, _Char> { };

template <typename _Literal, char _Char>
struct push_back
: detail::__string_literal_push_back<typename _Literal::type, _Char> { };

template <typename _Literal>
struct reverse
: detail::__string_literal_reverse<
    typename _Literal::type, magic_string<>> { };

template <typename _Literal>
struct pop_back
: reverse<
    typename pop_front<
      typename reverse<
        typename _Literal::type>::type>::type> { };

template <typename ..._Literals>
struct concat;

template <typename _Literal1, typename _Literal2>
struct concat<_Literal1, _Literal2>
: detail::__string_literal_concat<
    typename pop_back<
      typename _Literal1::type>::type,
    typename _Literal2::type> { };

template <typename _Literal1, typename _Literal2, typename ..._Literals>
struct concat<_Literal1, _Literal2, _Literals...>
: concat<
    typename concat<
      typename _Literal1::type, typename _Literal2::type>::type,
    typename _Literals::type...> { };

} //namespace string_literal

namespace detail
{
template <std::size_t _Num>
inline constexpr char
__string_separator(const char(&__str)[_Num], int __idx)
{ return __idx < _Num ? __str[__idx] : '\0'; }

} // namespace detail

} // namespace swing


#if __cplusplus > 201703L
#define make_string_literal(__str)                                                                                     \
  decltype( []<std::size_t... __idx>(swing::index_sequence<__idx...>) constexpr {                                      \
    return swing::magic_string<swing::detail::__string_separator(__str, __idx)...> { };                                \
  } ( swing::make_index_sequence<sizeof(__str)> { } ) )

#elif __cplusplus > 201402L
#define make_string_literal(__str)            \
swing::magic_string<                          \
swing::detail::__string_separator(__str, 0),  \
swing::detail::__string_separator(__str, 1),  \
swing::detail::__string_separator(__str, 2),  \
swing::detail::__string_separator(__str, 3),  \
swing::detail::__string_separator(__str, 4),  \
swing::detail::__string_separator(__str, 5),  \
swing::detail::__string_separator(__str, 6),  \
swing::detail::__string_separator(__str, 7),  \
swing::detail::__string_separator(__str, 8),  \
swing::detail::__string_separator(__str, 9),  \
swing::detail::__string_separator(__str, 10), \
swing::detail::__string_separator(__str, 11), \
swing::detail::__string_separator(__str, 12), \
swing::detail::__string_separator(__str, 13), \
swing::detail::__string_separator(__str, 14), \
swing::detail::__string_separator(__str, 15), \
swing::detail::__string_separator(__str, 16), \
swing::detail::__string_separator(__str, 17), \
swing::detail::__string_separator(__str, 18), \
swing::detail::__string_separator(__str, 19), \
swing::detail::__string_separator(__str, 20), \
swing::detail::__string_separator(__str, 21), \
swing::detail::__string_separator(__str, 22), \
swing::detail::__string_separator(__str, 23), \
swing::detail::__string_separator(__str, 24), \
swing::detail::__string_separator(__str, 25), \
swing::detail::__string_separator(__str, 26), \
swing::detail::__string_separator(__str, 27), \
swing::detail::__string_separator(__str, 28), \
swing::detail::__string_separator(__str, 29), \
swing::detail::__string_separator(__str, 30), \
swing::detail::__string_separator(__str, 31), \
swing::detail::__string_separator(__str, 32), \
swing::detail::__string_separator(__str, 33), \
swing::detail::__string_separator(__str, 34), \
swing::detail::__string_separator(__str, 35), \
swing::detail::__string_separator(__str, 36), \
swing::detail::__string_separator(__str, 37), \
swing::detail::__string_separator(__str, 38), \
swing::detail::__string_separator(__str, 39), \
swing::detail::__string_separator(__str, 40), \
swing::detail::__string_separator(__str, 41), \
swing::detail::__string_separator(__str, 42), \
swing::detail::__string_separator(__str, 43), \
swing::detail::__string_separator(__str, 44), \
swing::detail::__string_separator(__str, 45), \
swing::detail::__string_separator(__str, 46), \
swing::detail::__string_separator(__str, 47), \
swing::detail::__string_separator(__str, 48), \
swing::detail::__string_separator(__str, 49), \
swing::detail::__string_separator(__str, 50), \
swing::detail::__string_separator(__str, 51), \
swing::detail::__string_separator(__str, 52), \
swing::detail::__string_separator(__str, 53), \
swing::detail::__string_separator(__str, 54), \
swing::detail::__string_separator(__str, 55), \
swing::detail::__string_separator(__str, 56), \
swing::detail::__string_separator(__str, 57), \
swing::detail::__string_separator(__str, 58), \
swing::detail::__string_separator(__str, 59), \
swing::detail::__string_separator(__str, 60), \
swing::detail::__string_separator(__str, 61), \
swing::detail::__string_separator(__str, 62), \
swing::detail::__string_separator(__str, 63)>

#endif

#endif // SWING_STRING_LITERAL_HPP
