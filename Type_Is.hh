#ifndef _TYPE_IS_HH_
#define _TYPE_IS_HH_

template<typename T>
struct type_is
{
  using type = T;
};

template<typename ...Ts>
using type_is_t = typename type_is<Ts...>::type;

#endif
