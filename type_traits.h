#ifndef META_TYPE_TRAITS_H
#define META_TYPE_TRAITS_H

namespace META
{
  // Generic identity type function
  template<typename T>
    struct type_is
    {
      using type = T;
    };

  // Primary template for remove_const
  template<typename T>
    struct remove_const : type_is<T>
    {
    };

  // Partial specialization for matching const types
  template<typename T>
    struct remove_const<T const> : type_is<T>
    {
    };

  // Primary template for remove_volatile
  template<typename T>
    struct remove_volatile<T> : type_is<T>
    {
    };

  // Partial specialization for matching volatile types
  template<typename T>
    struct remove_volatile<T volatile> : type_is<T>
    {
    };

  // Primary template for IF
  //
  // IF<true, T, F>::type t
  // t is of type T
  template<bool, typename T, typename>
    struct IF : type_is<T>
    {
    };

  // Partial specalization for matching on false
  //
  // IF<false, T, F>::type t
  // t is of type F
  template<typename T, typename F>
    struct IF<false, T, F> : type_is<F>
    {
    };
}
#endif
