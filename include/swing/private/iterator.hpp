// DR179. https://cplusplus.github.io/LWG/issue179
// DR685. https://cplusplus.github.io/LWG/issue685

#ifndef SWING_PRIVATE_ITERATOR_HPP
#define SWING_PRIVATE_ITERATOR_HPP 1

#include <swing/private/cxx_config.h>
#include <swing/private/iterator_base_type.hpp>
#include <swing/type_traits.hpp>

namespace swing
{
  template <typename _Iterator>
  class reverse_iterator
  : public iterator<
    typename iterator_traits<_Iterator>::iterator_category,
    typename iterator_traits<_Iterator>::value_type,
    typename iterator_traits<_Iterator>::difference_type,
    typename iterator_traits<_Iterator>::pointer,
    typename iterator_traits<_Iterator>::reference>
  {
    template <typename _Iter>
    friend class reverse_iterator;

  protected:
    _Iterator current;

    using __traits_type = iterator_traits<_Iterator>;

  public:
    using iterator_type     = _Iterator;

    // NOTE: iterator_category and value_type are traits
    // from concepts in implementation of libstdc++ CXX20
    //
    // before instance of the template, the inherited types are invisible,
    // so declare traits types which will be used in the reverse_iterator class
    using iterator_category = typename __traits_type::iterator_category;
    using value_type        = typename __traits_type::value_type;

    // declared before CXX20 in libstdc++
    using difference_type   = typename __traits_type::difference_type;
    using pointer           = typename __traits_type::pointer;
    using reference         = typename __traits_type::reference;

    // default constructor
    SWING_CXX17_CONSTEXPR
    reverse_iterator() SWING_NOEXCEPT_IF(noexcept(_Iterator()))
    : current()
    { }

    // convert construction
    explicit SWING_CXX17_CONSTEXPR
    reverse_iterator(iterator_type __x)
    SWING_NOEXCEPT_IF(noexcept(_Iterator(__x)))
    : current(__x) { }

    // normal copy constructor
    SWING_CXX17_CONSTEXPR
    reverse_iterator(const reverse_iterator& __x)
    SWING_NOEXCEPT_IF(noexcept(_Iterator(__x.current)))
    : current(__x.current) { }

    reverse_iterator& operator=(const reverse_iterator&) = default;

    template <typename _Iter>
#if __cplusplus > 201703L
    // convertible
#endif
    SWING_CXX17_CONSTEXPR
    reverse_iterator(const reverse_iterator<_Iter>& __x)
    SWING_NOEXCEPT_IF(noexcept(_Iterator(__x.current)))
    : current(__x.current) { }

    template <typename _Iter>
#if __cplusplus > 201703L
    // convertible and assignable
#endif
    SWING_CXX17_CONSTEXPR
    reverse_iterator&
    operator=(const reverse_iterator<_Iter>& __x)
    SWING_NOEXCEPT_IF(noexcept(current = __x.current))
    {
      current = __x.current;
      return *this;
    }

    SWING_NODISCARD
    SWING_CXX17_CONSTEXPR iterator_type
    base() const
    SWING_NOEXCEPT_IF(noexcept(_Iterator(current)))
    { return current; }

    SWING_NODISCARD
    SWING_CXX17_CONSTEXPR reference
    operator*() const
    {
      _Iterator __tmp = current;
      return *--__tmp;
    }

    SWING_NODISCARD
    SWING_CXX17_CONSTEXPR pointer
    operator->() const
#if __cplusplus > 201703L
    requires is_pointer<_Iterator>::value
    || requires(const _Iterator __i) { __i.operator->(); }
#endif
    {
      _Iterator __tmp = current;
      --__tmp;
      if constexpr (is_pointer<_Iterator>::value)
        return __tmp;
      else
        return __tmp.operator->();
    }

    SWING_CXX17_CONSTEXPR
    reverse_iterator&
    operator++()
    {
      --current;
      return *this;
    }

    SWING_CXX17_CONSTEXPR
    reverse_iterator
    operator++(int)
    {
      reverse_iterator __tmp = *this;
      --current;
      return __tmp;
    }

    SWING_CXX17_CONSTEXPR
    reverse_iterator&
    operator--()
    {
      ++current;
      return *this;
    }

    SWING_CXX17_CONSTEXPR
    reverse_iterator
    operator--(int)
    {
      reverse_iterator __tmp = *this;
      ++current;
      return __tmp;
    }

    SWING_NODISCARD SWING_CXX17_CONSTEXPR
    reverse_iterator
    operator+(difference_type __n) const
    { return reverse_iterator(current - __n); }

    SWING_CXX17_CONSTEXPR
    reverse_iterator&
    operator+=(difference_type __n)
    {
      current -= __n;
      return *this;
    }

    SWING_NODISCARD SWING_CXX17_CONSTEXPR
    reverse_iterator
    operator-(difference_type __n) const
    { return reverse_iterator(current + __n); }

    SWING_CXX17_CONSTEXPR
    reverse_iterator&
    operator-=(difference_type __n)
    {
      current += __n;
      return *this;
    }

    // random access is required
    SWING_NODISCARD SWING_CXX17_CONSTEXPR
    reference
    operator[](difference_type __n) const
    { return *(*this + __n); } // ? equal to current[__n]

    // TODO: C++20 iter_move and iter_swap

  };
#if __cplusplus > 201703L
  // TODO: C++20 reverse_iterator three-way comparison
#else
  template <typename _Iterator>
  SWING_NODISCARD SWING_CXX17_CONSTEXPR
  inline bool
  operator==(const reverse_iterator<_Iterator>& __x,
             const reverse_iterator<_Iterator>& __y)
  { return __x.base() == __y.base(); }

  template <typename _Iterator>
  SWING_NODISCARD SWING_CXX17_CONSTEXPR
  inline bool
  operator<(const reverse_iterator<_Iterator>& __x,
            const reverse_iterator<_Iterator>& __y)
  { return __x.base() < __y.base(); }

  template <typename _Iterator>
  SWING_NODISCARD SWING_CXX17_CONSTEXPR
  inline bool
  operator!=(const reverse_iterator<_Iterator>& __x,
             const reverse_iterator<_Iterator>& __y)
  { return !(__x == __y); }

  template <typename _Iterator>
  SWING_NODISCARD SWING_CXX17_CONSTEXPR
  inline bool
  operator>(const reverse_iterator<_Iterator>& __x,
            const reverse_iterator<_Iterator>& __y)
  { return __y < __x; }

  template <typename _Iterator>
  SWING_NODISCARD SWING_CXX17_CONSTEXPR
  inline bool
  operator<=(const reverse_iterator<_Iterator>& __x,
             const reverse_iterator<_Iterator>& __y)
  { return !(__y < __x); }

  template <typename _Iterator>
  SWING_NODISCARD SWING_CXX17_CONSTEXPR
  inline bool
  operator>=(const reverse_iterator<_Iterator>& __x,
             const reverse_iterator<_Iterator>& __y)
  { return !(__x < __y); }

  // NOTE: comparison for const iterator and const iterator
  // reverse_iterator<const _Tp*> and reverse_iterator<_Tp*>
  // only providing the two template param version is also OK
  // DR179.
  template <typename _IteratorL, typename _IteratorR>
  SWING_NODISCARD SWING_CXX17_CONSTEXPR
  inline bool
  operator==(const reverse_iterator<_IteratorL>& __x,
             const reverse_iterator<_IteratorR>& __y)
  { return __x.base() == __y.base(); }

  template <typename _IteratorL, typename _IteratorR>
  SWING_NODISCARD SWING_CXX17_CONSTEXPR
  inline bool
  operator<(const reverse_iterator<_IteratorL>& __x,
            const reverse_iterator<_IteratorR>& __y)
  { return __x.base() < __y.base(); }

  template <typename _IteratorL, typename _IteratorR>
  SWING_NODISCARD SWING_CXX17_CONSTEXPR
  inline bool
  operator!=(const reverse_iterator<_IteratorL>& __x,
             const reverse_iterator<_IteratorR>& __y)
  { return __x.base() != __y.base(); }

  template <typename _IteratorL, typename _IteratorR>
  SWING_NODISCARD SWING_CXX17_CONSTEXPR
  inline bool
  operator<=(const reverse_iterator<_IteratorL>& __x,
             const reverse_iterator<_IteratorR>& __y)
  { return __x.base() <= __y.base(); }

  template <typename _IteratorL, typename _IteratorR>
  SWING_NODISCARD SWING_CXX17_CONSTEXPR
  inline bool
  operator>=(const reverse_iterator<_IteratorL>& __x,
             const reverse_iterator<_IteratorR>& __y)
  { return __x.base() >= __y.base(); }

  // DR685. const and non-const iterator
  // only two template param version is OK
  // no need to provide another same type overload
  template <typename _IteratorL, typename _IteratorR>
  SWING_NODISCARD SWING_CXX17_CONSTEXPR
  inline auto
  operator-(const reverse_iterator<_IteratorL>& __x,
            const reverse_iterator<_IteratorR>& __y)
  -> decltype(__y.base() - __x.base())
  { return __y.base() - __x.base(); }

  template <typename _Iterator>
  SWING_NODISCARD SWING_CXX17_CONSTEXPR
  inline reverse_iterator<_Iterator>
  operator+(typename reverse_iterator<_Iterator>::difference_type __n,
            const reverse_iterator<_Iterator>& __i)
  { return reverse_iterator<_Iterator>(__i.base() - __n); }

#endif

  // TODO: other functions related to reverse_iterator

} // namespace swing

namespace cxx
{
  /**
    * @class  __normal_iterator
    * @brief  wrapper for raw pointers
    * @tparam _Iterator raw pointer type
    * @tparam _Container unique tag for containers
    *
    * NOTE: const member funtions operator*, ->, [], -, +, base
    */
  template <typename _Iterator, typename _Container>
  class __normal_iterator
  {
  protected:
    _Iterator _M_current;
    using __traits_type = swing::iterator_traits<_Iterator>;

    template <typename _Iter>
    using __convertible_from = swing::enable_if_t<swing::is_convertible<_Iter, _Iterator>::value>;

  public:
    using iterator_type     = _Iterator;

    // traits from iterator_traits
    using iterator_category = typename __traits_type::iterator_category;
    using value_type        = typename __traits_type::value_type;
    using difference_type   = typename __traits_type::difference_type;
    using pointer           = typename __traits_type::pointer;
    using reference         = typename __traits_type::reference;

  public:

    // NOTE: only default constructor is default constexpr
    // other member functions are all C++20 constexpr
    SWING_CONSTEXPR __normal_iterator() SWING_NOEXCEPT
    : _M_current(_Iterator()) { }

    // disable implicit copy from raw iterator
    explicit SWING_CXX20_CONSTEXPR
    __normal_iterator(const _Iterator& __i) SWING_NOEXCEPT
    : _M_current(__i) { }

    // template copy constructor
    template <typename _Iter, typename = __convertible_from<_Iter>>
    SWING_CXX20_CONSTEXPR
    __normal_iterator(const __normal_iterator<_Iter, _Container>& __i)
    SWING_NOEXCEPT
    : _M_current(__i.base()) { }

    SWING_CXX20_CONSTEXPR
    reference operator*() const SWING_NOEXCEPT
    { return *_M_current; }

    SWING_CXX20_CONSTEXPR
    pointer operator->() const SWING_NOEXCEPT
    { return _M_current; }

    /**
      * @brief index operator of __normal_iterator
      *
      * @param __n index of operator[]
      * @return always return reference of value_type
      *
      * NOTE: __normal_iterator<const _Tp*, _Container> is const_iterator,
      * so whether __normal_iterator<_Tp*, _Container> is const or not
      * operator[] always returns a reference is OK
      *
      * random access is required here
      */
    SWING_CXX20_CONSTEXPR
    reference operator[](difference_type __n) const SWING_NOEXCEPT
    { return _M_current[__n]; }

    SWING_CXX20_CONSTEXPR
    __normal_iterator& operator++() SWING_NOEXCEPT
    {
      ++_M_current;
      return *this;
    }

    SWING_CXX20_CONSTEXPR
    __normal_iterator operator++(int) SWING_NOEXCEPT
    { return __normal_iterator(_M_current++); }

    SWING_CXX20_CONSTEXPR
    __normal_iterator& operator--() SWING_NOEXCEPT
    {
      --_M_current;
      return *this;
    }

    SWING_CXX20_CONSTEXPR
    __normal_iterator operator--(int) SWING_NOEXCEPT
    { return __normal_iterator(_M_current--); }

    SWING_CXX20_CONSTEXPR
    __normal_iterator operator+(difference_type __n) const SWING_NOEXCEPT
    { return __normal_iterator(_M_current + __n); }

    SWING_CXX20_CONSTEXPR
    __normal_iterator& operator+=(difference_type __n) SWING_NOEXCEPT
    {
      _M_current+= __n;
      return *this;
    }

    SWING_CXX20_CONSTEXPR
    __normal_iterator operator-(difference_type __n) const SWING_NOEXCEPT
    { return __normal_iterator(_M_current - __n); }

    SWING_CXX20_CONSTEXPR
    __normal_iterator& operator-=(difference_type __n) SWING_NOEXCEPT
    {
      _M_current -= __n;
      return *this;
    }

    SWING_CXX20_CONSTEXPR
    const _Iterator& base() const SWING_NOEXCEPT
    { return _M_current; }
  };
#if __cplusplus > 201703L
  // TODO: C++20 __normal_iterator three-way comparison
#else
  // NOTE: template _IteratorL and _IteratorR are required
  // for const iterator and iterator like comparison between
  // __normal_iterator<const int*, vector> and __normal_iterator<int* vector>
  // related to gnu libstdc++ DR179.
  template <typename _IteratorL, typename _IteratorR, typename _Container>
  SWING_NODISCARD SWING_CXX20_CONSTEXPR
  inline bool
  operator==(const __normal_iterator<_IteratorL, _Container>& __lhs,
              const __normal_iterator<_IteratorR, _Container>& __rhs)
  SWING_NOEXCEPT { return __lhs.base() == __rhs.base(); }

  template <typename _Iterator, typename _Container>
  SWING_NODISCARD SWING_CXX20_CONSTEXPR
  inline bool
  operator==(const __normal_iterator<_Iterator, _Container>& __lhs,
              const __normal_iterator<_Iterator, _Container>& __rhs)
  SWING_NOEXCEPT { return __lhs.base() == __rhs.base(); }

  template <typename _IteratorL, typename _IteratorR, typename _Container>
  SWING_NODISCARD SWING_CXX20_CONSTEXPR
  inline bool
  operator!=(const __normal_iterator<_IteratorL, _Container>& __lhs,
              const __normal_iterator<_IteratorR, _Container>& __rhs)
  SWING_NOEXCEPT { return __lhs.base() != __rhs.base(); }

  template <typename _Iterator, typename _Container>
  SWING_NODISCARD SWING_CXX20_CONSTEXPR
  inline bool
  operator!=(const __normal_iterator<_Iterator, _Container>& __lhs,
              const __normal_iterator<_Iterator, _Container>& __rhs)
  SWING_NOEXCEPT { return __lhs.base() != __rhs.base(); }

  template <typename _IteratorL, typename _IteratorR, typename _Container>
  SWING_NODISCARD SWING_CXX20_CONSTEXPR
  inline bool
  operator<(const __normal_iterator<_IteratorL, _Container>& __lhs,
            const __normal_iterator<_IteratorR, _Container>& __rhs)
  SWING_NOEXCEPT { return __lhs.base() < __rhs.base(); }

  template <typename _Iterator, typename _Container>
  SWING_NODISCARD SWING_CXX20_CONSTEXPR
  inline bool
  operator<(const __normal_iterator<_Iterator, _Container>& __lhs,
            const __normal_iterator<_Iterator, _Container>& __rhs)
  SWING_NOEXCEPT { return __lhs.base() < __rhs.base(); }

  template <typename _IteratorL, typename _IteratorR, typename _Container>
  SWING_NODISCARD SWING_CXX20_CONSTEXPR
  inline bool
  operator>(const __normal_iterator<_IteratorL, _Container>& __lhs,
            const __normal_iterator<_IteratorR, _Container>& __rhs)
  SWING_NOEXCEPT { return __lhs.base() > __rhs.base(); }

  template <typename _Iterator, typename _Container>
  SWING_NODISCARD SWING_CXX20_CONSTEXPR
  inline bool
  operator>(const __normal_iterator<_Iterator, _Container>& __lhs,
            const __normal_iterator<_Iterator, _Container>& __rhs)
  SWING_NOEXCEPT { return __lhs.base() > __rhs.base(); }

  template <typename _IteratorL, typename _IteratorR, typename _Container>
  SWING_NODISCARD SWING_CXX20_CONSTEXPR
  inline bool
  operator<=(const __normal_iterator<_IteratorL, _Container>& __lhs,
              const __normal_iterator<_IteratorR, _Container>& __rhs)
  SWING_NOEXCEPT { return __lhs.base() <= __rhs.base(); }

  template <typename _Iterator, typename _Container>
  SWING_NODISCARD SWING_CXX20_CONSTEXPR
  inline bool
  operator<=(const __normal_iterator<_Iterator, _Container>& __lhs,
              const __normal_iterator<_Iterator, _Container>& __rhs)
  SWING_NOEXCEPT { return __lhs.base() <= __rhs.base(); }

  template <typename _IteratorL, typename _IteratorR, typename _Container>
  SWING_NODISCARD SWING_CXX20_CONSTEXPR
  inline bool
  operator>=(const __normal_iterator<_IteratorL, _Container>& __lhs,
              const __normal_iterator<_IteratorR, _Container>& __rhs)
  SWING_NOEXCEPT { return __lhs.base() >= __rhs.base(); }

  template <typename _Iterator, typename _Container>
  SWING_NODISCARD SWING_CXX20_CONSTEXPR
  inline bool
  operator>=(const __normal_iterator<_Iterator, _Container>& __lhs,
              const __normal_iterator<_Iterator, _Container>& __rhs)
  SWING_NOEXCEPT { return __lhs.base() >= __rhs.base(); }
#endif

  // NOTE: global operator- between two __normal_iterators
  // like __lhs - __rhs which will make sense,
  // and __lhs + __rhs makes no sense, we don't need the
  // operator+ between two __normal_iterators
  // 
  // DR685. before C++11, the return value is one of the iterator's
  // difference which needs same difference_type in traits
  template <typename _IteratorL, typename _IteratorR, typename _Container>
  SWING_NODISCARD SWING_CXX20_CONSTEXPR
  inline auto
  operator-(const __normal_iterator<_IteratorL, _Container>& __lhs,
            const __normal_iterator<_IteratorR, _Container>& __rhs)
  SWING_NOEXCEPT -> decltype(__lhs.base() - __rhs.base())
  { return __lhs.base() - __rhs.base(); }

  template <typename _Iterator, typename _Container>
  SWING_NODISCARD SWING_CXX20_CONSTEXPR
  inline typename __normal_iterator<_Iterator, _Container>::difference_type
  operator-(const __normal_iterator<_Iterator, _Container>& __lhs,
            const __normal_iterator<_Iterator, _Container>& __rhs)
  SWING_NOEXCEPT { return __lhs.base() - __rhs.base(); }

  // operator+ for usage like 1 + SomeIterator
  template <typename _Iterator, typename _Container>
  SWING_NODISCARD SWING_CXX20_CONSTEXPR
  inline __normal_iterator<_Iterator, _Container>
  operator+(typename __normal_iterator<_Iterator, _Container>::difference_type __n,
            const __normal_iterator<_Iterator, _Container>& __i)
  SWING_NOEXCEPT
  { return __normal_iterator<_Iterator, _Container>(__i.base() + __n); }
  
  // TODO: other functions related to __normal_iterator

} // namespace cxx

#endif // SWING_PRIVATE_ITERATOR_HPP
