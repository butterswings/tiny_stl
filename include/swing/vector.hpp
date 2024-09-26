#ifndef SWING_VECTOR_HPP
#define SWING_VECTOR_HPP 1

#include <initializer_list>
#include <new>
#include <swing/iterator.hpp>
#include <swing/private/cxx_config.h>
#include <swing/type_traits.hpp>
#include <swing/utility.hpp>

namespace swing
{

  template <typename _Tp>
  class vector
  {
    static_assert(is_same<typename remove_cv<_Tp>::type, _Tp>::value);

  public:
    using value_type             = _Tp;
    using reference              = _Tp&;
    using const_reference        = const _Tp&;
    using pointer                = _Tp*;
    using const_pointer          = const _Tp*;
    using iterator               = cxx::__normal_iterator<pointer, vector>;
    using const_iterator         = cxx::__normal_iterator<const_pointer, vector>;
    using reverse_iterator       = swing::reverse_iterator<iterator>;
    using const_reverse_iterator = swing::reverse_iterator<const_iterator>;
    using size_type              = std::size_t;

  public:
    vector() : _M_start(), _M_finish(), _M_end_of_storage() { }

    vector(std::initializer_list<_Tp> __il) : vector()
    {
      for (const auto& __v : __il)
        push_back(__v);
    }

    ~vector()
    {
      if (this->_M_start)
      {
        for (auto __p = this->_M_start; __p != this->_M_finish; ++__p)
          __p->~_Tp();

        ::operator delete(_M_start);
      }
    }

    void push_back(const _Tp& __v)
    { this->emplace_back(__v); }

    void push_back(_Tp&& __v)
    { this->emplace_back(swing::move(__v)); }

    template <typename... _Args, typename = _Require<swing::is_constructible<_Tp, _Args...>>>
    reference emplace_back(_Args&&... __args)
    {
      // out of storage or initialized defaultly
      if (this->_M_finish == this->_M_end_of_storage)
      {

        size_type __new_capacity = this->capacity() ? this->capacity() * 2 : 1;
        pointer __old_start = this->_M_start;
        pointer __old_finish = this->_M_finish;
        try
        {
          this->_M_start = static_cast<_Tp*>(::operator new(__new_capacity * sizeof(_Tp)));
          this->_M_finish = this->_M_start;
          this->_M_end_of_storage = this->_M_start + __new_capacity;

          for (auto __p = __old_start; __p != __old_finish; ++__p)
          {
            ::new ((void*)this->_M_finish) _Tp(swing::move(*__p));
            ++this->_M_finish;
            __p->~_Tp();
          }
          if (__new_capacity > 1)
            ::operator delete(__old_start);
        }
        catch(...)
        {
          // is throw an exception, the new memory should be correctly addressed
          for (auto __p = this->_M_start; __p != this->_M_finish; ++__p)
            __p->~_Tp();
          ::operator delete(this->_M_start);
          this->_M_start = __old_start;
          this->_M_finish = __old_finish;
          this->_M_end_of_storage = __old_start + __new_capacity / 2;

        }
      }

      ::new ((void*)this->_M_finish) _Tp(swing::forward<_Args>(__args)...);
      ++this->_M_finish;
      return back();
    }

    void pop_back()
    {
      SWING_ASSERT(!this->empty());
      --this->_M_finish;
      this->_M_finish->~_Tp();
    }

    void clear()
    {
      for (auto __p = this->_M_start; __p != this->_M_finish; ++__p)
        __p->~_Tp();
      this->_M_finish = this->_M_start;
    }


    bool empty() const noexcept
    { return begin() == end(); }

    size_type capacity()
    { return size_type(this->_M_end_of_storage - this->_M_start); }

    size_type size() const noexcept
    { return size_type(this->_M_finish - this->_M_start); }

    reference operator[](size_type __n)
    {
      SWING_ASSERT(__n < this->size());
      return *(this->_M_start + __n);
    }

    const_reference operator[](size_type __n) const
    {
      SWING_ASSERT(__n < this->size());
      return *(this->_M_start + __n);
    }

    reference back()
    {
      SWING_ASSERT(!this->empty());
      return *(end() - 1);
    }

    const_reference back() const
    {
      SWING_ASSERT(!this->empty());
      return *(end() - 1);
    }

    iterator begin() noexcept
    { return iterator(this->_M_start); }

    const_iterator begin() const noexcept
    { return const_iterator(this->_M_start); }

    const_iterator cbegin() const noexcept
    { return const_iterator(this->_M_start); }

    iterator end() noexcept
    { return iterator(this->_M_finish); }

    const_iterator end() const noexcept
    { return const_iterator(this->_M_finish); }

    const_iterator cend() const noexcept
    { return const_iterator(this->_M_finish); }

    reverse_iterator rbegin() noexcept
    { return reverse_iterator(end()); }

    const_reverse_iterator rbegin() const noexcept
    { return const_reverse_iterator(end()); }

    const_reverse_iterator crbegin() const noexcept
    { return const_reverse_iterator(end()); }

    reverse_iterator rend() noexcept
    { return reverse_iterator(begin()); }

    const_reverse_iterator rend() const noexcept
    { return const_reverse_iterator(begin()); }

    const_reverse_iterator crend() const noexcept
    { return const_reverse_iterator(begin()); }

  private:
    pointer _M_start;
    pointer _M_finish;
    pointer _M_end_of_storage;
  };

}

#endif // SWING_VECTOR_HPP
