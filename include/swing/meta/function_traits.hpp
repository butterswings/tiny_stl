#ifndef SWING_FUNCTION_TRAITS_HPP
#define SWING_FUNCTION_TRAITS_HPP 1

#include <swing/private/function_table.hpp>
#include <swing/type_traits.hpp>

namespace swing
{

  namespace detail
  {
    template <typename ..._Types>
    struct __varargs_extractor
    { using type = params_wrapper<_Types...>; };

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wambiguous-ellipsis"
#endif

#if SWING_HAS_MACRO_FN_TRAITS == 1

    #define SWING_MACRO_FN_TRAITS_DETAIL(_PTR_REF, _EXCEPTION)      \
    template <typename _Ret, typename ..._Types>                    \
    struct __function_traits<_Ret _PTR_REF (_Types...) _EXCEPTION>  \
    {                                                               \
      using return_type = _Ret;                                     \
      using params_type = params_wrapper<_Types...>;                \
    };                                                              \
    template <typename _Ret, typename ..._Types>                          \
    struct __function_traits<_Ret _PTR_REF (_Types... ...) _EXCEPTION>    \
    {                                                                     \
      using return_type = _Ret;                                           \
      using params_type = typename __varargs_extractor<_Types...>::type;  \
    };

    #define SWING_MACRO_FN_TRAITS(_EXCEPTION)         \
      SWING_MACRO_FN_TRAITS_DETAIL(   ,  _EXCEPTION)  \
      SWING_MACRO_FN_TRAITS_DETAIL((*),  _EXCEPTION)  \
      SWING_MACRO_FN_TRAITS_DETAIL((&),  _EXCEPTION)  \
      SWING_MACRO_FN_TRAITS_DETAIL((&&), _EXCEPTION)

    SWING_MACRO_FN_TRAITS()
    SWING_MACRO_FN_TRAITS(noexcept)

#endif

#if SWING_HAS_MACRO_MEM_FN_TRAITS == 1

    #define SWING_MACRO_MEM_FN_TRAITS_DETAIL(_CV, _REF_EXCEPTION)         \
    template <typename _Ret, typename _Cp, typename ..._Types>            \
    struct __function_traits<_Ret(_Cp::*)(_Types...) _CV _REF_EXCEPTION>  \
    {                                                                     \
      using return_type = _Ret;                                           \
      using params_type = params_wrapper<_Types...>;                      \
      using class_type  = _CV _Cp;                                        \
    };                                                                    \
    template <typename _Ret, typename _Cp, typename ..._Types>                \
    struct __function_traits<_Ret(_Cp::*)(_Types... ...) _CV _REF_EXCEPTION>  \
    {                                                                         \
      using return_type = _Ret;                                               \
      using params_type = typename __varargs_extractor<_Types...>::type;      \
      using class_type  = _CV _Cp;                                            \
    };

    #define SWING_MACRO_MEM_FN_TRAITS(_REF_EXCEPTION)                  \
      SWING_MACRO_MEM_FN_TRAITS_DETAIL(   , _REF_EXCEPTION)            \
      SWING_MACRO_MEM_FN_TRAITS_DETAIL(const, _REF_EXCEPTION)          \
      SWING_MACRO_MEM_FN_TRAITS_DETAIL(volatile, _REF_EXCEPTION)       \
      SWING_MACRO_MEM_FN_TRAITS_DETAIL(const volatile, _REF_EXCEPTION)

    SWING_MACRO_MEM_FN_TRAITS()
    SWING_MACRO_MEM_FN_TRAITS(&)
    SWING_MACRO_MEM_FN_TRAITS(&&)

    SWING_MACRO_MEM_FN_TRAITS(noexcept)
    SWING_MACRO_MEM_FN_TRAITS(& noexcept)
    SWING_MACRO_MEM_FN_TRAITS(&& noexcept)

#endif

#ifdef __clang__
#pragma clang diagnostic pop
#endif
  } // namespace detail

  namespace meta
  {
    template <typename _Func, typename _Traits = detail::__function_traits<_Func>, typename = void>
    struct is_rvalue_specified
    : false_type { };
  } // namespace meta

  namespace detail
  {

    template <typename _Tp, bool = is_function<_Tp>::value>
    struct __function_pointer_traits
    { using type = decay_t<_Tp>; };

    template <typename _Tp, typename _Cp>
    struct __function_pointer_traits<_Tp _Cp::*, false>
    { using type = _Tp _Cp::*; };

    template <typename _Tp>
    struct __function_pointer_traits<_Tp, true>
    { using type = _Tp*; };

    template <typename _Func, typename _Cp, typename _Params , typename _Seq, typename = void>
    struct __is_lvalue_specified_detector
    : false_type { };

    template <typename _Func, typename _Cp, typename _Params, std::size_t ...I>
    struct __is_lvalue_specified_detector<
             _Func, _Cp, _Params, std::index_sequence<I...>,
             void_t<decltype((declval<_Cp&>().*declval<_Func>())
                             (declval<std::tuple_element_t<I, _Params>>()...))>> : true_type { };

    template <typename _Func, typename _Cp, typename _Params , typename _Seq, typename = void>
    struct __is_rvalue_specified_detector
    : false_type { };

    template <typename _Func, typename _Cp, typename _Params, std::size_t ...I>
    struct __is_rvalue_specified_detector<
             _Func, _Cp, _Params, std::index_sequence<I...>,
             void_t<decltype((declval<_Cp&&>().*declval<_Func>())
                             (declval<std::tuple_element_t<I, _Params>>()...))>> : true_type { };

    template <typename _Func, typename _Traits = __function_traits<_Func>>
    struct __is_lvalue_specified
    : __is_lvalue_specified_detector<
        decay_t<_Func>,
        typename _Traits::class_type,
        typename _Traits::params_type,
        std::make_index_sequence<std::tuple_size_v<typename _Traits::params_type>>> { };

    template <typename _Func, typename _Traits = __function_traits<_Func>>
    struct __is_rvalue_specified
    : __is_rvalue_specified_detector<
        decay_t<_Func>,
        typename _Traits::class_type,
        typename _Traits::params_type,
        std::make_index_sequence<std::tuple_size_v<typename _Traits::params_type>>> { };

    template <typename _Ret, typename _Cp, typename ..._Types, typename _Traits>
    struct __is_rvalue_specified<_Ret(_Cp::*)(_Types...) const &, _Traits>
    : false_type { };

    template <typename _Ret, typename _Cp, typename ..._Types, typename _Traits>
    struct __is_rvalue_specified<_Ret(_Cp::*)(_Types...) const & noexcept, _Traits>
    : false_type { };

    template <typename _Func, typename _Traits, typename = void>
    struct __class_type_wrapper { };

    template <typename _Func, typename _Traits>
    struct __class_type_wrapper<_Func, _Traits, void_t<typename _Traits::class_type>>
    { using type = typename _Traits::class_type; };

    /// @brief  meta function which is used to judge
    /// a function is noexcept specified or not
    /// @tparam _Func Original member function pointer
    /// @tparam _Wrapper Wrapper of member function's class type (_Cp)
    /// @tparam _Params std::tuple with parameters of the function
    /// @tparam _Seq std::index_sequence to spread Params...
    /// @tparam bool (is_member_function::value ? _Func(_Params...) : (_Cp.*_Func)(_Params...))
    /// @tparam bool (is_rvalue_specified::value ? _Cp&& : _Cp&)
    ///
    template <typename _Func, typename _Wrapper, typename _Params , typename _Seq,
              bool = is_member_function_pointer<_Func>::value,
              bool = meta::is_rvalue_specified<_Func>::value>
    struct __is_noexcept_specified;

    template <typename _Func, typename _Wrapper, typename _Params , std::size_t ...I>
    struct __is_noexcept_specified<_Func, _Wrapper, _Params, std::index_sequence<I...>, true, true>
    : bool_constant<
        noexcept((declval<typename _Wrapper::type&&>().*declval<_Func>())
                 (declval<std::tuple_element_t<I, _Params>>()...))> { };

    template <typename _Func, typename _Wrapper, typename _Params , std::size_t ...I>
    struct __is_noexcept_specified<_Func, _Wrapper, _Params, std::index_sequence<I...>, true, false>
    : bool_constant<
        noexcept((declval<typename _Wrapper::type&>().*declval<_Func>())
                 (declval<std::tuple_element_t<I, _Params>>()...))> { };

    template <typename _Func, typename _Wrapper, typename _Params , std::size_t ...I>
    struct __is_noexcept_specified<_Func, _Wrapper, _Params, std::index_sequence<I...>, false, false>
    : bool_constant<noexcept(declval<_Func>()(declval<std::tuple_element_t<I, _Params>>()...))> { };
  } // namespace detail

  namespace meta
  {
    using detail::__function_traits;
    using detail::__class_type_wrapper;
    using detail::__is_lvalue_specified;
    using detail::__is_rvalue_specified;
    using detail::__is_noexcept_specified;

    template <typename _Func, typename _Traits = __function_traits<_Func>, typename = void>
    struct is_const_specified
    : false_type { };

    template <typename _Func, typename _Traits>
    struct is_const_specified<_Func, _Traits, void_t<typename _Traits::class_type>>
    : is_const<typename _Traits::class_type> { };

    template <typename _Func, typename _Traits = __function_traits<_Func>, typename = void>
    struct is_volatile_specified
    : false_type { };

    template <typename _Func, typename _Traits>
    struct is_volatile_specified<_Func, _Traits, void_t<typename _Traits::class_type>>
    : is_volatile<typename _Traits::class_type> { };

    template <typename _Func, typename _Traits = __function_traits<_Func>, typename = void>
    struct is_non_reference_specified
    : true_type { };

    template <typename _Func, typename _Traits>
    struct is_non_reference_specified<_Func, _Traits, void_t<typename _Traits::class_type>>
    : bool_constant<!(__is_lvalue_specified<_Func>::value ^ __is_rvalue_specified<_Func>::value)> { };

    template <typename _Func, typename _Traits = __function_traits<_Func>, typename = void>
    struct is_lvalue_specified
    : false_type { };

    template <typename _Func, typename _Traits>
    struct is_lvalue_specified<_Func, _Traits, void_t<typename _Traits::class_type>>
    : bool_constant<is_non_reference_specified<_Func>::value ^ __is_lvalue_specified<_Func>::value> { };

    template <typename _Func, typename _Traits>
    struct is_rvalue_specified<_Func, _Traits, void_t<typename _Traits::class_type>>
    : bool_constant<is_non_reference_specified<_Func>::value ^ __is_rvalue_specified<_Func>::value> { };

    template <typename _Func, typename _Traits = __function_traits<_Func>>
    struct is_noexcept_specified
    : __is_noexcept_specified<
        decay_t<_Func>,
        __class_type_wrapper<_Func, _Traits>,
        typename _Traits::params_type,
        std::make_index_sequence<std::tuple_size_v<typename _Traits::params_type>>> { };

    template <typename _Tp>
    struct function_traits
    : detail::__function_traits<_Tp>
    {
      using pointer       = typename detail::__function_pointer_traits<_Tp>::type;
      using function_type = remove_pointer_t<pointer>;
    };

    template <auto _Fn>
    using function_traits_t = function_traits<decltype(_Fn)>;

  } // namespace meta

} // namespace swing

#endif // SWING_FUNCTION_TRAITS_HPP
