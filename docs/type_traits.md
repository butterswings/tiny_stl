# `<type_traits>`

<details open>
<summary><h2>is_reference</h2></summary>

```cpp
template <typename _Tp>
struct is_reference
: false_type { };

template <typename _Tp>
struct is_reference<_Tp&>
: true_type { };

template <typename _Tp>
struct is_reference<_Tp&&>
: true_type { };
```

</details>

- 若类型满足特化`_Tp&`或`_Tp&&`则`is_reference<_Tp>::value`为`true`

<details open>
<summary><h2>is_lvalue_reference</h2></summary>

```cpp
template <typename _Tp>
struct is_lvalue_reference
: false_type { };

template <typename _Tp>
struct is_lvalue_reference<_Tp&>
: true_type { };
```

</details>

- 若类型满足特化`_Tp&`则`is_reference<_Tp>::value`为`true`

<details open>
<summary><h2>is_rvalue_reference</h2></summary>

```cpp
template <typename _Tp>
struct is_rvalue_reference
: false_type { };

template <typename _Tp>
struct is_rvalue_reference<_Tp&&>
: true_type { };
```

</details>

- 若类型满足特化`_Tp&&`则`is_reference<_Tp>::value`为`true`

<details open>
<summary><h2>is_const</h2></summary>

```cpp
template <typename _Tp>
struct is_const
: false_type { };

template <typename _Tp>
struct is_const<const _Tp>
: true_type { };
```

</details>

- 若类型满足特化`const _Tp`则`is_const<_Tp>::value`为`true`

<details open>
<summary><h2>is_function</h2></summary>

```cpp
template <typename _Tp>
struct is_function
: bool_constant<
  !is_const<const _Tp>::value &&
  !is_reference<_Tp>::value> { };
```

</details>

> 在C++中仅有两种类型add_const后得到的类型不满足static_assert(is_const<add_const_t<_Tp>>::value)

- 因此基于此特性可以实现精简的***is_funtion*** -> `!is_const<const _Tp>::value && !is_reference<_Tp>::value`

- 如果要进行反射，则需要偏特化打表，类似见[funtion_traits](../include/swing/meta/function_traits.hpp)

<details open>
<summary><h2>conditional</h2></summary>

<ul>
<li>
<details open>
<summary>实现一</summary>

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
</li>

<li>
<details open>
<summary>实现二</summary>

```cpp
template <bool>
struct conditional
{
  template <typename _If, typename>
  using type = _If;
};

template <>
struct conditional<false>
{
  template <typename, typename _Else>
  using type = _Else;
};

template <bool _Cond, typename _If, typename _Else>
using conditional_t = typename conditional<_Cond>::template type<_If, _Else>;
```

</details>
</li>
</ul>

</details>

> 编译期if，其中实现二的conditional中的类型别名type为模板，再为其编写alias时需要显式指明`template type<_If, _Else>`

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

- notice
  1. `declval`用于不求值语境(类型推导，TMP，...)，不得直接使用，如果被*ODR*使用，则程序`ill-formed`
  2. others: 可以未经构造进行`member-function call`的推导
- implementation
  1. (***optional*** `cv-qualified`) `void`得到原本类型
  2. 对其余类型添加右值引用，可能产生引用折叠(***reference collapsing***)

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

- 若nullptr能够向 `any _Tp::*` 转化，并且类型_Tp不是union类型，则符合type traits is_class

<details open>
<summary><h2>decay</h2></summary>

```cpp
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
```

</details>

对于函数和数组类型，转换到其指针；对于一般类型去除引用和cv限定

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
