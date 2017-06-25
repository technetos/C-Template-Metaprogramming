#ifndef _VALUE_TYPE_HH_
#define _VALUE_TYPE_HH_

#include "Type_Is.hh"

template <typename T>
struct Value_Type
{
  using value_type = type_is_t <T>;
};

template <template <typename> typename ClassName, typename Arg>
struct Value_Type <ClassName <Arg>>
{
  using value_type = type_is_t <Arg>;
};

template <template <typename ...> typename ClassName, typename Arg, typename ... Args>
struct Value_Type <ClassName <Arg, Args...>> 
{
  using value_type = type_is_t <Arg>;
};

#endif
