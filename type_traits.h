#ifndef META_TYPE_TRAITS_H
#define META_TYPE_TRAITS_H

#include<type_traits>

namespace meta { namespace type_traits {

  template<typename ...Ts>
    using void_t = void;

  // Primary template for has_type_member, SFINAE defaults to this one
  // in the event of the specialization case failing and being removed
  // from the overload set
  template<typename T, typename = void>
    struct has_type_member : ::std::false_type
    {
    };

  // Partial specialization on has_type_member matching on a successful
  // substitution of void_t<typename T::type> resolving to void
  template<typename T>
    struct has_type_member<T, void_t<typename T::type>> : ::std::true_type
    {
    };

// ---------------------------------------------------------------------------

}}

#endif
