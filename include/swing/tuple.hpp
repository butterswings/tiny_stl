#ifndef SWING_TUPLE_HPP
#define SWING_TUPLE_HPP 1

#include <cstddef>

#include <swing/type_traits.hpp>
#include <swing/utility.hpp>

namespace swing
{
  template <typename ...>
  class tuple;

  template <>
  class tuple<> { };

  template <typename _Head, typename ..._Tail>
  class tuple<_Head, _Tail...>
  {
    private:
      _Head _head;
      tuple<_Tail...> _tail;

    public:

      tuple() { }
      tuple(const _Head& head, const tuple<_Tail...>& tail)
        : _head(head), _tail(tail) { }

      tuple(const _Head& head, const _Tail& ...tail)
        : _head(head), _tail(tail...) { }

      tuple(_Head&& head, _Tail&& ...tail)
        : _head(move(head)), _tail(move(tail)...) { }

      tuple(const tuple<_Head,_Tail...>& __t)
        : tuple(__t.get_head(), __t.get_tail()) { }

      template <typename _UHead, typename ..._UTail,
                typename = enable_if_t<sizeof...(_UTail) == sizeof...(_Tail)>>
      tuple(const tuple<_UHead, _UTail...>& __t)
        : tuple(__t.get_head(), __t.get_tail()) { }

      template <typename _UHead, typename ..._UTail, 
                typename = enable_if_t<sizeof...(_UTail) == sizeof...(_Tail)>>
      tuple(_UHead&& _uhead, _UTail&& ..._utail)
        : _head(forward<_UHead>(_uhead)), _tail(forward<_UTail>(_utail)...) { }

      _Head& get_head() { return _head; }
      const _Head& get_head() const 
      { return _head; }

      tuple<_Tail...>& get_tail() { return _tail; }
      const tuple<_Tail...>& get_tail() const
      { return _tail; }

  };

  namespace detail
  {
    template <std::size_t __index>
    struct __get_helper
    {
      template <typename _Head, typename ..._Tail>
      static constexpr auto __get(const tuple<_Head, _Tail...>& __t)
      { return __get_helper<__index - 1>::template __get(__t.get_tail()); }
    };

    template <>
    struct __get_helper<0>
    {
      template <typename _Head, typename ..._Tail>
      static constexpr const _Head& __get(const tuple<_Head, _Tail...>& __t)
      { return __t.get_head(); }

    };

  }

  template <std::size_t __index, typename ..._Types>
  constexpr auto get(const tuple<_Types...>& __t)
  {
    static_assert(__index < sizeof...(_Types), "tuple index must be in range");
    return detail::__get_helper<__index>::template __get(__t);
  }

  template <typename ...Args>
  auto make_tuple(Args&& ...args)
  { return swing::tuple<decay_t<Args>...>(forward<Args>(args)...); }

  inline bool operator==(const tuple<>&, const tuple<>&)
  { return true; }

  template <typename ..._TElements, typename ..._UElements,
            typename = enable_if_t<sizeof...(_TElements) == sizeof...(_UElements)>>
  bool operator==(const tuple<_TElements...>& __t, const tuple<_UElements...>& __u)
  {
    return __t.get_head() == __u.get_head() &&
           __t.get_tail() == __u.get_tail();
  }

  template <typename ..._TElements, typename ..._UElements,
            typename = enable_if_t<sizeof...(_TElements) == sizeof...(_UElements)>>
  bool operator!=(const tuple<_TElements...>& __t, const tuple<_UElements...>& __u)
  { return !operator==(__t, __u); };

  template <typename _Tp>
  struct tuple_size;

  template <typename ..._Types>
  struct tuple_size<swing::tuple<_Types...>>
  : integral_constant<std::size_t, sizeof...(_Types)> { };

  template <std::size_t _Idx, typename _Tp>
  struct tuple_element;

  template <std::size_t _Idx, typename ..._Types>
  struct tuple_element<_Idx, swing::tuple<_Types...>>
  {
    static_assert(_Idx < sizeof...(_Types), "tuple index must be in range");
    using type = typename detail::_Nth_type<_Idx, _Types...>::type;
  };

  template <typename ..._UTypes>
  tuple(_UTypes...) -> tuple<_UTypes...>;

} // namespace swing


#endif // SWING_TUPLE_HPP
