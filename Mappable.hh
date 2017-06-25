#ifndef _MAPPABLE_H_
#define _MAPPABLE_H_

#pragma once

#include "Container_Traits.hh"
#include "Eval.hh"

#include <type_traits>
#include <utility>

template <typename T>
struct Mappable;

template <typename T>
struct Container_Mappable
{
  using value_type = value_t <T>;

  template <typename F, typename T_A, typename ... Args>
  static auto map (F && f, const T_A & in, const Args & ... args)
  -> rebind_t <T, std::remove_cv_t <decltype (eval (f,
        std::declval <value_t <T_A>> (),
        std::declval <value_t <Args>> () ...))>>
  {
    using T_B = rebind_t <T, std::remove_cv_t <decltype (eval (f,
          std::declval <value_t <T_A>> (),
          std::declval <value_t <Args>> () ...))>>;
    
    T_B out;

    for (auto & x : in)
    {
      Container_Traits <T_B>::add_element (out, eval (std::forward <F> (f), x));
    }

    return out;
  }

  static constexpr bool is_instance = true;
};

template <typename F, typename T>
auto map (F && f, const T & t)
{
  return Mappable <T>::map (std::forward <F> (f), t);
}

/**
 * Mappable for std::deque
 */
template <typename ... Args>
struct Mappable <std::deque <Args ...>> : Container_Mappable <std::deque <Args ...>>
{

};

/**
 * Mappable for std::list
 */
template <typename ... Args>
struct Mappable <std::list <Args ...>> : Container_Mappable <std::list <Args ...>>
{

};

/**
 * Mappable for std::vector
 */
template <typename ... Args>
struct Mappable <std::vector <Args ...>> : Container_Mappable <std::vector <Args ...>>
{

};

/**
 * Mappable for std::string
 */
template <typename ... Args>
struct Mappable <std::basic_string <Args ...>> : Container_Mappable <std::basic_string <Args ...>>
{

};

#endif
