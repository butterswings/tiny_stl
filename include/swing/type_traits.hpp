#ifndef SWING_TYPE_TRAITS_HPP
#define SWING_TYPE_TRAITS_HPP 1

#include <cstddef>
#if __cplusplus > 202002L
#include <stdfloat>

#endif

namespace swing
{
  template <typename _Tp>
  class reference_wrapper;

  template <typename _Tp, _Tp __v>
  struct integral_constant
  {
    static constexpr _Tp value = __v;
    using value_type           = _Tp;
    using type                 = integral_constant<_Tp, __v>;
    constexpr operator value_type() const noexcept { return value; }
    constexpr value_type operator ()() const noexcept { return value; };
  };

  template <bool __v>
  using bool_constant = integral_constant<bool, __v>;

  using true_type = bool_constant<true>;
  using false_type = bool_constant<false>;

  template <typename ...>
  using void_t = void;

  namespace detail
  {
    template <typename _Tp, typename _Up = _Tp&&>
    _Up __declval(int);

    template <typename _Tp>
    _Tp __declval(long);

  }

  namespace detail
  {
    template <typename _Tp>
    struct __declval_protector
    {
      static constexpr bool __stop = false;
    };
  }

  template <typename _Tp>
  auto declval() noexcept -> decltype(detail::__declval<_Tp>(0))
  {
    static_assert(detail::__declval_protector<_Tp>::__stop, "declval() must not be used!");
    return detail::__declval<_Tp>(0);
  }

  template <bool, typename _Tp = void>
  struct enable_if
  { };

  template <typename _Tp>
  struct enable_if<true, _Tp>
  { using type = _Tp; };

  template <bool _Cond, typename _Tp = void>
  using enable_if_t = typename enable_if<_Cond, _Tp>::type;

  // conditional
  template <bool>
  struct conditional
  {
    template <typename _Tp, typename>
    using type = _Tp;
  };

  template <>
  struct conditional<false>
  {
    template <typename, typename _Up>
    using type = _Up;
  };

  template <bool _Cond, typename _If, typename _Else>
  using conditional_t = typename conditional<_Cond>::template type<_If, _Else>;

  template <typename _Tp>
  struct type_identity
  { using type = _Tp; };

  template <typename _Tp>
  using type_identity_t = typename type_identity<_Tp>::type;

  template <typename _Tp, typename ...Types>
  struct first
  { using type = _Tp; };

  template <typename ...Types>
  using first_t = typename first<Types...>::type;

  template <typename _Tp, typename ...Types>
  struct last
  { using type = typename last<Types...>::type; };

  template <typename _Tp>
  struct last<_Tp>
  { using type = _Tp; };

  template <typename ...Ts>
  using last_t = typename last<Ts...>::type;

  template <typename ..._Bn>
  struct conjunction;

  template <>
  struct conjunction<>
  : true_type { };

  template <typename _B1, typename ..._Bn>
  struct conjunction<_B1, _Bn...>
  : conditional_t<bool(_B1::value), conjunction<_Bn...>, _B1> { };

  template <typename ...Bn>
  struct disjunction;

  template <>
  struct disjunction<>
  : false_type { };

  template <typename _B1, typename ..._Bn>
  struct disjunction<_B1, _Bn...>
  : conditional_t<bool(_B1::value), _B1, disjunction<_Bn...>> { };

  template <typename _Pp>
  struct negation
  : bool_constant<!bool(_Pp::value)> { };

  template <typename _Tp>
  struct remove_extent
  { using type = _Tp; };

  template <typename _Tp, std::size_t _Size>
  struct remove_extent<_Tp[_Size]>
  { using type = _Tp; };

  template <typename _Tp>
  struct remove_extent<_Tp[]>
  { using type = _Tp; };

  template <typename _Tp>
  using remove_extent_t = typename remove_extent<_Tp>::type;

  template <typename _Tp>
  struct remove_all_extents
  { using type = _Tp; };

  template <typename _Tp, std::size_t _Size>
  struct remove_all_extents<_Tp[_Size]>
  : remove_all_extents<_Tp> { };

  template <typename _Tp>
  struct remove_all_extents<_Tp[]>
  : remove_all_extents<_Tp> { };

  template <typename _Tp>
  using remove_all_extents_t = typename remove_all_extents<_Tp>::type;

  template <typename _Tp>
  struct remove_const 
  { using type = _Tp; };

  template <typename _Tp>
  struct remove_const<const _Tp>
  { using type = _Tp; };

  template <typename _Tp>
  using remove_const_t = typename remove_const<_Tp>::type;

  template <typename _Tp>
  struct remove_volatile
  { using type = _Tp; };

  template <typename _Tp>
  struct remove_volatile<volatile _Tp>
  { using type = _Tp; };

  template <typename _Tp>
  using remove_volatile_t = typename remove_volatile<_Tp>::type;

  template <typename _Tp>
  struct remove_cv
  { using type = typename remove_const<typename remove_volatile<_Tp>::type>::type; };

  template <typename _Tp>
  struct add_const
  { using type = const _Tp; };

  template <typename _Tp>
  struct add_volatile
  { using type = volatile _Tp; };

  template <typename _Tp>
  struct add_cv
  { using type = const volatile _Tp; };

  template <typename _Tp>
  using add_const_t = typename add_const<_Tp>::type;

  template <typename _Tp>
  using add_volatile_t = typename add_volatile<_Tp>::type;

  template <typename _Tp>
  using add_cv_t = typename add_cv<_Tp>::type;

  template <typename _Tp>
  using remove_cv_t = typename remove_cv<_Tp>::type;

  template <typename _Tp>
  struct remove_reference
  { using type = _Tp; };

  template <typename _Tp>
  struct remove_reference<_Tp&>
  { using type = _Tp; };

  template <typename _Tp>
  struct remove_reference<_Tp&&>
  { using type = _Tp; };

  template <typename _Tp>
  using remove_reference_t = typename remove_reference<_Tp>::type;

  template <typename _Tp>
  struct remove_cvref
  { using type = typename remove_cv<typename remove_reference<_Tp>::type>::type; };

  template <typename _Tp>
  using remove_cvref_t = typename remove_cvref<_Tp>::type;

  namespace detail
  {
    template <typename _Tp>
    struct __remove_pointer_helper
    { using type = _Tp; };

    template <typename _Tp>
    struct __remove_pointer_helper<_Tp*>
    { using type = _Tp; };
  }

  template <typename _Tp>
  struct remove_pointer
  : detail::__remove_pointer_helper<remove_cv_t<_Tp>> { };

  template <typename _Tp>
  using remove_pointer_t = typename remove_pointer<_Tp>::type;

  // rank
  template <typename _Tp>
  struct rank : integral_constant<std::size_t, 0> { };

  template <typename _Tp, std::size_t _Size>
  struct rank<_Tp[_Size]>
  : integral_constant<std::size_t, rank<_Tp>::value + 1> { };

  template <typename _Tp>
  struct rank<_Tp[]>
  : integral_constant<std::size_t, rank<_Tp>::value + 1> { };

  template <typename _Tp>
  inline constexpr std::size_t rank_v = rank<_Tp>::value;

  // extent
  template <typename _Tp, unsigned _Uint = 0>
  struct extent
  : integral_constant<std::size_t, 0> { };

  template <typename _Tp, std::size_t _Size>
  struct extent<_Tp[_Size], 0>
  : integral_constant<std::size_t, _Size> { };

  template <typename _Tp, unsigned _Uint, std::size_t _Size>
  struct extent<_Tp[_Size], _Uint>
  : extent<_Tp, _Uint - 1> { };

  template <typename _Tp>
  struct extent<_Tp[], 0>
  : integral_constant<std::size_t, 0> { };

  template <typename _Tp, unsigned _Uint>
  struct extent<_Tp[], _Uint>
  : extent<_Tp, _Uint - 1> { };

  template <typename _Tp, unsigned Uint = 0>
  inline constexpr std::size_t extent_v = extent<_Tp, Uint>::value;

  template <typename _Tp, typename _Up>
  struct is_same;

  template <typename _Tp>
  struct is_void;

  template <typename _From, typename _To>
  struct is_convertible;

  template <typename _Tp>
  struct is_null_pointer
  : is_same<std::nullptr_t, remove_cv_t<_Tp>> { };

  namespace detail
  {
    template <typename _Default, typename _AlwaysVoid,
              template <typename...> class _Op, typename ..._Args>
    struct __detector
    {
      using __is_detected = false_type; // value_t
      using type          = _Default;
    };

    template <typename _Default,
              template <typename...> class _Op, typename ..._Args>
    struct __detector<_Default, void_t<_Op<_Args...>>, _Op, _Args...>
    {
      using __is_detected = true_type;  // value_t
      using type          = _Op<_Args...>;
    };

    template <typename _Default,
              template <typename...> class _Op, typename ..._Args>
    using __detected_or = __detector<_Default, void, _Op, _Args...>;

    template <typename _Default,
              template <typename...> class _Op, typename ..._Args>
    using __detected_or_t = typename __detected_or<_Default, _Op, _Args...>::type;

    struct __nonesuchbase { };
    struct nonesuch : private __nonesuchbase
    {
      ~nonesuch() = delete;
      nonesuch(const nonesuch&) = delete;
      void operator=(const nonesuch&) = delete;
    };

  } // namespace detail

  template <typename _Default, template <typename...> class _Op, typename ..._Args>
  using detected_or = detail::__detected_or<_Default, _Op, _Args...>;

  template <typename _Default, template <typename...> class _Op, typename ..._Args>
  using detected_or_t = typename detected_or<_Default, _Op, _Args...>::type;

  // INFO: __is_detected is a bool_constant, usage: is_detected<...>::value
  template <template <typename...> class _Op, typename ..._Args>
  using is_detected = typename detected_or<void, _Op, _Args...>::__is_detected;

  template <template <typename...> class _Op, typename ..._Args>
  using detected_t = detected_or_t<detail::nonesuch, _Op, _Args...>;

  template <typename _Expected, template <typename...> class _Op, typename ..._Args>
  using is_detected_exact = is_same<_Expected, detected_t<_Op, _Args...>>;

  template <typename _To, template <typename...> class _Op, typename ..._Args>
  using is_detected_convertible = is_convertible<detected_t<_Op, _Args...>, _To>;

  // INFO: HAS_NESTED_TYPE macro is defined globally,
  // so it needs to be scope specified by swing::
  #define SWING_SCOPE swing::

  #define SWING_HAS_NESTED_TYPE(_NESTED)                                 \
  template <typename _Tp, typename = void>                               \
  struct __has_##_NESTED                                                 \
  : SWING_SCOPE false_type { };                                          \
  template <typename _Tp>                                                \
  struct __has_##_NESTED<_Tp, SWING_SCOPE void_t<typename _Tp::_NESTED>> \
  : SWING_SCOPE true_type { };                                           \
  template <typename _Tp>                                                \
  inline constexpr bool swing_has_##_NESTED(...)                         \
  { return __has_##_NESTED<_Tp>::value; }

  template <typename _Tp, typename ..._Types>
  struct is_one_of { };

  template <typename _Tp, typename ..._Types>
  struct is_one_of<_Tp, _Tp, _Types...>
  : true_type { };

  template <typename _Tp>
  struct is_one_of<_Tp>
  : false_type { };

  template <typename _Tp, typename _Up, typename ..._Types>
  struct is_one_of<_Tp, _Up, _Types...>
  : is_one_of<_Tp, _Types...> { };

#if __cplusplus > 201703L
#define SWING_CHAR8_T char8_t,
#else
#define SWING_CHAR8_T

#endif

#if __cplusplus > 202002L
#define SWING_CXX23_FLOAT \
, std::float16_t, std::float32_t, std::float64_t, std::float128_t, std::bfloat16_t
#else
#define SWING_CXX23_FLOAT

#endif

  template <typename _Tp>
  struct is_integral
  : is_one_of<remove_cv_t<_Tp>, bool,
              char, signed char, unsigned char,
              SWING_CHAR8_T char16_t, char32_t, wchar_t,
              short, unsigned short,
              int, unsigned int,
              long, unsigned long,
              long long, unsigned long long> { };

  template <typename _Tp>
  struct is_floating_point
  : is_one_of<remove_cv_t<_Tp>,
              float, double, long double SWING_CXX23_FLOAT> { };

  template <typename _Tp>
  struct is_arithmetic
  : disjunction<is_integral<_Tp>, is_floating_point<_Tp>> { };

  template <typename _Tp>
  struct is_fundamental
  : disjunction<is_null_pointer<_Tp>, is_void<_Tp>, is_arithmetic<_Tp>> { };

  template <typename _Tp>
  struct is_compound : negation<is_fundamental<_Tp>> { };

  template <typename _Tp>
  struct is_const : false_type { };

  template <typename _Tp>
  struct is_const<const _Tp> : true_type { };

  template <typename _Tp>
  struct is_volatile : false_type { };

  template <typename _Tp>
  struct is_volatile<volatile _Tp> : true_type { };

  template <typename _Tp, typename _Up>
  struct is_same : false_type { };

  template <typename _Tp>
  struct is_same<_Tp, _Tp> : true_type { };

  template <typename _Tp>
  struct is_void
#if __cplusplus > 201103L
  : is_same<void, remove_cv_t<_Tp>> { };

#else
  : false_type { };

  template <>
  struct is_void<void> : true_type { };

  template <>
  struct is_void<const void> : true_type { };

  template <>
  struct is_void<volatile void> : true_type { };

  template <>
  struct is_void<const volatile void> : true_type { };
#endif

  namespace detail
  {
    template <typename _Tp>
    struct __is_pointer_helper : false_type { };

    template <typename _Tp>
    struct __is_pointer_helper<_Tp*> : true_type { };
  }

  template <typename _Tp>
  struct is_pointer : detail::__is_pointer_helper<remove_cv_t<_Tp>> { };

  namespace detail
  {
    template <typename _Tp, typename = void>
    struct __add_pointer_helper
    { using type = _Tp; };

    template <typename _Tp>
    struct __add_pointer_helper<_Tp, void_t<_Tp*>>
    { using type = _Tp*; };

  }

  template <typename _Tp>
  struct is_reference
  : false_type { };

  template <typename _Tp>
  struct is_reference<_Tp&>
  : true_type { };

  template <typename _Tp>
  struct is_reference<_Tp&&>
  : true_type { };

  template <typename _Tp>
  struct is_lvalue_reference
  : false_type { };

  template <typename _Tp>
  struct is_lvalue_reference<_Tp&>
  : true_type { };

  template <typename _Tp>
  struct is_rvalue_reference
  : false_type { };

  template <typename _Tp>
  struct is_rvalue_reference<_Tp&&>
  : true_type { };

  template <typename _Tp, typename = void>
  struct add_lvalue_reference
  { using type = _Tp; };

  template <typename _Tp>
  struct add_lvalue_reference<_Tp, void_t<_Tp&>>
  { using type = _Tp&; };

  template <typename _Tp>
  using add_lvalue_reference_t = typename add_lvalue_reference<_Tp>::type;

  template <typename _Tp, typename = void>
  struct add_rvalue_reference
  { using type = _Tp; };

  template <typename _Tp>
  struct add_rvalue_reference<_Tp, void_t<_Tp&&>>
  { using type = _Tp&&; };

  template <typename _Tp>
  using add_rvalue_reference_t = typename add_rvalue_reference<_Tp>::type;

  template <typename _Tp>
  struct add_pointer
  : detail::__add_pointer_helper<remove_reference_t<_Tp>>{ };

  template <typename _Tp>
  using add_pointer_t = typename add_pointer<_Tp>::type;

  template <typename _Tp>
  struct is_array : false_type { };

  template <typename _Tp>
  struct is_array<_Tp[]> : true_type { };

  template <typename _Tp, std::size_t _Size>
  struct is_array<_Tp[_Size]> : true_type { };

  template <typename _Tp>
  struct is_bounded_array : false_type { };

  template <typename _Tp, std::size_t _Size>
  struct is_bounded_array<_Tp[_Size]> : true_type { };

  template <typename _Tp>
  struct is_unbounded_array : false_type { };

  template <typename _Tp>
  struct is_unbounded_array<_Tp[]> : true_type { };

  template <typename _Tp>
  struct is_function
  : bool_constant<
      !is_const<const _Tp>::value &&
      !is_reference<_Tp>::value> { };

  template <typename _Tp>
  struct is_union
  : bool_constant<__is_union(_Tp)> { };

  template <typename _Tp>
  struct is_enum
  : bool_constant<__is_enum(_Tp)> { };

  template <typename _Tp>
  struct is_empty
  : bool_constant<__is_empty(_Tp)> { };

  namespace detail
  {
    template <typename _Tp>
    bool_constant<!is_union<_Tp>::value> __is_class_helper(int _Tp::*);

    template <typename>
    false_type __is_class_helper(...);
  }

  template <typename _Tp>
  struct is_class
  : decltype(detail::__is_class_helper<_Tp>(nullptr)) { };

  namespace detail
  {
    template <typename _Base>
    true_type  __ptr_conversion_helper(const volatile _Base*);

    template <typename>
    false_type __ptr_conversion_helper(const volatile void*);

    template <typename _Base, typename _Derived>
    auto __is_base_of_helper(int) ->
      decltype(__ptr_conversion_helper<_Base>(static_cast<_Derived*>(nullptr)));

    // ambiguous and private
    template <typename, typename>
    auto __is_base_of_helper(...) -> true_type;
  }

  template <typename _Base, typename _Derived>
  struct is_base_of
  : bool_constant<
      conjunction<is_class<_Base>, is_class<_Derived>,
                  decltype(detail::__is_base_of_helper<_Base, _Derived>(0))>::value> { };

  namespace detail
  {
    // require type _To a legal return type
    // not array or function
    template <typename _To>
    auto __require_returnable(int) ->
      decltype(void(static_cast<_To(*)()>(nullptr)), true_type { });

    template <typename>
    auto __require_returnable(...) -> false_type;

    template <typename _From, typename _To>
    auto __require_implicit_conversion(int) ->
      decltype(void(declval<void(&)(_To)>()(declval<_From>())), true_type { });

    template <typename, typename>
    auto __require_implicit_conversion(...) -> false_type;

    template <typename _From, typename _To>
    struct __noexcept_implicit_convertion
    : bool_constant<noexcept(declval<void(&)(_To) noexcept>()(declval<_From>()))> { };
  }

  template <typename _From, typename _To>
  struct is_convertible
  : bool_constant<
      (decltype(detail::__require_returnable<_To>(0))::value &&
       decltype(detail::__require_implicit_conversion<_From, _To>(0))::value) ||
      (is_void<_From>::value && is_void<_To>::value)> { };

  template <typename _From, typename _To>
  struct is_nothrow_convertible
  : conjunction<
      is_convertible<_From, _To>,
      detail::__noexcept_implicit_convertion<_From, _To>> { };

  template <typename _ToElementType, typename _FromElementType>
  using __is_array_convertible
    = is_convertible<_FromElementType(*)[], _ToElementType(*)[]>;

  namespace detail
  {
    template <typename _Tp>
    struct __is_member_pointer_helper
    : false_type { };

    template <typename _Tp, typename _Cp>
    struct __is_member_pointer_helper<_Tp _Cp::*>
    : true_type { };

    template <typename _Tp>
    struct __is_member_function_pointer_helper
    : false_type { };

    template <typename _Tp, typename _Cp>
    struct __is_member_function_pointer_helper<_Tp _Cp::*>
    : is_function<_Tp>::type { };
  }

  template <typename _Tp>
  struct is_member_pointer
  : detail::__is_member_pointer_helper<remove_cv_t<_Tp>>::type
  { };

  template <typename _Tp>
  struct is_member_function_pointer
  : detail::__is_member_function_pointer_helper<remove_cv_t<_Tp>>::type
  { };

  template <typename _Tp>
  struct is_member_object_pointer
  : bool_constant<
      is_member_pointer<_Tp>::value &&
     !is_member_function_pointer<_Tp>::value> { };

  template <typename _Tp>
  struct is_scalar
  : disjunction<
      is_arithmetic<_Tp>, is_enum<_Tp>,
      is_pointer<_Tp>, is_null_pointer<_Tp>,
      is_member_pointer<_Tp>> { };

  template <typename _Tp>
  struct is_object
  : disjunction<
      is_scalar<_Tp>,
      is_array<_Tp>,
      is_union<_Tp>,
      is_class<_Tp>> { };

  namespace detail
  {
    template <typename _AlwaysVoid, typename _Tp, typename ..._Args>
    struct __is_constructible_impl : false_type { };

    template <typename _Tp, typename ..._Args>
    struct __is_constructible_impl<
      void_t<decltype(_Tp(declval<_Args>()...))>, _Tp, _Args...> : true_type { };

    template <typename _AlwaysVoid, typename _Tp, typename ..._Args>
    struct __is_constructible_helper : false_type { };

    template <typename _Tp, typename ..._Args>
    struct __is_constructible_helper<enable_if_t<
        disjunction<is_object<_Tp>, is_reference<_Tp>>::value>, _Tp, _Args...>
    : detail::__is_constructible_impl<void, _Tp, _Args...> { };
  }

  template <typename _Tp, typename ..._Args>
  struct is_constructible : bool_constant<__is_constructible(_Tp, _Args...)> { };

  // BUG: is_constructible<int&, float&> should be false
  // : detail::__is_constructible_helper<void, _Tp, _Args...> { };

  namespace detail
  {
    template <typename _Tp, typename _Up, typename = void>
    struct __try_assignment
    : false_type { };

    template <typename _Tp, typename _Up>
    struct __try_assignment<_Tp, _Up, void_t<decltype(declval<_Tp>() = declval<_Up>())>>
    : true_type { };

    template <typename _Tp, typename = void>
    struct __try_copy_assignment
    : false_type { };

    template <typename _Tp>
    struct __try_copy_assignment<_Tp, void_t<decltype(declval<_Tp&>() = declval<const _Tp&>())>>
    : true_type { };

    template <typename _Tp, typename = void>
    struct __try_move_assignment
    : false_type { };

    template <typename _Tp>
    struct __try_move_assignment<_Tp, void_t<decltype(declval<_Tp&>() = declval<_Tp&&>())>>
    : true_type { };
  }

#if __cplusplus > 201402L
  template <typename _Tp, typename _Up>
  struct is_assignable
  : detail::__try_assignment<_Tp, _Up>{ };

#endif

  template <typename _Tp>
  struct is_copy_assignable
#if   __cplusplus > 201402L
  : is_assignable<
      add_lvalue_reference_t<_Tp>,
      add_lvalue_reference_t<const _Tp>> { };

#elif __cplusplus > 201103L
  : detail::__try_copy_assignment<_Tp>::type { };
#else
  {
    private:
      template <typename _Up, typename = decltype(declval<_Up&>() = declval<const _Up&>())>
      static true_type try_assignment(_Up&&) noexcept;

      static false_type try_assignment(...) noexcept;

    public:
      using type = decltype(try_assignment(declval<_Tp>()));
      static constexpr bool value = type::value;
      using value_type = typename type::value_type;
      constexpr operator value_type() const noexcept { return value; }
      constexpr value_type operator ()() const noexcept { return value; };
  };

#endif

  template <typename _Tp>
  struct is_move_assignable
#if   __cplusplus > 201402L
  : is_assignable<
      add_lvalue_reference_t<_Tp>,
      add_rvalue_reference_t<_Tp>> { };
#elif __cplusplus > 201103L
  : detail::__try_move_assignment<_Tp>::type { };
#endif

  template <typename _Tp, typename _Up = remove_reference_t<_Tp>>
  struct decay
  {
    using type = conditional_t<
      is_array<_Up>::value,
      add_pointer_t<remove_extent_t<_Up>>,
      conditional_t<
        is_function<_Up>::value,
        add_pointer_t<_Up>, remove_cv_t<_Up>>>;
  };

  template <typename _Tp>
  using decay_t = typename decay<_Tp>::type;

  template <typename _Tp>
  struct __strip_reference_wrapper
  { using type = _Tp; };

  template <typename _Tp>
  struct __strip_reference_wrapper<swing::reference_wrapper<_Tp>>
  { using type = _Tp&; };

  template <typename _Tp>
  using __decay_and_strip = __strip_reference_wrapper<decay_t<_Tp>>;

  template <typename ..._Cond>
  using _Require = enable_if_t<conjunction<_Cond...>::value>;

  template <typename ...>
  struct common_type;

  template <typename _Tp>
  struct common_type<_Tp>
  : common_type<_Tp, _Tp> { };

  namespace detail
  {
    // declval will provide rvalue ref, decay is necessary
    template <typename _Tp, typename _Up>
    using __cond_t = decltype(declval<bool>() ? declval<_Tp>() : declval<_Up>());

    template <typename _Tp, typename _Up, typename = void>
    struct _decay_cond_t { };

    template <typename _Tp, typename _Up>
    struct _decay_cond_t<_Tp, _Up, void_t<__cond_t<_Tp, _Up>>>
    : decay<__cond_t<_Tp, _Up>> { };

    template <typename _Tp, typename _Up, typename = void>
    struct common_type_2_impl
    : _decay_cond_t<const _Tp&, const _Up&> { };

    template <typename _Tp, typename _Up>
    struct common_type_2_impl<_Tp, _Up, void_t<__cond_t<_Tp, _Up>>>
    : _decay_cond_t<_Tp, _Up> { };

    template <typename _AlwaysVoid, typename _Tp, typename _Up, typename ..._Rest>
    struct common_type_multi_impl { };

    template <typename _Tp, typename _Up, typename ..._Rest>
    struct common_type_multi_impl<void_t<typename common_type<_Tp, _Up>::type>, _Tp, _Up, _Rest...>
    : common_type<typename common_type<_Tp, _Up>::type, _Rest...> { };

  }

  template <typename _Tp, typename _Up>
  struct common_type<_Tp, _Up>
  : conditional_t<
      is_same<_Tp, decay_t<_Tp>>::value &&
      is_same<_Up, decay_t<_Up>>::value,
      detail::common_type_2_impl<_Tp, _Up>,
      common_type<decay_t<_Tp>, decay_t<_Up>>> { };

  template <typename _Tp, typename _Up, typename ..._Rest>
  struct common_type<_Tp, _Up, _Rest...>
  : detail::common_type_multi_impl<void, _Tp, _Up, _Rest...> { };

  template <typename ..._Types>
  using common_type_t = typename common_type<_Types...>::type;

  // TODO: C++20 std::common_reference


} // namespace swing

#endif // SWING_TYPE_TRAITS_HPP
