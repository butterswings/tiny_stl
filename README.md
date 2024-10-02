# tiny_stl

## `<type_traits>`

### common_type

```cpp
template <typename ...>
struct common_type;

template <typename _Tp>
struct common_type<_Tp>
: common_type<_Tp, _Tp> { };

template <typename _Tp, typename _Up>
using __cond_t =
  decltype(swing::declval<bool>() ? swing::declval<_Tp>() : swing::declval<_Up>());

template <typename _Tp, typename _Up, typename = void>
struct __decay_cond_t { };

template <typename _Tp, typename _Up>
struct __decay_cond_t<_Tp, _Up, swing::void_t<__cond_t<_Tp, _Up>>>
: swing::decay<__cond_t<_Tp, _Up>> { };

template <typename _Tp, typename _Up, typename = void>
struct common_type_2_impl
: __decay_cond_t<const _Tp&, const _Up&> { };

template <typename _Tp, typename _Up>
struct common_type_2_impl<_Tp, _Up, swing::void_t<__cond_t<_Tp, _Up>>>
: __decay_cond_t<_Tp, _Up> { };

template <typename _Tp, typename _Up>
struct common_type<_Tp, _Up>
: swing::conditional_t<
  swing::is_same<_Tp, swing::decay_t<_Tp>>::value &&
  swing::is_same<_Up, swing::decay_t<_Up>>::value,
  common_type_2_impl<_Tp, _Up>,
  common_type<swing::decay_t<_Tp>, swing::decay_t<_Up>>> { };

template <typename _Tp, typename _Up, typename... _Types>
struct common_type<_Tp, _Up, _Types...>
: common_type<typename common_type<_Tp, _Up>::type, _Types...> { };
```

## `<concepts>`

### same_as

### derived_from

### convertible_to
