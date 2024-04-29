#ifndef SWING_PRIVATE_FUNCTION_TABLE
#define SWING_PRIVATE_FUNCTION_TABLE 1

#define SWING_HAS_MACRO_FN_TRAITS 1
#define SWING_HAS_MACRO_MEM_FN_TRAITS 1

#include <tuple>

namespace swing
{
  namespace detail
  {

    template <typename _Tp>
    struct __function_traits
    {
      static_assert(false, "This type is not any of the following types: function type, "
                           "function pointer type, function reference type, "
                           "or member function pointer type");
    };
    template <typename ..._Types>
    using params_wrapper = std::tuple<_Types...>;

#if SWING_HAS_MACRO_FN_TRAITS == 0

    template <typename _Ret, typename ..._Types>
    struct __function_traits<_Ret(_Types...)>
    {
        using return_type = _Ret;
        using params_type = params_wrapper<_Types...>;
    };

    template <typename _Ret, typename ..._Types>
    struct __function_traits<_Ret(*)(_Types...)>
    {
        using return_type = _Ret;
        using params_type = params_wrapper<_Types...>;
    };

    template <typename _Ret, typename ..._Types>
    struct __function_traits<_Ret(&)(_Types...)>
    {
        using return_type = _Ret;
        using params_type = params_wrapper<_Types...>;
    };

    template <typename _Ret, typename ..._Types>
    struct __function_traits<_Ret(&&)(_Types...)>
    {
        using return_type = _Ret;
        using params_type = params_wrapper<_Types...>;
    };

    template <typename _Ret, typename ..._Types>
    struct __function_traits<_Ret(_Types...) noexcept>
    {
        using return_type = _Ret;
        using params_type = params_wrapper<_Types...>;
    };

    template <typename _Ret, typename ..._Types>
    struct __function_traits<_Ret(*)(_Types...) noexcept>
    {
        using return_type = _Ret;
        using params_type = params_wrapper<_Types...>;
    };

    template <typename _Ret, typename ..._Types>
    struct __function_traits<_Ret(&)(_Types...) noexcept>
    {
        using return_type = _Ret;
        using params_type = params_wrapper<_Types...>;
    };

    template <typename _Ret, typename ..._Types>
    struct __function_traits<_Ret(&&)(_Types...) noexcept>
    {
        using return_type = _Ret;
        using params_type = params_wrapper<_Types...>;
    };

#endif

#if SWING_HAS_MACRO_MEM_FN_TRAITS == 0

    template <typename _Ret, typename _Cp, typename ..._Types>
    struct __function_traits<_Ret(_Cp::*)(_Types...)>
    {
        using return_type = _Ret;
        using params_type = params_wrapper<_Types...>;
        using class_type  = _Cp;
    };

    template <typename _Ret, typename _Cp, typename ..._Types>
    struct __function_traits<_Ret(_Cp::*)(_Types...) const>
    {
        using return_type = _Ret;
        using params_type = params_wrapper<_Types...>;
        using class_type  = const _Cp;
    };

    template <typename _Ret, typename _Cp, typename ..._Types>
    struct __function_traits<_Ret(_Cp::*)(_Types...) volatile>
    {
        using return_type = _Ret;
        using params_type = params_wrapper<_Types...>;
        using class_type  = volatile _Cp;
    };

    template <typename _Ret, typename _Cp, typename ..._Types>
    struct __function_traits<_Ret(_Cp::*)(_Types...) const volatile>
    {
        using return_type = _Ret;
        using params_type = params_wrapper<_Types...>;
        using class_type  = const volatile _Cp;
    };

    template <typename _Ret, typename _Cp, typename ..._Types>
    struct __function_traits<_Ret(_Cp::*)(_Types...) &>
    {
        using return_type = _Ret;
        using params_type = params_wrapper<_Types...>;
        using class_type  = _Cp;
    };

    template <typename _Ret, typename _Cp, typename ..._Types>
    struct __function_traits<_Ret(_Cp::*)(_Types...) const &>
    {
        using return_type = _Ret;
        using params_type = params_wrapper<_Types...>;
        using class_type  = const _Cp;
    };

    template <typename _Ret, typename _Cp, typename ..._Types>
    struct __function_traits<_Ret(_Cp::*)(_Types...) volatile &>
    {
        using return_type = _Ret;
        using params_type = params_wrapper<_Types...>;
        using class_type  = volatile _Cp;
    };

    template <typename _Ret, typename _Cp, typename ..._Types>
    struct __function_traits<_Ret(_Cp::*)(_Types...) const volatile &>
    {
        using return_type = _Ret;
        using params_type = params_wrapper<_Types...>;
        using class_type  = const volatile _Cp;
    };

    template <typename _Ret, typename _Cp, typename ..._Types>
    struct __function_traits<_Ret(_Cp::*)(_Types...) &&>
    {
        using return_type = _Ret;
        using params_type = params_wrapper<_Types...>;
        using class_type  = _Cp;
    };

    template <typename _Ret, typename _Cp, typename ..._Types>
    struct __function_traits<_Ret(_Cp::*)(_Types...) const &&>
    {
        using return_type = _Ret;
        using params_type = params_wrapper<_Types...>;
        using class_type  = const _Cp;
    };

    template <typename _Ret, typename _Cp, typename ..._Types>
    struct __function_traits<_Ret(_Cp::*)(_Types...) volatile &&>
    {
        using return_type = _Ret;
        using params_type = params_wrapper<_Types...>;
        using class_type  = volatile _Cp;
    };

    template <typename _Ret, typename _Cp, typename ..._Types>
    struct __function_traits<_Ret(_Cp::*)(_Types...) const volatile &&>
    {
        using return_type = _Ret;
        using params_type = params_wrapper<_Types...>;
        using class_type  = const volatile _Cp;
    };

    template <typename _Ret, typename _Cp, typename ..._Types>
    struct __function_traits<_Ret(_Cp::*)(_Types...) noexcept>
    {
        using return_type = _Ret;
        using params_type = params_wrapper<_Types...>;
        using class_type  = _Cp;
    };

    template <typename _Ret, typename _Cp, typename ..._Types>
    struct __function_traits<_Ret(_Cp::*)(_Types...) const noexcept>
    {
        using return_type = _Ret;
        using params_type = params_wrapper<_Types...>;
        using class_type  = const _Cp;
    };

    template <typename _Ret, typename _Cp, typename ..._Types>
    struct __function_traits<_Ret(_Cp::*)(_Types...) volatile noexcept>
    {
        using return_type = _Ret;
        using params_type = params_wrapper<_Types...>;
        using class_type  = volatile _Cp;
    };

    template <typename _Ret, typename _Cp, typename ..._Types>
    struct __function_traits<_Ret(_Cp::*)(_Types...) const volatile noexcept>
    {
        using return_type = _Ret;
        using params_type = params_wrapper<_Types...>;
        using class_type  = const volatile _Cp;
    };

    template <typename _Ret, typename _Cp, typename ..._Types>
    struct __function_traits<_Ret(_Cp::*)(_Types...) & noexcept>
    {
        using return_type = _Ret;
        using params_type = params_wrapper<_Types...>;
        using class_type  = _Cp;
    };

    template <typename _Ret, typename _Cp, typename ..._Types>
    struct __function_traits<_Ret(_Cp::*)(_Types...) const & noexcept>
    {
        using return_type = _Ret;
        using params_type = params_wrapper<_Types...>;
        using class_type  = const _Cp;
    };

    template <typename _Ret, typename _Cp, typename ..._Types>
    struct __function_traits<_Ret(_Cp::*)(_Types...) volatile & noexcept>
    {
        using return_type = _Ret;
        using params_type = params_wrapper<_Types...>;
        using class_type  = volatile _Cp;
    };

    template <typename _Ret, typename _Cp, typename ..._Types>
    struct __function_traits<_Ret(_Cp::*)(_Types...) const volatile & noexcept>
    {
        using return_type = _Ret;
        using params_type = params_wrapper<_Types...>;
        using class_type  = const volatile _Cp;
    };

    template <typename _Ret, typename _Cp, typename ..._Types>
    struct __function_traits<_Ret(_Cp::*)(_Types...) && noexcept>
    {
        using return_type = _Ret;
        using params_type = params_wrapper<_Types...>;
        using class_type  = _Cp;
    };

    template <typename _Ret, typename _Cp, typename ..._Types>
    struct __function_traits<_Ret(_Cp::*)(_Types...) const && noexcept>
    {
        using return_type = _Ret;
        using params_type = params_wrapper<_Types...>;
        using class_type  = const _Cp;
    };

    template <typename _Ret, typename _Cp, typename ..._Types>
    struct __function_traits<_Ret(_Cp::*)(_Types...) volatile && noexcept>
    {
        using return_type = _Ret;
        using params_type = params_wrapper<_Types...>;
        using class_type  = volatile _Cp;
    };

    template <typename _Ret, typename _Cp, typename ..._Types>
    struct __function_traits<_Ret(_Cp::*)(_Types...) const volatile && noexcept>
    {
        using return_type = _Ret;
        using params_type = params_wrapper<_Types...>;
        using class_type  = const volatile _Cp;
    };

#endif
  } // namespace detail
} // namespace swing



#endif
