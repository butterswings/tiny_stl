#ifndef SWING_PRIVATE_ITERATOR_BASE_TYPE_HPP
#define SWING_PRIVATE_ITERATOR_BASE_TYPE_HPP 1

#include <swing/type_traits.hpp>

namespace swing
{
// iterator categories
struct input_iterator_tag { };
struct output_iterator_tag { };

struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };

#if __cplusplus > 201703L
struct contiguous_iterator_tag : public random_access_iterator_tag { };

#endif

template <typename _Category, typename _Tp, typename _Distance = ptrdiff_t,
          typename _Pointer = _Tp*, typename _Reference = _Tp&>
struct iterator
{
  using iterator_category = _Category;
  using value_type        = _Tp;
  using difference_type   = _Distance;
  using pointer           = _Pointer;
  using reference         = _Reference;
};

template <typename _Iterator, typename = void>
struct __iterator_traits;

template <typename _Iterator>
struct __iterator_traits<_Iterator,
       void_t<typename _Iterator::iterator_category,
              typename _Iterator::value_type,
              typename _Iterator::difference_type,
              typename _Iterator::pointer,
              typename _Iterator::reference>>
{
  using iterator_category = typename _Iterator::iterator_category;
  using value_type        = typename _Iterator::value_type;
  using difference_type   = typename _Iterator::difference_type;
  using pointer           = typename _Iterator::pointer;
  using reference         = typename _Iterator::reference;
};

template <typename _Iterator>
struct iterator_traits
 : __iterator_traits<_Iterator> { };

#if __cplusplus > 201703L
// partial specialization for object pointer types
template <typename _Tp>
requires swing::is_object<_Tp>::value 
struct iterator_traits<_Tp*>
{
  using iterator_concept  = contiguous_iterator_tag;
  using iterator_category = random_access_iterator_tag;
  using value_type        = remove_cv_t<_Tp>;
  using difference_type   = ptrdiff_t;
  using pointer           = _Tp*;
  using reference         = _Tp&;
};

#else

template <typename _Tp>
struct iterator_traits<_Tp*>
{
  using iterator_category = random_access_iterator_tag;
  using value_type        = _Tp;
  using difference_type   = ptrdiff_t;
  using pointer           = _Tp*;
  using reference         = _Tp&;
};

template <typename _Tp>
struct iterator_traits<const _Tp*>
{
  using iterator_category = random_access_iterator_tag;
  using value_type        = _Tp;
  using difference_type   = ptrdiff_t;
  using pointer           = const _Tp*;
  using reference         = const _Tp&;
};

#endif

template <typename _Iter>
inline constexpr typename iterator_traits<_Iter>::iterator_category
__iterator_category(const _Iter&)
{ return typename iterator_traits<_Iter>::iterator_category(); }

template <typename _Iter>
using __iterator_category_t
  = typename iterator_traits<_Iter>::iterator_category;

template <typename _InIter>
using _RequireInputIter
  = enable_if_t<
      is_convertible<__iterator_category_t<_InIter>, input_iterator_tag>::value>;

template <typename _It, 
          typename _Cat = typename __iterator_traits<_It>::iterator_category>
struct __is_random_acess_iter
 : is_base_of<random_access_iterator_tag, _Cat> { };

} // namespace swing

#endif // SWING_PRIVATE_ITERATOR_BASE_TYPE_HPP
