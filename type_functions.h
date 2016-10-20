#ifndef META_TYPE_FUNC_H
#define META_TYPE_FUNC_H

#include<type_traits> // std::true_type, std::false_type

namespace meta { namespace type_func {

  template<typename T>
    struct type_is
    {
      using type = T;
    };

  template<template<typename> class Tt, typename T>
    struct type_is<Tt<T>>
    {
      using type = Tt<T>;
    };

  template<template<typename, typename ...> class Tt, typename T, typename ...Ts>
    struct type_is<Tt<T, Ts...>>
    {
      using type = Tt<T, Ts...>;
    };

// ---------------------------------------------------------------------------

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

// ---------------------------------------------------------------------------

  // Primary template for remove_volatile
  template<typename T>
    struct remove_volatile : type_is<T>
    {
    };

  // Partial specialization for matching volatile types
  template<typename T>
    struct remove_volatile<T volatile> : type_is<T>
    {
    };

// ---------------------------------------------------------------------------

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
  
// ---------------------------------------------------------------------------

  // Primary template for is_one_of
  template<typename T, typename ...Ts>
    struct is_one_of;

  // Partial specialization matching on an empty parameter pack
  template<typename T>
    struct is_one_of<T> : ::std::false_type
    {
    };

  // Partial specialization matching on T being the first element in the pack
  template<typename T, typename ...Ts>
    struct is_one_of<T, T, Ts...> : ::std::true_type
    {
    };

  // Partial specialization matching on T not being the head of the pack
  template<typename T, typename T0, typename ...Ts>
    struct is_one_of<T, T0, Ts...> : is_one_of<T, Ts...>
    {
    };

// ---------------------------------------------------------------------------

  namespace detail
  {
    // Primary template for index_of
    template<size_t N, typename T, typename ...Ts>
      struct index_of;

    // Partial specialization matching on T not being in the pack at all, this
    // results in a SFINAE compatible substitution failure
    template<size_t N, typename T>
      struct index_of<N, T>
      {
      };

    // Partial specialization matching on T being the Nth element in the pack
    template<size_t N, typename T, typename ...Ts>
      struct index_of<N, T, T, Ts...>
      {
        static constexpr size_t value = N;
      };

    // Partial specialization matching on T not being the head of the pack
    template<size_t N, typename T, typename T0, typename ...Ts>
      struct index_of<N, T, T0, Ts...> : index_of<N + 1, T, Ts...>
      {
      };
  }

  // Convenience wrapper around index_of
  template<typename T, typename ...Ts>
    struct index_of : detail::index_of<0, T, Ts...>
    {
    }; 

// ---------------------------------------------------------------------------

  // Primary template for type_at
  template<size_t N, typename ...Ts>
    struct type_at;

  // Partial specialization matching on N != 0
  template<size_t N, typename T, typename ...Ts>
    struct type_at<N, T, Ts...> : type_at<N-1, Ts...>
    {
    };

  // Partial specialization matching on N = 0
  template<typename T, typename ...Ts>
    struct type_at<0, T, Ts...> : type_is<T>
    {
    };

// ---------------------------------------------------------------------------

  template<typename ...Ts>
    using head = type_at<0, Ts...>;

  template<typename ...Ts>
    using tail = type_at<(sizeof...(Ts)) - 1, Ts...>;
  
// ---------------------------------------------------------------------------

  template<typename T>
    struct inner_type : type_is<T>
    {
    };

  template<template<typename> class Tt, typename T>
    struct inner_type<Tt<T>> : type_is<T>
    {
    };

  template<template<typename, typename ...> class Tt, typename T, typename ...Ts>
    struct inner_type<Tt<T, Ts...>> : type_is<T>
    {
    };

// ---------------------------------------------------------------------------

  template<typename T, typename>
    struct rebind : type_is<T>
    {
    };

  template<template<typename> class Tt, typename T, typename U>
    struct rebind<Tt<T>, U> : type_is<Tt<U>>
    {
    };

  template<template<typename ...> class Tt, typename T, typename U, typename ...Ts>
    struct rebind<Tt<T, Ts...>, U> : type_is<Tt<U, Ts...>>
    {
    };

// ---------------------------------------------------------------------------

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

}}

#endif

