#ifndef SWING_EXPERIMETAL_TUPLE_HPP
#define SWING_EXPERIMETAL_TUPLE_HPP

#include <cstddef>
#include <swing/type_traits.hpp>
#include <swing/utility.hpp>

namespace swing
{
  template <std::size_t _Index, typename _Tp>
  class _Head_base
  {
  public:
    _Head_base(const _Tp& __v) : _M_head_impl(__v) { };
    _Head_base(_Tp&& __v) : _M_head_impl(swing::move(__v)) { }
    _Tp& _M_get() { return _M_head_impl; }

  private:
    _Tp _M_head_impl;
  };

  template <std::size_t _Index, typename ..._Types>
  class _Tuple_impl { };

  // partial specialization to avoid empty parameter pack
  template <std::size_t _Index, typename _Tp, typename ..._Types>
  class _Tuple_impl<_Index, _Tp, _Types...>
  : public _Head_base<_Index, _Tp>,
    public _Tuple_impl<_Index + 1, _Types...>
  {
  public:
    template <typename _Up, typename ..._Elements>
    _Tuple_impl(_Up&& __v, _Elements&& ... __args)
    : _Head_base<_Index, _Tp>(swing::forward<_Tp>(__v)),
      _Tuple_impl<_Index + 1, _Types...>(swing::forward<_Elements>(__args)...) { }
  };

  template <typename... _Types>
  class tuple : public _Tuple_impl<0, _Types...>
  {
  public:
    template <typename ..._Elements>
    tuple(_Elements&& ...__args)
    : _Tuple_impl<0, _Types...>(swing::forward<_Elements>(__args)...) { }
  };

  template <std::size_t _Index, typename... _Elements>
  auto& get(tuple<_Elements...>& __t)
  {
    using _Ret = typename swing::detail::_Nth_type<_Index, _Elements...>::type;
    return static_cast<_Head_base<_Index, _Ret>&>(__t)._M_get();
  }

} // namespace swing

#endif // SWING_EXPERIMETAL_TUPLE_HPP
