# `<type_traits>`

<details open>
<summary><h2>conditional</h2></summary>

```cpp
template <bool, typename _If, typename>
struct conditional
{ using type = _If; };

template <typename _If, typename _Else>
struct conditional<false, _If, _Else>
{ using type = _Else; };

// alias
template <bool _Cond, typename _If, typename _Else>
using conditional_t = typename conditional<_Cond, _If, _Else>::type;
```

</details>

<details open>
<summary><h2>declval</h2></summary>

```cpp
namespace detail
{
  template <typename _Tp, typename _Up = _Tp&&>
  _Up __declval(int);

  // optional cv-qualifier void
  template <typename _Tp>
  _Tp __declval(long);
}

template <typename _Tp>
auto declval() noexcept
-> decltype(detail::__declval<_Tp>(0))
{
  static_assert(false, "declval should not be used");
  return detail::__declval<_Tp>(0);
}
```

</details>

<details open>
<summary><h2>is_class</h2></summary>

```cpp
template <typename _Tp>
bool_constant<!is_union<_Tp>::value>
__is_class_helper(int _Tp::*);

template <typename>
false_type __is_class_helper(...);

template <typename _Tp>
struct is_class : decltype(__is_class_helper<_Tp>(nullptr)) { };
```

</details>

<details open>
<summary><h2>common_type</h2></summary>

```cpp
template <typename ...>
struct common_type;

template <typename _Tp>
struct common_type<_Tp>
: common_type<_Tp, _Tp> { };

template <typename _Tp, typename _Up>
using __cond_t =
  decltype(declval<bool>() ? declval<_Tp>() : declval<_Up>());

template <typename _Tp, typename _Up, typename = void>
struct __decay_cond_t { };

template <typename _Tp, typename _Up>
struct __decay_cond_t<_Tp, _Up, void_t<__cond_t<_Tp, _Up>>>
: decay<__cond_t<_Tp, _Up>> { };

template <typename _Tp, typename _Up, typename = void>
struct common_type_2_impl
: __decay_cond_t<const _Tp&, const _Up&> { };

template <typename _Tp, typename _Up>
struct common_type_2_impl<_Tp, _Up, void_t<__cond_t<_Tp, _Up>>>
: __decay_cond_t<_Tp, _Up> { };

template <typename _Tp, typename _Up>
struct common_type<_Tp, _Up>
: conditional_t<
  is_same<_Tp, decay_t<_Tp>>::value &&
  is_same<_Up, decay_t<_Up>>::value,
  common_type_2_impl<_Tp, _Up>,
  common_type<decay_t<_Tp>, decay_t<_Up>>> { };

template <typename _Tp, typename _Up, typename... _Types>
struct common_type<_Tp, _Up, _Types...>
: common_type<typename common_type<_Tp, _Up>::type, _Types...> { };
```

</details>
