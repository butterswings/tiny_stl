#ifndef SWING_PRIVATE_TUPLE_HPP
#define SWING_PRIVATE_TUPLE_HPP

#include <swing/type_traits.hpp>
#include <swing/private/move.hpp>

namespace swing
{
  template <typename ...>
  class tuple;

  template <typename ..._Elements>
  constexpr swing::tuple<typename __decay_and_strip<_Elements>::type...>
  make_tuple(_Elements&& ...__args)
  {
    return
      swing::tuple<typename __decay_and_strip<_Elements>::type...>
        (swing::forward<_Elements>(__args)...);
  }

  // type_list operations
  namespace type_list
  {
    template <typename _Tp, typename ..._Types>
    struct flat_first;

    template <template <typename ...> class _TypeList, typename ..._Types>
    struct flat_first<_TypeList<_Types...>>
    { using type = typename first<_Types...>::type; };

    template <typename _Tp, typename ..._Types>
    struct flat_last;

    template <template <typename ...> class _TypeList, typename ..._Types>
    struct flat_last<_TypeList<_Types...>>
    { using type = typename last<_Types...>::type; };

    template <typename ..._Types>
    using flat_first_t = typename flat_first<_Types...>::type;

    template <typename ..._Types>
    using flat_last_t = typename flat_last<_Types...>::type;

    template <typename _Tp, typename _Back>
    struct push_back;

    template <typename ..._Types, typename _Back>
    struct push_back<swing::tuple<_Types...>, _Back>
    { using type = swing::tuple<_Types..., _Back>; };

    template <typename _Tp, typename _Front>
    struct push_front;

    template <typename ..._Types, typename _Front>
    struct push_front<swing::tuple<_Types...>, _Front>
    { using type = swing::tuple<_Front, _Types...>;};

    template <typename _Tp>
    struct pop_front;

    template <typename _Front, typename ..._Types>
    struct pop_front<swing::tuple<_Front, _Types...>>
    { using type = swing::tuple<_Types...>; };

    template <typename _Tp>
    struct pop_back;

    template <typename _Tp, typename _Reversed = swing::tuple<>>
    struct reverse;

    template <typename _Front, typename ..._TElements, typename _Reversed>
    struct reverse<swing::tuple<_Front, _TElements...>, _Reversed>
    {
      using type =
        typename reverse<swing::tuple<_TElements...>,
          typename push_front<_Reversed, _Front>::type>::type;
    };

    template <typename _Reversed>
    struct reverse<swing::tuple<>, _Reversed>
    { using type = _Reversed; };

    template <typename _Tp>
    struct pop_back
    {
      using type =
        typename reverse<
          typename pop_front<
            typename reverse<_Tp>::type>::type>::type;
    };
  }

  namespace __tuple
  {
    template <typename ..._Elements, typename _Back>
    auto push_back(const swing::tuple<_Elements...>& __t, _Back&& __v)
      -> swing::tuple<_Elements..., typename __decay_and_strip<_Back>::type>;

    template <typename ..._Elements, typename _Front>
    auto push_front(const swing::tuple<_Elements...>& __t, _Front&& __v)
      -> swing::tuple<typename __decay_and_strip<_Front>::type, _Elements...>;

    template <typename _Front, typename ..._Elements>
    auto pop_front(const swing::tuple<_Front, _Elements...>& __t)
      -> swing::tuple<_Elements...>;

    template <typename ..._Elements>
    auto pop_back(const swing::tuple<_Elements...>& __t)
      -> typename swing::type_list::pop_back<swing::tuple<_Elements...>>::type;

    template <typename ..._Elements>
    auto reverse(const swing::tuple<_Elements...>& __t)
      -> typename swing::type_list::reverse<swing::tuple<_Elements...>>::type;
  }
}

#endif // SWING_PRIVATE_TUPLE_HPP
