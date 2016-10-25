#ifndef META_TYPE_FUNC_H
#define META_TYPE_FUNC_H

#include<type_traits> // std::true_type, std::false_type

namespace meta { namespace type_func {

  template<typename T>
    struct type_is
    {
      using type = T;
    };

  template<template<typename> typename Tt, typename T>
    struct type_is<Tt<T>>
    {
      using type = Tt<T>;
    };

  template<template<typename ...> typename Tt, typename T, typename ...Ts>
    struct type_is<Tt<T, Ts...>>
    {
      using type = Tt<T, Ts...>;
    };

// ---------------------------------------------------------------------------

  namespace detail
  {
    template<typename ...Ts> struct tseq {};
    
    template<typename T, typename ...Ts>
      struct prepend_tseq;

    template<typename T, typename ...Ts>
      struct prepend_tseq<T, tseq<Ts...>> : type_is<tseq<T, Ts...>> {};
  }

// ---------------------------------------------------------------------------

  // Get the type at a given index in a given parameter pack

  namespace detail
  {
    // Primary template for type_at
    template<size_t N, typename T, typename ...Ts>
      struct type_at : type_at<N-1, Ts...> {};

    // Partial specialization matching on N = 0
    template<typename T, typename ...Ts>
      struct type_at<0, T, Ts...> : type_is<T> {};
  }

  // Grab the first parameter in a parameter pack
  template<typename ...Ts>
    using first_t = typename detail::type_at<0, Ts...>::type;

  // Grab the last parameter in a parameter pack
  template<typename ...Ts>
    using last_t = typename detail::type_at<(sizeof...(Ts)) - 1, Ts...>::type;

// ---------------------------------------------------------------------------

  // Determine if a given type exists within a given parameter pack

  // Primary template for is_one_of
  template<typename T, typename ...Ts>
    struct is_one_of;

  // Partial specialization matching on an empty parameter pack
  template<typename T>
    struct is_one_of<T> : ::std::false_type {};

  // Partial specialization matching on T being the first element in the pack
  template<typename T, typename ...Ts>
    struct is_one_of<T, T, Ts...> : ::std::true_type {};

  // Partial specialization matching on T not being the head of the pack
  template<typename T, typename T0, typename ...Ts>
    struct is_one_of<T, T0, Ts...> : is_one_of<T, Ts...> {};

// ---------------------------------------------------------------------------

  namespace detail
  {
    template<template<typename> typename F, typename ...Ts>
      struct map_ts;

    template<template<typename> typename F>
      struct map_ts<F> : type_is<tseq<>> {};

    template<template<typename> typename F, typename T, typename ...Ts>
      struct map_ts<F, T, Ts...> : prepend_tseq<typename F<T>::type, typename map_ts<F, Ts...>::type> {};

    template<template<typename> typename F, typename ...Ts>
      struct map_ts<F, tseq<Ts...>> : map_ts<F, Ts...> {};
  }

  template<template<typename> typename F, typename ...Ts>
    using map_t = typename detail::map_ts<F,Ts...>::type;

// ---------------------------------------------------------------------------

  // Access the left inner-most type parameter of a given template
 
  namespace detail
  {
    template<typename T>
      struct inner_type : type_is<T> {};

    template<template<typename> typename Tt, typename T>
      struct inner_type<Tt<T>> : type_is<T> {};

    template<template<typename ...> typename Tt, typename T, typename ...Ts>
      struct inner_type<Tt<T, Ts...>> : type_is<T> {};
  }

  template<typename ...Ts>
    using inner_t = typename detail::inner_type<Ts...>::type;

// ---------------------------------------------------------------------------

  // Re-Parameterize the 'inner_type' (see above ^) with a new type

  namespace detail
  {
    // using meta::type_func::rebind;
    // rebind<int, std::string>::type t;
    // t would be of type int
    template<typename T, typename>
      struct rebind : type_is<T> {};

    // using meta::type_func::rebind; 
    // rebind<std::list<std::string>, int>::type t;
    // t would be of type std::list<int>
    template<template<typename> typename Tt, typename T, typename U>
      struct rebind<Tt<T>, U> : type_is<Tt<U>> {};

    // using meta::type_func::rebind; 
    // rebind<std::pair<int, int>, std::string>::type t;
    // t would be of type std::pair<std::string, int>
    template<template<typename ...> typename Tt, typename T, typename U, typename ...Ts>
      struct rebind<Tt<T, Ts...>, U> : type_is<Tt<U, Ts...>> {};
  }

  // using meta::type_func::rebind_t;
  // rebind_t<std::pair<int, int>, std::string> t;
  template<typename ...Ts>
    using rebind_t = typename detail::rebind<Ts...>::type;

// ---------------------------------------------------------------------------

  template<typename ...Ts>
    using void_t = void;

  // Primary template for has_type_member, SFINAE defaults to this one
  // in the event of the specialization case failing and being removed
  // from the overload set
  template<typename T, typename = void>
    struct has_type_member : ::std::false_type {};

  // Partial specialization on has_type_member matching on a successful
  // substitution of void_t<typename T::type> resolving to void
  template<typename T>
    struct has_type_member<T, void_t<typename T::type>> : ::std::true_type {};

// ---------------------------------------------------------------------------

  // Primary template for IF
  //
  // IF<true, T, F>::type t
  // t is of type T
  template<bool, typename T, typename>
    struct IF : type_is<T> {};

  // Partial specalization for matching on false
  //
  // IF<false, T, F>::type t
  // t is of type F
  template<typename T, typename F>
    struct IF<false, T, F> : type_is<F> {};

// ---------------------------------------------------------------------------

  // Get the index of a given type within a given parameter pack

  namespace detail
  {
    // Primary template for index_of
    template<size_t N, typename T, typename ...Ts>
      struct index_of;

    // Partial specialization matching on T not being in the pack at all, this
    // results in a SFINAE compatible substitution failure
    template<size_t N, typename T>
      struct index_of<N, T> {};

    // Partial specialization matching on T being the Nth element in the pack
    template<size_t N, typename T, typename ...Ts>
      struct index_of<N, T, T, Ts...>
      {
        static constexpr size_t value = N;
      };

    // Partial specialization matching on T not being the head of the pack
    template<size_t N, typename T, typename T0, typename ...Ts>
      struct index_of<N, T, T0, Ts...> : index_of<N + 1, T, Ts...> {};
  }

  // Convenience wrapper around index_of
  template<typename T, typename ...Ts>
    struct index_of : detail::index_of<0, T, Ts...> {}; 
}}

#endif
