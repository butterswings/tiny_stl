#ifndef SWING_STRING_LITERAL_HPP
#define SWING_STRING_LITERAL_HPP 1

#define SWING_STRING_LITERAL_DEPRECATED_DESIGN 1

#include <swing/utility.hpp>

namespace swing
{
#ifdef SWING_STRING_LITERAL_DEPRECATED_DESIGN

  // WARNING: using magic_string<> or macro make_string_literal("")
  // may lead to runtime potential undefined behavior
  template <char ..._Chars>
  struct magic_string
  {
    using type = magic_string;
    static constexpr std::size_t size() { return sizeof...(_Chars); }

    /**
    * @brief value member of swing::magic_string,
    * which stores compilation time characters
    * NOTE: read of element of array without known bound
    * is not allowed in a constant expression when using clang
    * in fact if without sizeof..., magic_string's value member
    * is still a char array which is known bound
    */
    static constexpr char value[sizeof...(_Chars)] = { _Chars... };
  };
#else
  /**
  * @brief this implementation can be used as non-type template parameter,
  * like usage: func<"   ">, func is a function template with non-type
  * template parameter magic_string, but to be non-type parameter
  * make it template meta programming unfriendly and diffcult to distinguish type
  * other implementation of string operations need constexpr functions
  * and they cannot be type only
  * @tparam _Num size of input string literal
  */
  template <std::size_t _Num>
  struct magic_string
  {
    /**
    * @brief constexpr constructor of non-type magic_string
    * @param __arr ref to string literal
    */
    constexpr magic_string(const char (&__arr)[_Num])
    { std::copy(__arr, __arr + _Num, value); }

    const char value[_Num] { };

    // other operations
  };

#endif

  namespace string_literal
  {
    // forward declarations
    template <typename _Literal, char _Char>       // for string_literal::reverse
    struct push_front;

    template <typename _Literal, char _Char>       // for string_literal::substr
    struct push_back;

    template <std::size_t _Idx, typename _Literal> // for string_literal::substr
    struct at;

    template <typename ..._Literals>               // for string_literal::substr
    struct concat;

  } // namespace string_literal

  namespace detail
  {
    template <typename _Literal>
    struct __string_literal_pop_front;

    template <char _Front, char ..._Chars>
    struct __string_literal_pop_front<magic_string<_Front, _Chars...>>
    : magic_string<_Chars...> { };

    template <>
    struct __string_literal_pop_front<magic_string<>>
    : magic_string<> { };

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
      magic_string<_Char, _Chars1...>, magic_string<_Chars2...>>
    : __string_literal_reverse<magic_string<_Chars1...>,
        typename swing::string_literal::push_front<magic_string<_Chars2...>, _Char>::type> { };

    template <char ..._Chars>
    struct __string_literal_reverse<magic_string<>, magic_string<_Chars...>>
    : magic_string<_Chars...> { };

    template <typename _Literal1, typename _Literal2>
    struct __string_literal_concat;

    template <char ..._Chars1, char ..._Chars2>
    struct __string_literal_concat<
      magic_string<_Chars1...>, magic_string<_Chars2...>>
    : magic_string<_Chars1..., _Chars2...> { };

    template <std::size_t _Idx, typename _Literal>
    struct __string_literal_at;

    template <std::size_t _Idx, char _1st, char ..._nth>
    struct __string_literal_at<_Idx, magic_string<_1st, _nth...>>
    : __string_literal_at<_Idx - 1, magic_string<_nth...>> { };

    template <char _1st, char ..._nth>
    struct __string_literal_at<0, magic_string<_1st, _nth...>>
    : magic_string<_1st> { };

    template <typename _Literal, std::size_t _Start, std::size_t _End,
              std::size_t _Steps = _End - _Start, typename _Sub = magic_string<>>
    struct __string_literal_substr
    : __string_literal_substr<_Literal, _Start, _End, _Steps - 1,
        // string_literal::push_back<_Sub,
        //   string_literal::at<_Start + (_End - _Start - _Steps), _Literal>::value[0]>> { };
        string_literal::concat<_Sub,
          string_literal::at<_Start + (_End - _Start - _Steps), _Literal>>> { };

    template <typename _Literal, std::size_t _Start, std::size_t _End, typename _Sub>
    struct __string_literal_substr<_Literal, _Start, _End, 0, _Sub> : _Sub { };

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
    : reverse<pop_front<reverse<_Literal>>> { };

    template <std::size_t _Idx, typename _Literal>
    struct at : detail::__string_literal_at<_Idx, typename _Literal::type>
    { static_assert(_Idx < _Literal::size(), "string literal index must be in range"); };

    template <typename _Literal>
    struct back : at<_Literal::size() - 1, _Literal>
    {
      static_assert(!is_same<magic_string<>, typename _Literal::type>::value,
        "string_literal::back<> on an empty string literal is invalid");
    };

    template <typename _Literal>
    struct front : at<0, _Literal>
    {
      static_assert(!is_same<magic_string<>, typename _Literal::type>::value,
        "string_literal::front<> on an empty string literal is invalid");
    };

    template <typename _Literal1, typename _Literal2>
    struct concat<_Literal1, _Literal2>
    : detail::__string_literal_concat<
        typename _Literal1::type,
        typename _Literal2::type> { };

    template <typename _Literal1, typename _Literal2, typename ..._Literals>
    struct concat<_Literal1, _Literal2, _Literals...>
    : concat<concat<_Literal1, _Literal2>, _Literals...> { };

    /**
    * @brief substr for swing::magic_string, range [_Start _End)
    * WARNING: like std::string::substr, usage like substr(1, 1), substr(2, 2)...
    * will get an empty string, swing::string_literal::substr<_Literal, val, val>
    * will provide magic_string<>, which value member is invalid
    *
    * @tparam _Literal input string_literal
    * @tparam _Start start of the required range (default = 0)
    * @tparam _End end of the required range (default = _Literal::size())
    */
    template <typename _Literal,
              std::size_t _Start = 0, std::size_t _End = _Literal::size()>
    struct substr : detail::__string_literal_substr<_Literal, _Start, _End> { };

    template <typename _Literal>
    struct c_str : conditional_t<
      is_same<magic_string<'\0'>, typename back<_Literal>::type>::value,
      _Literal, push_back<_Literal, '\0'>> { };

  } //namespace string_literal

#if __cplusplus <= 201703L
  namespace detail
  {
    template <std::size_t _Num>
    constexpr char
    __string_separator(const char(&__str)[_Num], std::size_t __idx)
    { return __idx < _Num ? __str[__idx] : '\0'; }

  } // namespace detail
#endif

} // namespace swing


#if __cplusplus > 201703L
#define make_string_literal(__str)                                                                   \
  decltype( []<std::size_t... __idx>(swing::index_sequence<__idx...>) constexpr {                    \
    return swing::magic_string<__str[__idx]...> { };                                                 \
  } ( swing::make_index_sequence<sizeof(__str) - 1> { } ) )

#elif __cplusplus > 201402L
#define make_string_literal(__str)                 \
swing::string_literal::substr<                     \
  swing::magic_string<                             \
    swing::detail::__string_separator(__str, 0),   \
    swing::detail::__string_separator(__str, 1),   \
    swing::detail::__string_separator(__str, 2),   \
    swing::detail::__string_separator(__str, 3),   \
    swing::detail::__string_separator(__str, 4),   \
    swing::detail::__string_separator(__str, 5),   \
    swing::detail::__string_separator(__str, 6),   \
    swing::detail::__string_separator(__str, 7),   \
    swing::detail::__string_separator(__str, 8),   \
    swing::detail::__string_separator(__str, 9),   \
    swing::detail::__string_separator(__str, 10),  \
    swing::detail::__string_separator(__str, 11),  \
    swing::detail::__string_separator(__str, 12),  \
    swing::detail::__string_separator(__str, 13),  \
    swing::detail::__string_separator(__str, 14),  \
    swing::detail::__string_separator(__str, 15),  \
    swing::detail::__string_separator(__str, 16),  \
    swing::detail::__string_separator(__str, 17),  \
    swing::detail::__string_separator(__str, 18),  \
    swing::detail::__string_separator(__str, 19),  \
    swing::detail::__string_separator(__str, 20),  \
    swing::detail::__string_separator(__str, 21),  \
    swing::detail::__string_separator(__str, 22),  \
    swing::detail::__string_separator(__str, 23),  \
    swing::detail::__string_separator(__str, 24),  \
    swing::detail::__string_separator(__str, 25),  \
    swing::detail::__string_separator(__str, 26),  \
    swing::detail::__string_separator(__str, 27),  \
    swing::detail::__string_separator(__str, 28),  \
    swing::detail::__string_separator(__str, 29),  \
    swing::detail::__string_separator(__str, 30),  \
    swing::detail::__string_separator(__str, 31),  \
    swing::detail::__string_separator(__str, 32),  \
    swing::detail::__string_separator(__str, 33),  \
    swing::detail::__string_separator(__str, 34),  \
    swing::detail::__string_separator(__str, 35),  \
    swing::detail::__string_separator(__str, 36),  \
    swing::detail::__string_separator(__str, 37),  \
    swing::detail::__string_separator(__str, 38),  \
    swing::detail::__string_separator(__str, 39),  \
    swing::detail::__string_separator(__str, 40),  \
    swing::detail::__string_separator(__str, 41),  \
    swing::detail::__string_separator(__str, 42),  \
    swing::detail::__string_separator(__str, 43),  \
    swing::detail::__string_separator(__str, 44),  \
    swing::detail::__string_separator(__str, 45),  \
    swing::detail::__string_separator(__str, 46),  \
    swing::detail::__string_separator(__str, 47),  \
    swing::detail::__string_separator(__str, 48),  \
    swing::detail::__string_separator(__str, 49),  \
    swing::detail::__string_separator(__str, 50),  \
    swing::detail::__string_separator(__str, 51),  \
    swing::detail::__string_separator(__str, 52),  \
    swing::detail::__string_separator(__str, 53),  \
    swing::detail::__string_separator(__str, 54),  \
    swing::detail::__string_separator(__str, 55),  \
    swing::detail::__string_separator(__str, 56),  \
    swing::detail::__string_separator(__str, 57),  \
    swing::detail::__string_separator(__str, 58),  \
    swing::detail::__string_separator(__str, 59),  \
    swing::detail::__string_separator(__str, 60),  \
    swing::detail::__string_separator(__str, 61),  \
    swing::detail::__string_separator(__str, 62),  \
    swing::detail::__string_separator(__str, 63)>, \
0, sizeof(__str) - 1>

#endif

#endif // SWING_STRING_LITERAL_HPP
