#ifndef _CONTAINER_TRAITS_HH_
#define _CONTAINER_TRAITS_HH_

#pragma once

#include "Value_Type.hh"

#include <deque>
#include <list>
#include <vector>
#include <string>

template <typename Container>
struct Container_Traits
{
  /**
   * A container has the following traits and operations:
   *  Type
   *    @prop value_type
   *
   *  Type
   *    @prop rebind
   *  
   *  add_element
   *    @param Container &
   *    @param Element &
   *    @return void
   *
   *  concat
   *    @param Container &
   *    @param Container &
   *    @return void
   */
};

template <typename Container>
struct Sequential_Container_Traits;

template <template <typename, typename> typename Container, typename Element, typename Allocator>
struct Sequential_Container_Traits <Container <Element, Allocator>> : Value_Type <Container <Element, Allocator>>
{
  static void add_element (Container <Element, Allocator> & container, const Element & element)
  {
    container.push_back (element);
  }

  static void concat (Container <Element, Allocator> & lhs, const Container <Element, Allocator> & rhs)
  {
    lhs.insert (lhs.end (), rhs.begin (), rhs.end ());
  }

  template <typename New_Value_Type>
  using rebind = Container <New_Value_Type, typename Allocator::template rebind <New_Value_Type>::other>;
};

/**
 * Container_Traits for std::deque
 */
template <typename ... Args>
struct Container_Traits <std::deque <Args ...>> : Sequential_Container_Traits <std::deque <Args ...>>
{

};

/**
 * Container_Traits for std::list
 */
template <typename ... Args>
struct Container_Traits <std::list <Args ...>> : Sequential_Container_Traits <std::list <Args ...>>
{

};

/**
 * Container_Traits for std::vector
 */
template <typename ... Args>
struct Container_Traits <std::vector <Args ...>> : Sequential_Container_Traits <std::vector <Args ...>>
{

};

template <typename String>
struct String_Container_Traits;

template <template <typename, typename, typename> typename String, typename CharT, template <typename> typename Traits, typename Allocator>
struct String_Container_Traits <String <CharT, Traits <CharT>, Allocator>> : Value_Type <String <CharT, Traits <CharT>, Allocator>>
{
  static void add_element (String <CharT, Traits <CharT>, Allocator> & str, const CharT & c)
  {
    str.push_back (c);
  }

  static void concat (String <CharT, Traits <CharT>, Allocator> & lhs, const String <CharT, Traits <CharT>, Allocator> & rhs)
  {
    lhs += rhs;
  }

  template <typename New_Value_Type>
  using rebind = String <New_Value_Type, Traits <New_Value_Type>, typename Allocator::template rebind <New_Value_Type>::other>;
};

/**
 * Container_Traits for std::basic_string
 */
template <typename ... Args>
struct Container_Traits <std::basic_string <Args ...>> : String_Container_Traits <std::basic_string <Args ...>>
{

};

/**
 * Convienence functions
 */
template <typename ContainerT, typename New_Value_Type>
using rebind_t = typename Container_Traits <ContainerT>::template rebind <New_Value_Type>;

template <typename ContainerT>
using value_t = typename Container_Traits <ContainerT>::value_type;

#endif
